#include "common_afx.h"
#include "m_mat4x4.h"

//===========================================================================
// math library declarations.
//===========================================================================

//PURPOSE:	Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this only once. See glhMultiplyMatrixByMatrix4by4f_2.
static void _glhMultiplyMatrixByMatrix4by4f_1(PxF32 *leftMatrix, PxF32 *rightMatrix, PxF32 *result);


//===========================================================================
// MMat4x4
//===========================================================================

static const PxF32 _identity4x4[] = {
	1.0F, 0.0F, 0.0F, 0.0F,
	0.0F, 1.0F, 0.0F, 0.0F,
	0.0F, 0.0F, 1.0F, 0.0F,
	0.0F, 0.0F, 0.0F, 1.0F,
};
const MMat4x4 MMat4x4::Identity( _identity4x4 );


//---------------------------------------------------------------------------
MMat4x4::MMat4x4( const MMat4x4& rhs )
{
	MemCopy( GetData(), rhs.GetData(), (4 * 4 * sizeof(PxF32)) );
}

//---------------------------------------------------------------------------
MMat4x4&
MMat4x4::operator=( const MMat4x4& rhs )
{
	MemCopy( GetData(), rhs.GetData(), (4 * 4 * sizeof(PxF32)) );
	return *this;
}

//---------------------------------------------------------------------------
MMat4x4::MMat4x4()
{
	// set to identity.
	MemCopy( &m_pfData[0], &_identity4x4[0], (sizeof(PxF32) * 16) );
}

//---------------------------------------------------------------------------
MMat4x4::MMat4x4( const PxF32* values4x4 )
{
	// copy from incoming.
	MemCopy( &m_pfData[0], values4x4, (sizeof(PxF32) * 16) );
}

//---------------------------------------------------------------------------
MMat4x4
MMat4x4::operator*( const MMat4x4& rhs ) const
{
	// allocate an uninitialized matrix.
	MMat4x4 ret(true);

	// multiply us with the other matrix, storing the result into ret.
	_glhMultiplyMatrixByMatrix4by4f_1( (PxF32*)&m_pfData[0], (PxF32*)&rhs.m_pfData[0], &ret.m_pfData[0] );

	// return the result.
	return ret;
}

//===========================================================================
// math library definitions.
//===========================================================================

//---------------------------------------------------------------------------
static void _glhMultiplyMatrixByMatrix4by4f_1(PxF32 *leftMatrix, PxF32 *rightMatrix, PxF32 *result)
{
	result[0]=leftMatrix[0]*rightMatrix[0]+
		leftMatrix[4]*rightMatrix[1]+
		leftMatrix[8]*rightMatrix[2]+
		leftMatrix[12]*rightMatrix[3];
	result[4]=leftMatrix[0]*rightMatrix[4]+
		leftMatrix[4]*rightMatrix[5]+
		leftMatrix[8]*rightMatrix[6]+
		leftMatrix[12]*rightMatrix[7];
	result[8]=leftMatrix[0]*rightMatrix[8]+
		leftMatrix[4]*rightMatrix[9]+
		leftMatrix[8]*rightMatrix[10]+
		leftMatrix[12]*rightMatrix[11];
	result[12]=leftMatrix[0]*rightMatrix[12]+
		leftMatrix[4]*rightMatrix[13]+
		leftMatrix[8]*rightMatrix[14]+
		leftMatrix[12]*rightMatrix[15];

	result[1]=leftMatrix[1]*rightMatrix[0]+
		leftMatrix[5]*rightMatrix[1]+
		leftMatrix[9]*rightMatrix[2]+
		leftMatrix[13]*rightMatrix[3];
	result[5]=leftMatrix[1]*rightMatrix[4]+
		leftMatrix[5]*rightMatrix[5]+
		leftMatrix[9]*rightMatrix[6]+
		leftMatrix[13]*rightMatrix[7];
	result[9]=leftMatrix[1]*rightMatrix[8]+
		leftMatrix[5]*rightMatrix[9]+
		leftMatrix[9]*rightMatrix[10]+
		leftMatrix[13]*rightMatrix[11];
	result[13]=leftMatrix[1]*rightMatrix[12]+
		leftMatrix[5]*rightMatrix[13]+
		leftMatrix[9]*rightMatrix[14]+
		leftMatrix[13]*rightMatrix[15];

	result[2]=leftMatrix[2]*rightMatrix[0]+
		leftMatrix[6]*rightMatrix[1]+
		leftMatrix[10]*rightMatrix[2]+
		leftMatrix[14]*rightMatrix[3];
	result[6]=leftMatrix[2]*rightMatrix[4]+
		leftMatrix[6]*rightMatrix[5]+
		leftMatrix[10]*rightMatrix[6]+
		leftMatrix[14]*rightMatrix[7];
	result[10]=leftMatrix[2]*rightMatrix[8]+
		leftMatrix[6]*rightMatrix[9]+
		leftMatrix[10]*rightMatrix[10]+
		leftMatrix[14]*rightMatrix[11];
	result[14]=leftMatrix[2]*rightMatrix[12]+
		leftMatrix[6]*rightMatrix[13]+
		leftMatrix[10]*rightMatrix[14]+
		leftMatrix[14]*rightMatrix[15];

	result[3]=leftMatrix[3]*rightMatrix[0]+
		leftMatrix[7]*rightMatrix[1]+
		leftMatrix[11]*rightMatrix[2]+
		leftMatrix[15]*rightMatrix[3];
	result[7]=leftMatrix[3]*rightMatrix[4]+
		leftMatrix[7]*rightMatrix[5]+
		leftMatrix[11]*rightMatrix[6]+
		leftMatrix[15]*rightMatrix[7];
	result[11]=leftMatrix[3]*rightMatrix[8]+
		leftMatrix[7]*rightMatrix[9]+
		leftMatrix[11]*rightMatrix[10]+
		leftMatrix[15]*rightMatrix[11];
	result[15]=leftMatrix[3]*rightMatrix[12]+
		leftMatrix[7]*rightMatrix[13]+
		leftMatrix[11]*rightMatrix[14]+
		leftMatrix[15]*rightMatrix[15];
}
