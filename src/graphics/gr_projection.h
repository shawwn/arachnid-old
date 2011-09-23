#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class GrProjection_impl;
class MMat4x4;

//===========================================================================
// GrProjection
//===========================================================================
class GrProjection
{
public:
	// copyable.
	GrProjection&	operator = ( const GrProjection& rhs );
	GrProjection( const GrProjection& rhs );

	// constructors.
	~GrProjection();
	GrProjection();

	// orthographic projection.
	GrProjection( PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 zNear, PxF32 zFar );

	// perspective projection.
	GrProjection( PxF32 fovy, PxF32 aspect, PxF32 zNear, PxF32 farCull = 10000.0F, PxF32 zFar = 0.0F );

	// a FOV of zero means orthographic mode.
	bool			IsOrtho() const				{	return (m_fFovY == 0.0F);		}

	// setters.
	void			SetFOVY( PxF32 fovy )		{	m_fFovY = fovy;			m_bDirty = true;	}
	void			SetAspect( PxF32 aspect )	{	m_fAspect = aspect;		m_bDirty = true;	}
	void			SetZNear( PxF32 zNear )		{	m_fZNear = zNear;		m_bDirty = true;	}
	void			SetZFar( PxF32 zFar )		{	m_fZFar = zFar;			m_bDirty = true;	}
	void			SetFarCull( PxF32 farCull )	{	m_fFarCull = farCull;	m_bDirty = true;	}
	void			SetLeft( PxF32 left )		{	m_fLeft = left;			m_bDirty = true;	}
	void			SetRight( PxF32 right )		{	m_fRight = right;		m_bDirty = true;	}
	void			SetBottom( PxF32 bottom )	{	m_fBottom = bottom;		m_bDirty = true;	}
	void			SetTop( PxF32 top )			{	m_fTop = top;			m_bDirty = true;	}

	// accessors.
	PxF32			GetFOVY() const				{	return m_fFovY;					}
	PxF32			GetAspect() const			{	return m_fAspect;				}
	PxF32			GetZNear() const			{	return m_fZNear;				}
	PxF32			GetZFar() const				{	return m_fZFar;					}
	PxF32			GetFarCull() const			{	return m_fFarCull;				}
	PxF32			GetLeft() const				{	return m_fLeft;					}
	PxF32			GetRight() const			{	return m_fRight;				}
	PxF32			GetBottom() const			{	return m_fBottom;				}
	PxF32			GetTop() const				{	return m_fTop;					}
	PxF32			GetWidth() const			{	return (m_fRight - m_fLeft);	}
	PxF32			GetHeight() const			{	return (m_fTop - m_fBottom);	}

	// returns the projection matrix, calculating it if necessary.
	const MMat4x4&	GetMatrix() const;

private:
	void			RecomputeMatrix() const;

	E_IMPL( GrProjection );

	// projection params.  
	PxF32			m_fFovY;		// a FOV of zero means orthographic mode.
	PxF32			m_fAspect;	// (screenWidth / screenHeight)
	PxF32			m_fZNear;
	PxF32			m_fZFar;		// a zFar of zero in perspective mode means infinite projection matrix.
	PxF32			m_fFarCull;

	// offcenter params.
	PxF32			m_fLeft;
	PxF32			m_fRight;
	PxF32			m_fBottom;
	PxF32			m_fTop;

	mutable bool	m_bDirty;
};
