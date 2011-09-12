// TBaseShape.h: interface for the TBaseShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBASESHAPE_H__559AB211_9D4F_4766_8A0A_9CE4F912FEFB__INCLUDED_)
#define AFX_TBASESHAPE_H__559AB211_9D4F_4766_8A0A_9CE4F912FEFB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//#include <GL/gl.h>
#include "glhlib.h"
#include "ObjectsDefines.h"
#include "global.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'




//PURPOSE:	This is the base class for other classes such as TSphere, TCylinder,
//			TCube, TCone, TTorus, ... and more complex classes.
//			It contains the basics for doing 3D Graphics like arrays for
//			index, vertex, normal, texture, color, secondary color.
//			THIS IS A DO NOTHING CLASS.
class TBaseShape
{
public:
	TBaseShape();
	virtual ~TBaseShape();

	schar	ObjectName[512];
	uint	ObjectID;			//Every object has a unique ID

	sint Startup() {return 1;}
	//**** For memory management - deallocators
	void DeallocateAllMemory();
protected:
	void GeneralReset();
public:
	void FlushAllMemory();
	void DeallocateMemoryForIndex();
	void DeallocateMemoryForVertex();
	void DeallocateMemoryForNormals();
	void DeallocateMemoryForTexCoords(sint texUnit);
	void DeallocateMemoryForColors();
	void DeallocateMemoryForSecondaryColors();
	void DeallocateMemoryForTangentVectors();
	void DeallocateMemoryForBinormalVectors();
	void DeallocateMemoryVertexInterlaced();

	sint AllocateMemoryForIndex(sint amount);
	sint AllocateMemoryForVertex(sint amount);
	sint AllocateMemoryForNormals();
	sint AllocateMemoryForTexCoords(sint texUnit, sint numberOfComponents=2);
	sint AllocateMemoryForColors();
	sint AllocateMemoryForSecondaryColors();
	sint AllocateMemoryForTangentVectors();
	sint AllocateMemoryForBinormalVectors();

	void SetUseIndexFormat(GLH_INDEXFORMAT indexFormat);

	
	sint GenerateIndices() {return 1;}
	sint GenerateVertices() {return 1;}
	sint GenerateNormals(sint isOrientNormalsOutside=1) {return 1;}
	sint IsOrientNormalsOutside;
	sint GenerateTexCoords(sint texUnit, sint numberOfComponents=2) {return 1;}
	void SetTexCoordComponents(sint texUnit, sint numberOfComponents=2);
	sint GenerateTangentVectors() {return 1;}
	sint GenerateBinormalVectors() {return 1;}
	sint GenerateTangentAndBinormalVectors() {return 1;}
	sint GenerateVisibleNormals(sreal length);					//This one is actually implemented

	//**** Properties setters
	//** Colors
	void SetColors(sreal red, sreal green, sreal blue, sreal alpha, sreal red2=1.0, sreal green2=1.0, sreal blue2=1.0, sreal alpha2=1.0);
	void SetColors(sreal *primaryColor, sreal *secondaryColor);
	void SetColors(uchar red, uchar green, uchar blue, uchar alpha, uchar red2=255, uchar green2=255, uchar blue2=255, uchar alpha2=255);
	void SetColors(uint *primaryColor, uint *secondaryColor);
	void SetMaterialColors(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2=NULL, sreal *diffuse2=NULL, sreal *emission2=NULL, sreal *specular2=NULL);
	void SetNormalColor(sreal red, sreal green, sreal blue, sreal alpha);
	void SetNormalColor(uchar red, uchar green, uchar blue, uchar alpha);


	sint		UsedIndex;
	ushort		*pIndex16Bit;			//Depends on IndexFormat, one or the other will be used
	uint		*pIndex32Bit;			//Depends on IndexFormat, one or the other will be used
	GLH_INDEXFORMAT	IndexFormat;		//The precision to use for pIndex (pIndex16Bit or pIndex32Bit)

	sint	UsedVertex, NumberOfComponentsForVertex;
	sreal	*pVertex;

	sint	UsedNormals, NumberOfComponentsForNormals;
	sreal	*pNormals;

	//**** For texture coordinates
	sint	UsedTexCoords[NUMBEROFTEXTUREUNITS];
	sreal	*pTexCoords[NUMBEROFTEXTUREUNITS];
	uint	TextureID[NUMBEROFTEXTUREUNITS];
	sint	NumberOfComponentsForTexCoords[NUMBEROFTEXTUREUNITS];
	schar	pTextureFilePath[NUMBEROFTEXTUREUNITS][512];
	sint	TexCoordStyle;
	sreal	ScaleFactorS[NUMBEROFTEXTUREUNITS], ScaleFactorT[NUMBEROFTEXTUREUNITS];

	sint	UsedColors, NumberOfComponentsForColor;
	sreal	*pColors;				//1 4D color per vertex, typically RGBA
	uint	*pColorsUINT;			//1 4D color per vertex, typically RGBA. See variable ColorsUINT for more info

	sint	UsedSecondaryColors, NumberOfComponentsForSecondaryColor;
	sreal	*pSecondaryColors;		//1 4D color per vertex, typically RGBA
	uint	*pSecondaryColorsUINT;	//1 4D color per vertex, typically RGBA
									//WARNING: I think alpha is not possible for current hardware.

	COLORPRECISION	ColorPrecision;	//For pColors, pColorsUINT, pSecondaryColors, pSecondaryColorsUINT

	sint	UsedTangents, NumberOfComponentsForTangents;
	sreal	*pTangents;		//1 3D vector per vertex

	sint	UsedBinormals, NumberOfComponentsForBinormals;
	sreal	*pBinormals;

	sint	TotalVisibleNormals, UsedVisibleNormals, VisibleNormalsExcess;
	sreal	*pVisibleNormals;

	sint	IsSimpleColorModeOn;										//If TRUE, then object has a single color, else 1 color per vertex Default=FALSE
	COLORPRECISION	ColorPrecision_Simple;								//For the variables below. This variable can be accessed by application and setup for conveniance, or just ignore it.
	sreal	Colors[8], SecondaryColors[8];								//RGBA : RGBA
	uint	ColorsUINT[2], SecondaryColorsUINT[2];						//RGBA : RGBA, each uint is 0xAABBGGRR on little endian
																		//WARNING: I think alpha is not possible for current hardware.
	sreal	VisibleNormalsColors[8];									//RGBA : RGBA
	uint	VisibleNormalsColorsUINT[8];								//RGBA : RGBA, each uint is 0xAABBGGRR on little endian

	
	uint	MaterialID;													//Every material has a unique ID >0
	sreal	MaterialAmbient[8], MaterialDiffuse[8];						//RGBA : RGBA
	sreal	MaterialEmission[8], MaterialSpecular[8];					//RGBA : RGBA
	sreal	MaterialShininess;
	sreal	IndexOfRefraction;
	sreal	CriticalAngleOfReflection;
	



	sreal	CenterPosition[4], OrientationVector[4];
	sint	PolygonNumber;

	//For TexEnv
	uint			TexTargetUnitX[NUMBEROFTEXTUREUNITS];				//GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
	sint			TexEnvModeUnitX[NUMBEROFTEXTUREUNITS];				//GL_DECAL, GL_MODULATE, GL_BLEND, GL_REPLACE, GL_COMBINE, GL_COMBINE4
	sint			TexEnvCombineRGBUnitX[NUMBEROFTEXTUREUNITS];		//Not used if not TexEnvModeUnit0 != GL_COMBINE or GL_COMBINE4 (NV) It can be GL_REPLACE, GL_MODULATE, GL_ADD, GL_ADD_SIGNED, GL_INTERPOLATE, GL_SUBSTRACT, GL_MODULATE_ADD_ATI, GL_MODULATE_SIGNED_ADD_ATI, GL_MODULATE_SUBSTRACT_ATI, GL_DOT3_RGB, GL_DOT3_RGBA
	sint			TexEnvCombineALPHAUnitX[NUMBEROFTEXTUREUNITS];		//Not used if not TexEnvModeUnit0 != GL_COMBINE or GL_COMBINE4 (NV)
	sint			TexEnvSourceRGBUnitX[NUMBEROFTEXTUREUNITS][4];		//Must be 0, GL_TEXTURE, GL_CONSTANT, GL_PRIMARY_COLOR, GL_PREVIOUS, GL_TEXTUREX (X = 0..7), GL_ONE, GL_ZERO for source 0, 1, 2, 3
	sint			TexEnvSourceALPHAUnitX[NUMBEROFTEXTUREUNITS][4];
	sint			TexEnvOperandRGBUnitX[NUMBEROFTEXTUREUNITS][4];		//Must be 0, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA for operand 0, 1, and 2
	sint			TexEnvOperandALPHAUnitX[NUMBEROFTEXTUREUNITS][4];

	//For TexGen
	//Coordinates S, T, R, Q must all have the same value.
	//One per texture unit.
	sint			IsTexGenForTexUnitXOn[NUMBEROFTEXTUREUNITS];
	sint			TexGenParameterUnitX[NUMBEROFTEXTUREUNITS];
	


	uint			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	uint			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements

	//The vertex format dictates the actual interlacing and number of components
	//You could read the data from a file and directly copy to here
	//and then put the data into a VBO
	//If you need the vertices and normals for some calculations, you can put them in
	//pVertex and pNormal and setup NumberOfComponentsForVertex and NumberOfComponentsForNormals
	//and you may also need the pIndex16Bit array.
	//Once you have uploaded to a VBO, you can delete pVertex_VNT (or the others)
	//set VertexFormat=NULL and VertexCount=0
	//to free some RAM
	//You can use pVertex and pNormal to do intersection tests on the CPU.
	GLH_VERTEXFORMAT	VertexFormat;
	sint			VertexCount;				//This is set when GenerateInterlacedFormat() is called or set it yourself
	GLHVertex_V		*pVertex_V;
	GLHVertex_VT		*pVertex_VT;
	GLHVertex_VNT		*pVertex_VNT;			//and allocate RAM for pVertex_VNT
	GLHVertex_VNT3		*pVertex_VNT3;
	GLHVertex_VNTT3T3	*pVertex_VNTT3T3;
	GLHVertex_VNTT3		*pVertex_VNTT3;



	schar	ErrorMessage[1024];
};

#endif // !defined(AFX_TBASESHAPE_H__559AB211_9D4F_4766_8A0A_9CE4F912FEFB__INCLUDED_)


//LOG:
//Created on Saturday, Nov 1, 2003
//I've should have created this class a long time ago I guess.
//Since I will be updating many of the other classes for support for
//tangent, binormal, secondary color... I have created this class and will
//derive the other classes off of this.
//ONE IMPORTANT change. Instead of having vertices be 3 components all the time,
//I will give the option to make them 4, incase we want to use SSE instructions
//or 3DNow! or maybe 4 components is faster to send to OpenGL.
//Better keep our options open!
//Same for normals, tangent, binormal. We should be able to have 3 or 4 components.
//So I have created NumberOfComponentsxxxxxxxxx variables for this.
//
//Thursday, Feb 03, 2005
//Added ObjectName[512] (should be enough for ASCII)
//
//Saturday, Feb 12, 2005
//Added TexEnv and TexGen related variables. I needed this in project 3DSLoader.
//
//Sunday, Feb 18, 2007
//Adding support for interlaced vertex format :
//MYVERTEXFORMAT	VertexFormat;
//sint			VertexCount;			//This is set when GenerateInterlacedFormat() is called or set it yourself
//TVertex_V		*pVertex_V;
//TVertex_VT		*pVertex_VT;
//TVertex_VNT		*pVertex_VNT;			//and allocate RAM for pVertex_VNT
//TVertex_VNT3	*pVertex_VNT3;
//TVertex_VNTT3T3	*pVertex_VNTT3T3;
//





