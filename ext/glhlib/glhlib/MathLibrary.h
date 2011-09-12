#if !defined(_MATHLIBRARY_H_)
#define _MATHLIBRARY_H_

#include <math.h>
#include "MemoryManagement.h"

#pragma warning(disable: 4244)	//Shut up about sreal2 to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const sreal2' to 'sreal'


#ifndef M_PI
#define M_PI		3.1415926535897932384626433832795
#endif


//Created: Monday, Dec 25, 2000
//Purpose: To make coding easier by having all functions in one place
//Performance: All functions are made inline
//Will contain all sorts of math functions, for doing linear algebra,
//computation, calculus
//CONTAINS A SECTION FOR DOING MATRIX MATH
//May contain assembly code for the x86 processors
//Don't use template functions.
//Make two versions, one for sreal and sreal2
//function name format is
//       functionxxxxxxxxxFLOAT_Y();   where Y can be any number indicating
//function version
//we may also have
//functionxxxxxxxxxDOUBLE_Y();
//functionxxxxxxxxxLONGDOUBLE_Y();

//PURPOSE: Given 3 points, compute the normal vector
	//[  i   j   k  ]
	//[1-2  1-2  1-2]
	//[3-2  3-2  3-2]
//Each pointer refers to three consecutive memory locations: x, y, z
//Pointers must be valid, function won't verify for NULL value.
//tail is common to both vectors (terminal1 and terminal2.
inline void ComputeNormalOfPlaneFLOAT_3(sreal *normal, const sreal *terminal1, const sreal *terminal2, const sreal *tail)
{
	normal[0]=((terminal1[1]-tail[1])*(terminal2[2]-tail[2]))-((terminal1[2]-tail[2])*(terminal2[1]-tail[1]));
	normal[1]=((terminal1[2]-tail[2])*(terminal2[0]-tail[0]))-((terminal1[0]-tail[0])*(terminal2[2]-tail[2]));
	normal[2]=((terminal1[0]-tail[0])*(terminal2[1]-tail[1]))-((terminal1[1]-tail[1])*(terminal2[0]-tail[0]));
	//VC++ will do better optimizing probably
	/*
	//The following is an optimized version of the cross product
	//It might be possible to improve it. No benchmarking done on it.
	__asm
	{
		//lea edi, go
		//mov eax, [edi]
		//mov eax, [edi+4]
		//mov eax, [edi+8]
		//Put some of the values in the integer registers (for speed up)
		//lea			edi, terminal2
		//mov			eax, [edi]
		//mov			ebx, [edi+4]
		//lea			edi, terminal1
		//mov			ecx, [edi]
		//mov			edx, [edi+8]
		//fld			ebx or whatever causes a compiler error!!!!!!

		//We can only load 2 terminal values in stack +
		//the 3 tail values, plus 2 temporary storage, plus st(0) for working on
		mov			eax, dword ptr[terminal2]
		mov			ebx, dword ptr[terminal1]
		mov			ecx, dword ptr[tail]
		mov			edx, dword ptr[normal]
		fld         dword ptr[eax+8]		;st(4)
		fld			dword ptr[ebx+4]		;st(3)
		fld			dword ptr[ecx]			;tail[0]        st(2)
		fld			dword ptr[ecx+4]		;tail[1]        st(1)
		fld			dword ptr[ecx+8]		;tail[2]        st(0)
		///////////////////////////////////////////////////
		//normal[0]=((terminal1[1]-tail[1])*(terminal2[2]-tail[2]))
		//		-((terminal1[2]-tail[2])*(terminal2[1]-tail[1]));
		//Now compute .... -((terminal1[2]-tail[2])*(terminal2[1]-tail[1]));
		fld			dword ptr[ebx+8]		;edx contains terminal1[2]
		//REMEMBER, for the next line, compiler wants 2 operands!!!!!!!!
		fsub		st(0), st(1)			;(terminal1[2]-tail[2])
		fxch		st(6)					;put it aside
		fstp		st(0)					;pop the stack
		fld			dword ptr[eax+4]		;ebx contains terminal2[1]
		fsub		st(0), st(2)			;(terminal2[1]-tail[1])
		fmulp		st(6), st(0)			;((terminal1[2]-tail[2])*(terminal2[1]-tail[1]))
		//Now compute  normal[0]=((terminal1[1]-tail[1])*(terminal2[2]-tail[2])) ...
		fld			st(3)					;or dword ptr[terminal1+4]
		fsub		st(0), st(2)			;compiler asks for 2 operands???
		fxch		st(7)					;put it aside for now
		fstp		st(0)					;pop the stack
		fld			st(4)					;dword ptr[terminal2+8]
		fsub		st(0), st(1)			;terminal2[2]-tail[2]
											;answer is in st(0)
		fmul		st(0), st(7)			;((terminal1[1]-tail[1])*(terminal2[2]-tail[2]))
		fsub		st(0), st(6)			;answer is in st(0)
		fstp		dword ptr[edx]
		////////////////////////////////////
		//normal[1]=((terminal1[2]-tail[2])*(terminal2[0]-tail[0]))
		//				-((terminal1[0]-tail[0])*(terminal2[2]-tail[2]));
		//Now do  ... -((terminal1[0]-tail[0])*(terminal2[2]-tail[2]));
		fld		dword ptr[ebx]
		fsub	st(0), st(3)				;(terminal1[2]-tail[2])
		fxch	st(6)						;put aside
		fstp	st(0)						;pop the stack
		fld		st(4)
		fsub	st(0), st(1)				;(terminal2[2]-tail[2]))
		fmulp	st(6), st(0)				;(terminal1[0]-tail[0])*(terminal2[2]-tail[2]))
		//Now do normal[1]=((terminal1[2]-tail[2])*(terminal2[0]-tail[0])) ...
		fld		dword ptr[ebx+8]
		fsub	st(0), st(1)				;terminal1[2]-tail[2])
		fxch	st(7)						;put aside
		fstp	st(0)						;pop the stack
		fld		dword ptr[eax]
		fsub	st(0), st(3)				;(terminal2[0]-tail[0])
		fmul	st(0), st(7)				;((terminal1[2]-tail[2])*(terminal2[0]-tail[0]))
		fsub	st(0), st(6)				;answer is in st(0)
		fstp	dword ptr[edx+4]			;edx already contains the address
		/////////////////////////////////////
		//normal[2]=((terminal1[0]-tail[0])*(terminal2[1]-tail[1]))
		//		-((terminal1[1]-tail[1])*(terminal2[0]-tail[0]));
		//Now do  ... -((terminal1[1]-tail[1])*(terminal2[0]-tail[0]));
		fld		st(3)
		fsub	st(0), st(2)				;(terminal1[1]-tail[1])
		fxch	st(6)						;put aside
		fstp	st(0)						;pop the stack
		fld		dword ptr[eax]
		fsub	st(0), st(3)				;(terminal2[0]-tail[0])
		fmulp	st(6), st(0)				;((terminal1[1]-tail[1])*(terminal2[0]-tail[0]))
		//Now do normal[2]=((terminal1[0]-tail[0])*(terminal2[1]-tail[1])) ...
		fld		dword ptr[ebx]
		fsub	st(0), st(3)				;(terminal1[0]-tail[0])
		fxch	st(7)						;put aside
		fstp	st(0)						;pop the stack
		fld		dword ptr[eax+4]
		fsub	st(0), st(2)				;(terminal2[0]-tail[0])
		fmul	st(0), st(7)				;((terminal1[0]-tail[0])*(terminal2[1]-tail[1]))
		fsub	st(0), st(6)				;answer is in st(0)
		fstp	dword ptr[edx+8]			;edx already contains the address
		finit								;we need to clear (reset) the FPU
		//This completes the cross product calculation.
	}*/
}

inline void ComputeNormalOfPlaneDOUBLE_3(sreal2 *normal, const sreal2 *terminal1, const sreal2 *terminal2, const sreal2 *tail)
{
	normal[0]=((terminal1[1]-tail[1])*(terminal2[2]-tail[2]))-((terminal1[2]-tail[2])*(terminal2[1]-tail[1]));
	normal[1]=((terminal1[2]-tail[2])*(terminal2[0]-tail[0]))-((terminal1[0]-tail[0])*(terminal2[2]-tail[2]));
	normal[2]=((terminal1[0]-tail[0])*(terminal2[1]-tail[1]))-((terminal1[1]-tail[1])*(terminal2[0]-tail[0]));
}

//PURPOSE: Given 3 points, compute the normal vector.
//Same as above, but one array holds all the points.
//The second point is the tail for both vectors.
inline void ComputeNormalOfPlaneFLOAT_1(sreal *normal, const sreal *pointarray)
{
	normal[0]=((pointarray[1]-pointarray[4])*(pointarray[8]-pointarray[5]))-((pointarray[2]-pointarray[5])*(pointarray[7]-pointarray[4]));
	normal[1]=((pointarray[2]-pointarray[5])*(pointarray[6]-pointarray[3]))-((pointarray[0]-pointarray[3])*(pointarray[8]-pointarray[5]));
	normal[2]=((pointarray[0]-pointarray[3])*(pointarray[7]-pointarray[4]))-((pointarray[1]-pointarray[4])*(pointarray[6]-pointarray[3]));
}
inline void ComputeNormalOfPlaneDOUBLE_1(sreal2 *normal, const sreal2 *pointarray)
{
	normal[0]=((pointarray[1]-pointarray[4])*(pointarray[8]-pointarray[5]))-((pointarray[2]-pointarray[5])*(pointarray[7]-pointarray[4]));
	normal[1]=((pointarray[2]-pointarray[5])*(pointarray[6]-pointarray[3]))-((pointarray[0]-pointarray[3])*(pointarray[8]-pointarray[5]));
	normal[2]=((pointarray[0]-pointarray[3])*(pointarray[7]-pointarray[4]))-((pointarray[1]-pointarray[4])*(pointarray[6]-pointarray[3]));
}

//2 pvectors are passed and normal is computed
inline void ComputeNormalOfPlaneFLOAT_2(sreal *normal, const sreal *pvector1, const sreal *pvector2)
{
	normal[0]=(pvector1[1]*pvector2[2])-(pvector1[2]*pvector2[1]);
	normal[1]=(pvector1[2]*pvector2[0])-(pvector1[0]*pvector2[2]);
	normal[2]=(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0]);
	//VC++ is able to better optimize (when it is turned on of course)
	/*__asm
	{
		fld			[pvector1+8]
		fmul		[pvector2+4]
		fld			[pvector1+4]
		fmul		[pvector2+8]
		fsubp		st(1), st(0)
		fstp		[normal]
		fld			[pvector1]
		fmul		[pvector2+8]
		fld			[pvector1+8]
		fmul		[pvector2]
		fsubp		st(1), st(0)
		fstp		[normal+4]
		fld			[pvector1+4]
		fmul		[pvector2]
		fld			[pvector1]
		fmul		[pvector2+4]
		fsubp		st(1), st(0)
		fstp		[normal+8]
	}*/
	/*__asm
	{
		mov			eax, [normal]
		mov			ebx, [pvector1]
		mov			ecx, [pvector2]
		fld			[ebx+8]
		fmul		[ecx+4]
		fld			[ebx+4]
		fmul		[ecx+8]
		fsubp		st(1), st(0)
		fstp		[eax]
		fld			[ebx]
		fmul		[ecx+8]
		fld			[ebx+8]
		fmul		[ecx]
		fsubp		st(1), st(0)
		fstp		[eax+4]
		fld			[ebx+4]
		fmul		[ecx]
		fld			[ebx]
		fmul		[ecx+4]
		fsubp		st(1), st(0)
		fstp		[eax+8]
	}*/
	//OLD CODE - slower
	/*__asm
	{
		mov			eax, dword ptr[normal]
		mov			ebx, dword ptr[pvector1]
		mov			ecx, dword ptr[pvector2]
		fld			dword ptr[ebx]
		fld			dword ptr[ebx+4]
		fld			dword ptr[ebx+8]
		fld			dword ptr[ecx]
		fld			dword ptr[ecx+4]
		fld			dword ptr[ecx+8]
		fld			st(4)
		fmul		st(0), st(1)		;(pvector1[1]*pvector2[2])
		fld			st(4)
		fmul		st(0), st(3)		;(pvector1[2]*pvector2[1])
		fsubp		st(1), st(0)		;(pvector1[1]*pvector2[2])-(pvector1[2]*pvector2[1])
		fstp		dword ptr[eax]		;normal[0]
		///////////////////////
		fld			st(3)
		fmul		st(0), st(3)		;(pvector1[2]*pvector2[0])
		fld			st(6)
		fmul		st(0), st(2)		;(pvector1[0]*pvector2[2])
		fsubp		st(1), st(0)		;(pvector1[2]*pvector2[0])-(pvector1[0]*pvector2[2])
		fstp		dword ptr[eax+4]	;normal[1]
		//////////////////////
		fld			st(5)
		fmul		st(0), st(2)		;(pvector1[0]*pvector2[1])
		fld			st(5)
		fmul		st(0), st(4)		;(pvector1[1]*pvector2[0])
		fsubp		st(1), st(0)		;(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0])
		fstp		dword ptr[eax+8]	;normal[2]
		finit
	}*/
}
inline void ComputeNormalOfPlaneDOUBLE_2(sreal2 *normal, const sreal2 *pvector1, const sreal2 *pvector2)
{
	normal[0]=(pvector1[1]*pvector2[2])-(pvector1[2]*pvector2[1]);
	normal[1]=(pvector1[2]*pvector2[0])-(pvector1[0]*pvector2[2]);
	normal[2]=(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0]);
}

//The following is a test code. It is wrong. I don't knwo how to compute the
//cross product in 4D
inline void ComputeNormal4DOfPlaneFLOAT_2(sreal *normal, const sreal *pvector1, const sreal *pvector2)
{
	normal[0]=(pvector1[1]*pvector2[2])-(pvector1[2]*pvector2[1]);
	normal[1]=(pvector1[2]*pvector2[0])-(pvector1[0]*pvector2[2]);
	normal[2]=(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0]);
	normal[3]=(pvector1[0]*pvector2[1])-(pvector1[1]*pvector2[0]);
}
/////////////////////////////////////////////////////////////////////////////
inline void ComputeNormalOfPlaneFLOAT_4(sreal *normal, const sreal *terminal1, const sreal *tail1, const sreal *terminal2, const sreal *tail2)
{
	normal[0]=((terminal1[1]-tail1[1])*(terminal2[2]-tail2[2]))-((terminal1[2]-tail1[2])*(terminal2[1]-tail2[1]));
	normal[1]=((terminal1[2]-tail1[2])*(terminal2[0]-tail2[0]))-((terminal1[0]-tail1[0])*(terminal2[2]-tail2[2]));
	normal[2]=((terminal1[0]-tail1[0])*(terminal2[1]-tail2[1]))-((terminal1[1]-tail1[1])*(terminal2[0]-tail2[0]));
}
/////////////////////////////////////////////////////////////////////////////
//PURPOSE: Normalize a vector: x, y, z
//         The normalizingConstant is just a memory area to be used.
inline void NormalizeVectorDOUBLE_1(sreal2 *pvector, sreal2 &normalizingConstant)
{
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
}

inline void NormalizeVectorFLOAT_1(sreal *pvector, sreal &normalizingConstant)
{
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
}

inline void NormalizeVectorDOUBLE_2(sreal2 *pvector)
{
	sreal2 normalizingConstant;
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
}

inline void NormalizeVectorFLOAT_2(sreal *pvector)
{
	sreal normalizingConstant;
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
}

//If you want to have the normalizingConstant as well
inline void NormalizeVector2DDOUBLE_1(sreal2 *pvector, sreal2 &normalizingConstant)
{
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
}

inline void NormalizeVector2DFLOAT_1(sreal *pvector, sreal &normalizingConstant)
{
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
}

inline void NormalizeVector2DDOUBLE_2(sreal2 *pvector)
{
	sreal2 normalizingConstant;
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
}


inline void NormalizeVector2DFLOAT_2(sreal *pvector)
{
	sreal normalizingConstant;
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
}


inline void NormalizeVector4DDOUBLE_1(sreal2 *pvector, sreal2 &normalizingConstant)
{
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]+pvector[3]*pvector[3]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
	pvector[3]*=normalizingConstant;
}

inline void NormalizeVector4DFLOAT_1(sreal *pvector, sreal &normalizingConstant)
{
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]+pvector[3]*pvector[3]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
	pvector[3]*=normalizingConstant;
}

//This can be used to normalize a plane equation.
inline void NormalizeVector4DDOUBLE_2(sreal2 *pvector)
{
	sreal2 normalizingConstant;
	normalizingConstant=1.0/sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]+pvector[3]*pvector[3]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
	pvector[3]*=normalizingConstant;
}
inline void NormalizeVector4DFLOAT_2(sreal *pvector)
{
	sreal normalizingConstant;
	normalizingConstant=1.0/sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]+pvector[3]*pvector[3]);
	pvector[0]*=normalizingConstant;
	pvector[1]*=normalizingConstant;
	pvector[2]*=normalizingConstant;
	pvector[3]*=normalizingConstant;
}


//totalpvector is the number of vectors * 4
inline void NormalizeVector4DFLOAT_2(sint totalpvector, sreal *pvector)
{
	sint i;
	sreal normalizingConstant;

	for(i=0; i<totalpvector; i+=4)
	{
		normalizingConstant=1.0/sqrtf(pvector[i]*pvector[i]+pvector[i+1]*pvector[i+1]+pvector[i+2]*pvector[i+2]+pvector[i+3]*pvector[i+3]);
		pvector[i  ]*=normalizingConstant;
		pvector[i+1]*=normalizingConstant;
		pvector[i+2]*=normalizingConstant;
		pvector[i+3]*=normalizingConstant;
	}
}

//pvector should be XYZW where W is 0.0
//A little bit faster than NormalizeVectorFLOAT_2, but not much
inline void NormalizeVector4DFLOAT_SSE_Aligned_2(sreal *pvector)
{
#if !USE_INLINE_ASM
	NormalizeVector4DFLOAT_2(1, pvector);
#else
	__asm
	{
		mov		ecx, pvector

		movaps	xmm0, [ecx]
		movaps	xmm2, xmm0			; Backup into xmm2
		mulps	xmm0, [ecx]
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 0x4E
		addps	xmm0, xmm1
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 0x11
		addps	xmm0, xmm1			; The vector dot product
		rsqrtps	xmm0, xmm0			; Compute 1/sqrt(...) This is a fast approximation with onchip lookup
		mulps	xmm2, xmm0
		movaps	[ecx], xmm2
	}
#endif
}

//totalpvector is the number of vectors * 4
//Using cache warmup and special SSE instructions, we should be able to quickly normalize vectors
//A little over 3 times faster than calling NormalizeVectorFLOAT_2 many times
inline void NormalizeVector4DFLOAT_SSE_Aligned_WarmCache_2(sint totalpvector, sreal *pvector)
{
#if !USE_INLINE_ASM
	NormalizeVector4DFLOAT_2(totalpvector, pvector);
#else
	__asm
	{
		mov		edx, totalpvector
		mov		eax, pvector
		add		edx, eax
		mov		ecx, eax

		cmp		ecx, edx
		jge		Terminal72

Restart_Norm_662:
		//Cache to L1
		prefetchnta	[ecx]			; Cache the vector (4*4 = 32 bytes)

		movaps	xmm0, [ecx]
		movaps	xmm2, xmm0			; Backup into xmm2
		mulps	xmm0, [ecx]
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 0x4E
		addps	xmm0, xmm1
		movaps	xmm1, xmm0
		shufps	xmm1, xmm1, 0x11
		addps	xmm0, xmm1			; The vector dot product
		rsqrtps	xmm0, xmm0			; Compute 1/sqrt(...) This is a fast approximation with onchip lookup
		mulps	xmm2, xmm0
		movaps	[ecx], xmm2

		add		ecx, 32				; Next vector
		cmp		ecx, edx
		jl		Restart_Norm_662

Terminal72:
	}
#endif
}

////////////////////////////////////////////////////////////////////
//PURPOSE:	Find the distance between a point and a plane or two parallel planes
//			point is defined by P0(x0, y0, z0)
//			equation of a plane is A*x+B*y+C*z+D=0 and normal pvector is (A, B, C)
//			||n||=sqrt(A^2+B^2+C^2)
//			By orthogonal projection onto normal pvector n
//			D=||proj(n)pvector(QP)||=|pvector(QP) * pvector(n)| / ||n||
//			D=|A*x0+B*y0+C*z0+D| / ||n||
//			point is an array of consecutive memory: x, y, z
//
//NOTE:		This is good if the normal of the plane equation is not a unit vector.
//			See also DistanceBetweenPointAndPlaneFLOAT_2, DistanceBetweenPointAndPlaneDOUBLE_2.
//			To normalize the plane, call NormalizeVector4DFLOAT_1, NormalizeVector4DDOUBLE_1.
inline void DistanceBetweenPointAndPlaneFLOAT_1(sreal *distance, const sreal *point, const sreal *planeEquation)
{
	*distance=fabsf(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3])/sqrtf(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);
}
inline void DistanceBetweenPointAndPlaneDOUBLE_1(sreal2 *distance, const sreal2 *point, const sreal2 *planeEquation)
{
	*distance=fabs(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3])/sqrt(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);
}

inline void SignedDistanceBetweenPointAndPlaneFLOAT_1(sreal *distance, const sreal *point, const sreal *planeEquation)
{
	*distance=(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3])/sqrtf(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);
}
inline void SignedDistanceBetweenPointAndPlaneDOUBLE_1(sreal2 *distance, const sreal2 *point, const sreal2 *planeEquation)
{
	*distance=(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3])/sqrt(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);
}

//PURPOSE:	Find the distance between a point and a plane or two parallel planes
//			point is defined by P0(x0, y0, z0)
//			equation of a plane is A*x+B*y+C*z+D=0 and normal pvector is (A, B, C)
//			||n||=sqrt(A^2+B^2+C^2)
//			By orthogonal projection onto normal pvector n
//			D=||proj(n)pvector(QP)||=|pvector(QP) * pvector(n)| / ||n||
//			D=|A*x0+B*y0+C*z0+D| / ||n||
//			point is an array of consecutive memory: x, y, z
//			**** We assume plane equation has been normalized. See DistanceBetweenPointAndPlaneFLOAT_1, DistanceBetweenPointAndPlaneDOUBLE_1
inline void DistanceBetweenPointAndPlaneFLOAT_2(sreal *distance, const sreal *point, const sreal *planeEquation)
{
	*distance=fabsf(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3]);
}
inline void DistanceBetweenPointAndPlaneDOUBLE_2(sreal2 *distance, const sreal2 *point, const sreal2 *planeEquation)
{
	*distance=fabs(planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3]);
}

inline void SignedDistanceBetweenPointAndPlaneFLOAT_2(sreal *distance, const sreal *point, const sreal *planeEquation)
{
	*distance=planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3];
}
inline void SignedDistanceBetweenPointAndPlaneDOUBLE_2(sreal2 *distance, const sreal2 *point, const sreal2 *planeEquation)
{
	*distance=planeEquation[0]*point[0]+planeEquation[1]*point[1]+planeEquation[2]*point[2]+planeEquation[3];
}


//PURPOSE:	Find the point on plane that is closest to a point.
//			point is an array of consecutive memory: x, y, z
inline void FindPointOnPlaneClosestToPointFLOAT_1(sreal *closestPoint, const sreal *point, const sreal *planeEquation)
{
	sreal t;
	t=(-planeEquation[0]*point[0]-planeEquation[1]*point[1]-planeEquation[2]*point[2]-planeEquation[3])/(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);

	closestPoint[0]=point[0]+t*planeEquation[0];
	closestPoint[1]=point[1]+t*planeEquation[1];
	closestPoint[2]=point[2]+t*planeEquation[2];
}
//This gives oportunity to optimize a little
//normalDOTnormal=(planeEquation[0]*planeEquation[0]+planeEquation[1]*planeEquation[1]+planeEquation[2]*planeEquation[2]);
inline void FindPointOnPlaneClosestToPointFLOAT_2(sreal *closestPoint, const sreal *point, const sreal *planeEquation, const sreal normalDOTnormal)
{
	sreal t;
	t=(-planeEquation[0]*point[0]-planeEquation[1]*point[1]-planeEquation[2]*point[2]-planeEquation[3])/normalDOTnormal;

	closestPoint[0]=point[0]+t*planeEquation[0];
	closestPoint[1]=point[1]+t*planeEquation[1];
	closestPoint[2]=point[2]+t*planeEquation[2];
}

//////////////////////////////////////////////////////////////////////////////
//PURPOSE: Project vector vector(u) along vector(a)
//           /
//          /   u
//         /  project(u)
//         --->----->  a
//         proj(a)vector(u) = ((vector(u) * vector(a)) / ||a||^2) * vector(a)
//         projection is an array of three consecutive memory: x, y, z
//         constant is this part ((vector(u) * vector(a)) / ||a||^2)
//         All arguments must be an array of three consecutive memory: x, y, z
inline void ProjectVectorAlongVectorFLOAT_2(sreal *projection, const sreal *vectorToProject, const sreal *alongVector)
{
	sreal constant;
	constant=(vectorToProject[0]*alongVector[0]+vectorToProject[1]*alongVector[1]+vectorToProject[2]*alongVector[2])/(alongVector[0]*alongVector[0]+alongVector[1]*alongVector[1]+alongVector[2]*alongVector[2]);
	projection[0]=constant*alongVector[0];
	projection[1]=constant*alongVector[1];
	projection[2]=constant*alongVector[2];
}
inline void ProjectVectorAlongVectorDOUBLE_2(sreal2 *projection, const sreal2 *vectorToProject, const sreal2 *alongVector)
{
	sreal2 constant;
	constant=(vectorToProject[0]*alongVector[0]+vectorToProject[1]*alongVector[1]+vectorToProject[2]*alongVector[2])/(alongVector[0]*alongVector[0]+alongVector[1]*alongVector[1]+alongVector[2]*alongVector[2]);
	projection[0]=constant*alongVector[0];
	projection[1]=constant*alongVector[1];
	projection[2]=constant*alongVector[2];
}

//These functions have a fourth argument:  the tail for the 2 vectors
inline void ProjectVectorAlongVectorFLOAT_3(sreal *projection, const sreal *vectorToProject, const sreal *alongVector, const sreal *tailVector)
{
	sreal constant, VectorToProject[3], AlongVector[3];
	VectorToProject[0]=vectorToProject[0]-tailVector[0];
	VectorToProject[1]=vectorToProject[1]-tailVector[1];
	VectorToProject[2]=vectorToProject[2]-tailVector[2];
	AlongVector[0]=alongVector[0]-tailVector[0];
	AlongVector[1]=alongVector[1]-tailVector[1];
	AlongVector[2]=alongVector[2]-tailVector[2];
	constant=(VectorToProject[0]*AlongVector[0]+VectorToProject[1]*AlongVector[1]+VectorToProject[2]*AlongVector[2])/(AlongVector[0]*AlongVector[0]+AlongVector[1]*AlongVector[1]+AlongVector[2]*AlongVector[2]);
	projection[0]=constant*AlongVector[0];
	projection[1]=constant*AlongVector[1];
	projection[2]=constant*AlongVector[2];
}
inline void ProjectVectorAlongVectorDOUBLE_3(sreal2 *projection, const sreal2 *vectorToProject, const sreal2 *alongVector, const sreal2 *tailVector)
{
	sreal2 constant, VectorToProject[3], AlongVector[3];
	VectorToProject[0]=vectorToProject[0]-tailVector[0];
	VectorToProject[1]=vectorToProject[1]-tailVector[1];
	VectorToProject[2]=vectorToProject[2]-tailVector[2];
	AlongVector[0]=alongVector[0]-tailVector[0];
	AlongVector[1]=alongVector[1]-tailVector[1];
	AlongVector[2]=alongVector[2]-tailVector[2];
	constant=(VectorToProject[0]*AlongVector[0]+VectorToProject[1]*AlongVector[1]+VectorToProject[2]*AlongVector[2])/(AlongVector[0]*AlongVector[0]+AlongVector[1]*AlongVector[1]+AlongVector[2]*AlongVector[2]);
	projection[0]=constant*AlongVector[0];
	projection[1]=constant*AlongVector[1];
	projection[2]=constant*AlongVector[2];
}

//////////////////////////////////////////////////////////////////////////////
//PURPOSE: pvector component of pvector(u) orthogonal to pvector(a)
//ortho(u) | /
//         |/   u
//         /
//         --------->  a
//         pvector(u) - [proj(a)pvector(u)] = pvector(u) - [((u * a) / ||a||^2) * pvector(a)]
//         orthoproj is an array of three consecutive memory: x, y, z
//         constant is this part ((pvector(u) * pvector(a)) / ||a||^2)
//         All arguments must be an array of three consecutive memory: x, y, z
//////////////////////////////////////////////////////////////////////////////
//PURPOSE: Vector component of vector(u) orthogonal to vector(a)
//ortho(u) | /
//         |/   u
//         /
//         --------->  a
//         vector(u) - [proj(a)vector(u)] = vector(u) - [((u * a) / ||a||^2) * vector(a)]
//         orthoproj is an array of three consecutive memory: x, y, z
//         constant is this part ((vector(u) * vector(a)) / ||a||^2)
//         All arguments must be an array of three consecutive memory: x, y, z
inline void OrthoProjectVectorToVectorFLOAT_2(sreal *orthoProj, const sreal *vectorToProject, const sreal *toVector)
{
	sreal constant;
	constant=(vectorToProject[0]*toVector[0]+vectorToProject[1]*toVector[1]+vectorToProject[2]*toVector[2])/(toVector[0]*toVector[0]+toVector[1]*toVector[1]+toVector[2]*toVector[2]);
	orthoProj[0]=vectorToProject[0]-constant*toVector[0];
	orthoProj[1]=vectorToProject[1]-constant*toVector[1];
	orthoProj[2]=vectorToProject[2]-constant*toVector[2];
}
inline void OrthoProjectVectorToVectorDOUBLE_2(sreal2 *orthoProj, const sreal2 *vectorToProject, const sreal2 *toVector)
{
	sreal2 constant;
	constant=(vectorToProject[0]*toVector[0]+vectorToProject[1]*toVector[1]+vectorToProject[2]*toVector[2])/(toVector[0]*toVector[0]+toVector[1]*toVector[1]+toVector[2]*toVector[2]);
	orthoProj[0]=vectorToProject[0]-constant*toVector[0];
	orthoProj[1]=vectorToProject[1]-constant*toVector[1];
	orthoProj[2]=vectorToProject[2]-constant*toVector[2];
}

//These functions have a fourth argument:  the tail for the 2 vectors
inline void OrthoProjectVectorToVectorFLOAT_3(sreal *orthoProj, const sreal *vectorToProject, const sreal *toVector, const sreal *tailVector)
{
	sreal constant, VectorToProject[3], ToVector[3];
	VectorToProject[0]=vectorToProject[0]-tailVector[0];
	VectorToProject[1]=vectorToProject[1]-tailVector[1];
	VectorToProject[2]=vectorToProject[2]-tailVector[2];
	ToVector[0]=toVector[0]-tailVector[0];
	ToVector[1]=toVector[1]-tailVector[1];
	ToVector[2]=toVector[2]-tailVector[2];
	constant=(VectorToProject[0]*ToVector[0]+VectorToProject[1]*ToVector[1]+VectorToProject[2]*ToVector[2])/(ToVector[0]*ToVector[0]+ToVector[1]*ToVector[1]+ToVector[2]*ToVector[2]);
	orthoProj[0]=VectorToProject[0]-constant*ToVector[0];
	orthoProj[1]=VectorToProject[1]-constant*ToVector[1];
	orthoProj[2]=VectorToProject[2]-constant*ToVector[2];
}
inline void OrthoProjectVectorToVectorDOUBLE_3(sreal2 *orthoProj, const sreal2 *vectorToProject, const sreal2 *toVector, const sreal2 *tailVector)
{
	sreal2 constant, VectorToProject[3], ToVector[3];
	VectorToProject[0]=vectorToProject[0]-tailVector[0];
	VectorToProject[1]=vectorToProject[1]-tailVector[1];
	VectorToProject[2]=vectorToProject[2]-tailVector[2];
	ToVector[0]=toVector[0]-tailVector[0];
	ToVector[1]=toVector[1]-tailVector[1];
	ToVector[2]=toVector[2]-tailVector[2];
	constant=(VectorToProject[0]*ToVector[0]+VectorToProject[1]*ToVector[1]+VectorToProject[2]*ToVector[2])/(ToVector[0]*ToVector[0]+ToVector[1]*ToVector[1]+ToVector[2]*ToVector[2]);
	orthoProj[0]=VectorToProject[0]-constant*ToVector[0];
	orthoProj[1]=VectorToProject[1]-constant*ToVector[1];
	orthoProj[2]=VectorToProject[2]-constant*ToVector[2];
}

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//This section is for doing matrix math
/////////////////////////////////////////////////////////////////////////////

//PURPOSE: Create the transpose of matrix A, returns in matrix B.
//         The r is to mean row major (we go from left to right before moving down)
//         m by n matrix.
inline void TransposeOfMatrixFLOAT_1_r(const sreal *matrixA, sint m, sint n, sreal *matrixB)
{
	sint i, j;
	for(i=0; i<m; i++)
		for(j=0; j<n; j++)
			matrixB[j+i*n]=matrixA[i+j*m];
}

//PURPOSE: Compute the trace of a matrix. Matrix must be a square matrix. Trace
//         is obtained by summing the main diagonal of a square matrix.
//         m by n matrix.
inline void ComputeTraceOfMatrixFLOAT_1_r(const sreal *matrixA, sint m, sint n, sreal *traceValue)
{
	sint i, offset, size;
	*traceValue=0;
	if(m==n)
	{
		size=m*m;
		offset=m+1;
		i=0;
		while(i<size)
		{
			(*traceValue)+=matrixA[i];
			i+=offset;
		}
	}
}
inline void ComputeTraceOfMatrixDOUBLE_1_r(sreal2 *matrixA, sint m, sint n, sreal2 *traceValue)
{
	sint i, offset, size;
	*traceValue=0;
	if(m==n)
	{
		size=m*m;
		offset=m+1;
		i=0;
		while(i<size)
		{
			(*traceValue)+=matrixA[i];
			i+=offset;
		}
	}
}


//PURPOSE:	Rotate a point (x, y, z) about a vector by an angle (in radians)
//			pvector must a (x, y, z) and need not be normalized.
//			The result is written back to point
inline void RotatePointAboutVectorFLOAT_2(sreal *pvector, sreal angle, sreal *point)
{
	sreal point2[3], CosAngle=1.0;
	sreal Matrix[9], OneMinusCosAngle, SinAngle;
	point2[0]=point[0];
	point2[1]=point[1];
	point2[2]=point[2];
	//These three are for efficiency
	NormalizeVectorFLOAT_2(pvector);
	CosAngle=cosf(angle);
	OneMinusCosAngle=1.0-CosAngle;
	SinAngle=sinf(angle);
	//construct a rotation matrix (just like matrices in OpenGL : top to bottom, left to right)
	Matrix[0]=pvector[0]*pvector[0]*OneMinusCosAngle+CosAngle;
	Matrix[1]=pvector[0]*pvector[1]*OneMinusCosAngle+pvector[2]*SinAngle;
	Matrix[2]=pvector[0]*pvector[2]*OneMinusCosAngle-pvector[1]*SinAngle;
	Matrix[3]=pvector[0]*pvector[1]*OneMinusCosAngle-pvector[2]*SinAngle;
	Matrix[4]=pvector[1]*pvector[1]*OneMinusCosAngle+CosAngle;
	Matrix[5]=pvector[1]*pvector[2]*OneMinusCosAngle+pvector[0]*SinAngle;
	Matrix[6]=pvector[0]*pvector[2]*OneMinusCosAngle+pvector[1]*SinAngle;
	Matrix[7]=pvector[1]*pvector[2]*OneMinusCosAngle-pvector[0]*SinAngle;
	Matrix[8]=pvector[2]*pvector[2]*OneMinusCosAngle+CosAngle;
	//Now compute the new point	
	point[0]=Matrix[0]*point2[0]+Matrix[3]*point2[1]+Matrix[6]*point2[2];
	point[1]=Matrix[1]*point2[0]+Matrix[4]*point2[1]+Matrix[7]*point2[2];
	point[2]=Matrix[2]*point2[0]+Matrix[5]*point2[1]+Matrix[8]*point2[2];
	return;
	/*__asm
	{
		//Notice that in the C/C++ code pvector[0]*OneMinusCosAngle
		//and pvector[1]*OneMinusCosAngle and pvector[2]*OneMinusCosAngle
		//are used often so we can reduce # of instruction.
		//Same for pvector[0]*SinAngle and pvector[1]*SinAngle and
		//pvector[2]*SinAngle
		//Unfortunatly not enough FPU registers!!!!!!!!!
		mov		esi, [point]
		lea		edi, point2
		mov		eax, [esi]
		mov		[edi], eax
		mov		eax, [esi+4]
		mov		[edi+4], eax
		mov		eax, [esi+8]		; now we have a copy of point[]
		mov		[edi+8], eax
		//We do not store anything into Matrix
		//We compute in one shot the new point values and store them immediatly
		//But the problem is we need 9 registers + 1 work register so ...
		mov		eax, esi		; save esi
		mov		esi, pvector
		mov		edx, esi
		fld		[esi]			; pvector[0]
		//fld		[esi+4]			; pvector[1] not enough registers!
		fld		[esi+8]			; pvector[2]
		fld		angle
		fld		st(0)			; make a copy of angle
		fcos					; cos(angle)
		fld		CosAngle		; load 1.0
		fsub	st(0), st(1)	; 1-cos(angle)
		fxch	st(2)
		fsin					; now st(0)=sin(angle) st(1)=1-cos(angle) st(2)=cos(angle)
		fld		st(4)
		fmul	st(0), st(5)
		fmul	st(0), st(2)
		fadd	st(0), st(3)	; Matrix[0] is in st(0) now
		fld		[edi]
		fmulp	st(1), st(0)
		fld		[esi+4]			; pvector[1]
		fmul	st(0), st(6)
		fmul	st(0), st(3)
		fld		st(6)			; All registers are in use now
		fmul	st(0), st(3)
		fsubp	st(1), st(0)	; Matrix[3] is in st(0) now
		fld		[edi+4]			; point[2], now all the FPU registers are used!
		fmulp	st(1), st(0)
		faddp	st(1), st(0)	; Matrix[0]*point2[0]+Matrix[3]*point2[1] in st(0)
		fld		st(5)
		fmul	st(0), st(5)
		fmul	st(0), st(3)
		fld		[esi+4]			; pvector[1], now all registers are used
		fmul	st(0), st(3)
		faddp	st(1), st(0)	; Matrix[6] is in st(0) nwo
		fld		[edi+8]			; point2[2], now all registers are in use
		fmulp	st(1), st(0)
		faddp	st(1), st(0)	; this is new point[0]
		mov		esi, eax
		fstp	[esi]			; copy to point[0]
		mov		esi, edx
		fld		[esi+4]		; pvector[1]
		fmul	st(0), st(5)
		fmul	st(0), st(2)
		fld		st(4)
		fmul	st(0), st(2)
		faddp	st(1), st(0)	; Matrix[1] is in st(0) now
		fld		[edi]			; point2[0]
		fmulp	st(1), st(0)
		fld		[esi+4]			; pvector[1]
		fmul	st(0), st(0)
		fmul	st(0), st(3)
		fadd	st(0), st(4)	; Matrix[4] is in st(0) now
		fld		[edi+4]			; point2[1]
		fmulp	st(1), st(0)
		faddp	st(1), st(0)	; Matrix[1]*point2[0]+Matrix[4]*point2[1] in st(0)
		fld		[edi+4]			; pvector[1]
		fmul	st(0), st(5)
		fmul	st(0), st(3)
		fld		st(6)			; All registers are used
		fmul	st(0), st(3)
		fsubp	st(1), st(0)	; Matrix[7] is in st(0) now
		fld		[edi+8]			; point2[2]
		fmulp	st(1), st(0)
		faddp	st(1), st(0)	; this is new point[1]
		mov		esi, eax
		fstp	[esi+4]
		mov		esi, edx
		fld		st(4)
		fmul	st(0), st(4)
		fmul	st(0), st(2)
		fld		[esi+4]			; pvector[1]
		fmul	st(0), st(2)
		fsubp	st(1), st(0)	; Matrix[2] is in st(0) now
		fld		[edi]			; point2[0]
		fmulp	st(1), st(0)	; Matrix[2]*point2[0]
		fld		[esi+4]			; pvector[1]
		fmul	st(0), st(5)
		fmul	st(0), st(3)
		fld		st(6)			; pvector[0], all registers are in use
		fmul	st(0), st(3)
		faddp	st(1), st(0)	; Matrix[5] is in st(0) now
		fld		[edi+4]			; point2[1]
		fmulp	st(1), st(0)	; Matrix[5]*point2[1]
		faddp	st(1), st(0)	; Matrix[2]*point2[0]+Matrix[5]*point2[1]
		fld		st(4)
		fmul	st(0), st(5)
		fmul	st(0), st(3)
		fadd	st(0), st(4)	; Matrix[8] is in st(0) now
		fld		[edi+8]			; point2[2]
		fmulp	st(1), st(0)
		faddp	st(1), st(0)	; this is new point[2]
		mov		esi, eax
		fstp	[esi+8]
		finit					; Clear the FPU stack
	}*/
}

//PURPOSE: Counterclockwise rotation of point about x axis.
inline void RotatePoint3DAboutXAxisFLOAT_1(sreal angle, sreal *point3D)
{
	sreal y, z;
	y=point3D[1];
	z=point3D[2];
	point3D[1]=y*cosf(angle)-z*sinf(angle);
	point3D[2]=y*sinf(angle)+z*cosf(angle);
}

//PURPOSE: Counterclockwise rotation of point about y axis.
inline void RotatePoint3DAboutYAxisFLOAT_1(sreal angle, sreal *point3D)
{
	sreal x, z;
	x=point3D[0];
	z=point3D[2];
	point3D[0]=x*cosf(angle)+z*sinf(angle);
	point3D[2]=z*cosf(angle)-x*sinf(angle);
}

//PURPOSE: Counterclockwise rotation of point about z axis.
inline void RotatePoint3DAboutZAxisFLOAT_1(sreal angle, sreal *point3D)
{
	sreal x, y;
	x=point3D[0];
	y=point3D[1];
	point3D[0]=x*cosf(angle)-y*sinf(angle);
	point3D[1]=x*sinf(angle)+y*cosf(angle);
}

//PURPOSE: Rotate a vector to a new orientation in 3D space. Orientation vectors need not be normalized.
//         WARNING: Can't do rotation if newOrientation and oldOrientation are PI to each other!!!!!
//         the angle between them I mean. Check out RotateToOrientationVectorFLOAT_3 for this purpose.
inline void RotateToOrientationVectorFLOAT_3(sreal *newOrientationVector, sreal *oldOrientationVector, sreal *pvector)
{
	//First normalize orientation.
	NormalizeVectorFLOAT_2(newOrientationVector);
	NormalizeVectorFLOAT_2(oldOrientationVector);
	//Orientation vector must be different
	if((fabsf(oldOrientationVector[0]-newOrientationVector[0])>1.0e-6)||
		(fabs(oldOrientationVector[1]-newOrientationVector[1])>1.0e-6)||
		(fabs(oldOrientationVector[2]-newOrientationVector[2])>1.0e-6))
	{
		//Use all of the above to compute angle between vectors
		sreal CrossProduct[3];
		sreal CosAngle=oldOrientationVector[0]*newOrientationVector[0]+oldOrientationVector[1]*newOrientationVector[1]+oldOrientationVector[2]*newOrientationVector[2];
		sreal Angle=acosf(CosAngle);
		//Compute the cross product vector, to be used to rotate around
		CrossProduct[0]=oldOrientationVector[1]*newOrientationVector[2]-oldOrientationVector[2]*newOrientationVector[1];
		CrossProduct[1]=oldOrientationVector[2]*newOrientationVector[0]-oldOrientationVector[0]*newOrientationVector[2];
		CrossProduct[2]=oldOrientationVector[0]*newOrientationVector[1]-oldOrientationVector[1]*newOrientationVector[0];
		//Almost zero (due to error) or zero
		if((CrossProduct[0]>=-1.0e-6)&&(CrossProduct[0]<=1.0e-6)&&
			(CrossProduct[1]>=-1.0e-6)&&(CrossProduct[1]<=1.0e-6)&&
			(CrossProduct[2]>=-1.0e-6)&&(CrossProduct[2]<=1.0e-6))
		{	//2 cases when cross product gives NULL: either vectors are in same direction or in
			//opposite direction. For sure they are opposite and angle between them is PI.
			//LEAVE CROSSPRODUCT AS IS
			//CosAngle=-1.0;				//cos(PI)=-1
			//Angle=3.1415926535897;		//PI
			//CAN'T be done!!!!!!!!!
			return;
		}
		else
		{
			//CrossProduct vector must be a unit vector!!!
			NormalizeVectorFLOAT_2(CrossProduct);
		}
		//These two are for efficiency
		sreal FTempo=1.0-CosAngle;
		sreal FTempo2=(sreal)sinf(Angle);
		//Construct a rotation matrix (just like matrices in OpenGL : top to bottom, left to right)
		sreal Matrix[9];
		Matrix[0]=CrossProduct[0]*CrossProduct[0]*FTempo+CosAngle;
		Matrix[1]=CrossProduct[0]*CrossProduct[1]*FTempo+CrossProduct[2]*FTempo2;
		Matrix[2]=CrossProduct[0]*CrossProduct[2]*FTempo-CrossProduct[1]*FTempo2;
		Matrix[3]=CrossProduct[0]*CrossProduct[1]*FTempo-CrossProduct[2]*FTempo2;
		Matrix[4]=CrossProduct[1]*CrossProduct[1]*FTempo+CosAngle;
		Matrix[5]=CrossProduct[1]*CrossProduct[2]*FTempo+CrossProduct[0]*FTempo2;
		Matrix[6]=CrossProduct[0]*CrossProduct[2]*FTempo+CrossProduct[1]*FTempo2;
		Matrix[7]=CrossProduct[1]*CrossProduct[2]*FTempo-CrossProduct[0]*FTempo2;
		Matrix[8]=CrossProduct[2]*CrossProduct[2]*FTempo+CosAngle;
		//Now do the actual rotation
		//MultiplyMatrixByVector3by3OpenGL_FLOAT_555A(Matrix, vector);
		//CANT COMPILE WITH THE ABOVE LINE SO I COPIED THE CODE HERE
		FTempo=pvector[0];				//Must save these while computing!!!
		FTempo2=pvector[1];
		sreal FTempo3=pvector[2];
		pvector[0]=Matrix[0]*FTempo+Matrix[3]*FTempo2+Matrix[6]*FTempo3;
		pvector[1]=Matrix[1]*FTempo+Matrix[4]*FTempo2+Matrix[7]*FTempo3;
		pvector[2]=Matrix[2]*FTempo+Matrix[5]*FTempo2+Matrix[8]*FTempo3;
	}
}

//PURPOSE: Rotate a vector to a new orientation in 3D space. Orientation vectors need not be normalized.
//         vectorOfRotation is the vector around which we will rotate only when angle between both orientation vectors is PI.
inline void RotateToOrientationVectorFLOAT_4(sreal *newOrientationVector, sreal *oldOrientationVector, sreal *pvector, sreal *vectorOfRotation)
{
	//First normalize orientation.
	NormalizeVectorFLOAT_2(newOrientationVector);
	NormalizeVectorFLOAT_2(oldOrientationVector);
	//Orientation vector must be different
	if((fabsf(oldOrientationVector[0]-newOrientationVector[0])>1.0e-6)||
		(fabs(oldOrientationVector[1]-newOrientationVector[1])>1.0e-6)||
		(fabs(oldOrientationVector[2]-newOrientationVector[2])>1.0e-6))
	{
		//Use all of the above to compute angle between vectors
		sreal CrossProduct[3];
		sreal CosAngle=oldOrientationVector[0]*newOrientationVector[0]+oldOrientationVector[1]*newOrientationVector[1]+oldOrientationVector[2]*newOrientationVector[2];
		sreal Angle=acosf(CosAngle);
		//Compute the cross product vector, to be used to rotate around
		CrossProduct[0]=oldOrientationVector[1]*newOrientationVector[2]-oldOrientationVector[2]*newOrientationVector[1];
		CrossProduct[1]=oldOrientationVector[2]*newOrientationVector[0]-oldOrientationVector[0]*newOrientationVector[2];
		CrossProduct[2]=oldOrientationVector[0]*newOrientationVector[1]-oldOrientationVector[1]*newOrientationVector[0];
		//Almost zero or zero
		if((CrossProduct[0]>=-1.0e-6)&&(CrossProduct[0]<=1.0e-6)&&
			(CrossProduct[1]>=-1.0e-6)&&(CrossProduct[1]<=1.0e-6)&&
			(CrossProduct[2]>=-1.0e-6)&&(CrossProduct[2]<=1.0e-6))
		{	//2 cases when cross product gives NULL: either vectors are in same direction or in
			//opposite direction. For sure they are opposite and angle between them is PI.
			CrossProduct[0]=vectorOfRotation[0];
			CrossProduct[1]=vectorOfRotation[1];
			CrossProduct[2]=vectorOfRotation[2];
			CosAngle=-1.0;				//cos(PI)=-1
			Angle=3.1415926535897;		//PI
		}
		else
		{
			//CrossProduct vector must be a unit vector!!!
			NormalizeVectorFLOAT_2(CrossProduct);
		}
		//These two are for efficiency
		sreal FTempo=1.0-CosAngle;
		sreal FTempo2=(sreal)sinf(Angle);
		//Construct a rotation matrix (just like matrices in OpenGL : top to bottom, left to right)
		sreal Matrix[9];
		Matrix[0]=CrossProduct[0]*CrossProduct[0]*FTempo+CosAngle;
		Matrix[1]=CrossProduct[0]*CrossProduct[1]*FTempo+CrossProduct[2]*FTempo2;
		Matrix[2]=CrossProduct[0]*CrossProduct[2]*FTempo-CrossProduct[1]*FTempo2;
		Matrix[3]=CrossProduct[0]*CrossProduct[1]*FTempo-CrossProduct[2]*FTempo2;
		Matrix[4]=CrossProduct[1]*CrossProduct[1]*FTempo+CosAngle;
		Matrix[5]=CrossProduct[1]*CrossProduct[2]*FTempo+CrossProduct[0]*FTempo2;
		Matrix[6]=CrossProduct[0]*CrossProduct[2]*FTempo+CrossProduct[1]*FTempo2;
		Matrix[7]=CrossProduct[1]*CrossProduct[2]*FTempo-CrossProduct[0]*FTempo2;
		Matrix[8]=CrossProduct[2]*CrossProduct[2]*FTempo+CosAngle;
		//Now do the actual rotation
		//MultiplyMatrixByVector3by3OpenGL_FLOAT_555A(Matrix, vector);
		//CANT COMPILE WITH THE ABOVE LINE SO I COPIED THE CODE HERE
		FTempo=pvector[0];				//Must save these while computing!!!
		FTempo2=pvector[1];
		sreal FTempo3=pvector[2];
		pvector[0]=Matrix[0]*FTempo+Matrix[3]*FTempo2+Matrix[6]*FTempo3;
		pvector[1]=Matrix[1]*FTempo+Matrix[4]*FTempo2+Matrix[7]*FTempo3;
		pvector[2]=Matrix[2]*FTempo+Matrix[5]*FTempo2+Matrix[8]*FTempo3;
	}
}

//PURPOSE: Compute the angle between 2 vectors with starting points.
//         Angle=(vectorU DOT vectorV) / (||vectorU|| * ||vectorV||)
inline void ComputeAngleBetweenTwoVectorFLOAT_2(const sreal *pvector1, const sreal *pvector2, sreal *angle)
{
	*angle=acosf((pvector1[0]*pvector2[0]+pvector1[1]*pvector2[1]+pvector1[2]*pvector2[2])/(sqrtf(pvector1[0]*pvector1[0]+pvector1[1]*pvector1[1]+pvector1[2]*pvector1[2])*sqrtf(pvector2[0]*pvector2[0]+pvector2[1]*pvector2[1]+pvector2[2]*pvector2[2])));
}
inline void ComputeAngleBetweenTwoVectorDOUBLE_2(const sreal2 *pvector1, const sreal2 *pvector2, sreal2 *angle)
{
	*angle=acos((pvector1[0]*pvector2[0]+pvector1[1]*pvector2[1]+pvector1[2]*pvector2[2])/(sqrt(pvector1[0]*pvector1[0]+pvector1[1]*pvector1[1]+pvector1[2]*pvector1[2])*sqrt(pvector2[0]*pvector2[0]+pvector2[1]*pvector2[1]+pvector2[2]*pvector2[2])));
}

//PURPOSE: Compute the angle between 2 vectors with starting points.
//         Angle=(vectorU DOT vectorV) / (||vectorU|| * ||vectorV||)
//         This version returns the angle
inline sreal ComputeAngleBetweenTwoVectorFLOAT_2x(const sreal *pvector1, const sreal *pvector2)
{
	return acosf((pvector1[0]*pvector2[0]+pvector1[1]*pvector2[1]+pvector1[2]*pvector2[2])/(sqrtf(pvector1[0]*pvector1[0]+pvector1[1]*pvector1[1]+pvector1[2]*pvector1[2])*sqrtf(pvector2[0]*pvector2[0]+pvector2[1]*pvector2[1]+pvector2[2]*pvector2[2])));
}
inline sreal2 ComputeAngleBetweenTwoVectorDOUBLE_2x(const sreal2 *pvector1, const sreal2 *pvector2)
{
	return acos((pvector1[0]*pvector2[0]+pvector1[1]*pvector2[1]+pvector1[2]*pvector2[2])/(sqrt(pvector1[0]*pvector1[0]+pvector1[1]*pvector1[1]+pvector1[2]*pvector1[2])*sqrt(pvector2[0]*pvector2[0]+pvector2[1]*pvector2[1]+pvector2[2]*pvector2[2])));
}

//PURPOSE: Compute the angle between 2 vectors with starting points.
//         Angle=(vectorU DOT vectorV) / (||vectorU|| * ||vectorV||)
inline void ComputeAngleBetweenTwoVectorFLOAT_4(const sreal *terminal1, const sreal *tail1, const sreal *terminal2, const sreal *tail2, sreal *angle)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail1[0];
	pvectorU[1]=terminal1[1]-tail1[1];
	pvectorU[2]=terminal1[2]-tail1[2];
	pvectorV[0]=terminal2[0]-tail2[0];
	pvectorV[1]=terminal2[1]-tail2[1];
	pvectorV[2]=terminal2[2]-tail2[2];
	*angle=acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}

//PURPOSE: Compute the angle between 2 vectors with starting points.
//         Angle=(vectorU DOT vectorV) / (||vectorU|| * ||vectorV||)
//         This version will return the angle.
inline sreal ComputeAngleBetweenTwoVectorFLOAT_4x(const sreal *terminal1, const sreal *tail1, const sreal *terminal2, const sreal *tail2)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail1[0];
	pvectorU[1]=terminal1[1]-tail1[1];
	pvectorU[2]=terminal1[2]-tail1[2];
	pvectorV[0]=terminal2[0]-tail2[0];
	pvectorV[1]=terminal2[1]-tail2[1];
	pvectorV[2]=terminal2[2]-tail2[2];
	return acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}

//This version has the vectors with a common tail
inline void ComputeAngleBetweenTwoVectorFLOAT_3a(const sreal *terminal1, const sreal *terminal2, const sreal *tail, sreal *angle)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	pvectorV[2]=terminal2[2]-tail[2];
	*angle=acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}

//This version has the pvectors with a common tail and will return the angle
inline sreal ComputeAngleBetweenTwoVectorFLOAT_3x(const sreal *terminal1, const sreal *terminal2, const sreal *tail)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	pvectorV[2]=terminal2[2]-tail[2];
	return acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}
inline sreal2 ComputeAngleBetweenTwoVectorDOUBLE_3x(const sreal2 *terminal1, const sreal2 *terminal2, const sreal2 *tail)
{
	sreal2 pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	pvectorV[2]=terminal2[2]-tail[2];
	return acos((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrt(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrt(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}

//This version has the vectors with a common tail and will return the angle (x, y)
inline sreal ComputeAngleBetweenTwoVector2DFLOAT_3x(const sreal *terminal1, const sreal *terminal2, const sreal *tail)
{
	sreal pvectorU[2], pvectorV[2];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	return acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1])));
}
inline sreal2 ComputeAngleBetweenTwoVector2DDOUBLE_3x(const sreal2 *terminal1, const sreal2 *terminal2, const sreal2 *tail)
{
	sreal2 pvectorU[2], pvectorV[2];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	return acos((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1])/(sqrt(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1])*sqrt(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1])));
}

//This version has one vector based on (0, 0, 0) with a common tail.
inline void ComputeAngleBetweenTwoVectorFLOAT_3b(const sreal *terminal1, const sreal *tail, const sreal *pvectorV, sreal *angle)
{
	sreal pvectorU[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	*angle=acosf((pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2])/(sqrtf(pvectorU[0]*pvectorU[0]+pvectorU[1]*pvectorU[1]+pvectorU[2]*pvectorU[2])*sqrtf(pvectorV[0]*pvectorV[0]+pvectorV[1]*pvectorV[1]+pvectorV[2]*pvectorV[2])));
}

//PURPOSE: Compute the dot product between 2 vectors.
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline sreal ComputeDotProductFLOAT_2a(const sreal *pvectorU, const sreal *pvectorV)
{
	return pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

//PURPOSE: Compute the dot product between 2 vectors.
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline void ComputeDotProductFLOAT_2b(sreal *pvectorU, sreal *pvectorV, sreal *dotProduct)
{
	*dotProduct=pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

//PURPOSE: Compute the dot product between 2 vectors.
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline sreal ComputeDotProductFLOAT_3a(const sreal *terminal1, const sreal *tail, const sreal *terminal2)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	pvectorV[2]=terminal2[2]-tail[2];
	return pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

//PURPOSE: Compute the dot product between 2 vectors.
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline void ComputeDotProductFLOAT_3b(const sreal *terminal1, const sreal *tail, const sreal *terminal2, sreal *dotProduct)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail[0];
	pvectorU[1]=terminal1[1]-tail[1];
	pvectorU[2]=terminal1[2]-tail[2];
	pvectorV[0]=terminal2[0]-tail[0];
	pvectorV[1]=terminal2[1]-tail[1];
	pvectorV[2]=terminal2[2]-tail[2];
	*dotProduct=pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

//PURPOSE: Compute the dot product between 2 vectors. with a common tail
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline sreal ComputeDotProductFLOAT_4a(const sreal *terminal1, const sreal *tail1, const sreal *terminal2, const sreal *tail2)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail1[0];
	pvectorU[1]=terminal1[1]-tail1[1];
	pvectorU[2]=terminal1[2]-tail1[2];
	pvectorV[0]=terminal2[0]-tail2[0];
	pvectorV[1]=terminal2[1]-tail2[1];
	pvectorV[2]=terminal2[2]-tail2[2];
	return pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

//PURPOSE: Compute the dot product between 2 vectors. with a common tail
//         DotProduct=vectorU[0]*vectorV[0]+vectorU[1]*vectorV[1]+vectorU[2]*vectorV[2]
inline void ComputeDotProductFLOAT_4b(const sreal *terminal1, const sreal *tail1, const sreal *terminal2, const sreal *tail2, sreal *dotProduct)
{
	sreal pvectorU[3], pvectorV[3];
	pvectorU[0]=terminal1[0]-tail1[0];
	pvectorU[1]=terminal1[1]-tail1[1];
	pvectorU[2]=terminal1[2]-tail1[2];
	pvectorV[0]=terminal2[0]-tail2[0];
	pvectorV[1]=terminal2[1]-tail2[1];
	pvectorV[2]=terminal2[2]-tail2[2];
	*dotProduct=pvectorU[0]*pvectorV[0]+pvectorU[1]*pvectorV[1]+pvectorU[2]*pvectorV[2];
}

///PURPOSE: Add two vectors (x, y, z) and store the result in result.
//         The two vectors have a common tail.
//         The vector must be computed using terminal and tail.
inline void AddTwoVectorsFLOAT_3(sreal *result, const sreal *terminal1, const sreal *terminal2, const sreal *tail)
{
	result[0]=(terminal1[0]-tail[0])+(terminal2[0]-tail[0]);
	result[1]=(terminal1[1]-tail[1])+(terminal2[1]-tail[1]);
	result[2]=(terminal1[2]-tail[2])+(terminal2[2]-tail[2]);
}
inline void AddTwoVectorsDOUBLE_3(sreal2 *result, const sreal2 *terminal1, const sreal2 *terminal2, const sreal2 *tail)
{
	result[0]=(terminal1[0]-tail[0])+(terminal2[0]-tail[0]);
	result[1]=(terminal1[1]-tail[1])+(terminal2[1]-tail[1]);
	result[2]=(terminal1[2]-tail[2])+(terminal2[2]-tail[2]);
}


//These versions only use one pointer, first memory holds head of vector 1,
//             second memory holds tail of both vectors, third memory holds head of last vector
inline void AddTwoVectorsFLOAT_1a(sreal *result, const sreal *terminal_tail_terminal2)
{
	result[0]=(terminal_tail_terminal2[0]-terminal_tail_terminal2[3])+(terminal_tail_terminal2[6]-terminal_tail_terminal2[3]);
	result[1]=(terminal_tail_terminal2[1]-terminal_tail_terminal2[4])+(terminal_tail_terminal2[7]-terminal_tail_terminal2[4]);
	result[2]=(terminal_tail_terminal2[2]-terminal_tail_terminal2[5])+(terminal_tail_terminal2[8]-terminal_tail_terminal2[5]);
}
inline void AddTwoVectorsDOUBLE_1a(sreal2 *result, const sreal2 *terminal_tail_terminal2)
{
	result[0]=(terminal_tail_terminal2[0]-terminal_tail_terminal2[3])+(terminal_tail_terminal2[6]-terminal_tail_terminal2[3]);
	result[1]=(terminal_tail_terminal2[1]-terminal_tail_terminal2[4])+(terminal_tail_terminal2[7]-terminal_tail_terminal2[4]);
	result[2]=(terminal_tail_terminal2[2]-terminal_tail_terminal2[5])+(terminal_tail_terminal2[8]-terminal_tail_terminal2[5]);
}


//These versions only use one pointer, first memory holds vector 1, next holds vector 2
inline void AddTwoVectorsFLOAT_1b(sreal *result, const sreal *vector_vector2)
{
	result[0]=vector_vector2[0]+vector_vector2[3];
	result[1]=vector_vector2[1]+vector_vector2[4];
	result[2]=vector_vector2[2]+vector_vector2[5];
}
inline void AddTwoVectorsDOUBLE_1b(sreal2 *result, const sreal2 *vector_vector2)
{
	result[0]=vector_vector2[0]+vector_vector2[3];
	result[1]=vector_vector2[1]+vector_vector2[4];
	result[2]=vector_vector2[2]+vector_vector2[5];
}


//PURPOSE: Test if 2 floating point numbers are close enough based on delta
//         Return 1 if yes else 0.
inline sint AreTheseFLOATsEqualFLOAT_2(sreal number1, sreal number2, sreal delta)
{
	if(number1-number2>=delta)
		return 0;
	else
		return 1;
}


//PURPOSE:	Return 1 if 3 points are colinear or at the same point
//			0 if not colinear.
inline sint Are3PointsColinearFLOAT_1(const sreal *point1, const sreal *point2, const sreal *point3)
{
	const sreal delta=1.0e-5;
	sreal lineVector[3], lineVector2[3];

	lineVector[0]=point3[0]-point1[0];
	lineVector[1]=point3[1]-point1[1];
	lineVector[2]=point3[2]-point1[2];

	lineVector2[0]=point2[0]-point1[0];
	lineVector2[1]=point2[1]-point1[1];
	lineVector2[2]=point2[2]-point1[2];

	if((lineVector[0]!=0.0)||(lineVector[1]!=0.0)||(lineVector[2]!=0.0))
	{
		NormalizeVectorFLOAT_2(lineVector);
		NormalizeVectorFLOAT_2(lineVector2);

		if((AreTheseFLOATsEqualFLOAT_2(lineVector[0], lineVector2[0], delta))&&
			(AreTheseFLOATsEqualFLOAT_2(lineVector[1], lineVector2[1], delta))&&
			(AreTheseFLOATsEqualFLOAT_2(lineVector[2], lineVector2[2], delta)))
		{
			return 1;
		}
	}
	return 0;
}

//PURPOSE:	Compute the length given two points. (x, y, z)
inline void ComputeLengthGivenTwoPointsFLOAT_3(sreal *length, const sreal *point1, const sreal *point2)
{
	sreal pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	*length=sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline void ComputeLengthSquaredGivenTwoPointsFLOAT_3(sreal *lengthSquared, const sreal *point1, const sreal *point2)
{
	sreal pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	*lengthSquared=pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}

inline void ComputeLengthGivenTwoPointsDOUBLE_3(sreal2 *length, const sreal2 *point1, const sreal2 *point2)
{
	sreal2 pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	*length=sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline void ComputeLengthSquaredGivenTwoPointsDOUBLE_3(sreal2 *lengthSquared, const sreal2 *point1, const sreal2 *point2)
{
	sreal2 pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	*lengthSquared=pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}


//Version 2 of ComputeLengthGivenTwoPoints_3 but this one returns the value
inline sreal ComputeLengthGivenTwoPointsFLOAT_2(const sreal *point1, const sreal *point2)
{
	sreal pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	return sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline sreal ComputeLengthSquaredGivenTwoPointsFLOAT_2(const sreal *point1, const sreal *point2)
{
	sreal pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	return pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}

inline sreal2 ComputeLengthGivenTwoPointsDOUBLE_2(const sreal2 *point1, const sreal2 *point2)
{
	sreal2 pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	return sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline sreal2 ComputeLengthSquaredGivenTwoPointsDOUBLE_2(const sreal2 *point1, const sreal2 *point2)
{
	sreal2 pvector[3];
	pvector[0]=point1[0]-point2[0];
	pvector[1]=point1[1]-point2[1];
	pvector[2]=point1[2]-point2[2];
	return pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}

//PURPOSE: Compute the length given vector. (x, y, z)
inline void ComputeLengthOfVectorFLOAT_2(sreal *length, const sreal *pvector)
{
	*length=sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline void ComputeLengthSquaredOfVectorFLOAT_2(sreal *lengthSquared, const sreal *pvector)
{
	*lengthSquared=pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}

inline void ComputeLengthSquaredOfVectorDOUBLE_2(sreal2 *lengthSquared, const sreal2 *pvector)
{
	*lengthSquared=pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}


//Version 2 of ComputeLengthGivenTwoPoints_3 but this one returns the value
inline sreal ComputeLengthOfVectorFLOAT_1(const sreal *pvector)
{
	return sqrtf(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline sreal ComputeLengthSquaredOfVectorFLOAT_1(const sreal *pvector)
{
	return pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}

inline sreal2 ComputeLengthOfVectorDOUBLE_1(const sreal2 *pvector)
{
	return sqrt(pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2]);
}

inline sreal2 ComputeLengthSquaredOfVectorDOUBLE_1(const sreal2 *pvector)
{
	return pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}


//PURPOSE: For square matrices. This is column major for OpenGL.
//         vector must be of size 3 and matrix must be 3x3
inline void MultiplyMatrixByVector3by3OpenGL_FLOAT_555(sreal *resultvector, const sreal *matrix, const sreal *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+matrix[3]*pvector[1]+matrix[6]*pvector[2];
	resultvector[1]=matrix[1]*pvector[0]+matrix[4]*pvector[1]+matrix[7]*pvector[2];
	resultvector[2]=matrix[2]*pvector[0]+matrix[5]*pvector[1]+matrix[8]*pvector[2];
}

//PURPOSE: For square matrices. This is column major for OpenGL.
//         vector must be of size 3 and matrix must be 3x3
inline void MultiplyVectorByMatrix3by3OpenGL_FLOAT_555(sreal *resultvector, const sreal *matrix, const sreal *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+matrix[3]*pvector[1]+matrix[6]*pvector[2];
	resultvector[1]=matrix[1]*pvector[0]+matrix[4]*pvector[1]+matrix[7]*pvector[2];
	resultvector[2]=matrix[2]*pvector[0]+matrix[5]*pvector[1]+matrix[8]*pvector[2];
}

//PURPOSE:	For square matrices. This is column major for OpenGL.
//			vector must be of size 4.
inline void MultiplyMatrixByVector4by4OpenGL_DOUBLE(sreal2 *resultvector, const sreal2 *matrix, const sreal2 *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
	resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
	resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
	resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

//PURPOSE:	For square matrices. This is column major for OpenGL.
//			vector must be of size 4.
inline void MultiplyMatrixByVector4by4OpenGL_FLOAT(sreal *resultvector, const sreal *matrix, const sreal *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
	resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
	resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
	resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

//PURPOSE:	For square matrices. This is column major for OpenGL
inline void MultiplyMatrices4by4OpenGL_FLOAT(sreal *result, sreal *matrix1, sreal *matrix2)
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

//PURPOSE: For square matrices. This is column major for OpenGL
inline void MultiplyMatrices4by4OpenGL_DOUBLE_3(sreal2 *result, const sreal2 *matrix1, const sreal2 *matrix2)
{
	//result[0]=matrix[0]*matrix2[0]+
	//	matrix[1]*matrix2[4]+
	//	matrix[2]*matrix2[8]+
	//	matrix[3]*matrix2[12];
	sint i, j;
	for(i=0; i<4; i++)
	{
		for(j=0; j<4; j++)
		{
			result[i*4+j]=matrix1[i*4]*matrix2[j]+
							matrix1[i*4+1]*matrix2[j+4]+
							matrix1[i*4+2]*matrix2[j+8]+
							matrix1[i*4+3]*matrix2[j+12];
		}
	}
}

//PURPOSE:	Compute the factorial of a integer and return result as sreal.
inline sreal FactorialFLOAT(sint number)
{
	if(number<=1)
		return 1.0;
	else
	{
		sint i;
		sreal value;
		value=1.0;
		for(i=number; i>1; i--)
			value*=(sreal)i;
		return value;
	}
}

//PURPOSE:	Compute the factorial of a integer and return result as sreal2.
inline sreal2 FactorialDOUBLE(sint number)
{
	if(number<=1)
		return 1.0;
	else
	{
		sint i;
		sreal2 value;
		value=1.0;
		for(i=number; i>1; i--)
			value*=(sreal2)i;
		return value;
	}
}

//PURPOSE:	Allocate space for pFTempo and then call this only once
//			and then use EvaluateBezierCurveFLOAT_4
//
//			**** I added _4 for the function name for no particular reason.
//
//PARAMETERS:
//			[in] numberOfControlPoints : should be at least 2 points.
//			[out] pFTempo : allocate this as pFTempo=new sreal2[numberOfControlPoints]. Yes, leave it as sreal2.
inline void EvaluateBezierCurvePrecalculateFLOAT_4(sint numberOfControlPoints, sreal2 *pFTempo)
{
	sint j;
	sint degree=numberOfControlPoints-1;
	sreal2 nFactorial=FactorialDOUBLE(degree);

	for(j=0; j<numberOfControlPoints; j++)
	{
		pFTempo[j]=nFactorial/FactorialDOUBLE(j)/FactorialDOUBLE(degree-j);
	}
}

//PURPOSE:	You must call EvaluateBezierCurvePrecalculateFLOAT_4 first tp precompute some values.
//			Evaluate the bezier curve. (x, y, z) or (x, y) or (x, y, z, w) or (x)
//			numberOfComponentsForVertex is the same for control points.
//			numberOfControlPoints=degree+1 and there must be at least 2.
//			intervalCount must be >0 and indicate how many interval there will be.
//			example: if 1, then there are 2 (x, y, z) evaluated for pvertex.	
//
//			**** I added _4 for the function name for no particular reason.
//
//			This function can be used for example, to compute the path of an object through 3D space,
//			perhaps the path of a camera. The down side is that the path will only go through
//			the first and last control point. Not the in-between control points.
//			This is why a Catmull-Rom spline is better.
//
//PARAMETERS:
//			[in] numberOfComponentsForVertex : 1, 2, 3, 4
//			[out] pvertex : memory for pvertex must be allocated externally : new sreal[(intervalCount+1)*numberOfComponentsForVertex]
//			                If numberOfComponentsForVertex==4, then w will always be set to 1.0.
//			[in] intervalCount : must be >0 and indicate how many interval there will be.
//			                     **** You would probably use a high value here so that the curve will have enough definition.
//			[in] controlPoints : the component count (xyz) must be the same as numberOfComponentsForVertex.
//			                     The array size must be [numberOfComponentsForVertex*numberOfControlPoints].
//			                     If numberOfComponentsForVertex==4, then you can set w to 1.0 if you want. It doesn't matter.
//			                     pvertex's w will be set to 1.0
//			[in] numberOfControlPoints : numberOfControlPoints=degree+1. Must be at least 2.
//			[in] pFTempo : this is an array of precomputed values. The size is numberOfControlPoints. Yes, leave it as sreal2.
//
//EXAMPLE:
//sint numberOfControlPoints=4;
//sreal2 *pFTempo=new sreal2[numberOfControlPoints];
////This will fill up pFTempo with some precomputed values
//EvaluateBezierCurvePrecalculateFLOAT_4(numberOfControlPoints, pFTempo);
//sint numberOfComponentsForVertex=3;
//sint intervalCount=10;
//sreal *pvertex=new sreal[(intervalCount+1)*numberOfComponentsForVertex];
//sint numberOfControlPoints=4;
//sreal *controlPoints=new sreal[numberOfComponentsForVertex*numberOfControlPoints];
////Let's setup the control points
//controlPoints[0]=0.0;
//controlPoints[1]=0.0;
//controlPoints[2]=0.0;
//controlPoints[3]=0.0;
//controlPoints[4]=5.0;
//controlPoints[5]=0.0;
//controlPoints[6]=5.0;
//controlPoints[7]=5.0;
//controlPoints[8]=0.0;
//controlPoints[9]=5.0;
//controlPoints[10]=-1.0;
//controlPoints[11]=0.0;
////Let's evaluate the curve
//EvaluateBezierCurveFLOAT_4(numberOfComponentsForVertex, pvertex, intervalCount, controlPoints, numberOfControlPoints, pFTempo);
////You can of course now render the curve that is inside pvertex with glBegin(GL_LINE_STRIP) or use a VBO and glDrawArrays or glDrawRangeElements.
////You can modify the control points and call EvaluateBezierCurveFLOAT_4 again.
//delete [] pFTempo;
//delete [] pvertex;
//delete [] controlPoints;
inline void EvaluateBezierCurveFLOAT_4(sint numberOfComponentsForVertex, sreal *pvertex, sint intervalCount, const sreal *controlPoints, sint numberOfControlPoints,
	const sreal2 *pFTempo)
{
	//**** Using sreal2 for more precision internally
	sint i, j;
	sint degree=numberOfControlPoints-1;
	sreal2 nFactorial=FactorialDOUBLE(degree);
	sreal2  FTempo;


	//**** Using sreal2 for more precision internally
	sreal2 u/*=0.0*/;
	sreal2 uinc=1.0/sreal2(intervalCount);

	//Start and final vertices are the same as start and final control points,
	//so we can simply copy them (also there is no loss of precision by simply copying them
	//instead of computing them)
	if(numberOfComponentsForVertex==3)
	{
		intervalCount*=3;
		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[2]=controlPoints[2];
		pvertex[intervalCount  ]=controlPoints[numberOfControlPoints*3-3];
		pvertex[intervalCount+1]=controlPoints[numberOfControlPoints*3-2];
		pvertex[intervalCount+2]=controlPoints[numberOfControlPoints*3-1];
		for(i=3, u=uinc; i<intervalCount; i+=3, u+=uinc)
		{
			pvertex[i]=pvertex[i+1]=pvertex[i+2]=0.0;
			for(j=0; j<numberOfControlPoints; j++)
			{
				FTempo=(sreal)(pFTempo[j]*pow(sreal2(u), sreal2(j))*pow(sreal2(1.0-u), sreal2(degree-j)));
				pvertex[i  ]+=FTempo*controlPoints[j*3  ];
				pvertex[i+1]+=FTempo*controlPoints[j*3+1];
				pvertex[i+2]+=FTempo*controlPoints[j*3+2];
			}
		}
	}
	else if(numberOfComponentsForVertex==2)
	{
		intervalCount*=2;
		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[intervalCount  ]=controlPoints[numberOfControlPoints*2-2];
		pvertex[intervalCount+1]=controlPoints[numberOfControlPoints*2-1];
		for(i=2, u=uinc; i<intervalCount; i+=2, u+=uinc)
		{
			pvertex[i]=pvertex[i+1]=0.0;
			for(j=0; j<numberOfControlPoints; j++)
			{
				FTempo=(sreal)(pFTempo[j]*pow(sreal2(u), sreal2(j))*pow(sreal2(1.0-u), sreal2(degree-j)));
				pvertex[i  ]+=FTempo*controlPoints[j*2  ];
				pvertex[i+1]+=FTempo*controlPoints[j*2+1];
			}
		}
	}
	else if(numberOfComponentsForVertex==4)
	{
		intervalCount*=4;
		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[2]=controlPoints[2];
		pvertex[3]=1.0;
		pvertex[intervalCount  ]=controlPoints[numberOfControlPoints*4-4];
		pvertex[intervalCount+1]=controlPoints[numberOfControlPoints*4-3];
		pvertex[intervalCount+2]=controlPoints[numberOfControlPoints*4-2];
		pvertex[intervalCount+3]=1.0;
		for(i=4, u=uinc; i<intervalCount; i+=4, u+=uinc)
		{
			pvertex[i]=pvertex[i+1]=pvertex[i+2]=0.0;
			pvertex[i+3]=1.0;
			for(j=0; j<numberOfControlPoints; j++)
			{
				FTempo=(sreal)(pFTempo[j]*pow(sreal2(u), sreal2(j))*pow(sreal2(1.0-u), sreal2(degree-j)));
				pvertex[i  ]+=FTempo*controlPoints[j*4  ];
				pvertex[i+1]+=FTempo*controlPoints[j*4+1];
				pvertex[i+2]+=FTempo*controlPoints[j*4+2];
			}
		}
	}
	else if(numberOfComponentsForVertex==1)
	{
		//intervalCount*=1;
		pvertex[0]=controlPoints[0];
		pvertex[intervalCount]=controlPoints[numberOfControlPoints-1];
		for(i=1, u=uinc; i<intervalCount; i++, u+=uinc)
		{
			pvertex[i]=0.0;
			for(j=0; j<numberOfControlPoints; j++)
			{
				FTempo=(sreal)(pFTempo[j]*pow(sreal2(u), sreal2(j))*pow(sreal2(1.0-u), sreal2(degree-j)));
				pvertex[i]+=FTempo*controlPoints[j];
			}
		}
	}
}


//PURPOSE:	You must allocate space for pFTempo and then call this.Evaluate the bezier surface. (x, y, z)
//
//			**** I added _3 for the function name for no particular reason.
//
//PARAMETERS:
//			[out] pindex16Bit : indices for rendering with GL (glDrawRangeElements(GL_TRIANGLES)). Must be allocated before calling this function.
//			                    Size must be intervalCountU*intervalCountV*6.
//			[in] intervalCountU : must be >0 and indicate how many interval there will be in U direction.
//			                      You would probably want a high value here so that the curve will have a high definition.
//			[in] intervalCountV : must be >0 and indicate how many interval there will be in V direction.
//			                      You would probably want a high value here so that the curve will have a high definition.
//			[in] numberOfControlPointsU : numberOfControlPointsU=degree+1 and there must be at least 2.
//			[in] numberOfControlPointsV : numberOfControlPointsV=degree2+1 and there must be at least 2.
//			[out] pFTempo : Allocate this yourself. Yes, leave it as sreal2. Size should be new sreal2[(intervalCountV+1)*(intervalCountU+1)*numberOfControlPointsV*numberOfControlPointsU+numberOfControlPointsU+numberOfControlPointsV]
//			                example: if 1, then there are 2 (x, y, z) evaluated for pvertex.
inline void EvaluateBezierSurfacePrecalculateFLOAT_3(ushort *pindex16Bit, sint intervalCountU, sint intervalCountV, sint numberOfControlPointsU, sint numberOfControlPointsV,
		sreal2 *pFTempo)
{
	sint i, j, k, m, temp, temp2, temp3;
	sint degreeU=numberOfControlPointsU-1;
	sint degreeV=numberOfControlPointsV-1;
	sreal2 nFactorialU=FactorialDOUBLE(degreeU);
	sreal2 nFactorialV=FactorialDOUBLE(degreeV);
	sreal2 FTempoU, FTempoV;
	sint total;
	ushort findex;

	//We will store all values for the bezier mesh in a linear array

	total=(intervalCountV+1)*(intervalCountU+1)*numberOfControlPointsV*numberOfControlPointsU+numberOfControlPointsU+numberOfControlPointsV;

	sreal2 *pFTempoU, *pFTempoV;
	temp2=total-numberOfControlPointsU-numberOfControlPointsV;
	pFTempoU=&pFTempo[temp2];
	temp3=total-numberOfControlPointsV;
	pFTempoV=&pFTempo[temp3];

	for(j=0; j<numberOfControlPointsU; j++)
	{
		pFTempoU[j]=nFactorialU/FactorialDOUBLE(j)/FactorialDOUBLE(degreeU-j);
	}
	for(j=0; j<numberOfControlPointsV; j++)
	{
		pFTempoV[j]=nFactorialV/FactorialDOUBLE(j)/FactorialDOUBLE(degreeV-j);
	}

	sreal2 u/*=0.0*/;
	sreal2 uinc=1.0/sreal2(intervalCountU);
	sreal2 v/*=0.0*/;
	sreal2 vinc=1.0/sreal2(intervalCountV);

	temp=0;
	for(m=0, v=0.0; m<=intervalCountV; m++, v+=vinc)
	{
		for(k=0, u=0.0; k<=intervalCountU; k++, u+=uinc)
		{
			for(i=0; i<numberOfControlPointsV; i++)
			{
				FTempoV=pFTempoV[i]*pow(sreal2(v), sreal2(i))*pow(sreal2(1.0-v), sreal2(degreeV-i));
				for(j=0; j<numberOfControlPointsU; j++)
				{
					FTempoU=pFTempoU[j]*pow(sreal2(u), sreal2(j))*pow(sreal2(1.0-u), sreal2(degreeU-j));
					pFTempo[temp++]=FTempoU*FTempoV;
				}
			}
		}
	}

	findex=0;
	for(i=k=0; i<intervalCountU; i++)
	{
		for(j=0; j<intervalCountV; j++)
		{
			pindex16Bit[k  ]=findex;
			pindex16Bit[k+1]=findex+1;
			pindex16Bit[k+2]=findex+intervalCountV+1;
			pindex16Bit[k+3]=pindex16Bit[k+2];
			pindex16Bit[k+4]=pindex16Bit[k+1];
			pindex16Bit[k+5]=pindex16Bit[k+2]+1;
			findex++;
			k+=6;
		}

		findex++;
	}
}

//PURPOSE:	You must call EvaluateBezierSurfacePrecalculateFLOAT_3 first to precompute some values.
//			Evaluate the bezier surface. (x, y, z) or (x, y) or (x, y, z, w) or (x)
//			numberOfComponentsForVertex is the same for control points.
//			The control points are arranged from 0<u_direction<numberOfControlPointsU then 0<v_direction<numberOfControlPointsV
//			pvertex will be generated in the same order, and then you can construct triangles from it.
//
//			Memory for pvertex must be allocated externally : new sreal[(intervalCountU+1)*(intervalCountV+1)*numberOfComponentsForVertex]
//
//			**** I added _3 for the function name for no particular reason.
//
//PARAMETERS:
//			[in] numberOfComponentsForVertex : 2, 3, 4
//			[out] pvertex : memory for pvertex must be allocated externally : new sreal[(intervalCountU+1)*(intervalCountV+1)*numberOfComponentsForVertex]
//			                If numberOfComponentsForVertex==4, then w will always be set to 1.0.
//			[in] intervalCountU : must be >0 and indicate how many interval there will be in U direction.
//			                      You would probably want a high value here so that the curve will have a high definition.
//			[in] intervalCountV : must be >0 and indicate how many interval there will be in V direction.
//			                      You would probably want a high value here so that the curve will have a high definition.
//			[in] controlPoints : the component count (xyz) must be the same as numberOfComponentsForVertex.
//			                     The array size must be [numberOfComponentsForVertex*numberOfControlPointsU*numberOfControlPointsV].
//			                     If numberOfComponentsForVertex==4, then you can set w to 1.0 if you want. It doesn't matter.
//			                     pvertex's w will be set to 1.0
//			[in] numberOfControlPointsU : numberOfControlPointsU=degree+1 and there must be at least 2.
//			[in] numberOfControlPointsV : numberOfControlPointsV=degree2+1 and there must be at least 2.
//			[in] pFTempo : this is an array of precomputed values. The size is (intervalCountV+1)*(intervalCountU+1)*numberOfControlPointsV*numberOfControlPointsU+numberOfControlPointsU+numberOfControlPointsV. Yes, leave it as sreal2.
//
//EXAMPLE:
//sint intervalCountU=10;
//sint intervalCountV=10;
//sint numberOfControlPointsU=4;
//sint numberOfControlPointsV=4;
//ushort *pindex16Bit=new ushort[intervalCountU*intervalCountV*6];
//sreal2 *pFTempo=new sreal2[(intervalCountV+1)*(intervalCountU+1)*numberOfControlPointsV*numberOfControlPointsU+numberOfControlPointsU+numberOfControlPointsV];
//This will fill up pFTempo with some precomputed values
//EvaluateBezierSurfacePrecalculateFLOAT_3(pindex16Bit, intervalCountU, intervalCountV, numberOfControlPointsU, numberOfControlPointsV, pFTempo);
//sint numberOfComponentsForVertex=3;
//sreal *pvertex=new sreal[(intervalCountU+1)*(intervalCountV+1)*3];
//sreal *controlPoints=new sreal[numberOfComponentsForVertex*numberOfControlPointsU*numberOfControlPointsV];
////Let's setup the control points
//controlPoints[0]=0.0;
//controlPoints[1]=0.0;
//controlPoints[2]=3.0;
//controlPoints[3]=1.0;
//controlPoints[4]=0.0;
//controlPoints[5]=0.0;
//controlPoints[6]=2.0;
//controlPoints[7]=0.0;
//controlPoints[8]=0.0;
//controlPoints[9]=3.0;
//controlPoints[10]=0.0;
//controlPoints[11]=0.0;
//controlPoints[12]=0.0;
//controlPoints[13]=1.0;
//controlPoints[14]=0.0;
//controlPoints[15]=1.0;
//controlPoints[16]=1.0;
//controlPoints[17]=0.0;
//controlPoints[18]=2.0;
//controlPoints[19]=1.0;
//controlPoints[20]=0.0;
//controlPoints[21]=3.0;
//controlPoints[22]=1.0;
//controlPoints[23]=5.0;
//controlPoints[24]=0.0;
//controlPoints[25]=2.0;
//controlPoints[26]=0.0;
//controlPoints[27]=1.0;
//controlPoints[28]=2.0;
//controlPoints[29]=0.0;
//controlPoints[30]=2.0;
//controlPoints[31]=2.0;
//controlPoints[32]=0.0;
//controlPoints[33]=3.0;
//controlPoints[34]=2.0;
//controlPoints[35]=0.0;
//controlPoints[36]=0.0;
//controlPoints[37]=3.0;
//controlPoints[38]=0.0;
//controlPoints[39]=1.0;
//controlPoints[40]=3.0;
//controlPoints[41]=0.0;
//controlPoints[42]=2.0;
//controlPoints[43]=3.0;
//controlPoints[44]=2.0;
//controlPoints[45]=3.0;
//controlPoints[46]=3.0;
//controlPoints[47]=2.0;
////Let's evaluate the surface
//EvaluateBezierSurfaceFLOAT_3(numberOfComponentsForVertex, pvertex, intervalCountU, intervalCountV, controlPoints, numberOfControlPointsU, numberOfControlPointsV, pFTempo);
//glDisable(GL_LIGHTING);
//glColor4f(1.0, 1.0, 1.0, 1.0);
//glVertexPointer(3, GL_FLOAT, sizeof(sreal)*numberOfComponentsForVertex, pvertex);
//sint lastvertex=(intervalCountU+1)*(intervalCountV+1)*numberOfComponentsForVertex-1;
//glDrawRangeElements(GL_TRIANGLES, 0, lastvertex, intervalCountU*intervalCountV*6, GL_UNSIGNED_SHORT, pindex16Bit);
//
////You can modify the control points and call EvaluateBezierSurfaceFLOAT_3 again.
////I am not using VBO/IBO to render but feel free to do so.
//delete [] pindex16Bit;
//delete [] pFTempo;
//delete [] pvertex;
//delete [] controlPoints;
inline void EvaluateBezierSurfaceFLOAT_3(sint numberOfComponentsForVertex, sreal *pvertex, sint intervalCountU, sint intervalCountV, const sreal *controlPoints, sint numberOfControlPointsU, sint numberOfControlPointsV,
			const sreal2 *pFTempo)
{
	//NOTE: The code here is similar to EvaluateBezierCurveFLOAT_1 but we are evaluating a mesh
	//here instead of a curve
	//Using sreal2 for more precision internally
	sint i, j, k, m, temp, temp2, temp3;
	sint degreeU=numberOfControlPointsU-1;
	sint degreeV=numberOfControlPointsV-1;
	sreal2 nFactorialU=FactorialDOUBLE(degreeU);
	sreal2 nFactorialV=FactorialDOUBLE(degreeV);
	sint intervalCountU2=intervalCountU+1;

	
	sreal2 u/*=0.0*/;
	sreal2 uinc=1.0/sreal2(intervalCountU);
	sreal2 v/*=0.0*/;
	sreal2 vinc=1.0/sreal2(intervalCountV);

	//The 4 corners of the mesh (patch) are the same as the 4 corners of the control points,
	//so we can simply copy them (also there is no loss of precision by simply copying them
	//instead of computing them)
	if(numberOfComponentsForVertex==3)
	{
		temp=temp3=0;
		for(m=0, v=0.0; m<=intervalCountV; m++, v+=vinc)
		{
			for(k=0, u=0.0; k<=intervalCountU; k++, u+=uinc)
			{
				pvertex[temp]=pvertex[temp+1]=pvertex[temp+2]=0.0;
				temp2=0;
				for(i=0; i<numberOfControlPointsV; i++)
				{
					for(j=0; j<numberOfControlPointsU; j++)
					{
						pvertex[temp  ]+=pFTempo[temp3]*controlPoints[temp2  ];
						pvertex[temp+1]+=pFTempo[temp3]*controlPoints[temp2+1];
						pvertex[temp+2]+=pFTempo[temp3]*controlPoints[temp2+2];
						temp2+=3;
						temp3++;
					}
				}
				temp+=3;
			}
		}

		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[2]=controlPoints[2];
		pvertex[intervalCountU*3  ]=controlPoints[(numberOfControlPointsU-1)*3  ];
		pvertex[intervalCountU*3+1]=controlPoints[(numberOfControlPointsU-1)*3+1];
		pvertex[intervalCountU*3+2]=controlPoints[(numberOfControlPointsU-1)*3+2];
		pvertex[intervalCountV*intervalCountU2*3  ]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*3  ];
		pvertex[intervalCountV*intervalCountU2*3+1]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*3+1];
		pvertex[intervalCountV*intervalCountU2*3+2]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*3+2];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*3  ]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*3  ];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*3+1]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*3+1];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*3+2]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*3+2];
	}
	else if(numberOfComponentsForVertex==2)
	{
		temp=temp3=0;
		for(m=0, v=0.0; m<=intervalCountV; m++, v+=vinc)
		{
			for(k=0, u=0.0; k<=intervalCountU; k++, u+=uinc)
			{
				pvertex[temp]=pvertex[temp+1]=0.0;
				temp2=0;
				for(i=0; i<numberOfControlPointsV; i++)
				{
					for(j=0; j<numberOfControlPointsU; j++)
					{
						pvertex[temp  ]+=pFTempo[temp3]*controlPoints[temp2  ];
						pvertex[temp+1]+=pFTempo[temp3]*controlPoints[temp2+1];
						temp2+=2;
						temp3++;
					}
				}
				temp+=2;
			}
		}

		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[intervalCountU*2  ]=controlPoints[(numberOfControlPointsU-1)*2  ];
		pvertex[intervalCountU*2+1]=controlPoints[(numberOfControlPointsU-1)*2+1];
		pvertex[intervalCountV*intervalCountU2*2  ]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*2  ];
		pvertex[intervalCountV*intervalCountU2*2+1]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*2+1];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*2  ]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*2  ];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*2+1]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*2+1];
	}
	else if(numberOfComponentsForVertex==4)
	{
		temp=temp3=0;
		for(m=0, v=0.0; m<=intervalCountV; m++, v+=vinc)
		{
			for(k=0, u=0.0; k<=intervalCountU; k++, u+=uinc)
			{
				pvertex[temp]=pvertex[temp+1]=pvertex[temp+2]=0.0;
				pvertex[temp+3]=1.0;
				temp2=0;
				for(i=0; i<numberOfControlPointsV; i++)
				{
					for(j=0; j<numberOfControlPointsU; j++)
					{
						pvertex[temp  ]+=pFTempo[temp3]*controlPoints[temp2  ];
						pvertex[temp+1]+=pFTempo[temp3]*controlPoints[temp2+1];
						pvertex[temp+2]+=pFTempo[temp3]*controlPoints[temp2+2];
						temp2+=4;
						temp3++;
					}
				}
				temp+=4;
			}
		}

		pvertex[0]=controlPoints[0];
		pvertex[1]=controlPoints[1];
		pvertex[2]=controlPoints[2];
		pvertex[3]=1.0;
		pvertex[intervalCountU*4  ]=controlPoints[(numberOfControlPointsU-1)*4  ];
		pvertex[intervalCountU*4+1]=controlPoints[(numberOfControlPointsU-1)*4+1];
		pvertex[intervalCountU*4+2]=controlPoints[(numberOfControlPointsU-1)*4+2];
		pvertex[intervalCountU*4+3]=1.0;
		pvertex[intervalCountV*intervalCountU2*4  ]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*4  ];
		pvertex[intervalCountV*intervalCountU2*4+1]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*4+1];
		pvertex[intervalCountV*intervalCountU2*4+2]=controlPoints[(numberOfControlPointsV-1)*(numberOfControlPointsU)*4+2];
		pvertex[intervalCountV*intervalCountU2*4+3]=1.0;
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*4  ]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*4  ];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*4+1]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*4+1];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*4+2]=controlPoints[((numberOfControlPointsV-1)*(numberOfControlPointsU)+numberOfControlPointsU-1)*4+2];
		pvertex[(intervalCountV*intervalCountU2+intervalCountU)*4+3]=1.0;
	}
}



//PURPOSE:	This function computes a k-th Order Catmull-Rom Spline.
//			Typically, the Catmull-Rom functions are limited to 4 points : P0, P1, P2, P3
//			and also, you vary a variable called t from 0.0 to 1.0 and you get a curve from P1 to P2.
//			This function doesn't have such limitations.
//			You can supply any number of points.
//			The curve is generated *starting* from the first control point *until* the last control point.
//
//PARAMETERS:
//			[in] numberOfComponentsForVertex : 1, 2, 3, 4
//			[out] pvertex : memory for pvertex must be allocated externally : new sreal[((numberOfControlPoints-1)*intervalCount+1)*numberOfComponentsForVertex]
//			                If numberOfComponentsForVertex==4, then w will always be set to 1.0.
//			[in] intervalCount : must be >0. This is the number of intervals between control points!!!
//			                     If you set this to 1, and you have 4 control points, then there will be 4 vertices generated.
//			                     If you set this to 2, and you have 4 control points, then there will be (4-1)*2+1=7 vertices generated.
//			                     If you set this to 3, and you have 4 control points, then there will be (4-1)*3+1=10 vertices generated.
//			                     If you set this to 4, and you have 4 control points, then there will be (4-1)*4+1=13 vertices generated.
//			                     If you set this to 5, and you have 4 control points, then there will be (4-1)*5+1=16 vertices generated.
//			                     If you set this to X, and you have 4 control points, then there will be (4-1)*X+1=Z vertices generated.
//			                     **** You would probably use a high value here so that the curve will have enough definition.
//			[in] controlPoints : the component count (xyz) must be the same as numberOfComponentsForVertex.
//			                     The array size must be [numberOfComponentsForVertex*numberOfControlPoints].
//			                     If numberOfComponentsForVertex==4, then you can set w to 1.0 if you want. It doesn't matter.
//			                     pvertex's w will be set to 1.0
//			[in] numberOfControlPoints : must be at least 4. 2 or 3 would be too little.
//			                             You need 4 if you don't want to cause a crash.
//
//EXAMPLE:
//sint numberOfComponentsForVertex=3;
//sint intervalCount=20;
//sint numberOfControlPoints=4;
//sint totalVertex=((numberOfControlPoints-1)*intervalCount+1)*numberOfComponentsForVertex;
//sreal *pvertex=new sreal[totalVertex];
//sreal *controlPoints=new sreal[numberOfComponentsForVertex*numberOfControlPoints];
////Let's setup the control points
//controlPoints[0]=0.0;
//controlPoints[1]=0.0;
//controlPoints[2]=0.0;
//controlPoints[3]=0.0;
//controlPoints[4]=1.0;
//controlPoints[5]=0.0;
//controlPoints[6]=1.0;
//controlPoints[7]=1.0;
//controlPoints[8]=0.0;
//controlPoints[9]=1.0;
//controlPoints[10]=0.0;
//controlPoints[11]=0.0;
////Let's evaluate the curve
//EvaluateCatmullRomCurveFLOAT_2(numberOfComponentsForVertex, pvertex, intervalCount, controlPoints, numberOfControlPoints);
//
//glDisable(GL_LIGHTING);
//glColor4f(1.0, 1.0, 1.0, 1.0);
//glBegin(GL_LINE_STRIP);
//for(i=0; i<totalVertex; i+=3)
//{
//	glVertex3fv(&pvertex[i]);
//}
//glEnd();
//glColor4f(1.0, 1.0, 0.0, 1.0);
//glPointSize(5.0);
//glBegin(GL_POINTS);
//for(i=0; i<numberOfComponentsForVertex*numberOfControlPoints; i+=3)
//{
//	glVertex3fv(&controlPoints[i]);
//}
//glEnd();
//delete [] pvertex;
//delete [] controlPoints;
inline void EvaluateCatmullRomCurveFLOAT_2(sint numberOfComponentsForVertex, sreal *pvertex, sint intervalCount, const sreal *controlPoints, sint numberOfControlPoints)
{
	sint i, j, k;
	sint totalIntervals;
	sreal t;			//t should go from 0.0 to 1.0
	sreal t_inc;
	sint p;
	sint p0;			//The 4 points for the Catmull-Rom spline calculation
	sint p1;
	sint p2;
	sint p3;
	sreal t2;
	sreal t3;
	sreal b0, b1, b2, b3;

	totalIntervals=(numberOfControlPoints-1)*intervalCount+1;
	t_inc=1.0/sreal(intervalCount);

	if(numberOfComponentsForVertex==3)
	{
		t=0.0;						//Start at 0.0
		p=0;
		for(i=j=k=0; i<totalIntervals; i++, j+=3, t+=t_inc, k++)
		{
			if(k>intervalCount)
			{
				k=1;
				p++;
				t=t_inc;			//Yes, this is correct. Don't set to 0.0
			}

			p0=p-1;
			//Make sure the index is within bounds
			if(p0<0)
				p0=0;
			else if(p0>=numberOfControlPoints-1)
				p0=numberOfControlPoints-1;

			p1=p;
			//Make sure the index is within bounds
			if(p1<0)
				p1=0;
			else if(p1>=numberOfControlPoints-1)
				p1=numberOfControlPoints-1;

			p2=p+1;
			//Make sure the index is within bounds
			if(p2<0)
				p2=0;
			else if(p2>=numberOfControlPoints-1)
				p2=numberOfControlPoints-1;

			p3=p+2;
			//Make sure the index is within bounds
			if(p3<0)
				p3=0;
			else if(p3>=numberOfControlPoints-1)
				p3=numberOfControlPoints-1;

			//Fix up the indices because this gives better results
			if(p0==numberOfControlPoints-2)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}
			else if(p0==numberOfControlPoints-1)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}


			//For indexing the controlPoints
			p0*=numberOfComponentsForVertex;
			p1*=numberOfComponentsForVertex;
			p2*=numberOfComponentsForVertex;
			p3*=numberOfComponentsForVertex;

			//Time to compute the vertex, using the 4 neighboring control points
			t2=t*t;
			t3=t2*t;

			//b0=0.5*(-t3+2.0*t2-t);		Rearrange into
			b0=0.5*(2.0*t2-t-t3);
			b1=0.5*(3.0*t3-5*t2+2.0);
			//b2=0.5*(-3.0*t3+4.0*t2+t);	Rearrange into
			b2=0.5*(4.0*t2+t-3.0*t3);
			b3=0.5*(t3-t2);

			pvertex[j  ]=controlPoints[p0  ]*b0+controlPoints[p1  ]*b1+controlPoints[p2  ]*b2+controlPoints[p3  ]*b3;
			pvertex[j+1]=controlPoints[p0+1]*b0+controlPoints[p1+1]*b1+controlPoints[p2+1]*b2+controlPoints[p3+1]*b3;
			pvertex[j+2]=controlPoints[p0+2]*b0+controlPoints[p1+2]*b1+controlPoints[p2+2]*b2+controlPoints[p3+2]*b3;
		}
	}
	else if(numberOfComponentsForVertex==4)
	{
		t=0.0;						//Start at 0.0
		p=0;
		for(i=j=k=0; i<totalIntervals; i++, j+=4, t+=t_inc, k++)
		{
			if(k>intervalCount)
			{
				k=1;
				p++;
				t=t_inc;			//Yes, this is correct. Don't set to 0.0
			}

			p0=p-1;
			//Make sure the index is within bounds
			if(p0<0)
				p0=0;
			else if(p0>=numberOfControlPoints-1)
				p0=numberOfControlPoints-1;

			p1=p;
			//Make sure the index is within bounds
			if(p1<0)
				p1=0;
			else if(p1>=numberOfControlPoints-1)
				p1=numberOfControlPoints-1;

			p2=p+1;
			//Make sure the index is within bounds
			if(p2<0)
				p2=0;
			else if(p2>=numberOfControlPoints-1)
				p2=numberOfControlPoints-1;

			p3=p+2;
			//Make sure the index is within bounds
			if(p3<0)
				p3=0;
			else if(p3>=numberOfControlPoints-1)
				p3=numberOfControlPoints-1;

			//Fix up the indices because this gives better results
			if(p0==numberOfControlPoints-2)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}
			else if(p0==numberOfControlPoints-1)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}


			//For indexing the controlPoints
			p0*=numberOfComponentsForVertex;
			p1*=numberOfComponentsForVertex;
			p2*=numberOfComponentsForVertex;
			p3*=numberOfComponentsForVertex;

			//Time to compute the vertex, using the 4 neighboring control points
			t2=t*t;
			t3=t2*t;

			//b0=0.5*(-t3+2.0*t2-t);		Rearrange into
			b0=0.5*(2.0*t2-t-t3);
			b1=0.5*(3.0*t3-5*t2+2.0);
			//b2=0.5*(-3.0*t3+4.0*t2+t);	Rearrange into
			b2=0.5*(4.0*t2+t-3.0*t3);
			b3=0.5*(t3-t2);

			pvertex[j  ]=controlPoints[p0  ]*b0+controlPoints[p1  ]*b1+controlPoints[p2  ]*b2+controlPoints[p3  ]*b3;
			pvertex[j+1]=controlPoints[p0+1]*b0+controlPoints[p1+1]*b1+controlPoints[p2+1]*b2+controlPoints[p3+1]*b3;
			pvertex[j+2]=controlPoints[p0+2]*b0+controlPoints[p1+2]*b1+controlPoints[p2+2]*b2+controlPoints[p3+2]*b3;
			pvertex[j+3]=1.0;			//Set w to 1.0
		}
	}
	else if(numberOfComponentsForVertex==2)
	{
		t=0.0;						//Start at 0.0
		p=0;
		for(i=j=k=0; i<totalIntervals; i++, j+=2, t+=t_inc, k++)
		{
			if(k>intervalCount)
			{
				k=1;
				p++;
				t=t_inc;			//Yes, this is correct. Don't set to 0.0
			}

			p0=p-1;
			//Make sure the index is within bounds
			if(p0<0)
				p0=0;
			else if(p0>=numberOfControlPoints-1)
				p0=numberOfControlPoints-1;

			p1=p;
			//Make sure the index is within bounds
			if(p1<0)
				p1=0;
			else if(p1>=numberOfControlPoints-1)
				p1=numberOfControlPoints-1;

			p2=p+1;
			//Make sure the index is within bounds
			if(p2<0)
				p2=0;
			else if(p2>=numberOfControlPoints-1)
				p2=numberOfControlPoints-1;

			p3=p+2;
			//Make sure the index is within bounds
			if(p3<0)
				p3=0;
			else if(p3>=numberOfControlPoints-1)
				p3=numberOfControlPoints-1;

			//Fix up the indices because this gives better results
			if(p0==numberOfControlPoints-2)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}
			else if(p0==numberOfControlPoints-1)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}


			//For indexing the controlPoints
			p0*=numberOfComponentsForVertex;
			p1*=numberOfComponentsForVertex;
			p2*=numberOfComponentsForVertex;
			p3*=numberOfComponentsForVertex;

			//Time to compute the vertex, using the 4 neighboring control points
			t2=t*t;
			t3=t2*t;

			//b0=0.5*(-t3+2.0*t2-t);		Rearrange into
			b0=0.5*(2.0*t2-t-t3);
			b1=0.5*(3.0*t3-5*t2+2.0);
			//b2=0.5*(-3.0*t3+4.0*t2+t);	Rearrange into
			b2=0.5*(4.0*t2+t-3.0*t3);
			b3=0.5*(t3-t2);

			pvertex[j  ]=controlPoints[p0  ]*b0+controlPoints[p1  ]*b1+controlPoints[p2  ]*b2+controlPoints[p3  ]*b3;
			pvertex[j+1]=controlPoints[p0+1]*b0+controlPoints[p1+1]*b1+controlPoints[p2+1]*b2+controlPoints[p3+1]*b3;
		}
	}
	else if(numberOfComponentsForVertex==1)
	{
		t=0.0;						//Start at 0.0
		p=0;
		for(i=j=k=0; i<totalIntervals; i++, j++, t+=t_inc, k++)
		{
			if(k>intervalCount)
			{
				k=1;
				p++;
				t=t_inc;			//Yes, this is correct. Don't set to 0.0
			}

			p0=p-1;
			//Make sure the index is within bounds
			if(p0<0)
				p0=0;
			else if(p0>=numberOfControlPoints-1)
				p0=numberOfControlPoints-1;

			p1=p;
			//Make sure the index is within bounds
			if(p1<0)
				p1=0;
			else if(p1>=numberOfControlPoints-1)
				p1=numberOfControlPoints-1;

			p2=p+1;
			//Make sure the index is within bounds
			if(p2<0)
				p2=0;
			else if(p2>=numberOfControlPoints-1)
				p2=numberOfControlPoints-1;

			p3=p+2;
			//Make sure the index is within bounds
			if(p3<0)
				p3=0;
			else if(p3>=numberOfControlPoints-1)
				p3=numberOfControlPoints-1;

			//Fix up the indices because this gives better results
			if(p0==numberOfControlPoints-2)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}
			else if(p0==numberOfControlPoints-1)
			{
				p0=numberOfControlPoints-3;
				p1=numberOfControlPoints-2;
				p2=numberOfControlPoints-1;
				p3=numberOfControlPoints-1;
			}


			//For indexing the controlPoints
			p0*=numberOfComponentsForVertex;
			p1*=numberOfComponentsForVertex;
			p2*=numberOfComponentsForVertex;
			p3*=numberOfComponentsForVertex;

			//Time to compute the vertex, using the 4 neighboring control points
			t2=t*t;
			t3=t2*t;

			//b0=0.5*(-t3+2.0*t2-t);		Rearrange into
			b0=0.5*(2.0*t2-t-t3);
			b1=0.5*(3.0*t3-5*t2+2.0);
			//b2=0.5*(-3.0*t3+4.0*t2+t);	Rearrange into
			b2=0.5*(4.0*t2+t-3.0*t3);
			b3=0.5*(t3-t2);

			pvertex[j  ]=controlPoints[p0  ]*b0+controlPoints[p1  ]*b1+controlPoints[p2  ]*b2+controlPoints[p3  ]*b3;
		}
	}
}









//PURPOSE:	Multiply vector by matrix. matrix is 16 and vector is 4 so is resultvector.
//			Matrix must be column major for OpenGL.
inline void MultiplyVectorByMatrix4by4OpenGL_FLOAT(sreal *resultvector, const sreal *matrix, const sreal *pvector)
{
	resultvector[0]=matrix[0]*pvector[0]+
		matrix[1]*pvector[1]+
		matrix[2]*pvector[2]+
		matrix[3]*pvector[3];
	resultvector[1]=matrix[4]*pvector[0]+
		matrix[5]*pvector[1]+
		matrix[6]*pvector[2]+
		matrix[7]*pvector[3];
	resultvector[2]=matrix[8]*pvector[0]+
		matrix[9]*pvector[1]+
		matrix[10]*pvector[2]+
		matrix[11]*pvector[3];
	resultvector[3]=matrix[12]*pvector[0]+
		matrix[13]*pvector[1]+
		matrix[14]*pvector[2]+
		matrix[15]*pvector[3];
}

//PURPOSE: Multiply vector by matrix. matrix is 16 and vector is 3, so is resultvector.
//         Matrix must be column major for OpenGL.
//         This is good for transforming normals. The upper 3x3 of the matrix will be used.
inline void MultiplyVectorByMatrix4by4OpenGL_FLOAT_3(sreal *resultvector3D, const sreal *matrix, const sreal *pvector3D)
{
	resultvector3D[0]=matrix[0]*pvector3D[0]+
		matrix[1]*pvector3D[1]+
		matrix[2]*pvector3D[2];
	resultvector3D[1]=matrix[4]*pvector3D[0]+
		matrix[5]*pvector3D[1]+
		matrix[6]*pvector3D[2];
	resultvector3D[2]=matrix[8]*pvector3D[0]+
		matrix[9]*pvector3D[1]+
		matrix[10]*pvector3D[2];
}

//PURPOSE: Take 2 points (x, y, z) and compute a vector.
//         vector=point2-point1
inline void ComputeVectorFromTwoPoints_FLOAT_3(sreal *pvector, const sreal *ppoint1, const sreal *ppoint2)
{
	pvector[0]=ppoint2[0]-ppoint1[0];
	pvector[1]=ppoint2[1]-ppoint1[1];
	pvector[2]=ppoint2[2]-ppoint1[2];
}

//PURPOSE:	Compute a orthonormal basis matrix (basis, 3x3 matrix) using 3 vertices (3D) and their
//			texture coordinates (2D) and using the faceNormal (3D) for the face.
//WARNING:	Matrix is column major for OpenGL
inline void ComputeOrthonormalBasis_v1_FLOAT(sreal *basis, const sreal *v0, const sreal *v1, const sreal *v2,
								const sreal *t0, const sreal *t1, const sreal *t2, const sreal *faceNormal)
{
	sreal cp[3];
	sreal e0[3], e1[3];
	e0[0]=v1[0]-v0[0];		//x-x
	e0[1]=t1[0]-t0[0];		//s-s
	e0[2]=t1[1]-t0[1];		//t-t
	e1[0]=v2[0]-v0[0];		//x-x
	e1[1]=t2[0]-t0[0];		//s-s
	e1[2]=t2[1]-t0[1];		//t-t

	//Compute the cross product cp = e0 CROSS e1
	cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);
	//The cross product of the two edge vectors yields
	//a normal vector to the plane in which they lie.
	//This vector defines a plane equation.
	//Ax + Bu + Cv + D = 0 
	//Assume Cv + D = 0
	// x = -Bu/A
	//
	//Assume Bu + D = 0
	//dudx = -B/A (basis[0])
	//dvdy = -C/A (basis[3])

	if(fabsf(cp[0])>1.0e-6)
	{
		basis[0]=-cp[1]/cp[0];
		basis[3]=-cp[2]/cp[0];
	}

	e0[0]=v1[1]-v0[1];
	e1[0]=v2[1]-v0[1];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);

	if(fabsf(cp[0])>1.0e-6)
	{
		basis[1]=-cp[1]/cp[0];
		basis[4]=-cp[2]/cp[0];
	}

	e0[0]=v1[2]-v0[2];
	e1[0]=v2[2]-v0[2];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);

	if(fabsf(cp[0])>1.0e-6)
	{
		basis[2]=-cp[1]/cp[0];
		basis[5]=-cp[2]/cp[0];
	}

	//Tangent... in first column of the matrix
	sreal oonorm=1.0/sqrtf(basis[0]*basis[0]+basis[1]*basis[1]+basis[2]*basis[2]);
	basis[0]*=oonorm;
	basis[1]*=oonorm;
	basis[2]*=oonorm;

	//Binormal... in the second column of the matrix
	oonorm=1.0/sqrtf(basis[3]*basis[3]+basis[4]*basis[4]+basis[5]*basis[5]);
	basis[3]*=oonorm;
	basis[4]*=oonorm;
	basis[5]*=oonorm;

	//Normal... in the third column of the matrix
	//Compute the cross product TxB
	basis[6]=basis[1]*basis[5]-basis[2]*basis[4];
	basis[7]=basis[2]*basis[3]-basis[0]*basis[5];
	basis[8]=basis[0]*basis[4]-basis[1]*basis[3];

	oonorm=1.0/sqrtf(basis[6]*basis[6]+basis[7]*basis[7]+basis[8]*basis[8]);
	basis[6]*=oonorm;
	basis[7]*=oonorm;
	basis[8]*=oonorm;

	//Gram-Schmidt orthogonalization process for B
	//Compute the cross product B=NxT to obtain an orthogonal basis
	//(Without this it's possible that complete orthogonality is not acheived)
	basis[3]=basis[7]*basis[2]-basis[8]*basis[1];
	basis[4]=basis[8]*basis[0]-basis[6]*basis[2];
	basis[5]=basis[6]*basis[1]-basis[7]*basis[0];

	if(basis[6]*faceNormal[0]+basis[7]*faceNormal[1]+basis[8]*faceNormal[2]<0.0)
	{
		basis[6]=-basis[6];
		basis[7]=-basis[7];
		basis[8]=-basis[8];
	}
}

//PURPOSE:	Compute a orthonormal basis matrix (basis, 3x3 matrix) using 3 vertices (3D) and their
//			texture coordinates (2D) and using the *normal (3D) for the face*.
//			This is similar to ComputeOrthonormalBasis_v1_FLOAT, except we store tangent, binormal
//          and normal separatly. **Normal is needed because these may not be the same as the vertex normal!
//
//PARAMETERS:
//			[out] tangent : xyz, normalized
//			[out] binormal : xyz, normalized
//			[out] normal : xyz, normalized. 90 degrees to tangent and binormal
//			[in] v0 : xyz vertex
//			[in] v1 : xyz vertex
//			[in] v2 : xyz vertex
//			[in] t0 : st tex coord
//			[in] t1 : st tex coord
//			[in] faceNormal : this could be an actual faceNormal or just one of the vertices normal should do the job OK
inline void ComputeOrthonormalBasis_v2_FLOAT(sreal *tangent, sreal *binormal, sreal *normal, const sreal *v0, const sreal *v1, const sreal *v2,
								const sreal *t0, const sreal *t1, const sreal *t2, const sreal *faceNormal)
{
	sreal cp[3];
	sreal e0[3], e1[3];
	e0[0]=v1[0]-v0[0];		//x-x
	e0[1]=t1[0]-t0[0];		//s-s
	e0[2]=t1[1]-t0[1];		//t-t
	e1[0]=v2[0]-v0[0];		//x-x
	e1[1]=t2[0]-t0[0];		//s-s
	e1[2]=t2[1]-t0[1];		//t-t

	//Compute the cross product cp = e0 CROSS e1
	cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);
	//The cross product of the two edge vectors yields
	//a normal vector to the plane in which they lie.
	//This vector defines a plane equation.
	//Ax + Bu + Cv + D = 0 
	//Assume Cv + D = 0
	// x = -Bu/A
	//
	//Assume Bu + D = 0
	//dudx = -B/A (basis[0])
	//dvdy = -C/A (basis[3])

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[0]=-cp[1]/cp[0];
		binormal[0]=-cp[2]/cp[0];
	}

	e0[0]=v1[1]-v0[1];
	e1[0]=v2[1]-v0[1];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[1]=-cp[1]/cp[0];
		binormal[1]=-cp[2]/cp[0];
	}

	e0[0]=v1[2]-v0[2];
	e1[0]=v2[2]-v0[2];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[2]=-cp[1]/cp[0];
		binormal[2]=-cp[2]/cp[0];
	}

	//Tangent... in first column of the matrix
	sreal oonorm=1.0/sqrtf(tangent[0]*tangent[0]+tangent[1]*tangent[1]+tangent[2]*tangent[2]);
	tangent[0]*=oonorm;
	tangent[1]*=oonorm;
	tangent[2]*=oonorm;

	//Binormal... in the second column of the matrix
	oonorm=1.0/sqrtf(binormal[0]*binormal[0]+binormal[1]*binormal[1]+binormal[2]*binormal[2]);
	binormal[0]*=oonorm;
	binormal[1]*=oonorm;
	binormal[2]*=oonorm;

	//Normal... in the third column of the matrix
	//Compute the cross product TxB
	normal[0]=tangent[1]*binormal[2]-tangent[2]*binormal[1];
	normal[1]=tangent[2]*binormal[0]-tangent[0]*binormal[2];
	normal[2]=tangent[0]*binormal[1]-tangent[1]*binormal[0];

	oonorm=1.0/sqrtf(normal[0]*normal[0]+normal[1]*normal[1]+normal[2]*normal[2]);
	normal[0]*=oonorm;			//I had a bug here, it was normal[6]
	normal[1]*=oonorm;			//I had a bug here, it was normal[7]
	normal[2]*=oonorm;			//I had a bug here, it was normal[8]

	//Gram-Schmidt orthogonalization process for B
	//Compute the cross product B=NxT to obtain an orthogonal basis
	//(Without this it's possible that complete orthogonality is not acheived)
	binormal[0]=normal[1]*tangent[2]-normal[2]*tangent[1];
	binormal[1]=normal[2]*tangent[0]-normal[0]*tangent[2];
	binormal[2]=normal[0]*tangent[1]-normal[1]*tangent[0];

	if(normal[0]*faceNormal[0]+normal[1]*faceNormal[1]+normal[2]*faceNormal[2]<0.0)
	{
		normal[0]=-normal[0];
		normal[1]=-normal[1];
		normal[2]=-normal[2];
	}
}

//PURPOSE:	In this version, I wonly want tangent as output. I don't want the binormal
//
//PARAMETERS:
//			[out] tangent : xyz, normalized
//			[in] v0 : xyz vertex
//			[in] v1 : xyz vertex
//			[in] v2 : xyz vertex
//			[in] t0 : st tex coord
//			[in] t1 : st tex coord
inline void ComputeOrthonormalBasis_v3_FLOAT(sreal *tangent, const sreal *v0, const sreal *v1, const sreal *v2,
								const sreal *t0, const sreal *t1, const sreal *t2)
{
	sreal cp[2];
	sreal e0[3], e1[3];
	e0[0]=v1[0]-v0[0];		//x-x
	e0[1]=t1[0]-t0[0];		//s-s
	e0[2]=t1[1]-t0[1];		//t-t
	e1[0]=v2[0]-v0[0];		//x-x
	e1[1]=t2[0]-t0[0];		//s-s
	e1[2]=t2[1]-t0[1];		//t-t

	//Compute the cross product cp = e0 CROSS e1
	cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	//The cross product of the two edge vectors yields
	//a normal vector to the plane in which they lie.
	//This vector defines a plane equation.
	//Ax + Bu + Cv + D = 0 
	//Assume Cv + D = 0
	// x = -Bu/A
	//
	//Assume Bu + D = 0
	//dudx = -B/A (basis[0])
	//dvdy = -C/A (basis[3])

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[0]=-cp[1]/cp[0];
	}

	e0[0]=v1[1]-v0[1];
	e1[0]=v2[1]-v0[1];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);
	cp[2]=(e0[0]*e1[1])-(e0[1]*e1[0]);

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[1]=-cp[1]/cp[0];
	}

	e0[0]=v1[2]-v0[2];
	e1[0]=v2[2]-v0[2];

	//Compute the cross product cp = e0 CROSS e1
	//cp[0]=(e0[1]*e1[2])-(e0[2]*e1[1]);	//Will not change
	cp[1]=(e0[2]*e1[0])-(e0[0]*e1[2]);

	if(fabsf(cp[0])>1.0e-6)
	{
		tangent[2]=-cp[1]/cp[0];
	}

	//Tangent... in first column of the matrix
	sreal oonorm=1.0/sqrtf(tangent[0]*tangent[0]+tangent[1]*tangent[1]+tangent[2]*tangent[2]);
	tangent[0]*=oonorm;
	tangent[1]*=oonorm;
	tangent[2]*=oonorm;
}

//PURPOSE:	Compute the orthonormal basis matrix (3x3) using the normals
//			of each vertex and the texture coordinates of each vertex.
//			The matrix is column major.
//			The matrix is stored in 3 different vectors here because it can become useful
//			if we want to do vertex_programs in OpenGL.
//			basisColumn3 is just normal3D1
//NOTE: I'm not sure if this code is CORRECT.
inline void ComputeOrthonormalBasisForVertex_FLOAT(sreal *basisColumn1, sreal *basisColumn2, sreal *basisColumn3, const sreal *vertex3D1, const sreal *vertex3D2, const sreal *vertex3D3,
	const sreal *texCoord2D1, const sreal *texCoord2D2, const sreal *texCoord2D3,
	const sreal *normal3D1)
{
	sreal edge0[3], edge1[3];
	sreal du, dv;
	sreal constant;

	edge0[0]=vertex3D2[0]-vertex3D1[0];
	edge0[1]=vertex3D2[1]-vertex3D1[1];
	edge0[2]=vertex3D2[2]-vertex3D1[2];
	edge1[0]=vertex3D3[0]-vertex3D1[0];
	edge1[1]=vertex3D3[1]-vertex3D1[1];
	edge1[2]=vertex3D3[2]-vertex3D1[2];
	du=texCoord2D2[0]-texCoord2D1[0];
	dv=texCoord2D2[1]-texCoord2D1[0];
	//Scale edge vectors by texture edge vectors
	edge0[0]*=du;
	edge0[1]*=du;
	edge0[2]*=du;
	edge1[0]*=dv;
	edge1[1]*=dv;
	edge1[2]*=dv;
	basisColumn1[0]=edge0[0]-edge1[0];
	basisColumn1[1]=edge0[1]-edge1[1];
	basisColumn1[2]=edge0[2]-edge1[2];
	//Normalize the tangent
	constant=1.0/sqrtf(basisColumn1[0]*basisColumn1[0]+basisColumn1[1]*basisColumn1[1]+basisColumn1[2]*basisColumn1[2]);
	basisColumn1[0]*=constant;
	basisColumn1[1]*=constant;
	basisColumn1[2]*=constant;
	//Crossproduct normal x tangent to get the binormal
	basisColumn2[0]=normal3D1[1]*basisColumn1[2]-normal3D1[2]*basisColumn1[1];
	basisColumn2[1]=normal3D1[2]*basisColumn1[0]-normal3D1[0]*basisColumn1[2];
	basisColumn2[2]=normal3D1[0]*basisColumn1[1]-normal3D1[1]*basisColumn1[0];

	//Redo the crossproduct
	basisColumn1[0]=normal3D1[2]*basisColumn2[1]-normal3D1[1]*basisColumn2[2];
	basisColumn1[1]=normal3D1[0]*basisColumn2[2]-normal3D1[2]*basisColumn2[0];
	basisColumn1[2]=normal3D1[1]*basisColumn2[0]-normal3D1[0]*basisColumn2[1];

	//Copy the normal
	basisColumn3[0]=normal3D1[0];
	basisColumn3[1]=normal3D1[1];
	basisColumn3[2]=normal3D1[2];
}

//PURPOSE: The following set of functions ar for cimputing volume and area.
//         Some of the equation come from Petit Larousse Illustre 1991, the words
//         are "aire" and "volume" p1030

//PURPOSE: This is for a basic cube
inline sreal ComputeVolumeOfCubeFLOAT_1(sreal side)
{
	return side*side*side;
}

//PURPOSE: This has a rectangular faces
inline sreal ComputeVolumeOfParallelipedeFLOAT_1(sreal width, sreal length, sreal height)
{
	return width*length*height;
}

//PURPOSE: All face are isoceles triangles
inline sreal ComputeVolumeOfTetrahedraFLOAT_1(sreal side, sreal height)
{
	return (side*side*0.5*height)/3.0;
}

inline sreal ComputeVolumeOfTetrahedraFLOAT_2(sreal base, sreal height)
{
	return (base*height)/3.0;
}

//PURPOSE: 
inline sreal ComputeVolumeOfSphereFLOAT_1(sreal radius)
{	//volume=4/3*PI*r^3 or =1/6*PI*D^3 where D is the diameter
	return 4.0/3.0*3.141592654*radius*radius*radius;
}

//PURPOSE: A segment a a slice through the sphere
//         -----
//        /     \     <--- cut from here
//        |     |     <--- cut from here
//        \     /
//         -----
inline sreal ComputeVolumeOfSphericalSegmentFLOAT_1(sreal baseArea1, sreal baseArea2, sreal height)
{	//volume=1/6*PI*height^3+(BaseArea1+BaseArea2)/2*height
	return 1.0/6.0*3.141592654*height*height*height+(baseArea1+baseArea2)*0.5*height;
}

//PURPOSE:
inline sreal ComputeVolumeOfSphericalRingFLOAT_1(sreal c, sreal height)
{
	return 1.0/6.0*3.141592654*c*c*height;
}

//PURPOSE:
inline sreal ComputeVolumeOfSphericalSectorFLOAT_1(sreal radius, sreal height)
{
	return 2.0/3.0*3.141592654*radius*radius*height;
}

//PURPOSE:
inline sreal ComputeVolumeOfGoldBrickFLOAT_1(sreal a, sreal a_prime, sreal l, sreal l_prime, sreal height)
{
	return height/6.0*(l*(2.0*a+a_prime)+l_prime*(2.0*a_prime+a));
}

//PURPOSE:
inline sreal ComputeVolumeOfWineBarrelFLOAT_1(sreal l, sreal d, sreal D)
{
	return 3.141592654*l*(d*0.5+2.0/3.0*(D*0.5-d*0.5))*(d*0.5+2.0/3.0*(D*0.5-d*0.5));
}

//PURPOSE:
inline sreal ComputeVolumeOfTruncatedPrismFLOAT_1(sreal base, sreal height, sreal height_prime, sreal height_primeprime)
{
	return base*(height+height_prime+height_primeprime)/3.0;
}

//PURPOSE:
inline sreal ComputeVolumeOfTruncatedPyramidFLOAT_1(sreal Base, sreal base, sreal height)
{
	return height/3.0*(Base+base+sqrtf(Base*base));
}

//PURPOSE:
inline sreal ComputeVolumeOfPyramidFLOAT_1(sreal base, sreal height)
{
	return (base*height)/3.0;
}

//PURPOSE:
inline sreal ComputeVolumeOfConeFLOAT_1(sreal radius, sreal height)
{
	return (3.141592654*radius*radius*height)/3.0;
}

//PURPOSE:
inline sreal ComputeVolumeOfCylinderFLOAT_1(sreal radius, sreal height)
{
	return 3.141592654*radius*radius*height;
}

//PURPOSE:
inline sreal ComputeVolumeOfTorusFLOAT_1(sreal radius, sreal bigradius)
{	//volume=(PI*radius^2)(2*PI*bigradius)=2*PI^2*radius^2*bigradius
	return 19.7392088*radius*radius*bigradius;
}

//PURPOSE: This will compute the are of a triangle of any shape.
//         The method used here comes from Linear Algebra.
inline sreal ComputeAreaOfTriangleFLOAT_555a(const sreal *vertex3D1, const sreal *vertex3D2, const sreal *vertex3D3)
{
	//Check to see if base has an area
	/*if((vertex3D1[0]==vertex3D2[0])&&(vertex3D1[1]==vertex3D2[1])&&(vertex3D1[2]==vertex3D2[2]))
		return 0.0;
	if((vertex3D1[0]==vertex3D3[0])&&(vertex3D1[1]==vertex3D3[1])&&(vertex3D1[2]==vertex3D3[2]))
		return 0.0;
	if((vertex3D2[0]==vertex3D3[0])&&(vertex3D2[1]==vertex3D3[1])&&(vertex3D2[2]==vertex3D3[2]))
		return 0.0;*/

	sreal vector1[3], vector2[3], normal3D[3];
	vector1[0]=vertex3D2[0]-vertex3D1[0];
	vector1[1]=vertex3D2[1]-vertex3D1[1];
	vector1[2]=vertex3D2[2]-vertex3D1[2];
	vector2[0]=vertex3D3[0]-vertex3D1[0];
	vector2[1]=vertex3D3[1]-vertex3D1[1];
	vector2[2]=vertex3D3[2]-vertex3D1[2];
	ComputeNormalOfPlaneFLOAT_2(normal3D, vector1, vector2);
	if((normal3D[0]==0.0)&&(normal3D[1]==0.0)&&(normal3D[2]==0.0))
		return 0.0;

	return 0.5*sqrtf(normal3D[0]*normal3D[0]+normal3D[1]*normal3D[1]+normal3D[2]*normal3D[2]);
}

inline sreal ComputeAreaOfTriangleFLOAT_555b(const sreal *vertex3D)
{
	//Check to see if base has an area
	/*if((vertex3D1[0]==vertex3D2[0])&&(vertex3D1[1]==vertex3D2[1])&&(vertex3D1[2]==vertex3D2[2]))
		return 0.0;
	if((vertex3D1[0]==vertex3D3[0])&&(vertex3D1[1]==vertex3D3[1])&&(vertex3D1[2]==vertex3D3[2]))
		return 0.0;
	if((vertex3D2[0]==vertex3D3[0])&&(vertex3D2[1]==vertex3D3[1])&&(vertex3D2[2]==vertex3D3[2]))
		return 0.0;*/

	sreal vector1[3], vector2[3], normal3D[3];
	vector1[0]=vertex3D[3]-vertex3D[0];
	vector1[1]=vertex3D[4]-vertex3D[1];
	vector1[2]=vertex3D[5]-vertex3D[2];
	vector2[0]=vertex3D[6]-vertex3D[0];
	vector2[1]=vertex3D[7]-vertex3D[1];
	vector2[2]=vertex3D[8]-vertex3D[2];
	ComputeNormalOfPlaneFLOAT_2(normal3D, vector1, vector2);
	if((normal3D[0]==0.0)&&(normal3D[1]==0.0)&&(normal3D[2]==0.0))
		return 0.0;

	return 0.5*sqrtf(normal3D[0]*normal3D[0]+normal3D[1]*normal3D[1]+normal3D[2]*normal3D[2]);
}

//PURPOSE: This will compute the are of a triangle of any shape.
//         The method used here is called accurate. It comes from Heron of Alexandria
inline sreal ComputeAreaOfTriangleFLOAT_556(const sreal *vertex3D1, const sreal *vertex3D2, const sreal *vertex3D3)
{
	//Check to see if base has an area
	/*if((vertex3D1[0]==vertex3D2[0])&&(vertex3D1[1]==vertex3D2[1])&&(vertex3D1[2]==vertex3D2[2]))
		return 0.0;
	if((vertex3D1[0]==vertex3D3[0])&&(vertex3D1[1]==vertex3D3[1])&&(vertex3D1[2]==vertex3D3[2]))
		return 0.0;
	if((vertex3D2[0]==vertex3D3[0])&&(vertex3D2[1]==vertex3D3[1])&&(vertex3D2[2]==vertex3D3[2]))
		return 0.0;*/

	sreal vector1[3], vector2[3], normal3D[3];
	vector1[0]=vertex3D2[0]-vertex3D1[0];
	vector1[1]=vertex3D2[1]-vertex3D1[1];
	vector1[2]=vertex3D2[2]-vertex3D1[2];
	vector2[0]=vertex3D3[0]-vertex3D1[0];
	vector2[1]=vertex3D3[1]-vertex3D1[1];
	vector2[2]=vertex3D3[2]-vertex3D1[2];
	ComputeNormalOfPlaneFLOAT_2(normal3D, vector1, vector2);
	if((normal3D[0]==0.0)&&(normal3D[1]==0.0)&&(normal3D[2]==0.0))
		return 0.0;

	sreal length1, length2, length3, vector3[3];
	vector3[0]=vertex3D3[0]-vertex3D2[0];
	vector3[1]=vertex3D3[1]-vertex3D2[1];
	vector3[2]=vertex3D3[2]-vertex3D2[2];
	length1=sqrtf(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
	length2=sqrtf(vector2[0]*vector2[0]+vector2[1]*vector2[1]+vector2[2]*vector2[2]);
	length3=sqrtf(vector3[0]*vector3[0]+vector3[1]*vector3[1]+vector3[2]*vector3[2]);

	return sqrtf(((length1+length2+length3)*(length2-length3+length1)*(length3-length1+length2)*(length1-length2+length3)))/4.0;
}

//PURPOSE: This will compute the are of a triangle of any shape.
//         The equation used here may yeild less accuracy than
//         ComputeAreaOfTriangleFLOAT_556 or ComputeAreaOfTriangleFLOAT_555
//         It comes from Heron of Alexandria
inline sreal ComputeAreaOfTriangleFLOAT_557(const sreal *vertex3D1, const sreal *vertex3D2, const sreal *vertex3D3)
{
	//Check to see if base has an area
	/*if((vertex3D1[0]==vertex3D2[0])&&(vertex3D1[1]==vertex3D2[1])&&(vertex3D1[2]==vertex3D2[2]))
		return 0.0;
	if((vertex3D1[0]==vertex3D3[0])&&(vertex3D1[1]==vertex3D3[1])&&(vertex3D1[2]==vertex3D3[2]))
		return 0.0;
	if((vertex3D2[0]==vertex3D3[0])&&(vertex3D2[1]==vertex3D3[1])&&(vertex3D2[2]==vertex3D3[2]))
		return 0.0;*/

	sreal vector1[3], vector2[3], normal3D[3];
	vector1[0]=vertex3D2[0]-vertex3D1[0];
	vector1[1]=vertex3D2[1]-vertex3D1[1];
	vector1[2]=vertex3D2[2]-vertex3D1[2];
	vector2[0]=vertex3D3[0]-vertex3D1[0];
	vector2[1]=vertex3D3[1]-vertex3D1[1];
	vector2[2]=vertex3D3[2]-vertex3D1[2];
	ComputeNormalOfPlaneFLOAT_2(normal3D, vector1, vector2);
	if((normal3D[0]==0.0)&&(normal3D[1]==0.0)&&(normal3D[2]==0.0))
		return 0.0;

	sreal length1, length2, length3, vector3[3];
	vector3[0]=vertex3D3[0]-vertex3D2[0];
	vector3[1]=vertex3D3[1]-vertex3D2[1];
	vector3[2]=vertex3D3[2]-vertex3D2[2];
	length1=sqrtf(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
	length2=sqrtf(vector2[0]*vector2[0]+vector2[1]*vector2[1]+vector2[2]*vector2[2]);
	length3=sqrtf(vector3[0]*vector3[0]+vector3[1]*vector3[1]+vector3[2]*vector3[2]);

	sreal s=(length1+length2+length3)/2.0;
	return sqrtf(s*(s-length1)*(s-length2)*(s-length3));
}

//PURPOSE:	Linear interpolation
inline sreal LinearInterpolation(sreal a, sreal b, sreal weightOfb)
{
	//return  a*(1.0-weightOfb)+b*weightOfb;
	return a+weightOfb*(b-a);	//Better!
}

//PURPOSE:	Unlike linear interpolation, this gives a smoother interpolation
inline sreal CosineInterpolation(sreal a, sreal b, sreal weightOfb)
{
	//f=(1.0-cosf(weightOfb*M_PI))*0.5;
	//return  a*(1.0-f)+b*f;
	//return a+f*(b-a);	//Better!
	return a+((1.0-cosf(weightOfb*M_PI))*0.5)*(b-a);	//Better!
}

//PURPOSE:	Unlike linear interpolation, this gives a smoother interpolation
//			but is slower than CosineInterpolate and doesn't give a huge improvement
//			but it's there.
inline sreal CubicInterpolation(sreal point_before_a, sreal a, sreal b, sreal point_after_b, sreal weightOfb)
{
	/*P = (v3 - v2) - (v0 - v1)
	Q = (v0 - v1) - P
	R = v2 - v0
	S = v1

	return Px3 + Qx2 + Rx + S*/

	sreal P=(point_after_b-b)+(point_before_a-a);
	sreal Q=(point_before_a-a)-P;
	sreal R=b-point_before_a;
	return P*weightOfb*weightOfb*weightOfb+
		Q*weightOfb*weightOfb+
		a;
}

//PURPOSE:	According to globalWeight from 0.0 to 1.0 usually, but not necessary.
//			Assign weight to weightCount number of variables.
//			The assignment follows a bell curve based on globalWeight.
//			Sum of all of weights will be 1.0
inline void MultiLinearInterpolatorf_1(sreal weightDistributionControl, sint weightCount, sreal *weights, sreal globalWeight)
{
	//This is like a bell curve.
	//Max is at x = globalWeight
	//Max is 1.0
	//y = exp(-(x-globalWeight)*(x-globalWeight))

	sint i;
	sreal x, n, sum;
	sreal start, increment;

	start=0.0;
	increment=1.0/(sreal)weightCount;

	sum=0.0;
	for(i=0; i<weightCount; i++)
	{
		x=start-globalWeight;
		weights[i]=expf(-x*x*weightDistributionControl);
		sum+=weights[i];
		start+=increment;
	}

	//Normalize
	n=1.0/sum;
	for(i=0; i<weightCount; i++)
	{
		weights[i]*=n;
	}
}

//PURPOSE:	Based on 3 vertices, generate plane equation
//			pvertex3D1 is first, pvertex3D2 is second, pvertex3D3 is the tail.
inline void GeneratePlaneEquationFromVertices_v1(sreal *planeEquation, sreal *pvertex3D1, sreal *pvertex3D2, sreal *pvertex3D3)
{
	ComputeNormalOfPlaneFLOAT_3(planeEquation, pvertex3D1, pvertex3D2, pvertex3D3);
	NormalizeVectorFLOAT_2(planeEquation);

	//Ax + By + Cz + D = 0
	//-(Ax + By + Cz) = D
	planeEquation[3]=-(planeEquation[0]*pvertex3D3[0]+planeEquation[1]*pvertex3D3[1]+planeEquation[2]*pvertex3D3[2]);
}

//PURPOSE:	Given 2 xyz points, compute distance.
//			Return distance^2
inline sreal DistanceSquareBetweenTwoPoints_FLOAT(sreal *ppoint1, sreal *ppoint2)
{
	sreal pvector[3];

	pvector[0]=ppoint1[0]-ppoint2[0];
	pvector[1]=ppoint1[1]-ppoint2[1];
	pvector[2]=ppoint1[2]-ppoint2[2];

	return pvector[0]*pvector[0]+pvector[1]*pvector[1]+pvector[2]*pvector[2];
}




#endif		//_MATHLIBRARY_H_

