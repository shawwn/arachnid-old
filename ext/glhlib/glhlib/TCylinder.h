// TCylinder.h: interface for the TCylinder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCYLINDER_H__015D2B07_D6A8_40F7_8182_D6AF45194906__INCLUDED_)
#define AFX_TCYLINDER_H__015D2B07_D6A8_40F7_8182_D6AF45194906__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <windows.h>
//#include <GL/gl.h>
#include "TBaseShape.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'








//PURPOSE:	This class is capable of doing a basic cylinder (Radius>0, Height>0)
//			Surface is always smooth shaded, one normal per vertex.
//			The cylinder is split into three parts : body, top and bottom.
//			Can render entire cylinder, or without top and/or bottom.
//			Can generate texture coordinates for OpenGL, and can render
//			textured in single shot, or can render body textured, top textured,
//			then bottom textured. This is so that different textures can be applied
//			to body, top and bottom. SEE THE VARIABLES FOR THE BODY, TOP AND BOTTOM
//			AND MAKE SURE YOU SET THEM UP. ALSO, THERE ARE NEW FUNCTIONS ADDED TO THIS CLASS.
class TCylinder : public TBaseShape
{
public:
	TCylinder();
	virtual ~TCylinder();

	//**** Startup functions
	sint StartupByPolygon(tbool isThereATop, tbool isThereABottom, sreal radius, sreal height, sint polygonNumber);
	sint StartupByStacksAndSlices(tbool isThereATop, tbool isThereABottom, sreal radius, sreal height, sint stacks, sint slices);
	sint StartupByTwoRadiusPolygon(tbool isThereATop, tbool isThereABottom, sreal aRadius, sreal bRadius, sreal height, sint polygonNumber);
	sint StartupByTwoRadiusStacksAndSlices(tbool isThereATop, tbool isThereABottom, sreal aRadius, sreal bRadius, sreal height, sint stacks, sint slices);

	sint StartupByTwoRadiusStacksAndSlices2(GLH_INDEXFORMAT indexFormat, GLH_VERTEXFORMAT vertexFormat, tbool isThereATop, tbool isThereABottom, sreal aRadius, sreal bRadius, sreal height, sint stacks, sint slices,
		sint texCoordStyle, sreal scaleFactorS, sreal scaleFactorT);

	//**** For memory management - deallocators
	void DeallocateAllMemory();
private:
	void GeneralReset();
	sint SameStartupPatternForCylinder(sreal height);

	sint SameStartupPatternForCylinder2(sreal height);

public:


	//**** Properties setters
	//** Colors
	void SetColors(sreal red, sreal green, sreal blue, sreal alpha, sreal red2=1.0, sreal green2=1.0, sreal blue2=1.0, sreal alpha2=1.0);
	void SetColors_Body(sreal red, sreal green, sreal blue, sreal alpha, sreal red2=1.0, sreal green2=1.0, sreal blue2=1.0, sreal alpha2=1.0);
	void SetColors_Top(sreal red, sreal green, sreal blue, sreal alpha, sreal red2=1.0, sreal green2=1.0, sreal blue2=1.0, sreal alpha2=1.0);
	void SetColors_Bottom(sreal red, sreal green, sreal blue, sreal alpha, sreal red2=1.0, sreal green2=1.0, sreal blue2=1.0, sreal alpha2=1.0);
	void SetColors(sreal *primaryColor, sreal *secondaryColor);
	void SetColors_Body(sreal *primaryColor, sreal *secondaryColor);
	void SetColors_Top(sreal *primaryColor, sreal *secondaryColor);
	void SetColors_Bottom(sreal *primaryColor, sreal *secondaryColor);
	void SetColors(uchar red, uchar green, uchar blue, uchar alpha, uchar red2=255, uchar green2=255, uchar blue2=255, uchar alpha2=255);
	void SetColors_Body(uchar red, uchar green, uchar blue, uchar alpha, uchar red2=255, uchar green2=255, uchar blue2=255, uchar alpha2=255);
	void SetColors_Top(uchar red, uchar green, uchar blue, uchar alpha, uchar red2=255, uchar green2=255, uchar blue2=255, uchar alpha2=255);
	void SetColors_Bottom(uchar red, uchar green, uchar blue, uchar alpha, uchar red2=255, uchar green2=255, uchar blue2=255, uchar alpha2=255);
	void SetColors(uint *primaryColor, uint *secondaryColor);
	void SetColors_Body(uint *primaryColor, uint *secondaryColor);
	void SetColors_Top(uint *primaryColor, uint *secondaryColor);
	void SetColors_Bottom(uint *primaryColor, uint *secondaryColor);

	void SetMaterialColors(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2=NULL, sreal *diffuse2=NULL, sreal *emission2=NULL, sreal *specular2=NULL);
	void SetMaterialColors_Body(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2=NULL, sreal *diffuse2=NULL, sreal *emission2=NULL, sreal *specular2=NULL);
	void SetMaterialColors_Top(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2=NULL, sreal *diffuse2=NULL, sreal *emission2=NULL, sreal *specular2=NULL);
	void SetMaterialColors_Bottom(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2=NULL, sreal *diffuse2=NULL, sreal *emission2=NULL, sreal *specular2=NULL);



	sint GenerateVertices();
	sint GenerateIndices();
	sint GenerateNormals();
	sint GenerateTexCoords(sint texUnit, sint numberOfComponents, sreal scaleS, sreal scaleT,
		sint style=1, sreal widthTextureLimit=1.0, sreal heightTextureLimit=1.0);
	sint GenerateTangentAndBinormalVectors();
	sint GenerateVisibleNormals(sreal length);

	sint GenerateVertices2();
	sint GenerateNormals2();
	sint GenerateTexCoords2(sreal widthTextureLimit, sreal heightTextureLimit);


	//Getter methods
	sint GetPolygonMinimum() { return 8;}
	sint GetStacksMinimum() { return 1;}
	sint GetSlicesMinimum() { return 3;}
	
	sint GetPolygonNumber() { return PolygonNumber;}

	sreal GetHeight() { return Height;}
	sreal GetARadius() { return ARadius;}
	sreal GetBRadius() { return BRadius;}

	sint GetSlicesNumber() { return Slices;}
	sint GetStacksNumber() { return Stacks;}


	//**** Main variables for cylinder
	tbool	IsThereATop, IsThereABottom;							//Compute vertices and index or not?
	sreal	ARadius, BRadius, Height;
	sint	Stacks;													//Number of divisions in the Theta direction
	sint	SlicesPlusOne;											//Number of divisions in the Phi direction
	sint	Slices;													//This is the actual slice number, SlicesPlusOne has been incremented


	//These 2 are for rendering the entire cylinder in one call to glDrawRangeElements
	//uint			Start_DrawRangeElements;						//Parameter to pass to glDrawRangeElements
	//uint			End_DrawRangeElements;							//Parameter to pass to glDrawRangeElements



	uint	Start_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_TopOfCylinder;						//Parameter to pass to glDrawRangeElements
	uint	Start_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements
	uint	End_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements

	//Vertex related
	sint	StartIndexIn_pVertex_BodyOfCylinder, EndIndexIn_pVertex_BodyOfCylinder;		//End-3 gives the last vertex
																						//If Interlaced vertex format is used such as pVertex_VNT, then this is End-1
	sint	StartIndexIn_pVertex_TopOfCylinder, EndIndexIn_pVertex_TopOfCylinder;		//End-3 gives the last vertex
	sint	StartIndexIn_pVertex_BottomOfCylinder, EndIndexIn_pVertex_BottomOfCylinder;	//End-3 gives the last vertex

	sreal	*pVertex_BodyOfCylinder;								//Pointer to pVertex where body starts
	sreal	*pVertex_TopOfCylinder;									//Pointer to pVertex where top starts
	sreal	*pVertex_BottomOfCylinder;								//Pointer to pVertex where bottom starts

	sint	TotalVertex_BodyOfCylinder;
	sint	TotalVertex_TopOfCylinder;
	sint	TotalVertex_BottomOfCylinder;

	//Index related
	sint	StartIndexIn_pIndex_BodyOfCylinder, EndIndexIn_pIndex_BodyOfCylinder;		//End-1 gives the last index
	sint	StartIndexIn_pIndex_TopOfCylinder, EndIndexIn_pIndex_TopOfCylinder;			//End-1 gives the last index
	sint	StartIndexIn_pIndex_BottomOfCylinder, EndIndexIn_pIndex_BottomOfCylinder;	//End-1 gives the last index

	ushort	*pIndex16Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	ushort	*pIndex16Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.


	sint	TotalIndex_BodyOfCylinder;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_TopOfCylinder;								//Parameter to pass to glDrawRangeElements
	sint	TotalIndex_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements

	uint	*pIndex32Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	uint	*pIndex32Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.

	//Normal
	sint	StartIndexIn_pNormals_BodyOfCylinder, EndIndexIn_pNormals_BodyOfCylinder;			//End-3 gives last normal
	sreal	*pNormals_BodyOfCylinder;
	sint	StartIndexIn_pNormals_TopOfCylinder, EndIndexIn_pNormals_TopOfCylinder;				//End-3 gives last normal
	sreal	*pNormals_TopOfCylinder;
	sint	StartIndexIn_pNormals_BottomOfCylinder, EndIndexIn_pNormals_BottomOfCylinder;		//End-3 gives last normal
	sreal	*pNormals_BottomOfCylinder;

	sint	TotalNormals_BodyOfCylinder;
	sint	TotalNormals_TopOfCylinder;
	sint	TotalNormals_BottomOfCylinder;

	//Texcoord
	sint	StartIndexIn_pTexCoords_BodyOfCylinder[NUMBEROFTEXTUREUNITS], EndIndexIn_pTexCoords_BodyOfCylinder[NUMBEROFTEXTUREUNITS];		//End-2 gives last texcoord
	sreal	*pTexCoords_BodyOfCylinder[NUMBEROFTEXTUREUNITS];
	sint	StartIndexIn_pTexCoords_TopOfCylinder[NUMBEROFTEXTUREUNITS], EndIndexIn_pTexCoords_TopOfCylinder[NUMBEROFTEXTUREUNITS];			//End-2 gives last texcoord
	sreal	*pTexCoords_TopOfCylinder[NUMBEROFTEXTUREUNITS];
	sint	StartIndexIn_pTexCoords_BottomOfCylinder[NUMBEROFTEXTUREUNITS], EndIndexIn_pTexCoords_BottomOfCylinder[NUMBEROFTEXTUREUNITS];	//End-2 gives last texcoord
	sreal	*pTexCoords_BottomOfCylinder[NUMBEROFTEXTUREUNITS];

	sint	TotalTexCoords_BodyOfCylinder[NUMBEROFTEXTUREUNITS];
	sint	TotalTexCoords_TopOfCylinder[NUMBEROFTEXTUREUNITS];
	sint	TotalTexCoords_BottomOfCylinder[NUMBEROFTEXTUREUNITS];

	//TextureID
	uint TextureID_BodyOfCylinder[NUMBEROFTEXTUREUNITS];
	uint TextureID_TopOfCylinder[NUMBEROFTEXTUREUNITS];
	uint TextureID_BottomOfCylinder[NUMBEROFTEXTUREUNITS];

	//Color
	sint	StartIndexIn_pColors_BodyOfCylinder, EndIndexIn_pColors_BodyOfCylinder;				//End-4 gives last color
	sreal	*pColors_BodyOfCylinder;
	sint	StartIndexIn_pColors_TopOfCylinder, EndIndexIn_pColors_TopOfCylinder;				//End-4 gives last color
	sreal	*pColors_TopOfCylinder;
	sint	StartIndexIn_pColors_BottomOfCylinder, EndIndexIn_pColors_BottomOfCylinder;			//End-4 gives last color
	sreal	*pColors_BottomOfCylinder;

	sint	TotalColors_BodyOfCylinder;
	sint	TotalColors_TopOfCylinder;
	sint	TotalColors_BottomOfCylinder;

	//Secondary color
	sint	StartIndexIn_pSecondaryColors_BodyOfCylinder, EndIndexIn_pSecondaryColors_BodyOfCylinder;				//End-4 gives last color
	sreal	*pSecondaryColors_BodyOfCylinder;
	sint	StartIndexIn_pSecondaryColors_TopOfCylinder, EndIndexIn_pSecondaryColors_TopOfCylinder;					//End-4 gives last color
	sreal	*pSecondaryColors_TopOfCylinder;
	sint	StartIndexIn_pSecondaryColors_BottomOfCylinder, EndIndexIn_pSecondaryColors_BottomOfCylinder;			//End-4 gives last color
	sreal	*pSecondaryColors_BottomOfCylinder;

	sint	TotalSecondaryColors_BodyOfCylinder;
	sint	TotalSecondaryColors_TopOfCylinder;
	sint	TotalSecondaryColors_BottomOfCylinder;

	//Tangent
	sint	StartIndexIn_pTangents_BodyOfCylinder, EndIndexIn_pTangents_BodyOfCylinder;				//End-3 gives last tangent
	sreal	*pTangents_BodyOfCylinder;
	sint	StartIndexIn_pTangents_TopOfCylinder, EndIndexIn_pTangents_TopOfCylinder;				//End-3 gives last tangent
	sreal	*pTangents_TopOfCylinder;
	sint	StartIndexIn_pTangents_BottomOfCylinder, EndIndexIn_pTangents_BottomOfCylinder;			//End-3 gives last tangent
	sreal	*pTangents_BottomOfCylinder;

	sint	TotalTangents_BodyOfCylinder;
	sint	TotalTangents_TopOfCylinder;
	sint	TotalTangents_BottomOfCylinder;

	//Binormal
	sint	StartIndexIn_pBinormals_BodyOfCylinder, EndIndexIn_pBinormals_BodyOfCylinder;				//End-3 gives last tangent
	sreal	*pBinormals_BodyOfCylinder;
	sint	StartIndexIn_pBinormals_TopOfCylinder, EndIndexIn_pBinormals_TopOfCylinder;					//End-3 gives last tangent
	sreal	*pBinormals_TopOfCylinder;
	sint	StartIndexIn_pBinormals_BottomOfCylinder, EndIndexIn_pBinormals_BottomOfCylinder;			//End-3 gives last tangent
	sreal	*pBinormals_BottomOfCylinder;

	sint	TotalBinormals_BodyOfCylinder;
	sint	TotalBinormals_TopOfCylinder;
	sint	TotalBinormals_BottomOfCylinder;

	//Visible normals
	sint	StartIndexIn_pVisibleNormals_BodyOfCylinder, EndIndexIn_pVisibleNormals_BodyOfCylinder;			//End-6 gives last visible normal (GL_LINES)
	sreal	*pVisibleNormals_BodyOfCylinder;
	sint	StartIndexIn_pVisibleNormals_TopOfCylinder, EndIndexIn_pVisibleNormals_TopOfCylinder;			//End-6 gives last visible normal (GL_LINES)
	sreal	*pVisibleNormals_TopOfCylinder;
	sint	StartIndexIn_pVisibleNormals_BottomOfCylinder, EndIndexIn_pVisibleNormals_BottomOfCylinder;		//End-6 gives last visible normal (GL_LINES)
	sreal	*pVisibleNormals_BottomOfCylinder;

	sint	TotalVisibleNormals_BodyOfCylinder;
	sint	TotalVisibleNormals_TopOfCylinder;
	sint	TotalVisibleNormals_BottomOfCylinder;

	//Object color
	sreal	Colors_Top[8], SecondaryColors_Top[8];						//RGBA : RGBA
	uint	ColorsUINT_Top[2], SecondaryColorsUINT_Top[2];				//RGBA : RGBA, each uint is 0xAABBGGRR on little endian
																		//WARNING: I think alpha is not possible for current hardware.

	sreal	Colors_Bottom[8], SecondaryColors_Bottom[8];				//RGBA : RGBA
	uint	ColorsUINT_Bottom[2], SecondaryColorsUINT_Bottom[2];		//RGBA : RGBA, each uint is 0xAABBGGRR on little endian
																		//WARNING: I think alpha is not possible for current hardware.


	//Material - MaterialID and the rest are for the body
	uint	MaterialID_Top;												//Every material has a unique ID >0
	sreal	MaterialAmbient_Top[8], MaterialDiffuse_Top[8];				//RGBA : RGBA
	sreal	MaterialEmission_Top[8], MaterialSpecular_Top[8];			//RGBA : RGBA
	sreal	MaterialShininess_Top;

	uint	MaterialID_Bottom;											//Every material has a unique ID >0
	sreal	MaterialAmbient_Bottom[8], MaterialDiffuse_Bottom[8];		//RGBA : RGBA
	sreal	MaterialEmission_Bottom[8], MaterialSpecular_Bottom[8];		//RGBA : RGBA
	sreal	MaterialShininess_Bottom;
};


#endif // !defined(AFX_TCYLINDER_H__015D2B07_D6A8_40F7_8182_D6AF45194906__INCLUDED_)

//LOG:
//Friday, August 25, 2000
//Reworking GL_SELECT related functions
//Added TranslateCenterTo() function
//
//Friday, January 26, 2001
//Fixing up for texturing the cylinder
//
//Sunday, October 13, 2002
//Making minor updates. This class needs to be brought up to date someday
//but it will take hours to fix it.
//
//Thursday, Jan 20, 2005
//I need this class sometimes for tests. It's time to bring it up to date with the others.
//Will derive from class TBaseShape.
//I will be deleting most of the previous code and implemeting my basic needs for now.
//
//Saturday, Jan 22, 2005
//Conntinuing...
//I think I'll forget about primitive list feature because it wastes memory.
//No duplicate vertices will be created.
//
//Sunday, Feb 18, 2007
//Adding support for interlaced vertex format :
//StartupByTwoRadiusStacksAndSlices2
//SameStartupPatternForCylinder2
//GenerateVertices2
//GenerateTexCoords2
//

