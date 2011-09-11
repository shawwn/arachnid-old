
// Windows headers.
#ifdef WIN32
#  include <windows.h>
#  include <shellapi.h>
#endif

// standard C headers.
#include <cstdio>

// engine headers.
#include "engine/engine_afx.h"
#include "engine/c_system.h"
#include "engine/c_engine.h"


//===========================================================================
// Application entry point.
//===========================================================================

//---------------------------------------------------------------------------
int	App_Main( int argc, char** argv )
{
	System.Init( argc, argv );

	if ( !Engine.Startup() )
		return 0;

	printf("hello\n");
	return 0;
}


//===========================================================================
// Platform-specific entry points.
//===========================================================================

#ifdef WIN32
//---------------------------------------------------------------------------
int WINAPI	WinMain( HINSTANCE, HINSTANCE, LPSTR, int )
{
	return App_Main( __argc, __argv );
}
#else
//---------------------------------------------------------------------------
int	main( int argc, char** argv )
{
	return App_Main( argc, argv );
}
#endif
