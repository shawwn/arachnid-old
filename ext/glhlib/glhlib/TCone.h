// TCone.h: interface for the TCone class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCONE_H__CFB1828C_7CB9_431C_BB58_68147A24D136__INCLUDED_)
#define AFX_TCONE_H__CFB1828C_7CB9_431C_BB58_68147A24D136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'








//PURPOSE:	This class is capable of doing a basic cone.
//			If you want a cone with the top cut off, use class TCylinder since with
//			that class, you can have different radii for bottom and top.
//			Surface is always smooth shaded, one normal per vertex.
//			You can make a cone with no bottom.
//			Can generate texture coordinates for OpenGL, and can render
//			textured in single shot, or can render body textured, then bottom textured.
//			This is so that different textures can be applied for body and bottom.
//			SEE THE VARIABLES FOR THE BODY AND BOTTOM
//			AND MAKE SURE YOU SET THEM UP. ALSO, THERE ARE NEW FUNCTIONS ADDED TO THIS CLASS.
class TCone : public TBaseShape
{
public:
	TCone();
	virtual ~TCone();

	sint StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		tbool isThereABottom, sreal radius, sreal height, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT);

	//**** For memory management - deallocators
	void DeallocateAllMemory();

protected:
	void GeneralReset();

public:
	sint GenerateIndices();

	sint GenerateVertices2();
	sint GenerateNormals2();
	sint GenerateTexCoords2(tbool invertS, tbool invertT);

	sint GetPolygonMinimum() { return 9;}
	sint GetStacksMinimum() { return 1;}
	sint GetSlicesMinimum() { return 3;}
	sint GetStacksNumber() { return Stacks;}
	sint GetSlicesNumber() { return Slices;}
	sint GetPolygonNumber() { return PolygonNumber;}
	sreal GetRadius() { return Radius;}
	sreal GetHeight() { return Height;}

	tbool	IsThereABottom;

	sreal	Radius;
	sreal	Height;

	sint	Stacks;					//Division from top to bottom
	sint	Slices;					//The slices are like the slices of a cake.

	//For rendering the body and bottom of cone separately
	uint	Start_DrawRangeElements_BodyOfCone;						//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BodyOfCone;						//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_BottomOfCone;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BottomOfCone;						//Parameter to pass to glDrawRangeElements

	//Index related
	sint	StartIndexIn_pIndex_BodyOfCone, EndIndexIn_pIndex_BodyOfCone;			//End-1 gives the last index
	sint	StartIndexIn_pIndex_BottomOfCone, EndIndexIn_pIndex_BottomOfCone;		//End-1 gives the last index

	sint	TotalIndex_BodyOfCone;
	sint	TotalIndex_BottomOfCone;

	ushort	*pIndex16Bit_BodyOfCone;
	ushort	*pIndex16Bit_BottomOfCone;

	uint	*pIndex32Bit_BodyOfCone;
	uint	*pIndex32Bit_BottomOfCone;

	//Vertex related
	sint	StartIndexIn_pVertex_BodyOfCone, EndIndexIn_pVertex_BodyOfCone;			//If Interlaced vertex format is used such as pVertex_VNT, then this is End-1
	sint	StartIndexIn_pVertex_BottomOfCone, EndIndexIn_pVertex_BottomOfCone;		//If Interlaced vertex format is used such as pVertex_VNT, then this is End-1

	sint	TotalVertex_BodyOfCone;
	sint	TotalVertex_BottomOfCone;

	sreal	*pVertex_BodyOfCone;									//Pointer to pVertex or pVertex_VNT where body starts
	sreal	*pVertex_BottomOfCone;									//Pointer to pVertex or pVertex_VNT where bottom starts
};

#endif // !defined(AFX_TCONE_H__CFB1828C_7CB9_431C_BB58_68147A24D136__INCLUDED_)

//LOG:
//Wednesday, Oct 27, 2010
//Created this class.
//


