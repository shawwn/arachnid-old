#include "engine_afx.h"
#include "c_luacontext.h"

// engine headers.
#include "c_filemanager.h"

// Lua headers.
#include "include_lua.h"

// JSON headers.
#include "include_libjson.h"

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

	void		ConvertJsonToLuaTable( JSONNODE* pNode );

	lua_State*	L;
};

//---------------------------------------------------------------------------
CLuaContext_impl::~CLuaContext_impl()
{
	lua_close( L );
}

//---------------------------------------------------------------------------
CLuaContext_impl::CLuaContext_impl( lua_State* L )
: L( L )
{
}

//---------------------------------------------------------------------------
void
CLuaContext_impl::ConvertJsonToLuaTable( JSONNODE* pParent )
{
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

			char type = json_type( pChild );
			switch ( type )
			{
			case JSON_NULL:
			case JSON_STRING:
			case JSON_NUMBER:
			case JSON_BOOL:
				{
					if ( bHasName )
						lua_pushstring( L, sName );

					if ( type == JSON_NULL )
						lua_pushnil( L );
					else if ( type == JSON_STRING )
						lua_pushstring( L, json_as_string(pChild) );
					else if ( type == JSON_NUMBER )
						lua_pushnumber( L, (lua_Number)json_as_float(pChild) );
					else if ( type == JSON_BOOL )
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
					ConvertJsonToLuaTable( pChild );

					if ( type == JSON_NULL )
						lua_pushnil( L );
					else if ( type == JSON_STRING )
						lua_pushstring( L, json_as_string(pChild) );
					else if ( type == JSON_NUMBER )
						lua_pushnumber( L, (lua_Number)json_as_float(pChild) );
					else if ( type == JSON_BOOL )
						lua_pushboolean( L, json_as_bool(pChild) );

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


//===========================================================================
// LuaContext
//===========================================================================
#undef _LUA_BEGIN
#define _LUA_BEGIN()				\
	lua_State*& L = m.L;

#undef LUA_END_N
#define LUA_END_N( iNumResults )	\
	/* verify that Lua is OK. */	\
	VERIFY_STACK( iNumResults )

//---------------------------------------------------------------------------
CLuaContext::CLuaContext( lua_State* L )
: E_IMPL_NEW(CLuaContext, L)
{
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
		m.ConvertJsonToLuaTable( pValue );
		lua_setglobal( L, sName );
	}
	LUA_END();
}

//---------------------------------------------------------------------------
int
CLuaContext::RunScript( const string& sPath )
{
	int iNumReturnValues = -1;
	LUA_BEGIN();
	{
		CFileHandle hScript( FileManager.OpenFile(sPath + ".lua") );
		if ( !hScript.IsOpen() )
		{
			fprintf(stderr, "Couldn't load script: %s\n", sPath.c_str());
		}
		else
		{
			int status = luaL_loadstring( L, hScript.GetFileMem() );
			if ( status ) 
			{
				// if something went wrong, error message is at the top of
				// the stack.
				fprintf( stderr, "Couldn't load file: %s\n", lua_tostring(L, -1) );
				CLEAR_STACK();
			}
			else
			{
				// ask Lua to run our little script.
				int result = lua_pcall( L, 0, LUA_MULTRET, 0 );
				if ( result )
				{
					fprintf( stderr, "Failed to run script: %s\n", lua_tostring(L, -1) );
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
