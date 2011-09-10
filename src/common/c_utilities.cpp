#include "common_afx.h"
#include "c_utilities.h"

//===========================================================================
// Utilities
//===========================================================================

//---------------------------------------------------------------------------
PxU32
HashMem32( const void* mem, PxU32 size, PxU32 hashStart )
{
	// hash the string.
	const char* str = ( const char* )mem;
	const int kPrime = 16777619;
	for ( PxU32 i = 0; i < size; ++i )
	{
		// hash the current character.
		int ch = str[ i ] & 0xFF;
		hashStart = ( hashStart * kPrime );
		hashStart = hashStart ^ ch;
	}

	// return the new hash.
	return hashStart;
}

//---------------------------------------------------------------------------
PxU64
HashMem64( const void* mem, PxU32 size, PxU64 hashStart )
{
	// hash the string.
	const char* str = ( const char* )mem;
	const PxI64 kPrime = 1099511628211LL;
	for ( PxU32 i = 0; i < size; ++i )
	{
		// hash the current character.
		PxI64 ch = str[ i ] & 0xFF;
		hashStart = ( hashStart * kPrime );
		hashStart = hashStart ^ ch;
	}

	// return the new hash.
	return hashStart;
}
