// TCube.h: interface for the TCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCUBE_H__3B2ADEDC_CBCA_41A4_AB0E_278F91828545__INCLUDED_)
#define AFX_TCUBE_H__3B2ADEDC_CBCA_41A4_AB0E_278F91828545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include <windows.h>
//#include <GL/gl.h>		//need OpenGL related datatypes and such
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'



//THIS IS A COPY OF TCube in my VC++ folder, but I removed the rendering functions
//and I removed #include <GL/glew.h>



//PURPOSE:	This is a class for rendering a cube or more precisely a rectangular prism.
//			Be sure to call Startup() before using the main functions of this class.
//WARNING:	This class needs further development since I'm only implementing
//			the essentials for now.
class TCube : public TBaseShape
{
public:
	TCube();
	virtual ~TCube();

	sint Startup(sreal width, sreal height, sreal length, sint widthDivision, sint heightDivision, sint lengthDivision);
	sint Startup2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, sreal width, sreal height, sreal length,
		sint widthDivision, sint heightDivision, sint lengthDivision, tbool isAveragedOverFacesOn, sint texCoordStyle);

private:
	sint SameStartUpPatternForCube();
	sint SameStartUpPatternForCube_Interleaved();

public:
	void GeneralReset();

	//**** For memory management - deallocators
	void DeallocateAllMemory();

	sint GenerateIndices();
	sint GenerateVertices();
	sint GenerateNormals(tbool isAveragedOverFacesOn=FALSE);
	sint GenerateTexCoords(sint texUnit, sint style=1, sint numberOfComponents=2);
	sint GenerateTangentAndBinormalVectors();

	sint GenerateVertices_Interleaved();
	sint GenerateVertices_Interleaved_Part10();
	sint GenerateVertices_Interleaved_Part10b();
	sint GenerateVertices_Interleaved_Part11();
	sint GenerateVertices_Interleaved_Part12();
	sint GenerateVertices_Interleaved_Part13();
	sint GenerateVertices_Interleaved_Part14();
	sint GenerateVertices_Interleaved_Part15();
	sint GenerateVertices_Interleaved_Part15b();
	


	sreal	Width, Height, Length;
	sint	WidthDivision, HeightDivision, LengthDivision;
	tbool	IsAveragedOverFacesOn;
	sint	TexCoordStyle;
};


#endif // !defined(AFX_TCUBE_H__3B2ADEDC_CBCA_41A4_AB0E_278F91828545__INCLUDED_)


//LOG:
//Created this class for project OpenGLPanelTesting on
//Monday, May 20, 2002
//Im only implementing the basic stuff I need for now.
//
//Friday, August 9, 2002
//Updating this class and generalizing properties while working on raytrace project
//
//Monday, October 13, 2003
//Updating for index support. Minor update. I will not remove pPrimitiveList. 
//
//Monday, December 1, 2003
//Changing some things. Deriving from TBaseShape and making some additions to TBaseShape.
//Needs update for NumberOfComponents for normals, vertex, ...
//
//Sunday, Feb 18, 2007
//Adding support for interlaced vertex format
//
//Tuesday, April 22, 2008
//Updating Startup and Startup2. Getting rid of PolygonNumber.
//Adding support for WidthDivision, HeightDivision, LengthDivision
//



