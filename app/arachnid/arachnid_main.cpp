
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

//***************************************************************************
// Declarations
//***************************************************************************
void App_OnExit( void* pUserdata );

//===========================================================================
// Application entry point.
//===========================================================================

//---------------------------------------------------------------------------
int	App_Main( int argc, char** argv )
{
	// initialize the system.
	System.Init( argc, argv );
	System.NotifyOnExit( App_OnExit );

	// initialize the engine.
	if ( !Engine.Startup() )
		return 0;

	// enter into the main application loop.
	while ( Engine.Frame() )
	{
	}

	// graceefully exit the application.
	System.Exit();
	return 0;
}

//---------------------------------------------------------------------------
void	App_OnExit( void* /*pUserdata*/ )
{
	// shutdown the engine.
	Engine.Shutdown();
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
