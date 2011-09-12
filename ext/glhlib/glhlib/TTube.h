// TTube.h: interface for the TTube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTUBE_H__46ECE4E2_FE8A_453C_8E8F_9AE14E70026F__INCLUDED_)
#define AFX_TTUBE_H__46ECE4E2_FE8A_453C_8E8F_9AE14E70026F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000






//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'






//PURPOSE:	This class is capable of doing a basic tube (which is basicaly a hollowed out cylinder)
//			Surface is always smooth shaded, one normal per vertex.
//			The tube is split into four parts : outer body, inner body, top and bottom.
//			Can render entire tube, or without top and/or bottom or
//			just the outer tube or just the inner tube or just top or just bottom.
//			Can generate texture coordinates for OpenGL, and can render
//			textured in single shot, or can render body textured, top textured,
//			then bottom textured. This is so that different textures can be applied
//			to outer body, inner body, top and bottom. SEE THE VARIABLES FOR THE BODY, TOP AND BOTTOM
//			AND MAKE SURE YOU SET THEM UP. ALSO, THERE ARE NEW FUNCTIONS ADDED TO THIS CLASS.
class TTube : public TBaseShape
{
public:
	TTube();
	virtual ~TTube();

	sint StartupByTwoRadiusStacksAndSlices(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, tbool isThereATop, tbool isThereABottom,
		sreal outer_aRadius, sreal outer_bRadius, sreal inner_aRadius, sreal inner_bRadius,
		sreal height, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT);

	//**** For memory management - deallocators
	void DeallocateAllMemory();
protected:
	void GeneralReset();

public:
	sint GenerateIndices();

	sint GenerateVertices2();
	sint GenerateNormals2();
	sint GenerateTexCoords2();


	//Getter methods
	sint GetPolygonMinimum() { return 24;}
	sint GetStacksMinimum() { return 1;}
	sint GetSlicesMinimum() { return 3;}

	sint GetPolygonNumber() { return PolygonNumber;}

	sreal GetHeight() { return Height;}
	sreal GetAOuterRadius() { return AOuterRadius;}
	sreal GetBOuterRadius() { return BOuterRadius;}
	sreal GetAInnerRadius() { return AInnerRadius;}
	sreal GetBInnerRadius() { return BInnerRadius;}

	sint GetSlicesNumber() { return Slices;}
	sint GetStacksNumber() { return Stacks;}


	sint SameStartupPatternForTube(sreal *centerXYZ, sreal height);

	//**** Main variables for tube
	tbool	IsThereATop, IsThereABottom;							//Render or not?
	sreal	AOuterRadius, BOuterRadius;								//Top outer radius, bottom outer radius
	sreal	AInnerRadius, BInnerRadius;								//Top inner radius, bottom inner radius
	sreal	Height;
	sint	Stacks;													//Number of divisions in the Theta direction
	sint	Slices;													//This is the actual slice number
	sint	SlicesPlusOne;											//Number of divisions in the Phi direction


	//These 2 are for rendering the entire tube in one call to glDrawRangeElements
	//uint			Start_DrawRangeElements;						//Parameter to pass to glDrawRangeElements
	//uint			End_DrawRangeElements;							//Parameter to pass to glDrawRangeElements


	//TextureID
	uint TextureID_OuterBodyOfTube[NUMBEROFTEXTUREUNITS];
	uint TextureID_InnerBodyOfTube[NUMBEROFTEXTUREUNITS];
	uint TextureID_TopOfTube[NUMBEROFTEXTUREUNITS];
	uint TextureID_BottomOfTube[NUMBEROFTEXTUREUNITS];


	uint	Start_DrawRangeElements_OuterBodyOfTube;				//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_OuterBodyOfTube;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_InnerBodyOfTube;				//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_InnerBodyOfTube;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_TopOfTube;						//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_TopOfTube;						//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_BottomOfTube;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BottomOfTube;						//Parameter to pass to glDrawRangeElements

	//Vertex related
	sint	StartIndexIn_pVertex_OuterBodyOfTube, EndIndexIn_pVertex_OuterBodyOfTube;		//End-1 gives the last vertex for interlaced formats such as pVertex_VNT
	sint	StartIndexIn_pVertex_InnerBodyOfTube, EndIndexIn_pVertex_InnerBodyOfTube;		//End-1 gives the last vertex for interlaced formats such as pVertex_VNT
	sint	StartIndexIn_pVertex_TopOfTube, EndIndexIn_pVertex_TopOfTube;					//End-1 gives the last vertex
	sint	StartIndexIn_pVertex_BottomOfTube, EndIndexIn_pVertex_BottomOfTube;				//End-1 gives the last vertex

	sreal	*pVertex_OuterBodyOfTube;							//Pointer to pVertex where outer body starts
	sreal	*pVertex_InnerBodyOfTube;							//Pointer to pVertex where inner body starts
	sreal	*pVertex_TopOfTube;									//Pointer to pVertex where top starts
	sreal	*pVertex_BottomOfTube;								//Pointer to pVertex where bottom starts

	sint	TotalVertex_OuterBodyOfTube;
	sint	TotalVertex_InnerBodyOfTube;
	sint	TotalVertex_TopOfTube;
	sint	TotalVertex_BottomOfTube;

	//Index related
	sint	StartIndexIn_pIndex_OuterBodyOfTube, EndIndexIn_pIndex_OuterBodyOfTube;		//End-1 gives the last index
	sint	StartIndexIn_pIndex_InnerBodyOfTube, EndIndexIn_pIndex_InnerBodyOfTube;		//End-1 gives the last index
	sint	StartIndexIn_pIndex_TopOfTube, EndIndexIn_pIndex_TopOfTube;					//End-1 gives the last index
	sint	StartIndexIn_pIndex_BottomOfTube, EndIndexIn_pIndex_BottomOfTube;			//End-1 gives the last index

	ushort	*pIndex16Bit_OuterBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_InnerBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_TopOfTube;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_BottomOfTube;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.


	sint	TotalIndex_OuterBodyOfTube;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_InnerBodyOfTube;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_TopOfTube;									//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_BottomOfTube;								//Parameter to pass to glDrawRangeElements

	uint	*pIndex32Bit_OuterBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_InnerBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_TopOfTube;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_BottomOfTube;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
};

#endif // !defined(AFX_TTUBE_H__46ECE4E2_FE8A_453C_8E8F_9AE14E70026F__INCLUDED_)

//LOG:
//Monday, Mar 01, 2010
//Creating this class.
//



