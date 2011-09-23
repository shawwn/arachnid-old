#include "render_gl2_afx.h"
#include "gl2_texture2d.h"

// engine headers.
#include "engine/c_filemanager.h"

// graphics headers.
#include "graphics/gr_image.h"

//***************************************************************************
// Sorts a list of image filenames from "most common extension" to "least
// common extension".  The priorities are:
//
static const char* kImageExtensionPriority[] = { 
	".tga", ".png", ".jpg", ".jpeg", ".exr", ".bmp", ".tif", NULL
};
//***************************************************************************
void	PrioritizeImageFilenames( StringList& vFilenames );

//***************************************************************************
// Other helpers.
//***************************************************************************

// load a single miplevel from image in memory.
bool	glLoadMiplevelFromImage2D( GLenum glTarget, GLint iMiplevel, GrImage* pImage,
								  // when nonzero, the loaded image must be exactly these dimensions.
								  PxU32 kRequiredWidth = 0, PxU32 kRequiredHeight = 0 );
// from a single miplevel file.
bool	glLoadMiplevelFromFile2D( GLenum glTarget, GLint iMiplevel, const string& sPath,
								  // when nonzero, the loaded image must be exactly these dimensions.
								  PxU32 kRequiredWidth = 0, PxU32 kRequiredHeight = 0 )
{
	// load the image.
	GrImage* pTestImage = GrImage::LoadImageFromFile( FileManager.OpenFile( sPath ) );
	// load the miplevel.
	bool bSuccess = glLoadMiplevelFromImage2D( glTarget, iMiplevel, GrImage::LoadImageFromFile(sPath), kRequiredWidth, kRequiredHeight );
	// delete the image.
	E_DELETE( pTestImage );
	return bSuccess;
}

//===========================================================================
// Texture2D
//===========================================================================

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::LoadFromImage( GrImage* pImage )
{
	if ( !pImage )
		return NULL;

	// allocate a GL texture, loading the image into it.  (No mipmaps are generated.)
	GLuint gl = 0;
	glGenTextures( 1, &gl );
	glBindTexture( GL_TEXTURE_2D, gl );
	glLoadMiplevelFromImage2D( GL_TEXTURE_2D, 0, pImage );

	// verify GL is okay.
	CHECK_GL();

	// return the texture.
	GL2Texture2D* pRet		= E_NEW(GL2Texture2D);
	pRet->m_hGLHandle		= gl;
	pRet->m_uiMiplevels		= 1;
	pRet->m_uiWidth			= pImage->GetWidth();
	pRet->m_uiHeight		= pImage->GetHeight();
	return pRet;
}

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::LoadFromFile( const string& sPath )
{
	// load the image.
	GrImage* pTestImage = GrImage::LoadImageFromFile( FileManager.OpenFile( sPath ) );
	// make the texture.
	GL2Texture2D* pTex = GL2Texture2D::LoadFromImage( pTestImage );
	// delete the image.
	E_DELETE( pTestImage );
	// return the texture.
	return pTex;
}

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::LoadMipsFromFile( const string& sDirtyPath, PxU32 kMaxResolution, string sFileExt )
{
	// ensure our file extension is not empty, and begins with a period.  This is required.
	if ( sFileExt.empty() )
		sFileExt = ".png";
	else if ( sFileExt[0] != '.' )
		sFileExt = ( "." + sFileExt );

	// the path may not contain wildcard characters.
	if ( sDirtyPath.find_first_of( "*?" ) != string::npos )
	{
		E_ASSERT( !"wildcard characters not allowed in this path." );
		return NULL;
	}
	string sPath( FileManager.NormalizePath(sDirtyPath) );

	// extract the base path and its filename.
	string sDirectory;
	string sFilenameBase;
	{
		size_t uiSlashPos = sPath.find_last_of( "/\\" );
		if ( uiSlashPos == string::npos )
		{
			sDirectory = "./";
			sFilenameBase = sPath;
		}
		else
		{
			sDirectory = sPath.substr( 0, (uiSlashPos + 1) );
			sFilenameBase = sPath.substr( uiSlashPos + 1 );
		}
	}

	// fetch a list of all miplevel textures.  (foo_512.png, foo_256.png, foo_128.png, etc.)
	StringList vFiles;
	FileManager.FindFiles( vFiles, sDirectory, (sFilenameBase + "_*" + sFileExt) );

	// build a list of all possible resolutions in the directory.
	vector<PxU32> vPossibleResolutions;
	for ( size_t i(0); i < vFiles.size(); ++i )
	{
		const string& sFile( vFiles[i] );

		// skip to "_" in the filename like "/media/textures/rock_512.png"
		size_t uiUnderscorePos( sFile.find_last_of( '_' ) );
		E_ASSERT( uiUnderscorePos != string::npos );
		if ( uiUnderscorePos == string::npos )
			continue;

		// figure out the resolution indicated by the filename suffix.
		PxU32 uiResolution = atoi( &sFile[uiUnderscorePos + 1] );
		if ( uiResolution == 0 )
			continue;

		// if the resolution is greater than the max allowed, skip it.
		if ( uiResolution > kMaxResolution )
			continue;

		vPossibleResolutions.push_back( uiResolution );
	}

	// remove duplicates.
	std::unique( vPossibleResolutions.begin(), vPossibleResolutions.end() );
	if ( vPossibleResolutions.empty() )
		return NULL;

	// sort from highest to highest resolution.
	std::sort( vPossibleResolutions.begin(), vPossibleResolutions.end(), std::greater<PxU32>() );

	// attempt to build the mipchain.
	for ( vector<PxU32>::iterator it( vPossibleResolutions.begin() );
		it != vPossibleResolutions.end();
		++it )
	{
		PxU32 uiHighestResolution( *it );
		GL2Texture2D* pMipChain = BuildMipChain( sDirectory, vFiles, uiHighestResolution );

		// if we loaded it successfully, we're done!
		if ( pMipChain != NULL )
			return pMipChain;
	}

	// verify GL is okay.
	CHECK_GL();

	// failed to load.  =(
	return NULL;
}

//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::CreateTexture2D( GLenum glInternalFormat, // GL_RGBA8 etc
							  GLenum glFormat, // GL_BGRA etc
							  GLenum glType, // GL_UNSIGNED_BYTE or GL_FLOAT, probably.
							  PxU32 uiWidth, PxU32 uiHeight, bool bIsRectangle )
{
	// allocate a GL texture, loading the image into it.
	GLuint handle = 0;
	GLenum stage = bIsRectangle ? GL_TEXTURE_RECTANGLE_ARB : GL_TEXTURE_2D;
	glGenTextures( 1, &handle );
	glBindTexture( stage, handle );
	{
		GLint uiMiplevel = 0;
		glTexImage2D( stage, uiMiplevel, glInternalFormat, uiWidth, uiHeight,
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
	pRet->m_bRectangleTexture = bIsRectangle;
	return pRet;
}

//---------------------------------------------------------------------------
GL2Texture2D::GL2Texture2D()
: GL2TextureBase( GL2_TEXTYPE_2D )
, m_hGLHandle( 0 )
, m_uiMiplevels( 0 )
, m_uiWidth( 0 )
, m_uiHeight( 0 )
, m_bRectangleTexture( false )
{
}

//---------------------------------------------------------------------------
GL2Texture2D::~GL2Texture2D()
{
}

//---------------------------------------------------------------------------
void
GL2Texture2D::Bind( PxU32 uiTexUnit, GL2TextureBase* pTexBase, bool bIsDepth )
{
	// sanity check the texunit.
	E_ASSERT( uiTexUnit < 16 );
	uiTexUnit = Min(16U, uiTexUnit);

	if ( !pTexBase )
	{
#if 1
		// unbind the texture.
		Unbind( uiTexUnit );
#endif
	}
	else
	{
		// sanity check the texture type.
		E_ASSERT( pTexBase->GetType() == GL2_TEXTYPE_2D );
		if ( pTexBase->GetType() != GL2_TEXTYPE_2D )
			return;

#if 1
		// unbind the texture.
		Unbind( uiTexUnit );
#endif

		GLenum glTarget = pTexBase->GetGLTarget();

		// bind the 2D texture.
		GL2Texture2D* pTexture2D = (GL2Texture2D*)pTexBase;
		CHECK_GL();
		glActiveTexture( GL_TEXTURE0 + uiTexUnit );
		CHECK_GL();
		glBindTexture( glTarget, pTexture2D->GetGLHandle() );

		// hardcode linear filtering for now.
		CHECK_GL();
		glTexParameteri( glTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		CHECK_GL();
		glTexParameteri( glTarget, GL_TEXTURE_MIN_FILTER,
			(glTarget == GL_TEXTURE_RECTANGLE_ARB) ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR ); //GL_LINEAR_MIPMAP_LINEAR );

		CHECK_GL();
		glTexParameterf(glTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		CHECK_GL();
		glTexParameterf(glTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if ( bIsDepth )
		{
			glTexParameteri( glTarget, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE );
			glTexParameteri( glTarget, GL_TEXTURE_COMPARE_MODE_ARB, GL_NONE );
		}
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

	CHECK_GL();
	glActiveTexture( GL_TEXTURE0 + uiTexUnit );
	CHECK_GL();
	glBindTexture( GL_TEXTURE_2D, GLNULL );
	CHECK_GL();
	glBindTexture( GL_TEXTURE_RECTANGLE_ARB, GLNULL );
	CHECK_GL();
}

//---------------------------------------------------------------------------
void
GL2Texture2D::DrawFullscreen( PxF32 fL, PxF32 fR, PxF32 fB, PxF32 fT )
{
	glBegin( GL_QUADS );
	{
		glVertexAttrib2f( GL2_ATTRIB_TEXCOORD_INDEX, 0.0F, 0.0F );  glVertexAttrib3f( GL2_ATTRIB_POSITION_INDEX, fL, fB, 0.0F );
		glVertexAttrib2f( GL2_ATTRIB_TEXCOORD_INDEX, 0.0F, 1.0F );  glVertexAttrib3f( GL2_ATTRIB_POSITION_INDEX, fL, fT, 0.0F );
		glVertexAttrib2f( GL2_ATTRIB_TEXCOORD_INDEX, 1.0F, 1.0F );  glVertexAttrib3f( GL2_ATTRIB_POSITION_INDEX, fR, fT, 0.0F );
		glVertexAttrib2f( GL2_ATTRIB_TEXCOORD_INDEX, 1.0F, 0.0F );  glVertexAttrib3f( GL2_ATTRIB_POSITION_INDEX, fR, fB, 0.0F );
	}
	glEnd();
}

//***************************************************************************
// Internal Functions
//***************************************************************************

//---------------------------------------------------------------------------
bool
glLoadMiplevelFromImage2D( GLenum glTarget, GLint iMiplevell, GrImage* pMip,
						  // when nonzero, the loaded image must be exactly these dimensions.
						  PxU32 kRequiredWidth, PxU32 kRequiredHeight )
{
	if ( pMip == NULL )
		return false;

	// if a required resolution is specified, then the loaded image must be exactly that.
	{
		if ( kRequiredWidth > 0 )
		{
			E_ASSERT( pMip->GetWidth() == kRequiredWidth );
			if ( pMip->GetWidth() != kRequiredWidth )
				return false;
		}

		if ( kRequiredHeight > 0 )
		{
			E_ASSERT( pMip->GetHeight() == kRequiredHeight );
			if ( pMip->GetHeight() != kRequiredHeight )
				return false;
		}
	}

	// load the mipmap.
	E_ASSERT( pMip->GetBitsPerPixel() >= 24 );
	switch ( pMip->GetBitsPerPixel() )
	{
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 3-channel images (RGB, BGR)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	case 24: // 8-bit ints in BGR format.
		glTexImage2D( glTarget, iMiplevell, GL_RGB8, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_BGR, GL_UNSIGNED_BYTE, (const GLvoid*)pMip->GetPixels() );
		break;

	case 48: // 16-bit ints in BGR format.
		glTexImage2D( glTarget, iMiplevell, GL_RGB16, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_BGR, GL_UNSIGNED_SHORT, (const GLvoid*)pMip->GetPixels() );
		break;

	case 96: // 32-bit floats in RGB format.  (EXR stores in RGB, and EXR is the primary use case.)
		glTexImage2D( glTarget, iMiplevell, GL_RGB32F, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_BGR, GL_FLOAT, (const GLvoid*)pMip->GetPixels() );
		break;

		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		// 4-channel images (RGBA, BGRA)
		//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	case 32: // 8-bit ints in BGRA format.
		glTexImage2D( glTarget, iMiplevell, GL_RGB8, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_BGRA, GL_UNSIGNED_BYTE, (const GLvoid*)pMip->GetPixels() );
		break;

	case 64: // 16-bit ints in BGRA format.
		glTexImage2D( glTarget, iMiplevell, GL_RGB16, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_BGRA, GL_UNSIGNED_SHORT, (const GLvoid*)pMip->GetPixels() );
		break;

	case 128: // 32-bit floats in RGBA format.  (EXR stores in premultiplied RGBA, and EXR is the primary use case.)
		glTexImage2D( glTarget, iMiplevell, GL_RGB32F, pMip->GetWidth(), pMip->GetHeight(),
			0, GL_RGBA, GL_FLOAT, (const GLvoid*)pMip->GetPixels() );
		break;
	}

	// verify GL is okay.
	CHECK_GL();

	return true;
}

//---------------------------------------------------------------------------
void
PrioritizeImageFilenames( StringList& vFilenames )
{
	// If there are multiple filenames (different extensions) for a given resolution, then
	// prioritize by the table of image extensions defined above.
	struct PrioritizeByExtension
	{
		bool operator()( const string& sPathA, const string& sPathB )
		{
#ifdef WIN32
			// on windows, we ignore filename case.
			bool bIgnoreCase( true );
#else
			// on unix, filenames are case-sensitive.
			bool bIgnoreCase( false );
#endif

			string sLeftExt( Path_GetFilenameExtension( sPathA ) );
			string sRightExt( Path_GetFilenameExtension( sPathB ) );

			PxI32 iLeftPriority = FindStringInList( sLeftExt.c_str(), kImageExtensionPriority, bIgnoreCase );
			PxI32 iRightPriority = FindStringInList( sRightExt.c_str(), kImageExtensionPriority, bIgnoreCase );

			return ( iLeftPriority >= iRightPriority );
		}
	};
	std::sort( vFilenames.begin(), vFilenames.end(), PrioritizeByExtension() );
}


//---------------------------------------------------------------------------
GL2Texture2D*
GL2Texture2D::BuildMipChain( const string& sDirectory, const StringList& vFiles, PxU32 uiHighestResolution )
{
	E_ASSERT( uiHighestResolution > 0 );
	if ( uiHighestResolution <= 0 )
		return GLNULL;

	// create our GL texture.
	GLuint glHandle(0);
	glGenTextures( 1, &glHandle );
	glActiveTexture( GL_TEXTURE0 );
	glBindTexture( GL_TEXTURE_2D, glHandle );

	PxU32 uiMiplevel( 0 );
	PxU32 uiResolution( uiHighestResolution );
	while ( uiResolution > 0 )
	{
		// try to load the current miplevel.
		bool bLoaded( false );
		{
			// find the current resolution's files.
			StringList vTryLoadFiles;
			MatchWildcard( vTryLoadFiles, (string("*_")+ItoA(uiResolution)+".*"), vFiles );

			// prioritize them by extension (from most common extension, to least common.)
			PrioritizeImageFilenames( vTryLoadFiles );

			for ( StringList::const_iterator itFile( vTryLoadFiles.begin() );
				(itFile != vTryLoadFiles.end());
				++itFile )
			{
				const string& sFilename( *itFile );
				if ( glLoadMiplevelFromFile2D( GL_TEXTURE_2D, uiMiplevel, FileManager.Join(sDirectory, sFilename), uiResolution, uiResolution ) )
				{
					if ( uiResolution == uiHighestResolution )
					{
						// if this is our highest-resolution miplevel, then auto-generate the rest of the mipchain
						// (to fill any gaps --- the miplevels not found on disk.)  This way an artist doesn't
						// have to specify every single miplevel all the way down to 1x1 -- that would be silly. =)
						glGenerateMipmap( GL_TEXTURE_2D );
					}

					bLoaded = true;
					break;
				}
			}
		}

		// if the miplevel failed to load, we're done.
		if ( !bLoaded )
			break;

		// calculate our next miplevel resolution: half of our current resolution.
		//
		// If the resolution is odd, then we round DOWN.  Here's the relevant GL spec.
		// (Skip reading this, unless you're a graphics nerd like me.)
		//
		//		"Each successively smaller mipmap level is half the size
		//		of the previous level, but if this half value is a fractional value,
		//		you should round down to the next largest integer.  Essentially:
		//
		//		The primary reason to favor the floor convention is that Direct3D
		//		uses the floor convention.
		//
		//		Also, the "ceiling" convention potentially implies one more mipmap
		//		level than the "floor" convention.
		//
		//		Some regard the "ceiling" convention to have nicer properties with
		//		respect to making sure that each level samples at at least 2x the
		//		frequency of the next level.  This can reduce the chances of
		//		sampling artifacts.  However, it's probably not worth diverging
		//		from the Direct3D convention just for this.  A more sophisticated
		//		downsampling algorithm (using a larger kernel perhaps) during
		//		mipmap level generation can help reduce artifacts related to using 
		//		the "floor" convention."
		//
		++uiMiplevel;
		uiResolution >>= 1;
	}

	// if we failed to load any miplevels, abort.
	if ( uiMiplevel <= 0 )
	{
		glBindTexture( GL_TEXTURE_2D, GLNULL );
		glDeleteTextures( 1, &glHandle );
		return GLNULL;
	}

	// determine our miplevel count.
	PxU32 uiMiplevelCount( 1 + Log2(uiHighestResolution) );
	E_ASSERT( uiMiplevel <= uiMiplevelCount );

	// otherwise, return the texture.
	GL2Texture2D* pRet		= E_NEW(GL2Texture2D);
	pRet->m_hGLHandle		= glHandle;
	pRet->m_uiMiplevels		= uiMiplevelCount;
	pRet->m_uiWidth			= uiHighestResolution;
	pRet->m_uiHeight		= uiHighestResolution;
	return pRet;
}
