#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CTests_impl;

//===========================================================================
// Tests
//===========================================================================
class E_API CTests
{
	SINGLETON_DEF( CTests );
public:
	~CTests();

	bool			RunAll();

private:
	// implementation is hidden from the header file, to avoid excessive recompilation.
	E_IMPL( CTests );
};

#define Tests	(*CTests::GetInstance())
