#include "engine_afx.h"
#include "c_engine.h"

#include "engine/c_filemanager.h"
#include "engine/i_renderer.h"

//***************************************************************************
// Declarations
//***************************************************************************
SINGLETON_INIT( CEngine );

typedef map<string, SRendererDLL>	FilepathRendererDLLMap;
typedef map<string, IRenderer*>		RendererByNameMap;

//===========================================================================
// Engine - Private State
//===========================================================================
class CEngine_impl
{
public:
	~CEngine_impl();
	CEngine_impl();

	FilepathRendererDLLMap		mapRendererDLLs;
	RendererByNameMap			mapRenderers;
	bool						bInitialized;
};

//---------------------------------------------------------------------------
CEngine_impl::~CEngine_impl()
{
}

//---------------------------------------------------------------------------
CEngine_impl::CEngine_impl()
: bInitialized( false )
{
}


//===========================================================================
// Engine
//===========================================================================

//---------------------------------------------------------------------------
CEngine::CEngine()
: E_IMPL_NEW(CEngine)
{
}

//---------------------------------------------------------------------------
CEngine::~CEngine()
{
	E_IMPL_DEL(CEngine);
}

//---------------------------------------------------------------------------
bool
CEngine::Startup()
{
	E_ASSERT( !m.bInitialized );
	if ( m.bInitialized )
		return false;

	// determine our possible renderers.
	{
		// first, find each render_*.dll within our exe dir.
		StringList vRendererLibs;
		PxU32 uiCount = FileManager.FindFiles( vRendererLibs, FileManager.GetExeDir(), "render_*.dll" );

		// then, load each each renderer DLL.
		for ( PxU32 i = 0; i < uiCount; ++i )
		{
			const string& sFilename( vRendererLibs[i] );

			// if we're in debug mode, only load debug renderers.  And vice versa.
			{
				bool bFilenameContainsDebug = StringContains( sFilename, "DEBUG_" );
#ifdef _DEBUG
				if ( !bFilenameContainsDebug )
					continue;
#else
				if ( bFilenameContainsDebug )
					continue;
#endif
			}

			// if we're in 64-bit mode, only load 64-bit renderers.  And vice versa.
			{
				bool bFilenameContainsX64 = StringContains( sFilename, "_x64." );
#ifdef E_X64
				if ( !bFilenameContainsX64 )
					continue;
#else
				if ( bFilenameContainsX64 )
					continue;
#endif
			}

			// determine whether there's been a name collision.
			if ( m.mapRendererDLLs.find(sFilename) != m.mapRendererDLLs.end() )
			{
				E_ASSERT( !"Renderer name collision" );
				continue;
			}

			// load the renderer plugin.
			string sFilepath( FileManager.GetExeDir() + sFilename );
			if ( !LoadRendererDLL( m.mapRendererDLLs[sFilename], sFilepath ) )
			{
				E_ASSERT( !"Failed to load renderer" );
				continue;
			}
		}
	}

	// loop over each renderer, requesting the "gl2" renderer.
	for ( FilepathRendererDLLMap::iterator it(m.mapRendererDLLs.begin()), itEnd(m.mapRendererDLLs.end());
		it != itEnd;
		++it )
	{
		const string& sFilepath( it->first );
		SRendererDLL& cRenderer( it->second );
		E_UNREF_PARAM2( sFilepath, cRenderer );

		IRenderer* pRenderer = cRenderer.CreateRenderer( "gl2", 1024, 768 );
		if ( pRenderer != NULL )
		{
			const string& sRendererName( pRenderer->GetName() );
			if ( !m.mapRenderers.insert( make_pair( sRendererName, pRenderer ) ).second )
			{
				E_ASSERT( !"Renderer name collision" );
				cRenderer.ReleaseRenderer( pRenderer );
				pRenderer = NULL;
			}
		}
	}

	m.bInitialized = true;
	return true;
}

//---------------------------------------------------------------------------
void
CEngine::Shutdown()
{
	if ( !m.bInitialized )
		return;
	m.bInitialized = false;

	// unload the renderers.
	while ( !m.mapRenderers.empty() )
	{
		RendererByNameMap::iterator it( m.mapRenderers.begin() );
		const string& sName( it->first );
		IRenderer* pRenderer( it->second );
		E_UNREF_PARAM2( sName, pRenderer );

		E_ASSERT( pRenderer );
		if ( pRenderer )
			pRenderer->Shutdown();

		m.mapRenderers.erase( it );
	}

	// unload the renderer DLLs.
	while ( !m.mapRendererDLLs.empty() )
	{
		FilepathRendererDLLMap::iterator it( m.mapRendererDLLs.begin() );
		const string& sFilepath( it->first );
		SRendererDLL& cRenderer( it->second );
		E_UNREF_PARAM2( sFilepath, cRenderer );

		UnloadRendererDLL( cRenderer );
		m.mapRendererDLLs.erase( it );
	}
}
