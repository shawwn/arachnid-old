// TTube.cpp: implementation of the TTube class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TTube.h"
#include <math.h>
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"




#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TTube::TTube()
{
	GeneralReset();
}

TTube::~TTube()
{
	DeallocateAllMemory();
}

//PURPOSE:	Resets principal values to default
void TTube::GeneralReset()
{
	sint i;

	TBaseShape::GeneralReset();

	IsThereATop=IsThereABottom=FALSE;
	AOuterRadius=BOuterRadius=0.0;
	AInnerRadius=BInnerRadius=0.0;
	Height=0.0;
	PolygonNumber=Stacks=SlicesPlusOne=Slices=0;
	SlicesPlusOne=0;

	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		TextureID_OuterBodyOfTube[i]=TextureID_InnerBodyOfTube[i]=0;
		TextureID_TopOfTube[i]=TextureID_BottomOfTube[i]=0;
	}

	Start_DrawRangeElements_OuterBodyOfTube=Start_DrawRangeElements_InnerBodyOfTube=0;
	Start_DrawRangeElements_TopOfTube=Start_DrawRangeElements_BottomOfTube=0;
	End_DrawRangeElements_OuterBodyOfTube=End_DrawRangeElements_InnerBodyOfTube=0;
	End_DrawRangeElements_TopOfTube=End_DrawRangeElements_BottomOfTube=0;

	StartIndexIn_pVertex_OuterBodyOfTube=EndIndexIn_pVertex_OuterBodyOfTube=0;
	pVertex_OuterBodyOfTube=pVertex_InnerBodyOfTube=NULL;
	pVertex_TopOfTube=pVertex_BottomOfTube=NULL;
	StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube=0;
	StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube=0;
	TotalVertex_OuterBodyOfTube=TotalVertex_InnerBodyOfTube=0;
	TotalVertex_TopOfTube=TotalVertex_BottomOfTube=0;

	StartIndexIn_pIndex_OuterBodyOfTube=EndIndexIn_pIndex_OuterBodyOfTube=0;
	StartIndexIn_pIndex_InnerBodyOfTube=EndIndexIn_pIndex_InnerBodyOfTube=0;
	StartIndexIn_pIndex_TopOfTube=EndIndexIn_pIndex_TopOfTube=0;
	StartIndexIn_pIndex_BottomOfTube=EndIndexIn_pIndex_BottomOfTube=0;

	pIndex16Bit_OuterBodyOfTube=pIndex16Bit_InnerBodyOfTube=NULL;
	pIndex16Bit_TopOfTube=pIndex16Bit_BottomOfTube=NULL;
	pIndex32Bit_OuterBodyOfTube=pIndex32Bit_InnerBodyOfTube=NULL;
	pIndex32Bit_TopOfTube=pIndex32Bit_BottomOfTube=NULL;
	TotalIndex_OuterBodyOfTube=TotalIndex_InnerBodyOfTube=0;
	TotalIndex_TopOfTube=TotalIndex_BottomOfTube=0;
}

//PURPOSE:	Deallocate all reserved memory
void TTube::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}

//PURPOSE:	Must call this at startup to create the tube.
//			Consider a right handed coordinate system where the x axis is along the horizontal
//			of your screen, y is up. z is pointing to you.
//			The tube will be along the z axis, from -height/2 to +height/2
sint TTube::StartupByTwoRadiusStacksAndSlices(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, tbool isThereATop, tbool isThereABottom,
		sreal outer_aRadius, sreal outer_bRadius, sreal inner_aRadius, sreal inner_bRadius,
		sreal height, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<1) ? 1 : stacks;

	SlicesPlusOne=Slices+1;

	PolygonNumber=Slices*Stacks*4+Slices*4;		//Triangle count

	AOuterRadius=(outer_aRadius<=0.0) ? 1.0 : outer_aRadius;
	BOuterRadius=(outer_bRadius<=0.0) ? 0.1 : outer_bRadius;
	AInnerRadius=(inner_aRadius<=0.0) ? 1.0 : inner_aRadius;
	BInnerRadius=(inner_bRadius<=0.0) ? 0.1 : inner_bRadius;

	Height=height;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 114");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 120");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 125");
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

	GenerateVertices2();

	return 1;
}

//PURPOSE:	The indices will be generated whether vertices exist or not.
//
//RETURN:
//			1 for success
//			-1 if 16 bit indices are not enough, so switching to 32 bit.
sint TTube::GenerateIndices()
{
	sint i, j, tempo, tempo2, tempo4;
	tbool switchOccured=FALSE;
	ushort indexCount;
	uint indexCount2;
	sint vertexCount;

	vertexCount=((Slices+1)*(Stacks+1))*2;			//For the inner and outer part of the tube
	if(IsThereATop)
		vertexCount+=(Slices+1)*2;
	if(IsThereABottom)
		vertexCount+=(Slices+1)*2;

	StartIndexIn_pIndex_OuterBodyOfTube=EndIndexIn_pIndex_InnerBodyOfTube=0;
	pIndex16Bit_OuterBodyOfTube=pIndex16Bit_InnerBodyOfTube=NULL;
	StartIndexIn_pIndex_TopOfTube=EndIndexIn_pIndex_TopOfTube=0;
	pIndex16Bit_TopOfTube=NULL;
	StartIndexIn_pIndex_BottomOfTube=EndIndexIn_pIndex_BottomOfTube=0;
	pIndex16Bit_BottomOfTube=NULL;
	TotalIndex_OuterBodyOfTube=TotalIndex_InnerBodyOfTube=0;
	TotalIndex_TopOfTube=TotalIndex_BottomOfTube=0;


	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 660");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	tempo=Stacks*Slices*12;
	if(IsThereATop)
		tempo+=Slices*6;
	if(IsThereABottom)
		tempo+=Slices*6;

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		indexCount=0;

		AllocateExact_1(pIndex16Bit, &UsedIndex, tempo);

		tempo=Stacks*Slices*6;

		pIndex16Bit_OuterBodyOfTube=pIndex16Bit;
		StartIndexIn_pIndex_OuterBodyOfTube=0;
		EndIndexIn_pIndex_OuterBodyOfTube=tempo;
		TotalIndex_OuterBodyOfTube=EndIndexIn_pIndex_OuterBodyOfTube-StartIndexIn_pIndex_OuterBodyOfTube;

		StartIndexIn_pIndex_InnerBodyOfTube=EndIndexIn_pIndex_OuterBodyOfTube;
		EndIndexIn_pIndex_InnerBodyOfTube=Stacks*Slices*12;
		TotalIndex_InnerBodyOfTube=EndIndexIn_pIndex_InnerBodyOfTube-StartIndexIn_pIndex_InnerBodyOfTube;
		pIndex16Bit_InnerBodyOfTube=&pIndex16Bit[StartIndexIn_pIndex_InnerBodyOfTube];

		//Outer part of the tube
		tempo4=0;
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the tube 2 triangles = a quad
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

		indexCount+=(ushort)SlicesPlusOne;

		//Inner part of the tube
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the tube 2 triangles = a quad
				pIndex16Bit[tempo4  ]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[tempo4+1]=indexCount;
				pIndex16Bit[tempo4+2]=pIndex16Bit[tempo4]+1;

				pIndex16Bit[tempo4+3]=indexCount+1;
				pIndex16Bit[tempo4+4]=pIndex16Bit[tempo4+2];
				pIndex16Bit[tempo4+5]=pIndex16Bit[tempo4+1];

				indexCount++;
				tempo4+=6;
			}

			indexCount++;	//Go to next level
		}

		if(IsThereATop)
		{
			//The vertices are like this.
			//The outer circle first, then the inner circle
			tempo=Stacks*Slices*12;
			StartIndexIn_pIndex_TopOfTube=tempo;
			tempo2=tempo+Slices*6;
			EndIndexIn_pIndex_TopOfTube=tempo2;
			TotalIndex_TopOfTube=EndIndexIn_pIndex_TopOfTube-StartIndexIn_pIndex_TopOfTube;
			pIndex16Bit_TopOfTube=&pIndex16Bit[StartIndexIn_pIndex_TopOfTube];

			indexCount=(ushort)(((Slices+1)*(Stacks+1))*2);

			for(j=0, i=tempo; j<Slices; j++, i+=6)		//i+=6 because two triangles added
			{
				pIndex16Bit[i  ]=indexCount+1;
				pIndex16Bit[i+1]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[i+2]=indexCount;
				pIndex16Bit[i+3]=pIndex16Bit[i+1];
				pIndex16Bit[i+4]=indexCount+1;
				pIndex16Bit[i+5]=pIndex16Bit[i+1]+1;
				indexCount++;
			}
		}

		if(IsThereABottom)
		{
			//The vertices are like this.
			//The outer circle first, then the inner circle
			tempo=Stacks*Slices*12;
			if(IsThereATop)
				tempo+=Slices*6;

			StartIndexIn_pIndex_BottomOfTube=tempo;
			tempo2=tempo+Slices*6;
			EndIndexIn_pIndex_BottomOfTube=tempo2;
			TotalIndex_BottomOfTube=EndIndexIn_pIndex_BottomOfTube-StartIndexIn_pIndex_BottomOfTube;
			pIndex16Bit_BottomOfTube=&pIndex16Bit[StartIndexIn_pIndex_BottomOfTube];

			indexCount=(ushort)(((Slices+1)*(Stacks+1))*2);
			if(IsThereATop)
				indexCount+=(ushort)((Slices+1)*2);

			for(j=0, i=tempo; j<Slices; j++, i+=6)		//i+=6 because two triangles added
			{
				pIndex16Bit[i  ]=indexCount+1;
				pIndex16Bit[i+1]=indexCount+(ushort)SlicesPlusOne;
				pIndex16Bit[i+2]=indexCount;
				pIndex16Bit[i+3]=pIndex16Bit[i+1];
				pIndex16Bit[i+4]=indexCount+1;
				pIndex16Bit[i+5]=pIndex16Bit[i+1]+1;
				indexCount++;
			}
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		indexCount2=0;

		AllocateExact_1(pIndex32Bit, &UsedIndex, tempo);

		tempo=Stacks*Slices*6;

		pIndex32Bit_OuterBodyOfTube=pIndex32Bit;
		StartIndexIn_pIndex_OuterBodyOfTube=0;
		EndIndexIn_pIndex_OuterBodyOfTube=tempo;
		TotalIndex_OuterBodyOfTube=EndIndexIn_pIndex_OuterBodyOfTube-StartIndexIn_pIndex_OuterBodyOfTube;

		StartIndexIn_pIndex_InnerBodyOfTube=EndIndexIn_pIndex_OuterBodyOfTube;
		EndIndexIn_pIndex_InnerBodyOfTube=Stacks*Slices*12;
		TotalIndex_InnerBodyOfTube=EndIndexIn_pIndex_InnerBodyOfTube-StartIndexIn_pIndex_InnerBodyOfTube;
		pIndex32Bit_InnerBodyOfTube=&pIndex32Bit[StartIndexIn_pIndex_InnerBodyOfTube];

		//Outer part of the tube
		tempo4=0;
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the tube 2 triangles = a quad
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

		indexCount2+=(uint)SlicesPlusOne;

		//Inner part of the tube
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the tube 2 triangles = a quad
				pIndex32Bit[tempo4  ]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[tempo4+1]=indexCount2;
				pIndex32Bit[tempo4+2]=pIndex32Bit[tempo4]+1;

				pIndex32Bit[tempo4+3]=indexCount2+1;
				pIndex32Bit[tempo4+4]=pIndex32Bit[tempo4+2];
				pIndex32Bit[tempo4+5]=pIndex32Bit[tempo4+1];

				indexCount2++;
				tempo4+=6;
			}

			indexCount2++;	//Go to next level
		}

		if(IsThereATop)
		{
			//The vertices are like this.
			//The outer circle first, then the inner circle
			tempo=Stacks*Slices*12;
			StartIndexIn_pIndex_TopOfTube=tempo;
			tempo2=tempo+Slices*6;
			EndIndexIn_pIndex_TopOfTube=tempo2;
			TotalIndex_TopOfTube=EndIndexIn_pIndex_TopOfTube-StartIndexIn_pIndex_TopOfTube;
			pIndex32Bit_TopOfTube=&pIndex32Bit[StartIndexIn_pIndex_TopOfTube];

			indexCount2=(uint)(((Slices+1)*(Stacks+1))*2);

			for(j=0, i=tempo; j<Slices; j++, i+=6)		//i+=6 because two triangles added
			{
				pIndex32Bit[i  ]=indexCount2+1;
				pIndex32Bit[i+1]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[i+2]=indexCount2;
				pIndex32Bit[i+3]=pIndex32Bit[i+1];
				pIndex32Bit[i+4]=indexCount2+1;
				pIndex32Bit[i+5]=pIndex32Bit[i+1]+1;
				indexCount2++;
			}
		}

		if(IsThereABottom)
		{
			//The vertices are like this.
			//The outer circle first, then the inner circle
			tempo=Stacks*Slices*12;
			if(IsThereATop)
				tempo+=Slices*6;

			StartIndexIn_pIndex_BottomOfTube=tempo;
			tempo2=tempo+Slices*6;
			EndIndexIn_pIndex_BottomOfTube=tempo2;
			TotalIndex_BottomOfTube=EndIndexIn_pIndex_BottomOfTube-StartIndexIn_pIndex_BottomOfTube;
			pIndex32Bit_BottomOfTube=&pIndex32Bit[StartIndexIn_pIndex_BottomOfTube];

			indexCount2=(uint)(((Slices+1)*(Stacks+1))*2);
			if(IsThereATop)
				indexCount2+=(uint)((Slices+1)*2);

			for(j=0, i=tempo; j<Slices; j++, i+=6)		//i+=6 because two triangles added
			{
				pIndex32Bit[i  ]=indexCount2+1;
				pIndex32Bit[i+1]=indexCount2+(uint)SlicesPlusOne;
				pIndex32Bit[i+2]=indexCount2;
				pIndex32Bit[i+3]=pIndex32Bit[i+1];
				pIndex32Bit[i+4]=indexCount2+1;
				pIndex32Bit[i+5]=pIndex32Bit[i+1]+1;
				indexCount2++;
			}
		}
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
sint TTube::GenerateVertices2()
{
	sint i, j, k, tempo, tempo3, tempo5, tempo6, returnVal;
	sreal FTempo, FTempo2, phi;
	sreal radialIncrement;
	sint slicesLessOne;
	sreal heightIncrement;
	sreal phiIncrement;

	heightIncrement=Height/(sreal)Stacks;
	phiIncrement=(2.0*PI_DEF)/(sreal)Slices;

	//This is just for the body for now, later it is increased
	VertexCount=(Slices+1)*(Stacks+1);		//Stacks+1 for the last circle on the bottom of tube

	StartIndexIn_pVertex_OuterBodyOfTube=0;
	EndIndexIn_pVertex_OuterBodyOfTube=VertexCount;
	pVertex_OuterBodyOfTube=NULL;

	Start_DrawRangeElements_OuterBodyOfTube=0;
	End_DrawRangeElements_OuterBodyOfTube=VertexCount-1;
	TotalVertex_OuterBodyOfTube=EndIndexIn_pVertex_OuterBodyOfTube-StartIndexIn_pVertex_OuterBodyOfTube;

	StartIndexIn_pVertex_InnerBodyOfTube=EndIndexIn_pVertex_OuterBodyOfTube;
	EndIndexIn_pVertex_InnerBodyOfTube=VertexCount*2;
	pVertex_InnerBodyOfTube=NULL;

	Start_DrawRangeElements_InnerBodyOfTube=End_DrawRangeElements_OuterBodyOfTube+1;
	End_DrawRangeElements_InnerBodyOfTube=VertexCount*2-1;
	TotalVertex_InnerBodyOfTube=EndIndexIn_pVertex_InnerBodyOfTube-StartIndexIn_pVertex_InnerBodyOfTube;

	VertexCount*=2;

	//Init to 0
	StartIndexIn_pVertex_TopOfTube=0;
	EndIndexIn_pVertex_TopOfTube=0;
	pVertex_TopOfTube=NULL;

	StartIndexIn_pVertex_BottomOfTube=0;
	EndIndexIn_pVertex_BottomOfTube=0;
	pVertex_BottomOfTube=NULL;

	TotalVertex_TopOfTube=0;
	TotalVertex_BottomOfTube=0;

	Start_DrawRangeElements_TopOfTube=0;
	End_DrawRangeElements_TopOfTube=0;
	Start_DrawRangeElements_BottomOfTube=0;
	End_DrawRangeElements_BottomOfTube=0;

	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_V=new GLHVertex_V[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_V[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_V[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_V[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_V;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_V[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_V=new GLHVertex_V[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_V;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_V[(Slices+1)*(Stacks+1)].x;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_VT[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_VT[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_VT[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_VT;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VT[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_VT;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VT[(Slices+1)*(Stacks+1)].x;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNT;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNT[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNT;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNT[(Slices+1)*(Stacks+1)].x;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNT3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNT3[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNT3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNT3[(Slices+1)*(Stacks+1)].x;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNTT3T3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNTT3T3[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNTT3T3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNTT3T3[(Slices+1)*(Stacks+1)].x;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=(Slices+1)*2;
				tempo6=2*tempo5;					//*2 for the bottom as well
			}
			else	//One or the other
				tempo5=tempo6=(Slices+1)*2;

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfTube=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_InnerBodyOfTube].x;
				StartIndexIn_pVertex_TopOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_TopOfTube=StartIndexIn_pVertex_TopOfTube+tempo5;
				TotalVertex_TopOfTube=EndIndexIn_pVertex_TopOfTube-StartIndexIn_pVertex_TopOfTube;

				Start_DrawRangeElements_TopOfTube=StartIndexIn_pVertex_TopOfTube;
				End_DrawRangeElements_TopOfTube=EndIndexIn_pVertex_TopOfTube-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfTube=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_TopOfTube].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_TopOfTube;
					EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
					TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

					Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
					End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfTube=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_InnerBodyOfTube].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfTube=EndIndexIn_pVertex_InnerBodyOfTube;
				EndIndexIn_pVertex_BottomOfTube=StartIndexIn_pVertex_BottomOfTube+tempo5;
				TotalVertex_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-StartIndexIn_pVertex_BottomOfTube;

				Start_DrawRangeElements_BottomOfTube=StartIndexIn_pVertex_BottomOfTube;
				End_DrawRangeElements_BottomOfTube=EndIndexIn_pVertex_BottomOfTube-1;
			}

			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNTT3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNTT3[(Slices+1)*(Stacks+1)].x;
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];
			pVertex_OuterBodyOfTube=(sreal *)pVertex_VNTT3;
			pVertex_InnerBodyOfTube=(sreal *)&pVertex_VNTT3[(Slices+1)*(Stacks+1)].x;
		}
	}


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_V[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_V[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_V[i].x=pVertex_V[k].x;
			pVertex_V[i].y=pVertex_V[k].y;
			pVertex_V[i].z=pVertex_V[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_V[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_V[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_V[i].x=pVertex_V[k].x;
			pVertex_V[i].y=pVertex_V[k].y;
			pVertex_V[i].z=pVertex_V[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_V[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_V[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_V[i].x=pVertex_V[k].x;
			pVertex_V[i].y=pVertex_V[k].y;
			pVertex_V[i].z=pVertex_V[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_V[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_V[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_V[i].x=pVertex_V[k].x;
			pVertex_V[i].y=pVertex_V[k].y;
			pVertex_V[i].z=pVertex_V[k].z;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VT[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_VT[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VT[i].x=pVertex_VT[k].x;
			pVertex_VT[i].y=pVertex_VT[k].y;
			pVertex_VT[i].z=pVertex_VT[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VT[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VT[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VT[i].x=pVertex_VT[k].x;
			pVertex_VT[i].y=pVertex_VT[k].y;
			pVertex_VT[i].z=pVertex_VT[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VT[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_VT[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VT[i].x=pVertex_VT[k].x;
			pVertex_VT[i].y=pVertex_VT[k].y;
			pVertex_VT[i].z=pVertex_VT[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VT[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VT[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VT[i].x=pVertex_VT[k].x;
			pVertex_VT[i].y=pVertex_VT[k].y;
			pVertex_VT[i].z=pVertex_VT[k].z;
		}

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_VNT[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT[i].x=pVertex_VNT[k].x;
			pVertex_VNT[i].y=pVertex_VNT[k].y;
			pVertex_VNT[i].z=pVertex_VNT[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNT[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT[i].x=pVertex_VNT[k].x;
			pVertex_VNT[i].y=pVertex_VNT[k].y;
			pVertex_VNT[i].z=pVertex_VNT[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_VNT[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT[i].x=pVertex_VNT[k].x;
			pVertex_VNT[i].y=pVertex_VNT[k].y;
			pVertex_VNT[i].z=pVertex_VNT[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNT[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT[i].x=pVertex_VNT[k].x;
			pVertex_VNT[i].y=pVertex_VNT[k].y;
			pVertex_VNT[i].z=pVertex_VNT[k].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT3[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_VNT3[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT3[i].x=pVertex_VNT3[k].x;
			pVertex_VNT3[i].y=pVertex_VNT3[k].y;
			pVertex_VNT3[i].z=pVertex_VNT3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNT3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT3[i].x=pVertex_VNT3[k].x;
			pVertex_VNT3[i].y=pVertex_VNT3[k].y;
			pVertex_VNT3[i].z=pVertex_VNT3[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT3[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_VNT3[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT3[i].x=pVertex_VNT3[k].x;
			pVertex_VNT3[i].y=pVertex_VNT3[k].y;
			pVertex_VNT3[i].z=pVertex_VNT3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNT3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNT3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNT3[i].x=pVertex_VNT3[k].x;
			pVertex_VNT3[i].y=pVertex_VNT3[k].y;
			pVertex_VNT3[i].z=pVertex_VNT3[k].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3T3[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[k].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[k].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3T3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[k].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[k].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3T3[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[k].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[k].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3T3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[k].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[k].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[k].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2();

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1804");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1813");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		radialIncrement=(BOuterRadius-AOuterRadius)/(sreal)Stacks;			//From AOuterRadius to BOuterRadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR OUTER BODY OF TUBE
		phi=0.0;
		FTempo=AOuterRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BOuterRadius
		tempo3=TotalVertex_OuterBodyOfTube-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BOuterRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_OuterBodyOfTube; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS IS FOR INNER BODY OF TUBE
		radialIncrement=(BInnerRadius-AInnerRadius)/(sreal)Stacks;			//From AInnerRadius to BInnerRadius
		FTempo2=Height*0.5;		//POSITION_CENTER, start from AOuterRadius, go down to BOuterRadius
		phi=0.0;
		FTempo=AInnerRadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BInnerRadius
		tempo3=TotalVertex_OuterBodyOfTube+TotalVertex_InnerBodyOfTube-SlicesPlusOne;
		for(i=TotalVertex_OuterBodyOfTube, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				tempo=2;
				FTempo+=radialIncrement;
				FTempo2-=heightIncrement;
				phi=0.0;
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}

		//Last row
		FTempo=BInnerRadius;
		FTempo2=-Height*0.5;
		tempo3+=SlicesPlusOne;
		for(; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the tube
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF TUBE ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_TopOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3[i].x=AOuterRadius*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=AOuterRadius*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3[i].x=pVertex_VNTT3[k].x;
			pVertex_VNTT3[i].y=pVertex_VNTT3[k].y;
			pVertex_VNTT3[i].z=pVertex_VNTT3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				phi+=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3[i].x=pVertex_VNTT3[k].x;
			pVertex_VNTT3[i].y=pVertex_VNTT3[k].y;
			pVertex_VNTT3[i].z=pVertex_VNTT3[k].z;
		}

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF TUBE 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER

			k=i=StartIndexIn_pVertex_BottomOfTube;
			phi=0.0;
			//First circle
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3[i].x=BOuterRadius*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=BOuterRadius*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3[i].x=pVertex_VNTT3[k].x;
			pVertex_VNTT3[i].y=pVertex_VNTT3[k].y;
			pVertex_VNTT3[i].z=pVertex_VNTT3[k].z;
			i++;

			//Second circle
			phi=0.0;
			k=i;
			for(j=0; j<Slices; j++, i++)
			{
				pVertex_VNTT3[i].x=AInnerRadius*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=AInnerRadius*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				phi-=phiIncrement;
			}

			//The last vertices is the same as the first vertex
			pVertex_VNTT3[i].x=pVertex_VNTT3[k].x;
			pVertex_VNTT3[i].y=pVertex_VNTT3[k].y;
			pVertex_VNTT3[i].z=pVertex_VNTT3[k].z;
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
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 2099");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 2108");
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
			End_DrawRangeElements=End_DrawRangeElements_BottomOfTube;
		}
		else
		{
			End_DrawRangeElements=End_DrawRangeElements_TopOfTube;
		}
	}
	else if(IsThereABottom)
	{
		End_DrawRangeElements=End_DrawRangeElements_BottomOfTube;
	}
	else
	{
		End_DrawRangeElements=End_DrawRangeElements_InnerBodyOfTube;
	}

	return 1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//			Since the tube can have a different radius for the top and bottom, computing
//			the normals needs to take into account the angle.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TTube::GenerateNormals2()
{
	sint i, j, k, m, tempo2;
	sreal vector1[3], vector2[3], vector3[3];
	sreal vector4[3], vector5[3], vector6[3];

	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		//Normals
		//**** Start with the body of tube. Vertex - center
		//tempo2=TotalVertex_OuterBodyOfTube;

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT[i+1].x-pVertex_VNT[i].x;
				vector1[1]=pVertex_VNT[i+1].y-pVertex_VNT[i].y;
				vector1[2]=pVertex_VNT[i+1].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT[i+SlicesPlusOne].x-pVertex_VNT[i].x;
				vector2[1]=pVertex_VNT[i+SlicesPlusOne].y-pVertex_VNT[i].y;
				vector2[2]=pVertex_VNT[i+SlicesPlusOne].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i+Slices-1].x;
				vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i+Slices-1].y;
				vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT[i+Slices-1+SlicesPlusOne].x-pVertex_VNT[i+Slices-1].x;
				vector5[1]=pVertex_VNT[i+Slices-1+SlicesPlusOne].y-pVertex_VNT[i+Slices-1].y;
				vector5[2]=pVertex_VNT[i+Slices-1+SlicesPlusOne].z-pVertex_VNT[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT[i].nx=vector3[0]+vector6[0];
				pVertex_VNT[i].ny=vector3[1]+vector6[1];
				pVertex_VNT[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

				pVertex_VNT[i].nx=-pVertex_VNT[i].nx;
				pVertex_VNT[i].ny=-pVertex_VNT[i].ny;
				pVertex_VNT[i].nz=-pVertex_VNT[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT[i+1].x-pVertex_VNT[i].x;
				vector1[1]=pVertex_VNT[i+1].y-pVertex_VNT[i].y;
				vector1[2]=pVertex_VNT[i+1].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT[i+SlicesPlusOne].x-pVertex_VNT[i].x;
				vector2[1]=pVertex_VNT[i+SlicesPlusOne].y-pVertex_VNT[i].y;
				vector2[2]=pVertex_VNT[i+SlicesPlusOne].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i-1].x;
				vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i-1].y;
				vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT[i-1+SlicesPlusOne].x-pVertex_VNT[i-1].x;
				vector5[1]=pVertex_VNT[i-1+SlicesPlusOne].y-pVertex_VNT[i-1].y;
				vector5[2]=pVertex_VNT[i-1+SlicesPlusOne].z-pVertex_VNT[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT[i].nx=vector3[0]+vector6[0];
				pVertex_VNT[i].ny=vector3[1]+vector6[1];
				pVertex_VNT[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

				pVertex_VNT[i].nx=-pVertex_VNT[i].nx;
				pVertex_VNT[i].ny=-pVertex_VNT[i].ny;
				pVertex_VNT[i].nz=-pVertex_VNT[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNT[i].nx=pVertex_VNT[0].nx;
		pVertex_VNT[i].ny=pVertex_VNT[0].ny;
		pVertex_VNT[i].nz=pVertex_VNT[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNT[i].nx=pVertex_VNT[m].nx;
				pVertex_VNT[i].ny=pVertex_VNT[m].ny;
				pVertex_VNT[i].nz=pVertex_VNT[m].nz;
			}
		}

		//**** The inner part of the tube has the same normals as the outer part except they point the
		//opposite way.
		i=StartIndexIn_pVertex_InnerBodyOfTube;
		j=StartIndexIn_pVertex_OuterBodyOfTube;
		tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
		for(; i<tempo2; i++, j++)
		{
			pVertex_VNT[i].nx=-pVertex_VNT[j].nx;
			pVertex_VNT[i].ny=-pVertex_VNT[j].ny;
			pVertex_VNT[i].nz=-pVertex_VNT[j].nz;
		}

		//**** For top of tube
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfTube;
			tempo2=EndIndexIn_pVertex_TopOfTube;

			pVertex_VNT[i].nx=0.0;					//They all point to positive z
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNT[i].nx=pVertex_VNT[j].nx;
				pVertex_VNT[i].ny=pVertex_VNT[j].ny;
				pVertex_VNT[i].nz=pVertex_VNT[j].nz;
			}
		}

		//**** For bottom of tube
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfTube;
			tempo2=EndIndexIn_pVertex_BottomOfTube;

			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].nx=pVertex_VNT[j].nx;
				pVertex_VNT[i].ny=pVertex_VNT[j].ny;
				pVertex_VNT[i].nz=pVertex_VNT[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		//Normals
		//**** Start with the body of tube. Vertex - center
		//tempo2=TotalVertex_OuterBodyOfTube;

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT3[i+1].x-pVertex_VNT3[i].x;
				vector1[1]=pVertex_VNT3[i+1].y-pVertex_VNT3[i].y;
				vector1[2]=pVertex_VNT3[i+1].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT3[i+SlicesPlusOne].x-pVertex_VNT3[i].x;
				vector2[1]=pVertex_VNT3[i+SlicesPlusOne].y-pVertex_VNT3[i].y;
				vector2[2]=pVertex_VNT3[i+SlicesPlusOne].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i+Slices-1].x;
				vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i+Slices-1].y;
				vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].x-pVertex_VNT3[i+Slices-1].x;
				vector5[1]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].y-pVertex_VNT3[i+Slices-1].y;
				vector5[2]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].z-pVertex_VNT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

				pVertex_VNT3[i].nx=-pVertex_VNT3[i].nx;
				pVertex_VNT3[i].ny=-pVertex_VNT3[i].ny;
				pVertex_VNT3[i].nz=-pVertex_VNT3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT3[i+1].x-pVertex_VNT3[i].x;
				vector1[1]=pVertex_VNT3[i+1].y-pVertex_VNT3[i].y;
				vector1[2]=pVertex_VNT3[i+1].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT3[i+SlicesPlusOne].x-pVertex_VNT3[i].x;
				vector2[1]=pVertex_VNT3[i+SlicesPlusOne].y-pVertex_VNT3[i].y;
				vector2[2]=pVertex_VNT3[i+SlicesPlusOne].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i-1].x;
				vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i-1].y;
				vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT3[i-1+SlicesPlusOne].x-pVertex_VNT3[i-1].x;
				vector5[1]=pVertex_VNT3[i-1+SlicesPlusOne].y-pVertex_VNT3[i-1].y;
				vector5[2]=pVertex_VNT3[i-1+SlicesPlusOne].z-pVertex_VNT3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

				pVertex_VNT3[i].nx=-pVertex_VNT3[i].nx;
				pVertex_VNT3[i].ny=-pVertex_VNT3[i].ny;
				pVertex_VNT3[i].nz=-pVertex_VNT3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNT3[i].nx=pVertex_VNT3[0].nx;
		pVertex_VNT3[i].ny=pVertex_VNT3[0].ny;
		pVertex_VNT3[i].nz=pVertex_VNT3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNT3[i].nx=pVertex_VNT3[m].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[m].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[m].nz;
			}
		}

		//**** The inner part of the tube has the same normals as the outer part except they point the
		//opposite way.
		i=StartIndexIn_pVertex_InnerBodyOfTube;
		j=StartIndexIn_pVertex_OuterBodyOfTube;
		tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
		for(; i<tempo2; i++, j++)
		{
			pVertex_VNT3[i].nx=-pVertex_VNT3[j].nx;
			pVertex_VNT3[i].ny=-pVertex_VNT3[j].ny;
			pVertex_VNT3[i].nz=-pVertex_VNT3[j].nz;
		}

		//**** For top of tube
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfTube;
			tempo2=EndIndexIn_pVertex_TopOfTube;

			pVertex_VNT3[i].nx=0.0;					//They all point to positive z
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNT3[i].nx=pVertex_VNT3[j].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[j].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[j].nz;
			}
		}

		//**** For bottom of tube
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfTube;
			tempo2=EndIndexIn_pVertex_BottomOfTube;

			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].nx=pVertex_VNT3[j].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[j].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		//Normals
		//**** Start with the body of tube. Vertex - center
		//tempo2=TotalVertex_OuterBodyOfTube;

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3T3[i+1].x-pVertex_VNTT3T3[i].x;
				vector1[1]=pVertex_VNTT3T3[i+1].y-pVertex_VNTT3T3[i].y;
				vector1[2]=pVertex_VNTT3T3[i+1].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3T3[i+SlicesPlusOne].x-pVertex_VNTT3T3[i].x;
				vector2[1]=pVertex_VNTT3T3[i+SlicesPlusOne].y-pVertex_VNTT3T3[i].y;
				vector2[2]=pVertex_VNTT3T3[i+SlicesPlusOne].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i+Slices-1].x;
				vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i+Slices-1].y;
				vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].x-pVertex_VNTT3T3[i+Slices-1].x;
				vector5[1]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].y-pVertex_VNTT3T3[i+Slices-1].y;
				vector5[2]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].z-pVertex_VNTT3T3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3T3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3T3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3T3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

				pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[i].nx;
				pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[i].ny;
				pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3T3[i+1].x-pVertex_VNTT3T3[i].x;
				vector1[1]=pVertex_VNTT3T3[i+1].y-pVertex_VNTT3T3[i].y;
				vector1[2]=pVertex_VNTT3T3[i+1].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3T3[i+SlicesPlusOne].x-pVertex_VNTT3T3[i].x;
				vector2[1]=pVertex_VNTT3T3[i+SlicesPlusOne].y-pVertex_VNTT3T3[i].y;
				vector2[2]=pVertex_VNTT3T3[i+SlicesPlusOne].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i-1].x;
				vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i-1].y;
				vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3T3[i-1+SlicesPlusOne].x-pVertex_VNTT3T3[i-1].x;
				vector5[1]=pVertex_VNTT3T3[i-1+SlicesPlusOne].y-pVertex_VNTT3T3[i-1].y;
				vector5[2]=pVertex_VNTT3T3[i-1+SlicesPlusOne].z-pVertex_VNTT3T3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3T3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3T3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3T3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

				pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[i].nx;
				pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[i].ny;
				pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[0].nx;
		pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[0].ny;
		pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[m].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[m].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[m].nz;
			}
		}

		//**** The inner part of the tube has the same normals as the outer part except they point the
		//opposite way.
		i=StartIndexIn_pVertex_InnerBodyOfTube;
		j=StartIndexIn_pVertex_OuterBodyOfTube;
		tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
		for(; i<tempo2; i++, j++)
		{
			pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[j].nx;
			pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[j].ny;
			pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[j].nz;
		}

		//**** For top of tube
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfTube;
			tempo2=EndIndexIn_pVertex_TopOfTube;

			pVertex_VNTT3T3[i].nx=0.0;					//They all point to positive z
			pVertex_VNTT3T3[i].ny=0.0;
			pVertex_VNTT3T3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[j].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[j].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[j].nz;
			}
		}

		//**** For bottom of tube
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfTube;
			tempo2=EndIndexIn_pVertex_BottomOfTube;

			pVertex_VNTT3T3[i].nx=0.0;
			pVertex_VNTT3T3[i].ny=0.0;
			pVertex_VNTT3T3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[j].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[j].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		//Normals
		//**** Start with the body of tube. Vertex - center
		//tempo2=TotalVertex_OuterBodyOfTube;

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3[i+1].x-pVertex_VNTT3[i].x;
				vector1[1]=pVertex_VNTT3[i+1].y-pVertex_VNTT3[i].y;
				vector1[2]=pVertex_VNTT3[i+1].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3[i+SlicesPlusOne].x-pVertex_VNTT3[i].x;
				vector2[1]=pVertex_VNTT3[i+SlicesPlusOne].y-pVertex_VNTT3[i].y;
				vector2[2]=pVertex_VNTT3[i+SlicesPlusOne].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i+Slices-1].x;
				vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i+Slices-1].y;
				vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].x-pVertex_VNTT3[i+Slices-1].x;
				vector5[1]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].y-pVertex_VNTT3[i+Slices-1].y;
				vector5[2]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].z-pVertex_VNTT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

				pVertex_VNTT3[i].nx=-pVertex_VNTT3[i].nx;
				pVertex_VNTT3[i].ny=-pVertex_VNTT3[i].ny;
				pVertex_VNTT3[i].nz=-pVertex_VNTT3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3[i+1].x-pVertex_VNTT3[i].x;
				vector1[1]=pVertex_VNTT3[i+1].y-pVertex_VNTT3[i].y;
				vector1[2]=pVertex_VNTT3[i+1].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3[i+SlicesPlusOne].x-pVertex_VNTT3[i].x;
				vector2[1]=pVertex_VNTT3[i+SlicesPlusOne].y-pVertex_VNTT3[i].y;
				vector2[2]=pVertex_VNTT3[i+SlicesPlusOne].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i-1].x;
				vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i-1].y;
				vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3[i-1+SlicesPlusOne].x-pVertex_VNTT3[i-1].x;
				vector5[1]=pVertex_VNTT3[i-1+SlicesPlusOne].y-pVertex_VNTT3[i-1].y;
				vector5[2]=pVertex_VNTT3[i-1+SlicesPlusOne].z-pVertex_VNTT3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

				pVertex_VNTT3[i].nx=-pVertex_VNTT3[i].nx;
				pVertex_VNTT3[i].ny=-pVertex_VNTT3[i].ny;
				pVertex_VNTT3[i].nz=-pVertex_VNTT3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNTT3[i].nx=pVertex_VNTT3[0].nx;
		pVertex_VNTT3[i].ny=pVertex_VNTT3[0].ny;
		pVertex_VNTT3[i].nz=pVertex_VNTT3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNTT3[i].nx=pVertex_VNTT3[m].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[m].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[m].nz;
			}
		}

		//**** The inner part of the tube has the same normals as the outer part except they point the
		//opposite way.
		i=StartIndexIn_pVertex_InnerBodyOfTube;
		j=StartIndexIn_pVertex_OuterBodyOfTube;
		tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
		for(; i<tempo2; i++, j++)
		{
			pVertex_VNTT3[i].nx=-pVertex_VNTT3[j].nx;
			pVertex_VNTT3[i].ny=-pVertex_VNTT3[j].ny;
			pVertex_VNTT3[i].nz=-pVertex_VNTT3[j].nz;
		}

		//**** For top of tube
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfTube;
			tempo2=EndIndexIn_pVertex_TopOfTube;

			pVertex_VNTT3[i].nx=0.0;					//They all point to positive z
			pVertex_VNTT3[i].ny=0.0;
			pVertex_VNTT3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNTT3[i].nx=pVertex_VNTT3[j].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[j].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[j].nz;
			}
		}

		//**** For bottom of tube
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfTube;
			tempo2=EndIndexIn_pVertex_BottomOfTube;

			pVertex_VNTT3[i].nx=0.0;
			pVertex_VNTT3[i].ny=0.0;
			pVertex_VNTT3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].nx=pVertex_VNTT3[j].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[j].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[j].nz;
			}
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
sint TTube::GenerateTexCoords2()
{
	sint i, k, tempo2, tempo3;
	sreal FTempo, FTempo2, angle;

	if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if(TexCoordStyle==1)
		{
			FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
			FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

			//***************** OUTER BODY OF TUBE
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

			tempo2=EndIndexIn_pVertex_OuterBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
				pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** INNER BODY OF TUBE
			i=StartIndexIn_pVertex_InnerBodyOfTube;
			//Generate for first stack
			pVertex_VT[i].s0=0.0;
			pVertex_VT[i].t0=0.0;
			tempo2=StartIndexIn_pVertex_InnerBodyOfTube+Slices;
			for(i++; i<tempo2; i++)
			{
				pVertex_VT[i].s0=(pVertex_VT[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VT[i].t0=0.0;
			}
			pVertex_VT[i].s0=1.0*ScaleFactorS[0];
			pVertex_VT[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
				pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}


			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_TopOfTube;
				tempo3=EndIndexIn_pVertex_TopOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VT[i].s0=pVertex_VT[k].s0;
				pVertex_VT[i].t0=pVertex_VT[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_TopOfTube-1;
				FTempo2=0.5*AInnerRadius/AOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VT[i].s0=pVertex_VT[k].s0;
				pVertex_VT[i].t0=pVertex_VT[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_BottomOfTube;
				tempo3=EndIndexIn_pVertex_BottomOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VT[i].s0=pVertex_VT[k].s0;
				pVertex_VT[i].t0=pVertex_VT[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_BottomOfTube-1;
				FTempo2=0.5*BInnerRadius/BOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VT[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VT[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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
			FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
			FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

			//***************** OUTER BODY OF TUBE
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

			tempo2=EndIndexIn_pVertex_OuterBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
				pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** INNER BODY OF TUBE
			i=StartIndexIn_pVertex_InnerBodyOfTube;
			//Generate for first stack
			pVertex_VNT[i].s0=0.0;
			pVertex_VNT[i].t0=0.0;
			tempo2=StartIndexIn_pVertex_InnerBodyOfTube+Slices;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=(pVertex_VNT[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNT[i].t0=0.0;
			}
			pVertex_VNT[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNT[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
				pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}


			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_TopOfTube;
				tempo3=EndIndexIn_pVertex_TopOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT[i].s0=pVertex_VNT[k].s0;
				pVertex_VNT[i].t0=pVertex_VNT[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_TopOfTube-1;
				FTempo2=0.5*AInnerRadius/AOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT[i].s0=pVertex_VNT[k].s0;
				pVertex_VNT[i].t0=pVertex_VNT[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_BottomOfTube;
				tempo3=EndIndexIn_pVertex_BottomOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNT[i].s0=pVertex_VNT[k].s0;
				pVertex_VNT[i].t0=pVertex_VNT[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_BottomOfTube-1;
				FTempo2=0.5*BInnerRadius/BOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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
			FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
			FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

			//***************** OUTER BODY OF TUBE
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

			tempo2=EndIndexIn_pVertex_OuterBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
				pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				pVertex_VNT3[i].r0=0.0;
			}

			//***************** INNER BODY OF TUBE
			i=StartIndexIn_pVertex_InnerBodyOfTube;
			//Generate for first stack
			pVertex_VNT3[i].s0=0.0;
			pVertex_VNT3[i].t0=0.0;
			pVertex_VNT3[i].r0=0.0;
			tempo2=StartIndexIn_pVertex_InnerBodyOfTube+Slices;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=(pVertex_VNT3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNT3[i].t0=0.0;
				pVertex_VNT3[i].r0=0.0;
			}
			pVertex_VNT3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNT3[i].t0=0.0;
			pVertex_VNT3[i].r0=0.0;

			tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
				pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				pVertex_VNT3[i].r0=0.0;
			}


			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_TopOfTube;
				tempo3=EndIndexIn_pVertex_TopOfTube-SlicesPlusOne-1;
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
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_TopOfTube-1;
				FTempo2=0.5*AInnerRadius/AOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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

				k=i=StartIndexIn_pVertex_BottomOfTube;
				tempo3=EndIndexIn_pVertex_BottomOfTube-SlicesPlusOne-1;
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
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_BottomOfTube-1;
				FTempo2=0.5*BInnerRadius/BOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNT3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNT3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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
			FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
			FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

			//***************** OUTER BODY OF TUBE
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

			tempo2=EndIndexIn_pVertex_OuterBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
				pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** INNER BODY OF TUBE
			i=StartIndexIn_pVertex_InnerBodyOfTube;
			//Generate for first stack
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
			tempo2=StartIndexIn_pVertex_InnerBodyOfTube+Slices;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=(pVertex_VNTT3T3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNTT3T3[i].t0=0.0;
			}
			pVertex_VNTT3T3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNTT3T3[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
				pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}


			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_TopOfTube;
				tempo3=EndIndexIn_pVertex_TopOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[k].s0;
				pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_TopOfTube-1;
				FTempo2=0.5*AInnerRadius/AOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[k].s0;
				pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_BottomOfTube;
				tempo3=EndIndexIn_pVertex_BottomOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[k].s0;
				pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_BottomOfTube-1;
				FTempo2=0.5*BInnerRadius/BOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3T3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3T3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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
			FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
			FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

			//***************** OUTER BODY OF TUBE
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

			tempo2=EndIndexIn_pVertex_OuterBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
				pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}

			//***************** INNER BODY OF TUBE
			i=StartIndexIn_pVertex_InnerBodyOfTube;
			//Generate for first stack
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
			tempo2=StartIndexIn_pVertex_InnerBodyOfTube+Slices;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=(pVertex_VNTT3[i-1].s0+FTempo)*ScaleFactorS[0];
				pVertex_VNTT3[i].t0=0.0;
			}
			pVertex_VNTT3[i].s0=1.0*ScaleFactorS[0];
			pVertex_VNTT3[i].t0=0.0;

			tempo2=EndIndexIn_pVertex_InnerBodyOfTube;
			tempo3=SlicesPlusOne;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
				pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
			}


			if(IsThereATop)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_TopOfTube;
				tempo3=EndIndexIn_pVertex_TopOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3[i].s0=pVertex_VNTT3[k].s0;
				pVertex_VNTT3[i].t0=pVertex_VNTT3[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_TopOfTube-1;
				FTempo2=0.5*AInnerRadius/AOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3[i].s0=pVertex_VNTT3[k].s0;
				pVertex_VNTT3[i].t0=pVertex_VNTT3[k].t0;
			}

			if(IsThereABottom)
			{
				angle=PI_DEF/4.0;		//Start at 45 degrees
				FTempo=PI_FULL_DEF/sreal(Slices);

				k=i=StartIndexIn_pVertex_BottomOfTube;
				tempo3=EndIndexIn_pVertex_BottomOfTube-SlicesPlusOne-1;
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
				}

				//Fix last one cause it's the same point as the first
				pVertex_VNTT3[i].s0=pVertex_VNTT3[k].s0;
				pVertex_VNTT3[i].t0=pVertex_VNTT3[k].t0;
				i++;

				angle=PI_DEF/4.0;		//Start at 45 degrees
				k=i;
				tempo3=EndIndexIn_pVertex_BottomOfTube-1;
				FTempo2=0.5*BInnerRadius/BOuterRadius;		//Compute the radius
				for(; i<tempo3; i++, angle-=FTempo)
				{
					pVertex_VNTT3[i].s0=FTempo2*cosf(angle)*ScaleFactorS[0]+0.5;
					pVertex_VNTT3[i].t0=FTempo2*sinf(angle)*ScaleFactorT[0]+0.5;
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



