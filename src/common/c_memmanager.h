#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#define E_NEW(type)						new type
#define E_DELETE( p )					(delete (p), (p)=NULL)
#define E_DELETE_ARRAY( p )				(delete [] (p), (p)=NULL)

#define MemAlloc( size )				malloc( size )
#define MemFree( p )					( free(p), (p) = NULL )

#define MemSet( p, val, size )			memset( p, val, size )
#define MemZero( p, size )				memset( p, 0, size )

// TODO: replace with an optimized memcpy (prefetching / SSE, etc).
#define MemCopy( dst, src, size )		memcpy( dst, src, size );