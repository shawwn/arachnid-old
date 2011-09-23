#pragma once

#include "include_lua_decls.h"
#include "include_libjson_decls.h"

//***************************************************************************
// Declarations
//***************************************************************************
class CLuaContext_impl;
class CLuaClosure;
class CLuaClosure_impl;

//===========================================================================
// LuaContext
//===========================================================================
class E_API CLuaContext
{
	friend class CLua;
	CLuaContext( lua_State* L );

	void		ReleaseState( lua_State* pReplacement );

public:
	~CLuaContext();

	// returns the lua state associated with this context.  (Currently used
	// to parse the return values from scripts.)
	lua_State*	GetState();

	void		Set( JSONNODE* pValue, const char* sName = "" );
	void		SetFromJson( const char* json, const char* sName = "" );
	void		SetFromJsonFile( const string& sFilePath, const char* sName = "" );

	// runs a script; returns the number of returned values.
	int			RunScriptGetResults( const string& sPath );

	// runs a script, ignoring any returned values.
	bool		RunScript( const string& sPath );

private:
	E_IMPL( CLuaContext );
};

