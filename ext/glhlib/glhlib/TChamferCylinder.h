// TChamferCylinder.h: interface for the TChamferCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCHAMFERCYLINDER_H__7AC00B5E_EC69_4F7A_9E3E_EE38D1E610E5__INCLUDED_)
#define AFX_TCHAMFERCYLINDER_H__7AC00B5E_EC69_4F7A_9E3E_EE38D1E610E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'






//PURPOSE:	This class is capable of doing a Chamfer Cylinder, which is basically a tire-like shape.
//			You can control BodyRadius, ChamferRadius and Height for the size of the cylinder.
//			Stacks controls the tesselation.
//			Slices controls the tesselation.
//			ChamferStack controls the tesselation.
//			You can generate texcoords. scaleFactorS and scaleFactorT controls the texcoords.
//			Can generate tangents and binormals.
//
//			IMPORTANT: The normals for the top and bottom part are generated in the same was as
//			for class TCylinder. In TCylinder, the normals point to {0, 0, 1} or {0, 0, -1}.
///			For TChamferCylinder, the normals are smooth so that the entire surface looks organic.
//			A generic normal vector generating algorithm is used.
class TChamferCylinder : public TBaseShape
{
public:
	TChamferCylinder();
	virtual ~TChamferCylinder();

	sint StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		tbool isThereATop, tbool isThereABottom,
		sreal bodyRadius, sreal chamferRadius, sreal height, sint stacks, sint slices,
		sint chamferStacks, sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT);

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
	sint GetPolygonMinimum() { return 30;}
	sint GetStacksMinimum() { return 1;}
	sint GetSlicesMinimum() { return 3;}
	sint GetChamferStacksMinimum() { return 1;}
	
	sint GetPolygonNumber() { return PolygonNumber;}

	sreal GetHeight() { return Height;}
	sreal GetBodyRadius() { return BodyRadius;}
	sreal GetChamferRadius() { return ChamferRadius;}

	sint GetSlicesNumber() { return Slices;}
	sint GetStacksNumber() { return Stacks;}


	//**** Main variables for chamfercylinder
	tbool	IsThereATop, IsThereABottom;							//Compute vertices and index or not?
	sreal	BodyRadius;					//Radius of the body. This is larger than ChamferRadius
	sreal	ChamferRadius;				//This is the radius of the top and also the bottom of the tire
										//Must be less than BodyRadius
	sreal	Height;						//Height of the entire "ChamferCylinder"
	sint	Stacks;						//Stack count for the body part.
	sint	Slices;						//Slices for the entire "ChamferCylinder"
	sint	ChamferStacks;				//Stack count for just the chamfered part.
										//You would probably give a value like 3 or 4 which will generate a lot of
										//polygons at the chamfered part.
	sint	SlicesPlusOne;				//Number of divisions in the Phi direction +1


	//Leave the names as is (Start_DrawRangeElements_BodyOfCylinder and the rest)
	uint	Start_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_TopChamfer;						//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_TopChamfer;						//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_BottomChamfer;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BottomChamfer;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_BottomOfCylinder;				//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements

	//Vertex related
	sint	StartIndexIn_pVertex_BodyOfCylinder, EndIndexIn_pVertex_BodyOfCylinder;		//End-1 gives the last vertex
	sint	StartIndexIn_pVertex_TopChamfer, EndIndexIn_pVertex_TopChamfer;				//End-1 gives the last vertex
	sint	StartIndexIn_pVertex_BottomChamfer, EndIndexIn_pVertex_BottomChamfer;		//End-1 gives the last vertex
	sint	StartIndexIn_pVertex_TopOfCylinder, EndIndexIn_pVertex_TopOfCylinder;		//End-1 gives the last vertex
	sint	StartIndexIn_pVertex_BottomOfCylinder, EndIndexIn_pVertex_BottomOfCylinder;	//End-1 gives the last vertex

	sreal	*pVertex_BodyOfCylinder;								//Pointer to pVertex where body starts
	sreal	*pVertex_TopChamfer;
	sreal	*pVertex_BottomChamfer;
	sreal	*pVertex_TopOfCylinder;									//Pointer to pVertex where top starts
	sreal	*pVertex_BottomOfCylinder;								//Pointer to pVertex where bottom starts

	sint	TotalVertex_BodyOfCylinder;
	sint	TotalVertex_TopChamfer;
	sint	TotalVertex_BottomChamfer;
	sint	TotalVertex_TopOfCylinder;
	sint	TotalVertex_BottomOfCylinder;

	//Index related
	sint	StartIndexIn_pIndex_BodyOfCylinder, EndIndexIn_pIndex_BodyOfCylinder;		//End-1 gives the last index
	sint	StartIndexIn_pIndex_TopChamfer, EndIndexIn_pIndex_TopChamfer;				//End-1 gives the last index
	sint	StartIndexIn_pIndex_BottomChamfer, EndIndexIn_pIndex_BottomChamfer;			//End-1 gives the last index
	sint	StartIndexIn_pIndex_TopOfCylinder, EndIndexIn_pIndex_TopOfCylinder;			//End-1 gives the last index
	sint	StartIndexIn_pIndex_BottomOfCylinder, EndIndexIn_pIndex_BottomOfCylinder;	//End-1 gives the last index

	ushort	*pIndex16Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_TopChamfer;
	ushort	*pIndex16Bit_BottomChamfer;
	ushort	*pIndex16Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.


	sint	TotalIndex_BodyOfCylinder;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_TopChamfer;
	sint	TotalIndex_BottomChamfer;
	sint	TotalIndex_TopOfCylinder;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements

	uint	*pIndex32Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_TopChamfer;
	uint	*pIndex32Bit_BottomChamfer;
	uint	*pIndex32Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
};

#endif // !defined(AFX_TCHAMFERCYLINDER_H__7AC00B5E_EC69_4F7A_9E3E_EE38D1E610E5__INCLUDED_)

//LOG:
//Tuesday, Nov 23, 2010
//Create this class to make tire-like shape.
//


