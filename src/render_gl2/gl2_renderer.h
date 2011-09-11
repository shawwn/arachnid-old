#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#include "engine/i_renderer.h"

//===========================================================================
// GL2Renderer
//===========================================================================
class CGL2Renderer : public IRenderer
{
public:
	~CGL2Renderer();
	CGL2Renderer();

	virtual void			Shutdown();
	virtual const char*		GetName()		{	return "gl2";	}
};
