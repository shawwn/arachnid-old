// TChamferCylinder.cpp: implementation of the TChamferCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TChamferCylinder.h"
#include <math.h>
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"




#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



/*#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif*/

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TChamferCylinder::TChamferCylinder()
{
	GeneralReset();
}

TChamferCylinder::~TChamferCylinder()
{
	DeallocateAllMemory();
}

void TChamferCylinder::GeneralReset()
{
	TBaseShape::GeneralReset();

	IsThereATop=IsThereABottom=FALSE;
	BodyRadius=ChamferRadius=Height=0.0;
	Stacks=Slices=ChamferStacks=0;
	SlicesPlusOne=0;

	Start_DrawRangeElements_BodyOfCylinder=Start_DrawRangeElements_TopOfCylinder=Start_DrawRangeElements_BottomOfCylinder=0;
	Start_DrawRangeElements_TopChamfer=Start_DrawRangeElements_BottomChamfer=0;
	End_DrawRangeElements_BodyOfCylinder=End_DrawRangeElements_TopOfCylinder=End_DrawRangeElements_BottomOfCylinder=0;
	End_DrawRangeElements_TopChamfer=End_DrawRangeElements_BottomChamfer=0;

	StartIndexIn_pVertex_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder=0;
	pVertex_BodyOfCylinder=pVertex_TopOfCylinder=pVertex_BottomOfCylinder=NULL;
	StartIndexIn_pVertex_TopChamfer=EndIndexIn_pVertex_TopChamfer=0;
	pVertex_TopChamfer=NULL;
	StartIndexIn_pVertex_BottomChamfer=EndIndexIn_pVertex_BottomChamfer=0;
	pVertex_BottomChamfer=NULL;
	StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder=0;
	StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder=0;
	TotalVertex_BodyOfCylinder=TotalVertex_TopOfCylinder=TotalVertex_BottomOfCylinder=0;

	StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder=0;
	StartIndexIn_pIndex_TopChamfer=EndIndexIn_pIndex_TopChamfer=0;
	StartIndexIn_pIndex_BottomChamfer=EndIndexIn_pIndex_BottomChamfer=0;
	StartIndexIn_pIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder=0;
	StartIndexIn_pIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder=0;

	pIndex16Bit_BodyOfCylinder=pIndex16Bit_TopOfCylinder=pIndex16Bit_BottomOfCylinder=NULL;
	pIndex16Bit_TopChamfer=pIndex16Bit_BottomChamfer=NULL;
	pIndex32Bit_BodyOfCylinder=pIndex32Bit_TopOfCylinder=pIndex32Bit_BottomOfCylinder=NULL;
	pIndex32Bit_TopChamfer=pIndex32Bit_BottomChamfer=NULL;
	TotalIndex_BodyOfCylinder=TotalIndex_TopOfCylinder=TotalIndex_BottomOfCylinder=0;
	TotalIndex_TopChamfer=TotalIndex_BottomChamfer=0;

	TexCoordStyle=0;
}

//PURPOSE:	Deallocate all reserved memory
void TChamferCylinder::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}

//PURPOSE:	Must call this at startup to create the chamfered cylinder.
//			Consider a right handed coordinate system where the x axis is along the horizontal
//			of your screen, y is up. z is pointing to you.
//			The cylinder will be along the z axis, from -height/2 to +height/2
//
//PARAMETERS:
//			bodyRadius : Top radius of cylinder, >0.0
//			chamferRadius : This is the radius of the circle at the top and also the bottom.
//			                This should be less than BodyRadius. It would create a tire-like object.
//			                If this is 0.0, then a pill shaped object will be created. Can't be less than 0.0.
//			height : Height of cylinder in z >0.0
//			stacks : Think of it like layers of a cake. >=1
//			slices : Think of it like sections of a cake. >=3
sint TChamferCylinder::StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		tbool isThereATop, tbool isThereABottom,
		sreal bodyRadius, sreal chamferRadius, sreal height, sint stacks, sint slices,
		sint chamferStacks, sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<1) ? 1 : stacks;
	ChamferStacks=(chamferStacks<1) ? 1 : chamferStacks;

	SlicesPlusOne=Slices+1;

	PolygonNumber=((Stacks+ChamferStacks*2)*Slices+Slices)*2;			//Triangle count

	BodyRadius=(bodyRadius<=0.0) ? 1.0 : bodyRadius;
	ChamferRadius=(chamferRadius<0.0) ? 0.9 : chamferRadius;			//Can't be less than 0.0 but it can be 0.0

	if(ChamferRadius>=BodyRadius)
	{
		strcpy(ErrorMessage, "chamferRadius must not be larger or equal than bodyRadius. @ Startup 122");
		return -1;
	}

	if(ChamferRadius==0.0)
	{
		IsThereATop=IsThereABottom=TRUE;								//In this case, make sure they are TRUE

		if(ChamferStacks==1)
		{
			strcpy(ErrorMessage, "Please set ChamferStacks to 2. @ Startup 141");
			//Because in the vertex generation code, we will do ChamferStacks-- and it must not become zero.
			return -1;
		}
	}

	Height=(height<=0.0) ? 1.0 : height;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 96");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 102");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 107");
		return -1;
	}

	DeallocateMemoryForIndex();
	DeallocateMemoryForVertex();
	DeallocateMemoryForNormals();
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		DeallocateMemoryForTexCoords(i);
	}
	DeallocateMemoryForColors();
	DeallocateMemoryForSecondaryColors();
	//DeallocateMemoryForTangentVectors();
	//DeallocateMemoryForBinormalVectors();

	//DeallocateMemoryVertexInterlaced();

	if(GenerateIndices()!=1)
		return -1;

	if(GenerateVertices2()!=1)
		return -1;

	return 1;
}

//PURPOSE:	The indices will be generated whether vertices exist or not.
//
//RETURN:
//			1 for success
//			-1 if 16 bit indices are not enough, so switching to 32 bit.
sint TChamferCylinder::GenerateIndices()
{
	sint i, j, tempo, tempo2, tempo4;
	tbool switchOccured=FALSE;
	ushort indexCount;
	uint indexCount2;
	ushort centerVertexIndex;
	uint centerVertexIndex2;
	sint vertexCount;
	tbool changedChamferRadius=FALSE;

	if(ChamferRadius==0.0)
	{
		ChamferStacks--;				//I'm going to pretend there is one less stack
		changedChamferRadius=TRUE;
	}

	//This is just for the body for now, later it is increased
	vertexCount=(SlicesPlusOne*(Stacks+ChamferStacks+1));		//Stacks+1 for the last circle on the bottom of cylinder
	if(IsThereATop)
		vertexCount+=SlicesPlusOne+1;
	if(IsThereABottom)
		vertexCount+=SlicesPlusOne+1;

	DeallocateMemoryForIndex();

	StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder=0;
	pIndex16Bit_BodyOfCylinder=NULL;
	StartIndexIn_pIndex_TopChamfer=EndIndexIn_pIndex_TopChamfer=0;
	pIndex16Bit_TopChamfer=NULL;
	StartIndexIn_pIndex_BottomChamfer=EndIndexIn_pIndex_BottomChamfer=0;
	pIndex16Bit_BottomChamfer=NULL;
	StartIndexIn_pIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder=0;
	pIndex16Bit_TopOfCylinder=NULL;
	StartIndexIn_pIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder=0;
	pIndex16Bit_BottomOfCylinder=NULL;
	TotalIndex_BodyOfCylinder=TotalIndex_TopOfCylinder=TotalIndex_BottomOfCylinder=0;
	TotalIndex_TopChamfer=TotalIndex_BottomChamfer=0;


	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 660");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	tempo=(Stacks+ChamferStacks*2)*Slices*6;
	if(IsThereATop)
		tempo+=Slices*3;
	if(IsThereABottom)
		tempo+=Slices*3;

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		indexCount=0;

		AllocateExact_1(pIndex16Bit, &UsedIndex, tempo);

		tempo=ChamferStacks*Slices*6;

		pIndex16Bit_TopChamfer=pIndex16Bit;
		StartIndexIn_pIndex_TopChamfer=0;
		EndIndexIn_pIndex_TopChamfer=tempo;
		TotalIndex_TopChamfer=EndIndexIn_pIndex_TopChamfer-StartIndexIn_pIndex_TopChamfer;

		StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_TopChamfer;
		pIndex16Bit_BodyOfCylinder=&pIndex16Bit[StartIndexIn_pIndex_BodyOfCylinder];
		EndIndexIn_pIndex_BodyOfCylinder=StartIndexIn_pIndex_BodyOfCylinder+Stacks*Slices*6;
		TotalIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder-StartIndexIn_pIndex_BodyOfCylinder;

		StartIndexIn_pIndex_BottomChamfer=EndIndexIn_pIndex_BodyOfCylinder;
		pIndex16Bit_BottomChamfer=&pIndex16Bit[StartIndexIn_pIndex_BottomChamfer];
		EndIndexIn_pIndex_BottomChamfer=StartIndexIn_pIndex_BottomChamfer+ChamferStacks*Slices*6;
		TotalIndex_BottomChamfer=EndIndexIn_pIndex_BottomChamfer-StartIndexIn_pIndex_BottomChamfer;


		//This is for the top chamfer area of the cylinder
		tempo4=0;
		for(i=0; i<ChamferStacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex16Bit[tempo4  ]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[tempo4+1]=pIndex16Bit[tempo4]+1;
				pIndex16Bit[tempo4+2]=indexCount;

				pIndex16Bit[tempo4+3]=pIndex16Bit[tempo4+2];
				pIndex16Bit[tempo4+4]=pIndex16Bit[tempo4+1];
				pIndex16Bit[tempo4+5]=indexCount+1;

				indexCount++;
				tempo4+=6;
			}

			indexCount++;	//Go to next level
		}

		//This is for the body area of the cylinder
		indexCount=(ushort)(ChamferStacks*SlicesPlusOne);
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex16Bit[tempo4  ]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[tempo4+1]=pIndex16Bit[tempo4]+1;
				pIndex16Bit[tempo4+2]=indexCount;

				pIndex16Bit[tempo4+3]=pIndex16Bit[tempo4+2];
				pIndex16Bit[tempo4+4]=pIndex16Bit[tempo4+1];
				pIndex16Bit[tempo4+5]=indexCount+1;

				indexCount++;
				tempo4+=6;
			}

			indexCount++;	//Go to next level
		}

		//This is for the bottom chamfer area of the cylinder
		indexCount=(ushort)((ChamferStacks+Stacks)*SlicesPlusOne);
		for(i=0; i<ChamferStacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex16Bit[tempo4  ]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[tempo4+1]=pIndex16Bit[tempo4]+1;
				pIndex16Bit[tempo4+2]=indexCount;

				pIndex16Bit[tempo4+3]=pIndex16Bit[tempo4+2];
				pIndex16Bit[tempo4+4]=pIndex16Bit[tempo4+1];
				pIndex16Bit[tempo4+5]=indexCount+1;

				indexCount++;
				tempo4+=6;
			}

			indexCount++;	//Go to next level
		}

		if(IsThereATop)
		{
			//One vertex at the center, and vertices at each circle.
			//Slices is number of triangles, *3 for number of indices
			tempo=(Stacks+ChamferStacks*2)*Slices*6;
			StartIndexIn_pIndex_TopOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_TopOfCylinder=tempo2;
			TotalIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder-StartIndexIn_pIndex_TopOfCylinder;
			pIndex16Bit_TopOfCylinder=&pIndex16Bit[StartIndexIn_pIndex_TopOfCylinder];

			centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+ChamferStacks*2+1));		//All the vertices of the body and the top and bottom chamfer, there is the vertex for the top
			indexCount=centerVertexIndex+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex16Bit[i  ]=centerVertexIndex;
				pIndex16Bit[i+1]=indexCount++;
				pIndex16Bit[i+2]=indexCount;
			}
		}

		if(IsThereABottom)
		{
			//One vertex at the center, and vertices at each circle.
			//Slices is number of triangles, *3 for number of indices
			tempo=(Stacks+ChamferStacks*2)*Slices*6;
			if(IsThereATop)
				tempo+=Slices*3;

			StartIndexIn_pIndex_BottomOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_BottomOfCylinder=tempo2;
			TotalIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder-StartIndexIn_pIndex_BottomOfCylinder;
			pIndex16Bit_BottomOfCylinder=&pIndex16Bit[StartIndexIn_pIndex_BottomOfCylinder];

			if(IsThereATop)
				centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+ChamferStacks*2+1)+SlicesPlusOne+1);	//All the vertices of the body and the top and bottom chamfer, after the top vertex, there is the vertex for the bottom
			else
				centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+ChamferStacks*2+1));

			indexCount=centerVertexIndex+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex16Bit[i  ]=centerVertexIndex;
				pIndex16Bit[i+1]=indexCount+1;
				pIndex16Bit[i+2]=indexCount++;
			}
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		indexCount2=0;

		AllocateExact_1(pIndex32Bit, &UsedIndex, tempo);

		tempo=ChamferStacks*Slices*6;

		pIndex32Bit_TopChamfer=pIndex32Bit;
		StartIndexIn_pIndex_TopChamfer=0;
		EndIndexIn_pIndex_TopChamfer=tempo;
		TotalIndex_TopChamfer=EndIndexIn_pIndex_TopChamfer-StartIndexIn_pIndex_TopChamfer;

		StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_TopChamfer;
		pIndex32Bit_BodyOfCylinder=&pIndex32Bit[StartIndexIn_pIndex_BodyOfCylinder];
		EndIndexIn_pIndex_BodyOfCylinder=StartIndexIn_pIndex_BodyOfCylinder+Stacks*Slices*6;
		TotalIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder-StartIndexIn_pIndex_BodyOfCylinder;

		StartIndexIn_pIndex_BottomChamfer=EndIndexIn_pIndex_BodyOfCylinder;
		pIndex32Bit_BottomChamfer=&pIndex32Bit[StartIndexIn_pIndex_BottomChamfer];
		EndIndexIn_pIndex_BottomChamfer=StartIndexIn_pIndex_BottomChamfer+ChamferStacks*Slices*6;
		TotalIndex_BottomChamfer=EndIndexIn_pIndex_BottomChamfer-StartIndexIn_pIndex_BottomChamfer;


		//This is for the top chamfer area of the cylinder
		tempo4=0;
		for(i=0; i<ChamferStacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex32Bit[tempo4  ]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[tempo4+1]=pIndex32Bit[tempo4]+1;
				pIndex32Bit[tempo4+2]=indexCount2;

				pIndex32Bit[tempo4+3]=pIndex32Bit[tempo4+2];
				pIndex32Bit[tempo4+4]=pIndex32Bit[tempo4+1];
				pIndex32Bit[tempo4+5]=indexCount2+1;

				indexCount2++;
				tempo4+=6;
			}

			indexCount2++;	//Go to next level
		}

		//This is for the body area of the cylinder
		indexCount2=(uint)(ChamferStacks*SlicesPlusOne);
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex32Bit[tempo4  ]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[tempo4+1]=pIndex32Bit[tempo4]+1;
				pIndex32Bit[tempo4+2]=indexCount2;

				pIndex32Bit[tempo4+3]=pIndex32Bit[tempo4+2];
				pIndex32Bit[tempo4+4]=pIndex32Bit[tempo4+1];
				pIndex32Bit[tempo4+5]=indexCount2+1;

				indexCount2++;
				tempo4+=6;
			}

			indexCount2++;	//Go to next level
		}

		//This is for the bottom chamfer area of the cylinder
		indexCount2=(uint)((ChamferStacks+Stacks)*SlicesPlusOne);
		for(i=0; i<ChamferStacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex32Bit[tempo4  ]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[tempo4+1]=pIndex32Bit[tempo4]+1;
				pIndex32Bit[tempo4+2]=indexCount2;

				pIndex32Bit[tempo4+3]=pIndex32Bit[tempo4+2];
				pIndex32Bit[tempo4+4]=pIndex32Bit[tempo4+1];
				pIndex32Bit[tempo4+5]=indexCount2+1;

				indexCount2++;
				tempo4+=6;
			}

			indexCount2++;	//Go to next level
		}

		if(IsThereATop)
		{
			//One vertex at the center, and vertices at each circle.
			//Slices is number of triangles, *3 for number of indices
			tempo=(Stacks+ChamferStacks*2)*Slices*6;
			StartIndexIn_pIndex_TopOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_TopOfCylinder=tempo2;
			TotalIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder-StartIndexIn_pIndex_TopOfCylinder;
			pIndex32Bit_TopOfCylinder=&pIndex32Bit[StartIndexIn_pIndex_TopOfCylinder];

			centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+ChamferStacks*2+1));		//All the vertices of the body and the top and bottom chamfer, there is the vertex for the top
			indexCount2=centerVertexIndex2+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex32Bit[i  ]=centerVertexIndex2;
				pIndex32Bit[i+1]=indexCount2++;
				pIndex32Bit[i+2]=indexCount2;
			}
		}

		if(IsThereABottom)
		{
			//One vertex at the center, and vertices at each circle.
			//Slices is number of triangles, *3 for number of indices
			tempo=(Stacks+ChamferStacks*2)*Slices*6;
			if(IsThereATop)
				tempo+=Slices*3;

			StartIndexIn_pIndex_BottomOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_BottomOfCylinder=tempo2;
			TotalIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder-StartIndexIn_pIndex_BottomOfCylinder;
			pIndex32Bit_BottomOfCylinder=&pIndex32Bit[StartIndexIn_pIndex_BottomOfCylinder];

			if(IsThereATop)
				centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+ChamferStacks*2+1)+SlicesPlusOne+1);	//All the vertices of the body and the top and bottom chamfer, after the top vertex, there is the vertex for the bottom
			else
				centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+ChamferStacks*2+1));

			indexCount2=centerVertexIndex2+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex32Bit[i  ]=centerVertexIndex2;
				pIndex32Bit[i+1]=indexCount2+1;
				pIndex32Bit[i+2]=indexCount2++;
			}
		}
	}

	if(changedChamferRadius)
	{
		ChamferStacks++;			//Readjust
	}

	if(switchOccured==FALSE)
		return 1;
	else
		return -1;
}

//PURPOSE:	If there won't be a top or bottom, then those won't be computed so some memory will be saved.
//
//RETURN:
//			1 for success
sint TChamferCylinder::GenerateVertices2()
{
	sint i, j, tempo, tempo3, tempo5, tempo6, returnVal;
	sreal FTempo, FTempo2, FTempo3, FTempo4, FTempo5, phi;
	sint slicesLessOne;
	sreal heightIncrement;
	sreal phiIncrement;
	sreal chamferHeight;
	sreal heightIncrement2;
	tbool changedChamferRadius=FALSE;

	phiIncrement=(2.0*PI_DEF)/(sreal)Slices;

	FTempo3=acosf(ChamferRadius/BodyRadius);					//Angle
	chamferHeight=BodyRadius*sinf(FTempo3);						//The height for the chamfer part
	heightIncrement2=chamferHeight/(sreal)ChamferStacks;

	heightIncrement=(Height-chamferHeight-chamferHeight)/(sreal)(Stacks);		//-chamferHeight-chamferHeight to remove the top and bottom chamfer part

	if(heightIncrement<0.0)
	{	//It can happen
		strcpy(ErrorMessage, "Height is not large enough. @ GenerateVertices2 569");
		return -1;
	}

	//This is just for the body for now, later it is increased
	VertexCount=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));		//Stacks+ChamferStacks*2+1 for the last circle on the bottom of cylinder

	if(ChamferRadius==0.0)
	{
		FTempo5=asinf((chamferHeight-heightIncrement2)/BodyRadius);
		ChamferRadius=BodyRadius*cosf(FTempo5);
		chamferHeight=chamferHeight-heightIncrement2;
		ChamferStacks--;
		changedChamferRadius=TRUE;
	}

	StartIndexIn_pVertex_TopChamfer=0;
	EndIndexIn_pVertex_TopChamfer=(SlicesPlusOne*(ChamferStacks+1));
	TotalVertex_TopChamfer=EndIndexIn_pVertex_TopChamfer-StartIndexIn_pVertex_TopChamfer;
	pVertex_TopChamfer=NULL;

	Start_DrawRangeElements_TopChamfer=0;
	End_DrawRangeElements_TopChamfer=EndIndexIn_pVertex_TopChamfer-1;

	//-SlicesPlusOne because the body cylinder walks on the top chamfer
	StartIndexIn_pVertex_BodyOfCylinder=EndIndexIn_pVertex_TopChamfer-SlicesPlusOne;
	EndIndexIn_pVertex_BodyOfCylinder=(SlicesPlusOne*(Stacks+ChamferStacks+1));
	TotalVertex_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder-StartIndexIn_pVertex_BodyOfCylinder;
	pVertex_BodyOfCylinder=NULL;

	Start_DrawRangeElements_BodyOfCylinder=StartIndexIn_pVertex_BodyOfCylinder;
	End_DrawRangeElements_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder-1;
	
	//This is for the bottom chamfer
	StartIndexIn_pVertex_BottomChamfer=EndIndexIn_pVertex_BodyOfCylinder-SlicesPlusOne;
	EndIndexIn_pVertex_BottomChamfer=SlicesPlusOne*(Stacks+ChamferStacks*2+1);
	TotalVertex_BottomChamfer=EndIndexIn_pVertex_BottomChamfer-StartIndexIn_pVertex_BottomChamfer;
	pVertex_BottomChamfer=NULL;

	Start_DrawRangeElements_BottomChamfer=StartIndexIn_pVertex_BottomChamfer;
	End_DrawRangeElements_BottomChamfer=EndIndexIn_pVertex_BottomChamfer-1;

	//Init to 0
	StartIndexIn_pVertex_TopOfCylinder=0;
	EndIndexIn_pVertex_TopOfCylinder=0;
	pVertex_TopOfCylinder=NULL;

	StartIndexIn_pVertex_BottomOfCylinder=0;
	EndIndexIn_pVertex_BottomOfCylinder=0;
	pVertex_BottomOfCylinder=NULL;

	TotalVertex_TopOfCylinder=0;
	TotalVertex_BottomOfCylinder=0;

	Start_DrawRangeElements_TopOfCylinder=0;
	End_DrawRangeElements_TopOfCylinder=0;
	Start_DrawRangeElements_BottomOfCylinder=0;
	End_DrawRangeElements_BottomOfCylinder=0;

	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_V=new GLHVertex_V[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_V;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_V[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_V[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_V=new GLHVertex_V[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_V;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_V[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_V[StartIndexIn_pVertex_BottomChamfer];
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_VT;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VT[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VT[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_VT;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VT[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VT[StartIndexIn_pVertex_BottomChamfer];
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_VNT;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNT[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNT[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_VNT;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNT[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNT[StartIndexIn_pVertex_BottomChamfer];
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_VNT3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNT3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNT3[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_VNT3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNT3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNT3[StartIndexIn_pVertex_BottomChamfer];
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_VNTT3T3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNTT3T3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNTT3T3[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_VNTT3T3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNTT3T3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNTT3T3[StartIndexIn_pVertex_BottomChamfer];
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if(IsThereATop)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center
			if(IsThereABottom)
				VertexCount+=SlicesPlusOne+1;			//One vertex for the center

			//Allocate space for the top and bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_BottomChamfer].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+SlicesPlusOne+1;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;					//Special case, just give it all the vertices
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_BottomChamfer].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomChamfer;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+SlicesPlusOne+1;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;						//Special case, just give it all the vertices
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_TopChamfer=(sreal *)pVertex_VNTT3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNTT3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNTT3[StartIndexIn_pVertex_BottomChamfer];
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];
			pVertex_TopChamfer=(sreal *)pVertex_VNTT3;
			pVertex_BodyOfCylinder=(sreal *)&pVertex_VNTT3[StartIndexIn_pVertex_BodyOfCylinder];
			pVertex_BottomChamfer=(sreal *)&pVertex_VNTT3[StartIndexIn_pVertex_BottomChamfer];
		}
	}


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_V[i].x=0.0;					//This is the center point at top
			pVertex_V[i].y=0.0;
			pVertex_V[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_V[i].x=pVertex_V[j].x;			//Get the vertices that are already computed
				pVertex_V[i].y=pVertex_V[j].y;
				pVertex_V[i].z=pVertex_V[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_V[i].x=0.0;					//This is the center point at bottom
			pVertex_V[i].y=0.0;
			pVertex_V[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_V[i].x=pVertex_V[j].x;			//Get the vertices that are already computed
				pVertex_V[i].y=pVertex_V[j].y;
				pVertex_V[i].z=pVertex_V[j].z;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VT[i].x=0.0;					//This is the center point at top
			pVertex_VT[i].y=0.0;
			pVertex_VT[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_VT[i].x=pVertex_VT[j].x;			//Get the vertices that are already computed
				pVertex_VT[i].y=pVertex_VT[j].y;
				pVertex_VT[i].z=pVertex_VT[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VT[i].x=0.0;					//This is the center point at bottom
			pVertex_VT[i].y=0.0;
			pVertex_VT[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_VT[i].x=pVertex_VT[j].x;			//Get the vertices that are already computed
				pVertex_VT[i].y=pVertex_VT[j].y;
				pVertex_VT[i].z=pVertex_VT[j].z;
			}
		}

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNT[i].x=0.0;				//This is the center point at top
			pVertex_VNT[i].y=0.0;
			pVertex_VNT[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_VNT[i].x=pVertex_VNT[j].x;		//Get the vertices that are already computed
				pVertex_VNT[i].y=pVertex_VNT[j].y;
				pVertex_VNT[i].z=pVertex_VNT[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNT[i].x=0.0;				//This is the center point at bottom
			pVertex_VNT[i].y=0.0;
			pVertex_VNT[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_VNT[i].x=pVertex_VNT[j].x;		//Get the vertices that are already computed
				pVertex_VNT[i].y=pVertex_VNT[j].y;
				pVertex_VNT[i].z=pVertex_VNT[j].z;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNT3[i].x=0.0;				//This is the center point at top
			pVertex_VNT3[i].y=0.0;
			pVertex_VNT3[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_VNT3[i].x=pVertex_VNT3[j].x;		//Get the vertices that are already computed
				pVertex_VNT3[i].y=pVertex_VNT3[j].y;
				pVertex_VNT3[i].z=pVertex_VNT3[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNT3[i].x=0.0;				//This is the center point at bottom
			pVertex_VNT3[i].y=0.0;
			pVertex_VNT3[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_VNT3[i].x=pVertex_VNT3[j].x;		//Get the vertices that are already computed
				pVertex_VNT3[i].y=pVertex_VNT3[j].y;
				pVertex_VNT3[i].z=pVertex_VNT3[j].z;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNTT3T3[i].x=0.0;				//This is the center point at top
			pVertex_VNTT3T3[i].y=0.0;
			pVertex_VNTT3T3[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[j].x;		//Get the vertices that are already computed
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[j].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNTT3T3[i].x=0.0;				//This is the center point at bottom
			pVertex_VNTT3T3[i].y=0.0;
			pVertex_VNTT3T3[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[j].x;		//Get the vertices that are already computed
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[j].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[j].z;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1695");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1704");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if(changedChamferRadius)
			FTempo2=Height*0.5-heightIncrement2;
		else
			FTempo2=Height*0.5;

		slicesLessOne=Slices;

		//*********** THIS IS FOR TOP CHAMFER
		phi=0.0;
		FTempo=ChamferRadius;
		FTempo4=chamferHeight;

		tempo3=TotalVertex_TopChamfer;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4-=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BODY OF CYLINDER
		FTempo2+=heightIncrement2;				//Resposition
		FTempo2-=heightIncrement;				//Resposition
		FTempo=BodyRadius;						//Should be equal to BodyRadius by now anyway
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//*********** THIS IS FOR BOTTOM CHAMFER
		FTempo2+=heightIncrement;				//Resposition
		FTempo2-=heightIncrement2;				//Resposition
		//Compute the new radius using the height parameter : FTempo4
		FTempo4=heightIncrement2;
		FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
		tempo3=(SlicesPlusOne*(Stacks+ChamferStacks*2+1));
		for(tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				tempo=2;
				FTempo2-=heightIncrement2;
				FTempo4+=heightIncrement2;
				phi=0.0;

				//Compute the new radius using the height parameter : FTempo4
				FTempo=BodyRadius*cosf(asinf(FTempo4/BodyRadius));
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNTT3[i].x=0.0;				//This is the center point at top
			pVertex_VNTT3[i].y=0.0;
			pVertex_VNTT3[i].z=FTempo2;

			for(i++, j=0; i<EndIndexIn_pVertex_TopOfCylinder; i++, j++)
			{
				pVertex_VNTT3[i].x=pVertex_VNTT3[j].x;		//Get the vertices that are already computed
				pVertex_VNTT3[i].y=pVertex_VNTT3[j].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[j].z;
			}
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNTT3[i].x=0.0;				//This is the center point at bottom
			pVertex_VNTT3[i].y=0.0;
			pVertex_VNTT3[i].z=FTempo2;

			j=EndIndexIn_pVertex_BottomChamfer-SlicesPlusOne;
			for(i++; i<EndIndexIn_pVertex_BottomOfCylinder; i++, j++)
			{
				pVertex_VNTT3[i].x=pVertex_VNTT3[j].x;		//Get the vertices that are already computed
				pVertex_VNTT3[i].y=pVertex_VNTT3[j].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[j].z;
			}
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();

		//We need to make the pVertex_VNTT3T3 format because ComputeTangentAndBinormalVectors_v1_FLOAT_4 and _3 needs it.
		pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3T3[i].x=pVertex_VNTT3[i].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3[i].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3[i].z;
			pVertex_VNTT3T3[i].nx=pVertex_VNTT3[i].nx;
			pVertex_VNTT3T3[i].ny=pVertex_VNTT3[i].ny;
			pVertex_VNTT3T3[i].nz=pVertex_VNTT3[i].nz;
			pVertex_VNTT3T3[i].s0=pVertex_VNTT3[i].s0;
			pVertex_VNTT3T3[i].t0=pVertex_VNTT3[i].t0;
		}

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1856");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1865");
				return -1;
			}
		}

		//Copy the tangents
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3[i].s1=pVertex_VNTT3T3[i].s1;
			pVertex_VNTT3[i].t1=pVertex_VNTT3T3[i].t1;
			pVertex_VNTT3[i].r1=pVertex_VNTT3T3[i].r1;
		}

		delete [] pVertex_VNTT3T3;
		pVertex_VNTT3T3=NULL;
	}


	Start_DrawRangeElements=0;
	if(IsThereATop)
	{
		if(IsThereABottom)
		{
			End_DrawRangeElements=End_DrawRangeElements_BottomOfCylinder;
		}
		else
		{
			End_DrawRangeElements=End_DrawRangeElements_TopOfCylinder;
		}
	}
	else if(IsThereABottom)
	{
		End_DrawRangeElements=End_DrawRangeElements_BottomOfCylinder;
	}
	else
	{
		End_DrawRangeElements=End_DrawRangeElements_BottomChamfer;
	}

	if(changedChamferRadius)
	{
		ChamferRadius=0.0;
		ChamferStacks++;
	}

	return 1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//			Since the cylinder can have a different radius for the top and bottom, computing
//			the normals needs to take into account the angle.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TChamferCylinder::GenerateNormals2()
{
	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_3(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNT);
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_4(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNT);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_5(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNT3);
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_6(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNT3);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_7(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_8(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_9(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3);
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			return GenerateNormalsForIndexList_Shared_FLOAT_10(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3);
		}

		return 1;
	}

	return -1;
}

//PURPOSE:	Calculate the texture coordinates for the torus.
//
//RETURN:
//			1 if successful
//			-1 if TexCoordStyle is not 1 or if the VertexFormat is not supported
sint TChamferCylinder::GenerateTexCoords2()
{
	sint i, k, tempo2, tempo3;
	sreal FTempo, FTempo2, angle;
	sreal FTempo3;
	sreal chamferHeight;

	FTempo3=acosf(ChamferRadius/BodyRadius);						//Angle
	chamferHeight=BodyRadius*sinf(FTempo3);							//The height for the chamfer part

	if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);								//Used as x Tex coord increment
			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment

			//***************** THE TOP CHAMFER, BODY AND BOTTOM CHAMFER
			//Generate for first stack
			pVertex_VT[0].s0=0.0;
			pVertex_VT[0].t0=0.0;
			tempo2=Slices;
			for(i=1; i<tempo2; i++)
			{
				pVertex_VT[i].s0=(pVertex_VT[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VT[i].t0=0.0;
			}
			pVertex_VT[i].s0=1.0*ScaleFactorS[0];
			pVertex_VT[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_TopChamfer;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
				pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=((Height-chamferHeight-chamferHeight)/Height)/sreal(Stacks);			//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BodyOfCylinder;
			for(; i<tempo2; i++)
			{
				pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
				pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BottomChamfer;
			for(; i<tempo2; i++)
			{
				pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
				pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** THE TOP OF THE CYLINDER
			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VT[StartIndexIn_pVertex_TopOfCylinder].s0=0.5;				//Center vertex
				pVertex_VT[StartIndexIn_pVertex_TopOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_TopOfCylinder+1;
				tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VT[i].s0=pVertex_VT[k].s0;
				pVertex_VT[i].t0=pVertex_VT[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VT[StartIndexIn_pVertex_BottomOfCylinder].s0=0.5;			//Center vertex
				pVertex_VT[StartIndexIn_pVertex_BottomOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_BottomOfCylinder+1;
				tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VT[i].s0=pVertex_VT[k].s0;
				pVertex_VT[i].t0=pVertex_VT[k].t0;
			}

			return 1;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);								//Used as x Tex coord increment
			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment

			//***************** THE TOP CHAMFER, BODY AND BOTTOM CHAMFER
			//Generate for first stack
			pVertex_VNT[0].s0=0.0;
			pVertex_VNT[0].t0=0.0;
			tempo2=Slices;
			for(i=1; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=(pVertex_VNT[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNT[i].t0=0.0;
			}
			pVertex_VNT[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNT[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_TopChamfer;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
				pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=((Height-chamferHeight-chamferHeight)/Height)/sreal(Stacks);			//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BodyOfCylinder;
			for(; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
				pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BottomChamfer;
			for(; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
				pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** THE TOP OF THE CYLINDER
			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNT[StartIndexIn_pVertex_TopOfCylinder].s0=0.5;				//Center vertex
				pVertex_VNT[StartIndexIn_pVertex_TopOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_TopOfCylinder+1;
				tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT[i].s0=pVertex_VNT[k].s0;
				pVertex_VNT[i].t0=pVertex_VNT[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNT[StartIndexIn_pVertex_BottomOfCylinder].s0=0.5;			//Center vertex
				pVertex_VNT[StartIndexIn_pVertex_BottomOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_BottomOfCylinder+1;
				tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT[i].s0=pVertex_VNT[k].s0;
				pVertex_VNT[i].t0=pVertex_VNT[k].t0;
			}

			return 1;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);								//Used as x Tex coord increment
			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment

			//***************** THE TOP CHAMFER, BODY AND BOTTOM CHAMFER
			//Generate for first stack
			pVertex_VNT3[0].s0=0.0;
			pVertex_VNT3[0].t0=0.0;
			pVertex_VNT3[0].r0=0.0;
			tempo2=Slices;
			for(i=1; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=(pVertex_VNT3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNT3[i].t0=0.0;
				pVertex_VNT3[i].r0=0.0;
			}
			pVertex_VNT3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNT3[i].t0=0.0;
			pVertex_VNT3[i].r0=0.0;

			tempo2=EndIndexIn_pVertex_TopChamfer;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
				pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				pVertex_VNT3[i].r0=0.0;
			}

			FTempo2=((Height-chamferHeight-chamferHeight)/Height)/sreal(Stacks);			//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BodyOfCylinder;
			for(; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
				pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				pVertex_VNT3[i].r0=0.0;
			}

			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BottomChamfer;
			for(; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
				pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				pVertex_VNT3[i].r0=0.0;
			}

			//***************** THE TOP OF THE CYLINDER
			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNT3[StartIndexIn_pVertex_TopOfCylinder].s0=0.5;				//Center vertex
				pVertex_VNT3[StartIndexIn_pVertex_TopOfCylinder].t0=0.5;
				pVertex_VNT3[StartIndexIn_pVertex_TopOfCylinder].r0=0.0;

				k=i=StartIndexIn_pVertex_TopOfCylinder+1;
				tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					pVertex_VNT3[i].r0=0.0;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT3[i].s0=pVertex_VNT3[k].s0;
				pVertex_VNT3[i].t0=pVertex_VNT3[k].t0;
				pVertex_VNT3[i].r0=0.0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNT3[StartIndexIn_pVertex_BottomOfCylinder].s0=0.5;			//Center vertex
				pVertex_VNT3[StartIndexIn_pVertex_BottomOfCylinder].t0=0.5;
				pVertex_VNT3[StartIndexIn_pVertex_BottomOfCylinder].r0=0.0;

				k=i=StartIndexIn_pVertex_BottomOfCylinder+1;
				tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					pVertex_VNT3[i].r0=0.0;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT3[i].s0=pVertex_VNT3[k].s0;
				pVertex_VNT3[i].t0=pVertex_VNT3[k].t0;
				pVertex_VNT3[i].r0=0.0;
			}

			return 1;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);								//Used as x Tex coord increment
			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment

			//***************** THE TOP CHAMFER, BODY AND BOTTOM CHAMFER
			//Generate for first stack
			pVertex_VNTT3T3[0].s0=0.0;
			pVertex_VNTT3T3[0].t0=0.0;
			tempo2=Slices;
			for(i=1; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=(pVertex_VNTT3T3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNTT3T3[i].t0=0.0;
			}
			pVertex_VNTT3T3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNTT3T3[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_TopChamfer;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
				pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=((Height-chamferHeight-chamferHeight)/Height)/sreal(Stacks);			//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BodyOfCylinder;
			for(; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
				pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BottomChamfer;
			for(; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
				pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** THE TOP OF THE CYLINDER
			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNTT3T3[StartIndexIn_pVertex_TopOfCylinder].s0=0.5;				//Center vertex
				pVertex_VNTT3T3[StartIndexIn_pVertex_TopOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_TopOfCylinder+1;
				tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[k].s0;
				pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNTT3T3[StartIndexIn_pVertex_BottomOfCylinder].s0=0.5;			//Center vertex
				pVertex_VNTT3T3[StartIndexIn_pVertex_BottomOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_BottomOfCylinder+1;
				tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[k].s0;
				pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[k].t0;
			}

			return 1;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);								//Used as x Tex coord increment
			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment

			//***************** THE TOP CHAMFER, BODY AND BOTTOM CHAMFER
			//Generate for first stack
			pVertex_VNTT3[0].s0=0.0;
			pVertex_VNTT3[0].t0=0.0;
			tempo2=Slices;
			for(i=1; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=(pVertex_VNTT3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNTT3[i].t0=0.0;
			}
			pVertex_VNTT3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNTT3[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_TopChamfer;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
				pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=((Height-chamferHeight-chamferHeight)/Height)/sreal(Stacks);			//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BodyOfCylinder;
			for(; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
				pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			FTempo2=(chamferHeight/Height)/sreal(ChamferStacks);	//Used as y Tex coord increment
			tempo2=EndIndexIn_pVertex_BottomChamfer;
			for(; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
				pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** THE TOP OF THE CYLINDER
			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNTT3[StartIndexIn_pVertex_TopOfCylinder].s0=0.5;				//Center vertex
				pVertex_VNTT3[StartIndexIn_pVertex_TopOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_TopOfCylinder+1;
				tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3[i].s0=pVertex_VNTT3[k].s0;
				pVertex_VNTT3[i].t0=pVertex_VNTT3[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				pVertex_VNTT3[StartIndexIn_pVertex_BottomOfCylinder].s0=0.5;			//Center vertex
				pVertex_VNTT3[StartIndexIn_pVertex_BottomOfCylinder].t0=0.5;

				k=i=StartIndexIn_pVertex_BottomOfCylinder+1;
				tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3[i].s0=pVertex_VNTT3[k].s0;
				pVertex_VNTT3[i].t0=pVertex_VNTT3[k].t0;
			}

			return 1;
		}
	}

	return -1;
}



//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//




