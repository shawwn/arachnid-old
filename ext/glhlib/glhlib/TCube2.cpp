// TCube2.cpp: implementation of the TCube class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCube.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"


#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'







//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents binormals)
sint TCube::GenerateVertices_Interleaved_Part10()
{
	sint i, returnVal;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		i=0;
		//Front face
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		//Left face
		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=-1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=-1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=-1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=-1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		//Back face
		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		//Right face
		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=1.0;
		pVertex_VNTT3T3[i].ny=0.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		//Bottom
		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=-1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=-1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=-1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=-1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		//Top
		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=1.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3T3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3T3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3T3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3T3[i].nx=0.0;
		pVertex_VNTT3T3[i].ny=1.0;
		pVertex_VNTT3T3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3T3[i].s0=0.0;
			pVertex_VNTT3T3[i].t0=1.0;
		}
	}

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 469");
			return -1;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 479");
			return -1;
		}
	}

	return 1;
}

//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents)
sint TCube::GenerateVertices_Interleaved_Part10b()
{
	sint i, returnVal;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;

	if((WidthDivision==1)&&(HeightDivision==1)&&(LengthDivision==1))
	{	//PROBABLY I CAN GET RID OF THIS SECTION IN THE FUTURE
		i=0;
		//Front face
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		//Left face
		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=-1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=-1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=-1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=-1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		//Back face
		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=-1.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		//Right face
		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=1.0;
		pVertex_VNTT3[i].ny=0.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		//Bottom
		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=-1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=-1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=-1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=-halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=-1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		//Top
		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=1.0;
			pVertex_VNTT3[i].t0=1.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}

		i++;
		pVertex_VNTT3[i].x=-halfWidth+CenterPosition[0];
		pVertex_VNTT3[i].y=halfHeight+CenterPosition[1];
		pVertex_VNTT3[i].z=-halfLength+CenterPosition[2];
		pVertex_VNTT3[i].nx=0.0;
		pVertex_VNTT3[i].ny=1.0;
		pVertex_VNTT3[i].nz=0.0;
		if(TexCoordStyle==0)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=0.0;
		}
		else if(TexCoordStyle==1)
		{
			pVertex_VNTT3[i].s0=0.0;
			pVertex_VNTT3[i].t0=1.0;
		}
	}

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
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 469");
			return -1;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 479");
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

	return 1;
}

//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//RETURN:
//			1 for success
sint TCube::GenerateVertices_Interleaved_Part11()
{
	sint i, j, k;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

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

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

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

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

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

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_V[k].x=positionX;
			pVertex_V[k].y=positionY;
			pVertex_V[k].z=positionZ;

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
	}

	return 1;
}


//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//RETURN:
//			1 for success
sint TCube::GenerateVertices_Interleaved_Part12()
{
	sint i, j, k;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;
	sreal texCoordS, texCoordT;
	sreal texCoordS_Inc, texCoordT_Inc;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)HeightDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionX+=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Back face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionX-=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Left face
	positionX=-halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=-halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionZ+=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Right face
	positionX=halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VT[k].x=positionX;
			pVertex_VT[k].y=positionY;
			pVertex_VT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VT[k].s0=texCoordS;
				pVertex_VT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	return 1;
}

//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//			**** I'm only doing if(isAveragedOverFacesOn==FALSE)
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents binormals)
sint TCube::GenerateVertices_Interleaved_Part13()
{
	sint i, j, k;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;
	sreal texCoordS, texCoordT;
	sreal texCoordS_Inc, texCoordT_Inc;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)HeightDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=0.0;
			pVertex_VNT[k].ny=0.0;
			pVertex_VNT[k].nz=1.0;

			positionX+=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Back face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=0.0;
			pVertex_VNT[k].ny=0.0;
			pVertex_VNT[k].nz=-1.0;

			positionX-=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Left face
	positionX=-halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=-halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=-1.0;
			pVertex_VNT[k].ny=0.0;
			pVertex_VNT[k].nz=0.0;

			positionZ+=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Right face
	positionX=halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=1.0;
			pVertex_VNT[k].ny=0.0;
			pVertex_VNT[k].nz=0.0;

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=0.0;
			pVertex_VNT[k].ny=1.0;
			pVertex_VNT[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT[k].x=positionX;
			pVertex_VNT[k].y=positionY;
			pVertex_VNT[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT[k].s0=texCoordS;
				pVertex_VNT[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT[k].nx=0.0;
			pVertex_VNT[k].ny=-1.0;
			pVertex_VNT[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	return 1;
}

//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//			**** I'm only doing if(isAveragedOverFacesOn==FALSE)
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents binormals)
sint TCube::GenerateVertices_Interleaved_Part14()
{
	sint i, j, k;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;
	sreal texCoordS, texCoordT;
	sreal texCoordS_Inc, texCoordT_Inc;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)HeightDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=0.0;
			pVertex_VNT3[k].ny=0.0;
			pVertex_VNT3[k].nz=1.0;

			positionX+=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Back face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=0.0;
			pVertex_VNT3[k].ny=0.0;
			pVertex_VNT3[k].nz=-1.0;

			positionX-=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Left face
	positionX=-halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=-halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=-1.0;
			pVertex_VNT3[k].ny=0.0;
			pVertex_VNT3[k].nz=0.0;

			positionZ+=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Right face
	positionX=halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=1.0;
			pVertex_VNT3[k].ny=0.0;
			pVertex_VNT3[k].nz=0.0;

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=0.0;
			pVertex_VNT3[k].ny=1.0;
			pVertex_VNT3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNT3[k].x=positionX;
			pVertex_VNT3[k].y=positionY;
			pVertex_VNT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNT3[k].s0=texCoordS;
				pVertex_VNT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNT3[k].nx=0.0;
			pVertex_VNT3[k].ny=-1.0;
			pVertex_VNT3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	return 1;
}


//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//			**** I'm only doing if(isAveragedOverFacesOn==FALSE)
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents binormals)
sint TCube::GenerateVertices_Interleaved_Part15()
{
	sint i, j, k, returnVal;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;
	sreal texCoordS, texCoordT;
	sreal texCoordS_Inc, texCoordT_Inc;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)HeightDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=0.0;
			pVertex_VNTT3T3[k].ny=0.0;
			pVertex_VNTT3T3[k].nz=1.0;

			positionX+=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Back face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=0.0;
			pVertex_VNTT3T3[k].ny=0.0;
			pVertex_VNTT3T3[k].nz=-1.0;

			positionX-=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Left face
	positionX=-halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=-halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=-1.0;
			pVertex_VNTT3T3[k].ny=0.0;
			pVertex_VNTT3T3[k].nz=0.0;

			positionZ+=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Right face
	positionX=halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=1.0;
			pVertex_VNTT3T3[k].ny=0.0;
			pVertex_VNTT3T3[k].nz=0.0;

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=0.0;
			pVertex_VNTT3T3[k].ny=1.0;
			pVertex_VNTT3T3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3T3[k].x=positionX;
			pVertex_VNTT3T3[k].y=positionY;
			pVertex_VNTT3T3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3T3[k].s0=texCoordS;
				pVertex_VNTT3T3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3T3[k].nx=0.0;
			pVertex_VNTT3T3[k].ny=-1.0;
			pVertex_VNTT3T3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 1562");
			return -1;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 1571");
			return -1;
		}
	}

	return 1;
}


//PURPOSE:	Since the code was getting long in GenerateVertices_Interleaved()
//			I moved part of it to here.
//
//			**** I'm only doing if(isAveragedOverFacesOn==FALSE)
//
//RETURN:
//			1 for success
//			-1 if error (could not generate tangents binormals)
sint TCube::GenerateVertices_Interleaved_Part15b()
{
	sint i, j, k, returnVal;
	sreal halfWidth=Width*0.5, halfHeight=Height*0.5, halfLength=Length*0.5;
	sreal positionX, positionY, positionZ;
	sreal positionX_Inc, positionY_Inc, positionZ_Inc;
	positionX_Inc=Width/(sreal)WidthDivision;
	positionY_Inc=Height/(sreal)HeightDivision;
	positionZ_Inc=Length/(sreal)LengthDivision;
	sreal texCoordS, texCoordT;
	sreal texCoordS_Inc, texCoordT_Inc;

	k=0;
	//Front face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)HeightDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=0.0;
			pVertex_VNTT3[k].ny=0.0;
			pVertex_VNTT3[k].nz=1.0;

			positionX+=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Back face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionX=halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=0.0;
			pVertex_VNTT3[k].ny=0.0;
			pVertex_VNTT3[k].nz=-1.0;

			positionX-=positionX_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Left face
	positionX=-halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=-halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=-1.0;
			pVertex_VNTT3[k].ny=0.0;
			pVertex_VNTT3[k].nz=0.0;

			positionZ+=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Right face
	positionX=halfWidth+CenterPosition[0];
	positionY=-halfHeight+CenterPosition[1];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=HeightDivision; j++)
	{
		positionZ=halfLength+CenterPosition[2];
		texCoordS=0.0;

		for(i=0; i<=LengthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=1.0;
			pVertex_VNTT3[k].ny=0.0;
			pVertex_VNTT3[k].nz=0.0;

			positionZ-=positionZ_Inc;
		}

		positionY+=positionY_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Top face
	positionY=halfHeight+CenterPosition[1];
	positionZ=halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordS_Inc=1.0/(sreal)WidthDivision;
		texCoordT_Inc=1.0/(sreal)LengthDivision;
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=0.0;
			pVertex_VNTT3[k].ny=1.0;
			pVertex_VNTT3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ-=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

	//Bottom face
	positionY=-halfHeight+CenterPosition[1];
	positionZ=-halfLength+CenterPosition[2];
	if(TexCoordStyle==1)
	{
		texCoordT=0.0;
	}
	for(j=0; j<=LengthDivision; j++)
	{
		positionX=-halfWidth+CenterPosition[0];
		texCoordS=0.0;

		for(i=0; i<=WidthDivision; i++, k++)
		{
			pVertex_VNTT3[k].x=positionX;
			pVertex_VNTT3[k].y=positionY;
			pVertex_VNTT3[k].z=positionZ;
			if(TexCoordStyle==1)
			{
				pVertex_VNTT3[k].s0=texCoordS;
				pVertex_VNTT3[k].t0=texCoordT;

				texCoordS+=texCoordS_Inc;
			}
			pVertex_VNTT3[k].nx=0.0;
			pVertex_VNTT3[k].ny=-1.0;
			pVertex_VNTT3[k].nz=0.0;

			positionX+=positionX_Inc;
		}

		positionZ+=positionZ_Inc;
		texCoordT+=texCoordT_Inc;
	}

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
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 1562");
			return -1;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed. @ GenerateTangentAndBinormalVectors 1571");
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

	return 1;
}


//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//




