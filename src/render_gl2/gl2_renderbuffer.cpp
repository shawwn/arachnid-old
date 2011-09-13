#include "render_gl2_afx.h"
#include "gl2_renderbuffer.h"

//===========================================================================
// Renderbuffer
//===========================================================================

//---------------------------------------------------------------------------
GL2Renderbuffer::GL2Renderbuffer()
: GL2TextureBase( GL2_TEXTYPE_RENDERBUFFER )
, m_hGLHandle( 0 )
, m_uiWidth( 0 )
, m_uiHeight( 0 )
, m_uiMultisample( 0 )
{
}

//---------------------------------------------------------------------------
GL2Renderbuffer::~GL2Renderbuffer()
{
	// delete the renderbuffer.
	glDeleteRenderbuffers( 1, &m_hGLHandle );
}

//---------------------------------------------------------------------------
GL2Renderbuffer*
GL2Renderbuffer::CreateRenderbuffer( // possible glType values:
									//	* GL_RGBA8
									//	* GL_RGBA16F
									//	* GL_DEPTH_COMPONENT24
									//	* GL_STENCIL_INDEX
									GLenum glType,

									// the size of the renderbuffer.
									PxU32 uiWidth, PxU32 uiHeight,

									// the amount of multisampling per pixel:
									//	* 0, 1, 2, 4, 8, or 16.
									PxU32 uiMultisample )
{
	// sanity check the inputs.
	{
		E_ASSERT( uiWidth > 0 );
		E_ASSERT( uiHeight > 0 );
		if ( uiWidth <= 0 || uiHeight <= 0 )
			return NULL;

		switch ( uiMultisample )
		{
		case 0:
		case 1:
		case 2:
		case 4:
		case 8:
		case 16:
			break;
		default:
			E_ASSERT( !"Invalid 'multisample' param for CreateRenderuffer()" );
			return NULL;
		}

		switch ( glType )
		{
		case GL_RGBA8:
		case GL_RGBA16F:
		case GL_DEPTH_COMPONENT24:
		case GL_STENCIL_INDEX:
			break;
		default:
			E_ASSERT( !"Unexpected GL type for CreateRenderuffer(); proceeding..." );
		}
	}

	// allocate the renderbuffer.
	GLuint handle = 0;
	{
		glGenRenderbuffers( 1, &handle );
		glBindRenderbuffer( GL_RENDERBUFFER, handle );
		if ( uiMultisample > 0 )
			glRenderbufferStorageMultisample( GL_RENDERBUFFER, uiMultisample, glType, uiWidth, uiHeight );
		else
			glRenderbufferStorage( GL_RENDERBUFFER, glType, uiWidth, uiHeight );
	}

	// make sure GL is okay.
	CHECK_GL();

	// return it.
	GL2Renderbuffer* pRet	= E_NEW(GL2Renderbuffer);
	pRet->m_hGLHandle		= handle;
	pRet->m_uiWidth			= uiWidth;
	pRet->m_uiHeight		= uiHeight;
	pRet->m_uiMultisample	= uiMultisample;
	return pRet;
}

//---------------------------------------------------------------------------
void
GL2Renderbuffer::Bind( GL2TextureBase* pTexBase )
{
	if ( !pTexBase )
	{
		// unbind the renderbuffer.
		glBindRenderbuffer( GL_RENDERBUFFER, 0 );
	}
	else
	{
		// verify the texture is a renderbuffer.
		E_ASSERT( pTexBase->GetType() == GL2_TEXTYPE_RENDERBUFFER );
		if ( pTexBase->GetType() != GL2_TEXTYPE_RENDERBUFFER )
			return;

		// bind the renderbuffer.
		GL2Renderbuffer* pRenderbuffer = (GL2Renderbuffer*)pTexBase;
		glBindRenderbuffer( GL_RENDERBUFFER, pRenderbuffer->GetGLHandle() );
	}

	// verify GL is okay.
	CHECK_GL();
}

//---------------------------------------------------------------------------
void
GL2Renderbuffer::Unbind()
{
	// unbind the renderbuffer.
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );

	// verify GL is okay.
	CHECK_GL();
}
