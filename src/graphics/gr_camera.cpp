#include "graphics_afx.h"
#include "gr_camera.h"

// graphics headers.
#include "graphics/gr_projection.h"

// math headers.
#include "common/m_vec3.h"
#include "common/m_mat4x4.h"

//***************************************************************************
// Math library declarations.
//***************************************************************************
static void _glhLookAtf2(PxF32 *matrix, PxF32 *eyePosition3D, PxF32 *center3D, PxF32 *upVector3D);
static void _glhLookAtf2_calcSideDir(PxF32 *side, PxF32 *eyePosition3D, PxF32 *center3D, PxF32 *upVector3D);

//===========================================================================
// GrCamera - Private state.
//===========================================================================
class GrCamera_impl
{
public:
	// copyable.
	GrCamera_impl( const GrCamera_impl& rhs );
	GrCamera_impl&			operator = ( const GrCamera_impl& rhs );

	// constructors.
	GrCamera_impl();
	GrCamera_impl( const MVec3& pos, const MVec3& lookAt, const MVec3& upDir, const GrProjection& proj );

	MMat4x4					cViewMat;

	MVec3					v3Pos;
	MVec3					v3UpDir;
	MVec3					v3LookDir;

	GrProjection			cProj;

	bool					bDirty;
};

//---------------------------------------------------------------------------
GrCamera_impl&
GrCamera_impl::operator=( const GrCamera_impl& rhs )
{
	cViewMat	= rhs.cViewMat;
	v3Pos		= rhs.v3Pos;
	v3LookDir	= rhs.v3LookDir;
	v3UpDir		= rhs.v3UpDir;
	cProj		= rhs.cProj;
	bDirty		= rhs.bDirty;
	return *this;
}

//---------------------------------------------------------------------------
GrCamera_impl::GrCamera_impl( const GrCamera_impl& rhs )
: cViewMat	( rhs.cViewMat )
, v3Pos		( rhs.v3Pos )
, v3LookDir	( rhs.v3LookDir )
, v3UpDir	( rhs.v3UpDir )
, cProj		( rhs.cProj )
, bDirty	( rhs.bDirty )
{
}

//---------------------------------------------------------------------------
GrCamera_impl::GrCamera_impl()
: v3Pos		( 0.0F, 0.0F, 0.0F )
, v3LookDir	( 0.0F, 0.0F, -1.0F )
, v3UpDir	( 0.0F, 1.0F, 0.0F )
, cProj		( GrProjection(DegToRad(60.0F), 1.0F, 1.0F) )
, bDirty	( true )
{
}

//---------------------------------------------------------------------------
GrCamera_impl::GrCamera_impl( const MVec3& pos, const MVec3& lookAt, const MVec3& upDir, const GrProjection& proj )
: v3Pos		( pos )
, v3LookDir	( lookAt - pos )
, v3UpDir	( upDir )
, cProj		( proj )
, bDirty	( true )
{
	v3LookDir.Normalize();
}

//===========================================================================
// GrCamera
//===========================================================================

//---------------------------------------------------------------------------
GrCamera::GrCamera( const GrCamera& rhs )
: E_IMPL_NEW( GrCamera, rhs.m )
{
}

//---------------------------------------------------------------------------
GrCamera&
GrCamera::operator=( const GrCamera& rhs )
{
	m = rhs.m;
	return *this;
}

//---------------------------------------------------------------------------
GrCamera::~GrCamera()
{
	E_IMPL_DEL( GrCamera );
}

//---------------------------------------------------------------------------
GrCamera::GrCamera()
: E_IMPL_NEW( GrCamera )
{
}

//---------------------------------------------------------------------------
GrCamera::GrCamera( const MVec3& pos, const MVec3& lookAt, const MVec3& upDir, const GrProjection& proj )
: E_IMPL_NEW( GrCamera, pos, lookAt, upDir, proj )
{
}

//---------------------------------------------------------------------------
const MMat4x4&
GrCamera::GetViewMat() const
{
	if ( m.bDirty )
	{
		RecomputeMatrix();
		m.bDirty = false;
	}
	return m.cViewMat;
}

//---------------------------------------------------------------------------
void
GrCamera::RecomputeMatrix() const
{
	assert( m.bDirty );

	m.cViewMat = MMat4x4::Identity;
	_glhLookAtf2( (PxF32*)m.cViewMat.GetData(),
		(PxF32*)m.v3Pos.GetData(),
		( m.v3Pos + m.v3LookDir ).GetData(),
		(PxF32*)m.v3UpDir.GetData() );
}

//---------------------------------------------------------------------------
void
GrCamera::SetPos( const MVec3& pos )
{
	m.v3Pos = pos;			m.bDirty = true;
}

//---------------------------------------------------------------------------
void
GrCamera::SetUpDir( const MVec3& up )
{
	m.v3UpDir = up;		m.bDirty = true;
}

//---------------------------------------------------------------------------
void
GrCamera::SetLookDir( const MVec3& look )
{
	m.v3LookDir = look;	
	m.v3LookDir.Normalize();
	m.bDirty = true;
}

//---------------------------------------------------------------------------
void
GrCamera::LookAt( const MVec3& lookAt )
{
	MVec3 v3Dir( lookAt - GetPos() );
	if ( v3Dir.MagSq() != 0.0F )
	{
		m.v3LookDir = v3Dir;	
		m.v3LookDir.Normalize();
		m.bDirty = true;
	}
}

//---------------------------------------------------------------------------
void
GrCamera::SetProj( const GrProjection& proj )
{
	m.cProj = proj;		m.bDirty = true;
}

//---------------------------------------------------------------------------
const MVec3&
GrCamera::GetPos() const
{
	return m.v3Pos;
}

//---------------------------------------------------------------------------
MVec3
GrCamera::CalcSideDir() const
{
	PxF32 side[3];
	_glhLookAtf2_calcSideDir( side,
		(PxF32*)m.v3Pos.GetData(),
		( m.v3Pos + m.v3LookDir ).GetData(),
		(PxF32*)m.v3UpDir.GetData() );
	return MVec3(side[0], side[1], side[2]);
}

//---------------------------------------------------------------------------
const MVec3&
GrCamera::GetUpDir() const
{
	return m.v3UpDir;
}

//---------------------------------------------------------------------------
const MVec3&
GrCamera::GetLookDir() const
{
	return m.v3LookDir;
}

//---------------------------------------------------------------------------
const GrProjection&
GrCamera::GetProj() const
{
	return m.cProj;
}

//---------------------------------------------------------------------------
const MMat4x4&
GrCamera::GetProjMat() const
{
	return m.cProj.GetMatrix();
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
inline void _ComputeNormalOfPlaneFLOAT_2(PxF32 *normal, const PxF32 *pvector1, const PxF32 *pvector2)
{
	normal[0]=(pvector1[1]*pvector2[2])-(pvector1[2]*pvector2[1]);
	normal[1]=(pvector1[2]*pvector2[0])-(pvector1[0]*pvector2[2]);
	normal[2]=(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0]);
}

//---------------------------------------------------------------------------
inline void _NormalizeVectorFLOAT_2(PxF32 *pvector)
{
	PxF32 normalizingConstant;
	normalizingConstant=1.0F/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
}

//---------------------------------------------------------------------------
static void _glhTranslatef2(PxF32 *matrix, PxF32 x, PxF32 y, PxF32 z)
{
	matrix[12]=matrix[0]*x+matrix[4]*y+matrix[8]*z+matrix[12];
	matrix[13]=matrix[1]*x+matrix[5]*y+matrix[9]*z+matrix[13];
	matrix[14]=matrix[2]*x+matrix[6]*y+matrix[10]*z+matrix[14];
	matrix[15]=matrix[3]*x+matrix[7]*y+matrix[11]*z+matrix[15];
}

//---------------------------------------------------------------------------
static void _glhLookAtf2(PxF32 *matrix, PxF32 *eyePosition3D, PxF32 *center3D, PxF32 *upVector3D)
{
	PxF32 forward[3], side[3], up[3];
	PxF32 matrix2[16], resultMatrix[16];

	forward[0]=center3D[0]-eyePosition3D[0];
	forward[1]=center3D[1]-eyePosition3D[1];
	forward[2]=center3D[2]-eyePosition3D[2];
	_NormalizeVectorFLOAT_2(forward);

	//Side = forward x up
	_ComputeNormalOfPlaneFLOAT_2(side, forward, upVector3D);
	_NormalizeVectorFLOAT_2(side);

	//Recompute up as: up = side x forward
	_ComputeNormalOfPlaneFLOAT_2(up, side, forward);

	matrix2[0]=side[0];
	matrix2[4]=side[1];
	matrix2[8]=side[2];
	matrix2[12]=0.0;

	matrix2[1]=up[0];
	matrix2[5]=up[1];
	matrix2[9]=up[2];
	matrix2[13]=0.0;

	matrix2[2]=-forward[0];
	matrix2[6]=-forward[1];
	matrix2[10]=-forward[2];
	matrix2[14]=0.0;

	matrix2[3]=matrix2[7]=matrix2[11]=0.0;
	matrix2[15]=1.0;

	_MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
	_glhTranslatef2(resultMatrix, -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
	
	memcpy(matrix, resultMatrix, 16*sizeof(PxF32));
}

//---------------------------------------------------------------------------
static void _glhLookAtf2_calcSideDir(PxF32 *side, PxF32 *eyePosition3D, PxF32 *center3D, PxF32 *upVector3D)
{
	PxF32 forward[3], up[3];

	forward[0]=center3D[0]-eyePosition3D[0];
	forward[1]=center3D[1]-eyePosition3D[1];
	forward[2]=center3D[2]-eyePosition3D[2];
	_NormalizeVectorFLOAT_2(forward);

	//Side = forward x up
	_ComputeNormalOfPlaneFLOAT_2(side, forward, upVector3D);
	_NormalizeVectorFLOAT_2(side);

	//Recompute up as: up = side x forward
	_ComputeNormalOfPlaneFLOAT_2(up, side, forward);
}
