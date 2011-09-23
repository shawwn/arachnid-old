#include "engine_afx.h"
#include "c_tests.h"

// various headers for the modules we need to test.
#include "include_libjson.h"
#include "include_lua.h"
#include "c_lua.h"
#include "c_luacontext.h"

//***************************************************************************
// Definitions
//***************************************************************************
SINGLETON_INIT( CTests );

//===========================================================================
// Tests - Private State
//===========================================================================
class CTests_impl
{
public:
	~CTests_impl();
	CTests_impl();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Json
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool	TestJson();

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	// Lua
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	bool	TestLua();
	bool	TestLuaContext();
	bool	PrintLuaResults( CLuaContext* pContext, int iNumResults );

};

//---------------------------------------------------------------------------
CTests_impl::~CTests_impl()
{
}

//---------------------------------------------------------------------------
CTests_impl::CTests_impl()
{
}

//---------------------------------------------------------------------------
bool
CTests_impl::TestJson()
{
	printf("\n===============" );
	printf("\n== Test Json #1" );
	printf("\n===============\n" );
	{
		JSONNODE* n(json_new(JSON_NODE));
		json_push_back( n, json_new_a("RootA", "Hello World") );
		JSONNODE* c( json_new(JSON_ARRAY) );
		json_set_name( c, "ArrayOfNumbers" );
		json_push_back( c, json_new_i("", 16) );
		json_push_back( c, json_new_i("", 42) );
		json_push_back( c, json_new_i("", 128) );
		json_push_back( n, c );
		json_char* jc = json_write_formatted( n );
		printf( "%s\n", jc );
		json_delete( n );
	}

	printf("\n===============" );
	printf("\n== Test Json #2" );
	printf("\n===============\n" );

	{
		struct SParseJSON
		{
			static void ParseJSON( JSONNODE* pParent )
			{
				JSONNODE_ITERATOR i = json_begin( pParent );
				while ( i != json_end( pParent ) )
				{
					JSONNODE* pChild( *i );

					// recursively call ourselves to dig deeper into the tree.
					char type = json_type( pChild );
					if ( type == JSON_ARRAY || type == JSON_NODE )
						ParseJSON( pChild );

					json_char* sName = json_name( pChild );

					if ( StringEqual( sName, "RootA" ) )
					{
						printf( "RootA: %s\n",
							json_as_string( pChild ) );
					}
					else if ( StringEqual( sName, "ChildA" ) )
					{
						printf( "ChildA: %s\n",
							json_as_string( pChild ) );
					}
					else if ( StringEqual( sName, "ChildB" ) )
					{
						printf( "ChildB: %d\n",
							json_as_int( pChild ) );
					}

					++i;
				}
			}
		};

		std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
		JSONNODE* n = json_parse( json.c_str() );
		json_char* jc = json_write_formatted( n );
		printf( "%s\n", jc );
		SParseJSON::ParseJSON( n );
		json_delete( n );
	}

	return true;
}

//---------------------------------------------------------------------------
bool
CTests_impl::TestLua()
{
	printf("\n===============" );
	printf("\n== Test Lua" );
	printf("\n===============\n" );

	int status, result, i;
    double sum;
    lua_State *L;

    /*
     * All Lua contexts are held in this structure. We work with it almost
     * all the time.
     */
    L = luaL_newstate();

    luaL_openlibs(L); /* Load Lua libraries */

    /* Load the file containing the script we are going to run */
    status = luaL_loadfile(L, "../scripts/tests/test.lua");
    if (status) {
        /* If something went wrong, error message is at the top of */
        /* the stack */
        fprintf(stderr, "Couldn't load file: %s\n", lua_tostring(L, -1));
		lua_close(L);
		return false;
    }

    /*
     * Ok, now here we go: We pass data to the lua script on the stack.
     * That is, we first have to prepare Lua's virtual stack the way we
     * want the script to receive it, then ask Lua to run it.
     */
    lua_newtable(L);    /* We will pass a table */

    /*
     * To put values into the table, we first push the index, then the
     * value, and then call lua_rawset() with the index of the table in the
     * stack. Let's see why it's -3: In Lua, the value -1 always refers to
     * the top of the stack. When you create the table with lua_newtable(),
     * the table gets pushed into the top of the stack. When you push the
     * index and then the cell value, the stack looks like:
     *
     * <- [stack bottom] -- table, index, value [top]
     *
     * So the -1 will refer to the cell value, thus -3 is used to refer to
     * the table itself. Note that lua_rawset() pops the two last elements
     * of the stack, so that after it has been called, the table is at the
     * top of the stack.
     */
    for (i = 1; i <= 5; i++) {
        lua_pushnumber(L, i);   /* Push the table index */
        lua_pushnumber(L, i*2); /* Push the cell value */
        lua_rawset(L, -3);      /* Stores the pair in the table */
    }

    /* By what name is the script going to reference our table? */
    lua_setglobal(L, "foo");

    /* Ask Lua to run our little script */
    result = lua_pcall(L, 0, LUA_MULTRET, 0);
    if (result) {
        fprintf(stderr, "Failed to run script: %s\n", lua_tostring(L, -1));
		lua_close(L);
		return false;
    }

    /* Get the returned value at the top of the stack (index -1) */
    sum = lua_tonumber(L, -1);

    printf("Script returned: %.0f\n", sum);

    lua_pop(L, 1);  /* Take the returned value out of the stack */
    lua_close(L);   /* Cya, Lua */

	return true;
}

//---------------------------------------------------------------------------
bool
CTests_impl::TestLuaContext()
{
	printf("\n===============" );
	printf("\n== Test LuaContext" );
	printf("\n===============\n" );

	bool bSuccess = true; 
	{
		CLuaContext* pContext = Lua.CreateContext();

		{
			bSuccess &= PrintLuaResults( pContext, pContext->RunScript( "/bin/scripts/tests/hello" ) );
		}

		{
			//std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
			std::string json = "[2, 4, 6, 8, 10]";
			JSONNODE* n = json_parse( json.c_str() );
			pContext->Set( n, "foo" );
			json_delete( n );

			bSuccess &= PrintLuaResults( pContext, pContext->RunScript( "/bin/scripts/test" ) );
		}

		Lua.ReleaseContext( pContext );
	}

	return true;
}

//---------------------------------------------------------------------------
bool
CTests_impl::PrintLuaResults( CLuaContext* pContext, int iNumResults )
{
	if ( iNumResults < 0 )
		return false;

	lua_State* L = pContext->GetState();
	{
		for ( ; ( iNumResults > 0 ); --iNumResults )
		{
			printf( "[%d]: ", iNumResults );

			if ( lua_isnumber( L, -1 ) )
				printf( "%g\n", lua_tonumber( L, -1 ) );
			else if ( lua_isstring( L, -1 ) )
				printf( "\"%s\"\n", lua_tostring( L, -1 ) );
			else if ( lua_isfunction( L, -1 ) )
				printf( "func\n" );
			else if ( lua_istable( L, -1 ) )
				printf( "table\n" );
			else if ( lua_isnil( L, -1 ) )
				printf( "NIL\n" );

			lua_pop( L, 1 );
		}
	}
	return true;
}


//===========================================================================
// Tests
//===========================================================================

//---------------------------------------------------------------------------
CTests::CTests()
: E_IMPL_NEW(CTests)
{
}

//---------------------------------------------------------------------------
CTests::~CTests()
{
	E_IMPL_DEL(CTests);
}

//---------------------------------------------------------------------------
bool
CTests::RunAll()
{
	bool bSuccess = true;
	{
		// run each test.
		bSuccess &= m.TestJson();
		bSuccess &= m.TestLua();
		bSuccess &= m.TestLuaContext();
	}
	return bSuccess;
}
