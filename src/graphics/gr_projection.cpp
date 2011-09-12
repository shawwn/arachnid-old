#include "graphics_afx.h"
#include "gr_projection.h"

// math headers.
#include "common/m_mat4x4.h"

//***************************************************************************
// Math library declarations.
//***************************************************************************
static void _glhOrthof2(PxF32 *matrix, PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 znear, PxF32 zfar);
static void _glhFrustumInfiniteFarPlanef2(PxF32 *matrix, PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 znear);
static void _glhPerspectiveInfiniteFarPlanef2(PxF32 *matrix, PxF32 fovyInDegrees, PxF32 aspectRatio, PxF32 znear);
static void _glhPerspectivef2(PxF32 *matrix, PxF32 fovyInDegrees, PxF32 aspectRatio, PxF32 znear, PxF32 zfar);

//===========================================================================
// GrProjection - Private state.
//===========================================================================
class GrProjection_impl
{
public:
	GrProjection_impl( const GrProjection_impl& rhs );
	GrProjection_impl();

	MMat4x4			cProjMat;
};

//---------------------------------------------------------------------------
GrProjection_impl::GrProjection_impl( const GrProjection_impl& rhs )
: cProjMat( rhs.cProjMat )
{
}

//---------------------------------------------------------------------------
GrProjection_impl::GrProjection_impl()
{
}

//===========================================================================
// GrProjection
//===========================================================================

//---------------------------------------------------------------------------
GrProjection::GrProjection( const GrProjection& rhs )
: E_IMPL_NEW( GrProjection, rhs.m )
, m_fFovY	( rhs.m_fFovY )
, m_fAspect	( rhs.m_fAspect )
, m_fZNear	( rhs.m_fZNear )
, m_fZFar	( rhs.m_fZFar )
, m_fFarCull( rhs.m_fFarCull )
, m_fLeft	( rhs.m_fLeft )
, m_fRight	( rhs.m_fRight )
, m_fBottom	( rhs.m_fBottom )
, m_fTop	( rhs.m_fTop )
, m_bDirty	( rhs.m_bDirty )
{
}

//---------------------------------------------------------------------------
GrProjection&
GrProjection::operator=( const GrProjection& rhs )
{
	m			= rhs.m;
	m_fFovY		= rhs.m_fFovY;
	m_fAspect	= rhs.m_fAspect;
	m_fZNear	= rhs.m_fZNear;
	m_fZFar		= rhs.m_fZFar;
	m_fFarCull	= rhs.m_fFarCull;
	m_fLeft		= rhs.m_fLeft;
	m_fRight	= rhs.m_fRight;
	m_fBottom	= rhs.m_fBottom;
	m_fTop		= rhs.m_fTop;
	m_bDirty	= rhs.m_bDirty;
	return *this;
}

//---------------------------------------------------------------------------
GrProjection::~GrProjection()
{
	E_IMPL_DEL( GrProjection );
}

//---------------------------------------------------------------------------
GrProjection::GrProjection()
: E_IMPL_NEW( GrProjection )
, m_fFovY( DegToRad(60.0F) )
, m_fAspect( 1.0F )
, m_fZNear( 1.0F )
, m_fZFar( 0.0F )
, m_fFarCull( 10000.0F )
, m_fLeft( -1.0F )
, m_fRight( 1.0F )
, m_fBottom( -1.0F )
, m_fTop( 1.0F )
, m_bDirty( true )
{
}

//---------------------------------------------------------------------------
GrProjection::GrProjection( PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 zNear, PxF32 zFar /*= 10000.0F */ )
: E_IMPL_NEW( GrProjection )
, m_fFovY( 0.0F )
, m_fZNear( zNear )
, m_fZFar( zFar )
, m_fFarCull( zFar )
, m_fLeft( left )
, m_fRight( right )
, m_fBottom( bottom )
, m_fTop( top )
, m_bDirty( true )
{
	assert( GetWidth() > 0.0F );
	assert( GetHeight() > 0.0F );
	m_fAspect = GetWidth() / GetHeight();
}

//---------------------------------------------------------------------------
GrProjection::GrProjection( PxF32 fovy, PxF32 aspect, PxF32 zNear, PxF32 farCull /*= 10000.0F*/, PxF32 zFar /*= 0.0F */ )
: E_IMPL_NEW( GrProjection )
, m_fFovY( fovy )
, m_fAspect( aspect )
, m_fZNear( zNear )
, m_fZFar( zFar )
, m_fFarCull( farCull )
, m_fLeft( -1.0F )
, m_fRight( 1.0F )
, m_fBottom( -1.0F )
, m_fTop( 1.0F )
, m_bDirty( true )
{
}

//---------------------------------------------------------------------------
const MMat4x4&
GrProjection::GetMatrix() const
{
	if ( m_bDirty )
	{
		RecomputeMatrix();
		m_bDirty = false;
	}
	return m.cProjMat;
}

//---------------------------------------------------------------------------
void
GrProjection::RecomputeMatrix() const
{
	assert( m_bDirty );

	if ( IsOrtho() )
	{
		// orthographic.
		_glhOrthof2( (PxF32*)m.cProjMat.GetData(), m_fLeft, m_fRight, m_fBottom, m_fTop, m_fZNear, m_fZFar );
	}
	else
	{
		// TODO: incorporate left/right/bottom/top to perform offcenter projection!
		assert( (m_fLeft == -1.0F) && (m_fRight == 1.0F) && (m_fBottom == -1.0F) && (m_fTop == 1.0F) );

		if ( m_fZFar == 0.0F )
		{
			// infinite perspective projection matrix.
			_glhPerspectiveInfiniteFarPlanef2( (PxF32*)m.cProjMat.GetData(), RadToDeg(m_fFovY), m_fAspect, m_fZNear );
		}
		else
		{
			// regular perspective projection matrix.
			_glhPerspectivef2( (PxF32*)m.cProjMat.GetData(), RadToDeg(m_fFovY), m_fAspect, m_fZNear, m_fZFar );
		}
	}
}

//***************************************************************************
// Math library definitions.
//***************************************************************************

//---------------------------------------------------------------------------
inline void _MultiplyMatrices4by4OpenGL_FLOAT(PxF32 *result, PxF32 *matrix1, PxF32 *matrix2)
{
	result[0]=matrix1[0]*matrix2[0]+
		matrix1[4]*matrix2[1]+
		matrix1[8]*matrix2[2]+
		matrix1[12]*matrix2[3];
	result[4]=matrix1[0]*matrix2[4]+
		matrix1[4]*matrix2[5]+
		matrix1[8]*matrix2[6]+
		matrix1[12]*matrix2[7];
	result[8]=matrix1[0]*matrix2[8]+
		matrix1[4]*matrix2[9]+
		matrix1[8]*matrix2[10]+
		matrix1[12]*matrix2[11];
	result[12]=matrix1[0]*matrix2[12]+
		matrix1[4]*matrix2[13]+
		matrix1[8]*matrix2[14]+
		matrix1[12]*matrix2[15];

	result[1]=matrix1[1]*matrix2[0]+
		matrix1[5]*matrix2[1]+
		matrix1[9]*matrix2[2]+
		matrix1[13]*matrix2[3];
	result[5]=matrix1[1]*matrix2[4]+
		matrix1[5]*matrix2[5]+
		matrix1[9]*matrix2[6]+
		matrix1[13]*matrix2[7];
	result[9]=matrix1[1]*matrix2[8]+
		matrix1[5]*matrix2[9]+
		matrix1[9]*matrix2[10]+
		matrix1[13]*matrix2[11];
	result[13]=matrix1[1]*matrix2[12]+
		matrix1[5]*matrix2[13]+
		matrix1[9]*matrix2[14]+
		matrix1[13]*matrix2[15];

	result[2]=matrix1[2]*matrix2[0]+
		matrix1[6]*matrix2[1]+
		matrix1[10]*matrix2[2]+
		matrix1[14]*matrix2[3];
	result[6]=matrix1[2]*matrix2[4]+
		matrix1[6]*matrix2[5]+
		matrix1[10]*matrix2[6]+
		matrix1[14]*matrix2[7];
	result[10]=matrix1[2]*matrix2[8]+
		matrix1[6]*matrix2[9]+
		matrix1[10]*matrix2[10]+
		matrix1[14]*matrix2[11];
	result[14]=matrix1[2]*matrix2[12]+
		matrix1[6]*matrix2[13]+
		matrix1[10]*matrix2[14]+
		matrix1[14]*matrix2[15];

	result[3]=matrix1[3]*matrix2[0]+
		matrix1[7]*matrix2[1]+
		matrix1[11]*matrix2[2]+
		matrix1[15]*matrix2[3];
	result[7]=matrix1[3]*matrix2[4]+
		matrix1[7]*matrix2[5]+
		matrix1[11]*matrix2[6]+
		matrix1[15]*matrix2[7];
	result[11]=matrix1[3]*matrix2[8]+
		matrix1[7]*matrix2[9]+
		matrix1[11]*matrix2[10]+
		matrix1[15]*matrix2[11];
	result[15]=matrix1[3]*matrix2[12]+
		matrix1[7]*matrix2[13]+
		matrix1[11]*matrix2[14]+
		matrix1[15]*matrix2[15];
}

//---------------------------------------------------------------------------
static void _glhFrustumf2(PxF32 *matrix, PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 znear, PxF32 zfar)
{
	PxF32 matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0F*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0F;
	matrix2[2]=0.0F;
	matrix2[3]=0.0F;
	matrix2[4]=0.0F;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0F;
	matrix2[7]=0.0F;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=(-zfar-znear)/temp4;
	matrix2[11]=-1.0F;
	matrix2[12]=0.0F;
	matrix2[13]=0.0F;
	matrix2[14]=(-temp*zfar)/temp4;
	matrix2[15]=0.0F;
	_MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(PxF32));
}

//---------------------------------------------------------------------------
static void _glhOrthof2(PxF32 *matrix, PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 znear, PxF32 zfar)
{
	PxF32 matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0F/temp2;
	matrix2[1]=0.0F;
	matrix2[2]=0.0F;
	matrix2[3]=0.0F;
	matrix2[4]=0.0F;
	matrix2[5]=2.0F/temp3;
	matrix2[6]=0.0F;
	matrix2[7]=0.0F;
	matrix2[8]=0.0F;
	matrix2[9]=0.0F;
	matrix2[10]=-2.0F/temp4;
	matrix2[11]=0.0F;
	matrix2[12]=(-right-left)/temp2;
	matrix2[13]=(-top-bottom)/temp3;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=1.0F;
	_MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(PxF32));
}

//---------------------------------------------------------------------------
static void _glhFrustumInfiniteFarPlanef2(PxF32 *matrix, PxF32 left, PxF32 right, PxF32 bottom, PxF32 top, PxF32 znear)
{
	PxF32 matrix2[16], temp, temp2, temp3, resultMatrix[16];
	temp=2.0F*znear;
	temp2=right-left;
	temp3=top-bottom;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0F;
	matrix2[2]=0.0F;
	matrix2[3]=0.0F;
	matrix2[4]=0.0F;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0F;
	matrix2[7]=0.0F;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=-1.0F;
	matrix2[11]=-1.0F;
	matrix2[12]=0.0F;
	matrix2[13]=0.0F;
	matrix2[14]=-2.0F*znear;
	matrix2[15]=0.0F;
	_MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(PxF32));
}

//---------------------------------------------------------------------------
static void _glhPerspectiveInfiniteFarPlanef2(PxF32 *matrix, PxF32 fovyInDegrees, PxF32 aspectRatio, PxF32 znear)
{
	PxF32 ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*Pi/360.0F);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	_glhFrustumInfiniteFarPlanef2(matrix, -xmax, xmax, -ymax, ymax, znear);
}

//---------------------------------------------------------------------------
static void _glhPerspectivef2(PxF32 *matrix, PxF32 fovyInDegrees, PxF32 aspectRatio, PxF32 znear, PxF32 zfar)
{
	PxF32 ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*Pi/360.0F);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	_glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}
