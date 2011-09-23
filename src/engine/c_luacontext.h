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
	bool		SetFromJson( const char* json, const char* sName = "" );
	void		SetFromJsonFile( const string& sFilePath, const char* sName = "" );

	int			Get( const char* sVariable, bool bGlobally = true );
	bool		GetAsInt( int& iOutResult, const char* sVariable, int iDefault = 0, bool bGlobally = true );

	// runs a script; returns the number of returned values.
	int			ExecStringsGetResults( const char* sDebugName, const char** pLuaStrings, size_t uiCount );
	int			ExecStringGetResults( const char* sDebugName, const char* sLuaCode );
	int			ExecScriptGetResults( const string& sPath );

	// runs a script, ignoring any returned values.
	bool		ExecString( const char* sDebugName, const char* sLuaCode );
	bool		ExecScript( const string& sPath );

private:
	E_IMPL( CLuaContext );
};

