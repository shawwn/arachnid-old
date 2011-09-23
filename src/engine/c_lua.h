#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CLua_impl;
class CLuaContext;

//===========================================================================
// Lua
//===========================================================================
class E_API CLua
{
	SINGLETON_DEF( CLua );
public:
	~CLua();

	CLuaContext*	CreateContext();
	void			ResetContext( CLuaContext* pContext );
	void			ReleaseContext( CLuaContext*& pContext );

private:
	E_IMPL( CLua );
};

#define Lua		(*CLua::GetInstance())
