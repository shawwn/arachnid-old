#include "render_gl2_afx.h"
#include "gl2_texture2d.h"

// graphics headers.
#include "graphics/c_image.h"

//===========================================================================
// Texture2D
//===========================================================================

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::LoadFromImage( CImage* pImage )
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
	GL2Texture2D* pRet = E_NEW(GL2Texture2D);
	pRet->m_uiHandle = gl;
	return pRet;
}

//---------------------------------------------------------------------------
GL2Texture2D::GL2Texture2D()
: GL2TextureBase( TEXTYPE_2D )
, m_uiHandle( 0 )
{
}

//---------------------------------------------------------------------------
GL2Texture2D::~GL2Texture2D()
{
}

//---------------------------------------------------------------------------
void
GL2Texture2D::Bind( PxU32 uiTexUnit )
{
	E_ASSERT( uiTexUnit < 16 );
	//glActiveTexture( GL_TEXTURE0 + uiTexUnit );
	glBindTexture( GL_TEXTURE_2D, GetGLHandle() );

	// hardcode linear filtering for now.
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// verify GL is okay.
	CHECK_GL();
}
