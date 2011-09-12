// TSphere.h: interface for the TSphere class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSPHERE_H__2748A7F9_EA12_11D3_A68A_000000000000__INCLUDED_)
#define AFX_TSPHERE_H__2748A7F9_EA12_11D3_A68A_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'




//PURPOSE:	This class is capable of doing a sphere and ellipsoids (3 radius values)
//			Calculates normals (smooth shaded only).
//			Can generate texture coordinates.
//			Can generate tangents and binormals.
//			Use StartupByABCStacksAndSlices2 or StartupByABCStacksAndSlices_StackBound2.
//			The other creation functions are old and do not use the vertex interleave format.
//			Leaving them for historical reasons.
class TSphere : public TBaseShape
{
public:
	TSphere();
	virtual ~TSphere();

	//**** Startup functions
	sint StartupByPolygon(sreal radius, sint polygonNumber);
	sint StartupByABCPolygon(sreal aRadius, sreal bRadius, sreal cRadius, sint polygonNumber);
	sint StartupByStacksAndSlices(sreal radius, sint stacks, sint slices);
	sint StartupByABCStacksAndSlices(sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices);
	sint StartupByABCStacksAndSlices_StackBound(sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
		sint startingStack, sint endingStack);

	sint StartupByABCStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT);

	sint StartupByABCStacksAndSlices_StackBound2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, sreal aRadius, sreal bRadius, sreal cRadius, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT,
		sint startingStack, sint endingStack);

	//**** For memory management - deallocators
	void DeallocateAllMemory();

protected:
	void GeneralReset();
	sint SameStartupPatternForSphere();
	sint SameStartupPatternForSphere2();

public:
	sint GenerateVertices();
	sint GenerateIndices();
	sint GenerateNormals();
	sint GenerateTexCoords(sint texUnit, sint numberOfComponents, tbool isGlobeMapOn, tbool invertS, tbool invertT, sint texturingMode, sint chooseVertex, sreal orientationAngle, sreal widthTextureLimit, sreal heightTextureLimit, sreal sScale, sreal tScale);
	sint GenerateTangentAndBinormalVectors();

	sint GenerateVertices2();
	sint GenerateNormals2();
	sint GenerateTexCoords2(tbool invertS, tbool invertT);

	//Getter methods
	sint GetPolygonMinimum() { return 9;}
	sint GetStacksMinimum() { return 3;}
	sint GetSlicesMinimum() { return 3;}
	sint GetStacksNumber() { return Stacks;}
	sint GetSlicesNumber() { return Slices;}
	sint GetPolygonNumber() { return PolygonNumber;}
	sreal GetRadiusA() { return RadiusA;}
	sreal GetRadiusB() { return RadiusB;}
	sreal GetRadiusC() { return RadiusC;}



	//**** Variables
	sreal	RadiusA, RadiusB, RadiusC;
	sint	Stacks;												//Number of divisions in the Theta direction
	sint	StartingStack, EndingStack;
	sint	Slices;												//Number of divisions in the Phi direction

	//sint	StartIndex, EndIndex;								//For glDrawRangeElements
};



#endif // !defined(AFX_TSPHERE_H__2748A7F9_EA12_11D3_A68A_000000000000__INCLUDED_)

//LOG:
//Thursday, March 6, 2000
//Updating TSphere class
//Looking into phong rendering problem
//
//Friday, August 25, 2000
//Reworking GL_SELECT related functions
//Added TranslateCenterTo() function
//
//Monday, January 29, 2001
//Updating along with TCylinder
//
//Saturday, June 16, 2001
//Added swap color functions
//
//Thursday, August 15, 2002
//Updating for primitivelist support
//Only gouraud shading is posssible now.
//
//Saturday, March 19, 2005
//It's time to bring this up to date with new programming style
//and deriving off of TBaseShape class.
//
//Monday, March 21, 2005
//Continuing. Deleting some of the old crap. I won't use my "primitive list" feature in TSphere.
//
//Thursday, Sept 28, 2006
//I needed the ability to create dome (half-sphere, so why add the ability to generate
//the sphere between stack X and stack Y
//Adding StartupByABCStacksAndSlices_StackBound
//
//Saturday, Feb 24, 2007
//Adding support for interlaced vertex format :
//SameStartupPatternForSphere2
//GenerateVertices2
//GenerateTexCoords2
//
//Sunday, Oct 24, 2010
//Adding StartupByABCStacksAndSlices_StackBound2 and updating a few other functions.
//This is for creating partial spheres.
//



