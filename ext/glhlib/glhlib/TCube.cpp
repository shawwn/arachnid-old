// TCube.cpp: implementation of the TCube class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCube.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"


#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCube::TCube()
{
	GeneralReset();
}

TCube::~TCube()
{
	DeallocateAllMemory();
}

void TCube::GeneralReset()
{
	TBaseShape::GeneralReset();

	Width=Height=Length=0.0;
	WidthDivision=HeightDivision=LengthDivision=0;
	IsAveragedOverFacesOn=FALSE;
	TexCoordStyle=0;
}

void TCube::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();
	
	GeneralReset();
}

//PURPOSE:	Be sure to call Startup() before using the main functions of this class.
//			If center3D==NULL, {0, 0, 0} is used.
//			If orientation3D==NULL, {0, 1, 0} is used.
//
//PARAMETERS:
//			[in] width : >0.0 else it will be set to 1.0
//			[in] height : >0.0 else it will be set to 1.0
//			[in] length : >0.0 else it will be set to 1.0
//			[in] widthDivision : >1 else it will be set to 1
//			[in] heightDivision : >1 else it will be set to 1
//			[in] lengthDivision : >1 else it will be set to 1
//
//RETURN:
//			1 for success
//			-1 for error message
sint TCube::Startup(sreal width, sreal height, sreal length, sint widthDivision, sint heightDivision, sint lengthDivision)
{
	Width=(width<=0.0) ? 1.0 : width;
	Height=(height<=0.0) ? 1.0 : height;
	Length=(length<=0.0) ? 1.0 : length;
	WidthDivision=(widthDivision<=0) ? 1 : widthDivision;
	HeightDivision=(heightDivision<=0) ? 1 : heightDivision;
	LengthDivision=(lengthDivision<=0) ? 1 : lengthDivision;

	return SameStartUpPatternForCube();
}

//PURPOSE:	Be sure to call Startup() before using the main functions of this class.
//			If center3D==NULL, {0, 0, 0} is used.
//			If orientation3D==NULL, {0, 1, 0} is used.
//			**** Generate interlaced format ****
//
//PARAMETERS:
//			[in] indexFormat :  one of the defines in glhlib.h
//			[in] vertexFormat : one of the defines in glhlib.h
//			                    GLH_VERTEXFORMAT_VNTT3T3, T3 receives tangent and the other T3 receives binormal
//			[in] width : >0.0 else it will be set to 1.0
//			[in] height : >0.0 else it will be set to 1.0
//			[in] length : >0.0 else it will be set to 1.0
//			[in] widthDivision : >1 else it will be set to 1
//			[in] heightDivision : >1 else it will be set to 1
//			[in] lengthDivision : >1 else it will be set to 1
//			[in] polygonNumber : ignored for now. Set to 12
//			[in] texCoordStyle : 0 for nothing to be written
//			                     1 for standard texture mapping
//
//RETURN:
//			1 for success
//			-1 for error message
//			for tangent and binormal generation
sint TCube::Startup2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, sreal width, sreal height, sreal length,
					sint widthDivision, sint heightDivision, sint lengthDivision, tbool isAveragedOverFacesOn, sint texCoordStyle)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Width=(width<=0.0) ? 1.0 : width;
	Height=(height<=0.0) ? 1.0 : height;
	Length=(length<=0.0) ? 1.0 : length;
	WidthDivision=(widthDivision<=0) ? 1 : widthDivision;
	HeightDivision=(heightDivision<=0) ? 1 : heightDivision;
	LengthDivision=(lengthDivision<=0) ? 1 : lengthDivision;

	IsAveragedOverFacesOn=isAveragedOverFacesOn;
	TexCoordStyle=texCoordStyle;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 115");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 121");
		return -1;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 126");
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

	GenerateIndices();

	return SameStartUpPatternForCube_Interleaved();
}

//PURPOSE:	This function is used internally by this class, to make the Startup functions look cleaner,
//			since some of the code is inevitably identicle.
sint TCube::SameStartUpPatternForCube()
{
	return GenerateVertices();
}

//PURPOSE:	This function is used internally by this class, to make the Startup functions look cleaner,
//			since some of the code is inevitably identicle.
//
//RETURN:
//			1 for success
//			-1 for error message
sint TCube::SameStartUpPatternForCube_Interleaved()
{
	return GenerateVertices_Interleaved();
}

//PURPOSE:	Calculates the verticies for the cube.
//
//RETURN:
//			Always 1
//WARNING:	Only works for 6 sided cube, 6 polygons, NO orientation
sint TCube::GenerateVertices()
{
	sint i, j, k;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		if(pVertex==NULL)
		{
			AllocateExact_1(pVertex, &UsedVertex, 4*3*6);
			UsedVertex=4*3*6;
		}

		i=0;
		//Front face
		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];
		//Left face
		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];
		//Back face
		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];
		//Right face
		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];
		//Bottom
		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];
		
		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=-halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];
		//Top
		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=halfLength+CenterPosition[2];

		pVertex[i++]=-halfWidth+CenterPosition[0];
		pVertex[i++]=halfHeight+CenterPosition[1];
		pVertex[i++]=-halfLength+CenterPosition[2];
	}
	else //if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))==FALSE
	{
		sint vertexCount=(((WidthDivision+1)*(HeightDivision+1))+((WidthDivision+1)*(LengthDivision+1))+
			((HeightDivision+1)*(LengthDivision+1)))*2;
		sreal positionX, positionY, positionZ;
		sreal positionX_Inc, positionY_Inc, positionZ_Inc;
		positionX_Inc=Width/(sreal)WidthDivision;
		positionY_Inc=Height/(sreal)HeightDivision;
		positionZ_Inc=Length/(sreal)LengthDivision;

		if(pVertex==NULL)
		{
			AllocateExact_1(pVertex, &UsedVertex, vertexCount*3);
			UsedVertex=vertexCount*3;
		}

		k=0;
		//Front face
		positionY=-halfHeight+CenterPosition[1];
		positionZ=halfLength+CenterPosition[2];
		for(j=0; j<=HeightDivision; j++)
		{
			positionX=-halfWidth+CenterPosition[0];

			for(i=0; i<=WidthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionX+=positionX_Inc;
			}

			positionY+=positionY_Inc;
		}

		//Back face
		positionY=-halfHeight+CenterPosition[1];
		positionZ=-halfLength+CenterPosition[2];
		for(j=0; j<=HeightDivision; j++)
		{
			positionX=halfWidth+CenterPosition[0];

			for(i=0; i<=WidthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionX-=positionX_Inc;
			}

			positionY+=positionY_Inc;
		}

		//Left face
		positionX=-halfWidth+CenterPosition[0];
		positionY=-halfHeight+CenterPosition[1];
		for(j=0; j<=HeightDivision; j++)
		{
			positionZ=-halfLength+CenterPosition[2];

			for(i=0; i<=LengthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionZ+=positionZ_Inc;
			}

			positionY+=positionY_Inc;
		}

		//Right face
		positionX=halfWidth+CenterPosition[0];
		positionY=-halfHeight+CenterPosition[1];
		for(j=0; j<=HeightDivision; j++)
		{
			positionZ=halfLength+CenterPosition[2];

			for(i=0; i<=LengthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionZ-=positionZ_Inc;
			}

			positionY+=positionY_Inc;
		}

		//Top face
		positionY=halfHeight+CenterPosition[1];
		positionZ=halfLength+CenterPosition[2];
		for(j=0; j<=LengthDivision; j++)
		{
			positionX=-halfLength+CenterPosition[0];

			for(i=0; i<=WidthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionX+=positionX_Inc;
			}

			positionZ-=positionZ_Inc;
		}

		//Bottom face
		positionY=-halfHeight+CenterPosition[1];
		positionZ=-halfLength+CenterPosition[2];
		for(j=0; j<=LengthDivision; j++)
		{
			positionX=-halfLength+CenterPosition[0];

			for(i=0; i<=WidthDivision; i++, k+=3)
			{
				pVertex[k  ]=positionX;
				pVertex[k+1]=positionY;
				pVertex[k+2]=positionZ;

				positionX+=positionX_Inc;
			}

			positionZ+=positionZ_Inc;
		}
	}

	return 1;
}

//PURPOSE:	Calculates the verticies for the cube.
//
//RETURN:
//			1 on success
//			-1 for error message (could not generate tangents binormals)
//
//WARNING:	Only works for 6 sided cube, 6 polygons, NO orientation
sint TCube::GenerateVertices_Interleaved()
{
	sint i, returnVal;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		VertexCount=4*6;
		if(VertexFormat==GLH_VERTEXFORMAT_V)
		{
			pVertex_V=new GLHVertex_V[VertexCount];
			memset(pVertex_V, 0, sizeof(GLH_VERTEXFORMAT_V)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			pVertex_VT=new GLHVertex_VT[VertexCount];
			memset(pVertex_VT, 0, sizeof(GLH_VERTEXFORMAT_VT)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			pVertex_VNT=new GLHVertex_VNT[VertexCount];
			memset(pVertex_VNT, 0, sizeof(GLH_VERTEXFORMAT_VNT)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];
			memset(pVertex_VNT3, 0, sizeof(GLH_VERTEXFORMAT_VNT3)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];
			memset(pVertex_VNTT3T3, 0, sizeof(GLH_VERTEXFORMAT_VNTT3T3)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];
			memset(pVertex_VNTT3, 0, sizeof(GLH_VERTEXFORMAT_VNTT3)*VertexCount);
		}

		Start_DrawRangeElements=0;
		End_DrawRangeElements=VertexCount-1;

		if(VertexFormat==GLH_VERTEXFORMAT_V)
		{
			i=0;
			//Front face
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			//Left face
			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			//Back face
			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			//Right face
			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			//Bottom
			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=-halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			//Top
			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=halfLength+CenterPosition[2];

			i++;
			pVertex_V[i].x=-halfWidth+CenterPosition[0];
			pVertex_V[i].y=halfHeight+CenterPosition[1];
			pVertex_V[i].z=-halfLength+CenterPosition[2];
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			i=0;
			//Front face
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}

			//Left face
			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}

			//Back face
			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}

			//Right face
			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}

			//Bottom
			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}

			//Top
			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=1.0;
				pVertex_VT[i].t0=1.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}

			i++;
			pVertex_VT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VT[i].y=halfHeight+CenterPosition[1];
			pVertex_VT[i].z=-halfLength+CenterPosition[2];
			if(TexCoordStyle==0)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VT[i].s0=0.0;
				pVertex_VT[i].t0=1.0;
			}
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			i=0;
			//Front face
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}

			//Left face
			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=-1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=-1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=-1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=-1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}

			//Back face
			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}

			//Right face
			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=1.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}

			//Bottom
			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=-1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=-1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=-1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=-1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}

			//Top
			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=1.0;
				pVertex_VNT[i].t0=1.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}

			i++;
			pVertex_VNT[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=1.0;
			pVertex_VNT[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT[i].s0=0.0;
				pVertex_VNT[i].t0=1.0;
			}
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			i=0;
			//Front face
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Left face
			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=-1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=-1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=-1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=-1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Back face
			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Right face
			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=1.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Bottom
			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=-1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=-1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=-1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=-halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=-1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Top
			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=1.0;
				pVertex_VNT3[i].t0=1.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}

			i++;
			pVertex_VNT3[i].x=-halfWidth+CenterPosition[0];
			pVertex_VNT3[i].y=halfHeight+CenterPosition[1];
			pVertex_VNT3[i].z=-halfLength+CenterPosition[2];
			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=1.0;
			pVertex_VNT3[i].nz=0.0;
			if(TexCoordStyle==0)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=0.0;
			}
			else if(TexCoordStyle==1)
			{
				pVertex_VNT3[i].s0=0.0;
				pVertex_VNT3[i].t0=1.0;
			}

			//Put zeroes for the r component of the texcoord
			for(i=0; i<VertexCount; i++)
			{
				pVertex_VNT3[i].r0=0.0;
			}
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			returnVal=GenerateVertices_Interleaved_Part10();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			returnVal=GenerateVertices_Interleaved_Part10b();
			if(returnVal==-1)
				return -1;
		}
	}
	else //if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))==FALSE
	{
		VertexCount=(((WidthDivision+1)*(HeightDivision+1))+((WidthDivision+1)*(LengthDivision+1))+
			((HeightDivision+1)*(LengthDivision+1)))*2;

		if(VertexFormat==GLH_VERTEXFORMAT_V)
		{
			pVertex_V=new GLHVertex_V[VertexCount];
			memset(pVertex_V, 0, sizeof(GLH_VERTEXFORMAT_V)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			pVertex_VT=new GLHVertex_VT[VertexCount];
			memset(pVertex_VT, 0, sizeof(GLH_VERTEXFORMAT_VT)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			pVertex_VNT=new GLHVertex_VNT[VertexCount];
			memset(pVertex_VNT, 0, sizeof(GLH_VERTEXFORMAT_VNT)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];
			memset(pVertex_VNT3, 0, sizeof(GLH_VERTEXFORMAT_VNT3)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];
			memset(pVertex_VNTT3T3, 0, sizeof(GLH_VERTEXFORMAT_VNTT3T3)*VertexCount);
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];
			memset(pVertex_VNTT3, 0, sizeof(GLH_VERTEXFORMAT_VNTT3)*VertexCount);
		}

		Start_DrawRangeElements=0;
		End_DrawRangeElements=VertexCount-1;

		if(VertexFormat==GLH_VERTEXFORMAT_V)
		{
			returnVal=GenerateVertices_Interleaved_Part11();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VT)
		{
			returnVal=GenerateVertices_Interleaved_Part12();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
		{
			returnVal=GenerateVertices_Interleaved_Part13();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
		{
			returnVal=GenerateVertices_Interleaved_Part14();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
		{	//TexCoordStyle==1 when we reach here
			returnVal=GenerateVertices_Interleaved_Part15();
			if(returnVal==-1)
				return -1;
		}
		else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
		{	//TexCoordStyle==1 when we reach here
			returnVal=GenerateVertices_Interleaved_Part15b();
			if(returnVal==-1)
				return -1;
		}
	}


	return 1;
}


//PURPOSE:
//
//RETURN:
//			Always 1
sint TCube::GenerateIndices()
{
	sint i, j;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			AllocateExact_1(pIndex16Bit, &UsedIndex, 36);

			pIndex16Bit[0]=0;
			pIndex16Bit[1]=1;
			pIndex16Bit[2]=2;
			pIndex16Bit[3]=2;
			pIndex16Bit[4]=1;
			pIndex16Bit[5]=3;

			pIndex16Bit[6]=4;
			pIndex16Bit[7]=5;
			pIndex16Bit[8]=6;
			pIndex16Bit[9]=6;
			pIndex16Bit[10]=5;
			pIndex16Bit[11]=7;

			pIndex16Bit[12]=8;
			pIndex16Bit[13]=9;
			pIndex16Bit[14]=10;
			pIndex16Bit[15]=10;
			pIndex16Bit[16]=9;
			pIndex16Bit[17]=11;

			pIndex16Bit[18]=12;
			pIndex16Bit[19]=13;
			pIndex16Bit[20]=14;
			pIndex16Bit[21]=14;
			pIndex16Bit[22]=13;
			pIndex16Bit[23]=15;

			pIndex16Bit[24]=16;
			pIndex16Bit[25]=17;
			pIndex16Bit[26]=18;
			pIndex16Bit[27]=18;
			pIndex16Bit[28]=17;
			pIndex16Bit[29]=19;
			
			pIndex16Bit[30]=20;
			pIndex16Bit[31]=21;
			pIndex16Bit[32]=22;
			pIndex16Bit[33]=22;
			pIndex16Bit[34]=21;
			pIndex16Bit[35]=23;
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			AllocateExact_1(pIndex32Bit, &UsedIndex, 36);

			pIndex32Bit[0]=0;
			pIndex32Bit[1]=1;
			pIndex32Bit[2]=2;
			pIndex32Bit[3]=2;
			pIndex32Bit[4]=1;
			pIndex32Bit[5]=3;

			pIndex32Bit[6]=4;
			pIndex32Bit[7]=5;
			pIndex32Bit[8]=6;
			pIndex32Bit[9]=6;
			pIndex32Bit[10]=5;
			pIndex32Bit[11]=7;

			pIndex32Bit[12]=8;
			pIndex32Bit[13]=9;
			pIndex32Bit[14]=10;
			pIndex32Bit[15]=10;
			pIndex32Bit[16]=9;
			pIndex32Bit[17]=11;

			pIndex32Bit[18]=12;
			pIndex32Bit[19]=13;
			pIndex32Bit[20]=14;
			pIndex32Bit[21]=14;
			pIndex32Bit[22]=13;
			pIndex32Bit[23]=15;

			pIndex32Bit[24]=16;
			pIndex32Bit[25]=17;
			pIndex32Bit[26]=18;
			pIndex32Bit[27]=18;
			pIndex32Bit[28]=17;
			pIndex32Bit[29]=19;
			
			pIndex32Bit[30]=20;
			pIndex32Bit[31]=21;
			pIndex32Bit[32]=22;
			pIndex32Bit[33]=22;
			pIndex32Bit[34]=21;
			pIndex32Bit[35]=23;
		}
	}
	else //if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))==FALSE
	{
		sint numberOfTriangles=((WidthDivision*HeightDivision)+(WidthDivision*LengthDivision)+
			(HeightDivision*LengthDivision))*4;
		sint totalIndex=numberOfTriangles*3;		//3 index per triangle

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			sint k;
			ushort startIndex=0;
			ushort nextLevel=(ushort)(WidthDivision+2);
			ushort nextLevelLess=(ushort)(WidthDivision+1);

			AllocateExact_1(pIndex16Bit, &UsedIndex, totalIndex);

			k=0;
			//Front face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(ushort)(WidthDivision+1);
			nextLevel=startIndex+(ushort)(WidthDivision+2);
			nextLevelLess=startIndex+(ushort)(WidthDivision+1);
			//Back face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(ushort)(WidthDivision+1);
			nextLevel=startIndex+(ushort)(LengthDivision+2);
			nextLevelLess=startIndex+(ushort)(LengthDivision+1);
			//Left face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<LengthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(ushort)(LengthDivision+1);
			nextLevel=startIndex+(ushort)(LengthDivision+2);
			nextLevelLess=startIndex+(ushort)(LengthDivision+1);
			//Right face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<LengthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(ushort)(LengthDivision+1);
			nextLevel=startIndex+(ushort)(WidthDivision+2);
			nextLevelLess=startIndex+(ushort)(WidthDivision+1);
			//Top face
			for(j=0; j<LengthDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(ushort)(WidthDivision+1);
			nextLevel=startIndex+(ushort)(WidthDivision+2);
			nextLevelLess=startIndex+(ushort)(WidthDivision+1);
			//Bottom face
			for(j=0; j<LengthDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex16Bit[k  ]=startIndex+1;
					pIndex16Bit[k+1]=nextLevel;
					pIndex16Bit[k+2]=startIndex;
					pIndex16Bit[k+3]=startIndex;
					pIndex16Bit[k+4]=nextLevel;
					pIndex16Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			sint k;
			uint startIndex=0;
			uint nextLevel=(uint)(WidthDivision+2);
			uint nextLevelLess=(uint)(WidthDivision+1);

			AllocateExact_1(pIndex32Bit, &UsedIndex, totalIndex);

			k=0;
			//Front face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(uint)(WidthDivision+1);
			nextLevel=startIndex+(uint)(WidthDivision+2);
			nextLevelLess=startIndex+(uint)(WidthDivision+1);
			//Back face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(uint)(WidthDivision+1);
			nextLevel=startIndex+(uint)(LengthDivision+2);
			nextLevelLess=startIndex+(uint)(LengthDivision+1);
			//Left face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<LengthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(uint)(LengthDivision+1);
			nextLevel=startIndex+(uint)(LengthDivision+2);
			nextLevelLess=startIndex+(uint)(LengthDivision+1);
			//Right face
			for(j=0; j<HeightDivision; j++)
			{
				for(i=0; i<LengthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(uint)(LengthDivision+1);
			nextLevel=startIndex+(uint)(WidthDivision+2);
			nextLevelLess=startIndex+(uint)(WidthDivision+1);
			//Top face
			for(j=0; j<LengthDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}

			startIndex=startIndex+(uint)(WidthDivision+1);
			nextLevel=startIndex+(uint)(WidthDivision+2);
			nextLevelLess=startIndex+(uint)(WidthDivision+1);
			//Bottom face
			for(j=0; j<LengthDivision; j++)
			{
				for(i=0; i<WidthDivision; i++)
				{
					//This is a quad
					pIndex32Bit[k  ]=startIndex+1;
					pIndex32Bit[k+1]=nextLevel;
					pIndex32Bit[k+2]=startIndex;
					pIndex32Bit[k+3]=startIndex;
					pIndex32Bit[k+4]=nextLevel;
					pIndex32Bit[k+5]=nextLevelLess;

					k+=6;
					startIndex++;
					nextLevel++;
					nextLevelLess++;
				}
				startIndex++;
				nextLevel++;
				nextLevelLess++;
			}
		}
	}

	return 1;
}

//PURPOSE:	Generates the normal vector at each vertex.
//
//RETURN:
//			Always 1 for success
sint TCube::GenerateNormals(tbool isAveragedOverFacesOn)
{
	sint i, j;

	if(UsedVertex<=0)
		return 1;

	IsAveragedOverFacesOn=isAveragedOverFacesOn;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		if(pNormals==NULL)
		{
			AllocateExact_1(pNormals, &UsedNormals, UsedVertex);
			UsedNormals=UsedVertex;
		}

		if(isAveragedOverFacesOn==FALSE)
		{
			j=3*4;
			for(i=0; i<j; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=1.0;
			}
			j=j+3*4;
			for(; i<j; i+=3)
			{
				pNormals[i  ]=-1.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=0.0;
			}
			j=j+3*4;
			for(; i<j; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=-1.0;
			}
			j=j+3*4;
			for(; i<j; i+=3)
			{
				pNormals[i  ]=1.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=0.0;
			}
			j=j+3*4;
			for(; i<j; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=-1.0;
				pNormals[i+2]=0.0;
			}
			j=j+3*4;
			for(; i<j; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=1.0;
				pNormals[i+2]=0.0;
			}
		}
	}
	else //if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))==FALSE
	{
		sint total;
		sint vertexCount=(((WidthDivision+1)*(HeightDivision+1))+((WidthDivision+1)*(LengthDivision+1))+
			((HeightDivision+1)*(LengthDivision+1)))*2;

		if(pNormals==NULL)
		{
			AllocateExact_1(pNormals, &UsedNormals, vertexCount*3);
			UsedNormals=vertexCount*3;
		}

		if(isAveragedOverFacesOn==FALSE)
		{
			//Front face
			total=((WidthDivision+1)*(HeightDivision+1))*3;
			for(i=0; i<total; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=1.0;
			}

			//Back Face
			total+=((WidthDivision+1)*(HeightDivision+1))*3;
			for(; i<total; i+=3)
			{
				pNormals[i  ]=-1.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=0.0;
			}

			//Left face
			total+=((WidthDivision+1)*(LengthDivision+1))*3;
			for(; i<total; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=-1.0;
			}

			//Right face
			total+=((WidthDivision+1)*(LengthDivision+1))*3;
			for(; i<total; i+=3)
			{
				pNormals[i  ]=1.0;
				pNormals[i+1]=0.0;
				pNormals[i+2]=0.0;
			}

			//Top
			total+=((HeightDivision+1)*(LengthDivision+1))*3;
			for(; i<total; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=-1.0;
				pNormals[i+2]=0.0;
			}

			//Bottom
			total+=((HeightDivision+1)*(LengthDivision+1))*3;
			for(; i<total; i+=3)
			{
				pNormals[i  ]=0.0;
				pNormals[i+1]=1.0;
				pNormals[i+2]=0.0;
			}
		}
	}

	return 1;
}

//PURPOSE:	Generates the texture coordinates at each vertex.
//			If style==0, then memory is reserved for texcoords only,
//			if style==1, then texcoords are computed for each vertex so that each face gets an image.
//			texUnit must be between 0 and NUMBEROFTEXTUREUNITS
//			numberOfComponents should be 2, 3 or 4
//
//RETURN:
//			Always 1 for success
sint TCube::GenerateTexCoords(sint texUnit, sint style, sint numberOfComponents)
{
	sint i, j, k;

	if(UsedVertex<=0)
		return 1;

	TexCoordStyle=style;

	NumberOfComponentsForTexCoords[texUnit]=numberOfComponents;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		if(pTexCoords[texUnit]==NULL)
		{
			AllocateExact_1(pTexCoords[texUnit], &UsedTexCoords[texUnit], (UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForTexCoords[texUnit]);
			UsedTexCoords[texUnit]=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForTexCoords[texUnit];
		}
		
		if(TexCoordStyle==1)
		{
			if(NumberOfComponentsForTexCoords[texUnit]==2)
			{
				i=0;
				////////////// Front
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				//////////////////// Left
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				/////////////////// Back
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				//////////////////// Right
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				////////////////// Bottom
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				////////////////// Top
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
			}
			else if(NumberOfComponentsForTexCoords[texUnit]==3)
			{
				i=0;
				////////////// Front
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				//////////////////// Left
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				/////////////////// Back
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				//////////////////// Right
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				////////////////// Bottom
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				////////////////// Top
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=0.0;

				pTexCoords[texUnit][i++]=0.0;
				pTexCoords[texUnit][i++]=1.0;
				pTexCoords[texUnit][i++]=0.0;
			}
		}
	}
	else //if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))==FALSE
	{
		sint vertexCount=(((WidthDivision+1)*(HeightDivision+1))+((WidthDivision+1)*(LengthDivision+1))+
			((HeightDivision+1)*(LengthDivision+1)))*2;
		sreal texCoordS, texCoordT;
		sreal texCoordS_Inc, texCoordT_Inc;

		if(pTexCoords[texUnit]==NULL)
		{
			AllocateExact_1(pTexCoords[texUnit], &UsedTexCoords[texUnit], vertexCount*NumberOfComponentsForTexCoords[texUnit]);
			UsedTexCoords[texUnit]=vertexCount*NumberOfComponentsForTexCoords[texUnit];
		}
		
		if(TexCoordStyle==1)
		{
			if(NumberOfComponentsForTexCoords[texUnit]==2)
			{
				k=0;
				//Front face
				texCoordS_Inc=1.0/(sreal)WidthDivision;
				texCoordT_Inc=1.0/(sreal)HeightDivision;
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Back face
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Left face
				texCoordS_Inc=1.0/(sreal)LengthDivision;
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=LengthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Right face
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=LengthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Top face
				texCoordS_Inc=1.0/(sreal)WidthDivision;
				texCoordT_Inc=1.0/(sreal)LengthDivision;
				texCoordT=0.0;
				for(j=0; j<=LengthDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Bottom face
				texCoordT=0.0;
				for(j=0; j<=LengthDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=2)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}
			}
			else if(NumberOfComponentsForTexCoords[texUnit]==3)
			{
				k=0;
				//Front face
				texCoordS_Inc=1.0/(sreal)WidthDivision;
				texCoordT_Inc=1.0/(sreal)HeightDivision;
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Back face
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Left face
				texCoordS_Inc=1.0/(sreal)LengthDivision;
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=LengthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Right face
				texCoordT=0.0;
				for(j=0; j<=HeightDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=LengthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Top face
				texCoordS_Inc=1.0/(sreal)WidthDivision;
				texCoordT_Inc=1.0/(sreal)LengthDivision;
				texCoordT=0.0;
				for(j=0; j<=LengthDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}

				//Bottom face
				texCoordT=0.0;
				for(j=0; j<=LengthDivision; j++)
				{
					texCoordS=0.0;

					for(i=0; i<=WidthDivision; i++, k+=3)
					{
						pTexCoords[texUnit][k  ]=texCoordS;
						pTexCoords[texUnit][k+1]=texCoordT;
						pTexCoords[texUnit][k+2]=0.0;

						texCoordS+=texCoordS_Inc;
					}

					texCoordT+=texCoordT_Inc;
				}
			}
		}
	}

	return 1;
}


//PURPOSE:	Be sure to call GenerateNormals() before calling this.
//			WE NEED TO GENERATE BOTH VECTORS AT THE SAME TIME.
//			Indices are required.
//			Vertices are required.
//			Normals are required.
//			Texcoord[0] is required.
//			
//
//RETURNS:
//			1 for success.
//			-1 if UsedIndex<=0
//			-2 if UsedVertex<=0
//			-3 if UsedNormals<=0
//			-4 if UsedTexCoords[0]<=0
//			-5 if bad value passed to ComputeTangentAndBinormalVectors_v1_FLOAT_1 or ComputeTangentAndBinormalVectors_v1_FLOAT_2
sint TCube::GenerateTangentAndBinormalVectors()
{
	sint tempo, tempo2, returnVal;

	if(UsedIndex<=0)
	{
		strcpy(ErrorMessage, "Index count is zero or less. @ GenerateTangentAndBinormalVectors 640");
		return -1;
	}

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. @ GenerateTangentAndBinormalVectors 646");
		return -2;
	}

	if(UsedNormals<=0)
	{
		strcpy(ErrorMessage, "Normal count is zero or less. @ GenerateTangentAndBinormalVectors 652");
		return -3;
	}

	if(UsedTexCoords[0]<=0)
	{
		strcpy(ErrorMessage, "Texcoord[0] count is zero or less. @ GenerateTangentAndBinormalVectors 658");
		return -4;
	}

	UsedTangents=0;
	UsedBinormals=0;
	//DeallocateMemoryForTangentVectors();
	//DeallocateMemoryForBinormalVectors();

	//One per vertex
	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	AllocateExact_1(pTangents, &UsedTangents, tempo);

	tempo2=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	AllocateExact_1(pBinormals, &UsedBinormals, tempo);

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_2(UsedIndex, pIndex16Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 243");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 248");
			return 0;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT(UsedIndex, pIndex32Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 258");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 263");
			return 0;
		}
	}

	/*{
		if(NumberOfComponentsForTangents==3)
		{
			for(i=0; i<tempo; i+=3)
			{
				pTangents[i  ]=1.0;
				pTangents[i+1]=0.0;
				pTangents[i+2]=0.0;
			}
		}
		else if(NumberOfComponentsForTangents==4)
		{
			for(i=0; i<tempo; i+=4)
			{
				pTangents[i  ]=1.0;
				pTangents[i+1]=0.0;
				pTangents[i+2]=0.0;
				pTangents[i+3]=0.0;
			}
		}

		if(NumberOfComponentsForBinormals==3)
		{
			for(i=0; i<tempo2; i+=3)
			{
				pBinormals[i  ]=1.0;
				pBinormals[i+1]=0.0;
				pBinormals[i+2]=0.0;
			}
		}
		else if(NumberOfComponentsForBinormals==4)
		{
			for(i=0; i<tempo2; i+=4)
			{
				pBinormals[i  ]=1.0;
				pBinormals[i+1]=0.0;
				pBinormals[i+2]=0.0;
				pBinormals[i+3]=0.0;
			}
		}
	}*/

	UsedTangents=tempo;
	UsedBinormals=tempo2;

	return 1;
}


