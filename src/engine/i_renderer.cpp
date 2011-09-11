#include "engine_afx.h"
#include "i_renderer.h"

#include "engine/engine_version.h"
#include "engine/c_system.h"

#include <cstdio>

//===========================================================================
// SRendererDLL
//===========================================================================

//---------------------------------------------------------------------------
template< typename T >
bool	LoadRendererFunc( void* hDLL, T*& pFunction, const char* sFunctionName )
{
	pFunction = (T*)System.GetProcAddr( hDLL, sFunctionName );
	E_ASSERT( pFunction != NULL );
	return (*pFunction != NULL);
}

//---------------------------------------------------------------------------
bool	LoadRendererDLL( SRendererDLL& cOutRenderer, const string& sRendererDllPath )
{
	// zero out our return struct.
	E_ASSERT( cOutRenderer.hDLL == NULL );
	cOutRenderer = SRendererDLL();

	printf( "Loading renderer DLL '%s'...\n", sRendererDllPath.c_str() );

	// try to load the renderer DLL.
	void* hDLL = System.LoadLib( sRendererDllPath );
	if ( !hDLL )
		return false;
	cOutRenderer.hDLL = hDLL;

	// load each API function.  If we fail to load any,
	// then abort.
#define PP_TMP2( tok )				#tok
#define PP_TMP1( prefix, name )		PP_TMP2(prefix##name)
#define LOAD_RENDERER_FUNC( name )		if ( !bFailed && !LoadRendererFunc( hDLL, cOutRenderer.name, PP_TMP1(R_,name) ) ) { bFailed = true; }
	bool bFailed = false;
	{
		LOAD_RENDERER_FUNC( GlobalStartup );
		LOAD_RENDERER_FUNC( GlobalShutdown );
		LOAD_RENDERER_FUNC( GetEngineVersion );
		LOAD_RENDERER_FUNC( GetRendererCount );
		LOAD_RENDERER_FUNC( GetRendererName );
		LOAD_RENDERER_FUNC( CreateRenderer );
		LOAD_RENDERER_FUNC( ReleaseRenderer );
	}
#undef LOAD_RENDERER_FUNC

	if ( !bFailed )
	{
		// invoke renderer global startup.
		cOutRenderer.GlobalStartup();

		// fetch the renderer's engine version string.
		string sPluginEngineVersion( cOutRenderer.GetEngineVersion() );

		// string is in format "x.y.z.w", i.e. "<majorVersion>.<minorVersion>.<microVersion>.<hotfixVersion>"
		// Split it by period.
		StringList vEngineVersion;
		PxU32 uiCount = SplitBy( vEngineVersion, sPluginEngineVersion, "." );
		E_ASSERT( uiCount == 4 );
		if ( uiCount != 4 )
		{
			bFailed = true;
		}
		else
		{
			PxI32 iMajorVersion = atoi(vEngineVersion[0].c_str());
			PxI32 iMinorVersion = atoi(vEngineVersion[1].c_str());
			PxI32 iMicroVersion = atoi(vEngineVersion[2].c_str());
			PxI32 iHotfixVersion = atoi(vEngineVersion[3].c_str());

			// if the renderer was written for a *newer* version of the engine, then abort.
			bool bDone = false;
			if ( !bDone && (iMajorVersion > ENGINE_MAJOR_VERSION) )		bFailed = true;
			if ( !bDone && (iMajorVersion < ENGINE_MAJOR_VERSION) )		bDone = true;
			if ( !bDone && (iMinorVersion > ENGINE_MINOR_VERSION) )		bFailed = true;
			if ( !bDone && (iMinorVersion < ENGINE_MINOR_VERSION) )		bDone = true;
			if ( !bDone && (iMicroVersion > ENGINE_MICRO_VERSION) )		bFailed = true;
			if ( !bDone && (iMicroVersion < ENGINE_MICRO_VERSION) )		bDone = true;
			if ( !bDone && (iHotfixVersion > ENGINE_HOTFIX_VERSION) )	bFailed = true;
			if ( !bDone && (iHotfixVersion < ENGINE_HOTFIX_VERSION) )	bDone = true;
			if ( bFailed )
			{
				E_ASSERT( !"Renderer plugin was written for a newer version of the Arachnid engine" );
			}
		}
	}

	if ( bFailed )
	{
		UnloadRendererDLL( cOutRenderer );
		return false;
	}

	// success!
	return true;
}

//---------------------------------------------------------------------------
extern void
UnloadRendererDLL( SRendererDLL& cRenderer )
{
	// invoke renderer global shutdown.
	if ( cRenderer.GlobalShutdown )
		cRenderer.GlobalShutdown();

	// unload the renderer DLL.
	System.UnloadLib( cRenderer.hDLL );

	// zero our return value.
	cRenderer = SRendererDLL();
}
