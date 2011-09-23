#pragma once

//***************************************************************************
// Utilities
//***************************************************************************

//===========================================================================
// Hash functions.
//===========================================================================
PxU32 HashMem32( const void* mem, PxU32 size, PxU32 hashStart = 2166136261UL );
PxU64 HashMem64( const void* mem, PxU32 size, PxU64 hashStart = 14695981039346656037ULL );

//===========================================================================
// Mathematical / relational utilities.
//===========================================================================
#include <cmath>

template< typename T >
E_INLINE T			Min( T x, T y )				{	return (x < y) ? x : y;				}
template< typename T >
E_INLINE T			Max( T x, T y )				{	return (x > y) ? x : y;				}
template< typename T >
E_INLINE T			Min3( T x, T y, T z )		{	return Min( x, Min( y, z ) );		}
template< typename T >
E_INLINE T			Max3( T x, T y, T z )		{	return Max( x, Max( y, z ) );		}
template< typename T >
E_INLINE T			Clamp( T val, T lo, T hi )	{	return Max( lo, Min( hi, val ) );	}

E_INLINE PxF32		Ceil( PxF32 val )			{	return ceilf( val );	}
E_INLINE PxF64		Ceil( PxF64 val )			{	return ceil( val );		}
E_INLINE PxF32		Floor( PxF32 val )			{	return floorf( val );	}
E_INLINE PxF64		Floor( PxF64 val )			{	return floor( val );	}

E_INLINE PxF32		Sqrt( PxF32 x )				{	return sqrtf( x );		}
E_INLINE PxF64		Sqrt( PxF64 x )				{	return sqrt( x );		}
E_INLINE PxF32		Exp( PxF32 x )				{	return expf( x );		}
E_INLINE PxF64		Exp( PxF64 x )				{	return exp( x );		}
E_INLINE PxF32		Pow( PxF32 x, PxF32 n )		{	return powf( x, n );	}
E_INLINE PxF64		Pow( PxF64 x, PxF64 n )		{	return pow( x, n );		}
E_INLINE PxF32		Abs( PxF32 x )				{	return fabs( x );		}
E_INLINE PxF64		Abs( PxF64 x )				{	return abs( x );		}
E_INLINE PxF32		Sin( PxF32 x )				{	return sinf( x );		}
E_INLINE PxF64		Sin( PxF64 x )				{	return sin( x );		}
E_INLINE PxF32		Cos( PxF32 x )				{	return cosf( x );		}
E_INLINE PxF64		Cos( PxF64 x )				{	return cos( x );		}
E_INLINE PxF32		Tan( PxF32 x )				{	return tanf( x );		}
E_INLINE PxF64		Tan( PxF64 x )				{	return tan( x );		}

const PxF32			Pi(3.14159265358979323846f);
E_INLINE PxF32		RadToDeg( PxF32 rad )		{	return (180.0F / Pi) * rad;			}
E_INLINE PxF32		DegToRad( PxF32 deg )		{	return (Pi / 180.0F) * deg;			}

inline unsigned int Log2( unsigned int v )
{
	E_ASSERT( v > 0 );

	unsigned int i = 0;
	while ( v != 0 )
	{
		v >>= 1;
		++i;
	}
	i = Max( 1U, i );

	return ( i-1 );
}