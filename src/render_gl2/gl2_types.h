#pragma once

enum ETextureType
{
	GL2_TEXTYPE_1D,
	GL2_TEXTYPE_2D,
	GL2_TEXTYPE_3D,
	GL2_TEXTYPE_CUBE,
	GL2_TEXTYPE_RENDERBUFFER,

	GL2_TEXTYPES_COUNT,

	GL2_TEXTYPE_INVALID			= -1,
	FORCE_DWORD_GL2_TEXTYPES	= 0x7FFFFFFF,
};

enum EFramebufferAttachmentType
{
	GL2_ATTACH_COLOR0,
	GL2_ATTACH_COLOR1,
	GL2_ATTACH_COLOR2,
	GL2_ATTACH_COLOR3,
	GL2_ATTACH_DEPTH,
	GL2_ATTACH_STENCIL,

	GL2_ATTACHMENTS_COUNT,

	GL2_ATTACH_INVALID		= -1,
	FORCE_DWORD_GL2_ATTACH	= 0x7FFFFFFF,
};

//---------------------------------------------------------------------------
E_INLINE EFramebufferAttachmentType
GLAttachType_to_FramebufferAttachType( GLenum gl_eAttach )
{
	switch ( gl_eAttach )
	{
	case GL_COLOR_ATTACHMENT0:	return GL2_ATTACH_COLOR0;
	case GL_COLOR_ATTACHMENT1:	return GL2_ATTACH_COLOR1;
	case GL_COLOR_ATTACHMENT2:	return GL2_ATTACH_COLOR2;
	case GL_COLOR_ATTACHMENT3:	return GL2_ATTACH_COLOR3;
	case GL_DEPTH_ATTACHMENT:	return GL2_ATTACH_DEPTH;
	case GL_STENCIL_ATTACHMENT:	return GL2_ATTACH_STENCIL;
	}

	E_ASSERT( !"Invalid GL attachment type" );
	return GL2_ATTACH_INVALID;
}

//===========================================================================
// SVec2
//===========================================================================
struct SVec2
{
	SVec2()  { }
	SVec2( const SVec2& rhs );
	SVec2( PxF32 x, PxF32 y );

	// assignment.
	SVec2&			operator = ( const SVec2& rhs )			{	Set( rhs.x, rhs.y );  return *this;						}

	// accessors.
	PxF32*			GetData()								{	return &this->x;										}
	const PxF32*	GetData() const							{	return &this->x;										}

	// setters.
	void			Set( PxF32 x, PxF32 y )					{	this->x = x;  this->y = y;								}

	// arithmetic operators.
	SVec2			operator + ( const SVec2& rhs ) const	{	return SVec2( (x + rhs.x), (y + rhs.y) );				}
	SVec2			operator - ( const SVec2& rhs ) const	{	return SVec2( (x - rhs.x), (y - rhs.y) );				}
	friend SVec2	operator *( PxF32 s, const SVec2& vec ) {	return SVec2( (s * vec.x), (s * vec.y) );				}

	// member variables.
	PxF32			x;
	PxF32			y;
};


//===========================================================================
// SVec3
//===========================================================================
struct SVec3
{
	SVec3()  { }
	SVec3( const SVec3& rhs );
	SVec3( PxF32 x, PxF32 y, PxF32 z );

	// assignment.
	SVec3&			operator = ( const SVec3& rhs )			{	Set( rhs.x, rhs.y, rhs.z );  return *this;				}

	// accessors.
	PxF32*			GetData()								{	return &this->x;										}
	const PxF32*	GetData() const							{	return &this->x;										}

	// setters.
	void			Set( PxF32 x, PxF32 y, PxF32 z )		{	this->x = x;  this->y = y;  this->z = z;				}

	// arithmetic operators.
	SVec3			operator + ( const SVec3& rhs ) const	{	return SVec3( (x + rhs.x), (y + rhs.y), (z + rhs.z) );	}
	SVec3			operator - ( const SVec3& rhs ) const	{	return SVec3( (x - rhs.x), (y - rhs.y), (z - rhs.z) );	}
	friend SVec3	operator *( PxF32 s, const SVec3& vec ) {	return SVec3( (s * vec.x), (s * vec.y), (s * vec.z) );	}

	// member variables.
	PxF32			x;
	PxF32			y;
	PxF32			z;
};


//===========================================================================
// SVec4
//===========================================================================
struct SVec4
{
	SVec4()  { }
	SVec4( const SVec4& rhs );
	SVec4( PxF32 x, PxF32 y, PxF32 z, PxF32 w );

	// assignment.
	SVec4&			operator = ( const SVec4& rhs )			{	Set( rhs.x, rhs.y, rhs.z, rhs.w );  return *this;					}

	// accessors.
	PxF32*			GetData()								{	return &this->x;													}
	const PxF32*	GetData() const							{	return &this->x;													}

	// setters.
	void			Set( PxF32 x, PxF32 y, PxF32 z, PxF32 w )	{	this->x = x;  this->y = y;  this->z = z;  this->w = w;			}

	// arithmetic operators.
	SVec4			operator + ( const SVec4& rhs ) const	{	return SVec4( (x + rhs.x), (y + rhs.y), (z + rhs.z), (w + rhs.w) );	}
	SVec4			operator - ( const SVec4& rhs ) const	{	return SVec4( (x - rhs.x), (y - rhs.y), (z - rhs.z), (w - rhs.w) );	}
	friend SVec4	operator *( PxF32 s, const SVec4& vec ) {	return SVec4( (s * vec.x), (s * vec.y), (s * vec.z), (s * vec.w) );	}

	// member variables.
	PxF32			x;
	PxF32			y;
	PxF32			z;
	PxF32			w;
};
