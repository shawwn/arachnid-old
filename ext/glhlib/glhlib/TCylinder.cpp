// TCylinder.cpp: implementation of the TCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCylinder.h"
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

TCylinder::TCylinder()
{
	sint i;

	IsThereATop=IsThereABottom=FALSE;
	ARadius=BRadius=Height=0.0;
	PolygonNumber=Stacks=SlicesPlusOne=Slices=0;

	for(i=0; i<8; i++)
	{
		Colors_Top[i]=0.0;
		SecondaryColors_Top[i]=0.0;
		Colors_Bottom[i]=0.0;
		SecondaryColors_Bottom[i]=0.0;
	}

	for(i=0; i<2; i++)
	{
		ColorsUINT_Top[i]=0;
		SecondaryColorsUINT_Top[i]=0;
		ColorsUINT_Bottom[i]=0;
		SecondaryColorsUINT_Bottom[i]=0;
	}


	MaterialID_Top=MaterialID_Bottom=0;
	for(i=0; i<8; i++)
	{
		MaterialAmbient_Top[i]=MaterialDiffuse_Top[i]=0.0;
		MaterialEmission_Top[i]=MaterialSpecular_Top[i]=0.0;
		MaterialAmbient_Bottom[i]=MaterialDiffuse_Bottom[i]=0.0;
		MaterialEmission_Bottom[i]=MaterialSpecular_Bottom[i]=0.0;
	}
	MaterialShininess_Top=MaterialShininess_Bottom=0.0;

	GeneralReset();
}

TCylinder::~TCylinder()
{
	DeallocateAllMemory();
}

//PURPOSE:	Resets principal values to default
void TCylinder::GeneralReset()
{
	sint i;

	TBaseShape::GeneralReset();

	Start_DrawRangeElements_BodyOfCylinder=Start_DrawRangeElements_TopOfCylinder=Start_DrawRangeElements_BottomOfCylinder=0;
	End_DrawRangeElements_BodyOfCylinder=End_DrawRangeElements_TopOfCylinder=End_DrawRangeElements_BottomOfCylinder=0;

	StartIndexIn_pVertex_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder=0;
	pVertex_BodyOfCylinder=pVertex_TopOfCylinder=pVertex_BottomOfCylinder=NULL;
	StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder=0;
	StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder=0;
	TotalVertex_BodyOfCylinder=TotalVertex_TopOfCylinder=TotalVertex_BottomOfCylinder=0;

	StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder=0;
	StartIndexIn_pIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder=0;
	StartIndexIn_pIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder=0;

	pIndex16Bit_BodyOfCylinder=pIndex16Bit_TopOfCylinder=pIndex16Bit_BottomOfCylinder=NULL;
	pIndex32Bit_BodyOfCylinder=pIndex32Bit_TopOfCylinder=pIndex32Bit_BottomOfCylinder=NULL;
	TotalIndex_BodyOfCylinder=TotalIndex_TopOfCylinder=TotalIndex_BottomOfCylinder=0;

	StartIndexIn_pNormals_BodyOfCylinder=EndIndexIn_pNormals_BodyOfCylinder=0;
	StartIndexIn_pNormals_TopOfCylinder=EndIndexIn_pNormals_TopOfCylinder=0;
	StartIndexIn_pNormals_BottomOfCylinder=EndIndexIn_pNormals_BottomOfCylinder=0;
	pNormals_BodyOfCylinder=pNormals_TopOfCylinder=pNormals_BottomOfCylinder=NULL;
	TotalNormals_BodyOfCylinder=TotalNormals_TopOfCylinder=TotalNormals_BottomOfCylinder=0;


	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		StartIndexIn_pTexCoords_BodyOfCylinder[i]=EndIndexIn_pTexCoords_BodyOfCylinder[i]=0;
		StartIndexIn_pTexCoords_TopOfCylinder[i]=EndIndexIn_pTexCoords_TopOfCylinder[i]=0;
		StartIndexIn_pTexCoords_BottomOfCylinder[i]=EndIndexIn_pTexCoords_BottomOfCylinder[i]=0;
		pTexCoords_BodyOfCylinder[i]=pTexCoords_TopOfCylinder[i]=pTexCoords_BottomOfCylinder[i]=NULL;
		TotalTexCoords_BodyOfCylinder[i]=TotalTexCoords_TopOfCylinder[i]=TotalTexCoords_BottomOfCylinder[i]=0;

		TextureID_BodyOfCylinder[i]=TextureID_TopOfCylinder[i]=TextureID_BottomOfCylinder[i]=0;
	}

	StartIndexIn_pColors_BodyOfCylinder=EndIndexIn_pColors_BodyOfCylinder=0;
	StartIndexIn_pColors_TopOfCylinder=EndIndexIn_pColors_TopOfCylinder=0;
	StartIndexIn_pColors_BottomOfCylinder=EndIndexIn_pColors_BottomOfCylinder=0;
	pColors_BodyOfCylinder=pColors_TopOfCylinder=pColors_BottomOfCylinder=NULL;
	TotalColors_BodyOfCylinder=TotalColors_TopOfCylinder=TotalColors_BottomOfCylinder=0;

	StartIndexIn_pSecondaryColors_BodyOfCylinder=EndIndexIn_pSecondaryColors_BodyOfCylinder=0;
	StartIndexIn_pSecondaryColors_TopOfCylinder=EndIndexIn_pSecondaryColors_TopOfCylinder=0;
	StartIndexIn_pSecondaryColors_BottomOfCylinder=EndIndexIn_pSecondaryColors_BottomOfCylinder=0;
	pSecondaryColors_BodyOfCylinder=pSecondaryColors_TopOfCylinder=pSecondaryColors_BottomOfCylinder=NULL;
	TotalSecondaryColors_BodyOfCylinder=TotalSecondaryColors_TopOfCylinder=TotalSecondaryColors_BottomOfCylinder=0;

	StartIndexIn_pTangents_BodyOfCylinder=EndIndexIn_pTangents_BodyOfCylinder=0;
	StartIndexIn_pTangents_TopOfCylinder=EndIndexIn_pTangents_TopOfCylinder=0;
	StartIndexIn_pTangents_BottomOfCylinder=EndIndexIn_pTangents_BottomOfCylinder=0;
	pTangents_BodyOfCylinder=pTangents_TopOfCylinder=pTangents_BottomOfCylinder=NULL;
	TotalTangents_BodyOfCylinder=TotalTangents_TopOfCylinder=TotalTangents_BottomOfCylinder=0;

	StartIndexIn_pBinormals_BodyOfCylinder=EndIndexIn_pBinormals_BodyOfCylinder=0;
	StartIndexIn_pBinormals_TopOfCylinder=EndIndexIn_pBinormals_TopOfCylinder=0;
	StartIndexIn_pBinormals_BottomOfCylinder=EndIndexIn_pBinormals_BottomOfCylinder=0;
	pBinormals_BodyOfCylinder=pBinormals_TopOfCylinder=pBinormals_BottomOfCylinder=NULL;
	TotalBinormals_BodyOfCylinder=TotalBinormals_TopOfCylinder=TotalBinormals_BottomOfCylinder=0;

	StartIndexIn_pVisibleNormals_BodyOfCylinder=EndIndexIn_pVisibleNormals_BodyOfCylinder=0;
	StartIndexIn_pVisibleNormals_TopOfCylinder=EndIndexIn_pVisibleNormals_TopOfCylinder=0;
	StartIndexIn_pVisibleNormals_BottomOfCylinder=EndIndexIn_pVisibleNormals_BottomOfCylinder=0;
	pVisibleNormals_BodyOfCylinder=pVisibleNormals_TopOfCylinder=pVisibleNormals_BottomOfCylinder=NULL;
	TotalVisibleNormals_BodyOfCylinder=TotalVisibleNormals_TopOfCylinder=TotalVisibleNormals_BottomOfCylinder=0;

	TexCoordStyle=0;
}

//PURPOSE:	Deallocate all reserved memory
void TCylinder::DeallocateAllMemory()
{
	TBaseShape::DeallocateAllMemory();

	GeneralReset();
}


//PURPOSE:	Must call this at startup to create the cylinder.
//			The cylinder class is special in the sense that the top and bottom are considered separate
//			and can have different textures applied in various orientations.
//			This computes vertices by default. Everything else like normals, texcoords, vertex color, tangents, binormals
//			must be generated separately.
//			Polygon number is Stacks * Slices (for body) * 2 + 2 * (Slices) (for top and botttom)
//			Each flat face is made of two triangles and we count each triangle.
//
//PARAMETERS:
//			[in] isThereATop : If TRUE, it will be generated and rendered, else not.
//			[in] isThereABottom : If TRUE, it will be generated and rendered, else not.
//			[in] radius : >0.0, else default is 1.0
//			[in] height : >0.0, else default is 1.0
//			[in] polygonNumber : >=12, else default is 12.
//
//RETURNS:
//			1 for success
//			 0 for memory error
sint TCylinder::StartupByPolygon(tbool isThereATop, tbool isThereABottom, sreal radius, sreal height, sint polygonNumber)
{
	if(polygonNumber<12)								//If below 12, not enough polygons to make a cylinder
		polygonNumber=12;

	Slices=(sint)sqrtf((sreal)polygonNumber);
	if(Slices<3) Slices=3;
	Stacks=Slices-2;
	if(Stacks<1) Stacks=1;
	SlicesPlusOne=Slices+1;
	PolygonNumber=(Stacks*Slices+Slices)*2;

	ARadius=(radius<=0.0) ? 1.0 : radius;

	BRadius=ARadius;
	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	sint returnVal=SameStartupPatternForCylinder(height);
	return returnVal;
}

//PURPOSE:	Must call this at startup to create the cylinder.
//			The cylinder class is special in the sense that the top and bottom are considered separate
//			and can have different textures applied in various orientations.
//			This computes vertices by default. Everything else like normals, texcoords, vertex color, tangents, binormals
//			must be generated separately.
//			Polygon number is Stacks * Slices (for body) * 2 + 2 * (Slices) (for top and botttom)
//			Each flat face is made of two triangles and we count each triangle.
//
//PARAMRETERS:
//			[in] isThereATop : If TRUE, it will be generated and rendered, else not.
//			[in] isThereABottom : If TRUE, it will be generated and rendered, else not.
//			[in] radius : >0.0, else default is 1.0
//			[in] height : >0.0, else default is 1.0
//			[in] stacks : >=1, else default is 1
//			[in] slices : >=3, else default is 3
//
//RETURNS:
//			1 for success
//			0 for memory error.
sint TCylinder::StartupByStacksAndSlices(tbool isThereATop, tbool isThereABottom, sreal radius, sreal height, sint stacks, sint slices)
{
	Slices=(slices<3) ? 3 : slices;

	SlicesPlusOne=Slices+1;
	Stacks=(stacks<1) ? 1 : stacks;

	PolygonNumber=(Stacks*Slices+Slices)*2;
	ARadius=(radius<=0.0) ? 1.0 : radius;

	BRadius=ARadius;
	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	sint returnVal=SameStartupPatternForCylinder(height);
	return returnVal;
}

//PURPOSE:	Must call this at startup to create the cylinder.
//			The cylinder class is special in the sense that the top and bottom are considered separate
//			and can have different textures applied in various orientations.
//			This computes vertices by default. Everything else like normals, texcoords, vertex color, tangents, binormals
//			must be generated separately.
//			Polygon number is Stacks * Slices (for body) * 2 + 2 * (Slices) (for top and botttom)
//			Each flat face is made of two triangles and we count each triangle.
//
//PARAMRETERS:
//			[in] isThereATop : If TRUE, it will be generated and rendered, else not.
//			[in] isThereABottom : If TRUE, it will be generated and rendered, else not.
//			[in] aRadius : >0.0, else default is 1.0. The top radius.
//			[in] bRadius : >0.0, else default is 1.0. The bottom radius.
//			[in] height : >0.0, else default is 1.0
//			[in] polygonNumber : >=12, else default is 12.
//
//RETURNS:
//			1 for success
//			0 for memory error
sint TCylinder::StartupByTwoRadiusPolygon(tbool isThereATop, tbool isThereABottom, sreal aRadius,
										  sreal bRadius, sreal height, sint polygonNumber)
{
	if(polygonNumber<12)								//If below 12, not enough polygons to make a cylinder
		polygonNumber=12;

	Slices=(sint)sqrtf((sreal)polygonNumber);
	if(Slices<3) Slices=3;
	Stacks=Slices-2;
	if(Stacks<1) Stacks=1;
	SlicesPlusOne=Slices+1;
	PolygonNumber=(Stacks*Slices+Slices)*2;

	ARadius=(aRadius<=0.0) ? 1.0 : aRadius;
	BRadius=(bRadius<=0.0) ? 1.0 : bRadius;

	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	sint returnVal=SameStartupPatternForCylinder(height);
	return returnVal;
}

//PURPOSE:	Must call this at startup to create the cylinder.
//			The cylinder class is special in the sense that the top and bottom are considered separate
//			and can have different textures applied in various orientations.
//			This computes vertices by default. Everything else like normals, texcoords, vertex color, tangents, binormals
//			must be generated separately.
//			Polygon number is Stacks * Slices (for body) * 2 + 2 * (Slices) (for top and botttom)
//			Each flat face is made of two triangles and we count each triangle.
//
//PARAMRETERS:
//			[in] isThereATop : If TRUE, it will be generated and rendered, else not.
//			[in] isThereABottom : If TRUE, it will be generated and rendered, else not.
//			[in] aRadius : >0.0, else default is 1.0. The top radius.
//			[in] bRadius : >0.0, else default is 1.0. The bottom radius.
//			[in] height : >0.0, else default is 1.0
//			[in] stacks : >=1, else default is 1
//			[in] slices : >=3, else default is 3
//
//RETURNS:
//			1 for success
//			0 for memory error
sint TCylinder::StartupByTwoRadiusStacksAndSlices(tbool isThereATop, tbool isThereABottom, sreal aRadius,
										  sreal bRadius, sreal height, sint stacks, sint slices)
{
	Slices=(slices<3) ? 3 : slices;

	SlicesPlusOne=Slices+1;
	Stacks=(stacks<1) ? 1 : stacks;

	PolygonNumber=(Stacks*Slices+Slices)*2;

	ARadius=(aRadius<=0.0) ? 1.0 : aRadius;
	BRadius=(bRadius<=0.0) ? 1.0 : bRadius;

	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	sint returnVal=SameStartupPatternForCylinder(height);
	return returnVal;
}

//PURPOSE:	Must call this at startup to create the cylinder.
//			The cylinder class is special in the sense that the top and bottom are considered separate
//			and can have different textures applied in various orientations.
//			This computes vertices by default. Everything else like normals, texcoords, vertex color, tangents, binormals
//			must be generated separately.
//			Polygon number is Stacks * Slices (for body) * 2 + 2 * (Slices) (for top and botttom)
//			Each flat face is made of two triangles and we count each triangle.
//
//PARAMETERS:
//			[in] indexFormat :  one of the defines in glhlib.h
//			[in] vertexFormat : one of the defines in glhlib.h
//			                    GLH_VERTEXFORMAT_VNTT3T3, T3 receives tangent and the other T3 receives binormal
//			[in] isThereATop : If TRUE, it will be generated and rendered, else not.
//			[in] isThereABottom : If TRUE, it will be generated and rendered, else not.
//			[in] aRadius : >0.0, else default is 1.0. The top radius.
//			[in] bRadius : >0.0, else default is 1.0. The bottom radius.
//			[in] height : >0.0, else default is 1.0
//			[in] stacks : >=1, else default is 1
//			[in] slices : >=3, else default is 3
//			[in] texCoordStyle : 0 for nothing to be written
//			                     1 for standard texture mapping
//			[in] scaleFactorS : to scale the texcoords
//			[in] scaleFactorT : to scale the texcoords
//
//RETURNS:
//			1 for success
//			-1 for error message
//			for tangent and binormal generation
sint TCylinder::StartupByTwoRadiusStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
										tbool isThereATop, tbool isThereABottom, sreal aRadius,
										sreal bRadius, sreal height, sint stacks, sint slices, sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT)
{
	sint i;

	IndexFormat=indexFormat;
	VertexFormat=vertexFormat;

	Slices=(slices<3) ? 3 : slices;

	SlicesPlusOne=Slices+1;
	Stacks=(stacks<1) ? 1 : stacks;

	PolygonNumber=(Stacks*Slices+Slices)*2;			//Triangle count

	ARadius=(aRadius<=0.0) ? 1.0 : aRadius;
	BRadius=(bRadius<=0.0) ? 1.0 : bRadius;

	IsThereATop=isThereATop;
	IsThereABottom=isThereABottom;

	if((texCoordStyle!=0)&&(texCoordStyle!=1))
	{
		strcpy(ErrorMessage, "texCoordStyle is not 0 or 1 @ Startup 375");
		return -1;
	}

	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents and binormals @ Startup 386");
		return -2;
	}
	if((texCoordStyle==0)&&(VertexFormat==GLH_VERTEXFORMAT_VNTT3))
	{
		strcpy(ErrorMessage, "Texcoords are required to generate tangents @ Startup 391");
		return -2;
	}

	TexCoordStyle=texCoordStyle;
	ScaleFactorS[0]=scaleFactorS;
	ScaleFactorT[0]=scaleFactorT;

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

	sint returnVal=SameStartupPatternForCylinder2(height);
	return returnVal;
}

//PURPOSE:	To reduce the number of lines. Startup functions are rarely called.
sint TCylinder::SameStartupPatternForCylinder(sreal height)
{
	if(height<=0.0)
		Height=1.0;
	else
		Height=height;

	sint returnVal=GenerateVertices();
	return returnVal;
}

//PURPOSE:	To reduce the number of lines. Startup functions are rarely called.
sint TCylinder::SameStartupPatternForCylinder2(sreal height)
{
	if(height<=0.0)
		Height=1.0;
	else
		Height=height;

	sint returnVal=GenerateVertices2();
	return returnVal;
}

//PURPOSE:	If there won't a top or bottom, then those won't be computed so some memory will be saved.
//
//RETURN:
//			Always 1 for success
sint TCylinder::GenerateVertices()
{
	sint i, tempo, tempo3, tempo5, tempo6, tempo7;
	sreal FTempo, FTempo2, phi;
	sreal radialIncrement;
	sint threeTimesSlicesLessThree;
	sreal heightIncrement;
	sreal phiIncrement;

	heightIncrement=Height/(sreal)Stacks;
	phiIncrement=(2.0*PI_DEF)/(sreal)Slices;

	UsedVertex=0;
	UsedIndex=0;


	//DeallocateMemoryForVertex();

	tempo=NumberOfComponentsForVertex*(SlicesPlusOne*(Stacks+1));		//3* because (x, y, z), Stacks+1 for the last circle on the bottom of cylinder


	pVertex_BodyOfCylinder=pVertex;

	StartIndexIn_pVertex_BodyOfCylinder=0;
	EndIndexIn_pVertex_BodyOfCylinder=tempo;
	Start_DrawRangeElements_BodyOfCylinder=0;
	End_DrawRangeElements_BodyOfCylinder=tempo/NumberOfComponentsForVertex-1;
	TotalVertex_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder-StartIndexIn_pVertex_BodyOfCylinder;

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

	tempo7=tempo;
	tempo=1;

	if((IsThereATop)&&(IsThereABottom))
	{
		tempo5=NumberOfComponentsForVertex*(SlicesPlusOne*tempo+1);
		tempo6=2*tempo5;													//+1 because of center point, *2 because top and bottom faces of cylinder, *3 because of (x, y, z)
	}
	else	//One or the other
		tempo5=tempo6=NumberOfComponentsForVertex*(SlicesPlusOne*tempo+1);				//+1 because of center point, *3 because of (x, y, z)

	AllocateExact_1(pVertex, &UsedVertex, tempo7+tempo6);


	/*//This is to figure out the top and bottom pVertex number based on the radius and PhiIncrement
	//the following equation has been simplified!!! It is for figuring out the length of an "edge" segment
	FTempo=(sreal)sqrtf((sreal)((ARadius*cosf(PhiIncrement)-ARadius)*(ARadius*cosf(PhiIncrement)-ARadius)+ARadius*sinf(PhiIncrement)*ARadius*sinf(PhiIncrement)));
	//This is the radial division number
	if(FTempo!=0.0)
	{
		tempo=(sint)(ARadius/(sreal)FTempo);
		if(tempo==0)
			tempo=1;
	}
	else
		tempo=1;*/


	radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From ARadius to BRadius

	if((IsThereATop)||(IsThereABottom))
	{
		if((IsThereATop)&&(IsThereABottom))
		{
			tempo5=NumberOfComponentsForVertex*(SlicesPlusOne*tempo+1);
			tempo6=2*tempo5;													//+1 because of center point, *2 because top and bottom faces of cylinder, *3 because of (x, y, z)
		}
		else	//One or the other
			tempo5=tempo6=NumberOfComponentsForVertex*(SlicesPlusOne*tempo+1);				//+1 because of center point, *3 because of (x, y, z)


		pVertex_BodyOfCylinder=pVertex;			//Refresh pointer if address changed

		if(IsThereATop)
		{
			pVertex_TopOfCylinder=&pVertex[EndIndexIn_pVertex_BodyOfCylinder];
			StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
			EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
			TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

			Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex;
			End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex-1;

			if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=&pVertex[EndIndexIn_pVertex_TopOfCylinder];
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex-1;
			}
		}
		else //if(IsThereABottom)
		{
			pVertex_BottomOfCylinder=&pVertex[EndIndexIn_pVertex_BodyOfCylinder];
			StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
			EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
			TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

			Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex;
			End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex-1;
		}
	}


	/*if(Position==POSITION_BOTTOM)
		FTempo2=0.0;
	else if(Position==POSITION_CENTER)
		FTempo2=-Height*0.5;
	else if(Position==POSITION_TOP)
		FTempo2=-Height;*/

	FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

	threeTimesSlicesLessThree=NumberOfComponentsForVertex*Slices;

	//*********** THIS IS FOR BODY OF CYLINDER
	if(NumberOfComponentsForVertex==3)
	{
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne*NumberOfComponentsForVertex;
		for(i=0, tempo=2; i<tempo3; i+=3)
		{
			pVertex[i  ]=FTempo*(sreal)cosf(phi);
			pVertex[i+1]=FTempo*(sreal)sinf(phi);
			pVertex[i+2]=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i+=3;
				pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
				pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
				pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
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
		FTempo=BRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_BodyOfCylinder; i+=3)
		{
			pVertex[i  ]=FTempo*(sreal)cosf(phi);
			pVertex[i+1]=FTempo*(sreal)sinf(phi);
			pVertex[i+2]=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i+=3;
				pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
				pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
				pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
	}
	else if(NumberOfComponentsForVertex==4)
	{
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne*NumberOfComponentsForVertex;
		for(i=0, tempo=2; i<tempo3; i+=4)
		{
			pVertex[i  ]=FTempo*(sreal)cosf(phi);
			pVertex[i+1]=FTempo*(sreal)sinf(phi);
			pVertex[i+2]=FTempo2;
			pVertex[i+3]=1.0;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i+=3;
				pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
				pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
				pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
				pVertex[i+3]=1.0;
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
		FTempo=BRadius;
		FTempo2=-Height*0.5;
		for(; i<TotalVertex_BodyOfCylinder; i+=4)
		{
			pVertex[i  ]=FTempo*(sreal)cosf(phi);
			pVertex[i+1]=FTempo*(sreal)sinf(phi);
			pVertex[i+2]=FTempo2;
			pVertex[i+3]=1.0;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i+=3;
				pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
				pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
				pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
				pVertex[i+3]=1.0;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}
	}

	//*********** END ***************
	//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
	if(IsThereATop)
	{
		/*if(Position==POSITION_BOTTOM)
			FTempo2=0.0;
		else if(Position==POSITION_CENTER)
			FTempo2=-Height*0.5;
		else if(Position==POSITION_TOP)
			FTempo2=-Height;*/

		FTempo2=Height*0.5;		//POSITION_CENTER

		if(NumberOfComponentsForVertex==3)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex[i  ]=0.0;				//This is the center point at top
			pVertex[i+1]=0.0;
			pVertex[i+2]=FTempo2;
			FTempo=ARadius;
			phi=0.0;

			for(i+=3, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i+=3)
			{
				pVertex[i  ]=FTempo*(sreal)cosf(phi);
				pVertex[i+1]=FTempo*(sreal)sinf(phi);
				pVertex[i+2]=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i+=3;
					pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
					pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
					pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
					tempo=2;
					phi=0.0;
				}
				else
				{
					tempo++;
					phi+=phiIncrement;
				}
			}
		}
		else if(NumberOfComponentsForVertex==4)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex[i  ]=0.0;				//This is the center point at top
			pVertex[i+1]=0.0;
			pVertex[i+2]=FTempo2;
			pVertex[i+3]=1.0;
			FTempo=ARadius;
			phi=0.0;

			for(i+=4, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i+=4)
			{
				pVertex[i  ]=FTempo*(sreal)cosf(phi);
				pVertex[i+1]=FTempo*(sreal)sinf(phi);
				pVertex[i+2]=FTempo2;
				pVertex[i+3]=1.0;
				if(tempo==SlicesPlusOne)
				{
					i+=4;
					pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
					pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
					pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
					pVertex[i+3]=1.0;
					tempo=2;
					phi=0.0;
				}
				else
				{
					tempo++;
					phi+=phiIncrement;
				}
			}
		}
	}
	//*********** END ***************
	//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
	if(IsThereABottom)
	{
		//Allocate space for the top

		/*if(Position==POSITION_BOTTOM)
			FTempo2=0.0;
		else if(Position==POSITION_CENTER)
			FTempo2=-Height*0.5;
		else if(Position==POSITION_TOP)
			FTempo2=-Height;*/

		FTempo2=-Height*0.5;		//POSITION_CENTER


		if(NumberOfComponentsForVertex==3)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex[i  ]=0.0;				//This is the center point at bottom
			pVertex[i+1]=0.0;
			pVertex[i+2]=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i+=3, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i+=3)
			{
				pVertex[i  ]=FTempo*(sreal)cosf(phi);
				pVertex[i+1]=FTempo*(sreal)sinf(phi);
				pVertex[i+2]=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i+=3;
					pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
					pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
					pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
					tempo=2;
					phi=0.0;
				}
				else
				{
					tempo++;
					phi-=phiIncrement;
				}
			}
		}
		else if(NumberOfComponentsForVertex==4)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex[i  ]=0.0;				//This is the center point at bottom
			pVertex[i+1]=0.0;
			pVertex[i+2]=FTempo2;
			pVertex[i+3]=1.0;
			FTempo=BRadius;
			phi=0.0;
			for(i+=4, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i+=4)
			{
				pVertex[i  ]=FTempo*(sreal)cosf(phi);
				pVertex[i+1]=FTempo*(sreal)sinf(phi);
				pVertex[i+2]=FTempo2;
				pVertex[i+3]=1.0;
				if(tempo==SlicesPlusOne)
				{
					i+=4;
					pVertex[i  ]=pVertex[i-threeTimesSlicesLessThree  ];
					pVertex[i+1]=pVertex[i-threeTimesSlicesLessThree+1];
					pVertex[i+2]=pVertex[i-threeTimesSlicesLessThree+2];
					pVertex[i+3]=1.0;
					tempo=2;
					phi=0.0;
				}
				else
				{
					tempo++;
					phi-=phiIncrement;
				}
			}
		}
	}
	//********** END ******************

	GenerateIndices();


	return 1;
}

//PURPOSE:	The indices will be generated whether vertices exist or not.
//
//RETURN:
//			1 for success
//			-1 if 16 bit indices are not enough, so switching to 32 bit.
sint TCylinder::GenerateIndices()
{
	sint i, j, tempo, tempo2, tempo4;
	tbool switchOccured=FALSE;
	ushort indexCount;
	uint indexCount2;
	ushort centerVertexIndex;
	uint centerVertexIndex2;
	sint vertexCount;

	//This is just for the body for now, later it is increased
	vertexCount=(SlicesPlusOne*(Stacks+1));		//Stacks+1 for the last circle on the bottom of cylinder
	if(IsThereATop)
		vertexCount+=SlicesPlusOne+1;
	if(IsThereABottom)
		vertexCount+=SlicesPlusOne+1;

	DeallocateMemoryForIndex();

	StartIndexIn_pIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder=0;
	pIndex16Bit_BodyOfCylinder=NULL;
	StartIndexIn_pIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder=0;
	pIndex16Bit_TopOfCylinder=NULL;
	StartIndexIn_pIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder=0;
	pIndex16Bit_BottomOfCylinder=NULL;
	TotalIndex_BodyOfCylinder=TotalIndex_TopOfCylinder=TotalIndex_BottomOfCylinder=0;



	if((vertexCount>65535)&&(IndexFormat==GLH_INDEXFORMAT_16BIT))
	{
		//strcpy(WarningMessage, "Vertex count larger than 65535. Switch to 32 bit indices. @ GenerateIndices 660");
		switchOccured=TRUE;
		IndexFormat=GLH_INDEXFORMAT_32BIT;
	}

	tempo=Stacks*Slices*6;
	if(IsThereATop)
		tempo+=Slices*3;
	if(IsThereABottom)
		tempo+=Slices*3;

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		indexCount=0;

		AllocateExact_1(pIndex16Bit, &UsedIndex, tempo);

		tempo=Stacks*Slices*6;

		pIndex16Bit_BodyOfCylinder=pIndex16Bit;
		StartIndexIn_pIndex_BodyOfCylinder=0;
		EndIndexIn_pIndex_BodyOfCylinder=tempo;
		TotalIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder-StartIndexIn_pIndex_BodyOfCylinder;


		tempo4=0;
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


		if(IsThereATop)
		{
			//One vertex at the center, and vertices at each circle.
			//Slices is number of triangles, *3 for number of indices
			tempo=Stacks*Slices*6;
			StartIndexIn_pIndex_TopOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_TopOfCylinder=tempo2;
			TotalIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder-StartIndexIn_pIndex_TopOfCylinder;
			pIndex16Bit_TopOfCylinder=&pIndex16Bit[StartIndexIn_pIndex_TopOfCylinder];

			centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+1));
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
			tempo=Stacks*Slices*6;
			if(IsThereATop)
				tempo+=Slices*3;

			StartIndexIn_pIndex_BottomOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_BottomOfCylinder=tempo2;
			TotalIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder-StartIndexIn_pIndex_BottomOfCylinder;
			pIndex16Bit_BottomOfCylinder=&pIndex16Bit[StartIndexIn_pIndex_BottomOfCylinder];

			if(IsThereATop)
				centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+1)+SlicesPlusOne+1);
			else
				centerVertexIndex=(ushort)(SlicesPlusOne*(Stacks+1));
			indexCount=centerVertexIndex+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex16Bit[i  ]=centerVertexIndex;
				pIndex16Bit[i+1]=indexCount;
				pIndex16Bit[i+2]=++indexCount;
			}
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		indexCount2=0;

		AllocateExact_1(pIndex32Bit, &UsedIndex, tempo);

		tempo=Stacks*Slices*6;

		pIndex32Bit_BodyOfCylinder=pIndex32Bit;
		StartIndexIn_pIndex_BodyOfCylinder=0;
		EndIndexIn_pIndex_BodyOfCylinder=tempo;
		TotalIndex_BodyOfCylinder=EndIndexIn_pIndex_BodyOfCylinder-StartIndexIn_pIndex_BodyOfCylinder;


		tempo4=0;
		for(i=0; i<Stacks; i++)
		{
			for(j=0; j<Slices; j++)
			{
				//Each face on the cylinder 2 triangles = a quad
				pIndex32Bit[tempo4  ]=indexCount2+(ushort)SlicesPlusOne;
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
			tempo=Stacks*Slices*6;
			StartIndexIn_pIndex_TopOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_TopOfCylinder=tempo2;
			TotalIndex_TopOfCylinder=EndIndexIn_pIndex_TopOfCylinder-StartIndexIn_pIndex_TopOfCylinder;
			pIndex32Bit_TopOfCylinder=&pIndex32Bit[StartIndexIn_pIndex_TopOfCylinder];

			centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+1));
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
			tempo=Stacks*Slices*6;
			if(IsThereATop)
				tempo=tempo+Slices*3;

			StartIndexIn_pIndex_BottomOfCylinder=tempo;
			tempo2=tempo+Slices*3;
			EndIndexIn_pIndex_BottomOfCylinder=tempo2;
			TotalIndex_BottomOfCylinder=EndIndexIn_pIndex_BottomOfCylinder-StartIndexIn_pIndex_BottomOfCylinder;
			pIndex32Bit_BottomOfCylinder=&pIndex32Bit[StartIndexIn_pIndex_BottomOfCylinder];

			if(IsThereATop)
				centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+1)+SlicesPlusOne+1);
			else
				centerVertexIndex2=(uint)(SlicesPlusOne*(Stacks+1));
			indexCount2=centerVertexIndex2+1;

			for(i=tempo; i<tempo2; i+=3)		//i+=3 because one triangle added
			{
				pIndex32Bit[i  ]=centerVertexIndex2;
				pIndex32Bit[i+1]=indexCount2;
				pIndex32Bit[i+2]=++indexCount2;
			}
		}
	}

	if(switchOccured==FALSE)
		return 1;
	else
		return -1;
}


//PURPOSE:	Generate the appropriate normal vectors. The cylinder must be initialized first before
//			calling this. The normals will be smoothed over the body area of the cylinder. The top and bottom
//			of the cylinder are a different smoothing group.
//			One normal per vertex.
//			Number of components is 3 (xyz).
//
//PARAMETERS:
//
//RETURNS:
//			1 for success
//			-1 plus message if no vertices generated
sint TCylinder::GenerateNormals()
{
	sint i, j, tempo, tempo2;

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. Can't compute normal. @ GenerateNormals 1319");
		return -1;
	}

	UsedNormals=0;
	//DeallocateMemoryForNormals();

	StartIndexIn_pNormals_BodyOfCylinder=EndIndexIn_pNormals_BodyOfCylinder=0;
	StartIndexIn_pNormals_TopOfCylinder=EndIndexIn_pNormals_TopOfCylinder=0;
	StartIndexIn_pNormals_BottomOfCylinder=EndIndexIn_pNormals_BottomOfCylinder=0;
	pNormals_BodyOfCylinder=pNormals_TopOfCylinder=pNormals_BottomOfCylinder=NULL;
	TotalNormals_BodyOfCylinder=TotalNormals_TopOfCylinder=TotalNormals_BottomOfCylinder=0;


	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
	AllocateExact_1(pNormals, &UsedNormals, tempo);

	UsedNormals=tempo;

	if(NumberOfComponentsForVertex==3)
	{
		//**** Start with the body of cylinder. Vertex - center
		tempo2=TotalVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

		StartIndexIn_pNormals_BodyOfCylinder=0;
		EndIndexIn_pNormals_BodyOfCylinder=tempo2;
		TotalNormals_BodyOfCylinder=EndIndexIn_pNormals_BodyOfCylinder-StartIndexIn_pNormals_BodyOfCylinder;
		pNormals_BodyOfCylinder=pNormals;

		for(i=0; i<tempo2; i+=3)
		{
			pNormals[i  ]=pVertex[i  ];
			pNormals[i+1]=pVertex[i+1];
			pNormals[i+2]=0.0;
			NormalizeVectorFLOAT_2(&pNormals[i]);
		}


		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
			tempo2=EndIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

			StartIndexIn_pNormals_TopOfCylinder=i;
			EndIndexIn_pNormals_TopOfCylinder=tempo2;
			TotalNormals_TopOfCylinder=EndIndexIn_pNormals_TopOfCylinder-StartIndexIn_pNormals_TopOfCylinder;
			pNormals_TopOfCylinder=&pNormals[StartIndexIn_pNormals_TopOfCylinder];

			ComputeNormalOfPlaneFLOAT_3(&pNormals[i], &pVertex_TopOfCylinder[3], &pVertex_TopOfCylinder[6], &pVertex_TopOfCylinder[0]);
			NormalizeVectorFLOAT_2(&pNormals[i]);
			
			j=i;
			for(i+=3; i<tempo2; i+=3)
			{
				pNormals[i  ]=pNormals[j  ];
				pNormals[i+1]=pNormals[j+1];
				pNormals[i+2]=pNormals[j+2];
			}
		}
		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

			StartIndexIn_pNormals_BottomOfCylinder=i;
			EndIndexIn_pNormals_BottomOfCylinder=tempo2;
			TotalNormals_BottomOfCylinder=EndIndexIn_pNormals_BottomOfCylinder-StartIndexIn_pNormals_BottomOfCylinder;
			pNormals_BottomOfCylinder=&pNormals[StartIndexIn_pNormals_BottomOfCylinder];

			ComputeNormalOfPlaneFLOAT_3(&pNormals[i], &pVertex_BottomOfCylinder[3], &pVertex_BottomOfCylinder[6], &pVertex_BottomOfCylinder[0]);
			NormalizeVectorFLOAT_2(&pNormals[i]);
			
			j=i;
			for(i+=3; i<tempo2; i+=3)
			{
				pNormals[i  ]=pNormals[j  ];
				pNormals[i+1]=pNormals[j+1];
				pNormals[i+2]=pNormals[j+2];
			}
		}
	}
	else if(NumberOfComponentsForVertex==4)
	{
		//**** Start with the body of cylinder. Vertex - center
		tempo2=TotalVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

		StartIndexIn_pNormals_BodyOfCylinder=0;
		EndIndexIn_pNormals_BodyOfCylinder=tempo2;
		TotalNormals_BodyOfCylinder=EndIndexIn_pNormals_BodyOfCylinder-StartIndexIn_pNormals_BodyOfCylinder;
		pNormals_BodyOfCylinder=pNormals;

		for(i=0; i<tempo2; i+=4)
		{
			pNormals[i  ]=pVertex[i  ];
			pNormals[i+1]=pVertex[i+1];
			pNormals[i+2]=0.0;
			pNormals[i+3]=0.0;
			NormalizeVectorFLOAT_2(&pNormals[i]);
		}


		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
			tempo2=EndIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

			StartIndexIn_pNormals_TopOfCylinder=i;
			EndIndexIn_pNormals_TopOfCylinder=tempo2;
			TotalNormals_TopOfCylinder=EndIndexIn_pNormals_TopOfCylinder-StartIndexIn_pNormals_TopOfCylinder;
			pNormals_TopOfCylinder=&pNormals[StartIndexIn_pNormals_TopOfCylinder];

			ComputeNormalOfPlaneFLOAT_3(&pNormals[i], &pVertex_TopOfCylinder[3], &pVertex_TopOfCylinder[6], &pVertex_TopOfCylinder[0]);
			NormalizeVectorFLOAT_2(&pNormals[i]);
			
			j=i;
			for(i+=4; i<tempo2; i+=4)
			{
				pNormals[i  ]=pNormals[j  ];
				pNormals[i+1]=pNormals[j+1];
				pNormals[i+2]=pNormals[j+2];
				pNormals[i+3]=0.0;
			}
		}
		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForNormals;

			StartIndexIn_pNormals_BottomOfCylinder=i;
			EndIndexIn_pNormals_BottomOfCylinder=tempo2;
			TotalNormals_BottomOfCylinder=EndIndexIn_pNormals_BottomOfCylinder-StartIndexIn_pNormals_BottomOfCylinder;
			pNormals_BottomOfCylinder=&pNormals[StartIndexIn_pNormals_BottomOfCylinder];

			ComputeNormalOfPlaneFLOAT_3(&pNormals[i], &pVertex_BottomOfCylinder[3], &pVertex_BottomOfCylinder[6], &pVertex_BottomOfCylinder[0]);
			NormalizeVectorFLOAT_2(&pNormals[i]);
			
			j=i;
			for(i+=4; i<tempo2; i+=4)
			{
				pNormals[i  ]=pNormals[j  ];
				pNormals[i+1]=pNormals[j+1];
				pNormals[i+2]=pNormals[j+2];
				pNormals[i+3]=0.0;
			}
		}
	}

	return 1;
}

