#include "engine_afx.h"
#include "c_luacontext.h"

// engine headers.
#include "c_filemanager.h"

// Lua headers.
#include "include_lua.h"

// JSON headers.
#include "include_libjson.h"

//***************************************************************************
// Helper Functions
//***************************************************************************

// loads multiple strings as a single Lua code block.
static int	LuaLoadStrings( lua_State* L, const char** pStrings, size_t uiCount );

//***************************************************************************
// Helper Macros
//***************************************************************************
#define _LUA_BEGIN()

#define LUA_BEGIN()					\
	_LUA_BEGIN();					\
	int			iLuaTop( lua_gettop(L) );	\
	E_UNREF_PARAM( iLuaTop );

#define LUA_TOP(N)					\
	(iLuaTop + (N))

#define LUA_END_N( iNumResults )	\
	/*VERIFY_STACK( iNumResults )*/

#define LUA_END()					\
	LUA_END_N(0)

#define LUA_STATE					\
	(this->L)

#define VERIFY_STACK( iNumResults )	\
	E_ASSERT( lua_gettop(L) == LUA_TOP(iNumResults) );

#define CLEAR_STACK()				\
	lua_settop( L, LUA_TOP(0) );

//===========================================================================
// LuaContext - Private State
//===========================================================================
class CLuaContext_impl
{
public:
	~CLuaContext_impl();
	CLuaContext_impl( lua_State* L );

	void			ReleaseState( lua_State* pReplacement );

	void			ConvertJsonToLuaTable( const char* sDebugName, JSONNODE* pNode );

	void			Get( char* sVariable );

	CLuaContext*	pContext;
	lua_State*		L;
};

//---------------------------------------------------------------------------
CLuaContext_impl::~CLuaContext_impl()
{
	ReleaseState( NULL );
}

//---------------------------------------------------------------------------
CLuaContext_impl::CLuaContext_impl( lua_State* L )
: pContext( NULL )
, L( L )
{
}

//---------------------------------------------------------------------------
void
CLuaContext_impl::ReleaseState( lua_State* pReplacement )
{
	if ( L != NULL )
		lua_close( L );

	L = pReplacement;
}

//---------------------------------------------------------------------------
void
CLuaContext_impl::ConvertJsonToLuaTable( const char* sDebugName, JSONNODE* pParent )
{
	char eParentType = json_type( pParent );

	LUA_BEGIN();
	{
		E_ASSERT( lua_istable( L, -1 ) );

		int iLength = (int)lua_objlen( L, -1 );
		JSONNODE_ITERATOR iter = json_begin( pParent );
		while ( iter != json_end( pParent ) )
		{
			JSONNODE* pChild( *iter );

			json_char* sName = json_name( pChild );
			bool bHasName( !IsEmpty(sName) );

			char eChildType = json_type( pChild );
			switch ( eChildType )
			{
			case JSON_NULL:
			case JSON_STRING:
			case JSON_NUMBER:
			case JSON_BOOL:
				{
					if ( bHasName )
						lua_pushstring( L, sName );

					if ( eChildType == JSON_NULL )
						lua_pushnil( L );
					else if ( eChildType == JSON_STRING )
					{
						json_char* sValue = json_as_string( pChild );
						if ( IsEmpty( sValue ) )
							sValue = "";

						// if the string begins with an equals sign, then evaluate the rest
						// as literal Lua code.  E.g.:
						//
						//		"= (Screen.Width / Screen.Height)"
						//
						// ... evaluates to, say, (1024 / 768) == 1.3333...
						if ( sValue[0] == '=' )
						{
							// skip all whitespace leading up to the source code.
							const char* sCode = &sValue[1];
							while ( *sCode && IsWhitespace( sCode[0] ) )
								++sCode;

							// execute the string without the leading equals sign, and with
							// a leading "return" statement.  E.g:
							//
							//	"return (Screen.Width / Screen.Height)"
							//
							const char* pSourceCode[2] = { "return ", sCode };
							int iNumResults = pContext->ExecStringsGetResults( sDebugName, pSourceCode, 2 );

							if ( iNumResults <= 0 )
							{
								lua_pushnil( L );
							}
							else
							{
								// we only care about the first return value, so get rid of the rest.
								if ( iNumResults > 1 )
									lua_pop( L, (iNumResults - 1) );
							}
						}
						else
						{
							lua_pushstring( L, sValue );
						}
					}
					else if ( eChildType == JSON_NUMBER )
						lua_pushnumber( L, (lua_Number)json_as_float(pChild) );
					else if ( eChildType == JSON_BOOL )
						lua_pushboolean( L, json_as_bool(pChild) );

					if ( bHasName )
						lua_rawset( L, -3 );
					else
						lua_rawseti( L, -2, ++iLength );
				}
				break;

			case JSON_ARRAY:
			case JSON_NODE:
				// recursively call ourselves to dig deeper into the tree.
				{
					if ( bHasName )
						lua_pushstring( L, sName );

					lua_newtable( L );
					ConvertJsonToLuaTable( sDebugName, pChild );

					// if my parent is an array, and we have a 'Name' field,
					// then set "parent[name] = child" (so we can lookup
					// the child by its name, rather than purely by array
					// index).
					//
					if ( eParentType == JSON_ARRAY )
					{
						lua_pushliteral( L, "Name" );
						lua_rawget( L, -2 );
						if ( lua_isnil( L, -1 ) || !lua_isstring( L, -1 ) )
						{
							// our 'name' field wasn't set, or it wasn't a string.
							lua_pop( L, 1 );
						}
						else
						{
							// parent[name] = child;
							{
								// (note, at this point, the child 'name' is on the stack,
								// and it's the key, so that's why we don't push any
								// "key" param here --- it's already on the stack!)

								// push the 'child' table onto the stack.
								lua_pushvalue( L, -2 );

								// perform the action:  parent[name] = child
								lua_rawset( L, LUA_TOP(0) );
							}
						}
					}

					if ( bHasName )
						lua_rawset( L, -3 );
					else
						lua_rawseti( L, -2, ++iLength );
				}
				break;
			}

			++iter;
		}
	}
	LUA_END();
}

//---------------------------------------------------------------------------
void
CLuaContext_impl::Get( char* sVariable )
{
	bool bIsTable = lua_istable( L, -1 );
	E_ASSERT( bIsTable );
	if ( !bIsTable )
		return;

	char* sPeriod = strchr(sVariable, '.');
	if ( !sPeriod )
	{
		lua_getfield( L, -1, sVariable );
	}
	else
	{
		sPeriod[0] = '\0';
		lua_getfield( L, -1, sVariable );
		Get( &sPeriod[1] );
	}
}


//===========================================================================
// LuaContext
//===========================================================================
#undef _LUA_BEGIN
#define _LUA_BEGIN()				\
	lua_State*& L = m.L;

#undef LUA_END_N
#define LUA_END_N( iNumResults )	\
	/* verify that Lua is OK. */	\
	VERIFY_STACK( Max(0, iNumResults) )

#undef LUA_STATE
#define LUA_STATE					\
	(m.L)

//---------------------------------------------------------------------------
CLuaContext::CLuaContext( lua_State* L )
: E_IMPL_NEW(CLuaContext, L)
{
	m.pContext = this;
}

//---------------------------------------------------------------------------
void
CLuaContext::ReleaseState( lua_State* pReplacement )
{
	m.ReleaseState( pReplacement );
}

//---------------------------------------------------------------------------
CLuaContext::~CLuaContext()
{
	E_IMPL_DEL(CLuaContext);
}

//---------------------------------------------------------------------------
lua_State*
CLuaContext::GetState()
{
	return m.L;
}

//---------------------------------------------------------------------------
void
CLuaContext::Set( JSONNODE* pValue, const char* sName )
{
	if ( IsEmpty( sName ) )
		sName = json_name( pValue );

	LUA_BEGIN();
	{
		lua_newtable( L );
		m.ConvertJsonToLuaTable( sName, pValue );
		lua_setglobal( L, sName );
	}
	LUA_END();
}

//---------------------------------------------------------------------------
bool
CLuaContext::SetFromJson( const char* json, const char* sName )
{
	bool bSuccess = false;
	LUA_BEGIN();
	{
		if ( IsEmpty( json ) )
		{
			lua_pushnil( L );
			lua_setglobal( L, sName );
			bSuccess = true;
		}
		else
		{
			if ( json_is_valid( json ) )
			{
				JSONNODE* pNode( json_parse( json ) );
				if ( !pNode )
				{
					lua_pushnil( L );
					lua_setglobal( L, sName );
				}
				else
				{
					Set( pNode, sName );
					json_delete( pNode );
					bSuccess = true;
				}
			}
		}
	}
	LUA_END();
	return bSuccess;
}

//---------------------------------------------------------------------------
void
CLuaContext::SetFromJsonFile( const string& sFilePath, const char* sName )
{
	printf( "Loading JSON file '%s'... \n", sFilePath.c_str() );
	if ( !SetFromJson( FileManager.OpenFile( sFilePath ).GetFileMem(), sName ) )
	{
		fprintf(stderr, "Failed to load JSON file '%s' (invalid syntax?)\n", sFilePath.c_str());
	}
}

//---------------------------------------------------------------------------
int
CLuaContext::Get( const char* sVariable, bool bGlobally )
{
	size_t uiVariableNameLen = strlen( sVariable );
	E_ASSERT( uiVariableNameLen < 2047 );
	if ( uiVariableNameLen >= 2047 )
		return -1;

	char* scratch = (char*)alloca( uiVariableNameLen + 1 );
	strcpy_s( scratch, (uiVariableNameLen + 1), sVariable );

	// store the current top.
	int top = lua_gettop( LUA_STATE );

	if ( bGlobally )
	{
		// push the globals table on the stack.
		lua_getglobal( LUA_STATE, "_G" );
	}

	// follow the variable name chain, e.g. "Config.Screen.Width"
	m.Get( scratch );

	// return the number of things on the stack.
	int topnow = lua_gettop( LUA_STATE );
	E_ASSERT( topnow >= top );

	return (topnow - top);
}

//---------------------------------------------------------------------------
bool
CLuaContext::GetAsInt( int& iOutResult, const char* sVariable, int iDefault, bool bGlobally )
{
	iOutResult = iDefault;

	{
		bool bSuccess = false;

		int iPop = Get( sVariable, bGlobally );

		bool bIsNumber = (lua_isnumber( LUA_STATE, -1 ) != 0);
		if ( bIsNumber )
		{
			iOutResult = (int)lua_tonumber( LUA_STATE, -1 );
			bSuccess = true;
		}

		lua_pop( LUA_STATE, iPop );
		return bSuccess;
	}
}

//---------------------------------------------------------------------------
int
CLuaContext::ExecStringsGetResults( const char* sDebugName, const char** pLuaStrings, size_t uiCount )
{
	int iNumReturnValues = -1;
	LUA_BEGIN();
	{
		int status = LuaLoadStrings( L, pLuaStrings, uiCount );
		if ( status ) 
		{
			// if something went wrong, error message is at the top of
			// the stack.
			fprintf( stderr, "Errors in inline code from '%s.lua':\n%s\n", sDebugName, lua_tostring(L, -1) );
			CLEAR_STACK();
		}
		else
		{
			// ask Lua to run our code.
			int result = lua_pcall( L, 0, LUA_MULTRET, 0 );
			if ( result )
			{
				fprintf( stderr, "Failed to run inline code from '%s.lua':\n%s\n", sDebugName, lua_tostring(L, -1) );
				CLEAR_STACK();
			}
			else
			{
				iNumReturnValues = ( lua_gettop(L) - LUA_TOP(0) );
			}
		}
	}
	LUA_END_N( iNumReturnValues );
	return iNumReturnValues;
}

//---------------------------------------------------------------------------
int
CLuaContext::ExecStringGetResults( const char* sDebugName, const char* sLuaCode )
{
	return ExecStringsGetResults( sDebugName, &sLuaCode, 1 );
}

//---------------------------------------------------------------------------
int
CLuaContext::ExecScriptGetResults( const string& sPath )
{
	int iNumReturnValues = -1;
	LUA_BEGIN();
	{
		CFileHandle hScript( FileManager.OpenFile(sPath + ".lua") );
		if ( !hScript.IsOpen() )
		{
			fprintf(stderr, "Couldn't locate script '%s.lua'\n", sPath.c_str());
		}
		else
		{
			int status = luaL_loadstring( L, hScript.GetFileMem() );
			if ( status ) 
			{
				// if something went wrong, error message is at the top of
				// the stack.
				fprintf( stderr, "Errors in script '%s.lua':\n%s\n", sPath.c_str(), lua_tostring(L, -1) );
				CLEAR_STACK();
			}
			else
			{
				// ask Lua to run our code.
				int result = lua_pcall( L, 0, LUA_MULTRET, 0 );
				if ( result )
				{
					fprintf( stderr, "Failed to run script '%s.lua':\n%s\n", sPath.c_str(), lua_tostring(L, -1) );
					CLEAR_STACK();
				}
				else
				{
					iNumReturnValues = ( lua_gettop(L) - LUA_TOP(0) );
				}
			}
		}
	}
	LUA_END_N( iNumReturnValues );
	return iNumReturnValues;
}

//---------------------------------------------------------------------------
bool
CLuaContext::ExecString( const char* sDebugName, const char* sLuaCode )
{
	// run the script.
	int iNumResults = ExecStringGetResults( sDebugName, sLuaCode );
	if ( iNumResults < 0 )
		return false;

	// ignore any return values.
	lua_pop( LUA_STATE, iNumResults );
	return true;
}

//---------------------------------------------------------------------------
bool
CLuaContext::ExecScript( const string& sPath )
{
	// run the script.
	int iNumResults = ExecScriptGetResults( sPath );
	if ( iNumResults < 0 )
		return false;

	// ignore any return values.
	lua_pop( LUA_STATE, iNumResults );
	return true;
}

//***************************************************************************
// Helper Functions - Implementation
//***************************************************************************

//---------------------------------------------------------------------------
static int
LuaLoadStrings( lua_State* L, const char** pStrings, size_t uiCount )
{
	// push the strings onto the stack.
	for ( size_t i = 0; i < uiCount; ++i )
		lua_pushstring( L, pStrings[i] );

	// concatenate them.
	lua_concat( L, (int)uiCount );

	// fetch the result.
	const char* sSourceCode = lua_tostring( L, -1 );
	int iSourceCodeIdx = lua_gettop( L );

	// load it as Lua code.
	int iResult = luaL_loadstring( L, sSourceCode );

	// the stack currently looks like:
	//
	//	[source code]
	//	[loaded block or error message]
	//
	// remove the source code from the stack; we only want the block or error message.
	lua_remove( L, iSourceCodeIdx );

	// return the status code.
	return iResult;
}