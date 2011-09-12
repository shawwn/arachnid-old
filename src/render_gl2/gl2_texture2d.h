#pragma once

#include "render_gl2/gl2_texturebase.h"

//***************************************************************************
// Declarations
//***************************************************************************
class CImage;

//===========================================================================
// Texture2D
//===========================================================================
class GL2Texture2D : public GL2TextureBase
{
	GL2Texture2D();
public:
	~GL2Texture2D();

	static GL2Texture2D*	LoadFromImage( CImage* pImage );

	virtual PxU32			GetGLHandle()	{ return m_uiHandle; }
	virtual void			Bind( PxU32 uiTexUnit );

private:
	PxU32					m_uiHandle;
};
