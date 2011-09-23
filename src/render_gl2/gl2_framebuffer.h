#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class GL2Framebuffer_impl;
class GL2FramebufferAttachment;
class GL2TextureBase;

//===========================================================================
// FramebufferAttachment
//	A framebuffer is defined as a set of three "attachments": color, stencil,
//	and depth.	Each attachment is one of the following:
//
//	1) a specific mipmap level of a 2D texture, or
//	2) a specific mipmap level of a cubemap face, or
//	3) a specific mipmap level of a slice of a 3D texture, or
//	4) a "renderbuffer", which is (very roughly) a texture that supports
//		multisampling.  (There are more details to it than that, but that's
//		the primary reason for wanting to use a renderbuffer instead of
//		a texture2D.)
//===========================================================================
class GL2FramebufferAttachment
{
	// copyable.
public:
	GL2FramebufferAttachment();
	GL2FramebufferAttachment(	// the attachment:  GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT, GL_STENCIL_ATTACHMENT, etc.
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
								PxU32						uiMiplevel = 0 );

	// accessors.
	GLuint							GetGLHandle() const;
	GLenum							GetGLTarget() const				{	return m_glTarget;		}
	GLenum							GetGLAttachType() const			{	return m_glAttachType;	}
	ETextureType					GetImageType() const			{	return m_eImageType;	}
	GL2TextureBase*					GetImage() const				{	return m_pImage;		}
	PxU32							GetMiplevel() const				{	return m_uiMiplevel;	}

	// calls glFramebufferTexture2D() if the image is TEXTYPE_2D,
	// or gFramebufferRenderbuffer() if the image is TEXTYPE_RENDERBUFFER.
	void							BindAttachment() const;
	void							UnbindAttachment() const;

private:
	GLenum							m_glAttachType;
	GLenum							m_glTarget;
	ETextureType					m_eImageType;
	GL2TextureBase*					m_pImage;
	PxU32							m_uiMiplevel;
};

//===========================================================================
// Framebuffer
//	A class to represent a rendertarget.
//===========================================================================
class GL2Framebuffer
{
	GL2Framebuffer();
public:
	~GL2Framebuffer();

	static GL2Framebuffer*			CreateFramebuffer( PxU32 uiWidth, PxU32 uiHeight );

	static void						Bind( GL2Framebuffer* pDrawFramebuffer, GL2Framebuffer* pReadFramebuffer = NULL );
	static void						Unbind();
	static void						VerifyBind( GL2Framebuffer* pDrawFramebuffer, GL2Framebuffer* pReadFramebuffer = NULL );

	PxU32							GetWidth();
	PxU32							GetHeight();

	bool							Attach( const GL2FramebufferAttachment& cAttach );

private:
	E_IMPL( GL2Framebuffer );
};
