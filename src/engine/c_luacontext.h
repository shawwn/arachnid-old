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

	// returns the lua state associated with this context.  (Currently used
	// to parse the return values from scripts.)
	lua_State*	GetState();

	void		Set( JSONNODE* pValue, const char* sName = "" );

	// runs a script; returns the number of results.
	int			RunScript( const string& sPath );

private:
	E_IMPL( CLuaContext );
};

