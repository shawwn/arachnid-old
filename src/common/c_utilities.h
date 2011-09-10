#pragma once

//***************************************************************************
// Utilities
//***************************************************************************

// hash functions.
PxU32 HashMem32( const void* mem, PxU32 size, PxU32 hashStart = 2166136261UL );
PxU64 HashMem64( const void* mem, PxU32 size, PxU64 hashStart = 14695981039346656037ULL );

template < typename _T > E_FORCE_INLINE void Swap( _T& a, _T& b )
{
	_T c = a;
	a = b;
	b = c;
}
