#pragma once

#include "include_lua_decls.h"
#include "include_libjson_decls.h"

//***************************************************************************
// Declarations
//***************************************************************************
class CLuaContext_impl;

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

