// glhlib9.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
//#include <GL/gl.h>
//#include <GL/glext.h>
#include <math.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"
#include "GlobalsExternDeclarations.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'





extern ushort	_eLut[512];



//PURPOSE:	Used by SqrDistanceRayTriangle_FLOAT_SSE()
//			Use SSE whenever possible
//
//PARAMETERS:
//			[in] rayPoint1 : XYZW
//			[in] rayDirection : XYZW
//			[in] segmentPoint1 : XYZW
//			[in] segmentDirection : XYZW
//			[in] pfRayP : 
//			[in] pfSegP : 
//INCOMPLETE
sreal SqrDistanceRaySegment_FLOAT_SSE_Aligned(sreal *rayPoint1, sreal *rayDirection, sreal *segmentPoint1,
								  sreal *segmentDirection, sreal *pfRayP, sreal *pfSegP)
{
	/*sreal kDiff[3];
	sreal rayDirectionSquaredLength;
	sreal fA01;
	sreal fA11;
	sreal fB0;
	sreal fC;
	sreal fDet;
	sreal fB1, fS, fT, fSqrDist, fTmp;

	//There is a lot of dot products which sucks for SSE, but we can at least do parallel multiplication, addition,
	//subtraction

	__asm
	{
		lea		eax, kDiff
		mov		ebx, rayPoint1
		mov		ecx, segmentPoint1

		movaps	xmm0, [ebx]
		movaps	xmm1, [ecx]
		subps	xmm0, xmm1

		mov		ecx, rayDirection
		mov		edx, FArray32_SSE_ALIGNED
		movaps	xmm2, [ecx]
		mulps	xmm2, xmm2
		movaps	[edx], xmm2

		mov		eax, rayDirectionSquaredLength

		//DOT
	sreal rayDirectionSquaredLength=rayDirection[0]*rayDirection[0]+rayDirection[1]*rayDirection[1]+rayDirection[2]*rayDirection[2];
	//DOT
	sreal fA01=-(rayDirection[0]*segmentDirection[0]+rayDirection[1]*segmentDirection[1]+rayDirection[2]*segmentDirection[2]);
	//DOT
	sreal fA11=segmentDirection[0]*segmentDirection[0]+segmentDirection[1]*segmentDirection[1]+segmentDirection[2]*segmentDirection[2];
	//DOT
	sreal fB0=kDiff[0]*rayDirection[0]+kDiff[1]*rayDirection[1]+kDiff[2]*rayDirection[2];
	//DOT
	sreal fC=kDiff[0]*kDiff[0]+kDiff[1]*kDiff[1]+kDiff[2]*kDiff[2];
	sreal fDet=fabsf(rayDirectionSquaredLength*fA11-fA01*fA01);
	sreal fB1, fS, fT, fSqrDist, fTmp;

	if(fDet>=1e-05)		//gs_fTolerance=1e-05
	{
		//Ray and segment are not parallel
		//DOT
		fB1=-(kDiff[0]*segmentDirection[0]+kDiff[1]*segmentDirection[1]+kDiff[2]*segmentDirection[2]);
		fS=fA01*fB1-fA11*fB0;
		fT=fA01*fB0-rayDirectionSquaredLength*fB1;
		if(fS>=0.0)
		{
			if(fT>=0.0)
			{
				if(fT<=fDet)		//region 0
				{
					//minimum at interior points of ray and segment
					sreal fInvDet=1.0/fDet;
					fS*=fInvDet;
					fT*=fInvDet;
					fSqrDist=fS*(rayDirectionSquaredLength*fS+fA01*fT+2.0*fB0)+fT*(fA01*fS+fA11*fT+2.0*fB1)+fC;
				}
				else		//region 1
				{
					fT=1.0;
					if(fB0>=-fA01)
					{
						fS=0.0;
						fSqrDist=fA11+2.0*fB1+fC;
					}
					else
					{
						fTmp=fA01+fB0;
						fS=-fTmp/rayDirectionSquaredLength;
						fSqrDist=fTmp*fS+fA11+2.0*fB1+fC;
					}
				}
			}
			else		//region 5
			{
				fT=0.0;
				if(fB0>=0.0)
				{
					fS=0.0;
					fSqrDist=fC;
				}
				else
				{
					fS=-fB0/rayDirectionSquaredLength;
					fSqrDist=fB0*fS+fC;
				}
			}
		}
		else
		{
			if(fT<=0.0)		//region 4
			{
				if(fB0<0.0)
				{
					fS=-fB0/rayDirectionSquaredLength;
					fT=0.0;
					fSqrDist=fB0*fS+fC;
				}
				else
				{
					fS=0.0;
					if(fB1>=0.0)
					{
						fT=0.0;
						fSqrDist=fC;
					}
					else if(-fB1>=fA11)
					{
						fT=1.0;
						fSqrDist=fA11+2.0*fB1+fC;
					}
					else
					{
						fT=-fB1/fA11;
						fSqrDist=fB1*fT+fC;
					}
				}
			}
			else if(fT<=fDet)		//region 3
			{
				fS=0.0;
				if(fB1>=0.0)
				{
					fT=0.0;
					fSqrDist=fC;
				}
				else if(-fB1>=fA11)
				{
					fT=1.0;
					fSqrDist=fA11+2.0*fB1+fC;
				}
				else
				{
					fT=-fB1/fA11;
					fSqrDist=fB1*fT+fC;
				}
			}
			else		//region 2
			{
				fTmp=fA01+fB0;
				if(fTmp<0.0)
				{
					fS=-fTmp/rayDirectionSquaredLength;
					fT=1.0;
					fSqrDist=fTmp*fS+fA11+2.0*fB1+fC;
				}
				else
				{
					fS=0.0;
					if(fB1>=0.0)
					{
						fT=0.0;
						fSqrDist=fC;
					}
					else if(-fB1>=fA11)
					{
						fT=1.0;
						fSqrDist=fA11+2.0*fB1+fC;
					}
					else
					{
						fT=-fB1/fA11;
						fSqrDist=fB1*fT+fC;
					}
				}
			}
		}
	}
	else
	{
		//Ray and segment are parallel
		if(fA01>0.0)
		{
			//Opposite direction vectors
			fT=0.0;
			if(fB0>=0.0)
			{
				fS=0.0;
				fSqrDist=fC;
			}
			else
			{
				fS=-fB0/rayDirectionSquaredLength;
				fSqrDist=fB0*fS+fC;
			}
		}
		else
		{
			//Same direction vectors
			fB1=-(kDiff[0]*segmentDirection[0]+kDiff[1]*segmentDirection[1]+kDiff[2]*segmentDirection[2]);
			fT=1.0;
            fTmp=fA01+fB0;
			if(fTmp>=0.0)
			{
				fS=0.0;
				fSqrDist=fA11+2.0*fB1+fC;
			}
			else
			{
				fS=-fTmp/rayDirectionSquaredLength;
				fSqrDist=fTmp*fS+fA11+2.0*fB1+fC;
			}
		}
	}
	
	if(pfRayP)
		*pfRayP=fS;
	if(pfSegP)
		*pfSegP=fT;

	return fabsf(fSqrDist);*/
	return 1.0;
}


void glhTranslatef2_SSE_Aligned(sreal *matrix, sreal *xyzw)
{
#if !USE_INLINE_ASM
	glhTranslatef2(matrix, xyzw[0], xyzw[1], xyzw[2]);
#else
	__asm
	{
		mov		eax, matrix
		mov		ecx, xyzw

		movaps	xmm0, [eax]						; copy first column
		movaps	xmm7, [ecx]

		movaps	xmm4, xmm7
		movaps	xmm1, [eax+16]					; copy second column
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits. xxxx
		movaps	xmm2, [eax+32]					; copy third column
		movaps	xmm5, xmm7
		movaps	xmm3, [eax+48]					; copy fourth column
		shufps	xmm5, xmm5, 0x55				; second 32 bit copied to all 32 bits. yyyy
		shufps	xmm7, xmm7, 0xAA				; third 32 bit copied to all 32 bits. zzzz

		mulps	xmm0, xmm4						; This is matrix[0]*x, matrix[1]*x, matrix[2]*x, matrix[3]*x
		mulps	xmm1, xmm5						; This is matrix[4]*y, matrix[5]*y, matrix[6]*y, matrix[7]*y
		mulps	xmm2, xmm7						; This is matrix[8]*z, matrix[9]*z, matrix[10]*z, matrix[11]*z

		addps	xmm0, xmm1						; Add first and second
		addps	xmm0, xmm2						; Add previous and third
		addps	xmm0, xmm3						; Add previous and the last

		movaps	[eax+48], xmm0					; copy to matrix[12], matrix[13], matrix[14], matrix[15]
	}
#endif
}

void glhScalef2_SSE_Aligned(sreal *matrix, sreal *xyzw)
{
#if !USE_INLINE_ASM
	glhScalef2(matrix, xyzw[0], xyzw[1], xyzw[2]);
#else
	__asm
	{
		mov		eax, matrix
		mov		ecx, xyzw

		movaps	xmm0, [eax]						; copy first column
		movaps	xmm7, [ecx]

		movaps	xmm4, xmm7
		shufps	xmm4, xmm4, 0xC0				; first 32 bit copied to position 0, 1, 2. last 32 bit copied to 3 (same position)
		movaps	xmm1, [eax+16]					; copy second column
		mulps	xmm0, xmm4						; matrix[0]*=x, matrix[1]*=x, matrix[2]*=x, matrix[3]*=w
		movaps	[eax], xmm0
		movaps	xmm4, xmm7
		shufps	xmm4, xmm4, 0xD5				; second 32 bit copied to position 0, 1, 2. last 32 bit copied to 3 (same position)
		movaps	xmm2, [eax+32]					; copy third column
		mulps	xmm1, xmm4						; matrix[4]*=y, matrix[5]*=y, matrix[6]*=y, matrix[7]*=w
		movaps	[eax+16], xmm1
		shufps	xmm7, xmm7, 0xEA				; third 32 bit copied to position 0, 1, 2. last 32 bit copied to 3 (same position)
		mulps	xmm2, xmm7						; matrix[8]*=z, matrix[9]*=z, matrix[10]*=z, matrix[11]*=w
		movaps	[eax+32], xmm2

		; The last column stays the same
	}
#endif
}


void glhRotatef2_SSE_Aligned(sreal *matrix, sreal angleInRadians, sreal *xyzw)
{
#if !USE_INLINE_ASM
	glhTranslatef2(matrix, xyzw[0], xyzw[1], xyzw[2]);
#else
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal OneMinusCosAngle, CosAngle, SinAngle;
	sreal A_OneMinusCosAngle, C_OneMinusCosAngle;
	CosAngle=cosf(angleInRadians);			//Some stuff for optimizing code
	OneMinusCosAngle=1.0-CosAngle;
	SinAngle=sinf(angleInRadians);
	A_OneMinusCosAngle=xyzw[0]*OneMinusCosAngle;
	C_OneMinusCosAngle=xyzw[2]*OneMinusCosAngle;
	//Make a copy
	FArray128[0]=matrix[0];
	FArray128[1]=matrix[1];
	FArray128[2]=matrix[2];
	FArray128[3]=matrix[3];
	FArray128[4]=matrix[4];
	FArray128[5]=matrix[5];
	FArray128[6]=matrix[6];
	FArray128[7]=matrix[7];
	FArray128[8]=matrix[8];
	FArray128[9]=matrix[9];
	FArray128[10]=matrix[10];
	FArray128[11]=matrix[11];
	FArray128[12]=matrix[12];
	FArray128[13]=matrix[13];
	FArray128[14]=matrix[14];
	FArray128[15]=matrix[15];

	//Matrix rotation matrix
	FArray128[16]=xyzw[0]*A_OneMinusCosAngle+CosAngle;
	FArray128[17]=xyzw[1]*A_OneMinusCosAngle+xyzw[2]*SinAngle;
	FArray128[18]=xyzw[2]*A_OneMinusCosAngle-xyzw[1]*SinAngle;
	FArray128[19]=0.0;

	FArray128[20]=xyzw[1]*A_OneMinusCosAngle-xyzw[2]*SinAngle;
	FArray128[21]=xyzw[1]*xyzw[1]*OneMinusCosAngle+CosAngle;
	FArray128[22]=xyzw[1]*C_OneMinusCosAngle+xyzw[0]*SinAngle;
	FArray128[23]=0.0;

	FArray128[24]=xyzw[0]*C_OneMinusCosAngle+xyzw[1]*SinAngle;
	FArray128[25]=xyzw[1]*C_OneMinusCosAngle-xyzw[0]*SinAngle;
	FArray128[26]=xyzw[2]*C_OneMinusCosAngle+CosAngle;
	FArray128[27]=0.0;
	//The last column of rotate[] is {0 0 0 1}
	FArray128[28]=FArray128[29]=FArray128[30]=0.0;
	FArray128[31]=1.0;

	//We can do the matrix multiplication with SSE
	__asm
	{
		mov		eax, FArray128

		//Cache to L1
		prefetchnta	[eax]						; cache the left matrix second column now
		prefetchnta	[eax+16]					; cache the left matrix second column now
		prefetchnta	[eax+32]					; cache the left matrix second column now
		prefetchnta	[eax+48]					; cache the left matrix second column now

		mov		edx, matrix

		//Cache to L1
		prefetchnta [eax+80]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [eax+64]					; aligned move, get first column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx], xmm0

		//Cache to L1
		prefetchnta [eax+96]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [eax+80]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+16], xmm0

		//Cache to L1
		prefetchnta [eax+112]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [eax+96]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+32], xmm0

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [eax+112]					; aligned move, get fourth column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+48], xmm0
	}
#endif
}


void glhMultMatrixf2_SSE_Aligned(sreal *result_leftMatrix, sreal *rightMatrix)
{
#if !USE_INLINE_ASM
	glhMultMatrixf2(result_leftMatrix, rightMatrix);
#else
	//result_leftMatrix = result_leftMatrix * rightMatrix
	memcpy(FArray128, result_leftMatrix, 16*sizeof(sreal));

	//We can do the matrix multiplication with SSE
	__asm
	{
		mov		eax, FArray128

		//Cache to L1
		prefetchnta	[eax]						; cache the left matrix second column now
		prefetchnta	[eax+16]					; cache the left matrix second column now
		prefetchnta	[eax+32]					; cache the left matrix second column now
		prefetchnta	[eax+48]					; cache the left matrix second column now

		mov		edx, result_leftMatrix
		mov		ecx, rightMatrix

		//Cache to L1
		prefetchnta [ecx+16]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx]						; aligned move, get first column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx], xmm0

		//Cache to L1
		prefetchnta [ecx+32]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+16], xmm0

		//Cache to L1
		prefetchnta [ecx+48]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+32], xmm0

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+48]					; aligned move, get fourth column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+48], xmm0
	}
#endif
}


void glhMultMatrixSwappedf2_SSE_Aligned(sreal *result_rightMatrix, sreal *leftMatrix)
{
#if !USE_INLINE_ASM
	glhMultMatrixSwappedf2(result_rightMatrix, leftMatrix);
#else
	//result_rightMatrix = leftMatrix * result_rightMatrix
	memcpy(FArray128, result_rightMatrix, 16*sizeof(sreal));

	//We can do the matrix multiplication with SSE
	__asm
	{
		mov		eax, leftMatrix

		//Cache to L1
		prefetchnta	[eax]						; cache the left matrix second column now
		prefetchnta	[eax+16]					; cache the left matrix second column now
		prefetchnta	[eax+32]					; cache the left matrix second column now
		prefetchnta	[eax+48]					; cache the left matrix second column now

		mov		edx, result_rightMatrix
		mov		ecx, FArray128

		//Cache to L1
		prefetchnta [ecx+16]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx]						; aligned move, get first column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx], xmm0

		//Cache to L1
		prefetchnta [ecx+32]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+16], xmm0

		//Cache to L1
		prefetchnta [ecx+48]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+32], xmm0

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+48]					; aligned move, get fourth column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+48], xmm0
	}
#endif
}

void glhRotationMatrixFromRotationsf2(sreal *result_Matrix, sreal *R1, sreal *R2)
{
	sreal result[16];

	glhQuickInvertMatrixf2(R1, result);
	memcpy(result_Matrix, R2, sizeof(sreal)*16);
	glhMultMatrixf2(result_Matrix, result);
}


sint glhMultiplyVectorByMatrix4by4f_1(sint totalVector4D, sreal *vector4D,
				sreal *matrix,
				sreal *result4D)
{
	sint i;
	for(i=0; i<totalVector4D; i+=4)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&result4D[i], matrix, &vector3D[i]);
		result4D[i  ]=vector4D[i]*matrix[0]+
			vector4D[i+1]*matrix[1]+
			vector4D[i+2]*matrix[2]+
			vector4D[i+3]*matrix[3];		//w is 1
		result4D[i+1]=vector4D[i]*matrix[4]+
			vector4D[i+1]*matrix[5]+
			vector4D[i+2]*matrix[6]+
			vector4D[i+3]*matrix[7];
		result4D[i+2]=vector4D[i]*matrix[8]+
			vector4D[i+1]*matrix[9]+
			vector4D[i+2]*matrix[10]+
			vector4D[i+3]*matrix[11];
		result4D[i+3]=vector4D[i]*matrix[12]+
			vector4D[i+1]*matrix[13]+
			vector4D[i+2]*matrix[14]+
			vector4D[i+3]*matrix[15];
	}
	return 0;
}



sint glhMultiplyVectorByMatrix4by4f_3(sint totalVector3D, sreal *vector3D,
				sreal *matrix,
				sreal *result3D)
{
	sint i;
	for(i=0; i<totalVector3D; i+=3)
	{
		result3D[i  ]=vector3D[i]*matrix[0]+
			vector3D[i+1]*matrix[1]+
			vector3D[i+2]*matrix[2];				//w is 0
		result3D[i+1]=vector3D[i]*matrix[4]+
			vector3D[i+1]*matrix[5]+
			vector3D[i+2]*matrix[6];
		result3D[i+2]=vector3D[i]*matrix[8]+
			vector3D[i+1]*matrix[9]+
			vector3D[i+2]*matrix[10];
	}
	return 0;
}


sint glhMultiplyVectorByMatrix4by4f_2(sint totalVector3D, sreal *vector3D,
				sreal *matrix,
				sreal *result3D)
{
	sint i;
	for(i=0; i<totalVector3D; i+=3)
	{
		result3D[i  ]=vector3D[i]*matrix[0]+
			vector3D[i+1]*matrix[1]+
			vector3D[i+2]*matrix[2]+
			matrix[3];				//w is 1
		result3D[i+1]=vector3D[i]*matrix[4]+
			vector3D[i+1]*matrix[5]+
			vector3D[i+2]*matrix[6]+
			matrix[7];
		result3D[i+2]=vector3D[i]*matrix[8]+
			vector3D[i+1]*matrix[9]+
			vector3D[i+2]*matrix[10]+
			matrix[11];
	}
	return 0;
}


//-----------------------------------------------
// Overflow handler for float-to-half conversion;
// generates a hardware floating-point overflow,
// which may be trapped by the operating system.
//-----------------------------------------------

sreal HalfOverflow()
{
	volatile sreal f = 1e10;

	for(sint i = 0; i < 10; i++)
	{
		f *= f;				//This will overflow before
							//the for­loop terminates
	}

	return f;
}

//-----------------------------------------------------
// Float-to-half conversion -- general case, including
// zeroes, denormalized numbers and exponent overflows.
//-----------------------------------------------------
sshort FloatToHalfConvert(sint i)
{
	// Our floating point number, f, is represented by the bit
	// pattern in integer i.  Disassemble that bit pattern into
	// the sign, s, the exponent, e, and the significand, m.
	// Shift s into the position where it will go in in the
	// resulting half number.
	// Adjust e, accounting for the different exponent bias
	// of float and half (127 versus 15).


	register sint s =  (i >> 16) & 0x00008000;
	register sint e = ((i >> 23) & 0x000000ff) - (127 - 15);
	register sint m =   i        & 0x007fffff;

	//
	// Now reassemble s, e and m into a half:
	//

	if (e <= 0)
	{
		if (e < -10)
		{
			//
			// E is less than -10.  The absolute value of f is
			// less than HALF_MIN (f may be a small normalized
			// float, a denormalized float or a zero).
			//
			// We convert f to a half zero with the same sign as f.
			//

			return s;
		}

		//
		// E is between -10 and 0.  F is a normalized float
		// whose magnitude is less than HALF_NRM_MIN.
		//
		// We convert f to a denormalized half.
		//

		//
		// Add an explicit leading 1 to the significand.
		// 

		m = m | 0x00800000;

		//
		// Round to m to the nearest (10+e)-bit value (with e between
		// -10 and 0); in case of a tie, round to the nearest even value.
		//
		// Rounding may cause the significand to overflow and make
		// our number normalized.  Because of the way a half's bits
		// are laid out, we don't have to treat this case separately;
		// the code below will handle it correctly.
		// 

		sint t = 14 - e;
		sint a = (1 << (t - 1)) - 1;
		sint b = (m >> t) & 1;

		m = (m + a + b) >> t;

		//
		// Assemble the half from s, e (zero) and m.
		//

		return s | m;
	}
	else if (e == 0xff - (127 - 15))
	{
		if (m == 0)
		{
			//
			// F is an infinity; convert f to a half
			// infinity with the same sign as f.
			//

			return s | 0x7c00;
		}
		else
		{
			//
			// F is a NAN; we produce a half NAN that preserves
			// the sign bit and the 10 leftmost bits of the
			// significand of f, with one exception: If the 10
			// leftmost bits are all zero, the NAN would turn 
			// into an infinity, so we have to set at least one
			// bit in the significand.
			//

			m >>= 13;
			return s | 0x7c00 | m | (m == 0);
		}
	}
	else
	{
		//
		// E is greater than zero.  F is a normalized float.
		// We try to convert f to a normalized half.
		//

		//
		// Round to m to the nearest 10-bit value.  In case of
		// a tie, round to the nearest even value.
		//

		m = m + 0x00000fff + ((m >> 13) & 1);

		if (m & 0x00800000)
		{
			m =  0;		// overflow in significand,
			e += 1;		// adjust exponent
		}

		//
		// Handle exponent overflow
		//

		if (e > 30)
		{
			HalfOverflow();		//Cause a hardware floating point overflow;
			return s | 0x7c00;	//if this returns, the half becomes an
		}						//infinity with the same sign as f.

		//
		// Assemble the half from s, e and m.
		//

		return s | (e << 10) | (m >> 13);
	}
}

void glhFloatToHalf(sreal infloat, ushort &outhalf)
{
	if(infloat==0.0)
	{
		//Common special case - zero.
		//For speed, we don't preserve the zero's sign.
		outhalf=0;
	}
	else
	{
		//We extract the combined sign and exponent, e, from our
		//floating-point number, f.  Then we convert e to the sign
		//and exponent of the half number via a table lookup.
		//
		//For the most common case, where a normalized half is produced,
		//the table lookup returns a non-zero value; in this case, all
		//we have to do, is round f's significand to 10 bits and combine
		//the result with e.
		//
		//For all other cases (overflow, zeroes, denormalized numbers
		//resulting from underflow, infinities and NANs), the table
		//lookup returns zero, and we call a longer, non-inline function
		//to do the float-to-half conversion.
		union uif
		{
			uint	i;
			sreal	f;
		};
		uif x;

		x.f=infloat;

		register sint e=(x.i >> 23) & 0x000001ff;

		e=_eLut[e];

		if(e)
		{
			//Simple case - round the significand and
			//combine it with the sign and exponent.
			outhalf=e+(((x.i & 0x007fffff)+0x00001000)>>13);
		}
		else
		{
			//Difficult case - call a function.
			outhalf=FloatToHalfConvert(x.i);
		}
	}
}


void glhPickMatrixd2(sreal2 *matrix, sreal2 x, sreal2 y, sreal2 deltax, sreal2 deltay, sint *viewport)
{
	if((deltax<=0.0)||(deltay<=0.0))
		return;

	//Translate and scale the picked region to the entire window
	glhTranslated2(matrix, ((sreal2)viewport[2]-2.0*(x-(sreal2)viewport[0]))/deltax, ((sreal2)viewport[3]-2.0 *(y-(sreal2)viewport[1]))/deltay, 0.0);
	glhScaled2(matrix, (sreal2)viewport[2]/deltax, (sreal2)viewport[3]/deltay, 1.0);
}

void glhPickMatrixf2(sreal *matrix, sreal x, sreal y, sreal deltax, sreal deltay, sint *viewport)
{
	if((deltax<=0.0)||(deltay<=0.0))
		return;

	//Translate and scale the picked region to the entire window
	glhTranslatef2(matrix, ((sreal)viewport[2]-2.0*(x-(sreal)viewport[0]))/deltax, ((sreal)viewport[3]-2.0 *(y-(sreal)viewport[1]))/deltay, 0.0);
	glhScalef2(matrix, (sreal)viewport[2]/deltax, (sreal)viewport[3]/deltay, 1.0);
}





/*sint EmptyCache(GLHtesselator *tess)
{
	CachedVertex *v=tess->cache;
	CachedVertex *vLast;

	tess->mesh=__gl_meshNewMesh();
	if(tess->mesh==NULL)
		return 0;

	for(vLast=v+tess->cacheCount; v<vLast; v++)
	{
		if(!AddVertex(tess, v->coords, v->data))
			return 0;
	}

	tess->cacheCount=0;
	tess->emptyCache=FALSE;

	return 1;
}

void glhTessVertexf(GLHtesselator *tess, sreal coords[3], void *data)
{
	sint i;
	tbool tooLarge=FALSE;
	sreal x, clamped[3];

	RequireState(tess, T_IN_CONTOUR);

	if(tess->emptyCache)
	{
		if(!EmptyCache(tess))
		{
			CALL_ERROR_OR_ERROR_DATA(GLH_OUT_OF_MEMORY);
			return;
		}

		tess->lastEdge=NULL;
	}

	for(i=0; i<3; i++)
	{
		x=coords[i];
		if(x<-GLH_TESS_MAX_COORD)
		{
			x=-GLH_TESS_MAX_COORD;
			tooLarge=TRUE;
		}

		if(x>GLH_TESS_MAX_COORD)
		{
			x=GLH_TESS_MAX_COORD;
			tooLarge=TRUE;
		}

		clamped[i]=x;
	}

	if(tooLarge)
	{
		CALL_ERROR_OR_ERROR_DATA(GLH_TESS_COORD_TOO_LARGE);
	}

	if(tess->mesh==NULL)
	{
		if(tess->cacheCount<TESS_MAX_CACHE)
		{
			CacheVertex(tess, clamped, data);
			return;
		}

		if(!EmptyCache(tess))
		{
			CALL_ERROR_OR_ERROR_DATA(GLH_OUT_OF_MEMORY);
			return;
		}
	}

	if(!AddVertex(tess, clamped, data))
	{
		CALL_ERROR_OR_ERROR_DATA(GLH_OUT_OF_MEMORY);
	}
}

void glhTessBeginPolygonf(GLHtesselator *tess, void *data)
{
	RequireState(tess, T_DORMANT);

	tess->state=T_IN_POLYGON;
	tess->cacheCount=0;
	tess->emptyCache=FALSE;
	tess->mesh=NULL;

	tess->polygonData=data;
}

void glhTessBeginContourf(GLHtesselator *tess)
{
	RequireState(tess, T_IN_POLYGON);

	tess->state=T_IN_CONTOUR;
	tess->lastEdge=NULL;
	if(tess->cacheCount>0)
	{
		//Just set a flag so we don't get confused by empty contours
		// -- these can be generated accidentally with the obsolete
		// NextContour() interface.
		//
		tess->emptyCache=TRUE;
	}
}

void glhTessEndContourf(GLHtesselator *tess)
{
	RequireState(tess, T_IN_CONTOUR);
	tess->state=T_IN_POLYGON;
}

void glhTessEndPolygonf(GLHtesselator *tess)
{
	GLHmesh *mesh;

	if(setjmp(tess->env)!=0)
	{
		//come back here if out of memory
		CALL_ERROR_OR_ERROR_DATA(GLH_OUT_OF_MEMORY);
		return;
	}

	RequireState(tess, T_IN_POLYGON);
	tess->state=T_DORMANT;

	if(tess->mesh==NULL)
	{
		if((!tess->flagBoundary)&&(tess->callMesh==&noMesh))
		{
			// Try some special code to make the easy cases go quickly
			// (eg. convex polygons).  This code does NOT handle multiple contours,
			// intersections, edge flags, and of course it does not generate
			// an explicit mesh either.
			//
			if(__gl_renderCache(tess))
			{
				tess->polygonData=NULL;
				return;
			}
		}

		if(!EmptyCache(tess))
			longjmp(tess->env, 1);	//could've used a label
	}

	// Determine the polygon normal and project vertices onto the plane
	// of the polygon.
	//
	__gl_projectPolygon(tess);

	// __gl_computeInterior( tess ) computes the planar arrangement specified
	// by the given contours, and further subdivides this arrangement
	// into regions.  Each region is marked "inside" if it belongs
	// to the polygon, according to the rule given by tess->windingRule.
	// Each interior region is guaranteed be monotone.
	//
	if(!__gl_computeInterior(tess))
	{
		longjmp(tess->env, 1);	// could've used a label
	}

	mesh=tess->mesh;
	if(!tess->fatalError)
	{
		int rc=1;

		// If the user wants only the boundary contours, we throw away all edges
		// except those which separate the interior from the exterior.
		// Otherwise we tessellate all the regions marked "inside".
		//
		if(tess->boundaryOnly)
		{
			rc=__gl_meshSetWindingNumber(mesh, 1, TRUE);
		}
		else
		{
			rc=__gl_meshTessellateInterior(mesh);
		}

		if(rc==0)
			longjmp(tess->env, 1);	//could've used a label

		__gl_meshCheckMesh(mesh);

		if((tess->callBegin!=&noBegin)||(tess->callEnd!=&noEnd)
			||(tess->callVertex!=&noVertex)||(tess->callEdgeFlag!=&noEdgeFlag)
			||(tess->callBeginData!=&__gl_noBeginData)
			||(tess->callEndData!=&__gl_noEndData)
			||(tess->callVertexData!=&__gl_noVertexData
			||(tess->callEdgeFlagData!=&__gl_noEdgeFlagData))
		{
			if(tess->boundaryOnly)
			{
				__gl_renderBoundary(tess, mesh);	//output boundary contours
			}
			else
			{
				__gl_renderMesh(tess, mesh);		//output strips and fans
			}
		}

		if(tess->callMesh!=&noMesh)
		{
			// Throw away the exterior faces, so that all faces are interior.
			// This way the user doesn't have to check the "inside" flag,
			// and we don't need to even reveal its existence.  It also leaves
			// the freedom for an implementation to not generate the exterior
			// faces in the first place.
			//
			__gl_meshDiscardExterior(mesh);
			(*tess->callMesh)(mesh);		//user wants the mesh itself
			tess->mesh=NULL;
			tess->polygonData=NULL;
			return;
		}
	}

	__gl_meshDeleteMesh(mesh);
	tess->polygonData=NULL;
	tess->mesh=NULL;
}*/


