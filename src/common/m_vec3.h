#pragma once

//===========================================================================
// MVec3
//===========================================================================
class MVec3
{
public:
	MVec3();
	MVec3( const MVec3& rhs );
	MVec3( PxF32 x, PxF32 y, PxF32 z );

	PxF32			MagSq() const;
	PxF32			Mag() const;
	PxF32			Dot( const MVec3& rhs ) const;
	MVec3			Normalize();
	PxF32			NormalizeFast();

	// assignment.
	MVec3&			operator = ( const MVec3& rhs )			{	Set( rhs.GetX(), rhs.GetY(), rhs.GetZ() );  return *this;	}

	// accessors.
	PxF32			GetX() const							{	return _data[0];			}
	PxF32			GetY() const							{	return _data[1];			}
	PxF32			GetZ() const							{	return _data[2];			}
	PxF32*			GetData()								{	return &_data[0];			}
	const PxF32*	GetData() const							{	return &_data[0];			}

	// setters.
	void			SetX( PxF32 val )						{	_data[0] = val;				}
	void			SetY( PxF32 val )						{	_data[1] = val;				}
	void			SetZ( PxF32 val )						{	_data[2] = val;				}
	void			Set( PxF32 x, PxF32 y, PxF32 z )		{	SetX(x); SetY(y); SetZ(z);	}

	// arithmetic operators.
	MVec3			operator + ( const MVec3& rhs ) const	{	return MVec3( (GetX() + rhs.GetX()), (GetY() + rhs.GetY()), (GetZ() + rhs.GetZ()) );	}
	MVec3			operator - ( const MVec3& rhs ) const	{	return MVec3( (GetX() - rhs.GetX()), (GetY() - rhs.GetY()), (GetZ() - rhs.GetZ()) );	}
	friend MVec3	operator *( PxF32 s, const MVec3& vec ) {	return MVec3( (s * vec.GetX()), (s * vec.GetY()), (s * vec.GetZ()) );					}

	// constants.
	static const MVec3	Zero;
private:
	PxF32			_data[3];
};
