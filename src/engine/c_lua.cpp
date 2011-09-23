#include "engine_afx.h"
#include "c_lua.h"

// engine headers.
#include "c_luacontext.h"

// Lua headers.
#include "include_lua.h"

//***************************************************************************
// Definitions
//***************************************************************************
SINGLETON_INIT( CLua );

//===========================================================================
// Lua - Private State
//===========================================================================
class CLua_impl
{
public:
	~CLua_impl();
	CLua_impl();

};

//---------------------------------------------------------------------------
CLua_impl::~CLua_impl()
{
}

//---------------------------------------------------------------------------
CLua_impl::CLua_impl()
{
}


//===========================================================================
// Lua
//===========================================================================

//---------------------------------------------------------------------------
CLua::CLua()
: E_IMPL_NEW(CLua)
{
}

//---------------------------------------------------------------------------
CLua::~CLua()
{
	E_IMPL_DEL(CLua);
}

//---------------------------------------------------------------------------
CLuaContext*
CLua::CreateContext()
{
	// create a new Lua state.
	lua_State* L = luaL_newstate();
	if ( !L )
	{
		E_ASSERT( !"Could not create Lua state." );
		return NULL;
	}

	// provide the standard Lua libraries.
	luaopen_base( L );
	luaopen_math( L );
	luaopen_string( L );
	luaopen_table( L );
	luaopen_io( L );
	luaopen_os( L );

	return E_NEW( CLuaContext )( L );
}

//---------------------------------------------------------------------------
void
CLua::ReleaseContext( CLuaContext*& pContext )
{
	E_DELETE( pContext );
}
