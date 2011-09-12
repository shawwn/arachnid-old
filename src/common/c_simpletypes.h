#pragma once

//***************************************************************************
// Declarations
//***************************************************************************

#include "common/c_preprocessor.h"

// prevent PxSimpleTypes.h from ever being included.
#define PX_FOUNDATION_PX_SIMPLE_TYPES_H

// define NULL value.
#ifndef NULL
# ifdef __cplusplus
#  define NULL    0
# else
#  define NULL    ((void *)0)
# endif
#endif



//===========================================================================
// Basic Types
//===========================================================================
namespace physx
{
namespace pubfnd3
{


#if defined (E_WINDOWS)

	typedef signed __int64		PxI64;
	typedef signed int			PxI32;
	typedef signed short		PxI16;
	typedef signed char			PxI8;

	typedef unsigned __int64	PxU64;
	typedef unsigned int		PxU32;
	typedef unsigned short		PxU16;
	typedef unsigned char		PxU8;

	typedef float				PxF32;
	typedef double				PxF64;
		
#elif defined(E_LINUX) || defined(E_APPLE)

	typedef signed long long	PxI64;
	typedef signed int			PxI32;
	typedef signed short		PxI16;
	typedef signed char			PxI8;

	typedef unsigned long long	PxU64;
	typedef unsigned int		PxU32;
	typedef unsigned short		PxU16;
	typedef unsigned char		PxU8;

	typedef float				PxF32;
	typedef double				PxF64;

#else
	#error Unknown platform!
#endif

/**
Sanity Checks
*/

	E_COMPILE_TIME_ASSERT(sizeof(PxI8)==1);
	E_COMPILE_TIME_ASSERT(sizeof(PxU8)==1);
	E_COMPILE_TIME_ASSERT(sizeof(PxI16)==2);
	E_COMPILE_TIME_ASSERT(sizeof(PxU16)==2);
	E_COMPILE_TIME_ASSERT(sizeof(PxI32)==4);
	E_COMPILE_TIME_ASSERT(sizeof(PxU32)==4);
	E_COMPILE_TIME_ASSERT(sizeof(PxI64)==8);
	E_COMPILE_TIME_ASSERT(sizeof(PxU64)==8);
	E_COMPILE_TIME_ASSERT(sizeof(PxF32)==4);
	E_COMPILE_TIME_ASSERT(sizeof(PxF64)==8);

	// Type ranges
#define	E_MAX_I8			127					//maximum possible sbyte value, 0x7f
#define	E_MIN_I8			(-128)				//minimum possible sbyte value, 0x80
#define	E_MAX_U8			255U				//maximum possible ubyte value, 0xff
#define	E_MIN_U8			0					//minimum possible ubyte value, 0x00
#define	E_MAX_I16			32767				//maximum possible sword value, 0x7fff
#define	E_MIN_I16			(-32768)			//minimum possible sword value, 0x8000
#define	E_MAX_U16			65535U				//maximum possible uword value, 0xffff
#define	E_MIN_U16			0					//minimum possible uword value, 0x0000
#define	E_MAX_I32			2147483647			//maximum possible sdword value, 0x7fffffff
#define	E_MIN_I32			(-2147483647 - 1)	//minimum possible sdword value, 0x80000000
#define	E_MAX_U32			4294967295U			//maximum possible udword value, 0xffffffff
#define	E_MIN_U32			0					//minimum possible udword value, 0x00000000
#define	E_MAX_F32			3.4028234663852885981170418348452e+38F	
												//maximum possible float value
#define	E_MAX_F64			DBL_MAX				//maximum possible double value

#define E_EPS_F32			FLT_EPSILON			//maximum relative error of float rounding
#define E_EPS_F64			DBL_EPSILON			//maximum relative error of double rounding

#ifndef E_FOUNDATION_USE_F64

	typedef PxF32			PxReal;

	#define	E_MAX_REAL		E_MAX_F32
	#define E_EPS_REAL		E_EPS_F32

#else

	typedef PxF64			PxReal;

	#define	E_MAX_REAL		E_MAX_F64
	#define E_EPS_REAL		E_EPS_F64

#endif

}
using namespace pubfnd3;
}

//===========================================================================
// Forward Namespace
//===========================================================================
namespace physx
{
	using namespace pubfnd3;
}
using physx::PxU8;
using physx::PxU16;
using physx::PxU32;
using physx::PxU64;
using physx::PxI8;
using physx::PxI16;
using physx::PxI32;
using physx::PxI64;
using physx::PxF32;
using physx::PxF64;

// assume a char is always 8 bits.
typedef PxU8	byte;

//===========================================================================
// Assertion
//===========================================================================

#ifndef E_ASSERT
	// prevent PxAssert.h from ever being included.
	#define E_FOUNDATION_E_ASSERT_H

	#include <cassert>
	#define E_ASSERT(x)		assert(x)
	#define E_ALWAYS_ASSERT() E_ASSERT(0)
#endif


//===========================================================================
// Misc
//===========================================================================

#undef E_SIGN_BITMASK
#define E_SIGN_BITMASK		0x80000000


#pragma warning( disable : 4512 ) //  assignment operator could not be generated