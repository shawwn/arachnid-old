#include "render_gl2_afx.h"
#include "gl2_texture2d.h"

// graphics headers.
#include "graphics/gr_image.h"

//===========================================================================
// Texture2D
//===========================================================================

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::LoadFromImage( GrImage* pImage )
{
	if ( !pImage )
		return NULL;

	// allocate a GL texture, loading the image into it.
	GLuint gl = 0;
	glGenTextures(1, &gl);
	glBindTexture(GL_TEXTURE_2D, gl);
	{
		int level = 0;
		GLenum internal_format = GL_RGB8;
		GLenum image_format = GL_BGR;
		int border = 0;
		glTexImage2D(GL_TEXTURE_2D, level, internal_format, pImage->GetWidth(), pImage->GetHeight(),
			border, image_format, GL_UNSIGNED_BYTE, (const GLvoid*)pImage->GetPixels());
	}

	// verify GL is okay.
	CHECK_GL();

	// return the texture.
	GL2Texture2D* pRet		= E_NEW(GL2Texture2D);
	pRet->m_hGLHandle			= gl;
	pRet->m_uiMiplevels		= 1;
	pRet->m_uiWidth			= pImage->GetWidth();
	pRet->m_uiHeight		= pImage->GetHeight();
	return pRet;
}

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::CreateTexture2D( GLenum glInternalFormat, // GL_RGBA8 etc
							  GLenum glFormat, // GL_BGRA etc
							  GLenum glType, // GL_UNSIGNED_BYTE or GL_FLOAT, probably.
							  PxU32 uiWidth, PxU32 uiHeight )
{
	// allocate a GL texture, loading the image into it.
	GLuint handle = 0;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	{
		GLint uiMiplevel = 0;
		glTexImage2D( GL_TEXTURE_2D, uiMiplevel, glInternalFormat, uiWidth, uiHeight,
			0, glFormat, glType, NULL );
	}

	// verify GL is okay.
	CHECK_GL();

	// return the texture.
	GL2Texture2D* pRet		= E_NEW(GL2Texture2D);
	pRet->m_hGLHandle		= handle;
	pRet->m_uiMiplevels		= 1;
	pRet->m_uiWidth			= uiWidth;
	pRet->m_uiHeight		= uiHeight;
	return pRet;
}

//---------------------------------------------------------------------------
GL2Texture2D::GL2Texture2D()
: GL2TextureBase( GL2_TEXTYPE_2D )
, m_hGLHandle( 0 )
, m_uiMiplevels( 0 )
, m_uiWidth( 0 )
, m_uiHeight( 0 )
{
}

//---------------------------------------------------------------------------
GL2Texture2D::~GL2Texture2D()
{
}

//---------------------------------------------------------------------------
void
GL2Texture2D::Bind( PxU32 uiTexUnit, GL2TextureBase* pTexBase )
{
	// sanity check the texunit.
	E_ASSERT( uiTexUnit < 16 );
	uiTexUnit = Min(16U, uiTexUnit);

	if ( !pTexBase )
	{
		// unbind the texture.
		glActiveTexture( GL_TEXTURE0 + uiTexUnit );
		glBindTexture( GL_RENDERBUFFER, 0 );
	}
	else
	{
		// sanity check the texture type.
		E_ASSERT( pTexBase->GetType() == GL2_TEXTYPE_2D );
		if ( pTexBase->GetType() != GL2_TEXTYPE_2D )
			return;

		// bind the 2D texture.
		GL2Texture2D* pTexture2D = (GL2Texture2D*)pTexBase;
		glActiveTexture( GL_TEXTURE0 + uiTexUnit );
		glBindTexture( GL_TEXTURE_2D, pTexture2D->GetGLHandle() );

		// hardcode linear filtering for now.
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	}

	// verify GL is okay.
	CHECK_GL();
}

//---------------------------------------------------------------------------
void
GL2Texture2D::Unbind( PxU32 uiTexUnit )
{
	// activate the specified texunit.
	E_ASSERT( uiTexUnit < 16 );
	uiTexUnit = Min(16U, uiTexUnit);
	glActiveTexture( GL_TEXTURE0 + uiTexUnit );
}

//---------------------------------------------------------------------------
void
GL2Texture2D::DrawFullscreen( PxF32 fL, PxF32 fR, PxF32 fB, PxF32 fT )
{
	GL2Texture2D::Bind( GL2_TEXUNIT(0), this );

	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.0F, 0.0F );  glVertex2f( fL, fB );
		glTexCoord2f( 0.0F, 1.0F );  glVertex2f( fL, fT );
		glTexCoord2f( 1.0F, 1.0F );  glVertex2f( fR, fT );
		glTexCoord2f( 1.0F, 0.0F );  glVertex2f( fR, fB );
	}
	glEnd();
}
