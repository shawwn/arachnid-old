#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CLuaContext_impl;
typedef struct lua_State lua_State;

//===========================================================================
// LuaContext
//===========================================================================
class E_API CLuaContext
{
	friend class CLua;
	CLuaContext( lua_State* L );
public:
	~CLuaContext();

private:
	E_IMPL( CLuaContext );
};

