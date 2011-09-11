#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CEngine_impl;

//===========================================================================
// Engine
//===========================================================================
class E_API CEngine
{
	SINGLETON_DEF( CEngine );
public:
	~CEngine();

	bool		Startup();
	void		Shutdown();

private:
	// implementation is hidden from the header file, to avoid excessive recompilation.
	E_IMPL_DECL( CEngine );
};

#define Engine	(*CEngine::GetInstance())
