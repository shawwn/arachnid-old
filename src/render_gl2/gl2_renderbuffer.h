#pragma once

#include "render_gl2/gl2_texturebase.h"

//***************************************************************************
// Declarations
//***************************************************************************

//===========================================================================
// Renderbuffer
//===========================================================================
class GL2Renderbuffer : public GL2TextureBase
{
	GL2Renderbuffer();
public:
	~GL2Renderbuffer();

	static GL2Renderbuffer*		CreateRenderbuffer( // possible glType values:
													//	* GL_RGBA8
													//	* GL_RGBA16F
													//	* GL_DEPTH_COMPONENT24
													//	* GL_STENCIL_INDEX
													GLenum glType,

													// the size of the renderbuffer.
													PxU32 uiWidth, PxU32 uiHeight,

													// the amount of multisampling per pixel:
													//	* 0, 1, 2, 4, 8, or 16.
													PxU32 uiMultisample = 0 );

	static void					Bind( GL2TextureBase* pRenderbuffer );
	static void					Unbind();

	virtual PxU32				GetGLTarget()				{ return GL_RENDERBUFFER;	}
	virtual PxU32				GetGLHandle()				{ return m_hGLHandle;		}
	virtual PxU32				GetMiplevelCount()			{ return 1;					}
	virtual PxU32				GetWidth()					{ return m_uiWidth;			}
	virtual PxU32				GetHeight()					{ return m_uiHeight;		}

private:
	GLuint						m_hGLHandle;
	PxU32						m_uiWidth;
	PxU32						m_uiHeight;
	PxU32						m_uiMultisample;
};