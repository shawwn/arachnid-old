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

	void		Init( int argc, char** argv );
	void		Exit( int iCode = 0 );
	void		NotifyOnExit( OnExitCallback pCallback, void* pUserdata = NULL );

	void*		LoadLib( const string& sPath );
	void*		FindLib( const string& sPath );
	void		UnloadLib( void* hLib );
	void*		GetProcAddr( void* hLib, const char* sFuncName );

private:
	E_IMPL_DECL( CSystem );
};

#define System	(*CSystem::GetInstance())
