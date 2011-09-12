// TTorus.cpp: implementation of the TTorus class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TTorus.h"
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

TTorus::TTorus()
{
	GeneralReset();
}

TTorus::~TTorus()
{
	DeallocateAllMemory();
}

void TTorus::GeneralReset()
{
	TBaseShape::GeneralReset();

	RadiusOfTorus=RadiusOfCircle=0.0;
	Stacks=Slices=0;
}

//PURPOSE:	Deallocate all reserved memory
void TTorus::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}

//PURPOSE:	Must call this at startup to create the torus.
//			Consider a right handed coordinate system where the x axis is along the horizontal
//			of your screen, y is up. z is pointing to you.
//			The torus will be a wheel on the xy plane.
sint TTorus::StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		sreal radiusOfTorus, sreal radiusOfCircle, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Slices=(slices<3) ? 3 : slices;
	Stacks=(stacks<3) ? 3 : stacks;

	PolygonNumber=Slices*Stacks*2;		//Triangle count

	RadiusOfTorus=(radiusOfTorus<=0.0) ? 1.0 : radiusOfTorus;
	RadiusOfCircle=(radiusOfCircle<=0.0) ? 0.1 : radiusOfCircle;

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 84");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 90");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 95");
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
//			-2 for array related array (DEBUG) remove it in the future.
sint TTorus::GenerateIndices()
{
	sint i, j, k, tempo3;
	sint vertexCount;
	tbool switchOccured=FALSE;


	if((Stacks<=0)||(Slices<=0))
		return 1;

	vertexCount=(Slices+1)*(Stacks+1);

	tempo3=Slices*Stacks*2*3;			//*3 because 3 indices per triangle

	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 368");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		AllocateExact_1(pIndex16Bit, &UsedIndex, tempo3);

		ushort index=0;
		ushort otherStack=(ushort)(Slices+1);
		k=0;
		for(i=0; i<Stacks; i++)
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
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		AllocateExact_1(pIndex32Bit, &UsedIndex, tempo3);

		uint index=0;
		uint otherStack=(uint)(Slices+1);
		k=0;
		for(i=0; i<Stacks; i++)
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
//			-1 for array related array (DEBUG) remove it in the future.
sint TTorus::GenerateVertices2()
{
	sint i, j, k, m, n, returnVal;
	sreal FTempo3, FTempo4;
	sreal centerPos;


	VertexCount=(Slices+1)*(Stacks+1);

	Start_DrawRangeElements=0;
	End_DrawRangeElements=VertexCount-1;


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		pVertex_V=new GLHVertex_V[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_V[k].x=pcircle[m  ];
				pVertex_V[k].y=0.0;
				pVertex_V[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_V[k].x);

				//Translate to the right place
				pVertex_V[k].x+=centerPos*cosf(FTempo3);
				pVertex_V[k].y+=centerPos*sinf(FTempo3);
				//pVertex_V[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_V[k].x=pVertex_V[n].x;
			pVertex_V[k].y=pVertex_V[n].y;
			pVertex_V[k].z=pVertex_V[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_V[k].x=pVertex_V[i].x;
			pVertex_V[k].y=pVertex_V[i].y;
			pVertex_V[k].z=pVertex_V[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_V[k].x=pVertex_V[n].x;
		pVertex_V[k].y=pVertex_V[n].y;
		pVertex_V[k].z=pVertex_V[n].z;

		delete [] pcircle;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		pVertex_VT=new GLHVertex_VT[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_VT[k].x=pcircle[m  ];
				pVertex_VT[k].y=0.0;
				pVertex_VT[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VT[k].x);

				//Translate to the right place
				pVertex_VT[k].x+=centerPos*cosf(FTempo3);
				pVertex_VT[k].y+=centerPos*sinf(FTempo3);
				//pVertex_VT[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_VT[k].x=pVertex_VT[n].x;
			pVertex_VT[k].y=pVertex_VT[n].y;
			pVertex_VT[k].z=pVertex_VT[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_VT[k].x=pVertex_VT[i].x;
			pVertex_VT[k].y=pVertex_VT[i].y;
			pVertex_VT[k].z=pVertex_VT[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_VT[k].x=pVertex_VT[n].x;
		pVertex_VT[k].y=pVertex_VT[n].y;
		pVertex_VT[k].z=pVertex_VT[n].z;

		delete [] pcircle;

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		pVertex_VNT=new GLHVertex_VNT[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_VNT[k].x=pcircle[m  ];
				pVertex_VNT[k].y=0.0;
				pVertex_VNT[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNT[k].x);

				//Translate to the right place
				pVertex_VNT[k].x+=centerPos*cosf(FTempo3);
				pVertex_VNT[k].y+=centerPos*sinf(FTempo3);
				//pVertex_VNT[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_VNT[k].x=pVertex_VNT[n].x;
			pVertex_VNT[k].y=pVertex_VNT[n].y;
			pVertex_VNT[k].z=pVertex_VNT[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_VNT[k].x=pVertex_VNT[i].x;
			pVertex_VNT[k].y=pVertex_VNT[i].y;
			pVertex_VNT[k].z=pVertex_VNT[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_VNT[k].x=pVertex_VNT[n].x;
		pVertex_VNT[k].y=pVertex_VNT[n].y;
		pVertex_VNT[k].z=pVertex_VNT[n].z;

		delete [] pcircle;

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_VNT3[k].x=pcircle[m  ];
				pVertex_VNT3[k].y=0.0;
				pVertex_VNT3[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNT3[k].x);

				//Translate to the right place
				pVertex_VNT3[k].x+=centerPos*cosf(FTempo3);
				pVertex_VNT3[k].y+=centerPos*sinf(FTempo3);
				//pVertex_VNT3[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_VNT3[k].x=pVertex_VNT3[n].x;
			pVertex_VNT3[k].y=pVertex_VNT3[n].y;
			pVertex_VNT3[k].z=pVertex_VNT3[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_VNT3[k].x=pVertex_VNT3[i].x;
			pVertex_VNT3[k].y=pVertex_VNT3[i].y;
			pVertex_VNT3[k].z=pVertex_VNT3[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_VNT3[k].x=pVertex_VNT3[n].x;
		pVertex_VNT3[k].y=pVertex_VNT3[n].y;
		pVertex_VNT3[k].z=pVertex_VNT3[n].z;

		delete [] pcircle;

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_VNTT3T3[k].x=pcircle[m  ];
				pVertex_VNTT3T3[k].y=0.0;
				pVertex_VNTT3T3[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNTT3T3[k].x);

				//Translate to the right place
				pVertex_VNTT3T3[k].x+=centerPos*cosf(FTempo3);
				pVertex_VNTT3T3[k].y+=centerPos*sinf(FTempo3);
				//pVertex_VNTT3T3[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_VNTT3T3[k].x=pVertex_VNTT3T3[n].x;
			pVertex_VNTT3T3[k].y=pVertex_VNTT3T3[n].y;
			pVertex_VNTT3T3[k].z=pVertex_VNTT3T3[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_VNTT3T3[k].x=pVertex_VNTT3T3[i].x;
			pVertex_VNTT3T3[k].y=pVertex_VNTT3T3[i].y;
			pVertex_VNTT3T3[k].z=pVertex_VNTT3T3[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_VNTT3T3[k].x=pVertex_VNTT3T3[n].x;
		pVertex_VNTT3T3[k].y=pVertex_VNTT3T3[n].y;
		pVertex_VNTT3T3[k].z=pVertex_VNTT3T3[n].z;

		delete [] pcircle;

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(FALSE, FALSE);

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 589");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 598");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

		//1. Compute a circle with radius = RadiusOfCircle
		//2. The circle should be on the yz plane
		//3. Rotate and position the circle and write to the vertex array

		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		centerPos=RadiusOfTorus-RadiusOfCircle;
		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			m=0;
			n=k;		//The first vertex
			for(j=0; j<Slices; j++, k++, m+=3)
			{
				pVertex_VNTT3[k].x=pcircle[m  ];
				pVertex_VNTT3[k].y=0.0;
				pVertex_VNTT3[k].z=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNTT3[k].x);

				//Translate to the right place
				pVertex_VNTT3[k].x+=centerPos*cosf(FTempo3);
				pVertex_VNTT3[k].y+=centerPos*sinf(FTempo3);
				//pVertex_VNTT3[k].z=;
			}

			//The last slice vertex is the same as the first
			pVertex_VNTT3[k].x=pVertex_VNTT3[n].x;
			pVertex_VNTT3[k].y=pVertex_VNTT3[n].y;
			pVertex_VNTT3[k].z=pVertex_VNTT3[n].z;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;		//The first vertex
		i=0;
		for(j=0; j<Slices; j++, k++, i++)
		{
			pVertex_VNTT3[k].x=pVertex_VNTT3[i].x;
			pVertex_VNTT3[k].y=pVertex_VNTT3[i].y;
			pVertex_VNTT3[k].z=pVertex_VNTT3[i].z;
		}

		//The last slice vertex is the same as the first
		pVertex_VNTT3[k].x=pVertex_VNTT3[n].x;
		pVertex_VNTT3[k].y=pVertex_VNTT3[n].y;
		pVertex_VNTT3[k].z=pVertex_VNTT3[n].z;

		delete [] pcircle;

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
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 589");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 598");
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

//PURPOSE:	Compute texture coordinates for every vertex. Will only generate the default case
//			which is the texture wraps around the tube's body.
//			For the top and bottom, the center vertex gets {0.5, 0.5} texcoords and the
//			other vertices texcoords are generated by a circle.
//
//PARAMETERS:
//			[in] invertS : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//			[in] invertT : If FALSE, 0.0 to 1.0, else 1.0 to 0.0
//
//RETURN:
//			1 if successful
//			-1 if TexCoordStyle is not 1 or if the VertexFormat is not supported
sint TTorus::GenerateTexCoords2(tbool invertS, tbool invertT)
{
	sint i, j, k;
	sreal FTempo1, FTempo2;
	sreal texIncrementS, texIncrementT;		//Increment factors in texture coordinates

	if(TexCoordStyle==1)
	{
		//////////////////////////////////////////////////
		texIncrementS=(sreal)ScaleFactorS[0]/(sreal)Stacks;
		if(invertS)
			texIncrementS=-texIncrementS;		//Negate

		texIncrementT=(sreal)ScaleFactorT[0]/(sreal)Slices;
		if(invertT)
			texIncrementT=-texIncrementT;		//Negate

		if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			if(invertS)
				FTempo1=ScaleFactorS[0];
			else
				FTempo1=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertT)
					FTempo2=ScaleFactorT[0];
				else
					FTempo2=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VT[k].s0=FTempo1;
					pVertex_VT[k].t0=FTempo2;

					FTempo2+=texIncrementT;
				}

				//For the last vertex on the slice
				pVertex_VT[k].s0=FTempo1;
				if(ScaleFactorT[0]==1.0)
				{
					if(invertT)
						pVertex_VT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VT[k].t0=FTempo2;

				k++;

				FTempo1+=texIncrementS;
			}

			//For the last stack
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			if(ScaleFactorS[0]==1.0)
			{
				if(invertS)
					FTempo1=0.0;						//Make it exactly 0.0. Avoid floating point errors.
				else
					FTempo1=1.0;						//Make it exactly 0.0. Avoid floating point errors.
			}
			//else, FTempo1 is already ok

			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VT[k].s0=FTempo1;
				pVertex_VT[k].t0=FTempo2;

				FTempo2+=texIncrementT;
			}

			//For the last vertex on the slice
			pVertex_VT[k].s0=FTempo1;
			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VT[k].t0=FTempo2;

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			if(invertS)
				FTempo1=ScaleFactorS[0];
			else
				FTempo1=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertT)
					FTempo2=ScaleFactorT[0];
				else
					FTempo2=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT[k].s0=FTempo1;
					pVertex_VNT[k].t0=FTempo2;

					FTempo2+=texIncrementT;
				}

				//For the last vertex on the slice
				pVertex_VNT[k].s0=FTempo1;
				if(ScaleFactorT[0]==1.0)
				{
					if(invertT)
						pVertex_VNT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNT[k].t0=FTempo2;

				k++;

				FTempo1+=texIncrementS;
			}

			//For the last stack
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			if(ScaleFactorS[0]==1.0)
			{
				if(invertS)
					FTempo1=0.0;						//Make it exactly 0.0. Avoid floating point errors.
				else
					FTempo1=1.0;						//Make it exactly 0.0. Avoid floating point errors.
			}
			//else, FTempo1 is already ok

			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT[k].s0=FTempo1;
				pVertex_VNT[k].t0=FTempo2;

				FTempo2+=texIncrementT;
			}

			//For the last vertex on the slice
			pVertex_VNT[k].s0=FTempo1;
			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNT[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNT[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNT[k].t0=FTempo2;

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			if(invertS)
				FTempo1=ScaleFactorS[0];
			else
				FTempo1=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertT)
					FTempo2=ScaleFactorT[0];
				else
					FTempo2=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNT3[k].s0=FTempo1;
					pVertex_VNT3[k].t0=FTempo2;
					pVertex_VNT3[k].r0=0.0;			//Just set to 0.0

					FTempo2+=texIncrementT;
				}

				//For the last vertex on the slice
				pVertex_VNT3[k].s0=FTempo1;
				if(ScaleFactorT[0]==1.0)
				{
					if(invertT)
						pVertex_VNT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNT3[k].t0=FTempo2;

				pVertex_VNT3[k].r0=0.0;			//Just set to 0.0

				k++;

				FTempo1+=texIncrementS;
			}

			//For the last stack
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			if(ScaleFactorS[0]==1.0)
			{
				if(invertS)
					FTempo1=0.0;						//Make it exactly 0.0. Avoid floating point errors.
				else
					FTempo1=1.0;						//Make it exactly 0.0. Avoid floating point errors.
			}
			//else, FTempo1 is already ok

			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNT3[k].s0=FTempo1;
				pVertex_VNT3[k].t0=FTempo2;
				pVertex_VNT3[k].r0=0.0;			//Just set to 0.0

				FTempo2+=texIncrementT;
			}

			//For the last vertex on the slice
			pVertex_VNT3[k].s0=FTempo1;
			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNT3[k].t0=FTempo2;

			pVertex_VNT3[k].r0=0.0;			//Just set to 0.0

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			if(invertS)
				FTempo1=ScaleFactorS[0];
			else
				FTempo1=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertT)
					FTempo2=ScaleFactorT[0];
				else
					FTempo2=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3T3[k].s0=FTempo1;
					pVertex_VNTT3T3[k].t0=FTempo2;

					FTempo2+=texIncrementT;
				}

				//For the last vertex on the slice
				pVertex_VNTT3T3[k].s0=FTempo1;
				if(ScaleFactorT[0]==1.0)
				{
					if(invertT)
						pVertex_VNTT3T3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNTT3T3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNTT3T3[k].t0=FTempo2;

				k++;

				FTempo1+=texIncrementS;
			}

			//For the last stack
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			if(ScaleFactorS[0]==1.0)
			{
				if(invertS)
					FTempo1=0.0;						//Make it exactly 0.0. Avoid floating point errors.
				else
					FTempo1=1.0;						//Make it exactly 0.0. Avoid floating point errors.
			}
			//else, FTempo1 is already ok

			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3T3[k].s0=FTempo1;
				pVertex_VNTT3T3[k].t0=FTempo2;

				FTempo2+=texIncrementT;
			}

			//For the last vertex on the slice
			pVertex_VNTT3T3[k].s0=FTempo1;
			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNTT3T3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNTT3T3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNTT3T3[k].t0=FTempo2;

			return 1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			if(invertS)
				FTempo1=ScaleFactorS[0];
			else
				FTempo1=0.0;

			k=0;
			for(i=0; i<Stacks; i++)
			{
				if(invertT)
					FTempo2=ScaleFactorT[0];
				else
					FTempo2=0.0;

				for(j=0; j<Slices; j++, k++)
				{
					pVertex_VNTT3[k].s0=FTempo1;
					pVertex_VNTT3[k].t0=FTempo2;

					FTempo2+=texIncrementT;
				}

				//For the last vertex on the slice
				pVertex_VNTT3[k].s0=FTempo1;
				if(ScaleFactorT[0]==1.0)
				{
					if(invertT)
						pVertex_VNTT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
					else
						pVertex_VNTT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
				}
				else
					pVertex_VNTT3[k].t0=FTempo2;

				k++;

				FTempo1+=texIncrementS;
			}

			//For the last stack
			if(invertT)
				FTempo2=ScaleFactorT[0];
			else
				FTempo2=0.0;

			if(ScaleFactorS[0]==1.0)
			{
				if(invertS)
					FTempo1=0.0;						//Make it exactly 0.0. Avoid floating point errors.
				else
					FTempo1=1.0;						//Make it exactly 0.0. Avoid floating point errors.
			}
			//else, FTempo1 is already ok

			for(j=0; j<Slices; j++, k++)
			{
				pVertex_VNTT3[k].s0=FTempo1;
				pVertex_VNTT3[k].t0=FTempo2;

				FTempo2+=texIncrementT;
			}

			//For the last vertex on the slice
			pVertex_VNTT3[k].s0=FTempo1;
			if(ScaleFactorT[0]==1.0)
			{
				if(invertT)
					pVertex_VNTT3[k].t0=0.0;		//Make it exactly 0.0. Avoid floating point errors.
				else
					pVertex_VNTT3[k].t0=1.0;		//Make it exactly 1.0. Avoid floating point errors.
			}
			else
				pVertex_VNTT3[k].t0=FTempo2;

			return 1;
		}
	}

	return -1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TTorus::GenerateNormals2()
{
	sint i, j, k, m, n;
	sreal FTempo3, FTempo4;

	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=m=0; j<Slices; j++, m+=3, k++)
			{
				pVertex_VNT[k].nx=pcircle[m  ];
				pVertex_VNT[k].ny=pcircle[m+1];
				pVertex_VNT[k].nz=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNT[k].nx);

				NormalizeVectorFLOAT_2(&pVertex_VNT[k].nx);
			}

			//The last slice vertex is the same as the first
			pVertex_VNT[k].nx=pVertex_VNT[n].nx;
			pVertex_VNT[k].ny=pVertex_VNT[n].ny;
			pVertex_VNT[k].nz=pVertex_VNT[n].nz;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;
		for(j=0; j<Slices; j++, k++)
		{
			pVertex_VNT[k].nx=pVertex_VNT[j].nx;
			pVertex_VNT[k].ny=pVertex_VNT[j].ny;
			pVertex_VNT[k].nz=pVertex_VNT[j].nz;
		}

		//The last slice vertex is the same as the first
		pVertex_VNT[k].nx=pVertex_VNT[n].nx;
		pVertex_VNT[k].ny=pVertex_VNT[n].ny;
		pVertex_VNT[k].nz=pVertex_VNT[n].nz;

		delete [] pcircle;

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=m=0; j<Slices; j++, m+=3, k++)
			{
				pVertex_VNT3[k].nx=pcircle[m  ];
				pVertex_VNT3[k].ny=pcircle[m+1];
				pVertex_VNT3[k].nz=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNT3[k].nx);

				NormalizeVectorFLOAT_2(&pVertex_VNT3[k].nx);
			}

			//The last slice vertex is the same as the first
			pVertex_VNT3[k].nx=pVertex_VNT3[n].nx;
			pVertex_VNT3[k].ny=pVertex_VNT3[n].ny;
			pVertex_VNT3[k].nz=pVertex_VNT3[n].nz;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;
		for(j=0; j<Slices; j++, k++)
		{
			pVertex_VNT3[k].nx=pVertex_VNT3[j].nx;
			pVertex_VNT3[k].ny=pVertex_VNT3[j].ny;
			pVertex_VNT3[k].nz=pVertex_VNT3[j].nz;
		}

		//The last slice vertex is the same as the first
		pVertex_VNT3[k].nx=pVertex_VNT3[n].nx;
		pVertex_VNT3[k].ny=pVertex_VNT3[n].ny;
		pVertex_VNT3[k].nz=pVertex_VNT3[n].nz;

		delete [] pcircle;

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=m=0; j<Slices; j++, m+=3, k++)
			{
				pVertex_VNTT3T3[k].nx=pcircle[m  ];
				pVertex_VNTT3T3[k].ny=pcircle[m+1];
				pVertex_VNTT3T3[k].nz=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNTT3T3[k].nx);

				NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[k].nx);
			}

			//The last slice vertex is the same as the first
			pVertex_VNTT3T3[k].nx=pVertex_VNTT3T3[n].nx;
			pVertex_VNTT3T3[k].ny=pVertex_VNTT3T3[n].ny;
			pVertex_VNTT3T3[k].nz=pVertex_VNTT3T3[n].nz;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;
		for(j=0; j<Slices; j++, k++)
		{
			pVertex_VNTT3T3[k].nx=pVertex_VNTT3T3[j].nx;
			pVertex_VNTT3T3[k].ny=pVertex_VNTT3T3[j].ny;
			pVertex_VNTT3T3[k].nz=pVertex_VNTT3T3[j].nz;
		}

		//The last slice vertex is the same as the first
		pVertex_VNTT3T3[k].nx=pVertex_VNTT3T3[n].nx;
		pVertex_VNTT3T3[k].ny=pVertex_VNTT3T3[n].ny;
		pVertex_VNTT3T3[k].nz=pVertex_VNTT3T3[n].nz;

		delete [] pcircle;

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		sreal *pcircle=new sreal[(Slices+1)*3];			//*3 because XYZ

		FTempo3=M_PI/2.0;
		FTempo4=(M_PI*2.0)/(sreal)Slices;				//Increment

		for(i=j=0; i<Slices; i++, j+=3)
		{
			pcircle[j  ]=RadiusOfCircle*cosf(FTempo3);
			pcircle[j+1]=0.0;
			pcircle[j+2]=RadiusOfCircle*sinf(FTempo3);
			FTempo3-=FTempo4;
		}

		//The last vertex is the same as the first vertex
		pcircle[j  ]=pcircle[0];
		pcircle[j+1]=pcircle[1];
		pcircle[j+2]=pcircle[2];

		FTempo3=0.0;
		FTempo4=(M_PI*2.0)/(sreal)Stacks;				//Increment

		k=0;
		for(i=0; i<Stacks; i++)
		{
			n=k;
			for(j=m=0; j<Slices; j++, m+=3, k++)
			{
				pVertex_VNTT3[k].nx=pcircle[m  ];
				pVertex_VNTT3[k].ny=pcircle[m+1];
				pVertex_VNTT3[k].nz=pcircle[m+2];

				if(FTempo3!=0.0)
					RotatePoint3DAboutZAxisFLOAT_1(FTempo3, &pVertex_VNTT3[k].nx);

				NormalizeVectorFLOAT_2(&pVertex_VNTT3[k].nx);
			}

			//The last slice vertex is the same as the first
			pVertex_VNTT3[k].nx=pVertex_VNTT3[n].nx;
			pVertex_VNTT3[k].ny=pVertex_VNTT3[n].ny;
			pVertex_VNTT3[k].nz=pVertex_VNTT3[n].nz;
			k++;

			FTempo3+=FTempo4;
		}

		//The last stack vertices are the same as the first
		n=k;
		for(j=0; j<Slices; j++, k++)
		{
			pVertex_VNTT3[k].nx=pVertex_VNTT3[j].nx;
			pVertex_VNTT3[k].ny=pVertex_VNTT3[j].ny;
			pVertex_VNTT3[k].nz=pVertex_VNTT3[j].nz;
		}

		//The last slice vertex is the same as the first
		pVertex_VNTT3[k].nx=pVertex_VNTT3[n].nx;
		pVertex_VNTT3[k].ny=pVertex_VNTT3[n].ny;
		pVertex_VNTT3[k].nz=pVertex_VNTT3[n].nz;

		delete [] pcircle;

		return 1;
	}

	return -1;
}

//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//



