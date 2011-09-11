#include "render_gl2_afx.h"
#include "gl2_main.h"

// engine headers.
#include "engine/engine_version.h"

// project headers.
#include "gl2_renderer.h"

// std C headers.
#include <cstdio>

//***************************************************************************
// Globals
//***************************************************************************
CGL2Renderer*	g_pCurrentGLRenderer;

//***************************************************************************
// Definitions
//***************************************************************************

//---------------------------------------------------------------------------
// A renderer DLL's R_GlobalStartup() function is always the first function to
// be called;  and R_GlobalShutdown() is the last to be called.
//---------------------------------------------------------------------------
extern "C"
R_API void	R_GlobalStartup()
{
	printf("R_GlobalStartup()\n");
}


extern "C"
R_API void	R_GlobalShutdown()
{
	printf("R_GlobalShutdown()\n");
}

//---------------------------------------------------------------------------
// Responsible for returning ENGINE_VERSION_STRING.  Used to detect any
// mismatched DLLs.
//---------------------------------------------------------------------------
extern "C"
R_API const char*	R_GetEngineVersion()
{
	return ENGINE_VERSION_STRING;
}


//---------------------------------------------------------------------------
// Returns the number of renderers being returned by the DLL.
//---------------------------------------------------------------------------
extern "C"
R_API PxU32	R_GetRendererCount()
{
	return 1;
}


//---------------------------------------------------------------------------
// Fetches the name of the specified renderer.
//---------------------------------------------------------------------------
extern "C"
R_API const char*	R_GetRendererName( PxU32 idx )
{
	E_UNREF_PARAM( idx );
	return "gl2";
}


//---------------------------------------------------------------------------
// Requests a new instance of the specified renderer.
//---------------------------------------------------------------------------
extern "C"
R_API IRenderer*	R_CreateRenderer( const char* sName, PxU32 uiResW, PxU32 uiResH )
{
	if ( !StringEqual(sName, "gl2") )
		return NULL;

	// if the renderer has already been created, then fail.  (Only one at a time!)
	if ( g_pCurrentGLRenderer != NULL )
		return NULL;

	// allocate the renderer.
	CGL2Renderer* pRenderer = E_NEW( CGL2Renderer );
	g_pCurrentGLRenderer = pRenderer;

	// initialize the renderer.
	if ( !pRenderer->Startup( uiResW, uiResH ) )
	{
		E_DELETE( pRenderer );
		return NULL;
	}

	return pRenderer;
}


//---------------------------------------------------------------------------
// Indicates that the application is done using the renderer.
//---------------------------------------------------------------------------
extern "C"
R_API void	R_ReleaseRenderer( IRenderer* pRenderer )
{
	E_ASSERT( pRenderer == g_pCurrentGLRenderer );
	if ( !pRenderer )
		return;
	g_pCurrentGLRenderer = NULL;

	bool bIsSafe = StringEqual("gl2", pRenderer->GetName());
	E_ASSERT( bIsSafe );
	if ( bIsSafe )
	{
		CGL2Renderer* pGL2Renderer( (CGL2Renderer*)pRenderer );
		E_DELETE( pGL2Renderer );
	}
}

