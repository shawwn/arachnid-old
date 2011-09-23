#include "engine_afx.h"
#include "c_luacontext.h"

// Lua headers.
extern "C"
{
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

//***************************************************************************
// Definitions
//***************************************************************************

//===========================================================================
// LuaContext - Private State
//===========================================================================
class CLuaContext_impl
{
public:
	~CLuaContext_impl();
	CLuaContext_impl( lua_State* L );

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


//===========================================================================
// LuaContext
//===========================================================================

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
