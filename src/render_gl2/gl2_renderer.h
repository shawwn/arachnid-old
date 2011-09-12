#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#include "engine/i_renderer.h"

class CGL2Renderer_impl;

//===========================================================================
// GL2Renderer
//===========================================================================
class CGL2Renderer : public IRenderer
{
public:
	~CGL2Renderer();
	CGL2Renderer();

	const char*				GetName()		{	return "gl2";	}

	bool					Startup( PxU32 uiResolutionW, PxU32 uiResolutionH );
	void					Shutdown();

	void					SetViewpoint( const GrCamera& cCamera );

	void					Frame( PxU32 uiTotalTime, PxF32 fDeltaTime );

private:
	E_IMPL( CGL2Renderer );
};
