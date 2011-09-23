#pragma once

#define RUN_TESTS		1
#define RUN_TESTS_ONLY	0

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
