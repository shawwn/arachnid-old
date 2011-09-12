#pragma once

//===========================================================================
// MMat4x4
//===========================================================================
class MMat4x4
{
	// 'uninitialized' ctor
	MMat4x4(bool) {}
public:
	// copyable.
	MMat4x4( const MMat4x4& rhs );
	MMat4x4&				operator = ( const MMat4x4& rhs );

	// constructors.
	MMat4x4();
	MMat4x4( const PxF32* values4x4 );

	// accessors.
	PxF32*					GetData()			{	return &m_pfData[0];	}
	const PxF32*			GetData() const		{	return &m_pfData[0];	}

	// multiply two 4x4 matrices.
	MMat4x4					operator * ( const MMat4x4& rhs ) const;

	// an identity matrix.
	static const MMat4x4	Identity;

private:
	PxF32					m_pfData[16];
};
