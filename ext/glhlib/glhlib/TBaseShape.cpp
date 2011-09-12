// TBaseShape.cpp: implementation of the TBaseShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TBaseShape.h"
#include "MemoryManagement.h"
#include "MathLibrary.h"
#include <math.h>
#include <string.h>


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'


#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TBaseShape::TBaseShape()
{
	sint i;

	ObjectID=0;
	ObjectName[0]='\0';
	///////////////////////// Typical values for components
	NumberOfComponentsForVertex=3;
	NumberOfComponentsForNormals=3;
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
		NumberOfComponentsForTexCoords[i]=2;
	NumberOfComponentsForColor=4;
	NumberOfComponentsForSecondaryColor=4;
	NumberOfComponentsForTangents=3;
	NumberOfComponentsForBinormals=3;
	/////////////////////////
	for(i=0; i<8; i++)
	{
		Colors[i]=SecondaryColors[i]=0.0;
		MaterialAmbient[i]=MaterialDiffuse[i]=0.0;
		MaterialEmission[i]=MaterialSpecular[i]=0.0;
		VisibleNormalsColors[i]=0.0;
	}
	MaterialShininess=0.0;

	IndexOfRefraction=0.0;
	CriticalAngleOfReflection=0.0;

	for(i=0; i<2; i++)
	{
		ColorsUINT[i]=0;
		SecondaryColorsUINT[i]=0;
		VisibleNormalsColorsUINT[i]=0;
	}


	GeneralReset();
}

TBaseShape::~TBaseShape()
{
	DeallocateAllMemory();
}

void TBaseShape::GeneralReset()
{
	sint i, j;

	UsedIndex=0;
	pIndex16Bit=NULL;
	pIndex32Bit=NULL;
	UsedVertex=0;
	pVertex=NULL;
	UsedNormals=0;
	pNormals=NULL;
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		UsedTexCoords[i]=0;
		pTexCoords[i]=NULL;
		TextureID[i]=0;
		pTextureFilePath[i][0]='\0';
		ScaleFactorS[i]=ScaleFactorT[i]=0.0;
	}

	TexCoordStyle=1;

	UsedColors=0;
	pColors=NULL;
	pColorsUINT=NULL;
	UsedSecondaryColors=0;
	pSecondaryColors=NULL;
	pSecondaryColorsUINT=NULL;
	UsedTangents=0;
	pTangents=NULL;
	UsedBinormals=0;
	pBinormals=NULL;

	TotalVisibleNormals=UsedVisibleNormals=0;
	pVisibleNormals=NULL;


	CenterPosition[0]=CenterPosition[1]=CenterPosition[2]=0.0;
	OrientationVector[0]=OrientationVector[1]=0.0;
	OrientationVector[2]=1.0;
	PolygonNumber=0;

	ErrorMessage[0]='\0';

	//TexEnv
	//0 means these aren't set.
	//They are suppose to contain OpenGL's tokens normally.
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		TexTargetUnitX[i]=0;
		TexEnvModeUnitX[i]=0;
		TexEnvCombineRGBUnitX[i]=0;
		TexEnvCombineALPHAUnitX[i]=0;
		for(j=0; j<4; j++)
		{
			TexEnvSourceRGBUnitX[i][j]=-1;			//WE DON'T USE 0 BECAUSE 0 = GL_ZERO
			TexEnvSourceALPHAUnitX[i][j]=-1;
			TexEnvOperandRGBUnitX[i][j]=-1;
			TexEnvOperandALPHAUnitX[i][j]=-1;
		}
	}

	//TexEnv
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		IsTexGenForTexUnitXOn[i]=FALSE;
		TexGenParameterUnitX[i]=0;
	}

	IsOrientNormalsOutside=1;	//1 for outside, 0 for don't care, -1 for inside

	IsSimpleColorModeOn=FALSE;
	MaterialID=0;

	IndexFormat=GLH_INDEXFORMAT_16BIT;
	ColorPrecision=COLORPRECISION_32FLOAT;
	ColorPrecision_Simple=COLORPRECISION_32FLOAT;

	Start_DrawRangeElements=End_DrawRangeElements=0;

	VertexFormat=GLH_VERTEXFORMAT_NULL;
	VertexCount=0;
	pVertex_V=NULL;
	pVertex_VT=NULL;
	pVertex_VNT=NULL;
	pVertex_VNT3=NULL;
	pVertex_VNTT3T3=NULL;
	pVertex_VNTT3=NULL;
}


//PURPOSE:	GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT, pIndex16Bit or pIndex32Bit
//			Call this before using the rest of the function for creating the shape.
void TBaseShape::SetUseIndexFormat(GLH_INDEXFORMAT indexFormat)
{
	IndexFormat=indexFormat;
}

void TBaseShape::DeallocateAllMemory()
{
	sint i;

	if(pIndex16Bit)
		delete [] pIndex16Bit;
	if(pIndex32Bit)
		delete [] pIndex32Bit;
	if(pVertex)
		delete [] pVertex;
	if(pNormals)
		delete [] pNormals;
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if(pTexCoords[i])
			delete [] pTexCoords[i];
	}
	if(pColors)
		delete [] pColors;
	if(pColorsUINT)
		delete [] pColorsUINT;
	if(pSecondaryColors)
		delete [] pSecondaryColors;
	if(pSecondaryColorsUINT)
		delete [] pSecondaryColorsUINT;
	if(pTangents)
		delete [] pTangents;
	if(pBinormals)
		delete [] pBinormals;
	if(pVisibleNormals)
		delete [] pVisibleNormals;

	if(pVertex_V)
		delete [] pVertex_V;
	if(pVertex_VT)
		delete [] pVertex_VT;
	if(pVertex_VNT)
		delete [] pVertex_VNT;
	if(pVertex_VNT3)
		delete [] pVertex_VNT3;
	if(pVertex_VNTT3T3)
		delete [] pVertex_VNTT3T3;
	if(pVertex_VNTT3)
		delete [] pVertex_VNTT3;

	GeneralReset();
}


//PURPOSE:	Just allocates memory.
//			Returns 1 for success, 0 for memory error. amount should not be <=0
sint TBaseShape::AllocateMemoryForIndex(sint amount)
{
	UsedIndex=0;
	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		if(AllocateExact_1(pIndex16Bit, &UsedIndex, amount)!=1)
			return 0;
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		if(AllocateExact_1(pIndex32Bit, &UsedIndex, amount)!=1)
			return 0;
	}

	UsedIndex=amount;
	return 1;
}

//PURPOSE:	Just allocates memory.
//			Keep in mind NumberOfComponentsForVertex which is usually ==3.
//			If we need 5 vertices, then amount should be 5*NumberOfComponentsForVertex
//			Returns 1 for success, 0 for memory error. amount should not be <=0
sint TBaseShape::AllocateMemoryForVertex(sint amount)
{
	UsedVertex=0;
	if(AllocateExact_1(pVertex, &UsedVertex, amount)!=1)
		return 0;
	UsedVertex=amount;
	return 1;
}

//PURPOSE:	Just allocates memory. For every vertex, 1 attribute will be stored.
//			If array already allocated, then it *won't* deallocate. Use Deallocatexxxxxx()
//			for that
//			Make sure you allocate vertices first.
//			Returns 1 for success, 0 for memory error.
sint TBaseShape::AllocateMemoryForNormals()
{
	UsedNormals=0;
	if(UsedVertex==0)
		return 1;
	sint tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForNormals;
	if(AllocateExact_1(pNormals, &UsedNormals, tempo)!=1)
		return 0;

	UsedNormals=tempo;
	return 1;
}
sint TBaseShape::AllocateMemoryForColors()
{
	sint tempo;

	UsedColors=0;
	if(UsedVertex==0)
		return 1;

	if(ColorPrecision==COLORPRECISION_32FLOAT)
	{
		tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForColor;
		if(AllocateExact_1(pColors, &UsedColors, tempo)!=1)
			return 0;
	}
	else if(ColorPrecision==COLORPRECISION_UBYTE)
	{
		if(NumberOfComponentsForColor!=4)
		{
			strcpy(ErrorMessage, "NumberOfComponentsForColor is not 4 for vertex colors @ AllocateMemoryForColors 250");
			return -1;
		}

		tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForColor/4;
		if(AllocateExact_1(pColorsUINT, &UsedColors, tempo)!=1)
			return 0;
	}
	
	UsedColors=tempo;
	return 1;
}
sint TBaseShape::AllocateMemoryForSecondaryColors()
{
	sint tempo;

	UsedSecondaryColors=0;
	if(UsedVertex==0)
		return 1;

	if(ColorPrecision==COLORPRECISION_32FLOAT)
	{
		tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForSecondaryColor;
		if(AllocateExact_1(pSecondaryColors, &UsedSecondaryColors, tempo)!=1)
			return 0;
	}
	else if(ColorPrecision==COLORPRECISION_UBYTE)
	{
		if(NumberOfComponentsForColor!=4)
		{
			strcpy(ErrorMessage, "NumberOfComponentsForColor is not 4 for vertex colors @ AllocateMemoryForSecondaryColors 278");
			return -1;
		}

		tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForSecondaryColor/4;
		if(AllocateExact_1(pSecondaryColorsUINT, &UsedSecondaryColors, tempo)!=1)
			return 0;
	}
	
	UsedSecondaryColors=tempo;
	return 1;
}
sint TBaseShape::AllocateMemoryForTangentVectors()
{
	sint tempo;

	UsedTangents=0;
	if(UsedVertex==0)
		return 1;
	tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForTangents;
	if(AllocateExact_1(pTangents, &UsedTangents, tempo)!=1)
		return 0;

	UsedTangents=tempo;
	return 1;
}
sint TBaseShape::AllocateMemoryForBinormalVectors()
{
	sint tempo;

	UsedBinormals=0;
	if(UsedVertex==0)
		return 1;
	tempo=(UsedVertex/NumberOfComponentsForVertex)*NumberOfComponentsForBinormals;
	if(AllocateExact_1(pBinormals, &UsedBinormals, tempo)!=1)
		return 0;

	UsedBinormals=tempo;
	return 1;
}

//PURPOSE:	Just allocates memory for the tex coords. For every vertex, 1 tex coord will be stored.
//			If array already allocated, then it *won't* deallocate. Use DeallocateMemoryForTexCoords()
//			for that
//			texUnit must be between 0 and 7. numberOfComponents must be greater than zero.
//			Returns 1 for success, 0 for memory error, -1 if texUnit is out of range.
sint TBaseShape::AllocateMemoryForTexCoords(sint texUnit, sint numberOfComponents)
{
	sint tempo;

	if((texUnit<0)||(texUnit>=NUMBEROFTEXTUREUNITS))
		return -1;
	UsedTexCoords[texUnit]=0;
	if(UsedVertex==0)
		return 1;
	tempo=(UsedVertex/NumberOfComponentsForVertex)*numberOfComponents;
	if(AllocateExact_1(pTexCoords[texUnit], &UsedTexCoords[texUnit], tempo)!=1)
		return 0;
	
	UsedTexCoords[texUnit]=tempo;
	NumberOfComponentsForTexCoords[texUnit]=numberOfComponents;
	return 1;
}


void TBaseShape::DeallocateMemoryForIndex()
{
	if(pIndex16Bit)
	{
		delete [] pIndex16Bit;
		pIndex16Bit=NULL;
		UsedIndex=0;
	}

	if(pIndex32Bit)
	{
		delete [] pIndex32Bit;
		pIndex32Bit=NULL;
		UsedIndex=0;
	}
}

void TBaseShape::DeallocateMemoryForVertex()
{
	if(pVertex)
	{
		delete [] pVertex;
		pVertex=NULL;
		UsedVertex=0;
	}
}

void TBaseShape::DeallocateMemoryForNormals()
{
	if(pNormals)
	{
		delete [] pNormals;
		pNormals=NULL;
		UsedNormals=0;
	}
}

void TBaseShape::DeallocateMemoryForTexCoords(sint texUnit)
{
	if((texUnit<0)||(texUnit>=NUMBEROFTEXTUREUNITS))
		return;
	if(pTexCoords[texUnit])
	{
		delete [] pTexCoords[texUnit];
		pTexCoords[texUnit]=NULL;
		UsedTexCoords[texUnit]=0;
	}
}

void TBaseShape::DeallocateMemoryForColors()
{
	if(pColors)
	{
		delete [] pColors;
		pColors=NULL;
		UsedColors=0;
	}

	if(pColorsUINT)
	{
		delete [] pColorsUINT;
		pColorsUINT=NULL;
		UsedColors=0;
	}
}

void TBaseShape::DeallocateMemoryForSecondaryColors()
{
	if(pSecondaryColors)
	{
		delete [] pSecondaryColors;
		pSecondaryColors=NULL;
		UsedSecondaryColors=0;
	}

	if(pSecondaryColorsUINT)
	{
		delete [] pSecondaryColorsUINT;
		pSecondaryColorsUINT=NULL;
		UsedSecondaryColors=0;
	}
}

void TBaseShape::DeallocateMemoryForTangentVectors()
{
	if(pTangents)
	{
		delete [] pTangents;
		pTangents=NULL;
		UsedTangents=0;
	}
}

void TBaseShape::DeallocateMemoryForBinormalVectors()
{
	if(pBinormals)
	{
		delete [] pBinormals;
		pBinormals=NULL;
		UsedBinormals=0;
	}
}

void TBaseShape::DeallocateMemoryVertexInterlaced()
{
	if(pVertex_V)
	{
		delete [] pVertex_V;
		pVertex_V=NULL;
	}
	if(pVertex_VT)
	{
		delete [] pVertex_VT;
		pVertex_VT=NULL;
	}
	if(pVertex_VNT)
	{
		delete [] pVertex_VNT;
		pVertex_VNT=NULL;
	}
	if(pVertex_VNT3)
	{
		delete [] pVertex_VNT3;
		pVertex_VNT3=NULL;
	}
	if(pVertex_VNTT3T3)
	{
		delete [] pVertex_VNTT3T3;
		pVertex_VNTT3T3=NULL;
	}
	if(pVertex_VNTT3)
	{
		delete [] pVertex_VNTT3;
		pVertex_VNTT3=NULL;
	}

	Start_DrawRangeElements=End_DrawRangeElements=0;
	VertexCount=0;
}

void TBaseShape::FlushAllMemory()
{
	sint i;

	UsedIndex=
		UsedVertex=
		UsedNormals=
		UsedColors=
		UsedSecondaryColors=
		UsedTangents=
		UsedBinormals=0;
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
		UsedTexCoords[i]=0;
}

void TBaseShape::SetColors(sreal red, sreal green, sreal blue, sreal alpha, sreal red2, sreal green2, sreal blue2, sreal alpha2)
{
	Colors[0]=red;
	Colors[1]=green;
	Colors[2]=blue;
	Colors[3]=alpha;
	Colors[4]=red2;
	Colors[5]=blue2;
	Colors[6]=green2;
	Colors[7]=alpha2;
}

void TBaseShape::SetColors(sreal *primaryColor, sreal *secondaryColor)
{
	sint i;

	if(primaryColor)
	{
		for(i=0; i<4; i++) 
			Colors[i]=primaryColor[i];
	}
	else
	{
		for(i=0; i<4; i++) 
			Colors[i]=1.0;
	}
	if(secondaryColor)
	{
		for(i=4; i<8; i++) 
			Colors[i]=secondaryColor[i];
	}
	else
	{
		for(i=4; i<8; i++) 
			Colors[i]=1.0;
	}
}

void TBaseShape::SetColors(uchar red, uchar green, uchar blue, uchar alpha, uchar red2, uchar green2, uchar blue2, uchar alpha2)
{
	ColorsUINT[0]=COLOR_RGBA(red, green, blue, alpha);
	ColorsUINT[1]=COLOR_RGBA(red2, blue2, green2, alpha2);
}

//PURPOSE:
//
//PARAMETERS:
//				[in] primaryColor : NULL (white is written) or single uint (RGBA)
//				[in] secondaryColor : NULL (white is written) or single uint (RGBA)
void TBaseShape::SetColors(uint *primaryColor, uint *secondaryColor)
{
	if(primaryColor)
	{
		ColorsUINT[0]=*primaryColor;
	}
	else
	{
		ColorsUINT[0]=0xFFFFFFFF;
	}
	if(secondaryColor)
	{
		ColorsUINT[1]=*secondaryColor;
	}
	else
	{
		ColorsUINT[1]=0xFFFFFFFF;
	}
}

void TBaseShape::SetMaterialColors(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2, sreal *diffuse2, sreal *emission2, sreal *specular2)
{
	if(ambient)
	{
		MaterialAmbient[0]=ambient[0];
		MaterialAmbient[1]=ambient[1];
		MaterialAmbient[2]=ambient[2];
		MaterialAmbient[3]=ambient[3];
	}
	else
	{
		MaterialAmbient[0]=0.0;
		MaterialAmbient[1]=0.0;
		MaterialAmbient[2]=0.0;
		MaterialAmbient[3]=1.0;
	}
	if(diffuse)
	{
		MaterialDiffuse[0]=diffuse[0];
		MaterialDiffuse[1]=diffuse[1];
		MaterialDiffuse[2]=diffuse[2];
		MaterialDiffuse[3]=diffuse[3];
	}
	else
	{
		MaterialDiffuse[0]=1.0;
		MaterialDiffuse[1]=1.0;
		MaterialDiffuse[2]=1.0;
		MaterialDiffuse[3]=1.0;
	}
	if(emission)
	{
		MaterialEmission[0]=emission[0];
		MaterialEmission[1]=emission[1];
		MaterialEmission[2]=emission[2];
		MaterialEmission[3]=emission[3];
	}
	else
	{
		MaterialEmission[0]=0.0;
		MaterialEmission[1]=0.0;
		MaterialEmission[2]=0.0;
		MaterialEmission[3]=1.0;
	}
	if(specular)
	{
		MaterialSpecular[0]=specular[0];
		MaterialSpecular[1]=specular[1];
		MaterialSpecular[2]=specular[2];
		MaterialSpecular[3]=specular[3];
	}
	else
	{
		MaterialSpecular[0]=1.0;
		MaterialSpecular[1]=1.0;
		MaterialSpecular[2]=1.0;
		MaterialSpecular[3]=1.0;
	}
	if(shininess)
		MaterialShininess=*shininess;
	else
		MaterialShininess=128.0;
	if(ambient2)
	{
		MaterialAmbient[4]=ambient2[0];
		MaterialAmbient[5]=ambient2[1];
		MaterialAmbient[6]=ambient2[2];
		MaterialAmbient[7]=ambient2[3];
	}
	else
	{
		MaterialAmbient[4]=0.0;
		MaterialAmbient[5]=0.0;
		MaterialAmbient[6]=0.0;
		MaterialAmbient[7]=1.0;
	}
	if(diffuse2)
	{
		MaterialDiffuse[4]=diffuse2[0];
		MaterialDiffuse[5]=diffuse2[1];
		MaterialDiffuse[6]=diffuse2[2];
		MaterialDiffuse[7]=diffuse2[3];
	}
	else
	{
		MaterialDiffuse[4]=1.0;
		MaterialDiffuse[5]=1.0;
		MaterialDiffuse[6]=1.0;
		MaterialDiffuse[7]=1.0;
	}
	if(emission2)
	{
		MaterialEmission[4]=emission2[0];
		MaterialEmission[5]=emission2[1];
		MaterialEmission[6]=emission2[2];
		MaterialEmission[7]=emission2[3];
	}
	else
	{
		MaterialEmission[4]=0.0;
		MaterialEmission[5]=0.0;
		MaterialEmission[6]=0.0;
		MaterialEmission[7]=1.0;
	}
	if(specular2)
	{
		MaterialSpecular[4]=specular2[0];
		MaterialSpecular[5]=specular2[1];
		MaterialSpecular[6]=specular2[2];
		MaterialSpecular[7]=specular2[3];
	}
	else
	{
		MaterialSpecular[4]=1.0;
		MaterialSpecular[5]=1.0;
		MaterialSpecular[6]=1.0;
		MaterialSpecular[7]=1.0;
	}
}

void TBaseShape::SetNormalColor(sreal red, sreal green, sreal blue, sreal alpha)
{
	VisibleNormalsColors[0]=red;
	VisibleNormalsColors[1]=green;
	VisibleNormalsColors[2]=blue;
	VisibleNormalsColors[3]=alpha;
}

void TBaseShape::SetNormalColor(uchar red, uchar green, uchar blue, uchar alpha)
{
	VisibleNormalsColorsUINT[0]=COLOR_RGBA(red, green, blue, alpha);
}

void TBaseShape::SetTexCoordComponents(sint texUnit, sint numberOfComponents)
{
	if((texUnit<0)||(texUnit>NUMBEROFTEXTUREUNITS))
		return;

	NumberOfComponentsForTexCoords[texUnit]=numberOfComponents;
}

sint TBaseShape::GenerateVisibleNormals(sreal length)
{
	sint i, j, k, tempo;

	UsedVisibleNormals=0;
	if((UsedVertex==0)||(UsedNormals==0))
		return -1;

	tempo=(UsedVertex/NumberOfComponentsForVertex)*6;
	if(AllocateExact_1(pVisibleNormals, &UsedVisibleNormals, tempo)!=1)
		return 0;

	if((NumberOfComponentsForVertex!=3)||(NumberOfComponentsForNormals!=3))
	{
		for(i=j=k=0; i<UsedVertex; i+=NumberOfComponentsForVertex, j+=6, k+=NumberOfComponentsForNormals)
		{
			pVisibleNormals[j  ]=pVertex[i  ];
			pVisibleNormals[j+1]=pVertex[i+1];
			pVisibleNormals[j+2]=pVertex[i+2];
			pVisibleNormals[j+3]=pVertex[i  ]+length*pNormals[k  ];
			pVisibleNormals[j+4]=pVertex[i+1]+length*pNormals[k+1];
			pVisibleNormals[j+5]=pVertex[i+2]+length*pNormals[k+2];
		}
	}
	else
	{
		for(i=j=0; i<UsedVertex; i+=3, j+=6)
		{
			pVisibleNormals[j  ]=pVertex[i  ];
			pVisibleNormals[j+1]=pVertex[i+1];
			pVisibleNormals[j+2]=pVertex[i+2];
			pVisibleNormals[j+3]=pVertex[i  ]+length*pNormals[i  ];
			pVisibleNormals[j+4]=pVertex[i+1]+length*pNormals[i+1];
			pVisibleNormals[j+5]=pVertex[i+2]+length*pNormals[i+2];
		}
	}

	UsedVisibleNormals=tempo;
	return 1;
}




