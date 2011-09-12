// TCylinder2.cpp: implementation of the TCylinder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TCylinder.h"
#include <math.h>
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include "3DGraphicsLibrarySmall.h"


#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'





//Set color for body, top and bottom
void TCylinder::SetMaterialColors(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2, sreal *diffuse2, sreal *emission2, sreal *specular2)
{
	SetMaterialColors_Body(ambient, diffuse, emission, specular, shininess, ambient2, diffuse2, emission2, specular2);
	SetMaterialColors_Top(ambient, diffuse, emission, specular, shininess, ambient2, diffuse2, emission2, specular2);
	SetMaterialColors_Bottom(ambient, diffuse, emission, specular, shininess, ambient2, diffuse2, emission2, specular2);
}

//Set color for body
void TCylinder::SetMaterialColors_Body(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2, sreal *diffuse2, sreal *emission2, sreal *specular2)
{
	TBaseShape::SetMaterialColors(ambient, diffuse, emission, specular, shininess, ambient2, diffuse2, emission2, specular2);
}

//Set color for top
void TCylinder::SetMaterialColors_Top(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2, sreal *diffuse2, sreal *emission2, sreal *specular2)
{
	if(ambient)
	{
		MaterialAmbient_Top[0]=ambient[0];
		MaterialAmbient_Top[1]=ambient[1];
		MaterialAmbient_Top[2]=ambient[2];
		MaterialAmbient_Top[3]=ambient[3];
	}
	else
	{
		MaterialAmbient_Top[0]=0.0;
		MaterialAmbient_Top[1]=0.0;
		MaterialAmbient_Top[2]=0.0;
		MaterialAmbient_Top[3]=1.0;
	}
	if(diffuse)
	{
		MaterialDiffuse_Top[0]=diffuse[0];
		MaterialDiffuse_Top[1]=diffuse[1];
		MaterialDiffuse_Top[2]=diffuse[2];
		MaterialDiffuse_Top[3]=diffuse[3];
	}
	else
	{
		MaterialDiffuse_Top[0]=1.0;
		MaterialDiffuse_Top[1]=1.0;
		MaterialDiffuse_Top[2]=1.0;
		MaterialDiffuse_Top[3]=1.0;
	}
	if(emission)
	{
		MaterialEmission_Top[0]=emission[0];
		MaterialEmission_Top[1]=emission[1];
		MaterialEmission_Top[2]=emission[2];
		MaterialEmission_Top[3]=emission[3];
	}
	else
	{
		MaterialEmission_Top[0]=0.0;
		MaterialEmission_Top[1]=0.0;
		MaterialEmission_Top[2]=0.0;
		MaterialEmission_Top[3]=1.0;
	}
	if(specular)
	{
		MaterialSpecular_Top[0]=specular[0];
		MaterialSpecular_Top[1]=specular[1];
		MaterialSpecular_Top[2]=specular[2];
		MaterialSpecular_Top[3]=specular[3];
	}
	else
	{
		MaterialSpecular_Top[0]=1.0;
		MaterialSpecular_Top[1]=1.0;
		MaterialSpecular_Top[2]=1.0;
		MaterialSpecular_Top[3]=1.0;
	}
	if(shininess)
		MaterialShininess_Top=*shininess;
	else
		MaterialShininess_Top=128.0;
	if(ambient2)
	{
		MaterialAmbient_Top[4]=ambient2[0];
		MaterialAmbient_Top[5]=ambient2[1];
		MaterialAmbient_Top[6]=ambient2[2];
		MaterialAmbient_Top[7]=ambient2[3];
	}
	else
	{
		MaterialAmbient_Top[4]=0.0;
		MaterialAmbient_Top[5]=0.0;
		MaterialAmbient_Top[6]=0.0;
		MaterialAmbient_Top[7]=1.0;
	}
	if(diffuse2)
	{
		MaterialDiffuse_Top[4]=diffuse2[0];
		MaterialDiffuse_Top[5]=diffuse2[1];
		MaterialDiffuse_Top[6]=diffuse2[2];
		MaterialDiffuse_Top[7]=diffuse2[3];
	}
	else
	{
		MaterialDiffuse_Top[4]=1.0;
		MaterialDiffuse_Top[5]=1.0;
		MaterialDiffuse_Top[6]=1.0;
		MaterialDiffuse_Top[7]=1.0;
	}
	if(emission2)
	{
		MaterialEmission_Top[4]=emission2[0];
		MaterialEmission_Top[5]=emission2[1];
		MaterialEmission_Top[6]=emission2[2];
		MaterialEmission_Top[7]=emission2[3];
	}
	else
	{
		MaterialEmission_Top[4]=0.0;
		MaterialEmission_Top[5]=0.0;
		MaterialEmission_Top[6]=0.0;
		MaterialEmission_Top[7]=1.0;
	}
	if(specular2)
	{
		MaterialSpecular_Top[4]=specular2[0];
		MaterialSpecular_Top[5]=specular2[1];
		MaterialSpecular_Top[6]=specular2[2];
		MaterialSpecular_Top[7]=specular2[3];
	}
	else
	{
		MaterialSpecular_Top[4]=1.0;
		MaterialSpecular_Top[5]=1.0;
		MaterialSpecular_Top[6]=1.0;
		MaterialSpecular_Top[7]=1.0;
	}
}

//Set color for bottom
void TCylinder::SetMaterialColors_Bottom(sreal *ambient, sreal *diffuse, sreal *emission, sreal *specular, sreal *shininess, sreal *ambient2, sreal *diffuse2, sreal *emission2, sreal *specular2)
{
	if(ambient)
	{
		MaterialAmbient_Bottom[0]=ambient[0];
		MaterialAmbient_Bottom[1]=ambient[1];
		MaterialAmbient_Bottom[2]=ambient[2];
		MaterialAmbient_Bottom[3]=ambient[3];
	}
	else
	{
		MaterialAmbient_Bottom[0]=0.0;
		MaterialAmbient_Bottom[1]=0.0;
		MaterialAmbient_Bottom[2]=0.0;
		MaterialAmbient_Bottom[3]=1.0;
	}
	if(diffuse)
	{
		MaterialDiffuse_Bottom[0]=diffuse[0];
		MaterialDiffuse_Bottom[1]=diffuse[1];
		MaterialDiffuse_Bottom[2]=diffuse[2];
		MaterialDiffuse_Bottom[3]=diffuse[3];
	}
	else
	{
		MaterialDiffuse_Bottom[0]=1.0;
		MaterialDiffuse_Bottom[1]=1.0;
		MaterialDiffuse_Bottom[2]=1.0;
		MaterialDiffuse_Bottom[3]=1.0;
	}
	if(emission)
	{
		MaterialEmission_Bottom[0]=emission[0];
		MaterialEmission_Bottom[1]=emission[1];
		MaterialEmission_Bottom[2]=emission[2];
		MaterialEmission_Bottom[3]=emission[3];
	}
	else
	{
		MaterialEmission_Bottom[0]=0.0;
		MaterialEmission_Bottom[1]=0.0;
		MaterialEmission_Bottom[2]=0.0;
		MaterialEmission_Bottom[3]=1.0;
	}
	if(specular)
	{
		MaterialSpecular_Bottom[0]=specular[0];
		MaterialSpecular_Bottom[1]=specular[1];
		MaterialSpecular_Bottom[2]=specular[2];
		MaterialSpecular_Bottom[3]=specular[3];
	}
	else
	{
		MaterialSpecular_Bottom[0]=1.0;
		MaterialSpecular_Bottom[1]=1.0;
		MaterialSpecular_Bottom[2]=1.0;
		MaterialSpecular_Bottom[3]=1.0;
	}
	if(shininess)
		MaterialShininess_Bottom=*shininess;
	else
		MaterialShininess_Bottom=128.0;
	if(ambient2)
	{
		MaterialAmbient_Bottom[4]=ambient2[0];
		MaterialAmbient_Bottom[5]=ambient2[1];
		MaterialAmbient_Bottom[6]=ambient2[2];
		MaterialAmbient_Bottom[7]=ambient2[3];
	}
	else
	{
		MaterialAmbient_Bottom[4]=0.0;
		MaterialAmbient_Bottom[5]=0.0;
		MaterialAmbient_Bottom[6]=0.0;
		MaterialAmbient_Bottom[7]=1.0;
	}
	if(diffuse2)
	{
		MaterialDiffuse_Bottom[4]=diffuse2[0];
		MaterialDiffuse_Bottom[5]=diffuse2[1];
		MaterialDiffuse_Bottom[6]=diffuse2[2];
		MaterialDiffuse_Bottom[7]=diffuse2[3];
	}
	else
	{
		MaterialDiffuse_Bottom[4]=1.0;
		MaterialDiffuse_Bottom[5]=1.0;
		MaterialDiffuse_Bottom[6]=1.0;
		MaterialDiffuse_Bottom[7]=1.0;
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
		MaterialEmission_Bottom[4]=0.0;
		MaterialEmission_Bottom[5]=0.0;
		MaterialEmission_Bottom[6]=0.0;
		MaterialEmission_Bottom[7]=1.0;
	}
	if(specular2)
	{
		MaterialSpecular_Bottom[4]=specular2[0];
		MaterialSpecular_Bottom[5]=specular2[1];
		MaterialSpecular_Bottom[6]=specular2[2];
		MaterialSpecular_Bottom[7]=specular2[3];
	}
	else
	{
		MaterialSpecular_Bottom[4]=1.0;
		MaterialSpecular_Bottom[5]=1.0;
		MaterialSpecular_Bottom[6]=1.0;
		MaterialSpecular_Bottom[7]=1.0;
	}
}

//Set color of body, top and bottom
void TCylinder::SetColors(sreal red, sreal green, sreal blue, sreal alpha, sreal red2, sreal green2, sreal blue2, sreal alpha2)
{
	SetColors_Body(red, green, blue, alpha, red2, green2, blue2, alpha2);
	SetColors_Top(red, green, blue, alpha, red2, green2, blue2, alpha2);
	SetColors_Bottom(red, green, blue, alpha, red2, green2, blue2, alpha2);
}

//Set color of body
void TCylinder::SetColors_Body(sreal red, sreal green, sreal blue, sreal alpha, sreal red2, sreal green2, sreal blue2, sreal alpha2)
{
	TBaseShape::SetColors(red, green, blue, alpha, red2, green2, blue2, alpha2);
}

//Set color of top
void TCylinder::SetColors_Top(sreal red, sreal green, sreal blue, sreal alpha, sreal red2, sreal green2, sreal blue2, sreal alpha2)
{
	Colors_Top[0]=red;
	Colors_Top[1]=green;
	Colors_Top[2]=blue;
	Colors_Top[3]=alpha;
	Colors_Top[4]=red2;
	Colors_Top[5]=blue2;
	Colors_Top[6]=green2;
	Colors_Top[7]=alpha2;
}

//Set color of bottom
void TCylinder::SetColors_Bottom(sreal red, sreal green, sreal blue, sreal alpha, sreal red2, sreal green2, sreal blue2, sreal alpha2)
{
	Colors_Bottom[0]=red;
	Colors_Bottom[1]=green;
	Colors_Bottom[2]=blue;
	Colors_Bottom[3]=alpha;
	Colors_Bottom[4]=red2;
	Colors_Bottom[5]=blue2;
	Colors_Bottom[6]=green2;
	Colors_Bottom[7]=alpha2;
}

//Set color of body, top and bottom
void TCylinder::SetColors(sreal *primaryColor, sreal *secondaryColor)
{
	SetColors_Body(primaryColor, secondaryColor);
	SetColors_Top(primaryColor, secondaryColor);
	SetColors_Bottom(primaryColor, secondaryColor);
}

//Set color of body
void TCylinder::SetColors_Body(sreal *primaryColor, sreal *secondaryColor)
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

//Set color of top
void TCylinder::SetColors_Top(sreal *primaryColor, sreal *secondaryColor)
{
	TBaseShape::SetColors(primaryColor, secondaryColor);
}

//Set color of bottom
void TCylinder::SetColors_Bottom(sreal *primaryColor, sreal *secondaryColor)
{
	sint i;

	if(primaryColor)
	{
		for(i=0; i<4; i++) 
			Colors_Bottom[i]=primaryColor[i];
	}
	else
	{
		for(i=0; i<4; i++) 
			Colors_Bottom[i]=1.0;
	}
	if(secondaryColor)
	{
		for(i=4; i<8; i++) 
			Colors_Bottom[i]=secondaryColor[i];
	}
	else
	{
		for(i=4; i<8; i++) 
			Colors_Bottom[i]=1.0;
	}
}

//Set color of body, top and bottom
void TCylinder::SetColors(uchar red, uchar green, uchar blue, uchar alpha, uchar red2, uchar green2, uchar blue2, uchar alpha2)
{
	SetColors_Body(red, green, blue, alpha, red2, green2, blue2, alpha2);
	SetColors_Top(red, green, blue, alpha, red2, green2, blue2, alpha2);
	SetColors_Bottom(red, green, blue, alpha, red2, green2, blue2, alpha2);
}


//Set color of body
void TCylinder::SetColors_Body(uchar red, uchar green, uchar blue, uchar alpha, uchar red2, uchar green2, uchar blue2, uchar alpha2)
{
	TBaseShape::SetColors(red, green, blue, alpha, red2, green2, blue2, alpha2);
}

//Set color of top
void TCylinder::SetColors_Top(uchar red, uchar green, uchar blue, uchar alpha, uchar red2, uchar green2, uchar blue2, uchar alpha2)
{
	ColorsUINT_Top[0]=COLOR_RGBA(red, green, blue, alpha);
	ColorsUINT_Top[1]=COLOR_RGBA(red2, blue2, green2, alpha2);
}

//Set color of bottom
void TCylinder::SetColors_Bottom(uchar red, uchar green, uchar blue, uchar alpha, uchar red2, uchar green2, uchar blue2, uchar alpha2)
{
	ColorsUINT_Bottom[0]=COLOR_RGBA(red, green, blue, alpha);
	ColorsUINT_Bottom[1]=COLOR_RGBA(red2, blue2, green2, alpha2);
}

//Set color of body, top and bottom
void TCylinder::SetColors(uint *primaryColor, uint *secondaryColor)
{
	SetColors_Body(primaryColor, secondaryColor);
	SetColors_Top(primaryColor, secondaryColor);
	SetColors_Bottom(primaryColor, secondaryColor);
}

//Set color of body
void TCylinder::SetColors_Body(uint *primaryColor, uint *secondaryColor)
{
	TBaseShape::SetColors(primaryColor, secondaryColor);
}

//Set color of top
void TCylinder::SetColors_Top(uint *primaryColor, uint *secondaryColor)
{
	if(primaryColor)
	{
		ColorsUINT_Top[0]=*primaryColor;
	}
	else
	{
		ColorsUINT_Top[0]=0xFFFFFFFF;
	}
	if(secondaryColor)
	{
		ColorsUINT_Top[1]=*secondaryColor;
	}
	else
	{
		ColorsUINT_Top[1]=0xFFFFFFFF;
	}
}

//Set color of bottom
void TCylinder::SetColors_Bottom(uint *primaryColor, uint *secondaryColor)
{
	if(primaryColor)
	{
		ColorsUINT_Bottom[0]=*primaryColor;
	}
	else
	{
		ColorsUINT_Bottom[0]=0xFFFFFFFF;
	}
	if(secondaryColor)
	{
		ColorsUINT_Bottom[1]=*secondaryColor;
	}
	else
	{
		ColorsUINT_Bottom[1]=0xFFFFFFFF;
	}
}

//PURPOSE:	Compute texture coordinates for every vertex. Will only generate a default tex coords, must
//			call translate, rotate scale to reposition the texture right after this.
//			With respect to the OpenGL texture matrix being identity.
//			widthTextureLimit and heightTextureLimit is for the texture size limit for non-OpenGL standard size images,
//			they must be set to 0 or below or 1 or higher if there is no limit. 
//			Will reserve space for texture coordinates.
//			Returns 0 if memory error occurs.
//			style tells the function what kind of texture coordinate generation style is desired :
//			1 is the default wrap around cylinder, with one on top and one on bottom centered.
//			0 will simply reserve memory for pTexCoord.
//
//PARAMETERS:
//			[in] texUnit : must be from 0 to NUMBEROFTEXTUREUNITS-1 inclusively. We need 2 components at minimum.
//			[in] numberOfComponents : 2 (ST), 3 (STR), 4 (STRQ)
//			                          Only 2 is coded.
//			[in] scaleS : 1.0 means that in the S direction, texture is wrapped all around once.
//			[in] scaleT : 1.0 means that in the T direction, texture is wrapped all around once.
//			[in] style : 0 it writes nothing. 1 for normal texcoord generation
//			[in] widthTextureLimit : set this to 1.0 for now
//			[in] heightTextureLimit : set this to 1.0 for now
//
//RETURNS:
//			1 for success, 0 for memory error plus message.
//			-1 if texUnit is out of range.
//			-2 if numberOfComponents is not 2, 3 or 4.
//			-3 if no vertices created plus message.
//			-4 if style is invalid plus message.
//
//WARNING: widthTextureLimit, heightTextureLimit are not used!
//numberOfComponents == 2 is the only one implemented.
sint TCylinder::GenerateTexCoords(sint texUnit, sint numberOfComponents, sreal scaleS, sreal scaleT,
								  sint style, sreal widthTextureLimit, sreal heightTextureLimit)
{
	sint i, tempo, tempo2, tempo3;
	sreal FTempo, FTempo2, angle;

	if((texUnit<0)||(texUnit>=NUMBEROFTEXTUREUNITS))
	{
		strcpy(ErrorMessage, "Tex unit is out of range. @ GenerateTexCoords 496");
		return -1;
	}

	if((numberOfComponents!=2)&&(numberOfComponents!=3)&&(numberOfComponents!=4))
	{
		strcpy(ErrorMessage, "Number of components for texcoord is not 2, 3 or 4. @ GenerateTexCoords 503");
		return -1;
	}

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. Can't compute texcoords. @ GenerateTexCoords 488");
		return -3;
	}

	if((style!=0)&&(style!=1))
	{
		strcpy(ErrorMessage, "Style is invalid. @ GenerateTexCoords 495");
		return -4;
	}

	UsedTexCoords[texUnit]=0;
	//DeallocateMemoryForTexCoords(texUnit);
	NumberOfComponentsForTexCoords[texUnit]=numberOfComponents;


	StartIndexIn_pTexCoords_BodyOfCylinder[texUnit]=EndIndexIn_pTexCoords_BodyOfCylinder[texUnit]=0;
	StartIndexIn_pTexCoords_TopOfCylinder[texUnit]=EndIndexIn_pTexCoords_TopOfCylinder[texUnit]=0;
	StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]=EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]=0;
	pTexCoords_BodyOfCylinder[texUnit]=pTexCoords_TopOfCylinder[texUnit]=pTexCoords_BottomOfCylinder[texUnit]=NULL;
	TotalTexCoords_BodyOfCylinder[texUnit]=TotalTexCoords_TopOfCylinder[texUnit]=TotalTexCoords_BottomOfCylinder[texUnit]=0;

	//one texcoord per vertex
	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForTexCoords[texUnit];
	AllocateExact_1(pTexCoords[texUnit], &UsedTexCoords[texUnit], tempo);

	UsedTexCoords[texUnit]=tempo;

	if(style==1)
	{
		if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
		{
			if(NumberOfComponentsForTexCoords[texUnit]==2)
			{
				StartIndexIn_pTexCoords_BodyOfCylinder[texUnit]=0;
				EndIndexIn_pTexCoords_BodyOfCylinder[texUnit]=EndIndexIn_pVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTexCoords[texUnit];
				TotalTexCoords_BodyOfCylinder[texUnit]=EndIndexIn_pTexCoords_BodyOfCylinder[texUnit]-StartIndexIn_pTexCoords_BodyOfCylinder[texUnit];
				pTexCoords_BodyOfCylinder[texUnit]=pTexCoords[texUnit];

				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pTexCoords[texUnit][0]=0.0;
				pTexCoords[texUnit][1]=0.0;
				tempo2=Slices*NumberOfComponentsForTexCoords[texUnit];
				for(i=2; i<tempo2; i+=2)
				{
					pTexCoords[texUnit][i  ]=(pTexCoords[texUnit][i-2]+FTempo)*scaleS;
					pTexCoords[texUnit][i+1]=0.0;
				}
				pTexCoords[texUnit][i  ]=1.0*scaleS;
				pTexCoords[texUnit][i+1]=0.0;

				tempo2=EndIndexIn_pTexCoords_BodyOfCylinder[texUnit];
				tempo3=SlicesPlusOne*NumberOfComponentsForTexCoords[texUnit];
				for(i+=2; i<tempo2; i+=2)
				{
					pTexCoords[texUnit][i  ]=pTexCoords[texUnit][i-tempo3];
					pTexCoords[texUnit][i+1]=(pTexCoords[texUnit][i-tempo3+1]+FTempo2)*scaleT;
				}

				if(IsThereATop)
				{
					tempo2=(1+SlicesPlusOne)*NumberOfComponentsForTexCoords[texUnit];		//+1 for the center
					StartIndexIn_pTexCoords_TopOfCylinder[texUnit]=EndIndexIn_pTexCoords_BodyOfCylinder[texUnit];
					EndIndexIn_pTexCoords_TopOfCylinder[texUnit]=StartIndexIn_pTexCoords_TopOfCylinder[texUnit]+tempo2;
					TotalTexCoords_TopOfCylinder[texUnit]=EndIndexIn_pTexCoords_TopOfCylinder[texUnit]-StartIndexIn_pTexCoords_TopOfCylinder[texUnit];
					pTexCoords_TopOfCylinder[texUnit]=&pTexCoords[texUnit][StartIndexIn_pTexCoords_TopOfCylinder[texUnit]];

					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);
					
					i=StartIndexIn_pTexCoords_TopOfCylinder[texUnit];
					pTexCoords[texUnit][i  ]=0.5;		//Center of texture
					pTexCoords[texUnit][i+1]=0.5;		//Center of texture
					tempo3=EndIndexIn_pTexCoords_TopOfCylinder[texUnit]-2;
					for(i+=2; i<tempo3; i+=2, angle+=FTempo)
					{
						pTexCoords[texUnit][i  ]=(0.5*cosf(angle)+0.5)*scaleS;
						pTexCoords[texUnit][i+1]=(0.5*sinf(angle)+0.5)*scaleT;
					}

					//Fix last one cause it's the same point as the first
					pTexCoords[texUnit][i  ]=pTexCoords[texUnit][StartIndexIn_pTexCoords_TopOfCylinder[texUnit]+2];
					pTexCoords[texUnit][i+1]=pTexCoords[texUnit][StartIndexIn_pTexCoords_TopOfCylinder[texUnit]+3];
				}
				if(IsThereABottom)
				{
					tempo2=(1+SlicesPlusOne)*NumberOfComponentsForTexCoords[texUnit];		//+1 for the center

					if(IsThereATop==FALSE)
					{
						StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]=EndIndexIn_pTexCoords_BodyOfCylinder[texUnit];
						EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]=StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]+tempo2;
						TotalTexCoords_BottomOfCylinder[texUnit]=EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]-StartIndexIn_pTexCoords_BottomOfCylinder[texUnit];
						pTexCoords_BottomOfCylinder[texUnit]=&pTexCoords[texUnit][StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]];
					}
					else
					{
						StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]=EndIndexIn_pTexCoords_TopOfCylinder[texUnit];
						EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]=StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]+tempo2;
						TotalTexCoords_BottomOfCylinder[texUnit]=EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]-StartIndexIn_pTexCoords_BottomOfCylinder[texUnit];
						pTexCoords_BottomOfCylinder[texUnit]=&pTexCoords[texUnit][StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]];
					}
					
					//Read the part with ****
					//Compute the coordinates starting at i=TotalTexCoordTopAndBottomHalf;
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pTexCoords_BottomOfCylinder[texUnit];
					pTexCoords[texUnit][i  ]=0.5;		//Center of texture
					pTexCoords[texUnit][i+1]=0.5;		//Center of texture
					tempo3=EndIndexIn_pTexCoords_BottomOfCylinder[texUnit]-2;
					for(i+=2; i<tempo3; i+=2, angle-=FTempo)	//**** Since vertices go in opposite direction with respect to top of cylinder vertices, angle-=FTempo
					{
						pTexCoords[texUnit][i  ]=(0.5+0.5*cosf(angle))*scaleS;
						pTexCoords[texUnit][i+1]=(0.5+0.5*sinf(angle))*scaleT;
					}

					//Fix last one cause it's the same point as the first
					pTexCoords[texUnit][i  ]=pTexCoords[texUnit][StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]+2];
					pTexCoords[texUnit][i+1]=pTexCoords[texUnit][StartIndexIn_pTexCoords_BottomOfCylinder[texUnit]+3];
				}
			}
		}
	}

	return 1;
}

//PURPOSE:	Be sure to call GenerateNormals() before calling this.
//			WE NEED TO GENERATE BOTH VECTORS AT THE SAME TIME.
//			Indices are required.
//			Vertices are required.
//			Normals are required.
//			Texcoord[0] is required.
//			
//
//RETURNS:
//			1 for success.
//			0 for memory error.
//			-1 if UsedIndex<=0
//			-2 if UsedVertex<=0
//			-3 if UsedNormals<=0
//			-4 if UsedTexCoords[0]<=0
//			-5 if bad value passed to ComputeTangentAndBinormalVectors_v1_FLOAT_1 or ComputeTangentAndBinormalVectors_v1_FLOAT_2
sint TCylinder::GenerateTangentAndBinormalVectors()
{
	sint tempo, tempo2, returnVal;

	if(UsedIndex<=0)
	{
		strcpy(ErrorMessage, "Index count is zero or less. @ GenerateTangentAndBinormalVectors 808");
		return -1;
	}

	if(UsedVertex<=0)
	{
		strcpy(ErrorMessage, "Vertex count is zero or less. @ GenerateTangentAndBinormalVectors 814");
		return -2;
	}

	if(UsedNormals<=0)
	{
		strcpy(ErrorMessage, "Normal count is zero or less. @ GenerateTangentAndBinormalVectors 820");
		return -3;
	}

	if(UsedTexCoords[0]<=0)
	{
		strcpy(ErrorMessage, "Texcoord[0] count is zero or less. @ GenerateTangentAndBinormalVectors 826");
		return -4;
	}

	UsedTangents=0;
	UsedBinormals=0;
	//DeallocateMemoryForTangentVectors();
	//DeallocateMemoryForBinormalVectors();

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

	//One per vertex
	tempo=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	AllocateExact_1(pTangents, &UsedTangents, tempo);

	tempo2=UsedVertex/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	AllocateExact_1(pBinormals, &UsedBinormals, tempo);

	if(IndexFormat==GLH_INDEXFORMAT_16BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_2(UsedIndex, pIndex16Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 868");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 873");
			return 0;
		}
	}
	else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
	{
		returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT(UsedIndex, pIndex32Bit, UsedVertex, NumberOfComponentsForVertex, pVertex, NumberOfComponentsForNormals, pNormals, NumberOfComponentsForTexCoords[0],
			pTexCoords[0], NumberOfComponentsForTangents, pTangents, NumberOfComponentsForBinormals, pBinormals);
		if(returnVal==-1)
		{
			strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 883");
			return -5;
		}
		else if(returnVal==0)
		{
			strcpy(ErrorMessage, "Memory error detected @ GenerateTangentAndBinormalVectors 888");
			return 0;
		}
	}

	StartIndexIn_pTangents_BodyOfCylinder=StartIndexIn_pVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	EndIndexIn_pTangents_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	StartIndexIn_pTangents_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	EndIndexIn_pTangents_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	StartIndexIn_pTangents_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	EndIndexIn_pTangents_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	pTangents_BodyOfCylinder=pTangents;
	pTangents_TopOfCylinder=&pTangents[StartIndexIn_pTangents_TopOfCylinder];
	pTangents_BottomOfCylinder=&pTangents[StartIndexIn_pTangents_BottomOfCylinder];
	TotalTangents_BodyOfCylinder=TotalVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	TotalTangents_TopOfCylinder=TotalVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;
	TotalTangents_BottomOfCylinder=TotalVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForTangents;

	StartIndexIn_pBinormals_BodyOfCylinder=StartIndexIn_pVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	EndIndexIn_pBinormals_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	StartIndexIn_pBinormals_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	EndIndexIn_pBinormals_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	StartIndexIn_pBinormals_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	EndIndexIn_pBinormals_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	pBinormals_BodyOfCylinder=pBinormals;
	pBinormals_TopOfCylinder=&pBinormals[StartIndexIn_pBinormals_TopOfCylinder];
	pBinormals_BottomOfCylinder=&pBinormals[StartIndexIn_pBinormals_BottomOfCylinder];
	TotalBinormals_BodyOfCylinder=TotalVertex_BodyOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	TotalBinormals_TopOfCylinder=TotalVertex_TopOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;
	TotalBinormals_BottomOfCylinder=TotalVertex_BottomOfCylinder/NumberOfComponentsForVertex*NumberOfComponentsForBinormals;

	UsedTangents=tempo;
	UsedBinormals=tempo2;

	return 1;
}


//PURPOSE:	If there won't be a top or bottom, then those won't be computed so some memory will be saved.
//
//RETURN:
//			1 for success
sint TCylinder::GenerateVertices2()
{
	sint i, tempo, tempo3, tempo5, tempo6, returnVal;
	sreal FTempo, FTempo2, phi;
	sreal radialIncrement;
	sint slicesLessOne;
	sreal heightIncrement;
	sreal phiIncrement;

	heightIncrement=Height/(sreal)Stacks;
	phiIncrement=(2.0*PI_DEF)/(sreal)Slices;

	//This is just for the body for now, later it is increased
	VertexCount=(SlicesPlusOne*(Stacks+1));		//Stacks+1 for the last circle on the bottom of cylinder

	StartIndexIn_pVertex_BodyOfCylinder=0;
	EndIndexIn_pVertex_BodyOfCylinder=VertexCount;
	TotalVertex_BodyOfCylinder=EndIndexIn_pVertex_BodyOfCylinder-StartIndexIn_pVertex_BodyOfCylinder;
	pVertex_BodyOfCylinder=NULL;

	Start_DrawRangeElements_BodyOfCylinder=0;
	End_DrawRangeElements_BodyOfCylinder=VertexCount-1;

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

	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_V=new GLHVertex_V[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_V[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_V;
		}
		else
		{	//No top and no bottom
			pVertex_V=new GLHVertex_V[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_V;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VT[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_VT;
		}
		else
		{	//No top and no bottom
			pVertex_VT=new GLHVertex_VT[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_VT;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_VNT;
		}
		else
		{	//No top and no bottom
			pVertex_VNT=new GLHVertex_VNT[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_VNT;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNT3[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_VNT3;
		}
		else
		{	//No top and no bottom
			pVertex_VNT3=new GLHVertex_VNT3[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_VNT3;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3T3[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_VNTT3T3;
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_VNTT3T3;
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if((IsThereATop)||(IsThereABottom))
		{
			if((IsThereATop)&&(IsThereABottom))
			{
				tempo5=SlicesPlusOne+1;
				tempo6=2*tempo5;														//+1 because of center point, *2 because top and bottom faces of cylinder
			}
			else	//One or the other
				tempo5=tempo6=SlicesPlusOne+1;											//+1 because of center point

			VertexCount+=tempo6;
			//Allocate space for the top and bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];

			if(IsThereATop)
			{
				pVertex_TopOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_BodyOfCylinder].x;
				StartIndexIn_pVertex_TopOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder+tempo5;
				TotalVertex_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-StartIndexIn_pVertex_TopOfCylinder;

				Start_DrawRangeElements_TopOfCylinder=StartIndexIn_pVertex_TopOfCylinder;
				End_DrawRangeElements_TopOfCylinder=EndIndexIn_pVertex_TopOfCylinder-1;

				if(IsThereABottom)
				{
					pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_TopOfCylinder].x;	//<<<<<<<<
					StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_TopOfCylinder;
					EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
					TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

					Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
					End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
				}
			}
			else //if(IsThereABottom)
			{
				pVertex_BottomOfCylinder=(sreal *)&pVertex_VNTT3[EndIndexIn_pVertex_BodyOfCylinder].x;	//<<<<<<<<
				StartIndexIn_pVertex_BottomOfCylinder=EndIndexIn_pVertex_BodyOfCylinder;
				EndIndexIn_pVertex_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder+tempo5;
				TotalVertex_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-StartIndexIn_pVertex_BottomOfCylinder;

				Start_DrawRangeElements_BottomOfCylinder=StartIndexIn_pVertex_BottomOfCylinder;
				End_DrawRangeElements_BottomOfCylinder=EndIndexIn_pVertex_BottomOfCylinder-1;
			}

			pVertex_BodyOfCylinder=(sreal *)pVertex_VNTT3;
		}
		else
		{	//No top and no bottom
			pVertex_VNTT3=new GLHVertex_VNTT3[VertexCount];
			pVertex_BodyOfCylinder=(sreal *)pVertex_VNTT3;
		}
	}


	if(VertexFormat==GLH_VERTEXFORMAT_V)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_V[i].x=FTempo*(sreal)cosf(phi);
			pVertex_V[i].y=FTempo*(sreal)sinf(phi);
			pVertex_V[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
				pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
				pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_V[i].x=0.0;				//This is the center point at top
			pVertex_V[i].y=0.0;
			pVertex_V[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;

			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_V[i].x=FTempo*(sreal)cosf(phi);
				pVertex_V[i].y=FTempo*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
					pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
					pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_V[i].x=0.0;					//This is the center point at bottom
			pVertex_V[i].y=0.0;
			pVertex_V[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_V[i].x=FTempo*(sreal)cosf(phi);
				pVertex_V[i].y=FTempo*(sreal)sinf(phi);
				pVertex_V[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_V[i].x=pVertex_V[i-slicesLessOne].x;
					pVertex_V[i].y=pVertex_V[i-slicesLessOne].y;
					pVertex_V[i].z=pVertex_V[i-slicesLessOne].z;
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
	else if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
				pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
				pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VT[i].x=0.0;				//This is the center point at top
			pVertex_VT[i].y=0.0;
			pVertex_VT[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;

			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
					pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
					pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VT[i].x=0.0;				//This is the center point at bottom
			pVertex_VT[i].y=0.0;
			pVertex_VT[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_VT[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VT[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VT[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VT[i].x=pVertex_VT[i-slicesLessOne].x;
					pVertex_VT[i].y=pVertex_VT[i-slicesLessOne].y;
					pVertex_VT[i].z=pVertex_VT[i-slicesLessOne].z;
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

		if(TexCoordStyle==1)
			GenerateTexCoords2(1.0, 1.0);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
				pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
				pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNT[i].x=0.0;				//This is the center point at top
			pVertex_VNT[i].y=0.0;
			pVertex_VNT[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;

			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
					pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
					pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNT[i].x=0.0;				//This is the center point at bottom
			pVertex_VNT[i].y=0.0;
			pVertex_VNT[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_VNT[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNT[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNT[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNT[i].x=pVertex_VNT[i-slicesLessOne].x;
					pVertex_VNT[i].y=pVertex_VNT[i-slicesLessOne].y;
					pVertex_VNT[i].z=pVertex_VNT[i-slicesLessOne].z;
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

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(1.0, 1.0);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
				pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
				pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNT3[i].x=0.0;				//This is the center point at top
			pVertex_VNT3[i].y=0.0;
			pVertex_VNT3[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;
			
			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
					pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
					pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			//Allocate space for the top

			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNT3[i].x=0.0;				//This is the center point at bottom
			pVertex_VNT3[i].y=0.0;
			pVertex_VNT3[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_VNT3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNT3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNT3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNT3[i].x=pVertex_VNT3[i-slicesLessOne].x;
					pVertex_VNT3[i].y=pVertex_VNT3[i-slicesLessOne].y;
					pVertex_VNT3[i].z=pVertex_VNT3[i-slicesLessOne].z;
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

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(1.0, 1.0);
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3T3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
				pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
				pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNTT3T3[i].x=0.0;				//This is the center point at top
			pVertex_VNTT3T3[i].y=0.0;
			pVertex_VNTT3T3[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;
			
			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
					pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
					pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			//Allocate space for the top

			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNTT3T3[i].x=0.0;			//This is the center point at bottom
			pVertex_VNTT3T3[i].y=0.0;
			pVertex_VNTT3T3[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_VNTT3T3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNTT3T3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNTT3T3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNTT3T3[i].x=pVertex_VNTT3T3[i-slicesLessOne].x;
					pVertex_VNTT3T3[i].y=pVertex_VNTT3T3[i-slicesLessOne].y;
					pVertex_VNTT3T3[i].z=pVertex_VNTT3T3[i-slicesLessOne].z;
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

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(1.0, 1.0);

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1971");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 1980");
				return -1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		radialIncrement=(BRadius-ARadius)/(sreal)Stacks;			//From BRadius to ARadius


		FTempo2=Height*0.5;		//POSITION_CENTER, start from ARadius, go down to BRadius

		slicesLessOne=Slices;

		//*********** THIS IS FOR BODY OF CYLINDER
		phi=0.0;
		FTempo=ARadius;

		//Make sure bottom row is exactly FTempo2=-Height*0.5;
		//and radius is exactly BRadius
		tempo3=TotalVertex_BodyOfCylinder-SlicesPlusOne;
		for(i=0, tempo=2; i<tempo3; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
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
		for(; i<TotalVertex_BodyOfCylinder; i++)
		{
			pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
			pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
			pVertex_VNTT3[i].z=FTempo2;
			if(tempo==SlicesPlusOne)
			{	//Must wrap around the cylinder
				i++;
				pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
				pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
				pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
				//We can call break here
			}
			else
			{
				tempo++;
				phi+=phiIncrement;
			}
		}


		//*********** END ***************
		//*********** THIS FOR TOP MESH OF TOP OF CYLINDER ************
		if(IsThereATop)
		{
			FTempo2=Height*0.5;		//POSITION_CENTER

			i=StartIndexIn_pVertex_TopOfCylinder;
			pVertex_VNTT3[i].x=0.0;				//This is the center point at top
			pVertex_VNTT3[i].y=0.0;
			pVertex_VNTT3[i].z=FTempo2;
			FTempo=ARadius;
			phi=0.0;
			
			for(i++, tempo=2; i<EndIndexIn_pVertex_TopOfCylinder; i++)
			{
				pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
					pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
					pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
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

		//*********** END ***************
		//*********** THIS IS FOR MESH OF BOTTOM OF CYLINDER 
		if(IsThereABottom)
		{
			//Allocate space for the top

			FTempo2=-Height*0.5;		//POSITION_CENTER


			i=StartIndexIn_pVertex_BottomOfCylinder;
			pVertex_VNTT3[i].x=0.0;				//This is the center point at bottom
			pVertex_VNTT3[i].y=0.0;
			pVertex_VNTT3[i].z=FTempo2;
			FTempo=BRadius;
			phi=0.0;
			for(i++, tempo=2; i<EndIndexIn_pVertex_BottomOfCylinder; i++)
			{
				pVertex_VNTT3[i].x=FTempo*(sreal)cosf(phi);
				pVertex_VNTT3[i].y=FTempo*(sreal)sinf(phi);
				pVertex_VNTT3[i].z=FTempo2;
				if(tempo==SlicesPlusOne)
				{
					i++;
					pVertex_VNTT3[i].x=pVertex_VNTT3[i-slicesLessOne].x;
					pVertex_VNTT3[i].y=pVertex_VNTT3[i-slicesLessOne].y;
					pVertex_VNTT3[i].z=pVertex_VNTT3[i-slicesLessOne].z;
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

		GenerateNormals2();

		if(TexCoordStyle==1)
			GenerateTexCoords2(1.0, 1.0);

		//We need to make the pVertex_VNTT3T3 format because ComputeTangentAndBinormalVectors_v1_FLOAT_4 and _3 needs it.
		pVertex_VNTT3T3=new GLHVertex_VNTT3T3[VertexCount];

		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3T3[i].x=pVertex_VNTT3[i].x;
			pVertex_VNTT3T3[i].y=pVertex_VNTT3[i].y;
			pVertex_VNTT3T3[i].z=pVertex_VNTT3[i].z;
			pVertex_VNTT3T3[i].nx=pVertex_VNTT3[i].nx;
			pVertex_VNTT3T3[i].ny=pVertex_VNTT3[i].ny;
			pVertex_VNTT3T3[i].nz=pVertex_VNTT3[i].nz;
			pVertex_VNTT3T3[i].s0=pVertex_VNTT3[i].s0;
			pVertex_VNTT3T3[i].t0=pVertex_VNTT3[i].t0;
		}

		if(IndexFormat==GLH_INDEXFORMAT_16BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_4(UsedIndex, pIndex16Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 2042");
				return -1;
			}
		}
		else if(IndexFormat==GLH_INDEXFORMAT_32BIT)
		{
			returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_3(UsedIndex, pIndex32Bit, VertexCount, pVertex_VNTT3T3);
			if(returnVal==-1)
			{
				strcpy(ErrorMessage, "Bad value passed.  @ GenerateTangentAndBinormalVectors 2051");
				return -1;
			}
		}

		//Copy the tangents
		for(i=0; i<VertexCount; i++)
		{
			pVertex_VNTT3[i].s1=pVertex_VNTT3T3[i].s1;
			pVertex_VNTT3[i].t1=pVertex_VNTT3T3[i].t1;
			pVertex_VNTT3[i].r1=pVertex_VNTT3T3[i].r1;
		}

		delete [] pVertex_VNTT3T3;
		pVertex_VNTT3T3=NULL;
	}



	Start_DrawRangeElements=0;
	if(IsThereATop)
	{
		if(IsThereABottom)
		{
			End_DrawRangeElements=End_DrawRangeElements_BottomOfCylinder;
		}
		else
		{
			End_DrawRangeElements=End_DrawRangeElements_TopOfCylinder;
		}
	}
	else if(IsThereABottom)
	{
		End_DrawRangeElements=End_DrawRangeElements_BottomOfCylinder;
	}
	else
	{
		End_DrawRangeElements=End_DrawRangeElements_BodyOfCylinder;
	}

	return 1;
}

//PURPOSE:	Generate the normals for one of the vertex formats.
//			Since the cylinder can have a different radius for the top and bottom, computing
//			the normals needs to take into account the angle.
//
//RETURN:
//			1 if successful
//			-1 if the VertexFormat is not supported
sint TCylinder::GenerateNormals2()
{
	sint i, j, k, m, tempo2;
	sreal vector1[3], vector2[3], vector3[3];
	sreal vector4[3], vector5[3], vector6[3];

	if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		//Normals
		//**** Start with the body of cylinder. Vertex - center
		//tempo2=TotalVertex_BodyOfCylinder;

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT[i+1].x-pVertex_VNT[i].x;
				vector1[1]=pVertex_VNT[i+1].y-pVertex_VNT[i].y;
				vector1[2]=pVertex_VNT[i+1].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT[i+SlicesPlusOne].x-pVertex_VNT[i].x;
				vector2[1]=pVertex_VNT[i+SlicesPlusOne].y-pVertex_VNT[i].y;
				vector2[2]=pVertex_VNT[i+SlicesPlusOne].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i+Slices-1].x;
				vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i+Slices-1].y;
				vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT[i+Slices-1+SlicesPlusOne].x-pVertex_VNT[i+Slices-1].x;
				vector5[1]=pVertex_VNT[i+Slices-1+SlicesPlusOne].y-pVertex_VNT[i+Slices-1].y;
				vector5[2]=pVertex_VNT[i+Slices-1+SlicesPlusOne].z-pVertex_VNT[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT[i].nx=vector3[0]+vector6[0];
				pVertex_VNT[i].ny=vector3[1]+vector6[1];
				pVertex_VNT[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

				pVertex_VNT[i].nx=-pVertex_VNT[i].nx;
				pVertex_VNT[i].ny=-pVertex_VNT[i].ny;
				pVertex_VNT[i].nz=-pVertex_VNT[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT[i+1].x-pVertex_VNT[i].x;
				vector1[1]=pVertex_VNT[i+1].y-pVertex_VNT[i].y;
				vector1[2]=pVertex_VNT[i+1].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT[i+SlicesPlusOne].x-pVertex_VNT[i].x;
				vector2[1]=pVertex_VNT[i+SlicesPlusOne].y-pVertex_VNT[i].y;
				vector2[2]=pVertex_VNT[i+SlicesPlusOne].z-pVertex_VNT[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT[i].x-pVertex_VNT[i-1].x;
				vector4[1]=pVertex_VNT[i].y-pVertex_VNT[i-1].y;
				vector4[2]=pVertex_VNT[i].z-pVertex_VNT[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT[i-1+SlicesPlusOne].x-pVertex_VNT[i-1].x;
				vector5[1]=pVertex_VNT[i-1+SlicesPlusOne].y-pVertex_VNT[i-1].y;
				vector5[2]=pVertex_VNT[i-1+SlicesPlusOne].z-pVertex_VNT[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT[i].nx=vector3[0]+vector6[0];
				pVertex_VNT[i].ny=vector3[1]+vector6[1];
				pVertex_VNT[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT[i].nx);

				pVertex_VNT[i].nx=-pVertex_VNT[i].nx;
				pVertex_VNT[i].ny=-pVertex_VNT[i].ny;
				pVertex_VNT[i].nz=-pVertex_VNT[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNT[i].nx=pVertex_VNT[0].nx;
		pVertex_VNT[i].ny=pVertex_VNT[0].ny;
		pVertex_VNT[i].nz=pVertex_VNT[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNT[i].nx=pVertex_VNT[m].nx;
				pVertex_VNT[i].ny=pVertex_VNT[m].ny;
				pVertex_VNT[i].nz=pVertex_VNT[m].nz;
			}
		}

		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			tempo2=EndIndexIn_pVertex_TopOfCylinder;

			pVertex_VNT[i].nx=0.0;					//They all point to positive z
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNT[i].nx=pVertex_VNT[j].nx;
				pVertex_VNT[i].ny=pVertex_VNT[j].ny;
				pVertex_VNT[i].nz=pVertex_VNT[j].nz;
			}
		}

		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder;

			pVertex_VNT[i].nx=0.0;
			pVertex_VNT[i].ny=0.0;
			pVertex_VNT[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT[i].nx=pVertex_VNT[j].nx;
				pVertex_VNT[i].ny=pVertex_VNT[j].ny;
				pVertex_VNT[i].nz=pVertex_VNT[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		//Normals
		//**** Start with the body of cylinder. Vertex - center

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT3[i+1].x-pVertex_VNT3[i].x;
				vector1[1]=pVertex_VNT3[i+1].y-pVertex_VNT3[i].y;
				vector1[2]=pVertex_VNT3[i+1].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT3[i+SlicesPlusOne].x-pVertex_VNT3[i].x;
				vector2[1]=pVertex_VNT3[i+SlicesPlusOne].y-pVertex_VNT3[i].y;
				vector2[2]=pVertex_VNT3[i+SlicesPlusOne].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i+Slices-1].x;
				vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i+Slices-1].y;
				vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].x-pVertex_VNT3[i+Slices-1].x;
				vector5[1]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].y-pVertex_VNT3[i+Slices-1].y;
				vector5[2]=pVertex_VNT3[i+Slices-1+SlicesPlusOne].z-pVertex_VNT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

				pVertex_VNT3[i].nx=-pVertex_VNT3[i].nx;
				pVertex_VNT3[i].ny=-pVertex_VNT3[i].ny;
				pVertex_VNT3[i].nz=-pVertex_VNT3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNT3[i+1].x-pVertex_VNT3[i].x;
				vector1[1]=pVertex_VNT3[i+1].y-pVertex_VNT3[i].y;
				vector1[2]=pVertex_VNT3[i+1].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNT3[i+SlicesPlusOne].x-pVertex_VNT3[i].x;
				vector2[1]=pVertex_VNT3[i+SlicesPlusOne].y-pVertex_VNT3[i].y;
				vector2[2]=pVertex_VNT3[i+SlicesPlusOne].z-pVertex_VNT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNT3[i].x-pVertex_VNT3[i-1].x;
				vector4[1]=pVertex_VNT3[i].y-pVertex_VNT3[i-1].y;
				vector4[2]=pVertex_VNT3[i].z-pVertex_VNT3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNT3[i-1+SlicesPlusOne].x-pVertex_VNT3[i-1].x;
				vector5[1]=pVertex_VNT3[i-1+SlicesPlusOne].y-pVertex_VNT3[i-1].y;
				vector5[2]=pVertex_VNT3[i-1+SlicesPlusOne].z-pVertex_VNT3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNT3[i].nx);

				pVertex_VNT3[i].nx=-pVertex_VNT3[i].nx;
				pVertex_VNT3[i].ny=-pVertex_VNT3[i].ny;
				pVertex_VNT3[i].nz=-pVertex_VNT3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNT3[i].nx=pVertex_VNT3[0].nx;
		pVertex_VNT3[i].ny=pVertex_VNT3[0].ny;
		pVertex_VNT3[i].nz=pVertex_VNT3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNT3[i].nx=pVertex_VNT3[m].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[m].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[m].nz;
			}
		}

		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			tempo2=EndIndexIn_pVertex_TopOfCylinder;

			pVertex_VNT3[i].nx=0.0;					//They all point to positive z
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNT3[i].nx=pVertex_VNT3[j].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[j].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[j].nz;
			}
		}

		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder;

			pVertex_VNT3[i].nx=0.0;
			pVertex_VNT3[i].ny=0.0;
			pVertex_VNT3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNT3[i].nx=pVertex_VNT3[j].nx;
				pVertex_VNT3[i].ny=pVertex_VNT3[j].ny;
				pVertex_VNT3[i].nz=pVertex_VNT3[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		//Normals
		//**** Start with the body of cylinder. Vertex - center

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3T3[i+1].x-pVertex_VNTT3T3[i].x;
				vector1[1]=pVertex_VNTT3T3[i+1].y-pVertex_VNTT3T3[i].y;
				vector1[2]=pVertex_VNTT3T3[i+1].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3T3[i+SlicesPlusOne].x-pVertex_VNTT3T3[i].x;
				vector2[1]=pVertex_VNTT3T3[i+SlicesPlusOne].y-pVertex_VNTT3T3[i].y;
				vector2[2]=pVertex_VNTT3T3[i+SlicesPlusOne].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i+Slices-1].x;
				vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i+Slices-1].y;
				vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].x-pVertex_VNTT3T3[i+Slices-1].x;
				vector5[1]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].y-pVertex_VNTT3T3[i+Slices-1].y;
				vector5[2]=pVertex_VNTT3T3[i+Slices-1+SlicesPlusOne].z-pVertex_VNTT3T3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3T3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3T3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3T3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

				pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[i].nx;
				pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[i].ny;
				pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3T3[i+1].x-pVertex_VNTT3T3[i].x;
				vector1[1]=pVertex_VNTT3T3[i+1].y-pVertex_VNTT3T3[i].y;
				vector1[2]=pVertex_VNTT3T3[i+1].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3T3[i+SlicesPlusOne].x-pVertex_VNTT3T3[i].x;
				vector2[1]=pVertex_VNTT3T3[i+SlicesPlusOne].y-pVertex_VNTT3T3[i].y;
				vector2[2]=pVertex_VNTT3T3[i+SlicesPlusOne].z-pVertex_VNTT3T3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3T3[i].x-pVertex_VNTT3T3[i-1].x;
				vector4[1]=pVertex_VNTT3T3[i].y-pVertex_VNTT3T3[i-1].y;
				vector4[2]=pVertex_VNTT3T3[i].z-pVertex_VNTT3T3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3T3[i-1+SlicesPlusOne].x-pVertex_VNTT3T3[i-1].x;
				vector5[1]=pVertex_VNTT3T3[i-1+SlicesPlusOne].y-pVertex_VNTT3T3[i-1].y;
				vector5[2]=pVertex_VNTT3T3[i-1+SlicesPlusOne].z-pVertex_VNTT3T3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3T3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3T3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3T3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3T3[i].nx);

				pVertex_VNTT3T3[i].nx=-pVertex_VNTT3T3[i].nx;
				pVertex_VNTT3T3[i].ny=-pVertex_VNTT3T3[i].ny;
				pVertex_VNTT3T3[i].nz=-pVertex_VNTT3T3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[0].nx;
		pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[0].ny;
		pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[m].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[m].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[m].nz;
			}
		}

		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			tempo2=EndIndexIn_pVertex_TopOfCylinder;

			pVertex_VNTT3T3[i].nx=0.0;					//They all point to positive z
			pVertex_VNTT3T3[i].ny=0.0;
			pVertex_VNTT3T3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[j].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[j].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[j].nz;
			}
		}

		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder;

			pVertex_VNTT3T3[i].nx=0.0;
			pVertex_VNTT3T3[i].ny=0.0;
			pVertex_VNTT3T3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3T3[i].nx=pVertex_VNTT3T3[j].nx;
				pVertex_VNTT3T3[i].ny=pVertex_VNTT3T3[j].ny;
				pVertex_VNTT3T3[i].nz=pVertex_VNTT3T3[j].nz;
			}
		}

		return 1;
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		//Normals
		//**** Start with the body of cylinder. Vertex - center

		//Compute for the first stack (the first circle)
		//and all the other stacks are the same.
		for(i=0; i<Slices; i++)
		{
			if(i==0)
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3[i+1].x-pVertex_VNTT3[i].x;
				vector1[1]=pVertex_VNTT3[i+1].y-pVertex_VNTT3[i].y;
				vector1[2]=pVertex_VNTT3[i+1].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3[i+SlicesPlusOne].x-pVertex_VNTT3[i].x;
				vector2[1]=pVertex_VNTT3[i+SlicesPlusOne].y-pVertex_VNTT3[i].y;
				vector2[2]=pVertex_VNTT3[i+SlicesPlusOne].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i+Slices-1].x;
				vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i+Slices-1].y;
				vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].x-pVertex_VNTT3[i+Slices-1].x;
				vector5[1]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].y-pVertex_VNTT3[i+Slices-1].y;
				vector5[2]=pVertex_VNTT3[i+Slices-1+SlicesPlusOne].z-pVertex_VNTT3[i+Slices-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

				pVertex_VNTT3[i].nx=-pVertex_VNTT3[i].nx;
				pVertex_VNTT3[i].ny=-pVertex_VNTT3[i].ny;
				pVertex_VNTT3[i].nz=-pVertex_VNTT3[i].nz;
			}
			else
			{
				//Compute normal vector 1
				vector1[0]=pVertex_VNTT3[i+1].x-pVertex_VNTT3[i].x;
				vector1[1]=pVertex_VNTT3[i+1].y-pVertex_VNTT3[i].y;
				vector1[2]=pVertex_VNTT3[i+1].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector1);

				vector2[0]=pVertex_VNTT3[i+SlicesPlusOne].x-pVertex_VNTT3[i].x;
				vector2[1]=pVertex_VNTT3[i+SlicesPlusOne].y-pVertex_VNTT3[i].y;
				vector2[2]=pVertex_VNTT3[i+SlicesPlusOne].z-pVertex_VNTT3[i].z;
				NormalizeVectorFLOAT_2(vector2);

				ComputeNormalOfPlaneFLOAT_2(vector3, vector1, vector2);

				//Compute normal vector 2
				vector4[0]=pVertex_VNTT3[i].x-pVertex_VNTT3[i-1].x;
				vector4[1]=pVertex_VNTT3[i].y-pVertex_VNTT3[i-1].y;
				vector4[2]=pVertex_VNTT3[i].z-pVertex_VNTT3[i-1].z;
				NormalizeVectorFLOAT_2(vector4);

				vector5[0]=pVertex_VNTT3[i-1+SlicesPlusOne].x-pVertex_VNTT3[i-1].x;
				vector5[1]=pVertex_VNTT3[i-1+SlicesPlusOne].y-pVertex_VNTT3[i-1].y;
				vector5[2]=pVertex_VNTT3[i-1+SlicesPlusOne].z-pVertex_VNTT3[i-1].z;
				NormalizeVectorFLOAT_2(vector5);

				ComputeNormalOfPlaneFLOAT_2(vector6, vector4, vector5);

				//Now average normal from face 1 and 2
				pVertex_VNTT3[i].nx=vector3[0]+vector6[0];
				pVertex_VNTT3[i].ny=vector3[1]+vector6[1];
				pVertex_VNTT3[i].nz=vector3[2]+vector6[2];
				NormalizeVectorFLOAT_2(&pVertex_VNTT3[i].nx);

				pVertex_VNTT3[i].nx=-pVertex_VNTT3[i].nx;
				pVertex_VNTT3[i].ny=-pVertex_VNTT3[i].ny;
				pVertex_VNTT3[i].nz=-pVertex_VNTT3[i].nz;
			}
		}

		//The last vertex on the stack is the same as the first
		pVertex_VNTT3[i].nx=pVertex_VNTT3[0].nx;
		pVertex_VNTT3[i].ny=pVertex_VNTT3[0].ny;
		pVertex_VNTT3[i].nz=pVertex_VNTT3[0].nz;
		i++;

		for(k=1; k<=Stacks; k++)
		{
			for(j=m=0; j<SlicesPlusOne; j++, i++, m++)
			{
				pVertex_VNTT3[i].nx=pVertex_VNTT3[m].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[m].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[m].nz;
			}
		}

		//**** For top of cylinder
		if(IsThereATop)
		{
			i=StartIndexIn_pVertex_TopOfCylinder;
			tempo2=EndIndexIn_pVertex_TopOfCylinder;

			pVertex_VNTT3[i].nx=0.0;					//They all point to positive z
			pVertex_VNTT3[i].ny=0.0;
			pVertex_VNTT3[i].nz=1.0;
			
			j=i;
			for(i++; i<tempo2; i++)
			{	//Same normal for all the vertices
				pVertex_VNTT3[i].nx=pVertex_VNTT3[j].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[j].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[j].nz;
			}
		}

		//**** For bottom of cylinder
		if(IsThereABottom)
		{
			i=StartIndexIn_pVertex_BottomOfCylinder;
			tempo2=EndIndexIn_pVertex_BottomOfCylinder;

			pVertex_VNTT3[i].nx=0.0;
			pVertex_VNTT3[i].ny=0.0;
			pVertex_VNTT3[i].nz=-1.0;					//They all point to negative z
			
			j=i;
			for(i++; i<tempo2; i++)
			{
				pVertex_VNTT3[i].nx=pVertex_VNTT3[j].nx;
				pVertex_VNTT3[i].ny=pVertex_VNTT3[j].ny;
				pVertex_VNTT3[i].nz=pVertex_VNTT3[j].nz;
			}
		}

		return 1;
	}

	return -1;
}

//PURPOSE:	Compute texture coordinates for every vertex. Will only generate the default case
//			which is the texture wraps around the cylinder's body.
//			For the top and bottom, the center vertex gets {0.5, 0.5} texcoords and the
//			other vertices texcoords are generated by a circle.
//
//PARAMETERS:
//			[in] widthTextureLimit : set this to 1.0 for now
//			[in] heightTextureLimit : set this to 1.0 for now
//
//RETURNS:
//			1 for success
//			-1 if TexCoordStyle is not 1 or if the VertexFormat is not supported
sint TCylinder::GenerateTexCoords2(sreal widthTextureLimit, sreal heightTextureLimit)
{
	sint i, tempo2, tempo3;
	sreal FTempo, FTempo2, angle;

	if(VertexFormat==GLH_VERTEXFORMAT_VT)
	{
		if(TexCoordStyle==1)
		{
			if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
			{
				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pVertex_VT[0].s0=0.0;
				pVertex_VT[0].t0=0.0;
				tempo2=Slices;
				for(i=1; i<tempo2; i++)
				{
					pVertex_VT[i].s0=(pVertex_VT[i-1].s0+FTempo)*ScaleFactorS[0];
					pVertex_VT[i].t0=0.0;
				}
				pVertex_VT[i].s0=1.0*ScaleFactorS[0];
				pVertex_VT[i].t0=0.0;

				tempo2=EndIndexIn_pVertex_BodyOfCylinder;
				tempo3=SlicesPlusOne;
				for(i++; i<tempo2; i++)
				{
					pVertex_VT[i].s0=pVertex_VT[i-tempo3].s0;
					pVertex_VT[i].t0=(pVertex_VT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				}

				if(IsThereATop)
				{
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_TopOfCylinder;
					pVertex_VT[i].s0=0.5;		//Center of texture
					pVertex_VT[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VT[i].s0=pVertex_VT[StartIndexIn_pVertex_TopOfCylinder+1].s0;
					pVertex_VT[i].t0=pVertex_VT[StartIndexIn_pVertex_TopOfCylinder+1].t0;
				}

				if(IsThereABottom)
				{
					//Read the part with ****
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_BottomOfCylinder;
					pVertex_VT[i].s0=0.5;		//Center of texture
					pVertex_VT[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VT[i].s0=pVertex_VT[StartIndexIn_pVertex_BottomOfCylinder+1].s0;
					pVertex_VT[i].t0=pVertex_VT[StartIndexIn_pVertex_BottomOfCylinder+1].t0;
				}

				return 1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT)
	{
		if(TexCoordStyle==1)
		{
			if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
			{
				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pVertex_VNT[0].s0=0.0;
				pVertex_VNT[0].t0=0.0;
				tempo2=Slices;
				for(i=1; i<tempo2; i++)
				{
					pVertex_VNT[i].s0=(pVertex_VNT[i-1].s0+FTempo)*ScaleFactorS[0];
					pVertex_VNT[i].t0=0.0;
				}
				pVertex_VNT[i].s0=1.0*ScaleFactorS[0];
				pVertex_VNT[i].t0=0.0;

				tempo2=EndIndexIn_pVertex_BodyOfCylinder;
				tempo3=SlicesPlusOne;
				for(i++; i<tempo2; i++)
				{
					pVertex_VNT[i].s0=pVertex_VNT[i-tempo3].s0;
					pVertex_VNT[i].t0=(pVertex_VNT[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				}

				if(IsThereATop)
				{
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_TopOfCylinder;
					pVertex_VNT[i].s0=0.5;		//Center of texture
					pVertex_VNT[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNT[i].s0=pVertex_VNT[StartIndexIn_pVertex_TopOfCylinder+1].s0;
					pVertex_VNT[i].t0=pVertex_VNT[StartIndexIn_pVertex_TopOfCylinder+1].t0;
				}

				if(IsThereABottom)
				{
					//Read the part with ****
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_BottomOfCylinder;
					pVertex_VNT[i].s0=0.5;		//Center of texture
					pVertex_VNT[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNT[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNT[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNT[i].s0=pVertex_VNT[StartIndexIn_pVertex_BottomOfCylinder+1].s0;
					pVertex_VNT[i].t0=pVertex_VNT[StartIndexIn_pVertex_BottomOfCylinder+1].t0;
				}

				return 1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNT3)
	{
		if(TexCoordStyle==1)
		{
			if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
			{
				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pVertex_VNT3[0].s0=0.0;
				pVertex_VNT3[0].t0=0.0;
				pVertex_VNT3[0].r0=0.0;				//Just set to 0.0
				tempo2=Slices;
				for(i=1; i<tempo2; i++)
				{
					pVertex_VNT3[i].s0=(pVertex_VNT3[i-1].s0+FTempo)*ScaleFactorS[0];
					pVertex_VNT3[i].t0=0.0;
					pVertex_VNT3[i].r0=0.0;			//Just set to 0.0
				}
				pVertex_VNT3[i].s0=1.0*ScaleFactorS[0];
				pVertex_VNT3[i].t0=0.0;
				pVertex_VNT3[i].r0=0.0;				//Just set to 0.0

				tempo2=EndIndexIn_pVertex_BodyOfCylinder;
				tempo3=SlicesPlusOne;
				for(i++; i<tempo2; i++)
				{
					pVertex_VNT3[i].s0=pVertex_VNT3[i-tempo3].s0;
					pVertex_VNT3[i].t0=(pVertex_VNT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
					pVertex_VNT3[i].r0=0.0;			//Just set to 0.0
				}

				if(IsThereATop)
				{
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_TopOfCylinder;
					pVertex_VNT3[i].s0=0.5;		//Center of texture
					pVertex_VNT3[i].t0=0.5;		//Center of texture
					pVertex_VNT3[i].r0=0.0;		//Just set to 0.0
					tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
						pVertex_VNT3[i].r0=0.0;	//Just set to 0.0
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNT3[i].s0=pVertex_VNT3[StartIndexIn_pVertex_TopOfCylinder+1].s0;
					pVertex_VNT3[i].t0=pVertex_VNT3[StartIndexIn_pVertex_TopOfCylinder+1].t0;
					pVertex_VNT3[i].r0=0.0;		//Just set to 0.0
				}

				if(IsThereABottom)
				{
					//Read the part with ****
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_BottomOfCylinder;
					pVertex_VNT3[i].s0=0.5;		//Center of texture
					pVertex_VNT3[i].t0=0.5;		//Center of texture
					pVertex_VNT3[i].r0=0.0;		//Just set to 0.0
					tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
						pVertex_VNT3[i].r0=0.0;	//Just set to 0.0
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNT3[i].s0=pVertex_VNT3[StartIndexIn_pVertex_BottomOfCylinder+1].s0;
					pVertex_VNT3[i].t0=pVertex_VNT3[StartIndexIn_pVertex_BottomOfCylinder+1].t0;
					pVertex_VNT3[i].r0=0.0;		//Just set to 0.0
				}

				return 1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3T3)
	{
		if(TexCoordStyle==1)
		{
			if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
			{
				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pVertex_VNTT3T3[0].s0=0.0;
				pVertex_VNTT3T3[0].t0=0.0;
				tempo2=Slices;
				for(i=1; i<tempo2; i++)
				{
					pVertex_VNTT3T3[i].s0=(pVertex_VNTT3T3[i-1].s0+FTempo)*ScaleFactorS[0];
					pVertex_VNTT3T3[i].t0=0.0;
				}
				pVertex_VNTT3T3[i].s0=1.0*ScaleFactorS[0];
				pVertex_VNTT3T3[i].t0=0.0;

				tempo2=EndIndexIn_pVertex_BodyOfCylinder;
				tempo3=SlicesPlusOne;
				for(i++; i<tempo2; i++)
				{
					pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[i-tempo3].s0;
					pVertex_VNTT3T3[i].t0=(pVertex_VNTT3T3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				}

				if(IsThereATop)
				{
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_TopOfCylinder;
					pVertex_VNTT3T3[i].s0=0.5;		//Center of texture
					pVertex_VNTT3T3[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[StartIndexIn_pVertex_TopOfCylinder+1].s0;
					pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[StartIndexIn_pVertex_TopOfCylinder+1].t0;
				}

				if(IsThereABottom)
				{
					//Read the part with ****
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_BottomOfCylinder;
					pVertex_VNTT3T3[i].s0=0.5;		//Center of texture
					pVertex_VNTT3T3[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNTT3T3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNTT3T3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNTT3T3[i].s0=pVertex_VNTT3T3[StartIndexIn_pVertex_BottomOfCylinder+1].s0;
					pVertex_VNTT3T3[i].t0=pVertex_VNTT3T3[StartIndexIn_pVertex_BottomOfCylinder+1].t0;
				}

				return 1;
			}
		}
	}
	else if(VertexFormat==GLH_VERTEXFORMAT_VNTT3)
	{
		if(TexCoordStyle==1)
		{
			if(((widthTextureLimit<=0.0)||(widthTextureLimit>=1.0))&&((heightTextureLimit<=0.0)||(heightTextureLimit>=1.0)))
			{
				FTempo=1.0/sreal(Slices);			//Used as x Tex coord increment
				FTempo2=1.0/sreal(Stacks);			//Used as y Tex coord increment

				//Generate for first stack
				pVertex_VNTT3[0].s0=0.0;
				pVertex_VNTT3[0].t0=0.0;
				tempo2=Slices;
				for(i=1; i<tempo2; i++)
				{
					pVertex_VNTT3[i].s0=(pVertex_VNTT3[i-1].s0+FTempo)*ScaleFactorS[0];
					pVertex_VNTT3[i].t0=0.0;
				}
				pVertex_VNTT3[i].s0=1.0*ScaleFactorS[0];
				pVertex_VNTT3[i].t0=0.0;

				tempo2=EndIndexIn_pVertex_BodyOfCylinder;
				tempo3=SlicesPlusOne;
				for(i++; i<tempo2; i++)
				{
					pVertex_VNTT3[i].s0=pVertex_VNTT3[i-tempo3].s0;
					pVertex_VNTT3[i].t0=(pVertex_VNTT3[i-tempo3].t0+FTempo2)*ScaleFactorT[0];
				}

				if(IsThereATop)
				{
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_TopOfCylinder;
					pVertex_VNTT3[i].s0=0.5;		//Center of texture
					pVertex_VNTT3[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_TopOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNTT3[i].s0=pVertex_VNTT3[StartIndexIn_pVertex_TopOfCylinder+1].s0;
					pVertex_VNTT3[i].t0=pVertex_VNTT3[StartIndexIn_pVertex_TopOfCylinder+1].t0;
				}

				if(IsThereABottom)
				{
					//Read the part with ****
					angle=PI_DEF/4.0;		//Start at 45 degrees
					FTempo=PI_FULL_DEF/sreal(Slices);

					i=StartIndexIn_pVertex_BottomOfCylinder;
					pVertex_VNTT3[i].s0=0.5;		//Center of texture
					pVertex_VNTT3[i].t0=0.5;		//Center of texture
					tempo3=EndIndexIn_pVertex_BottomOfCylinder-1;
					for(i++; i<tempo3; i++, angle-=FTempo)
					{
						pVertex_VNTT3[i].s0=0.5*cosf(angle)*ScaleFactorS[0]+0.5;
						pVertex_VNTT3[i].t0=0.5*sinf(angle)*ScaleFactorT[0]+0.5;
					}

					//Fix last one cause it's the same point as the first
					pVertex_VNTT3[i].s0=pVertex_VNTT3[StartIndexIn_pVertex_BottomOfCylinder+1].s0;
					pVertex_VNTT3[i].t0=pVertex_VNTT3[StartIndexIn_pVertex_BottomOfCylinder+1].t0;
				}

				return 1;
			}
		}
	}

	return -1;
}


//PURPOSE:
//
//PARAMETERS:
//
//RETURN:
//




