#include "render_gl2_afx.h"
#include "gl2_framebuffer.h"

// GL2 headers.
#include "gl2_texture2d.h"

//===========================================================================
// GL2FramebufferAttachment
//===========================================================================

//---------------------------------------------------------------------------
GL2FramebufferAttachment::GL2FramebufferAttachment()
: m_glAttachType( GL_MAX_COLOR_ATTACHMENTS )
, m_glTarget( GL_TEXTURE_2D )
, m_eImageType( GL2_TEXTYPE_INVALID )
, m_pImage( NULL )
, m_uiMiplevel( 0 )
{
}

//---------------------------------------------------------------------------
GL2FramebufferAttachment::GL2FramebufferAttachment(
								// the attachment:  GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, etc.
								GLenum						gl_eAttachType, 

								// the target:  GL_TEXTURE_2D or GL_TEXTURE_RECTANGLE_ARB
								GLenum						gl_eTarget, 

								// the specified should be GL2_TEXTYPE_2D or GL_TEXTYPE_RENDERBUFFER.
								// (it's necessary to specify this value, since pFBAttachableImage can be NULL.)
								ETextureType				gl2_eTextype, 

								// the image we're attaching to the framebuffer.
								GL2TextureBase*				pFBAttachableImage,

								// specifies which miplevel of the image we should attach. 
								// (Ignored for TEXTYPE_RENDERBUFFER textures, as they don't mip.)
								PxU32						uiMiplevel )
: m_glAttachType( gl_eAttachType )
, m_glTarget( gl_eTarget )
, m_eImageType( gl2_eTextype )
, m_pImage( pFBAttachableImage )
, m_uiMiplevel( uiMiplevel )
{
}

//---------------------------------------------------------------------------
GLuint
GL2FramebufferAttachment::GetGLHandle() const
{
	if ( !m_pImage )
		return GLNULL;

	return m_pImage->GetGLHandle();
}

//---------------------------------------------------------------------------
void
GL2FramebufferAttachment::BindAttachment() const
{
	// we must have an image of type "renderbuffer", or "2D texture".
	E_ASSERT( (m_eImageType == GL2_TEXTYPE_2D) || (m_eImageType == GL2_TEXTYPE_RENDERBUFFER) );

	if ( m_eImageType == GL2_TEXTYPE_2D )
	{
		// we have a 2D texture!
		{
			// unbind all.
			GL2Texture2D::Unbind( 0 );

			// bind it.
			GL2Texture2D::Bind( 0, GetImage() );

			if ( GetGLTarget() == GL_TEXTURE_2D )
			{
				// generate miplevels for TEXTURE_2D images.
				glGenerateMipmap( GL_TEXTURE_2D );
				CHECK_GL();
			}

			// attach it.
			glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GetGLAttachType(), GetGLTarget(), GetGLHandle(), GetMiplevel() );
		}
		CHECK_GL();
	}
	else if ( m_eImageType == GL2_TEXTYPE_RENDERBUFFER )
	{
		// we have a renderbuffer! 
		{
			// unbind all.
			GL2Texture2D::Unbind( 0 );

			// attach it.
			glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GetGLAttachType(), GL_RENDERBUFFER, GetGLHandle() );
		}
		CHECK_GL();
	}
}

//---------------------------------------------------------------------------
void
GL2FramebufferAttachment::UnbindAttachment() const
{
	// we must have an image of type "renderbuffer", or "2D texture".
	E_ASSERT( (m_eImageType == GL2_TEXTYPE_2D) || (m_eImageType == GL2_TEXTYPE_RENDERBUFFER) );

	if ( m_eImageType == GL2_TEXTYPE_2D )
	{
		// we have a 2D texture!
		glFramebufferTexture2D( GL_DRAW_FRAMEBUFFER, GetGLAttachType(), GetGLTarget(), GLNULL, GetMiplevel() );
	}
	else if ( m_eImageType == GL2_TEXTYPE_RENDERBUFFER )
	{
		// we have a renderbuffer!
		glFramebufferRenderbuffer( GL_DRAW_FRAMEBUFFER, GetGLAttachType(), GL_RENDERBUFFER, GLNULL );
	}
}

//===========================================================================
// Framebuffer - Private Implementation
//===========================================================================
class GL2Framebuffer_impl
{
public:
	~GL2Framebuffer_impl();
	GL2Framebuffer_impl();

	// the GL framebuffer handle.
	GLuint						hGLHandle;

	// the width and height of the framebuffer.
	PxU32						uiWidth;
	PxU32						uiHeight;

	// the attachments of the framebuffer.
	GL2FramebufferAttachment	pAttachments[GL2_ATTACHMENTS_COUNT];
};

//---------------------------------------------------------------------------
GL2Framebuffer_impl::~GL2Framebuffer_impl()
{
}

//---------------------------------------------------------------------------
GL2Framebuffer_impl::GL2Framebuffer_impl()
: uiWidth(0)
, uiHeight(0)
{
}

//===========================================================================
// Framebuffer
//===========================================================================

//---------------------------------------------------------------------------
GL2Framebuffer::GL2Framebuffer()
: E_IMPL_NEW(GL2Framebuffer)
{
}

//---------------------------------------------------------------------------
GL2Framebuffer::~GL2Framebuffer()
{
	glDeleteFramebuffers( 1, &m.hGLHandle );
	E_IMPL_DEL(GL2Framebuffer);
}

//---------------------------------------------------------------------------
GL2Framebuffer*
GL2Framebuffer::CreateFramebuffer( PxU32 uiFBWidth, PxU32 uiFBHeight )
{
	// validate our parameters.
	E_ASSERT( uiFBWidth > 0 );
	E_ASSERT( uiFBHeight > 0 );

	// create the framebuffer.
	GLuint hGLHandle = 0;
	glGenFramebuffers( 1, &hGLHandle );
	E_ASSERT( hGLHandle > 0 );

	// return it.
	GL2Framebuffer* pRet = E_NEW( GL2Framebuffer );
	pRet->m.uiWidth = uiFBWidth;
	pRet->m.uiHeight = uiFBHeight;
	pRet->m.hGLHandle = hGLHandle;
	return pRet;
}

//---------------------------------------------------------------------------
void
GL2Framebuffer::Bind( GL2Framebuffer* pDrawFramebuffer, GL2Framebuffer* pReadFramebuffer )
{
	glBindFramebuffer( GL_DRAW_FRAMEBUFFER, pDrawFramebuffer ? pDrawFramebuffer->m.hGLHandle : 0 );
	glBindFramebuffer( GL_READ_FRAMEBUFFER, pReadFramebuffer ? pReadFramebuffer->m.hGLHandle : 0 );
	CHECK_GL();
}

//---------------------------------------------------------------------------
void
GL2Framebuffer::Unbind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, GLNULL );
	CHECK_GL();
}

//---------------------------------------------------------------------------
void
GL2Framebuffer::VerifyBind( GL2Framebuffer* pDrawFramebuffer, GL2Framebuffer* pReadFramebuffer )
{
	Bind( pDrawFramebuffer, pReadFramebuffer );

	// validate the framebuffer state.
#define PrintF printf
#define B_ERROR(x) printf("%s\n",x)
	bool bValid = true;
	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if ( status == GL_FRAMEBUFFER_UNSUPPORTED )
	{
		// no can do!
		PrintF( "Framebuffer creation failed; configuration not supported.\n" );
		bValid = false;
	}
	else if ( status != GL_FRAMEBUFFER_COMPLETE )
	{
		switch( status )
		{
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			B_ERROR( "Framebuffer incomplete ( General programming error ): Incomplete attachment!" );

		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			B_ERROR( "Framebuffer incomplete ( General programming error ): Incomplete missing attachment!" );

		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			B_ERROR( "Framebuffer incomplete ( General programming error ): Incomplete draw buffer!" );

		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			B_ERROR( "Framebuffer incomplete ( General programming error ): Incomplete read buffer!" );

		default:
			B_ERROR( "Framebuffer incomplete ( General programming error ): Unknown error!" );
		};
		bValid = false;
	}
	E_ASSERT( bValid );
}

//---------------------------------------------------------------------------
PxU32
GL2Framebuffer::GetWidth()
{
	return m.uiWidth;
}

//---------------------------------------------------------------------------
PxU32
GL2Framebuffer::GetHeight()
{
	return m.uiHeight;
}

//---------------------------------------------------------------------------
bool
GL2Framebuffer::Attach( const GL2FramebufferAttachment& cAttach )
{
	// first, make sure GL's happy.
	CHECK_GL();

	// determine which attachment we're binding to.
	EFramebufferAttachmentType eAttachType = GLAttachType_to_FramebufferAttachType( cAttach.GetGLAttachType() );
	if ( eAttachType == GL2_ATTACH_INVALID )
		return false;

	// validate.
	if ( cAttach.GetImage() != NULL )
	{
		E_ASSERT ( cAttach.GetImageType() == cAttach.GetImage()->GetType() );
		if ( cAttach.GetImageType() != cAttach.GetImage()->GetType() )
			return false;
	}
	CHECK_GL();


	// attach.
	{
		Bind( this, this );
		CHECK_GL();

		cAttach.BindAttachment();
		CHECK_GL();

		Unbind();
		CHECK_GL();
	}

	// update bookkeeping.
	m.pAttachments[eAttachType] = cAttach;
	return true;
}
