// TTorus.h: interface for the TTorus class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTORUS_H__0D683F40_8F91_4D2D_96FE_D7FBCF67A292__INCLUDED_)
#define AFX_TTORUS_H__0D683F40_8F91_4D2D_96FE_D7FBCF67A292__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'








//PURPOSE:	This class is capable of doing a torus.
//			You can control RadiusOfTorus and RadiusOfCircle for the size of the torus.
//			Stacks controls the tesselation.
//			Slices controls the tesselation.
//			You can generate texcoords. scaleFactorS and scaleFactorT controls the texcoords.
//			Can generate tangents and binormals.
class TTorus : public TBaseShape
{
public:
	TTorus();
	virtual ~TTorus();

	sint StartupByStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat,
		sreal radiusOfTorus, sreal radiusOfCircle, sint stacks, sint slices,
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
	sint GetStacksMinimum() { return 3;}
	sint GetSlicesMinimum() { return 3;}
	sint GetStacksNumber() { return Stacks;}
	sint GetSlicesNumber() { return Slices;}
	sint GetPolygonNumber() { return PolygonNumber;}
	sreal GetRadiusOfTorus() { return RadiusOfTorus;}
	sreal GetRadiusOfCircle() { return RadiusOfCircle;}

	sreal	RadiusOfTorus;			//The radius from the center to the outermost edge of the torus
	sreal	RadiusOfCircle;			//The radius of the cross section of a part of the torus

	sint	Stacks;					//The slices along the wheel part of the torus. Minimum is 3.
	sint	Slices;					//The slices of each cross section. Minimum is 3.
};

#endif // !defined(AFX_TTORUS_H__0D683F40_8F91_4D2D_96FE_D7FBCF67A292__INCLUDED_)

//LOG:
//Sunday, Oct 24, 2010
//Created this class.
//



