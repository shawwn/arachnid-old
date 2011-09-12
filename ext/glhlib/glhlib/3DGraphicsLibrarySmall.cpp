#include "stdafx.h"
#include "3DGraphicsLibrarySmall.h"
#include <math.h>



#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'





sint DoesRayCrossTriangle_6_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal rayDirectionLengthSquared,
												sreal *trianglePoint1, sreal *trianglePoint2,
												sreal *triangleVector1, sreal *triangleVector2,
												sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
												sreal triangleVector1_DOT_triangleVector2,
												sreal *pointOfIntersection)
{
	sreal fRayP;
	if(SqrDistanceRayTriangle_FLOAT(rayPoint1, rayDirection, rayDirectionLengthSquared,
		trianglePoint1, trianglePoint2, triangleVector1, triangleVector2,
		triangleVector1LengthSquared, triangleVector2LengthSquared,
		triangleVector1_DOT_triangleVector2,
		&fRayP)<=1e-12)		//fEpsilon=1e-12
	{
		pointOfIntersection[0]=rayPoint1[0]+fRayP*rayDirection[0];
		pointOfIntersection[1]=rayPoint1[1]+fRayP*rayDirection[1];
		pointOfIntersection[2]=rayPoint1[2]+fRayP*rayDirection[2];
		return 1;			//Intersection occured
	}
	
	return 0;				//Intersection did not occur
}
//Used by DoesRayCrossTriangleWithNormalVector_6_FLOAT()
sreal SqrDistanceRayTriangle_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal rayDirectionLengthSquared,
					sreal *trianglePoint1, sreal *trianglePoint2,
					sreal *triangleVector1, sreal *triangleVector2,
					sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
					sreal triangleVector1_DOT_triangleVector2,
					sreal *pfRayP)
{
	//Length^2 of rayDirection
	//sreal fA00=rayDirection[0]*rayDirection[0]+
	//	rayDirection[1]*rayDirection[1]+
	//	rayDirection[2]*rayDirection[2];
	//-(rayDirection DOT triangle_vector1)
	sreal fA01=-(rayDirection[0]*triangleVector1[0]+
		rayDirection[1]*triangleVector1[1]+
		rayDirection[2]*triangleVector1[2]);
	sreal fA02=-(rayDirection[0]*triangleVector2[0]+
		rayDirection[1]*triangleVector2[1]+
		rayDirection[2]*triangleVector2[2]);
	//Length^2 of triangleVector1
	//sreal fA11=triangleVector1[0]*triangleVector1[0]+
	//	triangleVector1[0]*triangleVector1[0]
	//	triangleVector1[0]*triangleVector10]
	//triangleVector1 DOT triangleVector2
	//sreal fA12=triangleVector1[0]*triangleVector2[0]+
	//	triangleVector1[1]*triangleVector2[1]+
	//	triangleVector1[2]*triangleVector2[2];
	//triangleVector1 DOT triangleVector1
	//sreal fA22=triangleVector2[0]*triangleVector2[0]+
	//	triangleVector2[1]*triangleVector2[1]+
	//	triangleVector2[2]*triangleVector2[2];
	
	sreal kDiff[3];
	kDiff[0]=trianglePoint1[0]-rayPoint1[0];
	kDiff[1]=trianglePoint1[1]-rayPoint1[1];
	kDiff[2]=trianglePoint1[2]-rayPoint1[2];

	//DOT product
	sreal fB0		=	-(kDiff[0]*rayDirection[0]+kDiff[1]*rayDirection[1]+kDiff[2]*rayDirection[2]);
	//DOT product
	sreal fB1		=	kDiff[0]*triangleVector1[0]+kDiff[1]*triangleVector1[1]+kDiff[2]*triangleVector1[2];
	//DOT product
	sreal fB2		=	kDiff[0]*triangleVector2[0]+kDiff[1]*triangleVector2[1]+kDiff[2]*triangleVector2[2];
	sreal fCof00	=	(triangleVector1LengthSquared*triangleVector2LengthSquared)-
		(triangleVector1_DOT_triangleVector2*triangleVector1_DOT_triangleVector2);
	sreal fCof01	=	(fA02*triangleVector1_DOT_triangleVector2)-(fA01*triangleVector2LengthSquared);
	sreal fCof02	=	(fA01*triangleVector1_DOT_triangleVector2)-(fA02*triangleVector1LengthSquared);
	sreal fDet		=	(rayDirectionLengthSquared*fCof00)+(fA01*fCof01)+(fA02*fCof02);

	sreal kTriSeg[6];	//A segment with a starting point and direction
	sreal fSqrDist, fSqrDist0, fR, fS, fT, fR0, fS0, fT0;

	if(fabsf(fDet)>=1e-05)		//fTolerance=1e-05;
	{
		sreal fCof11	=	rayDirectionLengthSquared*triangleVector2LengthSquared-fA02*fA02;
		sreal fCof12	=	fA02*fA01-rayDirectionLengthSquared*triangleVector1_DOT_triangleVector2;
		sreal fCof22	=	rayDirectionLengthSquared*triangleVector1LengthSquared-fA01*fA01;
		sreal fInvDet	=	1.0/fDet;
		sreal fRhs0		=	-fB0*fInvDet;
		sreal fRhs1		=	-fB1*fInvDet;
		sreal fRhs2		=	-fB2*fInvDet;

		fR	=	fCof00*fRhs0+fCof01*fRhs1+fCof02*fRhs2;
		fS	=	fCof01*fRhs0+fCof11*fRhs1+fCof12*fRhs2;
		fT	=	fCof02*fRhs0+fCof12*fRhs1+fCof22*fRhs2;

		if(fR<=0.0)
		{
			if(fS+fT<=1.0)
			{
				if(fS<0.0)
				{
					if(fT<0.0)		//region 4m
					{
						//min on face s=0 or t=0 or r=0
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector2[0];
						kTriSeg[4]=triangleVector2[1];
						kTriSeg[5]=triangleVector2[2];
						fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], &fR, &fT);
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector1[0];
						kTriSeg[4]=triangleVector1[1];
						kTriSeg[5]=triangleVector1[2];
						fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], &fR0, &fS0);
						fS=fT0=0.0;
						if(fSqrDist0<fSqrDist)
						{
							fSqrDist=fSqrDist0;
							fR=fR0;
							fS=fS0;
							fT=fT0;
						}
						fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, 
							triangleVector1, triangleVector2, triangleVector1LengthSquared,
							triangleVector2LengthSquared, triangleVector1_DOT_triangleVector2,
							&fS0, &fT0);
						fR0=0.0;
						if(fSqrDist0<fSqrDist)
						{
							fSqrDist=fSqrDist0;
							fR=fR0;
							fS=fS0;
							fT=fT0;
						}
					}
					else	//region 3m
					{
						//min on face s=0 or r=0
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector2[0];
						kTriSeg[4]=triangleVector2[1];
						kTriSeg[5]=triangleVector2[2];
						fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], &fR, &fT);
						fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0, &fT0);
						fS=fR0=0.0;
						if(fSqrDist0<fSqrDist)
						{
							fSqrDist=fSqrDist0;
							fR=fR0;
							fS=fS0;
							fT=fT0;
						}
					}
				}
				else if(fT<0.0)		//region 5m
				{
					//min on face t=0 or r=0
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector1[0];
					kTriSeg[4]=triangleVector1[1];
					kTriSeg[5]=triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR, &fS);
					fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0, &fT0);
					fT=fR0=0.0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
				}
				else	//region 0m
				{
					//min on face r=0
					fSqrDist=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS, &fT);
					fR=0.0;
				}
			}
			else
			{
				if(fS<0.0)	//region 2m
				{
					//min on face s=0 or s+t=1 or r=0
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector2[0];
					kTriSeg[4]=triangleVector2[1];
					kTriSeg[5]=triangleVector2[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR, &fT);
					fS=0.0;
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR0, &fT0);
					fS0=1.0-fT0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
					fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0, &fT0);
					fR0=0.0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
				}
				else if(fT<0.0)		//region 6m
				{
					//min on face t=0 or s+t=1 or r=0
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector1[0];
					kTriSeg[4]=triangleVector1[1];
					kTriSeg[5]=triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR, &fS);
					fT=0.0;
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR0, &fT0);
					fS0=1.0-fT0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
					fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0, &fT0);
					fR0=0.0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
				}
				else	//region 1m
				{
					//min on face s+t=1 or r=0
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], &fR, &fT);
					fS=1.0-fT;
					fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0, &fT0);
					fR0=0.0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fR=fR0;
						fS=fS0;
						fT=fT0;
					}
				}
			}
		}
		else		//fR>0
		{
			if(fS+fT<=1.0)
			{
				if(fS<0.0)
				{
					if(fT<0.0)		//region 4p
					{
						//min on face s=0 or t=0
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector2[0];
						kTriSeg[4]=triangleVector2[1];
						kTriSeg[5]=triangleVector2[2];
						fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], NULL, &fT);
						fS=0.0;
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector1[0];
						kTriSeg[4]=triangleVector1[1];
						kTriSeg[5]=triangleVector1[2];
						fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], NULL, &fS0);
						fT0=0.0;
						if(fSqrDist0<fSqrDist)
						{
							fSqrDist=fSqrDist0;
							fS=fS0;
							fT=fT0;
						}
					}
					else		//region 3p
					{
						//min on face s=0
						kTriSeg[0]=trianglePoint1[0];
						kTriSeg[1]=trianglePoint1[1];
						kTriSeg[2]=trianglePoint1[2];
						kTriSeg[3]=triangleVector2[0];
						kTriSeg[4]=triangleVector2[1];
						kTriSeg[5]=triangleVector2[2];
						fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
							&kTriSeg[3], NULL, &fT);
						fS=0.0;
					}
				}
				else if(fT<0.0)		//region 5p
				{
					//min on face t=0
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector1[0];
					kTriSeg[4]=triangleVector1[1];
					kTriSeg[5]=triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fS);
					fT=0.0;
				}
				else			//region 0p
				{
					//ray intersects triangle
					fSqrDist=0.0;
				}
			}
			else
			{
				if(fS<0.0)		//region 2p
				{
					//min on face s=0 or s+t=1
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector2[0];
					kTriSeg[4]=triangleVector2[1];
					kTriSeg[5]=triangleVector2[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fT);
					fS=0.0;
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fT0);
					fS0=1.0-fT0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fS=fS0;
						fT=fT0;
					}
				}
				else if(fT<0.0)		//region 6p
				{
					//min on face t=0 or s+t=1
					kTriSeg[0]=trianglePoint1[0];
					kTriSeg[1]=trianglePoint1[1];
					kTriSeg[2]=trianglePoint1[2];
					kTriSeg[3]=triangleVector1[0];
					kTriSeg[4]=triangleVector1[1];
					kTriSeg[5]=triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fS);
					fT=0.0;
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fT0);
					fS0=1.0-fT0;
					if(fSqrDist0<fSqrDist)
					{
						fSqrDist=fSqrDist0;
						fS=fS0;
						fT=fT0;
					}
				}
				else	//region 1p
				{
					//min on face s+t=1
					kTriSeg[0]=trianglePoint2[0];
					kTriSeg[1]=trianglePoint2[1];
					kTriSeg[2]=trianglePoint2[2];
					kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
					kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
					kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
					fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
						&kTriSeg[3], NULL, &fT);
					fS=1.0-fT;
				}
			}
		}
	}
	else
	{
		//Ray and triangle are parallel
		kTriSeg[0]=trianglePoint1[0];
		kTriSeg[1]=trianglePoint1[1];
		kTriSeg[2]=trianglePoint1[2];
		kTriSeg[3]=triangleVector1[0];
		kTriSeg[4]=triangleVector1[1];
		kTriSeg[5]=triangleVector1[2];
		fSqrDist=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
			&kTriSeg[3], &fR, &fS);
		
		kTriSeg[3]=triangleVector2[0];
		kTriSeg[4]=triangleVector2[1];
		kTriSeg[5]=triangleVector2[2];
		fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fR0, &fT0);
		fT=fS0=0.0;
		if(fSqrDist0<fSqrDist)
		{
			fSqrDist=fSqrDist0;
			fR=fR0;
			fS=fS0;
			fT=fT0;
		}

		kTriSeg[0]=trianglePoint2[0];
		kTriSeg[1]=trianglePoint2[1];
		kTriSeg[2]=trianglePoint2[2];
		kTriSeg[3]=triangleVector2[0]-triangleVector1[0];
		kTriSeg[4]=triangleVector2[1]-triangleVector1[1];
		kTriSeg[5]=triangleVector2[2]-triangleVector1[2];
		fSqrDist0=SqrDistanceRaySegment_FLOAT(rayPoint1, rayDirection, kTriSeg,
			&kTriSeg[3], &fR0, &fT0);
		fS0=1.0-fT0;
		if(fSqrDist0<fSqrDist)
		{
			fSqrDist=fSqrDist0;
			fR=fR0;
			fS=fS0;
			fT=fT0;
		}

		fSqrDist0=SqrDistancePointTriangle_FLOAT(rayPoint1, trianglePoint1, triangleVector1,
							triangleVector2, triangleVector1LengthSquared, triangleVector2LengthSquared,
							triangleVector1_DOT_triangleVector2, &fS0,&fT0);
		fR0=0.0;
		if(fSqrDist0<fSqrDist)
		{
			fSqrDist=fSqrDist0;
			fR=fR0;
			fS=fS0;
			fT=fT0;
		}
	}

	*pfRayP=fR;

	return fSqrDist;
	//Final check!!!
}
//Used by SqrDistanceRayTriangle_FLOAT()
sreal SqrDistancePointTriangle_FLOAT(sreal *rkPoint, sreal *trianglepoint1, sreal *triangleVector1,
									sreal *triangleVector2,
									sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
									sreal triangleVector1_DOT_triangleVector2,
									sreal *pfSParam, sreal *pfTParam)
{
	sreal kDiff[3];
	kDiff[0]=trianglepoint1[0]-rkPoint[0];
	kDiff[1]=trianglepoint1[1]-rkPoint[1];
	kDiff[2]=trianglepoint1[2]-rkPoint[2];
	//sreal fA00=rkTri.Edge0().SquaredLength();
	//sreal fA01=rkTri.Edge0().Dot(rkTri.Edge1());
	//sreal fA11=rkTri.Edge1().SquaredLength();

	//DOT
	sreal fB0=kDiff[0]*triangleVector1[0]+kDiff[1]*triangleVector1[1]+kDiff[2]*triangleVector1[2];
	//DOT
	sreal fB1=kDiff[0]*triangleVector2[0]+kDiff[1]*triangleVector2[1]+kDiff[2]*triangleVector2[2];
	//DOT
	sreal fC=kDiff[0]*kDiff[0]+kDiff[1]*kDiff[1]+kDiff[2]*kDiff[2];
	sreal fDet=fabsf(triangleVector1LengthSquared*triangleVector2LengthSquared-triangleVector1_DOT_triangleVector2*triangleVector1_DOT_triangleVector2);
	sreal fS=triangleVector1_DOT_triangleVector2*fB1-triangleVector2LengthSquared*fB0;
	sreal fT=triangleVector1_DOT_triangleVector2*fB0-triangleVector1LengthSquared*fB1;
	sreal fSqrDist;

	if(fS+fT<=fDet)
	{
		if(fS<0.0)
		{
			if(fT<0.0)		//region 4
			{
				if(fB0<0.0)
				{
					fT=0.0;
					if(-fB0>=triangleVector1LengthSquared)
					{
						fS=1.0;
						fSqrDist=triangleVector1LengthSquared+2.0*fB0+fC;
					}
					else
					{
						fS=-fB0/triangleVector1LengthSquared;
						fSqrDist=fB0*fS+fC;
					}
				}
				else
				{
					fS=0.0;
					if(fB1>=0.0)
					{
						fT=0.0;
						fSqrDist=fC;
					}
					else if(-fB1>=triangleVector2LengthSquared)
					{
						fT=1.0;
						fSqrDist=triangleVector2LengthSquared+2.0*fB1+fC;
					}
					else
					{
						fT=-fB1/triangleVector2LengthSquared;
						fSqrDist=fB1*fT+fC;
					}
				}
			}
			else		//region 3
			{
				fS=0.0;
				if(fB1>=0.0)
				{
					fT=0.0;
					fSqrDist=fC;
				}
				else if(-fB1>=triangleVector2LengthSquared)
				{
					fT=1.0;
					fSqrDist=triangleVector2LengthSquared+2.0*fB1+fC;
				}
				else
				{
					fT=-fB1/triangleVector2LengthSquared;
					fSqrDist=fB1*fT+fC;
				}
			}
		}
		else if(fT<0.0)		//region 5
		{
			fT=0.0;
			if(fB0>=0.0)
			{
				fS=0.0;
				fSqrDist=fC;
			}
			else if(-fB0>=triangleVector1LengthSquared)
			{
				fS=1.0;
				fSqrDist=triangleVector1LengthSquared+2.0*fB0+fC;
			}
			else
			{
				fS=-fB0/triangleVector1LengthSquared;
				fSqrDist=fB0*fS+fC;
			}
		}
		else		//region 0
		{
			//minimum at interior point
			sreal fInvDet=1.0/fDet;
			fS*=fInvDet;
			fT*=fInvDet;
			fSqrDist=fS*(triangleVector1LengthSquared*fS+triangleVector1_DOT_triangleVector2*fT+2.0*fB0)+fT*(triangleVector1_DOT_triangleVector2*fS+triangleVector2LengthSquared*fT+2.0*fB1)+fC;
		}
	}
	else
	{
		sreal fTmp0, fTmp1, fNumer, fDenom;
		if(fS<0.0)		//region 2
		{
			fTmp0=triangleVector1_DOT_triangleVector2+fB0;
			fTmp1=triangleVector2LengthSquared+fB1;
			if(fTmp1>fTmp0)
			{
				fNumer=fTmp1-fTmp0;
				fDenom=triangleVector1LengthSquared-2.0*triangleVector1_DOT_triangleVector2+triangleVector2LengthSquared;
				if(fNumer>=fDenom)
				{
					fS=1.0;
					fT=0.0;
					fSqrDist=triangleVector1LengthSquared+2.0*fB0+fC;
				}
				else
				{
					fS=fNumer/fDenom;
					fT=1.0-fS;
					fSqrDist=fS*(triangleVector1LengthSquared*fS+triangleVector1_DOT_triangleVector2*fT+2.0*fB0)+fT*(triangleVector1_DOT_triangleVector2*fS+triangleVector2LengthSquared*fT+2.0*fB1)+fC;
				}
			}
			else
			{
				fS=0.0;
				if(fTmp1<=0.0)
				{
					fT=1.0;
					fSqrDist=triangleVector2LengthSquared+2.0*fB1+fC;
				}
				else if(fB1>=0.0)
				{
					fT=0.0;
					fSqrDist=fC;
				}
				else
				{
					fT=-fB1/triangleVector2LengthSquared;
					fSqrDist=fB1*fT+fC;
				}
			}
		}
		else if(fT<0.0)		//region 6
		{
			fTmp0=triangleVector1_DOT_triangleVector2+fB1;
			fTmp1=triangleVector1LengthSquared+fB0;
			if(fTmp1>fTmp0)
			{
				fNumer=fTmp1-fTmp0;
				fDenom=triangleVector1LengthSquared-2.0*triangleVector1_DOT_triangleVector2+triangleVector2LengthSquared;
				if(fNumer>=fDenom)
				{
					fT=1.0;
					fS=0.0;
					fSqrDist=triangleVector2LengthSquared+2.0*fB1+fC;
				}
				else
				{
					fT=fNumer/fDenom;
					fS=1.0-fT;
					fSqrDist=fS*(triangleVector1LengthSquared*fS+triangleVector1_DOT_triangleVector2*fT+2.0*fB0)+fT*(triangleVector1_DOT_triangleVector2*fS+triangleVector2LengthSquared*fT+2.0*fB1)+fC;
				}
			}
			else
			{
				fT=0.0;
				if(fTmp1<=0.0)
				{
					fS=1.0;
					fSqrDist=triangleVector1LengthSquared+2.0*fB0+fC;
				}
				else if(fB0>=0.0)
				{
					fS=0.0;
					fSqrDist=fC;
				}
				else
				{
					fS=-fB0/triangleVector1LengthSquared;
					fSqrDist=fB0*fS+fC;
				}
			}
		}
		else		//region 1
		{
			fNumer=triangleVector2LengthSquared+fB1-triangleVector1_DOT_triangleVector2-fB0;
			if(fNumer<=0.0)
			{
				fS=0.0;
				fT=1.0;
				fSqrDist=triangleVector2LengthSquared+2.0*fB1+fC;
			}
			else
			{
				fDenom=triangleVector1LengthSquared-2.0*triangleVector1_DOT_triangleVector2+triangleVector2LengthSquared;
				if(fNumer>=fDenom)
				{
					fS=1.0;
					fT=0.0;
					fSqrDist=triangleVector1LengthSquared+2.0*fB0+fC;
				}
				else
				{
					fS=fNumer/fDenom;
					fT=1.0-fS;
					fSqrDist=fS*(triangleVector1LengthSquared*fS+triangleVector1_DOT_triangleVector2*fT+2.0*fB0)+fT*(triangleVector1_DOT_triangleVector2*fS+triangleVector2LengthSquared*fT+2.0*fB1)+fC;
				}
			}
		}
	}

	*pfSParam=fS;
	*pfTParam=fT;

	return fabsf(fSqrDist);
}
//Used by SqrDistanceRayTriangle_FLOAT()
sreal SqrDistanceRaySegment_FLOAT(sreal *rayPoint1, sreal *rayDirection, sreal *segmentPoint1,
								  sreal *segmentDirection, sreal *pfRayP, sreal *pfSegP)
{
	sreal kDiff[3];
	kDiff[0]=rayPoint1[0]-segmentPoint1[0];
	kDiff[1]=rayPoint1[1]-segmentPoint1[1];
	kDiff[2]=rayPoint1[2]-segmentPoint1[2];
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

	return fabsf(fSqrDist);
}

sint ComputeTangentAndBinormalVectors_v1_FLOAT(sint totalIndices, uint *pindices, sint totalVertex, sint numCompVertex, sreal *pvertex, sint numCompNormals, sreal *pnormals, sint numCompTexCoord, sreal *ptexCoord, sint numCompTangent, sreal *ptangent, sint numCompBinormal, sreal *pbinormal)
{
	if((totalIndices==0)||(totalVertex==0))
		return -1;
	if((numCompVertex<3)||(numCompVertex>4))
		return -1;
	if((numCompNormals<3)||(numCompNormals>4))
		return -1;
	if((numCompTexCoord<2)||(numCompTexCoord>4))
		return -1;
	if((numCompTangent<3)||(numCompTangent>4))
		return -1;
	if((numCompBinormal<3)||(numCompBinormal>4))
		return -1;


	//Step 1. We will compute the tangent and binormal for each vertex in a triangle
	//Step 2. We have to average (normalize) the tangent and binormal for each vertex
	//The question is, how many triangles share this vertex. We must count for
	//each vertex, so a counter array is needed.
	sint i, j, k;
	sint index1, index2, index3;
	sint index1B, index2B, index3B;
	sint index1C, index2C, index3C;
	sint index1D, index2D, index3D;
	sint index1E, index2E, index3E;
	sint *vcounter;
	sreal tangent[3], binormal[3];
	sreal junknormal[3];		//We don't keep this
	sint realtotalVertex=totalVertex/numCompVertex;
	

	vcounter=new sint[totalVertex];
	if(vcounter==NULL)
		return 0;
	memset(vcounter, 0, sizeof(sint)*totalVertex);
	for(i=0; i<totalIndices; i+=3)
	{
		index1=(sint)(numCompVertex*pindices[i  ]);
		index2=(sint)(numCompVertex*pindices[i+1]);
		index3=(sint)(numCompVertex*pindices[i+2]);

		index1B=(sint)(numCompNormals*pindices[i  ]);
		index2B=(sint)(numCompNormals*pindices[i+1]);
		index3B=(sint)(numCompNormals*pindices[i+2]);

		index1C=(sint)(numCompTexCoord*pindices[i  ]);
		index2C=(sint)(numCompTexCoord*pindices[i+1]);
		index3C=(sint)(numCompTexCoord*pindices[i+2]);

		index1D=(sint)(numCompTangent*pindices[i  ]);
		index2D=(sint)(numCompTangent*pindices[i+1]);
		index3D=(sint)(numCompTangent*pindices[i+2]);

		index1E=(sint)(numCompBinormal*pindices[i  ]);
		index2E=(sint)(numCompBinormal*pindices[i+1]);
		index3E=(sint)(numCompBinormal*pindices[i+2]);


		//NOTE: I am passing a vertex normal instead of face, but it should be fine
		ComputeOrthonormalBasis_v2_FLOAT(tangent, binormal, junknormal, &pvertex[index1], &pvertex[index2], &pvertex[index3],
			&ptexCoord[index1C], &ptexCoord[index2C], &ptexCoord[index3C], &pnormals[index1B]);
		if(vcounter[index1]==0)
		{
			ptangent[index1D  ]=tangent[0];
			ptangent[index1D+1]=tangent[1];
			ptangent[index1D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index1D+3]=0.0;
			pbinormal[index1E  ]=binormal[0];
			pbinormal[index1E+1]=binormal[1];
			pbinormal[index1E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index1E+3]=0.0;
		}
		else
		{
			ptangent[index1D  ]+=tangent[0];
			ptangent[index1D+1]+=tangent[1];
			ptangent[index1D+2]+=tangent[2];
			pbinormal[index1E  ]+=binormal[0];
			pbinormal[index1E+1]+=binormal[1];
			pbinormal[index1E+2]+=binormal[2];
		}
		if(vcounter[index2]==0)
		{
			ptangent[index2D  ]=tangent[0];
			ptangent[index2D+1]=tangent[1];
			ptangent[index2D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index2D+3]=0.0;
			pbinormal[index2E  ]=binormal[0];
			pbinormal[index2E+1]=binormal[1];
			pbinormal[index2E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index2E+3]=0.0;
		}
		else
		{
			ptangent[index2D  ]+=tangent[0];
			ptangent[index2D+1]+=tangent[1];
			ptangent[index2D+2]+=tangent[2];
			pbinormal[index2E  ]+=binormal[0];
			pbinormal[index2E+1]+=binormal[1];
			pbinormal[index2E+2]+=binormal[2];
		}
		if(vcounter[index3]==0)
		{
			ptangent[index3D  ]=tangent[0];
			ptangent[index3D+1]=tangent[1];
			ptangent[index3D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index3D+3]=0.0;
			pbinormal[index3E  ]=binormal[0];
			pbinormal[index3E+1]=binormal[1];
			pbinormal[index3E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index3E+3]=0.0;
		}
		else
		{
			ptangent[index3D  ]+=tangent[0];
			ptangent[index3D+1]+=tangent[1];
			ptangent[index3D+2]+=tangent[2];
			pbinormal[index3E  ]+=binormal[0];
			pbinormal[index3E+1]+=binormal[1];
			pbinormal[index3E+2]+=binormal[2];
		}

		vcounter[index1]++;
		vcounter[index2]++;
		vcounter[index3]++;
	}

	//Normalize each tangent and binormal
	for(i=j=k=0; k<realtotalVertex; i+=numCompTangent, j+=numCompBinormal, k++)
	{
		NormalizeVectorFLOAT_2(&ptangent[i]);
		NormalizeVectorFLOAT_2(&pbinormal[j]);
	}

	delete [] vcounter;
	return 1;
}

sint ComputeTangentAndBinormalVectors_v1_FLOAT_2(sint totalIndices, ushort *pindices, sint totalVertex, sint numCompVertex, sreal *pvertex, sint numCompNormals, sreal *pnormals, sint numCompTexCoord, sreal *ptexCoord, sint numCompTangent, sreal *ptangent, sint numCompBinormal, sreal *pbinormal)
{
	if((totalIndices==0)||(totalVertex==0))
		return -1;
	if((numCompVertex<3)||(numCompVertex>4))
		return -1;
	if((numCompNormals<3)||(numCompNormals>4))
		return -1;
	if((numCompTexCoord<2)||(numCompTexCoord>4))
		return -1;
	if((numCompTangent<3)||(numCompTangent>4))
		return -1;
	if((numCompBinormal<3)||(numCompBinormal>4))
		return -1;

	//Step 1. We will compute the tangent and binormal for each vertex in a triangle
	//Step 2. We have to average (normalize) the tangent and binormal for each vertex
	//The question is, how many triangles share this vertex. We must count for
	//each vertex, so a counter array is needed.
	sint i, j, k;
	sint index1, index2, index3;
	sint index1B, index2B, index3B;
	sint index1C, index2C, index3C;
	sint index1D, index2D, index3D;
	sint index1E, index2E, index3E;
	sint *vcounter;
	sreal tangent[3], binormal[3];
	sreal junknormal[3];		//We don't keep this
	sint realtotalVertex=totalVertex/numCompVertex;
	

	vcounter=new sint[totalVertex];
	if(vcounter==NULL)
		return 0;

	memset(vcounter, 0, sizeof(sint)*totalVertex);
	for(i=0; i<totalIndices; i+=3)		//Each triangle is 3 indicea
	{
		index1=(sint)(numCompVertex*pindices[i  ]);
		index2=(sint)(numCompVertex*pindices[i+1]);
		index3=(sint)(numCompVertex*pindices[i+2]);

		index1B=(sint)(numCompNormals*pindices[i  ]);
		index2B=(sint)(numCompNormals*pindices[i+1]);
		index3B=(sint)(numCompNormals*pindices[i+2]);

		index1C=(sint)(numCompTexCoord*pindices[i  ]);
		index2C=(sint)(numCompTexCoord*pindices[i+1]);
		index3C=(sint)(numCompTexCoord*pindices[i+2]);

		index1D=(sint)(numCompTangent*pindices[i  ]);
		index2D=(sint)(numCompTangent*pindices[i+1]);
		index3D=(sint)(numCompTangent*pindices[i+2]);

		index1E=(sint)(numCompBinormal*pindices[i  ]);
		index2E=(sint)(numCompBinormal*pindices[i+1]);
		index3E=(sint)(numCompBinormal*pindices[i+2]);


		//NOTE: I am passing a vertex normal instead of face, but it should be fine
		ComputeOrthonormalBasis_v2_FLOAT(tangent, binormal, junknormal, &pvertex[index1], &pvertex[index2], &pvertex[index3],
			&ptexCoord[index1C], &ptexCoord[index2C], &ptexCoord[index3C], &pnormals[index1B]);
		if(vcounter[index1]==0)
		{
			ptangent[index1D  ]=tangent[0];
			ptangent[index1D+1]=tangent[1];
			ptangent[index1D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index1D+3]=0.0;
			pbinormal[index1E  ]=binormal[0];
			pbinormal[index1E+1]=binormal[1];
			pbinormal[index1E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index1E+3]=0.0;
		}
		else
		{
			ptangent[index1D  ]+=tangent[0];
			ptangent[index1D+1]+=tangent[1];
			ptangent[index1D+2]+=tangent[2];
			pbinormal[index1E  ]+=binormal[0];
			pbinormal[index1E+1]+=binormal[1];
			pbinormal[index1E+2]+=binormal[2];
		}
		if(vcounter[index2]==0)
		{
			ptangent[index2D  ]=tangent[0];
			ptangent[index2D+1]=tangent[1];
			ptangent[index2D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index2D+3]=0.0;
			pbinormal[index2E  ]=binormal[0];
			pbinormal[index2E+1]=binormal[1];
			pbinormal[index2E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index2E+3]=0.0;
		}
		else
		{
			ptangent[index2D  ]+=tangent[0];
			ptangent[index2D+1]+=tangent[1];
			ptangent[index2D+2]+=tangent[2];
			pbinormal[index2E  ]+=binormal[0];
			pbinormal[index2E+1]+=binormal[1];
			pbinormal[index2E+2]+=binormal[2];
		}
		if(vcounter[index3]==0)
		{
			ptangent[index3D  ]=tangent[0];
			ptangent[index3D+1]=tangent[1];
			ptangent[index3D+2]=tangent[2];
			if(numCompTangent==4)
				ptangent[index3D+3]=0.0;
			pbinormal[index3E  ]=binormal[0];
			pbinormal[index3E+1]=binormal[1];
			pbinormal[index3E+2]=binormal[2];
			if(numCompBinormal==4)
				pbinormal[index3E+3]=0.0;
		}
		else
		{
			ptangent[index3D  ]+=tangent[0];
			ptangent[index3D+1]+=tangent[1];
			ptangent[index3D+2]+=tangent[2];
			pbinormal[index3E  ]+=binormal[0];
			pbinormal[index3E+1]+=binormal[1];
			pbinormal[index3E+2]+=binormal[2];
		}

		vcounter[index1]++;
		vcounter[index2]++;
		vcounter[index3]++;
	}

	//Normalize each tangent and binormal
	for(i=j=k=0; k<realtotalVertex; i+=numCompTangent, j+=numCompBinormal, k++)
	{
		NormalizeVectorFLOAT_2(&ptangent[i]);
		NormalizeVectorFLOAT_2(&pbinormal[j]);
	}

	delete [] vcounter;
	return 1;
}




sint ComputeTangentAndBinormalVectors_v2_FLOAT(sint totalIndices, uint *pindices, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((totalIndices==0)||(vertexCount==0))
		return -1;

	//Step 1. We will compute the tangent and binormal for each vertex in a triangle
	//Step 2. We have to average (normalize) the tangent and binormal for each vertex
	//The question is, how many triangles share this vertex. We must count for
	//each vertex, so a counter array is needed.
	sint i, k;
	sint index1, index2, index3;
	sint *vcounter;
	sreal tangent[3], binormal[3];
	sreal junknormal[3];		//We don't keep this
	sint realtotalVertex=vertexCount;
	

	vcounter=new sint[vertexCount*3];
	if(vcounter==NULL)
		return 0;

	memset(vcounter, 0, sizeof(sint)*vertexCount*3);
	for(i=0; i<totalIndices; i+=3)		//Each triangle is 3 indicea
	{
		index1=(sint)(pindices[i  ]);
		index2=(sint)(pindices[i+1]);
		index3=(sint)(pindices[i+2]);

		//NOTE: I am passing a vertex normal instead of face, but it should be fine
		ComputeOrthonormalBasis_v2_FLOAT(tangent, binormal, junknormal, &pvertex[index1].x, &pvertex[index2].x, &pvertex[index3].x,
			&pvertex[index1].s0, &pvertex[index2].s0, &pvertex[index3].s0, &pvertex[index1].nx);
		if(vcounter[index1]==0)
		{
			//s1, t1, r1 are for tangent xyz
			//s2, t2, r2 are for binormal xyz
			pvertex[index1].s1=tangent[0];
			pvertex[index1].t1=tangent[1];
			pvertex[index1].r1=tangent[2];

			pvertex[index1].s2=binormal[0];
			pvertex[index1].t2=binormal[1];
			pvertex[index1].r2=binormal[2];
		}
		else
		{
			pvertex[index1].s1+=tangent[0];
			pvertex[index1].t1+=tangent[1];
			pvertex[index1].r1+=tangent[2];

			pvertex[index1].s2+=binormal[0];
			pvertex[index1].t2+=binormal[1];
			pvertex[index1].r2+=binormal[2];
		}
		if(vcounter[index2]==0)
		{
			pvertex[index2].s1=tangent[0];
			pvertex[index2].t1=tangent[1];
			pvertex[index2].r1=tangent[2];

			pvertex[index2].s2=binormal[0];
			pvertex[index2].t2=binormal[1];
			pvertex[index2].r2=binormal[2];
		}
		else
		{
			pvertex[index2].s1+=tangent[0];
			pvertex[index2].t1+=tangent[1];
			pvertex[index2].r1+=tangent[2];

			pvertex[index2].s2+=binormal[0];
			pvertex[index2].t2+=binormal[1];
			pvertex[index2].r2+=binormal[2];
		}
		if(vcounter[index3]==0)
		{
			pvertex[index3].s1=tangent[0];
			pvertex[index3].t1=tangent[1];
			pvertex[index3].r1=tangent[2];

			pvertex[index3].s2=binormal[0];
			pvertex[index3].t2=binormal[1];
			pvertex[index3].r2=binormal[2];
		}
		else
		{
			pvertex[index3].s1+=tangent[0];
			pvertex[index3].t1+=tangent[1];
			pvertex[index3].r1+=tangent[2];

			pvertex[index3].s2+=binormal[0];
			pvertex[index3].t2+=binormal[1];
			pvertex[index3].r2+=binormal[2];
		}

		vcounter[index1]++;
		vcounter[index2]++;
		vcounter[index3]++;
	}

	//Normalize each tangent and binormal
	for(k=0; k<realtotalVertex; k++)
	{
		NormalizeVectorFLOAT_2(&pvertex[k].s1);
		NormalizeVectorFLOAT_2(&pvertex[k].s2);
	}

	delete [] vcounter;
	return 1;

}

sint ComputeTangentAndBinormalVectors_v1_FLOAT_3(sint totalIndices, uint *pindices, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((totalIndices==0)||(vertexCount==0))
		return -1;


	//Step 1. We will compute the tangent and binormal for each vertex in a triangle
	//Step 2. We have to average (normalize) the tangent and binormal for each vertex
	//The question is, how many triangles share this vertex. We must count for
	//each vertex, so a counter array is needed.
	sint i;
	sint index1, index2, index3;
	sint *vcounter;
	sreal tangent[3], binormal[3];
	sreal junknormal[3];		//We don't keep this
	

	vcounter=new sint[vertexCount*3];
	if(vcounter==NULL)
		return 0;

	memset(vcounter, 0, sizeof(sint)*vertexCount);
	for(i=0; i<totalIndices; i+=3)		//Each triangle is 3 indicea
	{
		index1=(sint)(pindices[i  ]);
		index2=(sint)(pindices[i+1]);
		index3=(sint)(pindices[i+2]);

		//NOTE: I am passing a vertex normal instead of face, but it should be fine
		ComputeOrthonormalBasis_v2_FLOAT(tangent, binormal, junknormal, &pvertex[index1].x, &pvertex[index2].x, &pvertex[index3].x,
			&pvertex[index1].s0, &pvertex[index2].s0, &pvertex[index3].s0, &pvertex[index1].nx);
		if(vcounter[index1]==0)
		{
			pvertex[index1].s1=tangent[0];
			pvertex[index1].t1=tangent[1];
			pvertex[index1].r1=tangent[2];
			pvertex[index1].s2=binormal[0];
			pvertex[index1].t2=binormal[1];
			pvertex[index1].r2=binormal[2];
		}
		else
		{
			pvertex[index1].s1+=tangent[0];
			pvertex[index1].t1+=tangent[1];
			pvertex[index1].r1+=tangent[2];
			pvertex[index1].s2+=binormal[0];
			pvertex[index1].t2+=binormal[1];
			pvertex[index1].r2+=binormal[2];
		}
		if(vcounter[index2]==0)
		{
			pvertex[index2].s1=tangent[0];
			pvertex[index2].t1=tangent[1];
			pvertex[index2].r1=tangent[2];
			pvertex[index2].s2=binormal[0];
			pvertex[index2].t2=binormal[1];
			pvertex[index2].r2=binormal[2];
		}
		else
		{
			pvertex[index2].s1+=tangent[0];
			pvertex[index2].t1+=tangent[1];
			pvertex[index2].r1+=tangent[2];
			pvertex[index2].s2+=binormal[0];
			pvertex[index2].t2+=binormal[1];
			pvertex[index2].r2+=binormal[2];
		}
		if(vcounter[index3]==0)
		{
			pvertex[index3].s1=tangent[0];
			pvertex[index3].t1=tangent[1];
			pvertex[index3].r1=tangent[2];
			pvertex[index3].s2=binormal[0];
			pvertex[index3].t2=binormal[1];
			pvertex[index3].r2=binormal[2];
		}
		else
		{
			pvertex[index3].s1+=tangent[0];
			pvertex[index3].t1+=tangent[1];
			pvertex[index3].r1+=tangent[2];
			pvertex[index3].s2+=binormal[0];
			pvertex[index3].t2+=binormal[1];
			pvertex[index3].r2+=binormal[2];
		}

		vcounter[index1]++;
		vcounter[index2]++;
		vcounter[index3]++;
	}

	//Normalize each tangent and binormal
	for(i=0; i<vertexCount; i++)
	{
		NormalizeVectorFLOAT_2(&pvertex[i].s1);
		NormalizeVectorFLOAT_2(&pvertex[i].s2);
	}

	delete [] vcounter;
	return 1;
}

sint ComputeTangentAndBinormalVectors_v1_FLOAT_4(sint totalIndices, ushort *pindices, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((totalIndices==0)||(vertexCount==0))
		return -1;


	//Step 1. We will compute the tangent and binormal for each vertex in a triangle
	//Step 2. We have to average (normalize) the tangent and binormal for each vertex
	//The question is, how many triangles share this vertex. We must count for
	//each vertex, so a counter array is needed.
	sint i;
	sint index1, index2, index3;
	sint *vcounter;
	sreal tangent[3], binormal[3];
	sreal junknormal[3];		//We don't keep this
	

	vcounter=new sint[vertexCount*3];
	if(vcounter==NULL)
		return 0;

	memset(vcounter, 0, sizeof(sint)*vertexCount);
	for(i=0; i<totalIndices; i+=3)		//Each triangle is 3 indicea
	{
		index1=(sint)(pindices[i  ]);
		index2=(sint)(pindices[i+1]);
		index3=(sint)(pindices[i+2]);

		//NOTE: I am passing a vertex normal instead of face, but it should be fine
		ComputeOrthonormalBasis_v2_FLOAT(tangent, binormal, junknormal, &pvertex[index1].x, &pvertex[index2].x, &pvertex[index3].x,
			&pvertex[index1].s0, &pvertex[index2].s0, &pvertex[index3].s0, &pvertex[index1].nx);
		if(vcounter[index1]==0)
		{
			pvertex[index1].s1=tangent[0];
			pvertex[index1].t1=tangent[1];
			pvertex[index1].r1=tangent[2];
			pvertex[index1].s2=binormal[0];
			pvertex[index1].t2=binormal[1];
			pvertex[index1].r2=binormal[2];
		}
		else
		{
			pvertex[index1].s1+=tangent[0];
			pvertex[index1].t1+=tangent[1];
			pvertex[index1].r1+=tangent[2];
			pvertex[index1].s2+=binormal[0];
			pvertex[index1].t2+=binormal[1];
			pvertex[index1].r2+=binormal[2];
		}
		if(vcounter[index2]==0)
		{
			pvertex[index2].s1=tangent[0];
			pvertex[index2].t1=tangent[1];
			pvertex[index2].r1=tangent[2];
			pvertex[index2].s2=binormal[0];
			pvertex[index2].t2=binormal[1];
			pvertex[index2].r2=binormal[2];
		}
		else
		{
			pvertex[index2].s1+=tangent[0];
			pvertex[index2].t1+=tangent[1];
			pvertex[index2].r1+=tangent[2];
			pvertex[index2].s2+=binormal[0];
			pvertex[index2].t2+=binormal[1];
			pvertex[index2].r2+=binormal[2];
		}
		if(vcounter[index3]==0)
		{
			pvertex[index3].s1=tangent[0];
			pvertex[index3].t1=tangent[1];
			pvertex[index3].r1=tangent[2];
			pvertex[index3].s2=binormal[0];
			pvertex[index3].t2=binormal[1];
			pvertex[index3].r2=binormal[2];
		}
		else
		{
			pvertex[index3].s1+=tangent[0];
			pvertex[index3].t1+=tangent[1];
			pvertex[index3].r1+=tangent[2];
			pvertex[index3].s2+=binormal[0];
			pvertex[index3].t2+=binormal[1];
			pvertex[index3].r2+=binormal[2];
		}

		vcounter[index1]++;
		vcounter[index2]++;
		vcounter[index3]++;
	}

	//Normalize each tangent and binormal
	for(i=0; i<vertexCount; i++)
	{
		NormalizeVectorFLOAT_2(&pvertex[i].s1);
		NormalizeVectorFLOAT_2(&pvertex[i].s2);
	}

	delete [] vcounter;
	return 1;
}


sint GenerateNormalsForIndexList_FLOAT_3(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_4(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_5(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_6(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_7(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_8(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_9(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}

sint GenerateNormalsForIndexList_FLOAT_10(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i;

	//Index is a list of containing a list of 3 indices that make up a PRIMITIVE_TRIANGLES
	sint *vertexShareCount=new sint[vertexCount];				//How many times is this vertex shared?

	memset(vertexShareCount, 0, sizeof(sint)*(vertexCount));
	sreal *faceNormals3D=new sreal[totalIndex];							//(totalIndex/3)*3=totalIndex

	//First, compute face normals.
	for(i=0; i<totalIndex; i+=3)
	{
		ComputeNormalOfPlaneFLOAT_3(&faceNormals3D[i], &pvertex[pindex[i+1]].x, &pvertex[pindex[i+2]].x, &pvertex[pindex[i]].x);
		NormalizeVectorFLOAT_2(&faceNormals3D[i]);						//Normalize right here
		vertexShareCount[pindex[i  ]]++;
		vertexShareCount[pindex[i+1]]++;
		vertexShareCount[pindex[i+2]]++;
	}

	//Second, add the face normals for each vertex, we must also initialize to zero
	for(i=0; i<vertexCount; i++)
	{
		pvertex[i].nx=pvertex[i].ny=pvertex[i].nz=0.0;
	}


	for(i=0; i<totalIndex; i+=3)
	{
		pvertex[pindex[i]  ].nx+=faceNormals3D[i  ];
		pvertex[pindex[i]  ].ny+=faceNormals3D[i+1];
		pvertex[pindex[i]  ].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+1]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+1]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+1]].nz+=faceNormals3D[i+2];
		pvertex[pindex[i+2]].nx+=faceNormals3D[i  ];
		pvertex[pindex[i+2]].ny+=faceNormals3D[i+1];
		pvertex[pindex[i+2]].nz+=faceNormals3D[i+2];
	}

	//Third, we average the normals
	//sreal ftempo;
	for(i=0; i<vertexCount; i++)
	{
		//NO need to do this
		//ftempo=1.0/(sreal)vertexShareCount[i];
		//pvertex[i].nx*=ftempo;
		//pvertex[i].ny*=ftempo;
		//pvertex[i].nz*=ftempo;

		//Normalize one more time to make sure it's OK
		NormalizeVectorFLOAT_2(&pvertex[i].nx);
	}

	delete [] faceNormals3D;
	delete [] vertexShareCount;

	return 1;
}



sint GenerateNormalsForIndexList_Shared_FLOAT_3(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	ushort *pindex2=new ushort[totalIndex];
	ushort index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNT *pvertex2=new GLHVertex_VNT[vertexCount2];

	memcpy(pindex2, pindex, sizeof(ushort)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNT)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((ushort)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((ushort)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(ushort)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((ushort)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(ushort)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_3(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_4(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	uint *pindex2=new uint[totalIndex];
	uint index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNT *pvertex2=new GLHVertex_VNT[vertexCount2];

	memcpy(pindex2, pindex, sizeof(uint)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNT)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((uint)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((uint)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(uint)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((uint)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(uint)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_4(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_5(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	ushort *pindex2=new ushort[totalIndex];
	ushort index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNT3 *pvertex2=new GLHVertex_VNT3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(ushort)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNT3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((ushort)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((ushort)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(ushort)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((ushort)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(ushort)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_5(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_6(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	uint *pindex2=new uint[totalIndex];
	uint index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNT3 *pvertex2=new GLHVertex_VNT3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(uint)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNT3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((uint)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((uint)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(uint)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((uint)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNT3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(uint)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_6(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_7(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	ushort *pindex2=new ushort[totalIndex];
	ushort index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNTT3T3 *pvertex2=new GLHVertex_VNTT3T3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(ushort)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNTT3T3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((ushort)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((ushort)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3T3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(ushort)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((ushort)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3T3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(ushort)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_7(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_8(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3T3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	uint *pindex2=new uint[totalIndex];
	uint index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNTT3T3 *pvertex2=new GLHVertex_VNTT3T3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(uint)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNTT3T3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((uint)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((uint)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3T3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(uint)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((uint)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3T3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(uint)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_8(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_9(sint totalIndex, ushort *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	ushort *pindex2=new ushort[totalIndex];
	ushort index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNTT3 *pvertex2=new GLHVertex_VNTT3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(ushort)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNTT3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((ushort)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((ushort)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(ushort)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((ushort)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(ushort)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_9(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}

sint GenerateNormalsForIndexList_Shared_FLOAT_10(sint totalIndex, uint *pindex, sint vertexCount, GLHVertex_VNTT3 *pvertex)
{
	if((pindex==NULL)||(pvertex==NULL))
		return -1;
	if((totalIndex<=0)||(vertexCount<=0))
		return -1;


	sint i, j, k;
	uint *pindex2=new uint[totalIndex];
	uint index, index2;
	sint vertexCount2=vertexCount;
	GLHVertex_VNTT3 *pvertex2=new GLHVertex_VNTT3[vertexCount2];

	memcpy(pindex2, pindex, sizeof(uint)*totalIndex);
	memcpy(pvertex2, pvertex, sizeof(GLHVertex_VNTT3)*vertexCount);

	for(i=0; i<totalIndex; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((uint)j!=pindex2[i])		//Make sure it is not the same index
			{
				if((pvertex2[j].x==pvertex2[pindex2[i]].x)&&
					(pvertex2[j].y==pvertex2[pindex2[i]].y)&&
					(pvertex2[j].z==pvertex2[pindex2[i]].z))
				{	//The vertices are equal
					if((uint)j<pindex2[i])
					{	//Get rid of pvertex2[pindex2[i]]
						for(k=(sint)pindex2[i]+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3));
						}

						vertexCount2--;

						//Adjust indices that also point to pindex2[i] and above
						index=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=(uint)j;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
					else //if((uint)j>pindex2[i])
					{	//Get rid of pvertex2[j]
						for(k=j+1; k<vertexCount2; k++)
						{
							memcpy(&pvertex2[k-1].x, &pvertex2[k].x, sizeof(GLHVertex_VNTT3));
						}

						vertexCount2--;

						//Adjust indices that also point to j and above
						index=(uint)j;
						index2=pindex2[i];
						for(k=0; k<totalIndex; k++)
						{
							if(pindex2[k]==index)
								pindex2[k]=index2;
							else if(pindex2[k]>index)
								pindex2[k]--;
						}
					}
				}
			}
		}
	}

	//Array pindex2 and pvertex2 is now ready. We got rid of all the duplicate vertices
	if(GenerateNormalsForIndexList_FLOAT_10(totalIndex, pindex2, vertexCount2, pvertex2)==-1)
	{
		delete [] pindex2;
		delete [] pvertex2;
		return -1;
	}

	//Now, go through pvertex and get the normal vectors from pvertex2
	for(i=0; i<vertexCount; i++)
	{
		for(j=0; j<vertexCount2; j++)
		{
			if((pvertex[i].x==pvertex2[j].x)&&
				(pvertex[i].y==pvertex2[j].y)&&
				(pvertex[i].z==pvertex2[j].z))
			{
				pvertex[i].nx=pvertex2[j].nx;
				pvertex[i].ny=pvertex2[j].ny;
				pvertex[i].nz=pvertex2[j].nz;
			}
		}
	}

	delete [] pindex2;
	delete [] pvertex2;
	return 1;
}





