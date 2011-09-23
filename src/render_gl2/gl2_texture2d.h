#pragma once

#include "render_gl2/gl2_texturebase.h"

//***************************************************************************
// Declarations
//***************************************************************************
class GrImage;

//===========================================================================
// Texture2D
//===========================================================================
class GL2Texture2D : public GL2TextureBase
{
	GL2Texture2D();
public:
	~GL2Texture2D();

	static GL2Texture2D*	LoadFromImage( GrImage* pImage );
	static GL2Texture2D*	LoadFromFile( const string& sPath );
	static GL2Texture2D*	LoadMipsFromFile(
											// e.g: "/media/textures/rock" if you want to load "/media/textures/rock.jpg"
											const string& sPath,
											// the max resolution MIP that will be loaded.
											PxU32 kMaxResolution = 16384,
											// e.g.: ".png" ... however, typically just leave it as the wildcard search.
											string sFileExt = ".*" );
	static GL2Texture2D*	CreateTexture2D( GLenum glInternalFormat, GLenum glFormat, GLenum glType,
											PxU32 uiWidth, PxU32 uiHeight, bool bIsRectangle = false );
	static void				Bind( PxU32 uiTexUnit, GL2TextureBase* pTex2D );
	static void				Unbind( PxU32 uiTexUnit );

	virtual PxU32			GetGLTarget()				{ return m_bRectangleTexture ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;	}
	virtual PxU32			GetGLHandle()				{ return m_hGLHandle;	}
	virtual PxU32			GetMiplevelCount()			{ return m_uiMiplevels;	}
	virtual PxU32			GetWidth()					{ return m_uiWidth;		}
	virtual PxU32			GetHeight()					{ return m_uiHeight;	}

	virtual void			DrawFullscreen( PxF32 fLeft, PxF32 fRight, PxF32 fBot, PxF32 fTop );
	void					DrawFullscreen()			{ GL2TextureBase::DrawFullscreen();	}

private:
	static GL2Texture2D*	BuildMipChain( const string& sDirectory, const StringList& vFiles, PxU32 uiHighestResolution );

	GLuint					m_hGLHandle;
	PxU32					m_uiMiplevels;
	PxU32					m_uiWidth;
	PxU32					m_uiHeight;
	bool					m_bRectangleTexture;
};
