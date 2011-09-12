// glhlib3.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include <stack>
#include "3DGraphicsLibrarySmall.h"
#include "GlobalsExternDeclarations.h"


#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'


using namespace std;



//Not defined in gl.h or glext.h?
#ifndef GL_ABGR
#define GL_ABGR		GL_ABGR_EXT
#endif


//INTERNAL VARIABLES, CLASSES, STRUCTS
//sint	ErrorState;			//Record error if one occurs. I will no longer use this. I will instead use a stack with max depth of 100
sint			TotalErrorStateStack;
stack<sint>	ErrorStateStack;






//PURPOSE: This is an internal function used by
//         glhBuild2DNormalMipmaps
//         glhBuildCubeMapNormalMipmaps
//
//         We have to pay attention to the location of the alpha component since it
//         holds the length of the normal.
void ScaleNormalMap(uint format, sint widthin, sint heightin, uchar *datain, sint widthout, sint heightout, uchar *dataout)
{
	const sreal oneOver127=1.0/127.0;
	const sreal oneOver255=1.0/255.0;

	sreal x, y, z, l, invl;
	sreal mag00, mag01, mag10, mag11;
	sint i, j, ii, jj;

	if((format==GL_RGBA)||(format==GL_BGRA))
	{
		for(i=0; i<heightin; i+=2)
		{
			for(j=0; j<widthin; j+=2)
			{
				//The %w2 and %h2 modulo arithmetic makes sure that Nx1 and 1xN normal map
				//levels are handled correctly.
				
				//Fetch the magnitude of the 4 vectors to be down-sampled.
				mag00=oneOver255*(sreal)datain[(i*widthin+j)*4+3];
				mag01=oneOver255*(sreal)datain[(i*widthin+(j+1)%heightin)*4+3];
				mag10=oneOver255*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+3];
				mag11=oneOver255*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+3];

				//Sum 2x2 footprint of red component scaled back to [-1, 1] range
				x=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4]-1.0);
				x+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4]-1.0);
				x+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4]-1.0);
				x+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4]-1.0);

				//Sum 2x2 footprint of green component scaled back to [-1, 1] range
				y=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4+1]-1.0);
				y+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4+1]-1.0);
				y+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+1]-1.0);
				y+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+1]-1.0);

				//Sum 2x2 footprint of blue component scaled back to [-1, 1] range
				z=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4+2]-1.0);
				z+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4+2]-1.0);
				z+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+2]-1.0);
				z+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+2]-1.0);

				//Compute length of the (x, y, z) vector
				l=sqrtf(x*x+y*y+z*z);
				if((l>=-1.0e-6)&&(l<=1.0e-6))
				{
					//NULL vector, avoid division by zero and just use the unperturbed normal
					x=0.0;
					y=0.0;
					z=1.0;
				}
				else
				{
					//Normalize the vector to unit length
					invl=1.0/l;
					x*=invl;
					y*=invl;
					z*=invl;
				}
				ii=i>>1;		//Divide by 2
				jj=j>>1;		//Divide by 2
				dataout[(ii*widthout+jj)*4  ]=(uchar)(128.0+127.0*x);
				dataout[(ii*widthout+jj)*4+1]=(uchar)(128.0+127.0*y);
				dataout[(ii*widthout+jj)*4+2]=(uchar)(128.0+127.0*z);

				//Store the magnitude of the average
				l=l/4.0;
				if(l>1.0)
				{
					dataout[(ii*widthout+jj)*4+3]=255;
				}
				else
				{
					dataout[(ii*widthout+jj)*4+3]=(uchar)(255.0*l);
				}
			}
		}
	}
	else if(format==GL_ABGR)
	{
		for(i=0; i<heightin; i+=2)
		{
			for(j=0; j<widthin; j+=2)
			{
				//The %w2 and %h2 modulo arithmetic makes sure that Nx1 and 1xN normal map
				//levels are handled correctly.
				
				//Fetch the magnitude of the 4 vectors to be down-sampled.
				mag00=oneOver255*(sreal)datain[(i*widthin+j)*4];
				mag01=oneOver255*(sreal)datain[(i*widthin+(j+1)%heightin)*4];
				mag10=oneOver255*(sreal)datain[(((i+1)%widthin)*widthin+j)*4];
				mag11=oneOver255*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4];

				//Sum 2x2 footprint of red component scaled back to [-1, 1] range
				x=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4+3]-1.0);
				x+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4+3]-1.0);
				x+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+3]-1.0);
				x+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+3]-1.0);

				//Sum 2x2 footprint of green component scaled back to [-1, 1] range
				y=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4+2]-1.0);
				y+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4+2]-1.0);
				y+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+2]-1.0);
				y+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+2]-1.0);

				//Sum 2x2 footprint of blue component scaled back to [-1, 1] range
				z=mag00*(oneOver127*(sreal)datain[(i*widthin+j)*4+1]-1.0);
				z+=mag01*(oneOver127*(sreal)datain[(i*widthin+(j+1)%heightin)*4+1]-1.0);
				z+=mag10*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+j)*4+1]-1.0);
				z+=mag11*(oneOver127*(sreal)datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+1]-1.0);

				//Compute length of the (x, y, z) vector
				l=sqrtf(x*x+y*y+z*z);
				if((l>=-1.0e-6)&&(l<=1.0e-6))
				{
					//NULL vector, avoid division by zero and just use the unperturbed normal
					x=0.0;
					y=0.0;
					z=1.0;
				}
				else
				{
					//Normalize the vector to unit length
					invl=1.0/l;
					x*=invl;
					y*=invl;
					z*=invl;
				}
				ii=i>>1;		//Divide by 2
				jj=j>>1;		//Divide by 2
				dataout[(ii*widthout+jj)*4+3]=(uchar)(128.0+127.0*x);
				dataout[(ii*widthout+jj)*4+2]=(uchar)(128.0+127.0*y);
				dataout[(ii*widthout+jj)*4+1]=(uchar)(128.0+127.0*z);

				//Store the magnitude of the average
				l=l/4.0;
				if(l>1.0)
				{
					dataout[(ii*widthout+jj)*4]=255;
				}
				else
				{
					dataout[(ii*widthout+jj)*4]=(uchar)(255.0*l);
				}
			}
		}
	}
}

void ScaleNormalMap_FLOAT(uint format, sint widthin, sint heightin, GLfloat *datain, sint widthout, sint heightout, GLfloat *dataout)
{
	sreal x, y, z, l, invl;
	sreal mag00, mag01, mag10, mag11;
	sint i, j, ii, jj;

	if((format==GL_RGBA)||(format==GL_BGRA))
	{
		for(i=0; i<heightin; i+=2)
		{
			for(j=0; j<widthin; j+=2)
			{
				//The %w2 and %h2 modulo arithmetic makes sure that Nx1 and 1xN normal map
				//levels are handled correctly.
				
				//Fetch the magnitude of the 4 vectors to be down-sampled.
				mag00=datain[(i*widthin+j)*4+3];
				mag01=datain[(i*widthin+(j+1)%heightin)*4+3];
				mag10=datain[(((i+1)%widthin)*widthin+j)*4+3];
				mag11=datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+3];

				//Sum 2x2 footprint of red component scaled back to [-1, 1] range
				x=mag00*(2.0*datain[(i*widthin+j)*4]-1.0);
				x+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4]-1.0);
				x+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4]-1.0);
				x+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4]-1.0);

				//Sum 2x2 footprint of green component scaled back to [-1, 1] range
				y=mag00*(2.0*datain[(i*widthin+j)*4+1]-1.0);
				y+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4+1]-1.0);
				y+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4+1]-1.0);
				y+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+1]-1.0);

				//Sum 2x2 footprint of blue component scaled back to [-1, 1] range
				z=mag00*(2.0*datain[(i*widthin+j)*4+2]-1.0);
				z+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4+2]-1.0);
				z+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4+2]-1.0);
				z+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+2]-1.0);

				//Compute length of the (x, y, z) vector
				l=sqrtf(x*x+y*y+z*z);
				if((l>=-1.0e-6)&&(l<=1.0e-6))
				{
					//NULL vector, avoid division by zero and just use the unperturbed normal
					x=0.0;
					y=0.0;
					z=1.0;
				}
				else
				{
					//Normalize the vector to unit length
					invl=1.0/l;
					x*=invl;
					y*=invl;
					z*=invl;
				}
				ii=i>>1;		//Divide by 2
				jj=j>>1;		//Divide by 2
				dataout[(ii*widthout+jj)*4  ]=0.5+0.5*x;
				dataout[(ii*widthout+jj)*4+1]=0.5+0.5*y;
				dataout[(ii*widthout+jj)*4+2]=0.5+0.5*z;

				//Store the magnitude of the average
				l=l/4.0;
				if(l>1.0)
				{
					dataout[(ii*widthout+jj)*4+3]=1.0;
				}
				else
				{
					dataout[(ii*widthout+jj)*4+3]=l;
				}
			}
		}
	}
	else if(format==GL_ABGR)
	{
		for(i=0; i<heightin; i+=2)
		{
			for(j=0; j<widthin; j+=2)
			{
				//The %w2 and %h2 modulo arithmetic makes sure that Nx1 and 1xN normal map
				//levels are handled correctly.
				
				//Fetch the magnitude of the 4 vectors to be down-sampled.
				mag00=datain[(i*widthin+j)*4];
				mag01=datain[(i*widthin+(j+1)%heightin)*4];
				mag10=datain[(((i+1)%widthin)*widthin+j)*4];
				mag11=datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4];

				//Sum 2x2 footprint of red component scaled back to [-1, 1] range
				x=mag00*(2.0*datain[(i*widthin+j)*4+3]-1.0);
				x+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4+3]-1.0);
				x+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4+3]-1.0);
				x+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+3]-1.0);

				//Sum 2x2 footprint of green component scaled back to [-1, 1] range
				y=mag00*(2.0*datain[(i*widthin+j)*4+2]-1.0);
				y+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4+2]-1.0);
				y+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4+2]-1.0);
				y+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+2]-1.0);

				//Sum 2x2 footprint of blue component scaled back to [-1, 1] range
				z=mag00*(2.0*datain[(i*widthin+j)*4+1]-1.0);
				z+=mag01*(2.0*datain[(i*widthin+(j+1)%heightin)*4+1]-1.0);
				z+=mag10*(2.0*datain[(((i+1)%widthin)*widthin+j)*4+1]-1.0);
				z+=mag11*(2.0*datain[(((i+1)%widthin)*widthin+(j+1)%heightin)*4+1]-1.0);

				//Compute length of the (x, y, z) vector
				l=sqrtf(x*x+y*y+z*z);
				if((l>=-1.0e-6)&&(l<=1.0e-6))
				{
					//NULL vector, avoid division by zero and just use the unperturbed normal
					x=0.0;
					y=0.0;
					z=1.0;
				}
				else
				{
					//Normalize the vector to unit length
					invl=1.0/l;
					x*=invl;
					y*=invl;
					z*=invl;
				}
				ii=i>>1;		//Divide by 2
				jj=j>>1;		//Divide by 2
				dataout[(ii*widthout+jj)*4+3]=0.5+0.5*x;
				dataout[(ii*widthout+jj)*4+2]=0.5+0.5*y;
				dataout[(ii*widthout+jj)*4+1]=0.5+0.5*z;

				//Store the magnitude of the average
				l=l/4.0;
				if(l>1.0)
				{
					dataout[(ii*widthout+jj)*4]=1.0;
				}
				else
				{
					dataout[(ii*widthout+jj)*4]=l;
				}
			}
		}
	}
}

sint glhBuild2DNormalMipmaps(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const uchar *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;
	
	sint WidthOpenGL, HeightOpenGL, swap;
	sint width2, height2;
	uchar *dataout, *data2;
	sint level;

	WidthOpenGL=width;
	HeightOpenGL=height;
	
	int scanLinePadded;
	if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		int BytesPerPixel=4;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL];
	}
	else
		return -1;

	if((dataout==NULL)||(data2==NULL))
	{
		if(dataout)
			delete [] dataout;
		if(data2)
			delete [] data2;
		return 0;
	}

	/////////////////////////////////
	swap=0;
	level=0;
	width2=width;
	height2=height;
	while(1)
	{
		if(swap==0)
		{
			swap=1;
			if((level==0)&&(width==WidthOpenGL)&&(height==HeightOpenGL))
			{
				//Make a copy of the original
				memcpy(dataout, data, sizeof(uchar)*scanLinePadded*height);	
			}
			else
			{
				ScaleNormalMap(format, width2, height2, data2, WidthOpenGL, HeightOpenGL, dataout);
			}
			
			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL, 0, format, GL_UNSIGNED_BYTE, dataout);
		}
		else
		{
			swap=0;
			ScaleNormalMap(format, width2, height2, dataout, WidthOpenGL, HeightOpenGL, data2);

			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL, 0, format, GL_UNSIGNED_BYTE, data2);
		}
		level++;
		width2=WidthOpenGL;
		height2=HeightOpenGL;
		if((WidthOpenGL==1)&&(HeightOpenGL==1))
			break;
		if(WidthOpenGL>=2)		//If it turns into 1, do not divide anymore
			WidthOpenGL/=2;
		if(HeightOpenGL>=2)		//If it turns into 1, do not divide anymore
			HeightOpenGL/=2;
	}

	delete [] dataout;
	delete [] data2;
	////////////////////////////
	return 1;
}

sint glhBuild2DNormalMipmapsf(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const GLfloat *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;
	
	sint WidthOpenGL, HeightOpenGL, swap;
	sint width2, height2;
	sreal *dataout, *data2;
	sint level;

	WidthOpenGL=width;
	HeightOpenGL=height;
	
	sint scanLinePadded;
	if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		int BytesPerPixel=4;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new sreal[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new sreal[scanLinePadded*HeightOpenGL];
	}
	else
		return -1;

	if((dataout==NULL)||(data2==NULL))
	{
		if(dataout)
			delete [] dataout;
		if(data2)
			delete [] data2;
		return 0;
	}

	/////////////////////////////////
	swap=0;
	level=0;
	width2=width;
	height2=height;
	while(1)
	{
		if(swap==0)
		{
			swap=1;
			if((level==0)&&(width==WidthOpenGL)&&(height==HeightOpenGL))
			{
				//Make a copy of the original
				memcpy(dataout, data, sizeof(sreal)*scanLinePadded*height);	
			}
			else
			{
				ScaleNormalMap_FLOAT(format, width2, height2, data2, WidthOpenGL, HeightOpenGL, dataout);
			}
			
			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL, 0, format, GL_FLOAT, dataout);
		}
		else
		{
			swap=0;
			ScaleNormalMap_FLOAT(format, width2, height2, dataout, WidthOpenGL, HeightOpenGL, data2);

			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL, 0, format, GL_FLOAT, data2);
		}
		level++;
		width2=WidthOpenGL;
		height2=HeightOpenGL;
		if((WidthOpenGL==1)&&(HeightOpenGL==1))
			break;
		if(WidthOpenGL>=2)		//If it turns into 1, do not divide anymore
			WidthOpenGL/=2;
		if(HeightOpenGL>=2)		//If it turns into 1, do not divide anymore
			HeightOpenGL/=2;
	}

	delete [] dataout;
	delete [] data2;
	////////////////////////////
	return 1;
}

sint glhBuildCubeMapNormalMipmaps(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const uchar *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;

	sint WidthOpenGL, HeightOpenGL, swap;
	sint WidthOpenGL2, HeightOpenGL2;
	sint width2, height2;
	uchar *dataout, *data2;
	sint level;
	
	WidthOpenGL=width;
	HeightOpenGL=height;
		
	sint scanLinePadded;
	sint BytesPerPixel;
	if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		BytesPerPixel=4;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL];
	}
	else
		return -1;

	if((dataout==NULL)||(data2==NULL))
	{
		if(dataout)
			delete [] dataout;
		if(data2)
			delete [] data2;
		return 0;
	}

	sint total=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment*height;
	/////////////////////////////////
	for(sint k=0; k<6; k++)
	{
		swap=0;
		level=0;
		WidthOpenGL2=WidthOpenGL;
		HeightOpenGL2=HeightOpenGL;
		width2=width;
		height2=height;
		while(1)
		{
			if(swap==0)
			{
				swap=1;
				if((level==0)&&(width==WidthOpenGL2)&&(height==HeightOpenGL2))
				{
					//Make a copy of the original
					memcpy(dataout, &data[k*total], sizeof(uchar)*scanLinePadded*height);	
				}
				else
				{
					ScaleNormalMap(format, width2, height2, data2, WidthOpenGL2, HeightOpenGL2, dataout);
				}
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2, 0, format, GL_UNSIGNED_BYTE, dataout);
			}
			else
			{
				swap=0;
				ScaleNormalMap(format, width2, height2, dataout, WidthOpenGL2, HeightOpenGL2, data2);
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2, 0, format, GL_UNSIGNED_BYTE, data2);
			}
			level++;
			width2=WidthOpenGL2;
			height2=HeightOpenGL2;
			if((WidthOpenGL2==1)&&(HeightOpenGL2==1))
				break;
			if(WidthOpenGL2>=2)			//If it turns into 1, do not divide anymore
				WidthOpenGL2/=2;
			if(HeightOpenGL2>=2)		//If it turns into 1, do not divide anymore
				HeightOpenGL2/=2;
		}
	}

	delete [] dataout;
	delete [] data2;
	////////////////////////////
	return 1;
}

sint glhBuildCubeMapNormalMipmapsf(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const GLfloat *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;

	int WidthOpenGL, HeightOpenGL, swap;
	int WidthOpenGL2, HeightOpenGL2;
	int width2, height2;
	sreal *dataout, *data2;
	sint level;
	
	WidthOpenGL=width;
	HeightOpenGL=height;
		
	int scanLinePadded;
	int BytesPerPixel;
	if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		BytesPerPixel=4;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new sreal[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new sreal[scanLinePadded*HeightOpenGL];
	}
	else
		return -1;

	if((dataout==NULL)||(data2==NULL))
	{
		if(dataout)
			delete [] dataout;
		if(data2)
			delete [] data2;
		return 0;
	}

	int total=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment*height;
	/////////////////////////////////
	for(int k=0; k<6; k++)
	{
		swap=0;
		level=0;
		WidthOpenGL2=WidthOpenGL;
		HeightOpenGL2=HeightOpenGL;
		width2=width;
		height2=height;
		while(1)
		{
			if(swap==0)
			{
				swap=1;
				if((level==0)&&(width==WidthOpenGL2)&&(height==HeightOpenGL2))
				{
					//Make a copy of the original
					memcpy(dataout, &data[k*total], sizeof(sreal)*scanLinePadded*height);	
				}
				else
				{
					ScaleNormalMap_FLOAT(format, width2, height2, data2, WidthOpenGL2, HeightOpenGL2, dataout);
				}
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2, 0, format, GL_FLOAT, dataout);
			}
			else
			{
				swap=0;
				ScaleNormalMap_FLOAT(format, width2, height2, dataout, WidthOpenGL2, HeightOpenGL2, data2);
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2, 0, format, GL_FLOAT, data2);
			}
			level++;
			width2=WidthOpenGL2;
			height2=HeightOpenGL2;
			if((WidthOpenGL2==1)&&(HeightOpenGL2==1))
				break;
			if(WidthOpenGL2>=2)			//If it turns into 1, do not divide anymore
				WidthOpenGL2/=2;
			if(HeightOpenGL2>=2)		//If it turns into 1, do not divide anymore
				HeightOpenGL2/=2;
		}
	}

	delete [] dataout;
	delete [] data2;
	////////////////////////////
	return 1;
}

//PURPOSE: An internal function used by glhBuildNormalizationCubeMap
void GetCubeMapVector(int i, int width, int height, int x, int y, sreal *vector)
{
	sreal s, t, sc, tc, mag;

	s=((sreal)x+0.5)/(sreal)width;
	t=((sreal)y+0.5)/(sreal)height;
	sc=s*2.0-1.0;
	tc=t*2.0-1.0;

	switch(i)
	{
	case 0:
		vector[0]=1.0;
		vector[1]=-tc;
		vector[2]=-sc;
		break;
	case 1:
		vector[0]=-1.0;
		vector[1]=-tc;
		vector[2]=sc;
		break;
	case 2:
		vector[0]=sc;
		vector[1]=1.0;
		vector[2]=tc;
		break;
	case 3:
		vector[0]=sc;
		vector[1]=-1.0;
		vector[2]=-tc;
		break;
	case 4:
		vector[0]=sc;
		vector[1]=-tc;
		vector[2]=1.0;
		break;
	case 5:
		vector[0]=-sc;
		vector[1]=-tc;
		vector[2]=-1.0;
		break;
	}

	mag=1.0/sqrtf(vector[0]*vector[0]+vector[1]*vector[1]+vector[2]*vector[2]);
	vector[0]*=mag;
	vector[1]*=mag;
	vector[2]*=mag;
}

sint glhBuildNormalizationCubeMap(sint dataAlignment, sint width, sint height, uint format, uchar *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;

	uchar *pdata;
	sreal vector[3];
	int i, x, y;
	int imagesize=width*height*4;

	if(format==GL_RGBA)
	{
		for(i=0; i<6; i++)
		{
			pdata=&data[i*imagesize];
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					pdata[4*(y*width+x)  ]=(uchar)(128.0+127.0*vector[0]);
					pdata[4*(y*width+x)+1]=(uchar)(128.0+127.0*vector[1]);
					pdata[4*(y*width+x)+2]=(uchar)(128.0+127.0*vector[2]);
					pdata[4*(y*width+x)+3]=255;
				}
			}
		}
	}
	else if(format==GL_BGRA)
	{
		for(i=0; i<6; i++)
		{
			pdata=&data[i*imagesize];
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					pdata[4*(y*width+x)+2]=(uchar)(128.0+127.0*vector[0]);
					pdata[4*(y*width+x)+1]=(uchar)(128.0+127.0*vector[1]);
					pdata[4*(y*width+x)  ]=(uchar)(128.0+127.0*vector[2]);
					pdata[4*(y*width+x)+3]=255;
				}
			}
		}
	}
	else //if(format==GL_ABGR)
	{
		for(i=0; i<6; i++)
		{
			pdata=&data[i*imagesize];
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					pdata[4*(y*width+x)+3]=(uchar)(128.0+127.0*vector[0]);
					pdata[4*(y*width+x)+2]=(uchar)(128.0+127.0*vector[1]);
					pdata[4*(y*width+x)+1]=(uchar)(128.0+127.0*vector[2]);
					pdata[4*(y*width+x)  ]=255;
				}
			}
		}
	}

	return 1;
}

sint glhBuildNormalizationCubeMapf(sint dataAlignment, sint width, sint height, uint format, GLfloat *data)
{
	if((dataAlignment!=1)&&(dataAlignment!=4))
		return -1;
	if((format!=GL_RGBA)&&(format!=GL_BGRA)&&(format!=GL_ABGR))
		return -1;
	if(width%2!=0)
		return -1;
	if(height%2!=0)
		return -1;

	sreal vector[3];
	int i, x, y;

	if(format==GL_RGBA)
	{
		for(i=0; i<6; i++)
		{
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					data[4*(y*width+x)  ]=0.5+0.5*vector[0];
					data[4*(y*width+x)+1]=0.5+0.5*vector[1];
					data[4*(y*width+x)+2]=0.5+0.5*vector[2];
					data[4*(y*width+x)+3]=1.0;
				}
			}
		}
	}
	else if(format==GL_BGRA)
	{
		for(i=0; i<6; i++)
		{
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					data[4*(y*width+x)+2]=0.5+0.5*vector[0];
					data[4*(y*width+x)+1]=0.5+0.5*vector[1];
					data[4*(y*width+x)  ]=0.5+0.5*vector[2];
					data[4*(y*width+x)+3]=1.0;
				}
			}
		}
	}
	else //if(format==GL_ABGR)
	{
		for(i=0; i<6; i++)
		{
			for(y=0; y<height; y++)
			{
				for(x=0; x<width; x++)
				{
					GetCubeMapVector(i, width, height, x, y, vector);
					data[4*(y*width+x)+3]=0.5+0.5*vector[0];
					data[4*(y*width+x)+2]=0.5+0.5*vector[1];
					data[4*(y*width+x)+1]=0.5+0.5*vector[2];
					data[4*(y*width+x)  ]=1.0;
				}
			}
		}
	}

	return 1;
}


sint glhProjectf_2(sint totalVertex4D, GLfloat *objectVertex4D,
				GLfloat *modelview_projection,
				sint *viewport,
				GLfloat *windowCoordinate4D)
{
	sint i, counter=0;

	//Transformation vectors
	sreal fTempo[4];

	for(i=0; i<totalVertex4D; i+=4)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(fTempo, modelview_projection, &objectVertex4D[i]);
		//modelview_projection transform
		fTempo[0]=modelview_projection[0]*objectVertex4D[i]+
			modelview_projection[4]*objectVertex4D[i+1]+
			modelview_projection[8]*objectVertex4D[i+2]+
			modelview_projection[12]*objectVertex4D[i+3];
		fTempo[1]=modelview_projection[1]*objectVertex4D[i]+
			modelview_projection[5]*objectVertex4D[i+1]+
			modelview_projection[9]*objectVertex4D[i+2]+
			modelview_projection[13]*objectVertex4D[i+3];
		fTempo[2]=modelview_projection[2]*objectVertex4D[i]+
			modelview_projection[6]*objectVertex4D[i+1]+
			modelview_projection[10]*objectVertex4D[i+2]+
			modelview_projection[14]*objectVertex4D[i+3];
		fTempo[3]=modelview_projection[3]*objectVertex4D[i]+
			modelview_projection[7]*objectVertex4D[i+1]+
			modelview_projection[11]*objectVertex4D[i+2]+
			modelview_projection[15]*objectVertex4D[i+3];
		//The result normalizes between -1 and 1
		if(fTempo[3]==0.0)	//The w value
		{
			counter++;
			continue;
		}
		fTempo[3]=1.0/fTempo[3];

		//Perspective division
		fTempo[0]*=fTempo[3];
		fTempo[1]*=fTempo[3];
		fTempo[2]*=fTempo[3];

		//Window coordinates
		//Map x, y to range 0-1
		windowCoordinate4D[i  ]=(fTempo[0]*0.5+0.5)*viewport[2]+viewport[0];
		windowCoordinate4D[i+1]=(fTempo[1]*0.5+0.5)*viewport[3]+viewport[1];

		//This is only correct when glDepthRange(0.0, 1.0)
		windowCoordinate4D[i+2]=(1.0+fTempo[2])*0.5;	//Between 0 and 1
	}
	return counter;
}

sint glhProjectf_3(sint totalVertex3D, GLfloat *objectVertex3D,
				GLfloat *modelview_projection,
				sint *viewport,
				GLfloat *windowCoordinate3D)
{
	sint i, counter=0;

	//Transformation vectors
	sreal fTempo[4];

	for(i=0; i<totalVertex3D; i+=3)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(fTempo, modelview_projection, &objectVertex3D[i]);
		//modelview_projection transform
		fTempo[0]=modelview_projection[0]*objectVertex3D[i]+
			modelview_projection[4]*objectVertex3D[i+1]+
			modelview_projection[8]*objectVertex3D[i+2]+
			modelview_projection[12];		//w is 1
		fTempo[1]=modelview_projection[1]*objectVertex3D[i]+
			modelview_projection[5]*objectVertex3D[i+1]+
			modelview_projection[9]*objectVertex3D[i+2]+
			modelview_projection[13];
		fTempo[2]=modelview_projection[2]*objectVertex3D[i]+
			modelview_projection[6]*objectVertex3D[i+1]+
			modelview_projection[10]*objectVertex3D[i+2]+
			modelview_projection[14];
		fTempo[3]=modelview_projection[3]*objectVertex3D[i]+
			modelview_projection[7]*objectVertex3D[i+1]+
			modelview_projection[11]*objectVertex3D[i+2]+
			modelview_projection[15];
	
		//The result normalizes between -1 and 1
		if(fTempo[3]==0.0)	//The w value
		{
			counter++;
			continue;
		}
		fTempo[3]=1.0/fTempo[3];

		//Perspective division
		fTempo[0]*=fTempo[3];
		fTempo[1]*=fTempo[3];
		fTempo[2]*=fTempo[3];

		//Window coordinates
		//Map x, y to range 0-1
		windowCoordinate3D[i  ]=(fTempo[0]*0.5+0.5)*viewport[2]+viewport[0];
		windowCoordinate3D[i+1]=(fTempo[1]*0.5+0.5)*viewport[3]+viewport[1];

		//This is only correct when glDepthRange(0.0, 1.0)
		windowCoordinate3D[i+2]=(1.0+fTempo[2])*0.5;	//Between 0 and 1
	}
	return counter;
}

sint glhUnProjectf_2(sint totalVertex4D, GLfloat *windowCoordinate4D,
				GLfloat *inverse_modelview_projection,
				sint *viewport,
				GLfloat *objectVertex4D)
{
	sint i, counter=0;
	sreal out[4], in[4];
	sreal fTempo1=2.0/(sreal)viewport[2];
	sreal fTempo2=2.0/(sreal)viewport[3];

	for(i=0; i<totalVertex4D; i+=4)
	{
		//Transformation of normalized coordinates between -1 and 1
		in[0]=(windowCoordinate4D[i  ]-(sreal)viewport[0])*fTempo1-1.0;
		in[1]=(windowCoordinate4D[i+1]-(sreal)viewport[1])*fTempo2-1.0;
		in[2]=2.0*windowCoordinate4D[i+2]-1.0;
		//in[3]=1.0;
	
		//Objects coordinates
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&objectVertex4D[i], inverse_modelview_projection, in);
		out[0]=inverse_modelview_projection[0]*in[0]+
			inverse_modelview_projection[4]*in[1]+
			inverse_modelview_projection[8]*in[2]+
			inverse_modelview_projection[12];		//w is 1
		out[1]=inverse_modelview_projection[1]*in[0]+
			inverse_modelview_projection[5]*in[1]+
			inverse_modelview_projection[9]*in[2]+
			inverse_modelview_projection[13];
		out[2]=inverse_modelview_projection[2]*in[0]+
			inverse_modelview_projection[6]*in[1]+
			inverse_modelview_projection[10]*in[2]+
			inverse_modelview_projection[14];
		out[3]=inverse_modelview_projection[3]*in[0]+
			inverse_modelview_projection[7]*in[1]+
			inverse_modelview_projection[11]*in[2]+
			inverse_modelview_projection[15];

		if(out[3]==0.0)
		{
			counter++;
			continue;
		}
		out[3]=1.0/out[3];
		objectVertex4D[i  ]=out[0]*out[3];
		objectVertex4D[i+1]=out[1]*out[3];
		objectVertex4D[i+2]=out[2]*out[3];
		objectVertex4D[i+3]=1.0;
	}
	return counter;
}

sint glhUnProjectf_3(sint totalVertex3D, GLfloat *windowCoordinate3D,
				GLfloat *inverse_modelview_projection,
				sint *viewport,
				GLfloat *objectVertex3D)
{
	sint i, counter=0;
	sreal out[4], in[4];
	sreal fTempo1=2.0/(sreal)viewport[2];
	sreal fTempo2=2.0/(sreal)viewport[3];

	for(i=0; i<totalVertex3D; i+=3)
	{
		//Transformation of normalized coordinates between -1 and 1
		in[0]=(windowCoordinate3D[i  ]-(sreal)viewport[0])*fTempo1-1.0;
		in[1]=(windowCoordinate3D[i+1]-(sreal)viewport[1])*fTempo2-1.0;
		in[2]=2.0*windowCoordinate3D[i+2]-1.0;
		//in[3]=1.0;
	
		//Objects coordinates
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&objectVertex3D[i], inverse_modelview_projection, in);
		out[0]=inverse_modelview_projection[0]*in[0]+
			inverse_modelview_projection[4]*in[1]+
			inverse_modelview_projection[8]*in[2]+
			inverse_modelview_projection[12];		//w is 1
		out[1]=inverse_modelview_projection[1]*in[0]+
			inverse_modelview_projection[5]*in[1]+
			inverse_modelview_projection[9]*in[2]+
			inverse_modelview_projection[13];
		out[2]=inverse_modelview_projection[2]*in[0]+
			inverse_modelview_projection[6]*in[1]+
			inverse_modelview_projection[10]*in[2]+
			inverse_modelview_projection[14];
		out[3]=inverse_modelview_projection[3]*in[0]+
			inverse_modelview_projection[7]*in[1]+
			inverse_modelview_projection[11]*in[2]+
			inverse_modelview_projection[15];

		if(out[3]==0.0)
		{
			counter++;
			continue;
		}
		out[3]=1.0/out[3];
		objectVertex3D[i  ]=out[0]*out[3];
		objectVertex3D[i+1]=out[1]*out[3];
		objectVertex3D[i+2]=out[2]*out[3];
	}
	return counter;
}

#if USE_INLINE_ASM
sint glhProjectf_SSE_Aligned_2(sint totalVertex4D, GLfloat *objectVertex4D,
				 GLfloat *modelview_projection,
				 sint *viewport,
				 GLfloat *windowCoordinate4D)
{
	//FOR FUTURE : look into prefetch P3 instruction, good for cache performance
	//by loading into L2 and L1 cache.
	sint i, counter=0;		//Count number of errors
	sint viewport_1=viewport[0]+viewport[2];
	sint viewport_2=viewport[1]+viewport[3];
	sreal one=1.0;
	GLH_ALIGN16 sreal fTempo2[16];
	sreal halfVal=0.5;
	sreal *fTempo;		//Must be 16 byte (128 bit) aligned
	for(i=0; i<16; i++)
	{
		if(sint(&fTempo2[i]) % 16 == 0)
		{
			fTempo=&fTempo2[i];
			break;
		}
	}
	if(i==16)
		return -1;		//Should not occur

	__asm
	{
		//Load the vertex
		mov		eax, objectVertex4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		edi, windowCoordinate4D
		mov		esi, fTempo
		//Let's cache column 3 and 4 into xmm6 and xmm7
		mov		ecx, modelview_projection
		movaps	xmm6, [ecx+32]
		movaps	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//**** First, transform vertex from object space to clip space
		//We will be needing vertex 4 times (can't do movaps xmm0, [vertex])
		mov		ecx, modelview_projection
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [eax]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[esi], xmm0
		
		//**** Second, is w == 0.0?
		cmp		dword ptr[esi+0Ch], 0
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		
		//Instead of using FPU, we'll try SSE to do 1/w
		//I benchmarked it, and their actually isn't a diff between this
		//and the part 2
		rcpps	xmm1, xmm0
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2					; Intel says with NR we can reach 22 bit of precision
											; 0.111111 22 bit of precision and takes some more cycles
		//Broadcast to all elements 1/w, and then mul
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		movaps	[esi], xmm0

		/*fld		one
		fdiv	[esi+0Ch]
		fst		[esi+0Ch]
		fmul	[esi]
		fstp	[esi]
		fld		[esi+0Ch]
		fmul	[esi+4]
		fstp	[esi+4]
		fld		[esi+0Ch]
		fmul	[esi+8]
		fstp	[esi+8]*/

		//windowCoordinate[0]=(viewport[2]*fTempo[4]+(viewport[0]+viewport[2]))*0.5;
		//windowCoordinate[1]=(viewport[3]*fTempo[5]+(viewport[1]+viewport[3]))*0.5;
		mov		ecx, viewport
		fild	dword ptr[ecx+8]
		fmul	dword ptr[esi]
		fiadd	viewport_1
		fmul	halfVal
		fstp	dword ptr[edi]			; this is the result we want for x
		fild	dword ptr[ecx+0Ch]
		fmul	dword ptr[esi+4]
		fiadd	viewport_2
		fmul	halfVal
		fstp	dword ptr[edi+4]		; this is the result we want for y
		//This is only correct when glDepthRange(0.0, 1.0)
		//windowCoordinate[2]=(1.0+fTempo[6])*0.5;	//Between 0 and 1
		fld		dword ptr[esi+8]		; load + 1.0
		fadd	one
		fmul	halfVal
		fstp	dword ptr[edi+8]		; this is the result we want for z
		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}



sint glhProjectf_SSE_Aligned_WarmCache_2(sint totalVertex4D, GLfloat *objectVertex4D,
				 GLfloat *modelview_projection,
				 sint *viewport,
				 GLfloat *windowCoordinate4D)
{
	//FOR FUTURE : look into prefetch P3 instruction, good for cache performance
	//by loading into L2 and L1 cache.
	sint i, counter=0;		//Count number of errors
	sint viewport_1=viewport[0]+viewport[2];
	sint viewport_2=viewport[1]+viewport[3];
	sreal one=1.0;
	GLH_ALIGN16 sreal fTempo2[16];
	sreal halfVal=0.5;
	sreal *fTempo;		//Must be 16 byte (128 bit) aligned
	for(i=0; i<16; i++)
	{
		if(sint(&fTempo2[i]) % 16 == 0)
		{
			fTempo=&fTempo2[i];
			break;
		}
	}
	if(i==16)
		return -1;		//Should not occur

	__asm
	{
		//Load the vertex
		mov		eax, objectVertex4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		edi, windowCoordinate4D
		mov		esi, fTempo
		//Let's cache column 3 and 4 into xmm6 and xmm7
		mov		ecx, modelview_projection
		movaps	xmm6, [ecx+32]
		movaps	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//**** First, transform vertex from object space to clip space
		//We will be needing vertex 4 times (can't do movaps xmm0, [vertex])
		mov		ecx, modelview_projection
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [eax]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[esi], xmm0

		//Prefetch next 32 bytes into L2 and L1 cache
		//cache lines on P3 and P4 are 32 bytes. I think that on Athlon, it's 64 bytes.
		//Will this cause a invalid memory access if we go beyond array boundary? Not sure...
		//prefetch1	[eax+32]		These give error message "error C2400: inline assembler syntax error in 'opcode'; found '['"
		//even prefetch0 gives errors. Only prefetchnta works
		prefetchnta	[eax+32]
		////////////////////////////////
		
		//**** Second, is w == 0.0?
		cmp		dword ptr[esi+0Ch], 0
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		
		//Instead of using FPU, we'll try SSE to do 1/w
		//I benchmarked it, and their actually isn't a diff between this
		//and the part 2
		rcpps	xmm1, xmm0
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2					; Intel says with NR we can reach 22 bit of precision
											; 0.111111 22 bit of precision and takes some more cycles
		//Broadcast to all elements 1/w, and then mul
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		movaps	[esi], xmm0

		//part 2
		/*fld		one
		fdiv	[esi+0Ch]
		fst		[esi+0Ch]
		fmul	[esi]
		fstp	[esi]
		fld		[esi+0Ch]
		fmul	[esi+4]
		fstp	[esi+4]
		fld		[esi+0Ch]
		fmul	[esi+8]
		fstp	[esi+8]*/

		//windowCoordinate[0]=(viewport[2]*fTempo[4]+(viewport[0]+viewport[2]))*0.5;
		//windowCoordinate[1]=(viewport[3]*fTempo[5]+(viewport[1]+viewport[3]))*0.5;
		mov		ecx, viewport
		fild	dword ptr[ecx+8]
		fmul	dword ptr[esi]
		fiadd	viewport_1
		fmul	halfVal
		fstp	dword ptr[edi]			; this is the result we want for x
		fild	dword ptr[ecx+0Ch]
		fmul	dword ptr[esi+4]
		fiadd	viewport_2
		fmul	halfVal
		fstp	dword ptr[edi+4]		; this is the result we want for y
		//This is only correct when glDepthRange(0.0, 1.0)
		//windowCoordinate[2]=(1.0+fTempo[6])*0.5;	//Between 0 and 1
		fld		dword ptr[esi+8]		; load + 1.0
		fadd	one
		fmul	halfVal
		fstp	dword ptr[edi+8]		; this is the result we want for z
		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}

sint glhProjectf_SSE_Unaligned_2(sint totalVertex4D, GLfloat *objectVertex4D,
				 GLfloat *modelview_projection,
				 sint *viewport,
				 GLfloat *windowCoordinate4D)
{
	//FOR FUTURE : look into prefetch P3 instruction, good for cache performance
	//by loading into L2 and L1 cache.
	sint counter=0;		//Count number of errors
	sint viewport_1=viewport[0]+viewport[2];
	sint viewport_2=viewport[1]+viewport[3];
	sreal one=1.0;
	GLH_ALIGN16 sreal fTempo[4];
	sreal halfVal=0.5;
	__asm
	{
		//Load the vertex
		mov		eax, objectVertex4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		edi, windowCoordinate4D
		mov		ecx, modelview_projection
		movups	xmm6, [ecx+32]
		movups	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//**** First, transform vertex from object space to clip space
		//We will be needing vertex 4 times (can't do movaps xmm0, [vertex])
		mov		ecx, modelview_projection
		movups	xmm0, [ecx]						; aligned move, get first column
		movups	xmm5, [eax]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movups	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movups	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movups	[fTempo], xmm0
		
		//**** Second, is w == 0.0?
		cmp		dword ptr[fTempo+0Ch], 0
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		//Instead of using FPU, we'll try SSE to do 1/w
		rcpps	xmm1, xmm0
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2					; Intel says with NR we can reach 22 bit of precision
											; 0.111111 22 bit of precision and takes some more cycles
		//Broadcast to all elements 1/w, and then mul
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		movups	[fTempo], xmm0
		//windowCoordinate[0]=(viewport[2]*fTempo[4]+(viewport[0]+viewport[2]))*0.5;
		//windowCoordinate[1]=(viewport[3]*fTempo[5]+(viewport[1]+viewport[3]))*0.5;
		mov		ecx, viewport
		fild	dword ptr[ecx+8]
		fmul	dword ptr[fTempo]
		fiadd	viewport_1
		fmul	halfVal
		fstp	dword ptr[edi]			; this is the result we want for x
		fild	dword ptr[ecx+0Ch]
		fmul	dword ptr[fTempo+4]
		fiadd	viewport_2
		fmul	halfVal
		fstp	dword ptr[edi+4]		; this is the result we want for y

		//windowCoordinate[0]=(fTempo[0]*0.5+0.5)*viewport[2]+viewport[0];
		//windowCoordinate[1]=(fTempo[1]*0.5+0.5)*viewport[3]+viewport[1];

		//This is only correct when glDepthRange(0.0, 1.0)
		//windowCoordinate[2]=(1.0+fTempo[6])*0.5;	//Between 0 and 1
		fld		dword ptr[fTempo+8]		; load + 1.0
		fadd	one
		fmul	halfVal
		fstp	dword ptr[edi+8]		; this is the result we want for z
		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}

sint glhUnProjectf_SSE_Aligned_2(sint totalVertex4D, GLfloat *windowCoordinate4D,
	     GLfloat *inverse_modelview_projection,
	     sint *viewport,
	     GLfloat *objectVertex4D)
{
	sint counter=0;		//Count number of errors
	sreal fTempo1=2.0/(sreal)viewport[2];
	sreal fTempo2=2.0/(sreal)viewport[3];
	sreal NegOne=-1.0;
	GLH_ALIGN16 sreal out2[16], in2[16];	//in and out must be 16 byte (128 bit) aligned

	in2[3]=1.0;					//It will always be 1.0

	__asm
	{
		mov		eax, windowCoordinate4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		edi, objectVertex4D
		mov		ecx, inverse_modelview_projection
		movaps	xmm6, [ecx+32]
		movaps	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//Transformation of normalized coordinates between -1 and 1
		//in[0]=(windowCoordinate4D[i  ]-(sreal)viewport[0])/(sreal)viewport[2]*2.0-1.0;
		//in[1]=(windowCoordinate4D[i+1]-(sreal)viewport[1])/(sreal)viewport[3]*2.0-1.0;
		//in[2]=2.0*windowCoordinate4D[i+2]-1.0;
		//in[3]=1.0;
		mov		ecx, viewport
		fld		dword ptr[eax]
		fisub	dword ptr[ecx]
		fmul	fTempo1
		lea		esi, in2
		fadd	NegOne
		fstp	dword ptr[esi]
		fld		dword ptr[eax+4]
		fisub	dword ptr[ecx+4]
		fmul	fTempo2
		fadd	NegOne
		fstp	dword ptr[esi+4]
		fld		dword ptr[eax+8]
		fadd	dword ptr[eax+8]
		fadd	NegOne
		fstp	dword ptr[esi+8]
	
		//Time to do a matrix multiplication with SSE.
		//This will get us from clip space to object space.
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&objectVertex4D[i], inverse_modelview_projection, in);
		mov		ecx, inverse_modelview_projection
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [esi]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		lea		ecx, out2
		addps	xmm0, xmm3
		movaps	[ecx], xmm0

		cmp		dword ptr[ecx+12], 0			; w is 0?
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		//out[3]=1.0/out[3];
		//objectVertex4D[i  ]=out[0]*out[3];
		//objectVertex4D[i+1]=out[1]*out[3];
		//objectVertex4D[i+2]=out[2]*out[3];
		rcpps	xmm1, xmm0				; ~1/fval
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2		; Intel says with NR we can reach 22 bit of precision
								; 0.111111 22 bit of precision and takes some more cycles
		//Do a broadcast operation
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		//movaps	[edi], xmm0			; use the next line to avoid cache pollution
		movntps	[edi], xmm0

		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}

sint glhUnProjectf_SSE_Aligned_WarmCache_2(sint totalVertex4D, GLfloat *windowCoordinate4D,
				GLfloat *inverse_modelview_projection,
				sint *viewport,
				GLfloat *objectVertex4D)
{
	sint i, counter=0;		//Count number of errors
	sreal fTempo1=2.0/(sreal)viewport[2];
	sreal fTempo2=2.0/(sreal)viewport[3];
	sreal NegOne=-1.0;
	GLH_ALIGN16 sreal out2[16], in2[16];	//in and out must be 16 byte (128 bit) aligned
	sreal *out1, *in1;
	for(i=0; i<16; i++)
	{
		if(sint(&out2[i]) % 16 == 0)
		{
			out1=&out2[i];
			break;
		}
	}
	if(i==16)
		return -1;		//Should not occur
	for(i=0; i<16; i++)
	{
		if(sint(&in2[i]) % 16 == 0)
		{
			in1=&in2[i];
			break;
		}
	}
	if(i==16)
		return -1;		//Should not occur
	in1[3]=1.0;					//It will always be 1.0

	__asm
	{
		mov		eax, windowCoordinate4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		ecx, inverse_modelview_projection
		mov		edi, objectVertex4D
		movaps	xmm6, [ecx+32]
		movaps	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//Transformation of normalized coordinates between -1 and 1
		//in[0]=(windowCoordinate4D[i  ]-(sreal)viewport[0])/(sreal)viewport[2]*2.0-1.0;
		//in[1]=(windowCoordinate4D[i+1]-(sreal)viewport[1])/(sreal)viewport[3]*2.0-1.0;
		//in[2]=2.0*windowCoordinate4D[i+2]-1.0;
		//in[3]=1.0;
		mov		ecx, viewport
		fld		dword ptr[eax]
		fisub	dword ptr[ecx]
		fmul	fTempo1
		lea		esi, in1
		fadd	NegOne
		fstp	dword ptr[esi]
		fld		dword ptr[eax+4]
		fisub	dword ptr[ecx+4]
		fmul	fTempo2
		fadd	NegOne
		fstp	dword ptr[esi+4]
		fld		dword ptr[eax+8]
		fadd	dword ptr[eax+8]
		fadd	NegOne
		fstp	dword ptr[esi+8]
	
		//Time to do a matrix multiplication with SSE.
		//This will get us from clip space to object space.
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&objectVertex4D[i], inverse_modelview_projection, in);
		mov		ecx, inverse_modelview_projection
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [esi]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		lea		ecx, out1
		addps	xmm0, xmm3
		movaps	[ecx], xmm0

		//Prefetch next 32 bytes into L2 and L1 cache
		//cache lines on P3 and P4 are 32 bytes. I think that on Athlon, it's 64 bytes.
		//Will this cause a invalid memory access if we go beyond array boundary? Not sure...
		//prefetch1	[eax+32]		These give error message "error C2400: inline assembler syntax error in 'opcode'; found '['"
		//even prefetch0 gives errors. Only prefetchnta works
		prefetchnta	[eax+32]
		////////////////////////////////

		//////////HERE
		cmp		dword ptr[ecx+12], 0			; w is 0?
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		//out[3]=1.0/out[3];
		//objectVertex4D[i  ]=out[0]*out[3];
		//objectVertex4D[i+1]=out[1]*out[3];
		//objectVertex4D[i+2]=out[2]*out[3];
		rcpps	xmm1, xmm0				; ~1/fval
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2		; Intel says with NR we can reach 22 bit of precision
								; 0.111111 22 bit of precision and takes some more cycles
		//Do a broadcast operation
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		//movaps	[edi], xmm0			; use the next line to avoid cache pollution
		movntps	[edi], xmm0

		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}

sint glhUnProjectf_SSE_Unaligned_2(sint totalVertex4D, GLfloat *windowCoordinate4D,
	     GLfloat *inverse_modelview_projection,
	     sint *viewport,
	     GLfloat *objectVertex4D)
{
	sint counter=0;		//Count number of errors
	sreal fTempo1=2.0/(sreal)viewport[2];
	sreal fTempo2=2.0/(sreal)viewport[3];
	sreal NegOne=-1.0;
	GLH_ALIGN16 sreal out1[4], in1[4];
	in1[3]=1.0;					//It will always be 1.0

	__asm
	{
		mov		eax, windowCoordinate4D
		mov		edx, totalVertex4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		ecx, inverse_modelview_projection
		mov		edi, objectVertex4D
		movups	xmm6, [ecx+32]
		movups	xmm7, [ecx+48]
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//Transformation of normalized coordinates between -1 and 1
		//in[0]=(windowCoordinate4D[i  ]-(sreal)viewport[0])/(sreal)viewport[2]*2.0-1.0;
		//in[1]=(windowCoordinate4D[i+1]-(sreal)viewport[1])/(sreal)viewport[3]*2.0-1.0;
		//in[2]=2.0*windowCoordinate4D[i+2]-1.0;
		//in[3]=1.0;
		mov		ecx, viewport
		fld		dword ptr[eax]
		fisub	dword ptr[ecx]
		fmul	fTempo1
		lea		esi, in1
		fadd	NegOne
		fstp	dword ptr[esi]
		fld		dword ptr[eax+4]
		fisub	dword ptr[ecx+4]
		fmul	fTempo2
		fadd	NegOne
		fstp	dword ptr[esi+4]
		fld		dword ptr[eax+8]
		fadd	dword ptr[eax+8]
		fadd	NegOne
		fstp	dword ptr[esi+8]
	
		//Time to do a matrix multiplication with SSE.
		//This will get us from clip space to object space.
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&objectVertex4D[i], inverse_modelview_projection, in);
		mov		ecx, inverse_modelview_projection
		movups	xmm0, [ecx]						; aligned move, get first column
		movups	xmm5, [esi]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movups	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, xmm6						; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, xmm7						; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		lea		ecx, out1
		addps	xmm0, xmm3
		movups	[ecx], xmm0
		//////////HERE
		cmp		dword ptr[ecx+12], 0			; w is 0?
		jne		NoErrorDetected000
		inc		counter
		jmp		Skip1345
		NoErrorDetected000:
		//out[3]=1.0/out[3];
		//objectVertex4D[i  ]=out[0]*out[3];
		//objectVertex4D[i+1]=out[1]*out[3];
		//objectVertex4D[i+2]=out[2]*out[3];
		rcpps	xmm1, xmm0				; ~1/fval
		movaps	xmm2, xmm0
		mulps	xmm2, xmm1
		mulps	xmm2, xmm1
		addps	xmm1, xmm1
		subps	xmm1, xmm2		; Intel says with NR we can reach 22 bit of precision
								; 0.111111 22 bit of precision and takes some more cycles
		//Do a broadcast operation
		shufps	xmm1, xmm1, 0xFF
		mulps	xmm0, xmm1
		movups	[edi], xmm0

		Skip1345:
		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return counter;
}
#endif

sint glhDoesProcessorSupportMMX()
{
#if !USE_INLINE_ASM
	return 0;
#else
	sint returnVal=0;
	__asm
	{
		mov		eax, 1
		cpuid
		test	edx, 0x00800000		; is MMX technology bit (bit
									; 23)in feature flags equal to 1
		jz		NotFound
		mov		returnVal, 1
		NotFound:
	}
	return returnVal;
#endif
}

sint glhDoesProcessorSupportSSE()
{
#if !USE_INLINE_ASM
	return 0;
#else
	sint returnVal=0;
	__asm
	{
		mov		eax, 1
		cpuid
		test	edx, 0x02000000		; bit 25 in feature flags equal to 1
		jz		NotFound
		mov		returnVal, 1
		NotFound:
	}
	return returnVal;
#endif
}

sint glhDoesProcessorSupportSSE2()
{
#if !USE_INLINE_ASM
	return 0;
#else
	sint returnVal=0;
	__asm
	{
		mov		eax, 1
		cpuid
		test	edx, 0x04000000		; bit 26 in feature flags equal to 1
		jz		NotFound
		mov		returnVal, 1
		NotFound:
	}
	return returnVal;
#endif
}

sint glhDoesProcessorSupport3DNow()
{
#if !USE_INLINE_ASM
	return 0;
#else
	sint returnVal=0;
	__asm
	{
		mov		eax, 0x80000001
		cpuid
		test	edx, 0x80000000		; bit 31 in feature flags equal to 1
		jz		NotFound
		mov		returnVal, 1
		NotFound:
	}
	return returnVal;
#endif
}

sint glhDoesProcessorSupport3DNowExtended()
{
#if !USE_INLINE_ASM
	return 0;
#else
	sint returnVal=0;
	__asm
	{
		mov		eax, 0x80000001
		cpuid
		test	edx, 0x40000000		; bit 30 in feature flags equal to 1
		jz		NotFound
		mov		returnVal, 1
		NotFound:
	}
	return returnVal;
#endif
}


sint glhDoesOSSupportSSE()
{
#if !USE_INLINE_ASM
	return 0;
#else
	try
	{	//Execute a  SSE instruction and see if it works
		__asm	xorps		xmm0, xmm0	;Streaming SIMD Extension
	}
	catch(...)
	{
		return 0;				//Streaming SIMD Extensions not supported
	}
	//Streaming SIMD Extensions are supported by OS
	return 1;
#endif
}

sint glhMultiplyMatrixByVector4by4f_1(sint totalVector4D, GLfloat *vector4D,
				GLfloat *matrix,
				GLfloat *result4D)
{
	sint i;
	for(i=0; i<totalVector4D; i+=4)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&result4D[i], matrix, &vector3D[i]);
		result4D[i  ]=matrix[0]*vector4D[i]+
			matrix[4]*vector4D[i+1]+
			matrix[8]*vector4D[i+2]+
			matrix[12]*vector4D[i+3];		//w is 1
		result4D[i+1]=matrix[1]*vector4D[i]+
			matrix[5]*vector4D[i+1]+
			matrix[9]*vector4D[i+2]+
			matrix[13]*vector4D[i+3];
		result4D[i+2]=matrix[2]*vector4D[i]+
			matrix[6]*vector4D[i+1]+
			matrix[10]*vector4D[i+2]+
			matrix[14]*vector4D[i+3];
		result4D[i+3]=matrix[3]*vector4D[i]+
			matrix[7]*vector4D[i+1]+
			matrix[11]*vector4D[i+2]+
			matrix[15]*vector4D[i+3];
	}
	return 0;
}

sint glhMultiplyMatrixByVector4by4f_2(sint totalVector3D, GLfloat *vector3D,
				GLfloat *matrix,
				GLfloat *result3D)
{
	sint i;
	for(i=0; i<totalVector3D; i+=3)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&result4D[i], matrix, &vector3D[i]);
		result3D[i  ]=matrix[0]*vector3D[i]+
			matrix[4]*vector3D[i+1]+
			matrix[8]*vector3D[i+2]+
			matrix[12];				//w is 1
		result3D[i+1]=matrix[1]*vector3D[i]+
			matrix[5]*vector3D[i+1]+
			matrix[9]*vector3D[i+2]+
			matrix[13];
		result3D[i+2]=matrix[2]*vector3D[i]+
			matrix[6]*vector3D[i+1]+
			matrix[10]*vector3D[i+2]+
			matrix[14];
	}
	return 0;
}

sint glhMultiplyMatrixByVector4by4f_3(sint totalVector3D, GLfloat *vector3D,
				GLfloat *matrix,
				GLfloat *result3D)
{
	sint i;
	for(i=0; i<totalVector3D; i+=3)
	{
		//MultiplyMatrixByVector4by4OpenGL_FLOAT(&result4D[i], matrix, &vector3D[i]);
		result3D[i  ]=matrix[0]*vector3D[i]+
			matrix[4]*vector3D[i+1]+
			matrix[8]*vector3D[i+2];				//w is 0
		result3D[i+1]=matrix[1]*vector3D[i]+
			matrix[5]*vector3D[i+1]+
			matrix[9]*vector3D[i+2];
		result3D[i+2]=matrix[2]*vector3D[i]+
			matrix[6]*vector3D[i+1]+
			matrix[10]*vector3D[i+2];
	}
	return 0;
}

#if USE_INLINE_ASM
sint glhMultiplyMatrixByVector4by4f_SSE_Aligned_1(sint totalVector4D, GLfloat *vector4D,
				GLfloat *matrix,
				GLfloat *result4D)
{
	//Just multiply vector with matrix and store in result.
	__asm
	{
		//Load the vertex
		mov		eax, vector4D
		mov		edx, totalVector4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		ecx, matrix
		mov		edi, result4D
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//**** First, transform vertex from object space to clip space
		//We will be needing vertex 4 times (can't do movaps xmm0, [vertex])
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [eax]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [ecx+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [ecx+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		//movaps	[edi], xmm0					; use the next line to avoid cache pollution
		movntps	[edi], xmm0

		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return 0;
}

sint glhMultiplyMatrixByVector4by4f_SSE_Aligned_WarmCache_1(sint totalVector4D, GLfloat *vector4D,
				GLfloat *matrix,
				GLfloat *result4D)
{
	//Just multiply vector with matrix and store in result.
	__asm
	{
		//Load the vertex
		mov		eax, vector4D
		mov		edx, totalVector4D
		imul	edx, 4					; 4 bytes per sreal
		add		edx, eax
		mov		ecx, matrix
		mov		edi, result4D
		cmp		eax, edx
		jge		ExitLoop5000x
		Startup5000x:
		//**** First, transform vertex from object space to clip space
		//We will be needing vertex 4 times (can't do movaps xmm0, [vertex])
		movaps	xmm0, [ecx]						; aligned move, get first column
		movaps	xmm5, [eax]						; aligned move, get entire vector
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [ecx+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [ecx+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the resultVector
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		//movaps	[edi], xmm0					; use the next line to avoid cache pollution
		movntps	[edi], xmm0

		//Prefetch next 32 bytes into L2 and L1 cache
		//cache lines on P3 and P4 are 32 bytes. I think that on Athlon, it's 64 bytes.
		//Will this cause a invalid memory access if we go beyond array boundary? Not sure...
		//prefetch1	[eax+32]		These give error message "error C2400: inline assembler syntax error in 'opcode'; found '['"
		//even prefetch0 gives errors. Only prefetchnta works
		prefetchnta	[eax+32]
		////////////////////////////////

		add		eax, 10h				; advance 16 bytes (each vertex is 16 bytes)
		add		edi, 10h				; advance 16 bytes (each vertex is 16 bytes)
		cmp		eax, edx
		jl		Startup5000x
		ExitLoop5000x:
	}
	return 0;
}
#endif // #if USE_INLINE_ASM

void glhMultiplyMatrixByMatrix4by4f_1(GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
{
	result[0]=leftMatrix[0]*rightMatrix[0]+
		leftMatrix[4]*rightMatrix[1]+
		leftMatrix[8]*rightMatrix[2]+
		leftMatrix[12]*rightMatrix[3];
	result[4]=leftMatrix[0]*rightMatrix[4]+
		leftMatrix[4]*rightMatrix[5]+
		leftMatrix[8]*rightMatrix[6]+
		leftMatrix[12]*rightMatrix[7];
	result[8]=leftMatrix[0]*rightMatrix[8]+
		leftMatrix[4]*rightMatrix[9]+
		leftMatrix[8]*rightMatrix[10]+
		leftMatrix[12]*rightMatrix[11];
	result[12]=leftMatrix[0]*rightMatrix[12]+
		leftMatrix[4]*rightMatrix[13]+
		leftMatrix[8]*rightMatrix[14]+
		leftMatrix[12]*rightMatrix[15];

	result[1]=leftMatrix[1]*rightMatrix[0]+
		leftMatrix[5]*rightMatrix[1]+
		leftMatrix[9]*rightMatrix[2]+
		leftMatrix[13]*rightMatrix[3];
	result[5]=leftMatrix[1]*rightMatrix[4]+
		leftMatrix[5]*rightMatrix[5]+
		leftMatrix[9]*rightMatrix[6]+
		leftMatrix[13]*rightMatrix[7];
	result[9]=leftMatrix[1]*rightMatrix[8]+
		leftMatrix[5]*rightMatrix[9]+
		leftMatrix[9]*rightMatrix[10]+
		leftMatrix[13]*rightMatrix[11];
	result[13]=leftMatrix[1]*rightMatrix[12]+
		leftMatrix[5]*rightMatrix[13]+
		leftMatrix[9]*rightMatrix[14]+
		leftMatrix[13]*rightMatrix[15];

	result[2]=leftMatrix[2]*rightMatrix[0]+
		leftMatrix[6]*rightMatrix[1]+
		leftMatrix[10]*rightMatrix[2]+
		leftMatrix[14]*rightMatrix[3];
	result[6]=leftMatrix[2]*rightMatrix[4]+
		leftMatrix[6]*rightMatrix[5]+
		leftMatrix[10]*rightMatrix[6]+
		leftMatrix[14]*rightMatrix[7];
	result[10]=leftMatrix[2]*rightMatrix[8]+
		leftMatrix[6]*rightMatrix[9]+
		leftMatrix[10]*rightMatrix[10]+
		leftMatrix[14]*rightMatrix[11];
	result[14]=leftMatrix[2]*rightMatrix[12]+
		leftMatrix[6]*rightMatrix[13]+
		leftMatrix[10]*rightMatrix[14]+
		leftMatrix[14]*rightMatrix[15];

	result[3]=leftMatrix[3]*rightMatrix[0]+
		leftMatrix[7]*rightMatrix[1]+
		leftMatrix[11]*rightMatrix[2]+
		leftMatrix[15]*rightMatrix[3];
	result[7]=leftMatrix[3]*rightMatrix[4]+
		leftMatrix[7]*rightMatrix[5]+
		leftMatrix[11]*rightMatrix[6]+
		leftMatrix[15]*rightMatrix[7];
	result[11]=leftMatrix[3]*rightMatrix[8]+
		leftMatrix[7]*rightMatrix[9]+
		leftMatrix[11]*rightMatrix[10]+
		leftMatrix[15]*rightMatrix[11];
	result[15]=leftMatrix[3]*rightMatrix[12]+
		leftMatrix[7]*rightMatrix[13]+
		leftMatrix[11]*rightMatrix[14]+
		leftMatrix[15]*rightMatrix[15];
}

void glhMultiplyMatrixByMatrix4by4f_2(sint countMatrix, GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
{
	sint i, total;
	total=countMatrix*16;
	for(i=0; i<total; i+=16)
	{
		result[i]=leftMatrix[i]*rightMatrix[i]+
			leftMatrix[i+4]*rightMatrix[i+1]+
			leftMatrix[i+8]*rightMatrix[i+2]+
			leftMatrix[i+12]*rightMatrix[i+3];
		result[i+4]=leftMatrix[i]*rightMatrix[i+4]+
			leftMatrix[i+4]*rightMatrix[i+5]+
			leftMatrix[i+8]*rightMatrix[i+6]+
			leftMatrix[i+12]*rightMatrix[i+7];
		result[i+8]=leftMatrix[i]*rightMatrix[i+8]+
			leftMatrix[i+4]*rightMatrix[i+9]+
			leftMatrix[i+8]*rightMatrix[i+10]+
			leftMatrix[i+12]*rightMatrix[i+11];
		result[i+12]=leftMatrix[i]*rightMatrix[i+12]+
			leftMatrix[i+4]*rightMatrix[i+13]+
			leftMatrix[i+8]*rightMatrix[i+14]+
			leftMatrix[i+12]*rightMatrix[i+15];

		result[i+1]=leftMatrix[i+1]*rightMatrix[i]+
			leftMatrix[i+5]*rightMatrix[i+1]+
			leftMatrix[i+9]*rightMatrix[i+2]+
			leftMatrix[i+13]*rightMatrix[i+3];
		result[i+5]=leftMatrix[i+1]*rightMatrix[i+4]+
			leftMatrix[i+5]*rightMatrix[i+5]+
			leftMatrix[i+9]*rightMatrix[i+6]+
			leftMatrix[i+13]*rightMatrix[i+7];
		result[i+9]=leftMatrix[i+1]*rightMatrix[i+8]+
			leftMatrix[i+5]*rightMatrix[i+9]+
			leftMatrix[i+9]*rightMatrix[i+10]+
			leftMatrix[i+13]*rightMatrix[i+11];
		result[i+13]=leftMatrix[i+1]*rightMatrix[i+12]+
			leftMatrix[i+5]*rightMatrix[i+13]+
			leftMatrix[i+9]*rightMatrix[i+14]+
			leftMatrix[i+13]*rightMatrix[i+15];

		result[i+2]=leftMatrix[i+2]*rightMatrix[i]+
			leftMatrix[i+6]*rightMatrix[i+1]+
			leftMatrix[i+10]*rightMatrix[i+2]+
			leftMatrix[i+14]*rightMatrix[i+3];
		result[i+6]=leftMatrix[i+2]*rightMatrix[i+4]+
			leftMatrix[i+6]*rightMatrix[i+5]+
			leftMatrix[i+10]*rightMatrix[i+6]+
			leftMatrix[i+14]*rightMatrix[i+7];
		result[i+10]=leftMatrix[i+2]*rightMatrix[i+8]+
			leftMatrix[i+6]*rightMatrix[i+9]+
			leftMatrix[i+10]*rightMatrix[i+10]+
			leftMatrix[i+14]*rightMatrix[i+11];
		result[i+14]=leftMatrix[i+2]*rightMatrix[i+12]+
			leftMatrix[i+6]*rightMatrix[i+13]+
			leftMatrix[i+10]*rightMatrix[i+14]+
			leftMatrix[i+14]*rightMatrix[i+15];

		result[i+3]=leftMatrix[i+3]*rightMatrix[i]+
			leftMatrix[i+7]*rightMatrix[i+1]+
			leftMatrix[i+11]*rightMatrix[i+2]+
			leftMatrix[i+15]*rightMatrix[i+3];
		result[i+7]=leftMatrix[i+3]*rightMatrix[i+4]+
			leftMatrix[i+7]*rightMatrix[i+5]+
			leftMatrix[i+11]*rightMatrix[i+6]+
			leftMatrix[i+15]*rightMatrix[i+7];
		result[i+11]=leftMatrix[i+3]*rightMatrix[i+8]+
			leftMatrix[i+7]*rightMatrix[i+9]+
			leftMatrix[i+11]*rightMatrix[i+10]+
			leftMatrix[i+15]*rightMatrix[i+11];
		result[i+15]=leftMatrix[i+3]*rightMatrix[i+12]+
			leftMatrix[i+7]*rightMatrix[i+13]+
			leftMatrix[i+11]*rightMatrix[i+14]+
			leftMatrix[i+15]*rightMatrix[i+15];
	}
}


#if USE_INLINE_ASM
void glhMultiplyMatrixByMatrix4by4f_SSE_Aligned_WarmCache_1(GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
{
	__asm
	{
		mov		eax, leftMatrix

		//Cache to L1
		prefetchnta	[eax]						; cache the left matrix second column now
		prefetchnta	[eax+16]					; cache the left matrix second column now
		prefetchnta	[eax+32]					; cache the left matrix second column now
		prefetchnta	[eax+48]					; cache the left matrix second column now

		mov		edx, result
		mov		ecx, rightMatrix

		//Cache to L1
		prefetchnta [ecx+16]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx]						; aligned move, get first column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx], xmm0

		//Cache to L1
		prefetchnta [ecx+32]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+16], xmm0

		//Cache to L1
		prefetchnta [ecx+48]					; cache the right matrix second column now

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+32], xmm0

		movaps	xmm0, [eax]						; aligned move, get first column
		movaps	xmm5, [ecx+48]					; aligned move, get fourth column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0					; first 32 bit copied to all 32 bits
		mulps	xmm0, xmm4
		movaps	xmm1, [eax+16]					; aligned move, get second column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0x55				; second 32 bit copied to all 32 bits
		mulps	xmm1, xmm4
		movaps	xmm2, [eax+32]					; aligned move, get third column
		movaps	xmm4, xmm5
		shufps	xmm4, xmm4, 0xAA				; third 32 bit copied to all 32 bits
		mulps	xmm2, xmm4
		movaps	xmm3, [eax+48]					; aligned move, get fourth column
		shufps	xmm5, xmm5, 0xFF				; fourth 32 bit copied to all 32 bits
		mulps	xmm3, xmm5
		addps	xmm0, xmm1						; now add them all up to make the result
		addps	xmm0, xmm2
		addps	xmm0, xmm3
		movaps	[edx+48], xmm0
	}
}

void glhMultiplyMatrixByMatrix4by4f_3DNow(GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
{
	__asm
    {
        mov         ecx,[rightMatrix]
        mov         eax,[result]
        mov         edx,[leftMatrix]

        movq        mm0,[ecx]		//; a_21	| a_11
        movq        mm1,[ecx+8]		//; a_41	| a_31
        movq        mm4,[edx]		//; b_21	| b_11
        punpckhdq   mm2,mm0			//; a_21	| 
        movq        mm5,[edx+16]	//; b_22	| b_12
        punpckhdq   mm3,mm1			//; a_41	| 
        movq        mm6,[edx+32]	//; b_23	| b_13
        punpckldq   mm0,mm0			//; a_11	| a_11
        punpckldq   mm1,mm1			//; a_31	| a_31
        pfmul       mm4,mm0			//; a_11 * b_21 | a_11 * b_11
        punpckhdq   mm2,mm2			//; a_21	| a_21
        pfmul       mm0,[edx+8]		//; a_11 * b_41 | a_11 * b_31
        movq        mm7,[edx+48]	//; b_24	| b_14
        pfmul       mm5,mm2			//; a_21 * b_22 | a_21 * b_12
        punpckhdq   mm3,mm3			//; a_41	| a_41
        pfmul       mm2,[edx+24]	//; a_21 * b_42 | a_21 * b_32
        pfmul       mm6,mm1			//; a_31 * b_23 | a_31 * b_13 
        pfadd       mm5,mm4			//; a_21 * b_22 + a_11 * b_21 | a_21 * b_12 + a_11 * b_11
        pfmul       mm1,[edx+40]	//; a_31 * b_43 | a_31 * b_33
        pfadd       mm2,mm0			//; a_21 * b_42 + a_11 * b_41 | a_21 * b_32 + a_11 * b_31
        pfmul       mm7,mm3			//; a_41 * b_24 | a_41 * b_14 
        pfadd       mm6,mm5			//; a_21 * b_22 + a_11 * b_21 + a_31 * b_23 | a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        pfmul       mm3,[edx+56]	//; a_41 * b_44 | a_41 * b_34
        pfadd       mm2,mm1			//; a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_21 * b_32 + a_11 * b_31 + a_31 * b_33 
        pfadd       mm7,mm6			//; a_41 * b_24 + a_21 * b_22 + a_11 * b_21 + a_31 * b_23 |  a_41 * b_14 + a_21 * b_12 + a_11 * b_11 + a_31 * b_13
        movq        mm0,[ecx+16]	//; a_22	| a_12
        pfadd       mm3,mm2			//; a_41 * b_44 + a_21 * b_42 + a_11 * b_41 + a_31 * b_43 | a_41 * b_34 + a_21 * b_32 + a_11 * b_31 + a_31 * b_33 
        movq        mm1,[ecx+24]	//; a_42	| a_32
        movq        [eax],mm7		//; r_21	| r_11 
        movq        mm4,[edx]		//; b_21	| b_11
        movq        [eax+8],mm3		//; r_41	| r_31

        punpckhdq   mm2,mm0			//; a_22	| XXX
        movq        mm5,[edx+16]	//; b_22	| b_12
        punpckhdq   mm3,mm1			//; a_42	| XXX
        movq        mm6,[edx+32]	//; b_23	| b_13
        punpckldq   mm0,mm0			//; a_12	| a_12
        punpckldq   mm1,mm1			//; a_32	| a_32
        pfmul       mm4,mm0			//; a_12 * b_21 | a_12 * b_11
        punpckhdq   mm2,mm2			//; a_22	| a_22
        pfmul       mm0,[edx+8]		//; a_12 * b_41 | a_12 * b_31
        movq        mm7,[edx+48]	//; b_24	| b_14
        pfmul       mm5,mm2			//; a_22 * b_22 | a_22 * b12
        punpckhdq   mm3,mm3			//; a_42	| a_42
        pfmul       mm2,[edx+24]	//; a_22 * b_42 | a_22 * b_32
        pfmul       mm6,mm1			//; a_32 * b_23 | a_32 * b_13
        pfadd       mm5,mm4			//; a_12 * b_21 + a_22 * b_22 | a_12 * b_11 + a_22 * b12
        pfmul       mm1,[edx+40]	//; a_32 * b_43 | a_32 * b_33
        pfadd       mm2,mm0			//; a_12 * b_41 + a_22 * b_42 | a_12 * b_11 + a_22 * b_32
        pfmul       mm7,mm3			//; a_42 * b_24 | a_42 * b_14
        pfadd       mm6,mm5			//; a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_32 * b_13 + a_12 * b_11 + a_22 * b12
        pfmul       mm3,[edx+56]	//; a_42 * b_44 | a_42 * b_34
        pfadd       mm2,mm1			//; a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        pfadd       mm7,mm6			//; a_42 * b_24 + a_32 * b_23 + a_12 * b_21 + a_22 * b_22 | a_42 * b_14 + a_32 * b_13 + a_12 * b_11 + a_22 * b12
        movq        mm0,[ecx+32]	//; a_23 | a_13
        pfadd       mm3,mm2			//; a_42 * b_44 + a_32 * b_43 + a_12 * b_41 + a_22 * b_42 | a_42 * b_34 + a_32 * b_33 + a_12 * b_11 + a_22 * b_32
        movq        mm1,[ecx+40]	//; a_43 | a_33
        movq        [eax+16],mm7	//; r_22 | r_12
        movq        mm4,[edx]		//; b_21	| b_11
        movq        [eax+24],mm3	//; r_42 | r_32

        punpckhdq   mm2,mm0			//; a_23 | XXX
        movq        mm5,[edx+16]	//; b_22 | b_12
        punpckhdq   mm3,mm1			//; a_43 | XXX
        movq        mm6,[edx+32]	//; b_23 | b_13
        punpckldq   mm0,mm0			//; a_13 | a_13
        punpckldq   mm1,mm1			//; a_33 | a_33
        pfmul       mm4,mm0			//; a_13 * b_21 | a_13 * b_11
        punpckhdq   mm2,mm2			//; a_23 | a_23
        pfmul       mm0,[edx+8]		//; a_13 * b_41 | a_13 * b_31
        movq        mm7,[edx+48]	//; b_24 | b_14
        pfmul       mm5,mm2			//; a_23 * b_22 | a_23 * b_12
        punpckhdq   mm3,mm3			//; a_43 | a_43
        pfmul       mm2,[edx+24]	//; a_23 * b_42 | a_23 * b_32
        pfmul       mm6,mm1			//; a_33 * b_23 | a_33 * b_13
        pfadd       mm5,mm4			//; a_23 * b_22 + a_13 * b_21 | a_23 * b_12 + a_13 * b_11
        pfmul       mm1,[edx+40]	//; a_33 * b_43 | a_33 * b_33 
        pfadd       mm2,mm0			//; a_13 * b_41 + a_23 * b_42 | a_13 * b_31 + a_23 * b_32
        pfmul       mm7,mm3			//; a_43 * b_24 | a_43 * b_14
        pfadd       mm6,mm5			//; a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        pfmul       mm3,[edx+56]	//; a_43 * b_44 | a_43 * b_34
        pfadd       mm2,mm1			//; a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        pfadd       mm7,mm6			//; a_43 * b_24 + a_33 * b_23 + a_23 * b_22 + a_13 * b_21 | a_43 * b_14 + a_33 * b_13 + a_23 * b_12 + a_13 * b_11
        movq        mm0,[ecx+48]	//; a_24 | a_14
        pfadd       mm3,mm2			//; a_43 * b_44 + a_33 * b_43 * a_13 * b_41 + a_23 * b_42 | a_43 * b_34 + a_33 * b_33 + a_13 * b_31 + a_23 * b_32
        movq        mm1,[ecx+56]	//; a_44 | a_34
        movq        [eax+32],mm7	//; r_23 | r_13
        movq        mm4,[edx]		//; b_21 | b_11
        movq        [eax+40],mm3	//; r_43 | r_33

        punpckhdq   mm2,mm0			//; a_24 | XXX
        movq        mm5,[edx+16]	//; b_22 | b_12
        punpckhdq   mm3,mm1			//; a_44 | XXX
        movq        mm6,[edx+32]	//; b_23 | b_13
        punpckldq   mm0,mm0			//; a_14 | a_14
        punpckldq   mm1,mm1			//; a_34 | a_34
        pfmul       mm4,mm0			//; a_14 * b_21 | a_14 * b_11
        punpckhdq   mm2,mm2			//; a_24 | a_24
        pfmul       mm0,[edx+8]		//; a_14 * b_41 | a_14 * b_31
        movq        mm7,[edx+48]	//; b_24 | b_14
        pfmul       mm5,mm2			//; a_24 * b_22 | a_24 * b_12
        punpckhdq   mm3,mm3			//; a_44 | a_44
        pfmul       mm2,[edx+24]	//; a_24 * b_ 42 | a_24 * b_32
        pfmul       mm6,mm1			//; a_34 * b_23 | a_34 * b_13
        pfadd       mm5,mm4			//; a_14 * b_21 + a_24 * b_22 | a_14 * b_11 + a_24 * b_12
        pfmul       mm1,[edx+40]	//; a_34 * b_43 | a_34 * b_33
        pfadd       mm2,mm0			//; a_14 * b_41 + a_24 * b_ 42 | a_14 * b_31 + a_24 * b_32
        pfmul       mm7,mm3			//; a_44 * b_24 | a_44 * b_14
        pfadd       mm6,mm5			//; a_34 * b_23 + a_14 * b_21 + a_24 * b_22 | a_34 * b_13 + a_14 * b_11 + a_24 * b_12
        pfmul       mm3,[edx+56]	//; a_44 * b_44 | a_44 * b_34
        pfadd       mm2,mm1			//; a_34 * b_43 + a_14 * b_41 + a_24 * b_ 42 | a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        pfadd       mm7,mm6			//; a_44 * b_24 + a_14 * b_23 + a_24 * b_ 42 | a_44 * b_14 + a_14 * b_31 + a_24 * b_32
        pfadd       mm3,mm2			//; a_44 * b_44 + a_34 * b_43 + a_14 * b_41 + a_24 * b_42 | a_44 * b_34 + a_34 * b_33 + a_14 * b_31 + a_24 * b_32
        movq        [eax+48],mm7	//; r_24 | r_14
        movq        [eax+56],mm3	//; r_44 | r_34
        femms						; restore FPU state
    }
}
#endif // #if USE_INLINE_ASM

//PURPOSE:	A fast way to get the inverse.
//			Assuming no scale in the modelview.
//			Matrix should be orthogonal.
void glhQuickInvertMatrixd2(sreal2 *m, sreal2 *result)
{
	result[0]=m[0];
	result[1]=m[4];
	result[2]=m[8];
	result[3]=0.0;
	result[4]=m[1];
	result[5]=m[5];
	result[6]=m[9];
	result[7]=0.0;
	result[8]=m[2];
	result[9]=m[6];
	result[10]=m[10];
	result[11]=0.0;
	result[12]=-m[12]*m[0]-m[13]*m[1]-m[14]*m[2];
	result[13]=-m[12]*m[4]-m[13]*m[5]-m[14]*m[6];
	result[14]=-m[12]*m[8]-m[13]*m[9]-m[14]*m[10];
	result[15]=1.0;
}

void glhQuickInvertMatrixf2(GLfloat *m, GLfloat *result)
{
	result[0]=m[0];
	result[1]=m[4];
	result[2]=m[8];
	result[3]=0.0;
	result[4]=m[1];
	result[5]=m[5];
	result[6]=m[9];
	result[7]=0.0;
	result[8]=m[2];
	result[9]=m[6];
	result[10]=m[10];
	result[11]=0.0;
	result[12]=-m[12]*m[0]-m[13]*m[1]-m[14]*m[2];
	result[13]=-m[12]*m[4]-m[13]*m[5]-m[14]*m[6];
	result[14]=-m[12]*m[8]-m[13]*m[9]-m[14]*m[10];
	result[15]=1.0;
}

void glhInitLibrary()
{
	//ErrorState=GLH_ERROR_NONE;
	TotalErrorStateStack=0;

	MemoryAlignmentOnAllocation=16;
	FArray128=(sreal *)_aligned_malloc(sizeof(sreal)*128, MemoryAlignmentOnAllocation);
}

void glhShutDownLibrary()
{
	//ErrorState=GLH_ERROR_NONE;
	while(!ErrorStateStack.empty())
	{
		//Remove all elements
		ErrorStateStack.pop();
	}

	TotalErrorStateStack=0;

	if(FArray128)
	{
		_aligned_free(FArray128);
		FArray128=NULL;
	}
}

sint glhGetError()
{
	//sint errorState=ErrorState;
	//ErrorState=GLH_ERROR_NONE;

	sint errorState;

	if(TotalErrorStateStack==0)
	{
		errorState=GLH_ERROR_NONE;
	}
	else
	{
		if(ErrorStateStack.empty()==true)
		{
			errorState=GLH_ERROR_INTERNALERROR;
			TotalErrorStateStack=0;					//Reset
		}
		else
		{
			errorState=ErrorStateStack.top();
			ErrorStateStack.pop();
			TotalErrorStateStack--;
		}
	}

	return errorState;
}

sint glhCopyErrorInto_cBuffer(char *pstring)
{
	if(pstring==NULL)
		return 1;
	sint errorState=glhGetError();
	if(errorState==GLH_ERROR_NONE)
	{
		strcpy(pstring, "GLH_ERROR_NONE");
		return 1;
	}
	else if(errorState==GLH_ERROR_OUT_OF_MEMORY)
		strcpy(pstring, "GLH_ERROR_OUT_OF_MEMORY");
	else if(errorState==GLH_ERROR_INVALID_PARAM)
		strcpy(pstring, "GLH_ERROR_INVALID_PARAM");
	else if(errorState==GLH_ERROR_INVALID_ID)
		strcpy(pstring, "GLH_ERROR_INVALID_ID");
	else
		pstring[0]=0;

	return -1;
}






sint glhConvertHeightMapToNormalMap(sint dataAlignment, sint width, sint height, sint border, GLfloat scaleFactor,
								unsigned int isPromoteToHigherPowerOn, uint format, const uchar *indata, uint outformat, uchar *outdata, unsigned int alphaHeight, sint heightSource)
{
	sint i, j, tempo, wr, hr;
	sreal sqlen, reciplen, nx, ny, nz;
	const sreal oneOver255=1.0/255.0;
	sreal c, cx, cy, dcx, dcy;
	sint width2, height2;
	sint bytesPerPixel;
	sint scanLine;
	sint scanLine2;

	if((dataAlignment!=1)&&(dataAlignment!=4))
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if((border!=0)&&(border!=1))
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	
	if(format==GL_LUMINANCE)
	{
		//Make sure dimension are power of 2
		if(border==1)
		{
			i=glhLowerPowerOfTwo(width-1, &width2, height-1, &height2);
			if(i==0)
			{
				if(TotalErrorStateStack<100)
				{
					ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
					TotalErrorStateStack++;
				}
				return -1;
			}

			wr=width;
			width--;
			hr=height;
			height--;
		}
		else if(border==0)
		{
			i=glhLowerPowerOfTwo(width, &width2, height, &height2);
			if(i==0)
			{
				if(TotalErrorStateStack<100)
				{
					ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
					TotalErrorStateStack++;
				}
				return -1;
			}

			wr=width;
			hr=height;
		}
		else
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}

		bytesPerPixel=1;
		scanLine=((wr*bytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		scanLine2=((wr*4+dataAlignment-1)/dataAlignment)*dataAlignment;

		if(outformat==GL_RGBA)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+(j+1)%wr]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo  ]=128+127*nx;
					outdata[tempo+1]=128+127*ny;
					outdata[tempo+2]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo+3]=255;
					else
					{	//Copy LUMINANCE value to alpha
						outdata[tempo+3]=indata[i*scanLine+j];
					}
				}
			}
		}
		else if(outformat==GL_BGRA)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+(j+1)%wr]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo+2]=128+127*nx;
					outdata[tempo+1]=128+127*ny;
					outdata[tempo  ]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo+3]=255;
					else
					{	//Copy LUMINANCE value to alpha
						outdata[tempo+3]=indata[i*scanLine+j];
					}
				}
			}
		}
		else if(outformat==GL_ABGR)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+(j+1)%wr]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo+3]=128+127*nx;
					outdata[tempo+2]=128+127*ny;
					outdata[tempo+1]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo  ]=255;
					else
					{	//Copy LUMINANCE value to alpha
						outdata[tempo  ]=indata[i*scanLine+j];
					}
				}
			}
		}
		else
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}
	else if((format==GL_RGB)||(format==GL_BGR)||(format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		//Make sure dimension are power of 2
		if(border==1)
		{
			i=glhLowerPowerOfTwo(width-1, &width2, height-1, &height2);
			if(i==0)
			{
				if(TotalErrorStateStack<100)
				{
					ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
					TotalErrorStateStack++;
				}
				return -1;
			}

			wr=width;
			width--;
			hr=height;
			height--;
		}
		else if(border==0)
		{
			i=glhLowerPowerOfTwo(width, &width2, height, &height2);
			if(i==0)
			{
				if(TotalErrorStateStack<100)
				{
					ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
					TotalErrorStateStack++;
				}
				return -1;
			}

			wr=width;
			hr=height;
		}
		else
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}

		if((format==GL_RGB)||(format==GL_BGR))
			bytesPerPixel=3;
		else
			bytesPerPixel=4;

		scanLine=((wr*bytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		scanLine2=((wr*4+dataAlignment-1)/dataAlignment)*dataAlignment;
		
		if(outformat==GL_RGBA)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j*bytesPerPixel]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+((j+1)%wr)*bytesPerPixel]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j*bytesPerPixel]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo  ]=128+127*nx;
					outdata[tempo+1]=128+127*ny;
					outdata[tempo+2]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo+3]=255;
					else
					{
						switch(format)
						{
						case GL_RGB:
						case GL_RGBA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							}
							break;
						case GL_BGR:
						case GL_BGRA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel];
								break;
							}
							break;
						case GL_ABGR:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+3];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							}
							break;
						}
					}
				}
			}
		}
		else if(outformat==GL_BGRA)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j*bytesPerPixel]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+((j+1)%wr)*bytesPerPixel]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j*bytesPerPixel]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo+2]=128+127*nx;
					outdata[tempo+1]=128+127*ny;
					outdata[tempo  ]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo+3]=255;
					else
					{
						switch(format)
						{
						case GL_RGB:
						case GL_RGBA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							}
							break;
						case GL_BGR:
						case GL_BGRA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel];
								break;
							}
							break;
						case GL_ABGR:
							switch(heightSource)
							{
							case 0:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+3];
								break;
							case 1:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 2:
								outdata[tempo+3]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							}
							break;
						}
					}
				}
			}
		}
		else if(outformat==GL_ABGR)
		{
			for(i=0; i<height; i++)
			{
				for(j=0; j<width; j++)
				{
					//Expand [0,255] texel values to the [0,1] range.
					c=(sreal)indata[i*scanLine+j*bytesPerPixel]*oneOver255;
					//Expand the texel to its right.
					cx=indata[i*scanLine+((j+1)%wr)*bytesPerPixel]*oneOver255;
					//Expand the texel one up.
					cy=indata[((i+1)%hr)*scanLine+j*bytesPerPixel]*oneOver255;
					dcx=scaleFactor*(c-cx);
					dcy=scaleFactor*(c-cy);

					//Normalize the vector.
					sqlen=dcx*dcx+dcy*dcy+1.0;
					reciplen=1.0/sqrtf(sqlen);
					nx=dcy*reciplen;
					ny=-dcx*reciplen;
					nz=reciplen;

					//Repack the normalized vector into an RGB unsigned byte
					//vector in the normal map image.
					tempo=i*scanLine2+j*4;
					outdata[tempo+3]=128+127*nx;
					outdata[tempo+2]=128+127*ny;
					outdata[tempo+1]=128+127*nz;

					//The highest resolution mipmap level always has a
					//unit length magnitude.
					if(alphaHeight==FALSE)
						outdata[tempo+3]=255;
					else
					{
						switch(format)
						{
						case GL_RGB:
						case GL_RGBA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel];
								break;
							case 1:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							}
							break;
						case GL_BGR:
						case GL_BGRA:
							switch(heightSource)
							{
							case 0:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 1:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							case 2:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel];
								break;
							}
							break;
						case GL_ABGR:
							switch(heightSource)
							{
							case 0:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+3];
								break;
							case 1:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+2];
								break;
							case 2:
								outdata[tempo]=indata[i*scanLine+j*bytesPerPixel+1];
								break;
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}
	else
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	return 1;
}


sint glhNormalMapReducer(sint dataAlignment, sint width, sint height, sint border, GLfloat weightFactor, uint format, const uchar *indata, uchar *outdata, sint param)
{
	/*There are too many possibilities.
	I will only implement #5.
	1. LEFT
	2. LEFT RIGHT
	3. LEFT RIGHT UP
	4. LEFT RIGHT DOWN
	5. LEFT RIGHT UP DOWN
	6. LEFT UP
	7. LEFT UP DOWN
	8. LEFT DOWN
	9. RIGHT
	10. RIGHT UP
	11. RIGHT DOWN
	12. RIGHT UP DOWN
	13. UP
	14. UP DOWN
	15. DOWN*/

	sint i, j, wr, hr;
	sint scanLine;
	const sreal oneOver255=1.0/255.0;
	sint width2, height2;
	sint bytesPerPixel;
	sint temp, temp2, temp3, temp4;
	sreal myvector1[4], myvector2[4], myvector3[4], myvector4[4];
	sreal invlength, weightFactor2;

	if((dataAlignment!=1)&&(dataAlignment!=4))
	{
		goto ExitOnError_1098;
	}
	if((border!=0)&&(border!=1))
	{
		goto ExitOnError_1098;
	}

	if(param!=(GLH_UP | GLH_DOWN | GLH_LEFT | GLH_RIGHT))
	{
		goto ExitOnError_1098;
	}

	if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR))
	{
		//Make sure dimension are power of 2
		if(border==1)
		{
			i=glhLowerPowerOfTwo(width-1, &width2, height-1, &height2);
			if(i==0)
			{
				goto ExitOnError_1098;
			}

			wr=width;
			width--;
			hr=height;
			height--;
		}
		else if(border==0)
		{
			i=glhLowerPowerOfTwo(width, &width2, height, &height2);
			if(i==0)
			{
				goto ExitOnError_1098;
			}

			wr=width;
			hr=height;
		}
		else
		{
			goto ExitOnError_1098;
		}

		bytesPerPixel=4;		//Always 4
		scanLine=((wr*bytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		
		if(format==GL_RGBA)
		{
			switch(param)
			{
			case GLH_UP | GLH_DOWN | GLH_LEFT | GLH_RIGHT:
				for(i=0; i<height; i++)
				{
					for(j=0; j<width; j++)
					{
						//Expand [0,255] texel values to the [0,1] range.
						temp=i*scanLine+j*bytesPerPixel;
						temp2=temp+1;
						temp3=temp+2;
						myvector1[0]=(sreal)indata[temp ]*oneOver255;
						myvector1[1]=(sreal)indata[temp2]*oneOver255;
						myvector1[2]=(sreal)indata[temp3]*oneOver255;

						//Expand normal
						myvector1[0]=(myvector1[0]*2.0)-1.0;
						myvector1[1]=(myvector1[1]*2.0)-1.0;
						myvector1[2]=(myvector1[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=i*scanLine+((j+1)%wr)*bytesPerPixel;
						myvector2[0]=(sreal)indata[temp4  ]*oneOver255;
						myvector2[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector2[2]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector2[0]=(myvector2[0]*2.0)-1.0;
						myvector2[1]=(myvector2[1]*2.0)-1.0;
						myvector2[2]=(myvector2[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+j*bytesPerPixel;
						myvector3[0]=(sreal)indata[temp4  ]*oneOver255;
						myvector3[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector3[2]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector3[0]=(myvector3[0]*2.0)-1.0;
						myvector3[1]=(myvector3[1]*2.0)-1.0;
						myvector3[2]=(myvector3[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+((j+1)%wr)*bytesPerPixel;
						myvector4[0]=(sreal)indata[temp4  ]*oneOver255;
						myvector4[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector4[2]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector4[0]=(myvector4[0]*2.0)-1.0;
						myvector4[1]=(myvector4[1]*2.0)-1.0;
						myvector4[2]=(myvector4[2]*2.0)-1.0;

						//LERP between normal and the 3 other normals
						weightFactor2=(1.0-weightFactor)/3.0;
						myvector1[0]=myvector1[0]*weightFactor+
							(myvector2[0]+
							myvector3[0]+
							myvector4[0])*weightFactor2;
						myvector1[1]=myvector1[1]*weightFactor+
							(myvector2[1]+
							myvector3[1]+
							myvector4[1])*weightFactor2;
						myvector1[2]=myvector1[2]*weightFactor+
							(myvector2[2]+
							myvector3[2]+
							myvector4[2])*weightFactor2;

						//Normalize
						invlength=1.0/sqrtf(myvector1[0]*myvector1[0]+myvector1[1]*myvector1[1]+myvector1[2]*myvector1[2]);
						myvector1[0]*=invlength;
						myvector1[1]*=invlength;
						myvector1[2]*=invlength;

						//Compress normal and write to outdata
						outdata[temp ]=(uchar)((myvector1[0]+1.0)*127.5);
						outdata[temp2]=(uchar)((myvector1[1]+1.0)*127.5);
						outdata[temp3]=(uchar)((myvector1[2]+1.0)*127.5);
					}
				}
				break;
			}
		}
		else if(format==GL_BGRA)
		{
			switch(param)
			{
			case GLH_UP | GLH_DOWN | GLH_LEFT | GLH_RIGHT:
				for(i=0; i<height; i++)
				{
					for(j=0; j<width; j++)
					{
						//Expand [0,255] texel values to the [0,1] range.
						temp=i*scanLine+j*bytesPerPixel;
						temp2=temp+1;
						temp3=temp+2;
						myvector1[2]=(sreal)indata[temp ]*oneOver255;
						myvector1[1]=(sreal)indata[temp2]*oneOver255;
						myvector1[0]=(sreal)indata[temp3]*oneOver255;

						//Expand normal
						myvector1[0]=(myvector1[0]*2.0)-1.0;
						myvector1[1]=(myvector1[1]*2.0)-1.0;
						myvector1[2]=(myvector1[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=i*scanLine+((j+1)%wr)*bytesPerPixel;
						myvector2[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector2[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector2[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector2[0]=(myvector2[0]*2.0)-1.0;
						myvector2[1]=(myvector2[1]*2.0)-1.0;
						myvector2[2]=(myvector2[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+j*bytesPerPixel;
						myvector3[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector3[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector3[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector3[0]=(myvector3[0]*2.0)-1.0;
						myvector3[1]=(myvector3[1]*2.0)-1.0;
						myvector3[2]=(myvector3[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+((j+1)%wr)*bytesPerPixel;
						myvector4[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector4[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector4[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector4[0]=(myvector4[0]*2.0)-1.0;
						myvector4[1]=(myvector4[1]*2.0)-1.0;
						myvector4[2]=(myvector4[2]*2.0)-1.0;

						//LERP between normal and the 3 other normals
						weightFactor2=(1.0-weightFactor)/3.0;
						myvector1[0]=myvector1[0]*weightFactor+
							(myvector2[0]+
							myvector3[0]+
							myvector4[0])*weightFactor2;
						myvector1[1]=myvector1[1]*weightFactor+
							(myvector2[1]+
							myvector3[1]+
							myvector4[1])*weightFactor2;
						myvector1[2]=myvector1[2]*weightFactor+
							(myvector2[2]+
							myvector3[2]+
							myvector4[2])*weightFactor2;

						//Normalize
						invlength=1.0/sqrtf(myvector1[0]*myvector1[0]+myvector1[1]*myvector1[1]+myvector1[2]*myvector1[2]);
						myvector1[0]*=invlength;
						myvector1[1]*=invlength;
						myvector1[2]*=invlength;

						//Compress normal and write to outdata
						outdata[temp ]=(uchar)((myvector1[2]+1.0)*127.5);
						outdata[temp2]=(uchar)((myvector1[1]+1.0)*127.5);
						outdata[temp3]=(uchar)((myvector1[0]+1.0)*127.5);
					}
				}
				break;
			}
		}
		else if(format==GL_ABGR)
		{
			switch(param)
			{
			case GLH_UP | GLH_DOWN | GLH_LEFT | GLH_RIGHT:
				for(i=0; i<height; i++)
				{
					for(j=0; j<width; j++)
					{
						//Expand [0,255] texel values to the [0,1] range.
						temp=i*scanLine+j*bytesPerPixel+1;
						temp2=temp+1;
						temp3=temp+2;
						myvector1[2]=(sreal)indata[temp ]*oneOver255;
						myvector1[1]=(sreal)indata[temp2]*oneOver255;
						myvector1[0]=(sreal)indata[temp3]*oneOver255;

						//Expand normal
						myvector1[0]=(myvector1[0]*2.0)-1.0;
						myvector1[1]=(myvector1[1]*2.0)-1.0;
						myvector1[2]=(myvector1[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=i*scanLine+((j+1)%wr)*bytesPerPixel+1;
						myvector2[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector2[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector2[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector2[0]=(myvector2[0]*2.0)-1.0;
						myvector2[1]=(myvector2[1]*2.0)-1.0;
						myvector2[2]=(myvector2[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+j*bytesPerPixel+1;
						myvector3[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector3[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector3[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector3[0]=(myvector3[0]*2.0)-1.0;
						myvector3[1]=(myvector3[1]*2.0)-1.0;
						myvector3[2]=(myvector3[2]*2.0)-1.0;

						//Expand [0,255] texel values to the [0,1] range.
						temp4=((i+1)%hr)*scanLine+((j+1)%wr)*bytesPerPixel+1;
						myvector4[2]=(sreal)indata[temp4  ]*oneOver255;
						myvector4[1]=(sreal)indata[temp4+1]*oneOver255;
						myvector4[0]=(sreal)indata[temp4+2]*oneOver255;

						//Expand normal
						myvector4[0]=(myvector4[0]*2.0)-1.0;
						myvector4[1]=(myvector4[1]*2.0)-1.0;
						myvector4[2]=(myvector4[2]*2.0)-1.0;

						//LERP between normal and the 3 other normals
						weightFactor2=(1.0-weightFactor)/3.0;
						myvector1[0]=myvector1[0]*weightFactor+
							(myvector2[0]+
							myvector3[0]+
							myvector4[0])*weightFactor2;
						myvector1[1]=myvector1[1]*weightFactor+
							(myvector2[1]+
							myvector3[1]+
							myvector4[1])*weightFactor2;
						myvector1[2]=myvector1[2]*weightFactor+
							(myvector2[2]+
							myvector3[2]+
							myvector4[2])*weightFactor2;

						//Normalize
						invlength=1.0/sqrtf(myvector1[0]*myvector1[0]+myvector1[1]*myvector1[1]+myvector1[2]*myvector1[2]);
						myvector1[0]*=invlength;
						myvector1[1]*=invlength;
						myvector1[2]*=invlength;

						//Compress normal and write to outdata
						outdata[temp ]=(uchar)((myvector1[2]+1.0)*127.5);
						outdata[temp2]=(uchar)((myvector1[1]+1.0)*127.5);
						outdata[temp3]=(uchar)((myvector1[0]+1.0)*127.5);
					}
				}
				break;
			}
		}
		else
		{
			goto ExitOnError_1098;
		}
	}
	else
	{
		goto ExitOnError_1098;
	}

	return 1;
ExitOnError_1098:
	if(TotalErrorStateStack<100)
	{
		ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
		TotalErrorStateStack++;
	}
	return -1;
}


sint glhComputeTangentAndBinormalVectors_v1_f(sint totalIndices, uint *pindices, sint totalVertex, sint numCompVertex, GLfloat *pvertex, sint numCompNormals, GLfloat *pnormals, sint numCompTexCoord, GLfloat *ptexCoord, sint numCompTangent, GLfloat *ptangent, sint numCompBinormal, GLfloat *pbinormal)
{
	sint returnVal;
	//This function is also used by the TSphere, TCylinder and possibly other classes
	returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT(totalIndices, pindices, totalVertex, numCompVertex, pvertex, numCompNormals, pnormals, numCompTexCoord, ptexCoord, numCompTangent, ptangent, numCompBinormal, pbinormal);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
	}
	else if(returnVal==0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
	}

	return returnVal;
}

sint glhComputeTangentAndBinormalVectors_v1_f_2(sint totalIndices, ushort *pindices, sint totalVertex, sint numCompVertex, GLfloat *pvertex, sint numCompNormals, GLfloat *pnormals, sint numCompTexCoord, GLfloat *ptexCoord, sint numCompTangent, GLfloat *ptangent, sint numCompBinormal, GLfloat *pbinormal)
{
	sint returnVal;
	//This function is also used by the TSphere, TCylinder and possibly other classes
	returnVal=ComputeTangentAndBinormalVectors_v1_FLOAT_2(totalIndices, pindices, totalVertex, numCompVertex, pvertex, numCompNormals, pnormals, numCompTexCoord, ptexCoord, numCompTangent, ptangent, numCompBinormal, pbinormal);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
	}
	else if(returnVal==0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
	}

	return returnVal;
}


//PURPOSE:	Get the max mipmap count based on :
//			target : GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
//					RECT textures can't be mipmapped.
//			width : for 1D, 2D, 3D, cube
//			height : for 2D, cube, otherwise ignored
//			depth : for 3D, otherwise ignored
//
//			[OUTPUT] count : receives single sint, which is the mipmap count
//
//			Returns 1 for success, -1 if target is bad, -2 if components is bad
//			If width, height or depth are 0 or less when they matter total receives 0
//			and function returns 1.
sint glhGetMaxMipmapCount(sint *count, uint target, sint width, sint height, sint depth)
{
	sint counter=0;		//Mipmap counter
	if(target==GL_TEXTURE_1D)
	{
		if(width<=0)
		{
			*count=counter;
			return 1;
		}
		counter++;
		while(width>1)
		{
			width>>=1;
			counter++;
		}
		*count=counter;
		return 1;
	}
	else if((target==GL_TEXTURE_2D)||(target==GL_TEXTURE_CUBE_MAP))
	{
		if((width<=0)||(height<=0))
		{
			*count=counter;
			return 1;
		}
		counter++;
		while((width>1)||(height>1))
		{
			if(width>1)
				width>>=1;
			if(height>1)
				height>>=1;
			counter++;
		}
		*count=counter;
		return 1;
	}
	else if(target==GL_TEXTURE_3D)
	{
		if((width<=0)||(height<=0)||(depth<=0))
		{
			*count=counter;
			return 1;
		}
		counter++;
		while((width>1)||(height>1)||(depth>1))
		{
			if(width>1)
				width>>=1;
			if(height>1)
				height>>=1;
			if(depth>1)
				depth>>=1;
			counter++;
		}
		*count=counter;
		return 1;
	}
	else
		return -1;
}


sint glhMemoryUsageWithMipmaps(sint *total, uint target, sint components, sint dataAlignment, sint width, sint height, sint depth, sint lowRange, sint highRange)
{
	sint counter=0, total2=0;
	sint isDXTCompressionOn=FALSE, isDXT1;
	sint com_count;		//How many bytes?


	if(components==GL_ALPHA4)
		com_count=1;
	else if(components==GL_ALPHA8)
		com_count=1;
	else if(components==GL_ALPHA12)
		com_count=2;
	else if(components==GL_ALPHA16)
		com_count=2;
	else if(components==GL_LUMINANCE4)
		com_count=1;
	else if(components==GL_LUMINANCE8)
		com_count=1;
	else if(components==GL_LUMINANCE12)
		com_count=2;
	else if(components==GL_LUMINANCE16)
		com_count=2;
	else if(components==GL_LUMINANCE4_ALPHA4)
		com_count=1;
	else if(components==GL_LUMINANCE6_ALPHA2)
		com_count=1;
	else if(components==GL_LUMINANCE8_ALPHA8)
		com_count=2;
	else if(components==GL_LUMINANCE12_ALPHA4)
		com_count=2;
	else if(components==GL_LUMINANCE12_ALPHA12)
		com_count=3;
	else if(components==GL_LUMINANCE16_ALPHA16)
		com_count=4;
	else if(components==GL_INTENSITY4)
		com_count=1;
	else if(components==GL_INTENSITY8)
		com_count=1;
	else if(components==GL_INTENSITY12)
		com_count=2;
	else if(components==GL_INTENSITY16)
		com_count=2;
	else if(components==GL_R3_G3_B2)
		com_count=1;
	else if(components==GL_RGB4)
		com_count=2;
	else if(components==GL_RGB5)
		com_count=2;
	else if(components==GL_RGB8)
		com_count=3;
	else if(components==GL_RGB10)
		com_count=4;
	else if(components==GL_RGB12)
		com_count=5;
	else if(components==GL_RGB16)
		com_count=6;
	else if(components==GL_RGBA2)
		com_count=1;
	else if(components==GL_RGBA4)
		com_count=2;
	else if(components==GL_RGB5_A1)
		com_count=2;
	else if(components==GL_RGBA8)
		com_count=4;
	else if(components==GL_RGB10_A2)
		com_count=4;
	else if(components==GL_RGBA12)
		com_count=5;
	else if(components==GL_RGBA16)
		com_count=8;
	else if(components==GL_DEPTH_COMPONENT16)
		com_count=2;
	else if(components==GL_DEPTH_COMPONENT24)
		com_count=3;
	else if(components==GL_DEPTH_COMPONENT32)
		com_count=4;
	else if(components==GL_COMPRESSED_RGB_S3TC_DXT1_EXT)
	{
		com_count=0;
		isDXTCompressionOn=TRUE;
		isDXT1=TRUE;
	}
	else if(components==GL_COMPRESSED_RGBA_S3TC_DXT1_EXT)
	{
		com_count=0;
		isDXTCompressionOn=TRUE;
		isDXT1=TRUE;
	}
	//Other formats like DXT2 doesn't exist
	else if(components==GL_COMPRESSED_RGBA_S3TC_DXT3_EXT)
	{
		com_count=0;
		isDXTCompressionOn=TRUE;
		isDXT1=FALSE;
	}
	//Other formats like DXT4 doesn't exist
	else if(components==GL_COMPRESSED_RGBA_S3TC_DXT5_EXT)
	{
		com_count=0;
		isDXTCompressionOn=TRUE;
		isDXT1=FALSE;
	}
	else if(components==GL_RGBA32F_ARB)
		com_count=16;
	else if(components==GL_RGB32F_ARB)
		com_count=12;
	else if(components==GL_ALPHA32F_ARB)
		com_count=4;
	else if(components==GL_INTENSITY32F_ARB)
		com_count=4;
	else if(components==GL_LUMINANCE32F_ARB)
		com_count=4;
	else if(components==GL_LUMINANCE_ALPHA32F_ARB)
		com_count=8;
	else if(components==GL_RGBA16F_ARB)
		com_count=8;
	else if(components==GL_RGB16F_ARB)
		com_count=6;
	else if(components==GL_ALPHA16F_ARB)
		com_count=2;
	else if(components==GL_INTENSITY16F_ARB)
		com_count=2;
	else if(components==GL_LUMINANCE16F_ARB)
		com_count=2;
	else if(components==GL_LUMINANCE_ALPHA16F_ARB)
		com_count=4;
	else
		return -2;

	if(target==GL_TEXTURE_1D)
	{
		if(width<=0)
		{
			return 1;
		}
		if(isDXTCompressionOn)	//Not possible with 1D textures I think
		{
			*total=0;
			return 1;
		}
		while(1)
		{
			if((counter>=lowRange)&&(counter<=highRange))
			{
				total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment;
			}
			width>>=1;
			if(width==0)
				break;
			counter++;
		}
		*total=total2;
		return 1;
	}
	else if(target==GL_TEXTURE_2D)
	{
		if((width<=0)||(height<=0))
		{
			return 1;
		}
		if(isDXTCompressionOn==GL_FALSE)
		{
			while(1)
			{
				if((width!=1)||(height!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height;
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height;
					}
					break;	//Both are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				counter++;
			}
			*total=total2;
		}
		else //if(isDXTCompressionOn==GL_TRUE)
		{
			if((width % 4!=0)||(height % 4!=0))
			{
				*total=0;
				return 1;
			}
			while(1)
			{
				if((width!=1)||(height!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8;
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16;
						}
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8;
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16;
						}
					}
					break;	//Both are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				counter++;
			}
			*total=total2;
		}
		return 1;
	}
	else if(target==GL_TEXTURE_3D)
	{
		if((width<=0)||(height<=0)||(depth<=0))
		{
			return 1;
		}
		if(isDXTCompressionOn==GL_FALSE)
		{
			while(1)
			{
				if((width!=1)||(height!=1)||(depth!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height*depth;
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height*depth;
					}
					break;	//The three are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				if(depth>1)
					depth>>=1;
				counter++;
			}
			*total=total2;
		}
		else //if(isDXTCompressionOn==GL_TRUE)
		{
			if((width % 4!=0)||(height % 4!=0))	//Depth need not be div by 4. Each slice is treated as a 2D texture.
			{
				*total=0;
				return 1;
			}
			while(1)
			{
				if((width!=1)||(height!=1)||(depth!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8*depth;
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16*depth;
						}
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8*depth;
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16*depth;
						}
					}
					break;	//Both are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				if(depth>1)
					depth>>=1;
				counter++;
			}
			*total=total2;
		}
		return 1;
	}
	else if(target==GL_TEXTURE_CUBE_MAP)
	{
		if((width<=0)||(height<=0))
		{
			return 1;
		}
		if(isDXTCompressionOn==GL_FALSE)
		{
			while(1)
			{
				if((width!=1)||(height!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height*6;
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						total2+=((width*com_count+dataAlignment-1)/dataAlignment)*dataAlignment*height*6;
					}
					break;	//Both are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				counter++;
			}
			*total=total2;
		}
		else //if(isDXTCompressionOn==GL_TRUE)
		{
			if((width % 4!=0)||(height % 4!=0))
			{
				*total=0;
				return 1;
			}
			while(1)
			{
				if((width!=1)||(height!=1))
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8*6;	//*6 for the 6 faces
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16*6;	//*6 for the 6 faces
						}
					}
				}
				else
				{
					if((counter>=lowRange)&&(counter<=highRange))
					{
						if(isDXT1)
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*8*6;	//*6 for the 6 faces
						}
						else
						{
							total2+=((1<width/4) ? width/4 : 1)*
								((1<height/4) ? height/4 : 1)*16*6;	//*6 for the 6 faces
						}
					}
					break;	//Both are 1 now
				}
				if(width>1)
					width>>=1;
				if(height>1)
					height>>=1;
				counter++;
			}
			*total=total2;
		}
		return 1;
	}
	else
		return -1;
}
