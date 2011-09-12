// glhlib10.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
//#include <GL/gl.h>
//#include <GL/glext.h>
//#include <GL/wglext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include <stdlib.h>
#include "3DGraphicsLibrarySmall.h"
#include "TCube.h"
#include "TCone.h"
#include "TSphere.h"
#include "TTorus.h"
#include "TCone.h"
#include "TTube.h"
#include "TChamferCylinder.h"
#include <stack>




#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'



using namespace std;



//INTERNAL VARIABLES, CLASSES, STRUCTS
//sint	ErrorState;			//Record error if one occurs. I will no longer use this. I will instead use a stack with max depth of 100
extern sint			TotalErrorStateStack;
extern stack<sint>	ErrorStateStack;







sint glhCreateConef2(glhConeObjectf2 *pcone)
{
	sint i, returnVal;

	if(pcone==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, pcone->Radius, TRUE, pcone->Height, FALSE, pcone->Height,
		pcone->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((pcone->TexCoordStyle[i]!=0)&&(pcone->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(pcone->Stacks<=0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(pcone->Slices<=2)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TCone cone;

	returnVal=cone.StartupByStacksAndSlices2(pcone->IndexFormat, pcone->VertexFormat,
		(pcone->IsThereABottom) ? TRUE : FALSE,
		pcone->Radius, pcone->Height, pcone->Stacks, pcone->Slices,
		pcone->TexCoordStyle[0], pcone->ScaleFactorS[0], pcone->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		cone.DeallocateAllMemory();
		return -1;
	}


	//Write stuff to pcone
	pcone->TotalIndex=cone.UsedIndex;
	pcone->pIndex16Bit=cone.pIndex16Bit;
	cone.pIndex16Bit=NULL;
	pcone->pIndex32Bit=cone.pIndex32Bit;
	cone.pIndex32Bit=NULL;
	pcone->VertexCount=cone.VertexCount;

	switch(cone.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		pcone->pVertex=(sreal *)cone.pVertex_V;
		cone.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		pcone->pVertex=(sreal *)cone.pVertex_VT;
		cone.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		pcone->pVertex=(sreal *)cone.pVertex_VNT;
		cone.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		pcone->pVertex=(sreal *)cone.pVertex_VNT3;
		cone.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		pcone->pVertex=(sreal *)cone.pVertex_VNTT3T3;
		cone.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		pcone->pVertex=(sreal *)cone.pVertex_VNTT3;
		cone.pVertex_VNTT3=NULL;
		break;
	}


	pcone->Stacks=cone.Stacks;
	pcone->Slices=cone.Slices;

	//Copy the rest of it in a few shots
	pcone->Start_DrawRangeElements=cone.Start_DrawRangeElements;
	pcone->End_DrawRangeElements=cone.End_DrawRangeElements;

	pcone->Start_DrawRangeElements_BodyOfCone=cone.Start_DrawRangeElements_BodyOfCone;
	pcone->End_DrawRangeElements_BodyOfCone=cone.End_DrawRangeElements_BodyOfCone;
	pcone->Start_DrawRangeElements_BottomOfCone=cone.Start_DrawRangeElements_BottomOfCone;
	pcone->End_DrawRangeElements_BottomOfCone=cone.End_DrawRangeElements_BottomOfCone;

	pcone->StartIndexIn_pIndex_BodyOfCone=cone.StartIndexIn_pIndex_BodyOfCone;
	pcone->EndIndexIn_pIndex_BodyOfCone=cone.EndIndexIn_pIndex_BodyOfCone;
	pcone->StartIndexIn_pIndex_BottomOfCone=cone.StartIndexIn_pIndex_BottomOfCone;
	pcone->EndIndexIn_pIndex_BottomOfCone=cone.EndIndexIn_pIndex_BottomOfCone;

	pcone->TotalIndex_BodyOfCone=cone.TotalIndex_BodyOfCone;
	pcone->TotalIndex_BottomOfCone=cone.TotalIndex_BottomOfCone;

	pcone->pIndex16Bit_BodyOfCone=cone.pIndex16Bit_BodyOfCone;
	pcone->pIndex16Bit_BottomOfCone=cone.pIndex16Bit_BottomOfCone;

	pcone->pIndex32Bit_BodyOfCone=cone.pIndex32Bit_BodyOfCone;
	pcone->pIndex32Bit_BottomOfCone=cone.pIndex32Bit_BottomOfCone;

	pcone->StartIndexIn_pVertex_BodyOfCone=cone.StartIndexIn_pVertex_BodyOfCone;
	pcone->EndIndexIn_pVertex_BodyOfCone=cone.EndIndexIn_pVertex_BodyOfCone;
	pcone->StartIndexIn_pVertex_BottomOfCone=cone.StartIndexIn_pVertex_BottomOfCone;
	pcone->EndIndexIn_pVertex_BottomOfCone=cone.EndIndexIn_pVertex_BottomOfCone;

	pcone->TotalVertex_BodyOfCone=cone.TotalVertex_BodyOfCone;
	pcone->TotalVertex_BottomOfCone=cone.TotalVertex_BottomOfCone;

	pcone->pVertex_BodyOfCone=cone.pVertex_BodyOfCone;
	pcone->pVertex_BottomOfCone=cone.pVertex_BottomOfCone;

	cone.DeallocateAllMemory();
	return 1;
}

sint glhDeleteConef2(glhConeObjectf2 *pcone)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(pcone);
	DeleteCommonObject(common);
	return 1;
}

sint glhCreateTubef2(glhTubeObjectf2 *ptube)
{
	sint i, returnVal;

	if(ptube==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, ptube->OuterRadiusA, TRUE, ptube->OuterRadiusB, TRUE, ptube->Height,
		ptube->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, ptube->InnerRadiusA, TRUE, ptube->InnerRadiusB, TRUE, ptube->Height,
		ptube->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((ptube->TexCoordStyle[i]!=0)&&(ptube->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(ptube->Stacks<=0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(ptube->Slices<=2)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TTube tube;

	returnVal=tube.StartupByTwoRadiusStacksAndSlices(ptube->IndexFormat, ptube->VertexFormat,
		(ptube->IsThereATop) ? TRUE : FALSE,
		(ptube->IsThereABottom) ? TRUE : FALSE,
		ptube->OuterRadiusA, ptube->OuterRadiusB, ptube->InnerRadiusA, ptube->InnerRadiusB, ptube->Height,
		ptube->Stacks, ptube->Slices,
		ptube->TexCoordStyle[0], ptube->ScaleFactorS[0], ptube->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		tube.DeallocateAllMemory();
		return -1;
	}


	//Write stuff to ptube
	ptube->TotalIndex=tube.UsedIndex;
	ptube->pIndex16Bit=tube.pIndex16Bit;
	tube.pIndex16Bit=NULL;
	ptube->pIndex32Bit=tube.pIndex32Bit;
	tube.pIndex32Bit=NULL;
	ptube->VertexCount=tube.VertexCount;

	switch(tube.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		ptube->pVertex=(sreal *)tube.pVertex_V;
		tube.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		ptube->pVertex=(sreal *)tube.pVertex_VT;
		tube.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		ptube->pVertex=(sreal *)tube.pVertex_VNT;
		tube.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		ptube->pVertex=(sreal *)tube.pVertex_VNT3;
		tube.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		ptube->pVertex=(sreal *)tube.pVertex_VNTT3T3;
		tube.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		ptube->pVertex=(sreal *)tube.pVertex_VNTT3;
		tube.pVertex_VNTT3=NULL;
		break;
	}


	ptube->Stacks=tube.Stacks;
	ptube->Slices=tube.Slices;

	//Copy the rest of it in a few shots
	ptube->Start_DrawRangeElements=tube.Start_DrawRangeElements;
	ptube->End_DrawRangeElements=tube.End_DrawRangeElements;

	ptube->Start_DrawRangeElements_OuterBodyOfTube=tube.Start_DrawRangeElements_OuterBodyOfTube;
	ptube->End_DrawRangeElements_OuterBodyOfTube=tube.End_DrawRangeElements_OuterBodyOfTube;
	ptube->Start_DrawRangeElements_InnerBodyOfTube=tube.Start_DrawRangeElements_InnerBodyOfTube;
	ptube->End_DrawRangeElements_InnerBodyOfTube=tube.End_DrawRangeElements_InnerBodyOfTube;
	ptube->Start_DrawRangeElements_TopOfTube=tube.Start_DrawRangeElements_TopOfTube;
	ptube->End_DrawRangeElements_TopOfTube=tube.End_DrawRangeElements_TopOfTube;
	ptube->Start_DrawRangeElements_BottomOfTube=tube.Start_DrawRangeElements_BottomOfTube;
	ptube->End_DrawRangeElements_BottomOfTube=tube.End_DrawRangeElements_BottomOfTube;

	ptube->StartIndexIn_pIndex_OuterBodyOfTube=tube.StartIndexIn_pIndex_OuterBodyOfTube;
	ptube->EndIndexIn_pIndex_OuterBodyOfTube=tube.EndIndexIn_pIndex_OuterBodyOfTube;
	ptube->StartIndexIn_pIndex_InnerBodyOfTube=tube.StartIndexIn_pIndex_InnerBodyOfTube;
	ptube->EndIndexIn_pIndex_InnerBodyOfTube=tube.EndIndexIn_pIndex_InnerBodyOfTube;
	ptube->StartIndexIn_pIndex_TopOfTube=tube.StartIndexIn_pIndex_TopOfTube;
	ptube->EndIndexIn_pIndex_TopOfTube=tube.EndIndexIn_pIndex_TopOfTube;
	ptube->StartIndexIn_pIndex_BottomOfTube=tube.StartIndexIn_pIndex_BottomOfTube;
	ptube->EndIndexIn_pIndex_BottomOfTube=tube.EndIndexIn_pIndex_BottomOfTube;

	ptube->TotalIndex_OuterBodyOfTube=tube.TotalIndex_OuterBodyOfTube;
	ptube->TotalIndex_InnerBodyOfTube=tube.TotalIndex_InnerBodyOfTube;
	ptube->TotalIndex_TopOfTube=tube.TotalIndex_TopOfTube;
	ptube->TotalIndex_BottomOfTube=tube.TotalIndex_BottomOfTube;

	ptube->pIndex16Bit_OuterBodyOfTube=tube.pIndex16Bit_OuterBodyOfTube;
	ptube->pIndex16Bit_InnerBodyOfTube=tube.pIndex16Bit_InnerBodyOfTube;
	ptube->pIndex16Bit_TopOfTube=tube.pIndex16Bit_TopOfTube;
	ptube->pIndex16Bit_BottomOfTube=tube.pIndex16Bit_BottomOfTube;

	ptube->pIndex32Bit_OuterBodyOfTube=tube.pIndex32Bit_OuterBodyOfTube;
	ptube->pIndex32Bit_InnerBodyOfTube=tube.pIndex32Bit_InnerBodyOfTube;
	ptube->pIndex32Bit_TopOfTube=tube.pIndex32Bit_TopOfTube;
	ptube->pIndex32Bit_BottomOfTube=tube.pIndex32Bit_BottomOfTube;

	ptube->StartIndexIn_pVertex_OuterBodyOfTube=tube.StartIndexIn_pVertex_OuterBodyOfTube;
	ptube->EndIndexIn_pVertex_OuterBodyOfTube=tube.EndIndexIn_pVertex_OuterBodyOfTube;
	ptube->StartIndexIn_pVertex_InnerBodyOfTube=tube.StartIndexIn_pVertex_InnerBodyOfTube;
	ptube->EndIndexIn_pVertex_InnerBodyOfTube=tube.EndIndexIn_pVertex_InnerBodyOfTube;
	ptube->StartIndexIn_pVertex_TopOfTube=tube.StartIndexIn_pVertex_TopOfTube;
	ptube->EndIndexIn_pVertex_TopOfTube=tube.EndIndexIn_pVertex_TopOfTube;
	ptube->StartIndexIn_pVertex_BottomOfTube=tube.StartIndexIn_pVertex_BottomOfTube;
	ptube->EndIndexIn_pVertex_BottomOfTube=tube.EndIndexIn_pVertex_BottomOfTube;

	ptube->TotalVertex_OuterBodyOfTube=tube.TotalVertex_OuterBodyOfTube;
	ptube->TotalVertex_InnerBodyOfTube=tube.TotalVertex_InnerBodyOfTube;
	ptube->TotalVertex_TopOfTube=tube.TotalVertex_TopOfTube;
	ptube->TotalVertex_BottomOfTube=tube.TotalVertex_BottomOfTube;

	ptube->pVertex_OuterBodyOfTube=tube.pVertex_OuterBodyOfTube;
	ptube->pVertex_InnerBodyOfTube=tube.pVertex_InnerBodyOfTube;
	ptube->pVertex_TopOfTube=tube.pVertex_TopOfTube;
	ptube->pVertex_BottomOfTube=tube.pVertex_BottomOfTube;

	tube.DeallocateAllMemory();
	return 1;
}

sint glhDeleteTubef2(glhTubeObjectf2 *ptube)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(ptube);
	DeleteCommonObject(common);
	return 1;
}


sint glhCreateChamferCylinderf2(glhChamferCylinderObjectf2 *pchamferCylinder)
{
	sint i, returnVal;

	if(pchamferCylinder==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, pchamferCylinder->BodyRadius, FALSE, pchamferCylinder->ChamferRadius, TRUE, pchamferCylinder->Height,
		pchamferCylinder->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	if(pchamferCylinder->ChamferRadius<0.0)
	{
		ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
		TotalErrorStateStack++;
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((pchamferCylinder->TexCoordStyle[i]!=0)&&(pchamferCylinder->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(pchamferCylinder->Stacks<=0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(pchamferCylinder->Slices<=2)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TChamferCylinder chamferCylinder;

	returnVal=chamferCylinder.StartupByStacksAndSlices2(pchamferCylinder->IndexFormat, pchamferCylinder->VertexFormat,
		(pchamferCylinder->IsThereATop) ? TRUE : FALSE, (pchamferCylinder->IsThereABottom) ? TRUE : FALSE,
		pchamferCylinder->BodyRadius, pchamferCylinder->ChamferRadius, pchamferCylinder->Height, pchamferCylinder->Stacks, pchamferCylinder->Slices,
		pchamferCylinder->ChamferStacks, pchamferCylinder->TexCoordStyle[0], pchamferCylinder->ScaleFactorS[0], pchamferCylinder->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		chamferCylinder.DeallocateAllMemory();
		return -1;
	}


	//Write stuff to pcylinder
	pchamferCylinder->TotalIndex=chamferCylinder.UsedIndex;
	pchamferCylinder->pIndex16Bit=chamferCylinder.pIndex16Bit;
	chamferCylinder.pIndex16Bit=NULL;
	pchamferCylinder->pIndex32Bit=chamferCylinder.pIndex32Bit;
	chamferCylinder.pIndex32Bit=NULL;
	pchamferCylinder->VertexCount=chamferCylinder.VertexCount;

	switch(chamferCylinder.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_V;
		chamferCylinder.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_VT;
		chamferCylinder.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_VNT;
		chamferCylinder.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_VNT3;
		chamferCylinder.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_VNTT3T3;
		chamferCylinder.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		pchamferCylinder->pVertex=(sreal *)chamferCylinder.pVertex_VNTT3;
		chamferCylinder.pVertex_VNTT3=NULL;
		break;
	}


	pchamferCylinder->Stacks=chamferCylinder.Stacks;
	pchamferCylinder->Slices=chamferCylinder.Slices;
	pchamferCylinder->ChamferStacks=chamferCylinder.ChamferStacks;

	//Copy the rest of it in a few shots
	pchamferCylinder->Start_DrawRangeElements=chamferCylinder.Start_DrawRangeElements;
	pchamferCylinder->End_DrawRangeElements=chamferCylinder.End_DrawRangeElements;

	pchamferCylinder->Start_DrawRangeElements_TopChamfer=chamferCylinder.Start_DrawRangeElements_TopChamfer;
	pchamferCylinder->End_DrawRangeElements_TopChamfer=chamferCylinder.End_DrawRangeElements_TopChamfer;
	pchamferCylinder->Start_DrawRangeElements_BodyOfCylinder=chamferCylinder.Start_DrawRangeElements_BodyOfCylinder;
	pchamferCylinder->End_DrawRangeElements_BodyOfCylinder=chamferCylinder.End_DrawRangeElements_BodyOfCylinder;
	pchamferCylinder->Start_DrawRangeElements_BottomChamfer=chamferCylinder.Start_DrawRangeElements_BottomChamfer;
	pchamferCylinder->End_DrawRangeElements_BottomChamfer=chamferCylinder.End_DrawRangeElements_BottomChamfer;
	pchamferCylinder->Start_DrawRangeElements_TopOfCylinder=chamferCylinder.Start_DrawRangeElements_TopOfCylinder;
	pchamferCylinder->End_DrawRangeElements_TopOfCylinder=chamferCylinder.End_DrawRangeElements_TopOfCylinder;
	pchamferCylinder->Start_DrawRangeElements_BottomOfCylinder=chamferCylinder.Start_DrawRangeElements_BottomOfCylinder;
	pchamferCylinder->End_DrawRangeElements_BottomOfCylinder=chamferCylinder.End_DrawRangeElements_BottomOfCylinder;

	pchamferCylinder->StartIndexIn_pIndex_TopChamfer=chamferCylinder.StartIndexIn_pIndex_TopChamfer;
	pchamferCylinder->EndIndexIn_pIndex_TopChamfer=chamferCylinder.EndIndexIn_pIndex_TopChamfer;
	pchamferCylinder->StartIndexIn_pIndex_BodyOfCylinder=chamferCylinder.StartIndexIn_pIndex_BodyOfCylinder;
	pchamferCylinder->EndIndexIn_pIndex_BodyOfCylinder=chamferCylinder.EndIndexIn_pIndex_BodyOfCylinder;
	pchamferCylinder->StartIndexIn_pIndex_BottomChamfer=chamferCylinder.StartIndexIn_pIndex_BottomChamfer;
	pchamferCylinder->EndIndexIn_pIndex_BottomChamfer=chamferCylinder.EndIndexIn_pIndex_BottomChamfer;
	pchamferCylinder->StartIndexIn_pIndex_TopOfCylinder=chamferCylinder.StartIndexIn_pIndex_TopOfCylinder;
	pchamferCylinder->EndIndexIn_pIndex_TopOfCylinder=chamferCylinder.EndIndexIn_pIndex_TopOfCylinder;
	pchamferCylinder->StartIndexIn_pIndex_BottomOfCylinder=chamferCylinder.StartIndexIn_pIndex_BottomOfCylinder;
	pchamferCylinder->EndIndexIn_pIndex_BottomOfCylinder=chamferCylinder.EndIndexIn_pIndex_BottomOfCylinder;

	pchamferCylinder->TotalIndex_TopChamfer=chamferCylinder.TotalIndex_TopChamfer;
	pchamferCylinder->TotalIndex_BodyOfCylinder=chamferCylinder.TotalIndex_BodyOfCylinder;
	pchamferCylinder->TotalIndex_BottomChamfer=chamferCylinder.TotalIndex_BottomChamfer;
	pchamferCylinder->TotalIndex_TopOfCylinder=chamferCylinder.TotalIndex_TopOfCylinder;
	pchamferCylinder->TotalIndex_BottomOfCylinder=chamferCylinder.TotalIndex_BottomOfCylinder;

	pchamferCylinder->pIndex16Bit_TopChamfer=chamferCylinder.pIndex16Bit_TopChamfer;
	pchamferCylinder->pIndex16Bit_BodyOfCylinder=chamferCylinder.pIndex16Bit_BodyOfCylinder;
	pchamferCylinder->pIndex16Bit_BottomChamfer=chamferCylinder.pIndex16Bit_BottomChamfer;
	pchamferCylinder->pIndex16Bit_TopOfCylinder=chamferCylinder.pIndex16Bit_TopOfCylinder;
	pchamferCylinder->pIndex16Bit_BottomOfCylinder=chamferCylinder.pIndex16Bit_BottomOfCylinder;

	pchamferCylinder->pIndex32Bit_TopChamfer=chamferCylinder.pIndex32Bit_TopChamfer;
	pchamferCylinder->pIndex32Bit_BodyOfCylinder=chamferCylinder.pIndex32Bit_BodyOfCylinder;
	pchamferCylinder->pIndex32Bit_BottomChamfer=chamferCylinder.pIndex32Bit_BottomChamfer;
	pchamferCylinder->pIndex32Bit_TopOfCylinder=chamferCylinder.pIndex32Bit_TopOfCylinder;
	pchamferCylinder->pIndex32Bit_BottomOfCylinder=chamferCylinder.pIndex32Bit_BottomOfCylinder;

	pchamferCylinder->StartIndexIn_pVertex_TopChamfer=chamferCylinder.StartIndexIn_pVertex_TopChamfer;
	pchamferCylinder->EndIndexIn_pVertex_TopChamfer=chamferCylinder.EndIndexIn_pVertex_TopChamfer;
	pchamferCylinder->StartIndexIn_pVertex_BodyOfCylinder=chamferCylinder.StartIndexIn_pVertex_BodyOfCylinder;
	pchamferCylinder->EndIndexIn_pVertex_BodyOfCylinder=chamferCylinder.EndIndexIn_pVertex_BodyOfCylinder;
	pchamferCylinder->StartIndexIn_pVertex_BottomChamfer=chamferCylinder.StartIndexIn_pVertex_BottomChamfer;
	pchamferCylinder->EndIndexIn_pVertex_BottomChamfer=chamferCylinder.EndIndexIn_pVertex_BottomChamfer;
	pchamferCylinder->StartIndexIn_pVertex_TopOfCylinder=chamferCylinder.StartIndexIn_pVertex_TopOfCylinder;
	pchamferCylinder->EndIndexIn_pVertex_TopOfCylinder=chamferCylinder.EndIndexIn_pVertex_TopOfCylinder;
	pchamferCylinder->StartIndexIn_pVertex_BottomOfCylinder=chamferCylinder.StartIndexIn_pVertex_BottomOfCylinder;
	pchamferCylinder->EndIndexIn_pVertex_BottomOfCylinder=chamferCylinder.EndIndexIn_pVertex_BottomOfCylinder;

	pchamferCylinder->TotalVertex_TopChamfer=chamferCylinder.TotalVertex_TopChamfer;
	pchamferCylinder->TotalVertex_BodyOfCylinder=chamferCylinder.TotalVertex_BodyOfCylinder;
	pchamferCylinder->TotalVertex_BottomChamfer=chamferCylinder.TotalVertex_BottomChamfer;
	pchamferCylinder->TotalVertex_TopOfCylinder=chamferCylinder.TotalVertex_TopOfCylinder;
	pchamferCylinder->TotalVertex_BottomOfCylinder=chamferCylinder.TotalVertex_BottomOfCylinder;

	pchamferCylinder->pVertex_TopChamfer=chamferCylinder.pVertex_TopChamfer;
	pchamferCylinder->pVertex_BodyOfCylinder=chamferCylinder.pVertex_BodyOfCylinder;
	pchamferCylinder->pVertex_BottomChamfer=chamferCylinder.pVertex_BottomChamfer;
	pchamferCylinder->pVertex_TopOfCylinder=chamferCylinder.pVertex_TopOfCylinder;
	pchamferCylinder->pVertex_BottomOfCylinder=chamferCylinder.pVertex_BottomOfCylinder;

	chamferCylinder.DeallocateAllMemory();
	return 1;
}


sint glhDeleteChamferCylinderf2(glhChamferCylinderObjectf2 *pchamferCylinder)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(pchamferCylinder);
	DeleteCommonObject(common);
	return 1;
}


void glhEvaluateCatmullRomCurvef(int numberOfComponentsForVertex, float *pvertex, int intervalCount, const float *controlPoints, int numberOfControlPoints)
{
	EvaluateCatmullRomCurveFLOAT_2(numberOfComponentsForVertex, pvertex, intervalCount, controlPoints, numberOfControlPoints);
}



