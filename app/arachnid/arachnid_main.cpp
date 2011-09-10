
// Windows headers.
#ifdef WIN32
#  include <windows.h>
#  include <shellapi.h>
#endif

// standard C headers.
#include <cstdio>


//===========================================================================
// Application entry point.
//===========================================================================

//---------------------------------------------------------------------------
int	App_Main( int argc, char** argv )
{
	argc = argc;
	argv = argv;
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
