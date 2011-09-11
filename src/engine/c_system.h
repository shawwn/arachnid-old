#pragma once
#include "common\c_singleton.h"

//***************************************************************************
// Declarations
//***************************************************************************
class CSystem_impl;

typedef void	(*OnExitCallback)( void* pUserdata );

//===========================================================================
// System
//===========================================================================
class E_API CSystem
{
	SINGLETON_DEF( CSystem );
public:
	~CSystem();

	// must be called at the very top of main(), before doing anything else
	// (such as allocating memory etc).
	void		Init( int argc, char** argv );

	// calls each "OnExitCallback", then gracefully exits the application.
	void		Exit( int iCode = 0 );
	void		NotifyOnExit( OnExitCallback pCallback, void* pUserdata = NULL );

	// DLL management functions.
	void*		LoadLib( const string& sPath );
	void*		FindLib( const string& sPath );
	void		UnloadLib( void* hLib );
	void*		GetProcAddr( void* hLib, const char* sFuncName );

	// timing-related functions.
	PxU32		GetCurTime();

private:
	E_IMPL_DECL( CSystem );
};

#define System	(*CSystem::GetInstance())
