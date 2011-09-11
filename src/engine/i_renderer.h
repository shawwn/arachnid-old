#pragma once

//===========================================================================
// Renderer
//===========================================================================
class E_API IRenderer
{
protected:
	virtual ~IRenderer()	{ }
public:
	IRenderer()				{ }

	virtual void			Shutdown() =0;

	virtual const char*		GetName() =0;

	virtual void			RegisterTexture( ResHandle hTex )		{	E_UNREF_PARAM( hTex );	}
	virtual void			UnregisterTexture( ResHandle hTex )		{	E_UNREF_PARAM( hTex );	}
};


//***************************************************************************
// A DLL renderer shall export the following functions:
//***************************************************************************

//---------------------------------------------------------------------------
//		void			R_GlobalStartup();
//		void			R_GlobalShutdown();
//
// A renderer DLL's R_GlobalStartup() function is always the first function to
// be called;  and R_GlobalShutdown() is the last to be called.
//

//---------------------------------------------------------------------------
//		const char*		R_GetEngineVersion();
//
// Responsible for returning ENGINE_VERSION_STRING.  Used to detect any
// mismatched DLLs.
//

//---------------------------------------------------------------------------
//		PxU32			R_GetRendererCount();
//
// Returns the number of renderers being returned by the DLL.
//

//---------------------------------------------------------------------------
//		const char*		R_GetRendererName( PxU32 idx );
//
// Fetches the name of the specified renderer.
//

//---------------------------------------------------------------------------
//		IRenderer*		R_CreateRenderer( const char* sRendererName );
//
// Requests a new instance of the specified renderer.  Passes in the name
// returned by R_GetRenderName().
//

//---------------------------------------------------------------------------
//		void			R_ReleaseRenderer( IRenderer* pRenderer );
//
// Indicates that the application is done using the renderer.
//


//===========================================================================
// SRendererDLL
//===========================================================================
struct SRendererDLL
{
	SRendererDLL()
	{
		GlobalStartup = NULL;
		GlobalShutdown = NULL;
		GetEngineVersion = NULL;
		GetRendererCount = NULL;
		GetRendererName = NULL;
		CreateRenderer = NULL;
		ReleaseRenderer = NULL;

		hDLL = NULL;
	}

	void			(*GlobalStartup)();
	void			(*GlobalShutdown)();
	const char*		(*GetEngineVersion)();
	PxU32			(*GetRendererCount)();
	const char*		(*GetRendererName)( PxU32 idx );
	IRenderer*		(*CreateRenderer)( const char* sRendererName );
	void			(*ReleaseRenderer)( IRenderer* pRenderer );

	void*			hDLL;
};

extern E_API bool	LoadRendererDLL( SRendererDLL& cOutAPI, const string& sRendererDllPath );
extern E_API void	UnloadRendererDLL( SRendererDLL& cOutAPI );

