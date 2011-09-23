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
	bool		Frame();

private:
	// implementation is hidden from the header file, to avoid excessive recompilation.
	E_IMPL( CEngine );
};

#define Engine	(*CEngine::GetInstance())
