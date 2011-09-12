// TCone.cpp: implementation of the TCone class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCone.h"
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

TCone::TCone()
{
	GeneralReset();
}

TCone::~TCone()
{
	DeallocateAllMemory();
}

void TCone::GeneralReset()
{
	TBaseShape::GeneralReset();

	IsThereABottom=FALSE;
	Radius=Height=0.0;
	Stacks=Slices=0;

	Start_DrawRangeElements_BodyOfCone=End_DrawRangeElements_BodyOfCone=0;
	Start_DrawRangeElements_BottomOfCone=End_DrawRangeElements_BottomOfCone=0;
	StartIndexIn_pIndex_BodyOfCone=EndIndexIn_pIndex_BodyOfCone=0;
	StartIndexIn_pIndex_BottomOfCone=EndIndexIn_pIndex_BottomOfCone=0;

	TotalIndex_BodyOfCone=TotalIndex_BottomOfCone=0;

	pIndex16Bit_BodyOfCone=NULL;
	pIndex16Bit_BottomOfCone=NULL;

	pIndex32Bit_BodyOfCone=NULL;
	pIndex32Bit_BottomOfCone=NULL;

	StartIndexIn_pVertex_BodyOfCone=EndIndexIn_pVertex_BodyOfCone=0;
	StartIndexIn_pVertex_BottomOfCone=EndIndexIn_pVertex_BottomOfCone=0;

	TotalVertex_BodyOfCone=TotalVertex_BottomOfCone=0;

	pVertex_BodyOfCone=NULL;
	pVertex_BottomOfCone=NULL;
}

//PURPOSE:	Deallocate all reserved memory
void TCone::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}

//PURPOSE:	Must call this at startup to create the cone.
//			Consider a right handed coordinate system where the x axis is along the horizontal
//			of your screen, y is up. z is pointing to you.
//			The torus will be a wheel on the xy plane.
sint TCone::StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		tbool isThereABottom, sreal radius, sreal height, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<1) ? 1 : stacks;

	PolygonNumber=Slices*Stacks*2;		//Triangle count

	IsThereABottom=isThereABottom;
	Radius=(radius<=0.0) ? 1.0 : radius;
	Height=(height<=0.0) ? 1.0 : height;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 79");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 85");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 90");
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
	DeallocateMemoryForTangentVectors();
	DeallocateMemoryForBinormalVectors();

	DeallocateMemoryVertexInterlaced();

	if(GenerateIndices()!=1)
		return -1;

	GenerateVertices2();

	return 1;
}


//PURPOSE:	The indices will be generated whether vertices exist or not
//			but Stacks and Slices must be already set when calling this.
//
//RETURN:
//			1 for success
//			-1 if 16 bit indices are not enough, so switching to 32 bit.
sint TCone::GenerateIndices()
{
	sint i, j, k, tempo2, tempo3;
	sint vertexCount;
	tbool switchOccured=FALSE;


	if((Stacks<=0)||(Slices<=0))
		return 1;

	vertexCount=(Slices+1)*Stacks+1;		//+1 is for the vertex at the top
	End_DrawRangeElements_BodyOfCone=vertexCount-1;
	EndIndexIn_pVertex_BodyOfCone=vertexCount-1;
	Start_DrawRangeElements_BottomOfCone=vertexCount;
	StartIndexIn_pVertex_BottomOfCone=vertexCount;
	if(IsThereABottom)
	{
		vertexCount+=Slices+2;				//+1 for an extra vertex and +1 for the center of the bottom
		End_DrawRangeElements_BottomOfCone=vertexCount-1;
		EndIndexIn_pVertex_BottomOfCone=vertexCount-1;
	}

	tempo3=(Slices*2)*(Stacks-1)*3;				//*3 because 3 indices per triangle
	tempo3+=Slices*3;							//The is for the last stack
												//*3 because 3 indices per triangle
	TotalIndex_BodyOfCone=tempo3;
	
	if(IsThereABottom)
	{
		TotalIndex_BottomOfCone=Slices*3;	//*3 because 3 indices per triangle
		tempo3+=TotalIndex_BottomOfCone;
	}

	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 368");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		AllocateExact_1(pIndex16Bit, &UsedIndex, tempo3);

		pIndex16Bit_BodyOfCone=pIndex16Bit;

		ushort index=0;
		ushort otherStack=(ushort)(Slices+1);
		ushort pointyIndex;
		k=0;
		tempo2=Stacks-1;				//The last stack is the one with the tip of the cone which is a special case
		for(i=0; i<tempo2; i++)
		{
			for(j=0; j<Slices; j++, k+=6)
			{
				pIndex16Bit[k  ]=index+1;
				pIndex16Bit[k+1]=index+1+otherStack;
				pIndex16Bit[k+2]=index;
				pIndex16Bit[k+3]=index;
				pIndex16Bit[k+4]=pIndex16Bit[k+1];
				pIndex16Bit[k+5]=pIndex16Bit[k+1]-1;
				index++;
			}

			index++;
		}

		pointyIndex=(ushort)((Slices+1)*Stacks);
		for(j=0; j<Slices; j++, k+=3)
		{
			pIndex16Bit[k  ]=index;
			pIndex16Bit[k+1]=index+1;
			pIndex16Bit[k+2]=pointyIndex;
			index++;
		}

		EndIndexIn_pIndex_BodyOfCone=pointyIndex;
		index=pointyIndex+1;

		if(IsThereABottom)
		{
			StartIndexIn_pIndex_BottomOfCone=index;

			pIndex16Bit_BottomOfCone=&pIndex16Bit[TotalIndex_BodyOfCone];

			ushort bottomCenter=index;
			index++;

			for(j=0; j<Slices; j++, k+=3)
			{
				pIndex16Bit[k  ]=bottomCenter;
				pIndex16Bit[k+1]=index;
				pIndex16Bit[k+2]=index+1;
				index++;
			}

			EndIndexIn_pIndex_BottomOfCone=UsedIndex-1;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		AllocateExact_1(pIndex32Bit, &UsedIndex, tempo3);

		pIndex32Bit_BodyOfCone=pIndex32Bit;

		uint index=0;
		uint otherStack=(uint)(Slices+1);
		uint pointyIndex;
		k=0;
		tempo2=Stacks-1;				//The last stack is the one with the tip of the cone which is a special case
		for(i=0; i<tempo2; i++)
		{
			for(j=0; j<Slices; j++, k+=6)
			{
				pIndex32Bit[k  ]=index+1;
				pIndex32Bit[k+1]=index+1+otherStack;
				pIndex32Bit[k+2]=index;
				pIndex32Bit[k+3]=index;
				pIndex32Bit[k+4]=pIndex32Bit[k+1];
				pIndex32Bit[k+5]=pIndex32Bit[k+1]-1;
				index++;
			}

			index++;
		}

		pointyIndex=(uint)((Slices+1)*Stacks);
		for(j=0; j<Slices; j++, k+=3)
		{
			pIndex32Bit[k  ]=index;
			pIndex32Bit[k+1]=index+1;
			pIndex32Bit[k+2]=pointyIndex;
			index++;
		}

		EndIndexIn_pIndex_BodyOfCone=pointyIndex;
		index=pointyIndex+1;

		if(IsThereABottom)
		{
			StartIndexIn_pIndex_BottomOfCone=index;

			pIndex32Bit_BottomOfCone=&pIndex32Bit[TotalIndex_BodyOfCone];

			uint bottomCenter=index;
			index++;

			for(j=0; j<Slices; j++, k+=3)
			{
				pIndex32Bit[k  ]=bottomCenter;
				pIndex32Bit[k+1]=index;
				pIndex32Bit[k+2]=index+1;
				index++;
			}

			EndIndexIn_pIndex_BottomOfCone=UsedIndex-1;
		}
	}

	if(switchOccured==FALSE)
		return 1;
	else
		return -1;
}

//PURPOSE:	Calculate all the vertices.
//			There will be some duplicate vertices but not much. Indices must be generated as well.
//			Certain vertices that are the same will the same normal as well but texcoord
//			tangent and binormals will be different.
//
//RETURN:
//			1 for success.
sint TCone::GenerateVertices2()
{
	sint i, j, k, n, returnVal;
	sreal FTempo1, FTempo2, FTempo3, FTempo4, FTempo5, FTempo6;


	VertexCount=(Slices+1)*Stacks+1;		//+1 is for the vertex at the top
	TotalVertex_BodyOfCone=VertexCount;
	End_DrawRangeElements_BodyOfCone=VertexCount-1;
	EndIndexIn_pVertex_BodyOfCone=VertexCount-1;
	Start_DrawRangeElements_BottomOfCone=VertexCount;
	StartIndexIn_pVertex_BottomOfCone=VertexCount;
	if(IsThereABottom)
	{
		TotalVertex_BottomOfCone=Slices+2;				//+1 for an extra vertex and +1 for the center of the bottom
		VertexCount+=TotalVertex_BottomOfCone;
		End_DrawRangeElements_BottomOfCone=VertexCount-1;
		EndIndexIn_pVertex_BottomOfCone=VertexCount-1;
	}

	Start_DrawRangeElements=0;
	End_DrawRangeElements=VertexCount-1;


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		pVertex_V=new GLHVertex_V[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_V;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_V[k].x=FTempo5*cosf(FTempo1);
				pVertex_V[k].y=FTempo5*sinf(FTempo1);
				pVertex_V[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_V[k].x=pVertex_V[n].x;
			pVertex_V[k].y=pVertex_V[n].y;
			pVertex_V[k].z=pVertex_V[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_V[k].x=0.0;
		pVertex_V[k].y=0.0;
		pVertex_V[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_V[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_V[k].x=0.0;
			pVertex_V[k].y=0.0;
			pVertex_V[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_V[k].x=FTempo5*cosf(FTempo1);
				pVertex_V[k].y=FTempo5*sinf(FTempo1);
				pVertex_V[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_V[k].x=pVertex_V[n].x;
			pVertex_V[k].y=pVertex_V[n].y;
			pVertex_V[k].z=pVertex_V[n].z;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		pVertex_VT=new GLHVertex_VT[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_VT;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VT[k].x=FTempo5*cosf(FTempo1);
				pVertex_VT[k].y=FTempo5*sinf(FTempo1);
				pVertex_VT[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_VT[k].x=pVertex_VT[n].x;
			pVertex_VT[k].y=pVertex_VT[n].y;
			pVertex_VT[k].z=pVertex_VT[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_VT[k].x=0.0;
		pVertex_VT[k].y=0.0;
		pVertex_VT[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_VT[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_VT[k].x=0.0;
			pVertex_VT[k].y=0.0;
			pVertex_VT[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VT[k].x=FTempo5*cosf(FTempo1);
				pVertex_VT[k].y=FTempo5*sinf(FTempo1);
				pVertex_VT[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_VT[k].x=pVertex_VT[n].x;
			pVertex_VT[k].y=pVertex_VT[n].y;
			pVertex_VT[k].z=pVertex_VT[n].z;
		}

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		pVertex_VNT=new GLHVertex_VNT[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_VNT;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNT[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNT[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_VNT[k].x=pVertex_VNT[n].x;
			pVertex_VNT[k].y=pVertex_VNT[n].y;
			pVertex_VNT[k].z=pVertex_VNT[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_VNT[k].x=0.0;
		pVertex_VNT[k].y=0.0;
		pVertex_VNT[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_VNT[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_VNT[k].x=0.0;
			pVertex_VNT[k].y=0.0;
			pVertex_VNT[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNT[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNT[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_VNT[k].x=pVertex_VNT[n].x;
			pVertex_VNT[k].y=pVertex_VNT[n].y;
			pVertex_VNT[k].z=pVertex_VNT[n].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_VNT3;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNT3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNT3[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_VNT3[k].x=pVertex_VNT3[n].x;
			pVertex_VNT3[k].y=pVertex_VNT3[n].y;
			pVertex_VNT3[k].z=pVertex_VNT3[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_VNT3[k].x=0.0;
		pVertex_VNT3[k].y=0.0;
		pVertex_VNT3[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_VNT3[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_VNT3[k].x=0.0;
			pVertex_VNT3[k].y=0.0;
			pVertex_VNT3[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNT3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNT3[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_VNT3[k].x=pVertex_VNT3[n].x;
			pVertex_VNT3[k].y=pVertex_VNT3[n].y;
			pVertex_VNT3[k].z=pVertex_VNT3[n].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_VNTT3T3;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3T3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNTT3T3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNTT3T3[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_VNTT3T3[k].x=pVertex_VNTT3T3[n].x;
			pVertex_VNTT3T3[k].y=pVertex_VNTT3T3[n].y;
			pVertex_VNTT3T3[k].z=pVertex_VNTT3T3[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_VNTT3T3[k].x=0.0;
		pVertex_VNTT3T3[k].y=0.0;
		pVertex_VNTT3T3[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_VNTT3T3[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_VNTT3T3[k].x=0.0;
			pVertex_VNTT3T3[k].y=0.0;
			pVertex_VNTT3T3[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3T3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNTT3T3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNTT3T3[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_VNTT3T3[k].x=pVertex_VNTT3T3[n].x;
			pVertex_VNTT3T3[k].y=pVertex_VNTT3T3[n].y;
			pVertex_VNTT3T3[k].z=pVertex_VNTT3T3[n].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 717");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 726");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

		pVertex_BodyOfCone=(sreal *)pVertex_VNTT3;

		FTempo1=0.0;
		FTempo2=(2.0*M_PI)/(sreal)Slices;			//Increment
		FTempo3=-Height*0.5;
		FTempo4=Height/(sreal)Stacks;				//Increment
		FTempo5=Radius;
		FTempo6=-Radius/(sreal)Stacks;
		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNTT3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNTT3[k].z=FTempo3;
				FTempo1+=FTempo2;
			}

			//The last vertex is the same as the first
			pVertex_VNTT3[k].x=pVertex_VNTT3[n].x;
			pVertex_VNTT3[k].y=pVertex_VNTT3[n].y;
			pVertex_VNTT3[k].z=pVertex_VNTT3[n].z;
			k++;

			FTempo3+=FTempo4;
			FTempo5+=FTempo6;
		}

		//The top vertex
		pVertex_VNTT3[k].x=0.0;
		pVertex_VNTT3[k].y=0.0;
		pVertex_VNTT3[k].z=Height*0.5;
		k++;

		if(IsThereABottom)
		{
			pVertex_BottomOfCone=(sreal *)&pVertex_VNTT3[TotalVertex_BodyOfCone].x;

			FTempo3=-Height*0.5;

			//It is basically a circle with a vertex at the center.
			//The bottom center vertex :
			pVertex_VNTT3[k].x=0.0;
			pVertex_VNTT3[k].y=0.0;
			pVertex_VNTT3[k].z=FTempo3;
			k++;

			FTempo5=Radius;
			FTempo1=0.0;
			n=k;
			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3[k].x=FTempo5*cosf(FTempo1);
				pVertex_VNTT3[k].y=FTempo5*sinf(FTempo1);
				pVertex_VNTT3[k].z=FTempo3;
				FTempo1-=FTempo2;							//Decrement
			}

			//The last vertex is the same as the first
			pVertex_VNTT3[k].x=pVertex_VNTT3[n].x;
			pVertex_VNTT3[k].y=pVertex_VNTT3[n].y;
			pVertex_VNTT3[k].z=pVertex_VNTT3[n].z;
		}

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);

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
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 826");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 835");
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

	return 1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//			NOTE: The top of the cone, there is a single vertex and the normal
//			will point to {0.0, 0.0, 1.0}. This might give undersireable lighting effect.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TCone::GenerateNormals2()
{
	sint i, j, k, m;
	sreal vector1[3], vector2[3], vector3[3];
	sreal vector4[3], vector5[3], vector6[3];
	sint slicesPlusOne=Slices+1;
	sint pointyVertex;

	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if(Stacks>1)
		{
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

					vector2[0]=pVertex_VNT[i+slicesPlusOne].x-pVertex_VNT[i].x;
					vector2[1]=pVertex_VNT[i+slicesPlusOne].y-pVertex_VNT[i].y;
					vector2[2]=pVertex_VNT[i+slicesPlusOne].z-pVertex_VNT[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i+Slices-1].x;
					vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i+Slices-1].y;
					vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT[i+Slices-1+slicesPlusOne].x-pVertex_VNT[i+Slices-1].x;
					vector5[1]=pVertex_VNT[i+Slices-1+slicesPlusOne].y-pVertex_VNT[i+Slices-1].y;
					vector5[2]=pVertex_VNT[i+Slices-1+slicesPlusOne].z-pVertex_VNT[i+Slices-1].z;
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

					vector2[0]=pVertex_VNT[i+slicesPlusOne].x-pVertex_VNT[i].x;
					vector2[1]=pVertex_VNT[i+slicesPlusOne].y-pVertex_VNT[i].y;
					vector2[2]=pVertex_VNT[i+slicesPlusOne].z-pVertex_VNT[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i-1].x;
					vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i-1].y;
					vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT[i-1+slicesPlusOne].x-pVertex_VNT[i-1].x;
					vector5[1]=pVertex_VNT[i-1+slicesPlusOne].y-pVertex_VNT[i-1].y;
					vector5[2]=pVertex_VNT[i-1+slicesPlusOne].z-pVertex_VNT[i-1].z;
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
		}
		else //if(Stacks==1)
		{	//The case when there is only 1 stack
			pointyVertex=Slices+1;
			for(i=0; i<Slices; i++)
			{
				if(i==0)
				{
					//Compute normal vector 1
					vector1[0]=pVertex_VNT[i+1].x-pVertex_VNT[i].x;
					vector1[1]=pVertex_VNT[i+1].y-pVertex_VNT[i].y;
					vector1[2]=pVertex_VNT[i+1].z-pVertex_VNT[i].z;
					NormalizeVectorFLOAT_2(vector1);

					vector2[0]=pVertex_VNT[pointyVertex].x-pVertex_VNT[i].x;
					vector2[1]=pVertex_VNT[pointyVertex].y-pVertex_VNT[i].y;
					vector2[2]=pVertex_VNT[pointyVertex].z-pVertex_VNT[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i+Slices-1].x;
					vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i+Slices-1].y;
					vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT[pointyVertex].x-pVertex_VNT[i+Slices-1].x;
					vector5[1]=pVertex_VNT[pointyVertex].y-pVertex_VNT[i+Slices-1].y;
					vector5[2]=pVertex_VNT[pointyVertex].z-pVertex_VNT[i+Slices-1].z;
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

					vector2[0]=pVertex_VNT[pointyVertex].x-pVertex_VNT[i].x;
					vector2[1]=pVertex_VNT[pointyVertex].y-pVertex_VNT[i].y;
					vector2[2]=pVertex_VNT[pointyVertex].z-pVertex_VNT[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i-1].x;
					vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i-1].y;
					vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT[pointyVertex].x-pVertex_VNT[i-1].x;
					vector5[1]=pVertex_VNT[pointyVertex].y-pVertex_VNT[i-1].y;
					vector5[2]=pVertex_VNT[pointyVertex].z-pVertex_VNT[i-1].z;
					NormalizeVectorFLOAT_2(vector5);

					ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

					//Now average normal from face 1 and 2
					pVertex_VNT[i].nx=vector3[0]+vector6[0];
					pVertex_VNT[i].ny=vector3[1]+vector6[1];
					pVertex_VNT[i].nz=vector3[2]+vector6[2];
					NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

					//pVertex_VNT[i].nx=-pVertex_VNT[i].nx;
					//pVertex_VNT[i].ny=-pVertex_VNT[i].ny;
					//pVertex_VNT[i].nz=-pVertex_VNT[i].nz;
				}
			}

			//The last vertex on the stack is the same as the first
			pVertex_VNT[i].nx=pVertex_VNT[0].nx;
			pVertex_VNT[i].ny=pVertex_VNT[0].ny;
			pVertex_VNT[i].nz=pVertex_VNT[0].nz;
			i++;
		}

		for(k=1; k<Stacks; k++)
		{
			for(j=m=0; j<=Slices; j++, i++, m++)
			{
				pVertex_VNT[i].nx=pVertex_VNT[m].nx;
				pVertex_VNT[i].ny=pVertex_VNT[m].ny;
				pVertex_VNT[i].nz=pVertex_VNT[m].nz;
			}
		}

		//The top pointy part has 1 vertex. Normal will point to {0.0, 0.0, 1.0}
		pVertex_VNT[i].nx=0.0;
		pVertex_VNT[i].ny=0.0;
		pVertex_VNT[i].nz=1.0;
		i++;

		if(IsThereABottom)
		{	//All the normals point to {0.0, 0.0, -1.0}
			for(; i<VertexCount; i++)
			{
				pVertex_VNT[i].nx=0.0;
				pVertex_VNT[i].ny=0.0;
				pVertex_VNT[i].nz=-1.0;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if(Stacks>1)
		{
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

					vector2[0]=pVertex_VNT3[i+slicesPlusOne].x-pVertex_VNT3[i].x;
					vector2[1]=pVertex_VNT3[i+slicesPlusOne].y-pVertex_VNT3[i].y;
					vector2[2]=pVertex_VNT3[i+slicesPlusOne].z-pVertex_VNT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i+Slices-1].x;
					vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i+Slices-1].y;
					vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT3[i+Slices-1+slicesPlusOne].x-pVertex_VNT3[i+Slices-1].x;
					vector5[1]=pVertex_VNT3[i+Slices-1+slicesPlusOne].y-pVertex_VNT3[i+Slices-1].y;
					vector5[2]=pVertex_VNT3[i+Slices-1+slicesPlusOne].z-pVertex_VNT3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNT3[i+slicesPlusOne].x-pVertex_VNT3[i].x;
					vector2[1]=pVertex_VNT3[i+slicesPlusOne].y-pVertex_VNT3[i].y;
					vector2[2]=pVertex_VNT3[i+slicesPlusOne].z-pVertex_VNT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i-1].x;
					vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i-1].y;
					vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT3[i-1+slicesPlusOne].x-pVertex_VNT3[i-1].x;
					vector5[1]=pVertex_VNT3[i-1+slicesPlusOne].y-pVertex_VNT3[i-1].y;
					vector5[2]=pVertex_VNT3[i-1+slicesPlusOne].z-pVertex_VNT3[i-1].z;
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
		}
		else //if(Stacks==1)
		{	//The case when there is only 1 stack
			pointyVertex=Slices+1;
			for(i=0; i<Slices; i++)
			{
				if(i==0)
				{
					//Compute normal vector 1
					vector1[0]=pVertex_VNT3[i+1].x-pVertex_VNT3[i].x;
					vector1[1]=pVertex_VNT3[i+1].y-pVertex_VNT3[i].y;
					vector1[2]=pVertex_VNT3[i+1].z-pVertex_VNT3[i].z;
					NormalizeVectorFLOAT_2(vector1);

					vector2[0]=pVertex_VNT3[pointyVertex].x-pVertex_VNT3[i].x;
					vector2[1]=pVertex_VNT3[pointyVertex].y-pVertex_VNT3[i].y;
					vector2[2]=pVertex_VNT3[pointyVertex].z-pVertex_VNT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i+Slices-1].x;
					vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i+Slices-1].y;
					vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT3[pointyVertex].x-pVertex_VNT3[i+Slices-1].x;
					vector5[1]=pVertex_VNT3[pointyVertex].y-pVertex_VNT3[i+Slices-1].y;
					vector5[2]=pVertex_VNT3[pointyVertex].z-pVertex_VNT3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNT3[pointyVertex].x-pVertex_VNT3[i].x;
					vector2[1]=pVertex_VNT3[pointyVertex].y-pVertex_VNT3[i].y;
					vector2[2]=pVertex_VNT3[pointyVertex].z-pVertex_VNT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i-1].x;
					vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i-1].y;
					vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNT3[pointyVertex].x-pVertex_VNT3[i-1].x;
					vector5[1]=pVertex_VNT3[pointyVertex].y-pVertex_VNT3[i-1].y;
					vector5[2]=pVertex_VNT3[pointyVertex].z-pVertex_VNT3[i-1].z;
					NormalizeVectorFLOAT_2(vector5);

					ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

					//Now average normal from face 1 and 2
					pVertex_VNT3[i].nx=vector3[0]+vector6[0];
					pVertex_VNT3[i].ny=vector3[1]+vector6[1];
					pVertex_VNT3[i].nz=vector3[2]+vector6[2];
					NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

					//pVertex_VNT3[i].nx=-pVertex_VNT3[i].nx;
					//pVertex_VNT3[i].ny=-pVertex_VNT3[i].ny;
					//pVertex_VNT3[i].nz=-pVertex_VNT3[i].nz;
				}
			}

			//The last vertex on the stack is the same as the first
			pVertex_VNT3[i].nx=pVertex_VNT3[0].nx;
			pVertex_VNT3[i].ny=pVertex_VNT3[0].ny;
			pVertex_VNT3[i].nz=pVertex_VNT3[0].nz;
			i++;
		}

		for(k=1; k<Stacks; k++)
		{
			for(j=m=0; j<=Slices; j++, i++, m++)
			{
				pVertex_VNT3[i].nx=pVertex_VNT3[m].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[m].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[m].nz;
			}
		}

		//The top pointy part has 1 vertex. Normal will point to {0.0, 0.0, 1.0}
		pVertex_VNT3[i].nx=0.0;
		pVertex_VNT3[i].ny=0.0;
		pVertex_VNT3[i].nz=1.0;
		i++;

		if(IsThereABottom)
		{	//All the normals point to {0.0, 0.0, -1.0}
			for(; i<VertexCount; i++)
			{
				pVertex_VNT3[i].nx=0.0;
				pVertex_VNT3[i].ny=0.0;
				pVertex_VNT3[i].nz=-1.0;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if(Stacks>1)
		{
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

					vector2[0]=pVertex_VNTT3T3[i+slicesPlusOne].x-pVertex_VNTT3T3[i].x;
					vector2[1]=pVertex_VNTT3T3[i+slicesPlusOne].y-pVertex_VNTT3T3[i].y;
					vector2[2]=pVertex_VNTT3T3[i+slicesPlusOne].z-pVertex_VNTT3T3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i+Slices-1].x;
					vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i+Slices-1].y;
					vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3T3[i+Slices-1+slicesPlusOne].x-pVertex_VNTT3T3[i+Slices-1].x;
					vector5[1]=pVertex_VNTT3T3[i+Slices-1+slicesPlusOne].y-pVertex_VNTT3T3[i+Slices-1].y;
					vector5[2]=pVertex_VNTT3T3[i+Slices-1+slicesPlusOne].z-pVertex_VNTT3T3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNTT3T3[i+slicesPlusOne].x-pVertex_VNTT3T3[i].x;
					vector2[1]=pVertex_VNTT3T3[i+slicesPlusOne].y-pVertex_VNTT3T3[i].y;
					vector2[2]=pVertex_VNTT3T3[i+slicesPlusOne].z-pVertex_VNTT3T3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i-1].x;
					vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i-1].y;
					vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3T3[i-1+slicesPlusOne].x-pVertex_VNTT3T3[i-1].x;
					vector5[1]=pVertex_VNTT3T3[i-1+slicesPlusOne].y-pVertex_VNTT3T3[i-1].y;
					vector5[2]=pVertex_VNTT3T3[i-1+slicesPlusOne].z-pVertex_VNTT3T3[i-1].z;
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
		}
		else //if(Stacks==1)
		{	//The case when there is only 1 stack
			pointyVertex=Slices+1;
			for(i=0; i<Slices; i++)
			{
				if(i==0)
				{
					//Compute normal vector 1
					vector1[0]=pVertex_VNTT3T3[i+1].x-pVertex_VNTT3T3[i].x;
					vector1[1]=pVertex_VNTT3T3[i+1].y-pVertex_VNTT3T3[i].y;
					vector1[2]=pVertex_VNTT3T3[i+1].z-pVertex_VNTT3T3[i].z;
					NormalizeVectorFLOAT_2(vector1);

					vector2[0]=pVertex_VNTT3T3[pointyVertex].x-pVertex_VNTT3T3[i].x;
					vector2[1]=pVertex_VNTT3T3[pointyVertex].y-pVertex_VNTT3T3[i].y;
					vector2[2]=pVertex_VNTT3T3[pointyVertex].z-pVertex_VNTT3T3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i+Slices-1].x;
					vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i+Slices-1].y;
					vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3T3[pointyVertex].x-pVertex_VNTT3T3[i+Slices-1].x;
					vector5[1]=pVertex_VNTT3T3[pointyVertex].y-pVertex_VNTT3T3[i+Slices-1].y;
					vector5[2]=pVertex_VNTT3T3[pointyVertex].z-pVertex_VNTT3T3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNTT3T3[pointyVertex].x-pVertex_VNTT3T3[i].x;
					vector2[1]=pVertex_VNTT3T3[pointyVertex].y-pVertex_VNTT3T3[i].y;
					vector2[2]=pVertex_VNTT3T3[pointyVertex].z-pVertex_VNTT3T3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i-1].x;
					vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i-1].y;
					vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3T3[pointyVertex].x-pVertex_VNTT3T3[i-1].x;
					vector5[1]=pVertex_VNTT3T3[pointyVertex].y-pVertex_VNTT3T3[i-1].y;
					vector5[2]=pVertex_VNTT3T3[pointyVertex].z-pVertex_VNTT3T3[i-1].z;
					NormalizeVectorFLOAT_2(vector5);

					ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

					//Now average normal from face 1 and 2
					pVertex_VNTT3T3[i].nx=vector3[0]+vector6[0];
					pVertex_VNTT3T3[i].ny=vector3[1]+vector6[1];
					pVertex_VNTT3T3[i].nz=vector3[2]+vector6[2];
					NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

					//pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[i].nx;
					//pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[i].ny;
					//pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[i].nz;
				}
			}

			//The last vertex on the stack is the same as the first
			pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[0].nx;
			pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[0].ny;
			pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[0].nz;
			i++;
		}

		for(k=1; k<Stacks; k++)
		{
			for(j=m=0; j<=Slices; j++, i++, m++)
			{
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[m].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[m].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[m].nz;
			}
		}

		//The top pointy part has 1 vertex. Normal will point to {0.0, 0.0, 1.0}
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=1.0;
		i++;

		if(IsThereABottom)
		{	//All the normals point to {0.0, 0.0, -1.0}
			for(; i<VertexCount; i++)
			{
				pVertex_VNTT3T3[i].nx=0.0;
				pVertex_VNTT3T3[i].ny=0.0;
				pVertex_VNTT3T3[i].nz=-1.0;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if(Stacks>1)
		{
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

					vector2[0]=pVertex_VNTT3[i+slicesPlusOne].x-pVertex_VNTT3[i].x;
					vector2[1]=pVertex_VNTT3[i+slicesPlusOne].y-pVertex_VNTT3[i].y;
					vector2[2]=pVertex_VNTT3[i+slicesPlusOne].z-pVertex_VNTT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i+Slices-1].x;
					vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i+Slices-1].y;
					vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3[i+Slices-1+slicesPlusOne].x-pVertex_VNTT3[i+Slices-1].x;
					vector5[1]=pVertex_VNTT3[i+Slices-1+slicesPlusOne].y-pVertex_VNTT3[i+Slices-1].y;
					vector5[2]=pVertex_VNTT3[i+Slices-1+slicesPlusOne].z-pVertex_VNTT3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNTT3[i+slicesPlusOne].x-pVertex_VNTT3[i].x;
					vector2[1]=pVertex_VNTT3[i+slicesPlusOne].y-pVertex_VNTT3[i].y;
					vector2[2]=pVertex_VNTT3[i+slicesPlusOne].z-pVertex_VNTT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i-1].x;
					vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i-1].y;
					vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3[i-1+slicesPlusOne].x-pVertex_VNTT3[i-1].x;
					vector5[1]=pVertex_VNTT3[i-1+slicesPlusOne].y-pVertex_VNTT3[i-1].y;
					vector5[2]=pVertex_VNTT3[i-1+slicesPlusOne].z-pVertex_VNTT3[i-1].z;
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
		}
		else //if(Stacks==1)
		{	//The case when there is only 1 stack
			pointyVertex=Slices+1;
			for(i=0; i<Slices; i++)
			{
				if(i==0)
				{
					//Compute normal vector 1
					vector1[0]=pVertex_VNTT3[i+1].x-pVertex_VNTT3[i].x;
					vector1[1]=pVertex_VNTT3[i+1].y-pVertex_VNTT3[i].y;
					vector1[2]=pVertex_VNTT3[i+1].z-pVertex_VNTT3[i].z;
					NormalizeVectorFLOAT_2(vector1);

					vector2[0]=pVertex_VNTT3[pointyVertex].x-pVertex_VNTT3[i].x;
					vector2[1]=pVertex_VNTT3[pointyVertex].y-pVertex_VNTT3[i].y;
					vector2[2]=pVertex_VNTT3[pointyVertex].z-pVertex_VNTT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i+Slices-1].x;
					vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i+Slices-1].y;
					vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i+Slices-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3[pointyVertex].x-pVertex_VNTT3[i+Slices-1].x;
					vector5[1]=pVertex_VNTT3[pointyVertex].y-pVertex_VNTT3[i+Slices-1].y;
					vector5[2]=pVertex_VNTT3[pointyVertex].z-pVertex_VNTT3[i+Slices-1].z;
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

					vector2[0]=pVertex_VNTT3[pointyVertex].x-pVertex_VNTT3[i].x;
					vector2[1]=pVertex_VNTT3[pointyVertex].y-pVertex_VNTT3[i].y;
					vector2[2]=pVertex_VNTT3[pointyVertex].z-pVertex_VNTT3[i].z;
					NormalizeVectorFLOAT_2(vector2);

					ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

					//Compute normal vector 2
					vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i-1].x;
					vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i-1].y;
					vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i-1].z;
					NormalizeVectorFLOAT_2(vector4);

					vector5[0]=pVertex_VNTT3[pointyVertex].x-pVertex_VNTT3[i-1].x;
					vector5[1]=pVertex_VNTT3[pointyVertex].y-pVertex_VNTT3[i-1].y;
					vector5[2]=pVertex_VNTT3[pointyVertex].z-pVertex_VNTT3[i-1].z;
					NormalizeVectorFLOAT_2(vector5);

					ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

					//Now average normal from face 1 and 2
					pVertex_VNTT3[i].nx=vector3[0]+vector6[0];
					pVertex_VNTT3[i].ny=vector3[1]+vector6[1];
					pVertex_VNTT3[i].nz=vector3[2]+vector6[2];
					NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

					//pVertex_VNTT3[i].nx=-pVertex_VNTT3[i].nx;
					//pVertex_VNTT3[i].ny=-pVertex_VNTT3[i].ny;
					//pVertex_VNTT3[i].nz=-pVertex_VNTT3[i].nz;
				}
			}

			//The last vertex on the stack is the same as the first
			pVertex_VNTT3[i].nx=pVertex_VNTT3[0].nx;
			pVertex_VNTT3[i].ny=pVertex_VNTT3[0].ny;
			pVertex_VNTT3[i].nz=pVertex_VNTT3[0].nz;
			i++;
		}

		for(k=1; k<Stacks; k++)
		{
			for(j=m=0; j<=Slices; j++, i++, m++)
			{
				pVertex_VNTT3[i].nx=pVertex_VNTT3[m].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[m].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[m].nz;
			}
		}

		//The top pointy part has 1 vertex. Normal will point to {0.0, 0.0, 1.0}
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=1.0;
		i++;

		if(IsThereABottom)
		{	//All the normals point to {0.0, 0.0, -1.0}
			for(; i<VertexCount; i++)
			{
				pVertex_VNTT3[i].nx=0.0;
				pVertex_VNTT3[i].ny=0.0;
				pVertex_VNTT3[i].nz=-1.0;
			}
		}

		return 1;
	}

	return -1;
}

//PURPOSE:	Calculate the texture coordinates for the torus.
//
//PARAMETERS:
//			[in] invertS : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//			[in] invertT : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//
//RETURN:
//			1 if successful
//			-1 if TexCoordStyle is not 1 or if the VertexFormat is not supported
sint TCone::GenerateTexCoords2(tbool invertS, tbool invertT)
{
	sint i, j, k, n;
	sreal FTempo1, FTempo2;
	sreal texIncrementS, texIncrementT;		//Increment factors in texture coordinates

	if(TexCoordStyle==1)
	{
		texIncrementS=(sreal)ScaleFactorS[0]/(sreal)Slices;
		if(invertS)
			texIncrementS=-texIncrementS;		//Negate

		texIncrementT=(sreal)ScaleFactorT[0]/(sreal)Stacks;
		if(invertT)
			texIncrementT=-texIncrementT;		//Negate

		if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertS)
					FTempo1=ScaleFactorS[0];
				else
					FTempo1=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VT[k].s0=FTempo1;
					pVertex_VT[k].t0=FTempo2;

					FTempo1+=texIncrementS;
				}

				//The last vertex
				if(ScaleFactorS[0]==1.0)
				{
					if(invertS)
						pVertex_VT[k].s0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VT[k].s0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VT[k].s0=FTempo1;
				pVertex_VT[k].t0=FTempo2;

				k++;

				FTempo2+=texIncrementT;
			}

			//The top pointy part has 1 vertex
			//Let's just give it a U of 0.5
			pVertex_VT[k].s0=0.5*ScaleFactorT[0];

			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VT[k].s0=FTempo2;

			k++;

			if(IsThereABottom)
			{
				//Bottom center vertex
				pVertex_VT[k].s0=0.5;
				pVertex_VT[k].t0=0.5;
				k++;

				FTempo1=0.0;
				FTempo2=(2.0*M_PI)/(sreal)Slices;
				n=k;
				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VT[k].s0=0.5*cosf(FTempo1)+0.5;
					pVertex_VT[k].t0=0.5*cosf(FTempo1)+0.5;
					FTempo1+=FTempo2;
				}

				//The last vertex is the same as the first
				pVertex_VT[k].s0=pVertex_VT[n].s0;
				pVertex_VT[k].t0=pVertex_VT[n].t0;
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertS)
					FTempo1=ScaleFactorS[0];
				else
					FTempo1=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT[k].s0=FTempo1;
					pVertex_VNT[k].t0=FTempo2;

					FTempo1+=texIncrementS;
				}

				//The last vertex
				if(ScaleFactorS[0]==1.0)
				{
					if(invertS)
						pVertex_VNT[k].s0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNT[k].s0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNT[k].s0=FTempo1;
				pVertex_VNT[k].t0=FTempo2;

				k++;

				FTempo2+=texIncrementT;
			}

			//The top pointy part has 1 vertex
			//Let's just give it a U of 0.5
			pVertex_VNT[k].s0=0.5*ScaleFactorT[0];

			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNT[k].s0=FTempo2;

			k++;

			if(IsThereABottom)
			{
				//Bottom center vertex
				pVertex_VNT[k].s0=0.5;
				pVertex_VNT[k].t0=0.5;
				k++;

				FTempo1=0.0;
				FTempo2=(2.0*M_PI)/(sreal)Slices;
				n=k;
				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT[k].s0=0.5*cosf(FTempo1)+0.5;
					pVertex_VNT[k].t0=0.5*cosf(FTempo1)+0.5;
					FTempo1+=FTempo2;
				}

				//The last vertex is the same as the first
				pVertex_VNT[k].s0=pVertex_VNT[n].s0;
				pVertex_VNT[k].t0=pVertex_VNT[n].t0;
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertS)
					FTempo1=ScaleFactorS[0];
				else
					FTempo1=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT3[k].s0=FTempo1;
					pVertex_VNT3[k].t0=FTempo2;

					FTempo1+=texIncrementS;
				}

				//The last vertex
				if(ScaleFactorS[0]==1.0)
				{
					if(invertS)
						pVertex_VNT3[k].s0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNT3[k].s0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNT3[k].s0=FTempo1;
				pVertex_VNT3[k].t0=FTempo2;

				k++;

				FTempo2+=texIncrementT;
			}

			//The top pointy part has 1 vertex
			//Let's just give it a U of 0.5
			pVertex_VNT3[k].s0=0.5*ScaleFactorT[0];

			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNT3[k].s0=FTempo2;

			k++;

			if(IsThereABottom)
			{
				//Bottom center vertex
				pVertex_VNT3[k].s0=0.5;
				pVertex_VNT3[k].t0=0.5;
				k++;

				FTempo1=0.0;
				FTempo2=(2.0*M_PI)/(sreal)Slices;
				n=k;
				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT3[k].s0=0.5*cosf(FTempo1)+0.5;
					pVertex_VNT3[k].t0=0.5*cosf(FTempo1)+0.5;
					FTempo1+=FTempo2;
				}

				//The last vertex is the same as the first
				pVertex_VNT3[k].s0=pVertex_VNT3[n].s0;
				pVertex_VNT3[k].t0=pVertex_VNT3[n].t0;
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertS)
					FTempo1=ScaleFactorS[0];
				else
					FTempo1=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3T3[k].s0=FTempo1;
					pVertex_VNTT3T3[k].t0=FTempo2;

					FTempo1+=texIncrementS;
				}

				//The last vertex
				if(ScaleFactorS[0]==1.0)
				{
					if(invertS)
						pVertex_VNTT3T3[k].s0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNTT3T3[k].s0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNTT3T3[k].s0=FTempo1;
				pVertex_VNTT3T3[k].t0=FTempo2;

				k++;

				FTempo2+=texIncrementT;
			}

			//The top pointy part has 1 vertex
			//Let's just give it a U of 0.5
			pVertex_VNTT3T3[k].s0=0.5*ScaleFactorT[0];

			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNTT3T3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNTT3T3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNTT3T3[k].s0=FTempo2;

			k++;

			if(IsThereABottom)
			{
				//Bottom center vertex
				pVertex_VNTT3T3[k].s0=0.5;
				pVertex_VNTT3T3[k].t0=0.5;
				k++;

				FTempo1=0.0;
				FTempo2=(2.0*M_PI)/(sreal)Slices;
				n=k;
				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3T3[k].s0=0.5*cosf(FTempo1)+0.5;
					pVertex_VNTT3T3[k].t0=0.5*cosf(FTempo1)+0.5;
					FTempo1+=FTempo2;
				}

				//The last vertex is the same as the first
				pVertex_VNTT3T3[k].s0=pVertex_VNTT3T3[n].s0;
				pVertex_VNTT3T3[k].t0=pVertex_VNTT3T3[n].t0;
			}

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertS)
					FTempo1=ScaleFactorS[0];
				else
					FTempo1=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3[k].s0=FTempo1;
					pVertex_VNTT3[k].t0=FTempo2;

					FTempo1+=texIncrementS;
				}

				//The last vertex
				if(ScaleFactorS[0]==1.0)
				{
					if(invertS)
						pVertex_VNTT3[k].s0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNTT3[k].s0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNTT3[k].s0=FTempo1;
				pVertex_VNTT3[k].t0=FTempo2;

				k++;

				FTempo2+=texIncrementT;
			}

			//The top pointy part has 1 vertex
			//Let's just give it a U of 0.5
			pVertex_VNTT3[k].s0=0.5*ScaleFactorT[0];

			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNTT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNTT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNTT3[k].s0=FTempo2;

			k++;

			if(IsThereABottom)
			{
				//Bottom center vertex
				pVertex_VNTT3[k].s0=0.5;
				pVertex_VNTT3[k].t0=0.5;
				k++;

				FTempo1=0.0;
				FTempo2=(2.0*M_PI)/(sreal)Slices;
				n=k;
				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3[k].s0=0.5*cosf(FTempo1)+0.5;
					pVertex_VNTT3[k].t0=0.5*cosf(FTempo1)+0.5;
					FTempo1+=FTempo2;
				}

				//The last vertex is the same as the first
				pVertex_VNTT3[k].s0=pVertex_VNTT3[n].s0;
				pVertex_VNTT3[k].t0=pVertex_VNTT3[n].t0;
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



