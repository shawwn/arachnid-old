// glhlib7.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
//#include <GL/gl.h>
//#include <GL/glext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include <stdlib.h>
#include "3DGraphicsLibrarySmall.h"
#include "TCube.h"
#include "TCylinder.h"
#include "TSphere.h"
#include <stack>




#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'



using namespace std;

//This page (glhlib7.cpp) is about
//DXT compression and decompression.



//PURPOSE:	Called from Convert_A2B10G10R10_To_BGRA8
void Convert_A2B10G10R10_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData)
{
	sint i, size=width*height*4;
	//2^10 -1 = 1023 and 2^2 -1 = 3
		uint red, green, blue, alpha;
		uint *p;
		for(i=0; i<size; i+=4)
		{
			p=(uint *)&pcolorData[i];
			blue=(0x000003FF & *p);
			green=((0x000FFC00 & *p) >> 10);
			red=((0x3FF00000 & *p) >> 20);
			alpha=((0xC0000000 & *p) >> 30);
			
			red=(uint)(((sreal)red/1023.0)*255.0);
			if(red>255)
				red=255;
			green=(uint)(((sreal)green/1023.0)*255.0);
			if(green>255)
				green=255;
			blue=(uint)(((sreal)blue/1023.0)*255.0);
			if(blue>255)
				blue=255;
			alpha=(uint)(((sreal)alpha/3.0)*255.0);
			if(alpha>255)
				alpha=255;

			//Write as BGRA8
			colorData[i  ]=blue;
			colorData[i+1]=green;
			colorData[i+2]=red;
			colorData[i+3]=alpha;
		}
}


//PURPOSE:	Called from ReadFileDDS when format is A2R10G10B10
//			BGRA8 means A8R8G8B8
//			pcolorData contains the data
//			colorData will be allocated some memory and will receive the data
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
sint Convert_A2B10G10R10_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		//BGRA8 format, dataAlignment=4
		sint size=width*height*4;
		if((colorData=new uchar[size])==NULL)
			return 0;

		Convert_A2B10G10R10_To_BGRA8_2D(width, height, pcolorData, colorData);
		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint i;
		//BGRA8 format, dataAlignment=4
		sint size=width*height*4;		//Six faces
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		for(i=0; i<6; i++)
		{
			Convert_A2B10G10R10_To_BGRA8_2D(width, height, &pcolorData[i*size], &colorData[i*size]);
		}
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint i;
		//BGRA8 format, dataAlignment=4
		sint size=width*height*4;
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		for(i=0; i<depth; i++)
		{
			Convert_A2B10G10R10_To_BGRA8_2D(width, height, &pcolorData[i*size], &colorData[i*size]);
		}
	}*/
	return -1;
}

//PURPOSE:	Called from Convert_A16B16G16R16_To_BGRA8
void Convert_A16B16G16R16_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData)
{
	sint i, j, size=width*height*4;

	//2^16 -1 = 65535
		uint red, green, blue, alpha;
		uint *p;
		size*=2;		//Because of twice the size
		for(i=j=0; i<size; i+=8, j+=4)
		{
			p=(uint *)&pcolorData[i];
			red=(0x0000FFFF & p[0]);
			green=((0xFFFF0000 & p[0]) >> 16);
			blue=(0x0000FFFF & p[1]);
			alpha=((0xFFFF0000 & p[1]) >> 16);
			
			red=(uint)(((sreal)red/65535.0)*255.0);
			if(red>255)
				red=255;
			green=(uint)(((sreal)green/65535.0)*255.0);
			if(green>255)
				green=255;
			blue=(uint)(((sreal)blue/65535.0)*255.0);
			if(blue>255)
				blue=255;
			alpha=(uint)(((sreal)alpha/65535.0)*255.0);
			if(alpha>255)
				alpha=255;

			//Write as BGRA8
			colorData[j  ]=blue;
			colorData[j+1]=green;
			colorData[j+2]=red;
			colorData[j+3]=alpha;
		}
}

//PURPOSE:	Called from ReadFileDDS when format is A16R16G16B16
//			BGRA8 means A8R8G8B8
//			pcolorData contains the data
//			colorData will be allocated some memory and will receive the data
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
sint Convert_A16B16G16R16_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		//BGRA8 format, dataAlignment=4
		sint size=width*height*4;
		if((colorData=new uchar[size])==NULL)
			return 0;

		Convert_A16B16G16R16_To_BGRA8_2D(width, height, pcolorData, colorData);
		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint i;
		sint size=width*height*4;
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint size2=width*height*8;		//*8 because it is 64 bit (RGBA16)
		for(i=0; i<6; i++)
		{
			Convert_A16B16G16R16_To_BGRA8_2D(width, height, &pcolorData[i*size2], &colorData[i*size]);
		}
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint i;
		sint size=width*height*4;
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint size2=width*height*8;		//*8 because it is 64 bit (RGBA16)
		for(i=0; i<depth; i++)
		{
			Convert_A16B16G16R16_To_BGRA8_2D(width, height, &pcolorData[i*size2], &colorData[i*size]);
		}
	}*/

	return -1;
}

//PURPOSE:	Called from Convert_A1R5G5B5_To_BGRA8
void Convert_A1R5G5B5_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData)
{
	sint i, j, size=width*height*4;
	//2^5 = 31
		uint red, green, blue, alpha;
		uint *p;
		size/=2;		//Because of half the size
		for(i=j=0; i<size; i+=2, j+=4)
		{
			p=(uint *)&pcolorData[i];
			red=((0x00007C00 & p[0]) >> 10);
			green=((0x000003E0 & p[0]) >> 5);
			blue=(0x0000001F & p[0]);
			alpha=((0x00008000 & p[0]) >> 15);
			
			red=(uint)(((sreal)red/31.0)*255.0);
			if(red>255)
				red=255;
			green=(uint)(((sreal)green/31.0)*255.0);
			if(green>255)
				green=255;
			blue=(uint)(((sreal)blue/31.0)*255.0);
			if(blue>255)
				blue=255;
			if(alpha!=0)
				alpha=255;
			//else it is 0

			//Write as BGRA8
			colorData[j  ]=blue;
			colorData[j+1]=green;
			colorData[j+2]=red;
			colorData[j+3]=alpha;
		}
}

//PURPOSE:	Called from ReadFileDDS when format is A1R5G5B5
//			BGRA8 means A8R8G8B8
//			pcolorData contains the data
//			colorData will be allocated some memory and will receive the data
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
sint Convert_A1R5G5B5_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		//BGRA8 format, dataAlignment=4
		sint size=width*height*4;
		if((colorData=new uchar[size])==NULL)
			return 0;
		
		Convert_A1R5G5B5_To_BGRA8_2D(width, height, pcolorData, colorData);
		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint i;
		sint size=width*height*4;
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint size2=width*height*2;		//*2 because 16 bit
		for(i=0; i<6; i++)
		{
			Convert_A1R5G5B5_To_BGRA8_2D(width, height, &pcolorData[i*size2], &colorData[i*size]);
		}
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint i;
		sint size=width*height*4;
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint size2=width*height*2;		//*2 because 16 bit
		for(i=0; i<depth; i++)
		{
			Convert_A1R5G5B5_To_BGRA8_2D(width, height, &pcolorData[i*size2], &colorData[i*size]);
		}
	}*/

	return -1;
}

inline void ExtractRGBFromR5G6B5(ushort color, uint &red, uint &green, uint &blue)
{
	red=((color & 0xF800)>>11);
	green=((color & 0x07E0)>>5);
	blue=(color & 0x001F);
	//Shift to full precision - I think by scaling it would be better, but this is suppose to be fast code
	red<<=3;
	green<<=2;
	blue<<=3;
}

//PURPOSE:	This code comes from Image_DXTC.cpp from the nvidia/developer.
//			Some game company wrote it. I made some minor adjustments.
inline void GetColorBlockColors(DXTColBlock *pBlock, uint &color_0, uint &color_1,
													 uint &color_2, uint &color_3,
													 ushort &wrd, bool isDXT1)
{
	ushort *pCol;		//R5G6B5 format

	//No alpha in R5G6B5, write 0xFF
	pCol=&(pBlock->Color0);
	uint red, green, blue;
	ExtractRGBFromR5G6B5(*pCol, red, green, blue);
	color_0=(0xFF000000) | (red<<16) | (green<<8) | (blue);
	
	//No alpha in R5G6B5, write 0xFF
	pCol=&(pBlock->Color1);
	ExtractRGBFromR5G6B5(*pCol, red, green, blue);
	color_1=(0xFF000000) | (red<<16) | (green<<8) | (blue);

	if((pBlock->Color0>pBlock->Color1)||(isDXT1==false))
	{
		//Four-color block: derive the other two colors.    
		//00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
		//These two bit codes correspond to the 2-bit fields 
		//stored in the 64-bit block.
		wrd=((ushort)((color_0 & 0x00FF0000)>>16)*2+(ushort)((color_1 & 0x00FF0000)>>16))/3;	//Red
											//no +1 for rounding
											//as bits have been shifted to 888
		color_2=((((uint)wrd)<<16) & 0x00FF0000);			//Red

		wrd=((ushort)((color_0 & 0x0000FF00)>>8)*2+(ushort)((color_1 & 0x0000FF00)>>8))/3;	//Green
		color_2=color_2 | ((((uint)wrd)<<8) & 0x0000FF00);	//Green

		wrd=((ushort)(color_0 & 0x000000FF)*2+(ushort)(color_1 & 0x000000FF))/3;	//Blue
		color_2=color_2 | ((((uint)wrd)) & 0x000000FF);	//Blue
		color_2=color_2 | (0xFF000000);			//Alpha

		wrd=((ushort)((color_0 & 0x00FF0000)>>16)+(ushort)((color_1 & 0x00FF0000)>>16)*2)/3;		//Red
		color_3=((((uint)wrd)<<16) & 0x00FF0000);			//Red

		wrd=((ushort)((color_0 & 0x0000FF00)>>8) + (ushort)((color_1 & 0x0000FF00)>>8)*2)/3;
		color_3=color_3 | ((((uint)wrd)<<8) & 0x0000FF00);	//Green

		wrd=((ushort)(color_0 & 0x000000FF)+(ushort)(color_1 & 0x000000FF)*2)/3;	//Blue
		color_3=color_3 | (((uint)wrd) & 0x000000FF);	//Blue
		color_3=color_3 | (0xFF000000);			//Alpha
	}
	else
	{
		//Three-color block: derive the other color.
		//00 = color_0,  01 = color_1,  10 = color_2,  
		//11 = transparent.
		//These two bit codes correspond to the 2-bit fields 
		//stored in the 64-bit block. 

		//Explicit for each component, unlike some refrasts...  (MYCOMMENT : what is refrasts?)
		
		//TRACE("block has alpha\n");
		wrd=((ushort)((color_0 & 0x00FF0000)>>16) + (ushort)((color_1 & 0x00FF0000)>>16))/2;
		color_2=((((uint)wrd)<<16) & 0x00FF0000);		//Red

		wrd=((ushort)((color_0 & 0x0000FF00)>>8) + (ushort)((color_1 & 0x0000FF00)>>8))/2;
		color_2=color_2 | ((((uint)wrd)<<8) & 0x0000FF00);	//Green

		wrd=((ushort)(color_0 & 0x000000FF) + (ushort)(color_1 & 0x000000FF))/2;
		color_2=color_2 | (((uint)wrd) & 0x000000FF);	//Blue
		color_2=color_2 | (0xFF000000);		//Alpha

		//Random color to indicate transparency (green for now with alpha=0)
		color_3=0x0000FF00;
	}
}

//PURPOSE:	This code comes from Image_DXTC.cpp from the nvidia/developer.
//			Some game company wrote it. I made some minor adjustments.
inline void DecodeColorBlock(uint *pImPos, DXTColBlock *pColorBlock, sint width,
								uint color_0, uint color_1, uint color_2, uint color_3)
{
	//Width is width of image in pixels
	uint bits;
	sint r, n;

	//Bit masks = 00000011, 00001100, 00110000, 11000000
	const uint masks[]={0x00000003, 0x0000000C, 0x00000030, 0x000000C0};
	const sint shift[]={0, 2, 4, 6};

	//r steps through lines in y
	for(r=0; r<4; r++, pImPos+=width-4)	//No width*4 as DWORD ptr inc will *4
	{
		//width * 4 bytes per pixel per line
		//each j dxtc row is 4 lines of pixels

		//pImPos=(uint *)((uint)pBase + i*16 + (r+j*4) * Width * 4);

		//n steps through pixels
		for(n=0; n<4; n++)
		{
			bits=pColorBlock->Row[r] & masks[n];
			bits>>=shift[n];

			switch(bits)
			{
			case 0:
				*pImPos=color_0;
				pImPos++;		//Increment to next DWORD
				break;
			case 1:
				*pImPos=color_1;
				pImPos++;		//Increment to next DWORD
				break;
			case 2:
				*pImPos=color_2;
				pImPos++;		//Increment to next DWORD
				break;
			case 3:
				*pImPos=color_3;
				pImPos++;		//Increment to next DWORD
				break;
			}
		}
	}
}

//PURPOSE:	This code comes from Image_DXTC.cpp from the nvidia/developer.
//			Some game company wrote it. I made some minor adjustments.
inline void DecodeAlphaExplicit(uint *pImPos, DXTAlphaBlockExplicit *pAlphaBlock, sint width, uint alphazero)
{
	//alphazero is a bit mask that when & with the image color
	//will zero the alpha bits, so if the image DWORDs  are
	//ARGB then alphazero will be 0x00ffffff or if
	//RGBA then alphazero will be 0xffffff00
	//alphazero constructed automaticaly from field order of Color8888 structure

	//decodes to 32 bit format only
	
	sint row, pix;

	uint wrd;

	uint col;
	col=0;

	for(row=0; row<4; row++, pImPos+=width-4)
	{
		//pImPow+=pImPos+=width-4 moves to next row down
		wrd=pAlphaBlock->row[row];

		for(pix=0; pix<4; pix++)
		{
			//zero the alpha bits of image pixel
			*pImPos &= alphazero;

			col=0;
			col=col | (((uint)(wrd & 0x000f))<<24);//get only low 4 bits
			//col.a <<= 4;				//shift to full byte precision
										//NOTE:  with just a << 4 you'll never have alpha
										//of 0xff,  0xf0 is max so pure shift doesn't quite
										//cover full alpha range.
										//It's much cheaper than divide & scale though.
										//To correct for this, and get 0xff for max alpha,
										//or the low bits back in after left shifting
			//Already done
			//col.a = col.a | (col.a << 4 );	//This allows max 4 bit alpha to be 0xff alpha
												//in final image, and is crude approach to full 
												//range scale
			col=col | ((col & 0xFF000000)<<4);

			*pImPos |= col;						//or the bits into the prev. nulled alpha

			wrd>>=4;		//Move next bits to lowest 4
			pImPos++;		//Move to next pixel in the row
		}
	}
}

//PURPOSE:	Called from ConvertDXT1_To_BGRA8
void ConvertDXT1_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData)
{
	{
		sint i, j, xblocks, yblocks;

		xblocks=width/4;
		yblocks=height/4;

		//uint *pBase=(uint *)colorData;
		uint *pImPos=(uint *)colorData;			//pos in decompressed data

		DXTColBlock *pBlock;
		uint color_0, color_1, color_2, color_3;		//Will write BGRA info here (0xAARRGGBB)
		ushort wrd;

		for(j=0; j<yblocks; j++)
		{
			//8 bytes per block = 8*8 = 64 bits
			//pBlock=(DXTColBlock *)((uint)pcolorData+j*xblocks*8);
			pBlock=(DXTColBlock *)&pcolorData[j*xblocks*8];

			for(i=0; i<xblocks; i++, pBlock++)
			{
				//Inline func:
				GetColorBlockColors(pBlock, color_0, color_1, color_2, color_3, wrd, true);

				//Now decode the color block into the bitmap bits
				//inline func:
				//pImPos=(uint *)((uint)pBase+i*16+(j*4)*width*4);
				pImPos=(uint *)&colorData[(i*4+(j*4)*width)*4];

				DecodeColorBlock(pImPos, pBlock, width, color_0, color_1, color_2, color_3);

				//Set to RGB test pattern
				//pImPos = (DWORD*)((DWORD)pBase + i*4 + j*m_nWidth*4);
				//*pImPos = ((i*4) << 16) | ((j*4) << 8 ) | ( (63-i)*4 );

				//Checkerboard of only col_0 and col_1 basis colors:
				//pImPos = (DWORD*)((DWORD)pBase + i*8 + j*m_nWidth*8);
				//*pImPos = *((DWORD*)&col_0);
				//pImPos += 1 + m_nWidth;
				//*pImPos = *((DWORD*)&col_1);
			}
		}
	}
}
//PURPOSE:	Called from ReadFileDDS when image is DXT1 compressed.
//			This will use info in pcolorData and write value to colorData
//			with BGRA8 pixels. DataAlignent will be 4.
//			and it is expected that pColorData already is DWORD aligned.
//			colorData will receive a pointer to an array filled with BGRA8
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
//NOTE: I hope the following code is correct. The DirectX9 document suugests how to decompress
//		DXT1
sint ConvertDXT1_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size])==NULL)
			return 0;

		ConvertDXT1_To_BGRA8_2D(width, height, pcolorData, colorData);

		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*8;
		for(i=0; i<6; i++)
		{
			ConvertDXT1_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i]);
		}

		return 1;
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*8;
		for(i=0; i<depth; i++)
		{
			ConvertDXT1_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i]);
		}

		return 1;
	}*/

	return -1;
}

//PURPOSE:	Called from ConvertDXT2_DXT3_To_BGRA8
void ConvertDXT2_DXT3_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData, bool isDXT2OrDXT3)
{
	sint i, j, xblocks, yblocks;
	sint row, temp;

		xblocks=width/4;
		yblocks=height/4;

		uint *pBase=(uint *)colorData;
		uint *pImPos=(uint *)colorData;			//pos in decompressed data

		DXTColBlock *pBlock;
		DXTAlphaBlockExplicit *pAlphaBlock;
		uint color_0, color_1, color_2, color_3;		//Will write BGRA info here (0xAARRGGBB)
		ushort wrd;

		//Fill alphazero with appropriate value to zero out alpha when
		//alphazero is ANDed with the image color 32 bit DWORD:
		color_0=0x00FFFFFF;
		uint alphazero=color_0;

		if(isDXT2OrDXT3)
		{
			//DXT2 looks identical to the DXT3 file, the alpha values in the alpha
			//blocks are exactly the same, but the colors in the color blocks are premutliplied
			//by the alpha.
			for(j=0; j<yblocks; j++)
			{
				//8 bytes per block = 64 bit
				//1 block for alpha, 1 block for color
				pBlock=(DXTColBlock *)&pcolorData[j*xblocks*16];

				for(i=0; i<xblocks; i++, pBlock++)
				{
					//Get alpha block (first comes the alpha block, 64 bit)
					pAlphaBlock=(DXTAlphaBlockExplicit *)pBlock;

					//Get color block & colors (second comes the color block, 64 bit)
					pBlock++;
					GetColorBlockColors(pBlock, color_0, color_1, color_2, color_3, wrd, false);

					//Decode the color block into the bitmap bits
					//pImPos=(uint *)((uint)pBase+i*16+(j*4)*width*4);
					temp=(i*4+(j*4)*width)*4;
					pImPos=(uint *)&colorData[temp];
					
					DecodeColorBlock(pImPos, pBlock, width, color_0, color_1, color_2, color_3);

					//Overwrite the previous alpha bits with the alpha block info
					DecodeAlphaExplicit(pImPos, pAlphaBlock, width, alphazero);

					//For the 4x4 block of pixels, take BGR and divide each by alpha
					for(row=0; row<4; row++)
					{
						//Avoid using floats, so we mult by 1000000, then divided by 1000000
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=width*4-12;
					}
				}
			}
		}
		else
		{
			for(j=0; j<yblocks; j++)
			{
				//8 bytes per block = 64 bit
				//1 block for alpha, 1 block for color
				pBlock=(DXTColBlock *)&pcolorData[j*xblocks*16];

				for(i=0; i<xblocks; i++, pBlock++)
				{
					//Get alpha block (first comes the alpha block, 64 bit)
					pAlphaBlock=(DXTAlphaBlockExplicit *)pBlock;

					//Get color block & colors (second comes the color block, 64 bit)
					pBlock++;
					GetColorBlockColors(pBlock, color_0, color_1, color_2, color_3, wrd, false);

					//Decode the color block into the bitmap bits
					pImPos=(uint *)&colorData[(i*4+(j*4)*width)*4];

					DecodeColorBlock(pImPos, pBlock, width, color_0, color_1, color_2, color_3);

					//Overwrite the previous alpha bits with the alpha block info
					DecodeAlphaExplicit(pImPos, pAlphaBlock, width, alphazero);
				}
			}
		}

}

//PURPOSE:	Called from ReadFileDDS when image is DXT2 or DXT3 compressed.
//			Since DXT2 and DXT3 are so related, I have wrapped them up in a single function.
//			isDXT2OrDXT3 == true for DXT2 or isDXT2OrDXT3 == false for DXT3
//			This will use info in pcolorData and write value to colorData
//			with BGRA8 pixels. DataAlignent will be 4.
//			and it is expected that pcolorData already is DWORD aligned.
//			colorData will receive a pointer to an array filled with BGRA8
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
//NOTE: I hope the following code is correct. The DirectX9 document suugests how to decompress
//		DXT2, DXT3
sint ConvertDXT2_DXT3_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData, bool isDXT2OrDXT3)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size])==NULL)
			return 0;

		ConvertDXT2_DXT3_To_BGRA8_2D(width, height, pcolorData, colorData, isDXT2OrDXT3);

		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*16;
		for(i=0; i<6; i++)
		{
			ConvertDXT2_DXT3_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i], isDXT2OrDXT3);
		}

		return 1;
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*16;
		for(i=0; i<depth; i++)
		{
			ConvertDXT2_DXT3_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i], isDXT2OrDXT3);
		}

		return 1;
	}*/

	return -1;
}

//PURPOSE:	Called from ConvertDXT4_DXT5_To_BGRA8_2D
void DecodeAlpha3BitLinear(uint *pImPos, DXTAlphaBlock3BitLinear *pAlphaBlock, sint width, uint alphazero)
{
	ushort gAlphas[8];
	uchar gBits[4][4];
	uint	gACol[4][4];

	gAlphas[0]=pAlphaBlock->alpha0;		//Copy 8 bit to 16 bit
	gAlphas[1]=pAlphaBlock->alpha1;		//Copy 8 bit to 16 bit
	
	//8-alpha or 6-alpha block?    
	if(gAlphas[0]>gAlphas[1])
	{
		//8-alpha block:  derive the other 6 alphas.    
		//000 = alpha_0, 001 = alpha_1, others are interpolated
		gAlphas[2]=(6*gAlphas[0]+  gAlphas[1])/7;	//Bit code 010
		gAlphas[3]=(5*gAlphas[0]+2*gAlphas[1])/7;	//Bit code 011    
		gAlphas[4]=(4*gAlphas[0]+3*gAlphas[1])/7;	//Bit code 100    
		gAlphas[5]=(3*gAlphas[0]+4*gAlphas[1])/7;	//Bit code 101
		gAlphas[6]=(2*gAlphas[0]+5*gAlphas[1])/7;	//Bit code 110    
		gAlphas[7]=(  gAlphas[0]+6*gAlphas[1])/7;	//Bit code 111
	}    
	else
	{
		//6-alpha block:  derive the other alphas.    
		//000 = alpha_0, 001 = alpha_1, others are interpolated
		gAlphas[2]=(4*gAlphas[0]+  gAlphas[1])/5;	//Bit code 010
		gAlphas[3]=(3*gAlphas[0]+2*gAlphas[1])/5;	//Bit code 011    
		gAlphas[4]=(2*gAlphas[0]+3*gAlphas[1])/5;	//Bit code 100    
		gAlphas[5]=(  gAlphas[0]+4*gAlphas[1])/5;	//Bit code 101
		gAlphas[6]=0;								//Bit code 110
		gAlphas[7]=255;								//Bit code 111
	}


	//Decode 3-bit fields into array of 16 BYTES with same value
	//NOTE:	This pasrt is a bit nasty. It involves getting bit feild and
	//a lot of bit shifting.

	//First two rows of 4 pixels each:
	//pRows = (Alpha3BitRows*) & (pAlphaBlock->stuff[0]);
	const uint mask=0x00000007;		//bits = 00 00 01 11

	uint bits=*((uint *) &(pAlphaBlock->stuff[0]));

	gBits[0][0]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[0][1]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[0][2]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[0][3]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[1][0]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[1][1]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[1][2]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[1][3]=(BYTE)(bits & mask);

	//Now for last two rows:

	bits=*((uint *) &(pAlphaBlock->stuff[3]));		//Last 3 bytes

	gBits[2][0]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[2][1]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[2][2]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[2][3]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[3][0]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[3][1]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[3][2]=(BYTE)(bits & mask);
	bits>>=3;
	gBits[3][3]=(BYTE)(bits & mask);


	//Decode the codes into alpha values
	sint row, pix;


	for(row=0; row<4; row++)
	{
		for(pix=0; pix<4; pix++)
		{
			//Write to alpha component
			//BGR should be 0
			gACol[row][pix]=((uint)(0x00FF & gAlphas[gBits[row][pix]]) << 24);

			//MY COMMENT
			//I commented this out. Now sure what it's for
			//ASSERT( gACol[row][pix].r == 0 );
			//ASSERT( gACol[row][pix].g == 0 );
			//ASSERT( gACol[row][pix].b == 0 );
		}
	}



	//Write out alpha values to the image bits
	for(row=0; row<4; row++, pImPos+=width-4)
	{
		//pImPow += pImPos += width-4 moves to next row down
		for(pix=0; pix<4; pix++)
		{
			//Zero the alpha bits of image pixel
			*pImPos &= alphazero;

			//BGR in gACol[row][pix] should be 0
			*pImPos |=  gACol[row][pix];	//Or the bits into the prev. nulled alpha
			pImPos++;
		}
	}
}

//PURPOSE:	Called from ConvertDXT4_DXT5_To_BGRA8
//			DXT4 and 5 are very similar to DXT2 and 3 so code looks similar.
void ConvertDXT4_DXT5_To_BGRA8_2D(sint width, sint height, uchar *pcolorData, uchar *colorData, bool isDXT4OrDXT5)
{
	sint i, j, xblocks, yblocks;
	sint row, temp;

		xblocks=width/4;
		yblocks=height/4;

		uint *pBase=(uint *)colorData;
		uint *pImPos=(uint *)colorData;			//pos in decompressed data

		DXTColBlock *pBlock;
		DXTAlphaBlock3BitLinear *pAlphaBlock;
		uint color_0, color_1, color_2, color_3;		//Will write BGRA info here (0xAARRGGBB)
		ushort wrd;

		//Fill alphazero with appropriate value to zero out alpha when
		//alphazero is ANDed with the image color 32 bit DWORD:
		color_0=0x00FFFFFF;
		uint alphazero=color_0;

		if(isDXT4OrDXT5)
		{
			//DXT4 looks identical to the DXT5 file, the alpha values in the alpha
			//blocks are exactly the same, but the colors in the color blocks are premutliplied
			//by the alpha.
			for(j=0; j<yblocks; j++)
			{
				//8 bytes per block = 64 bit
				//1 block for alpha, 1 block for color
				pBlock=(DXTColBlock *)&pcolorData[j*xblocks*16];

				for(i=0; i<xblocks; i++, pBlock++)
				{
					//Get alpha block (first comes the alpha block, 64 bit)
					pAlphaBlock=(DXTAlphaBlock3BitLinear *)pBlock;

					//Get color block & colors (second comes the color block, 64 bit)
					pBlock++;
					GetColorBlockColors(pBlock, color_0, color_1, color_2, color_3, wrd, false);

					//Decode the color block into the bitmap bits
					//pImPos=(uint *)((uint)pBase+i*16+(j*4)*width*4);
					temp=(i*4+(j*4)*width)*4;
					pImPos=(uint *)&colorData[temp];
					
					DecodeColorBlock(pImPos, pBlock, width, color_0, color_1, color_2, color_3);

					//Overwrite the previous alpha bits with the alpha block info
					DecodeAlpha3BitLinear(pImPos, pAlphaBlock, width, alphazero);

					//For the 4x4 block of pixels, take BGR and divide each by alpha
					for(row=0; row<4; row++)
					{
						//Avoid using floats, so we mult by 1000000, then divided by 1000000
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=4;
						if(colorData[temp+3])
						{
							colorData[temp  ]=(uchar)((((((uint)colorData[temp  ])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+1]=(uchar)((((((uint)colorData[temp+1])*1000000)/(uint)colorData[temp+3])*255)/1000000);
							colorData[temp+2]=(uchar)((((((uint)colorData[temp+2])*1000000)/(uint)colorData[temp+3])*255)/1000000);
						}
						temp+=width*4-12;
					}
				}
			}
		}
		else
		{
			for(j=0; j<yblocks; j++)
			{
				//8 bytes per block = 64 bit
				//1 block for alpha, 1 block for color
				pBlock=(DXTColBlock *)&pcolorData[j*xblocks*16];

				for(i=0; i<xblocks; i++, pBlock++)
				{
					//Get alpha block (first comes the alpha block, 64 bit)
					pAlphaBlock=(DXTAlphaBlock3BitLinear *)pBlock;

					//Get color block & colors (second comes the color block, 64 bit)
					pBlock++;
					GetColorBlockColors(pBlock, color_0, color_1, color_2, color_3, wrd, false);

					//Decode the color block into the bitmap bits
					pImPos=(uint *)&colorData[(i*4+(j*4)*width)*4];

					DecodeColorBlock(pImPos, pBlock, width, color_0, color_1, color_2, color_3);

					//Overwrite the previous alpha bits with the alpha block info
					DecodeAlpha3BitLinear(pImPos, pAlphaBlock, width, alphazero);
				}
			}
		}
}

//PURPOSE:	Called from ReadFileDDS when image is DXT4 or DXT5 compressed.
//			Since DXT4 and DXT5 are so related, I have wrapped them up in a single function.
//			isDXT4OrDXT5 == true for DXT4 or isDXT4OrDXT5 == false for DXT5
//			This will use info in pcolorData and write value to colorData
//			with BGRA8 pixels. DataAlignent will be 4.
//			and it is expected that pcolorData already is DWORD aligned.
//			colorData will receive a pointer to an array filled with BGRA8
//			Returns 1 for success, -1 if something is wrong, 0 for memory error.
//NOTE: I hope the following code is correct. The DirectX9 document suugests how to decompress
//		DXT4, DXT5. DXT4 and 5 are similar to DXT2 and 3.
sint ConvertDXT4_DXT5_To_BGRA8(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, uchar *&colorData, bool isDXT4OrDXT5)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size])==NULL)
			return 0;

		ConvertDXT4_DXT5_To_BGRA8_2D(width, height, pcolorData, colorData, isDXT4OrDXT5);

		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*16;
		for(i=0; i<6; i++)
		{
			ConvertDXT4_DXT5_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i], isDXT4OrDXT5);
		}

		return 1;
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint size=width*height*4;		//*4 for BGRA8
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint i;
		sint size2=(width/4)*(height/4)*16;
		for(i=0; i<depth; i++)
		{
			ConvertDXT4_DXT5_To_BGRA8_2D(width, height, &pcolorData[size2*i], &colorData[size*i], isDXT4OrDXT5);
		}

		return 1;
	}*/

	return -1;
}

//PURPOSE:	Called from ChooseEndPoints to compute the distance squared
//			between two colors. The BGRA8 color are treated as 3D vectors, where
//			only the RGB part is taken into account.
ushort ColorDistanceSquared(uchar *c1, uchar *c2)
{
	return  (ushort)(c1[0]-c2[0])*(ushort)(c1[0]-c2[0])+
			(ushort)(c1[1]-c2[1])*(ushort)(c1[1]-c2[1])+
			(ushort)(c1[2]-c2[2])*(ushort)(c1[2]-c2[2]);
}

//PURPOSE:	Called from ConvertBGRA8_To_DXT1
//			colorBGRA8_16 is an arrays of 16 BGRA8 values (total = 4*16)
//			The two extremes are written to, where color_0 is lowest and color_1 is the highest.
void ChooseEndPoints(ushort *colorR5G6B5_16, uchar *colorBGRA8_16, ushort &color_0, ushort &color_1)
{
	sint i, j;
	sint farthest=-1, d;

	for(i=0; i<16; i++)
	{
		for(j=i+1; j<16; j++)
		{
			d=(sint)ColorDistanceSquared(&colorBGRA8_16[i*4], &colorBGRA8_16[j*4]);	//*4 because these are BGRA8
			if(d>farthest)
			{
				farthest=d;
				color_0=colorR5G6B5_16[i];
				color_1=colorR5G6B5_16[j];
			}
		}
	}
}

//PURPOSE:	Called from ConvertBGRA8_To_DXT1
//			color_0 and color_1 are the extremety colors
//			numOfColors should be either 3 or 4 (See DX9 documents)
//			color16 is an array of 16 R5G6B5 colors to be compressed.
//			colorBGRA8_16 is an array of 16 BGRA8 colors, we access the alpha
//			to know if pixel is transparent or not.
//			If alpha<128, then transparent, else opaque.
uint GenerateBitMask(ushort color_0, ushort color_1, sint numOfColors, ushort *color16, uchar *colorBGRA8_16)
{
	uint		i, j, closest, dist, bitMask=0;
	uchar		mask[16];
	uchar		c[4], colors[4*4];		//BGRA8 colors. colors are for the 00 01 10 11 bitmasks
	uint red, green, blue;

	ExtractRGBFromR5G6B5(color_0, red, green, blue);
	colors[0]=(uchar)red;
	colors[1]=(uchar)green;
	colors[2]=(uchar)blue;
	//colors[3]=255;		//Dont need
	
	ExtractRGBFromR5G6B5(color_1, red, green, blue);
	colors[4]=(uchar)red;
	colors[5]=(uchar)green;
	colors[6]=(uchar)blue;
	//colors[7]=255;		//Dont need

	if(numOfColors==3)		//There is an alpha
	{
		colors[8]=(colors[0]+colors[4])/2;
		colors[9]=(colors[1]+colors[5])/2;
		colors[10]=(colors[2]+colors[6])/2;
		//colors[11]=255;				//Dont need
		colors[12]=(colors[0]+colors[4])/2;
		colors[13]=(colors[1]+colors[5])/2;
		colors[14]=(colors[2]+colors[6])/2;
		//colors[15]=255;				//Dont need

		//Going over each of the 16 blocks (pixels)
		for(i=j=0; i<16; i++, j+=4)
		{
			if(colorBGRA8_16[j]<128)
			{
				mask[i]=3;				//Transparent
				continue;
			}

			//If no transparency, try to find which colour is the closest.
			closest=-1;
			ExtractRGBFromR5G6B5(color16[i], red, green, blue);
			c[0]=red;
			c[1]=green;
			c[2]=blue;
			
			for(j=0; j<(uint)numOfColors; j++)
			{
				dist=ColorDistanceSquared(c, &colors[j*4]);
				if(dist<closest)
				{
					closest=dist;
					mask[i]=j;
				}
			}
		}
	}
	else //if(numOfColors==4)
	{
		colors[8]=(uchar)(((2*(uint)colors[0])/3+((uint)colors[4])/3));
		colors[9]=(uchar)(((2*(uint)colors[1])/3+((uint)colors[5])/3));
		colors[10]=(uchar)(((2*(uint)colors[2])/3+((uint)colors[6])/3));
		//colors[11]=255;				//Dont need
		colors[12]=(uchar)((((uint)colors[0])/3+(2*(uint)colors[4])/3));
		colors[13]=(uchar)((((uint)colors[1])/3+(2*(uint)colors[5])/3));
		colors[14]=(uchar)((((uint)colors[2])/3+(2*(uint)colors[6])/3));
		//colors[15]=255;				//Dont need

		//Going over each of the 16 blocks (pixels)
		for(i=0; i<16; i++)
		{
			closest=-1;
			ExtractRGBFromR5G6B5(color16[i], red, green, blue);
			c[0]=red;
			c[1]=green;
			c[2]=blue;

			for(j=0; j<(uint)numOfColors; j++)
			{
				dist=ColorDistanceSquared(c, &colors[j*4]);
				if(dist<closest)
				{
					closest=dist;
					mask[i]=j;
				}
			}
		}
	}

	for(i=0; i<16; i++)
	{
		bitMask |= (mask[i]<<(i*2));
	}

	return bitMask;
}

//PURPOSE:	Called from ConvertBGRA8_To_DXT1
void ConvertBGRA8_To_DXT1_2D(sint width, sint height, uchar *pcolorData, uchar *colorData)
{
	{
		sint i, j, k, m, index;
		sint temp;

		//This format only support 1 bit alpha. If alpha in BGRA8 is <128, then it is transparent
		ushort color_0, color_1;		//R5G6B5
		ushort ushorttemp;
		uint interpolatedColors;		//Each block has the 2 bit interpolated color region
		ushort *pcolorData2;
		ushort color16[16];				//Convert a block to R5G6B5 format and store here
		uchar colorBGRA8_16[4*16];				//Like color16, but keep the original BGRA8 format
		sint red, green, blue;
		bool isTransparency;
		index=0;				//Used for pcolorData
		pcolorData2=(ushort *)colorData;
		
		for(j=0; j<height; j+=4)
		{
			for(i=0; i<width; i+=4)
			{
				//We need to process 4x4 block of pixels.
				//Find 2 extremes in this 4x4 block.
				//Convert all 16 to R5G6B5
				isTransparency=false;
				m=0;
				for(k=0; k<4; k++)
				{
					temp=(j*width+i+k)*4;
					colorBGRA8_16[m++]=pcolorData[temp  ];
					colorBGRA8_16[m++]=pcolorData[temp+1];
					colorBGRA8_16[m++]=pcolorData[temp+2];
					colorBGRA8_16[m++]=pcolorData[temp+3];

					red=pcolorData[temp+2];
					red>>=3;
					green=pcolorData[temp+1];
					green>>=2;
					blue=pcolorData[temp  ];
					blue>>=3;
					color16[k]=(red<<11) | (green<<5) | (blue);
					if(pcolorData[temp+3]<255)	//Only <128 is considered transparent
						isTransparency=true;
				}
				//Process line below, notice the (j+1)
				for(k=0; k<4; k++)
				{
					temp=((j+1)*width+i+k)*4;
					colorBGRA8_16[m++]=pcolorData[temp  ];
					colorBGRA8_16[m++]=pcolorData[temp+1];
					colorBGRA8_16[m++]=pcolorData[temp+2];
					colorBGRA8_16[m++]=pcolorData[temp+3];

					red=pcolorData[temp+2];
					red>>=3;
					green=pcolorData[temp+1];
					green>>=2;
					blue=pcolorData[temp  ];
					blue>>=3;
					color16[4+k]=(red<<11) | (green<<5) | (blue);
					if(pcolorData[temp+3]<255)	//Only <128 is considered transparent
						isTransparency=true;
				}
				//Process line below, notice the (j+2)
				for(k=0; k<4; k++)
				{
					temp=((j+2)*width+i+k)*4;
					colorBGRA8_16[m++]=pcolorData[temp  ];
					colorBGRA8_16[m++]=pcolorData[temp+1];
					colorBGRA8_16[m++]=pcolorData[temp+2];
					colorBGRA8_16[m++]=pcolorData[temp+3];

					red=pcolorData[temp+2];
					red>>=3;
					green=pcolorData[temp+1];
					green>>=2;
					blue=pcolorData[temp  ];
					blue>>=3;
					color16[8+k]=(red<<11) | (green<<5) | (blue);
					if(pcolorData[temp+3]<255)	//Only <128 is considered transparent
						isTransparency=true;
				}
				//Process line below, notice the (j+3)
				for(k=0; k<4; k++)
				{
					temp=((j+3)*width+i+k)*4;
					colorBGRA8_16[m++]=pcolorData[temp  ];
					colorBGRA8_16[m++]=pcolorData[temp+1];
					colorBGRA8_16[m++]=pcolorData[temp+2];
					colorBGRA8_16[m++]=pcolorData[temp+3];

					red=pcolorData[temp+2];
					red>>=3;
					green=pcolorData[temp+1];
					green>>=2;
					blue=pcolorData[temp  ];
					blue>>=3;
					color16[12+k]=(red<<11) | (green<<5) | (blue);
					if(pcolorData[temp+3]<255)	//Only <128 is considered transparent
						isTransparency=true;
				}

				//Find the 2 extremes in color16 array.
				//Devil uses the BGRA8 format for this
				ChooseEndPoints(color16, colorBGRA8_16, color_0, color_1);

				//If opaque, then color_0>color_1, else vice-versa
				if(isTransparency==false)
				{
					if(color_0<color_1)
					{
						ushorttemp=color_0;
						color_0=color_1;
						color_1=ushorttemp;
					}
				}
				else
				{
					if(color_0>color_1)
					{
						ushorttemp=color_0;
						color_0=color_1;
						color_1=ushorttemp;
					}
				}

				//Write the colors at either extreme
				pcolorData2[index  ]=color_0;
				pcolorData2[index+1]=color_1;

				//For each of the 16 pixels, check to see which they come close to best
				if(isTransparency)
					interpolatedColors=GenerateBitMask(color_0, color_1, 3, color16, colorBGRA8_16);
				else
					interpolatedColors=GenerateBitMask(color_0, color_1, 4, color16, colorBGRA8_16);
				
				memcpy(&pcolorData2[index+2], &interpolatedColors, 4);	//4bytes

				index+=4;		//64 bits written for each block for DXT1 compression
								//64 = 4 *16bit
			}
		}
	}
}

//PURPOSE:	The data in pcolorData is BGRA8 and not compressed. Compress it for DXT1
//			and allocate mem for colorData and write data into it.
//			Before calling this, make sure width and height are divisible by 4.
//			It's required for DXT compression.
sint ConvertBGRA8_To_DXT1(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, sint *totalCompressedColorData, uchar *&colorData)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		sint size=(width*height)/2;
		if((colorData=new uchar[size])==NULL)
			return 0;

		ConvertBGRA8_To_DXT1_2D(width, height, pcolorData, colorData);

		if(totalCompressedColorData)
			*totalCompressedColorData=size;
		
		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint size=(width*height)/2;
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint i;
		for(i=0; i<6; i++)
		{
			ConvertBGRA8_To_DXT1_2D(width, height, &pcolorData[size*i], &colorData[size*i]);
		}

		if(totalCompressedColorData)
			*totalCompressedColorData=size;

		return 1;
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint size=(width*height)/2;
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint i;
		for(i=0; i<depth; i++)
		{
			ConvertBGRA8_To_DXT1_2D(width, height, &pcolorData[size*i], &colorData[size*i]);
		}

		if(totalCompressedColorData)
			*totalCompressedColorData=size;
		
		return 1;
	}*/

	return -1;
}

//PURPOSE:	Called from ConvertBGRA8_To_DXT2OrDXT3
void ConvertBGRA8_To_DXT2OrDXT3_2D(sint width, sint height, uchar *pcolorData, uchar *colorData, bool isDXT2OrDXT3)
{
	if(isDXT2OrDXT3==false)
	{
		uchar alphaBlock[16];
		ushort colorBlock16[16], color_0, color_1, ushorttemp;
		uchar colorBGRA8_16[16*4];	//*4 for RGBA8
		sint i, j, temp;
		uint interpolatedColors;		//Each block has the 2 bit interpolated color region
		sint index=0;
		

		ushort *pcolorData2=(ushort *)colorData;

		for(j=0; j<height; j+=4)
		{
			for(i=0; i<width; i+=4)
			{
				//Get the alpha values of the 4x4 block and write to alphaBlock[16]
				//Get the color values of the 4x4 block, convert to B5G6R5, write to colorBlock16[16]
				//Get the color values of the 4x4 block as BGRA8 as well
				temp=(j*width+i)*4;
				alphaBlock[0]=pcolorData[temp+3];
				alphaBlock[1]=pcolorData[temp+7];
				alphaBlock[2]=pcolorData[temp+11];
				alphaBlock[3]=pcolorData[temp+15];

				colorBlock16[0]=(((ushort)pcolorData[temp+2])<<8) & 0xF800;
				colorBlock16[0] |= (((ushort)pcolorData[temp+1])<<3) & 0x07E0;
				colorBlock16[0] |= (((ushort)pcolorData[temp])>>3) & 0x001F;

				colorBGRA8_16[0]=pcolorData[temp];
				colorBGRA8_16[1]=pcolorData[temp+1];
				colorBGRA8_16[2]=pcolorData[temp+2];
				colorBGRA8_16[3]=pcolorData[temp+3];

				colorBlock16[1]=(((ushort)pcolorData[temp+6])<<8) & 0xF800;
				colorBlock16[1] |= (((ushort)pcolorData[temp+5])<<3) & 0x07E0;
				colorBlock16[1] |= (((ushort)pcolorData[temp+4])>>3) & 0x001F;
				colorBGRA8_16[4]=pcolorData[temp+4];
				colorBGRA8_16[5]=pcolorData[temp+5];
				colorBGRA8_16[6]=pcolorData[temp+6];
				colorBGRA8_16[7]=pcolorData[temp+7];

				colorBlock16[2]=(((ushort)pcolorData[temp+10])<<8) & 0xF800;
				colorBlock16[2] |= (((ushort)pcolorData[temp+9])<<3) & 0x07E0;
				colorBlock16[2] |= (((ushort)pcolorData[temp+8])>>3) & 0x001F;
				colorBGRA8_16[8]=pcolorData[temp+8];
				colorBGRA8_16[9]=pcolorData[temp+9];
				colorBGRA8_16[10]=pcolorData[temp+10];
				colorBGRA8_16[11]=pcolorData[temp+11];

				colorBlock16[3]=(((ushort)pcolorData[temp+14])<<8) & 0xF800;
				colorBlock16[3] |= (((ushort)pcolorData[temp+13])<<3) & 0x07E0;
				colorBlock16[3] |= (((ushort)pcolorData[temp+12])>>3) & 0x001F;
				colorBGRA8_16[12]=pcolorData[temp+12];
				colorBGRA8_16[13]=pcolorData[temp+13];
				colorBGRA8_16[14]=pcolorData[temp+14];
				colorBGRA8_16[15]=pcolorData[temp+15];

				temp=((j+1)*width+i)*4;
				alphaBlock[4]=pcolorData[temp+3];
				alphaBlock[5]=pcolorData[temp+7];
				alphaBlock[6]=pcolorData[temp+11];
				alphaBlock[7]=pcolorData[temp+15];

				colorBlock16[4]=(((ushort)pcolorData[temp+2])<<8) & 0xF800;
				colorBlock16[4] |= (((ushort)pcolorData[temp+1])<<3) & 0x07E0;
				colorBlock16[4] |= (((ushort)pcolorData[temp])>>3) & 0x001F;
				colorBGRA8_16[16]=pcolorData[temp];
				colorBGRA8_16[17]=pcolorData[temp+1];
				colorBGRA8_16[18]=pcolorData[temp+2];
				colorBGRA8_16[19]=pcolorData[temp+3];

				colorBlock16[5]=(((ushort)pcolorData[temp+6])<<8) & 0xF800;
				colorBlock16[5] |= (((ushort)pcolorData[temp+5])<<3) & 0x07E0;
				colorBlock16[5] |= (((ushort)pcolorData[temp+4])>>3) & 0x001F;
				colorBGRA8_16[20]=pcolorData[temp+4];
				colorBGRA8_16[21]=pcolorData[temp+5];
				colorBGRA8_16[22]=pcolorData[temp+6];
				colorBGRA8_16[23]=pcolorData[temp+7];

				colorBlock16[6]=(((ushort)pcolorData[temp+10])<<8) & 0xF800;
				colorBlock16[6] |= (((ushort)pcolorData[temp+9])<<3) & 0x07E0;
				colorBlock16[6] |= (((ushort)pcolorData[temp+8])>>3) & 0x001F;
				colorBGRA8_16[24]=pcolorData[temp+8];
				colorBGRA8_16[25]=pcolorData[temp+9];
				colorBGRA8_16[26]=pcolorData[temp+10];
				colorBGRA8_16[27]=pcolorData[temp+11];

				colorBlock16[7]=(((ushort)pcolorData[temp+14])<<8) & 0xF800;
				colorBlock16[7] |= (((ushort)pcolorData[temp+13])<<3) & 0x07E0;
				colorBlock16[7] |= (((ushort)pcolorData[temp+12])>>3) & 0x001F;
				colorBGRA8_16[28]=pcolorData[temp+12];
				colorBGRA8_16[29]=pcolorData[temp+13];
				colorBGRA8_16[30]=pcolorData[temp+14];
				colorBGRA8_16[31]=pcolorData[temp+15];

				temp=((j+2)*width+i)*4;
				alphaBlock[8]=pcolorData[temp+3];
				alphaBlock[9]=pcolorData[temp+7];
				alphaBlock[10]=pcolorData[temp+11];
				alphaBlock[11]=pcolorData[temp+15];

				colorBlock16[8]=(((ushort)pcolorData[temp+2])<<8) & 0xF800;
				colorBlock16[8] |= (((ushort)pcolorData[temp+1])<<3) & 0x07E0;
				colorBlock16[8] |= (((ushort)pcolorData[temp])>>3) & 0x001F;
				colorBGRA8_16[32]=pcolorData[temp];
				colorBGRA8_16[33]=pcolorData[temp+1];
				colorBGRA8_16[34]=pcolorData[temp+2];
				colorBGRA8_16[35]=pcolorData[temp+3];

				colorBlock16[9]=(((ushort)pcolorData[temp+6])<<8) & 0xF800;
				colorBlock16[9] |= (((ushort)pcolorData[temp+5])<<3) & 0x07E0;
				colorBlock16[9] |= (((ushort)pcolorData[temp+4])>>3) & 0x001F;
				colorBGRA8_16[36]=pcolorData[temp+4];
				colorBGRA8_16[37]=pcolorData[temp+5];
				colorBGRA8_16[38]=pcolorData[temp+6];
				colorBGRA8_16[39]=pcolorData[temp+7];

				colorBlock16[10]=(((ushort)pcolorData[temp+10])<<8) & 0xF800;
				colorBlock16[10] |= (((ushort)pcolorData[temp+9])<<3) & 0x07E0;
				colorBlock16[10] |= (((ushort)pcolorData[temp+8])>>3) & 0x001F;
				colorBGRA8_16[40]=pcolorData[temp+8];
				colorBGRA8_16[41]=pcolorData[temp+9];
				colorBGRA8_16[42]=pcolorData[temp+10];
				colorBGRA8_16[43]=pcolorData[temp+11];

				colorBlock16[11]=(((ushort)pcolorData[temp+14])<<8) & 0xF800;
				colorBlock16[11] |= (((ushort)pcolorData[temp+13])<<3) & 0x07E0;
				colorBlock16[11] |= (((ushort)pcolorData[temp+12])>>3) & 0x001F;
				colorBGRA8_16[44]=pcolorData[temp+12];
				colorBGRA8_16[45]=pcolorData[temp+13];
				colorBGRA8_16[46]=pcolorData[temp+14];
				colorBGRA8_16[47]=pcolorData[temp+15];

				temp=((j+3)*width+i)*4;
				alphaBlock[12]=pcolorData[temp+3];
				alphaBlock[13]=pcolorData[temp+7];
				alphaBlock[14]=pcolorData[temp+11];
				alphaBlock[15]=pcolorData[temp+15];

				colorBlock16[12]=(((ushort)pcolorData[temp+2])<<8) & 0xF800;
				colorBlock16[12] |= (((ushort)pcolorData[temp+1])<<3) & 0x07E0;
				colorBlock16[12] |= (((ushort)pcolorData[temp])>>3) & 0x001F;
				colorBGRA8_16[48]=pcolorData[temp];
				colorBGRA8_16[49]=pcolorData[temp+1];
				colorBGRA8_16[50]=pcolorData[temp+2];
				colorBGRA8_16[51]=pcolorData[temp+3];

				colorBlock16[13]=(((ushort)pcolorData[temp+6])<<8) & 0xF800;
				colorBlock16[13] |= (((ushort)pcolorData[temp+5])<<3) & 0x07E0;
				colorBlock16[13] |= (((ushort)pcolorData[temp+4])>>3) & 0x001F;
				colorBGRA8_16[52]=pcolorData[temp+4];
				colorBGRA8_16[53]=pcolorData[temp+5];
				colorBGRA8_16[54]=pcolorData[temp+6];
				colorBGRA8_16[55]=pcolorData[temp+7];

				colorBlock16[14]=(((ushort)pcolorData[temp+10])<<8) & 0xF800;
				colorBlock16[14] |= (((ushort)pcolorData[temp+9])<<3) & 0x07E0;
				colorBlock16[14] |= (((ushort)pcolorData[temp+8])>>3) & 0x001F;
				colorBGRA8_16[56]=pcolorData[temp+8];
				colorBGRA8_16[57]=pcolorData[temp+9];
				colorBGRA8_16[58]=pcolorData[temp+10];
				colorBGRA8_16[59]=pcolorData[temp+11];

				colorBlock16[15]=(((ushort)pcolorData[temp+14])<<8) & 0xF800;
				colorBlock16[15] |= (((ushort)pcolorData[temp+13])<<3) & 0x07E0;
				colorBlock16[15] |= (((ushort)pcolorData[temp+12])>>3) & 0x001F;
				colorBGRA8_16[60]=pcolorData[temp+12];
				colorBGRA8_16[61]=pcolorData[temp+13];
				colorBGRA8_16[62]=pcolorData[temp+14];
				colorBGRA8_16[63]=pcolorData[temp+15];


				//According to DX9 SDK, a variety of methods could be used to generate the values
				//for the 4x4 alpha block. A simple way is to use the 4 upper bits.
				pcolorData2[index  ]=(((ushort)((alphaBlock[0] & 0xF0) | ((alphaBlock[1]>>4) & 0x0F)))<<8) 
					| ((ushort)((alphaBlock[2] & 0xF0) | ((alphaBlock[3]>>4) & 0x0F)));
				pcolorData2[index+1]=(((ushort)((alphaBlock[4] & 0xF0) | ((alphaBlock[5]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[6] & 0xF0) | ((alphaBlock[7]>>4) & 0x0F)));
				pcolorData2[index+2]=(((ushort)((alphaBlock[8] & 0xF0) | ((alphaBlock[9]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[10] & 0xF0) | ((alphaBlock[11]>>4) & 0x0F)));
				pcolorData2[index+3]=(((ushort)((alphaBlock[12] & 0xF0) | ((alphaBlock[13]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[14] & 0xF0) | ((alphaBlock[15]>>4) & 0x0F)));
				
				ChooseEndPoints(colorBlock16, colorBGRA8_16, color_0, color_1);
				{	//Should we order the colors?
					if(color_0<color_1)
					{
						ushorttemp=color_0;
						color_0=color_1;
						color_1=ushorttemp;
					}
				}
				pcolorData2[index+4]=color_0;
				pcolorData2[index+5]=color_1;
				interpolatedColors=GenerateBitMask(color_0, color_1, 4, colorBlock16, colorBGRA8_16);
				
				memcpy(&pcolorData2[index+6], &interpolatedColors, 4);	//4bytes

				index+=8;		//128 bits written for each block for DXT2-3 compression
			}		
		}
	}
	else //if(isDXT2OrDXT3==true)
	{
		//In the case of DXT2, the B5G6R5 are multiplied by the alpha, where the alpha is
		//considered a percentage of opacity.
		//Avoid using the FPU, which would kill performance Doing mult 10000000 and div instead.
		uchar alphaBlock[16];
		ushort colorBlock16[16], color_0, color_1, ushorttemp;
		uchar colorBGRA8_16[16*4];	//*4 for RGBA8
		sint i, j, temp;
		uint red, green, blue;
		uint interpolatedColors;		//Each block has the 2 bit interpolated color region
		sint index=0;
		

		ushort *pcolorData2=(ushort *)colorData;

		for(j=0; j<height; j+=4)
		{
			for(i=0; i<width; i+=4)
			{
				//Get the alpha values of the 4x4 block and write to alphaBlock[16]
				//Get the color values of the 4x4 block, convert to B5G6R5, write to colorBlock16[16]
				//Get the color values of the 4x4 block as BGRA8 as well
				temp=(j*width+i)*4;
				alphaBlock[0]=pcolorData[temp+3];
				alphaBlock[1]=pcolorData[temp+7];
				alphaBlock[2]=pcolorData[temp+11];
				alphaBlock[3]=pcolorData[temp+15];

				red=((((((uint)pcolorData[temp+2])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				green=((((((uint)pcolorData[temp+1])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				blue=((((((uint)pcolorData[temp  ])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				colorBlock16[0]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[0] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[0] |= (((ushort)blue)>>3) & 0x001F;

				colorBGRA8_16[0]=pcolorData[temp];
				colorBGRA8_16[1]=pcolorData[temp+1];
				colorBGRA8_16[2]=pcolorData[temp+2];
				colorBGRA8_16[3]=pcolorData[temp+3];

				red=((((((uint)pcolorData[temp+6])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				green=((((((uint)pcolorData[temp+5])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				blue=((((((uint)pcolorData[temp+4])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				colorBlock16[1]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[1] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[1] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[4]=pcolorData[temp+4];
				colorBGRA8_16[5]=pcolorData[temp+5];
				colorBGRA8_16[6]=pcolorData[temp+6];
				colorBGRA8_16[7]=pcolorData[temp+7];

				red=((((((uint)pcolorData[temp+10])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				green=((((((uint)pcolorData[temp+9])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				blue=((((((uint)pcolorData[temp+8])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				colorBlock16[2]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[2] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[2] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[8]=pcolorData[temp+8];
				colorBGRA8_16[9]=pcolorData[temp+9];
				colorBGRA8_16[10]=pcolorData[temp+10];
				colorBGRA8_16[11]=pcolorData[temp+11];

				red=((((((uint)pcolorData[temp+14])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				green=((((((uint)pcolorData[temp+13])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				blue=((((((uint)pcolorData[temp+12])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				colorBlock16[3]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[3] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[3] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[12]=pcolorData[temp+12];
				colorBGRA8_16[13]=pcolorData[temp+13];
				colorBGRA8_16[14]=pcolorData[temp+14];
				colorBGRA8_16[15]=pcolorData[temp+15];

				temp=((j+1)*width+i)*4;
				alphaBlock[4]=pcolorData[temp+3];
				alphaBlock[5]=pcolorData[temp+7];
				alphaBlock[6]=pcolorData[temp+11];
				alphaBlock[7]=pcolorData[temp+15];

				red=((((((uint)pcolorData[temp+2])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				green=((((((uint)pcolorData[temp+1])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				blue=((((((uint)pcolorData[temp  ])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				colorBlock16[4]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[4] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[4] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[16]=pcolorData[temp];
				colorBGRA8_16[17]=pcolorData[temp+1];
				colorBGRA8_16[18]=pcolorData[temp+2];
				colorBGRA8_16[19]=pcolorData[temp+3];

				red=((((((uint)pcolorData[temp+6])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				green=((((((uint)pcolorData[temp+5])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				blue=((((((uint)pcolorData[temp+4])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				colorBlock16[5]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[5] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[5] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[20]=pcolorData[temp+4];
				colorBGRA8_16[21]=pcolorData[temp+5];
				colorBGRA8_16[22]=pcolorData[temp+6];
				colorBGRA8_16[23]=pcolorData[temp+7];

				red=((((((uint)pcolorData[temp+10])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				green=((((((uint)pcolorData[temp+9])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				blue=((((((uint)pcolorData[temp+8])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				colorBlock16[6]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[6] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[6] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[24]=pcolorData[temp+8];
				colorBGRA8_16[25]=pcolorData[temp+9];
				colorBGRA8_16[26]=pcolorData[temp+10];
				colorBGRA8_16[27]=pcolorData[temp+11];

				red=((((((uint)pcolorData[temp+14])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				green=((((((uint)pcolorData[temp+13])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				blue=((((((uint)pcolorData[temp+12])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				colorBlock16[7]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[7] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[7] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[28]=pcolorData[temp+12];
				colorBGRA8_16[29]=pcolorData[temp+13];
				colorBGRA8_16[30]=pcolorData[temp+14];
				colorBGRA8_16[31]=pcolorData[temp+15];

				temp=((j+2)*width+i)*4;
				alphaBlock[8]=pcolorData[temp+3];
				alphaBlock[9]=pcolorData[temp+7];
				alphaBlock[10]=pcolorData[temp+11];
				alphaBlock[11]=pcolorData[temp+15];

				red=((((((uint)pcolorData[temp+2])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				green=((((((uint)pcolorData[temp+1])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				blue=((((((uint)pcolorData[temp  ])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				colorBlock16[8]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[8] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[8] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[32]=pcolorData[temp];
				colorBGRA8_16[33]=pcolorData[temp+1];
				colorBGRA8_16[34]=pcolorData[temp+2];
				colorBGRA8_16[35]=pcolorData[temp+3];

				red=((((((uint)pcolorData[temp+6])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				green=((((((uint)pcolorData[temp+5])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				blue=((((((uint)pcolorData[temp+4])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				colorBlock16[9]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[9] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[9] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[36]=pcolorData[temp+4];
				colorBGRA8_16[37]=pcolorData[temp+5];
				colorBGRA8_16[38]=pcolorData[temp+6];
				colorBGRA8_16[39]=pcolorData[temp+7];

				red=((((((uint)pcolorData[temp+10])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				green=((((((uint)pcolorData[temp+9])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				blue=((((((uint)pcolorData[temp+8])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				colorBlock16[10]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[10] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[10] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[40]=pcolorData[temp+8];
				colorBGRA8_16[41]=pcolorData[temp+9];
				colorBGRA8_16[42]=pcolorData[temp+10];
				colorBGRA8_16[43]=pcolorData[temp+11];

				red=((((((uint)pcolorData[temp+14])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				green=((((((uint)pcolorData[temp+13])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				blue=((((((uint)pcolorData[temp+12])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				colorBlock16[11]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[11] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[11] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[44]=pcolorData[temp+12];
				colorBGRA8_16[45]=pcolorData[temp+13];
				colorBGRA8_16[46]=pcolorData[temp+14];
				colorBGRA8_16[47]=pcolorData[temp+15];

				temp=((j+3)*width+i)*4;
				alphaBlock[12]=pcolorData[temp+3];
				alphaBlock[13]=pcolorData[temp+7];
				alphaBlock[14]=pcolorData[temp+11];
				alphaBlock[15]=pcolorData[temp+15];

				red=((((((uint)pcolorData[temp+2])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				green=((((((uint)pcolorData[temp+1])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				blue=((((((uint)pcolorData[temp  ])*1000000)/(uint)pcolorData[temp+3])*255)/1000000);
				colorBlock16[12]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[12] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[12] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[48]=pcolorData[temp];
				colorBGRA8_16[49]=pcolorData[temp+1];
				colorBGRA8_16[50]=pcolorData[temp+2];
				colorBGRA8_16[51]=pcolorData[temp+3];

				red=((((((uint)pcolorData[temp+6])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				green=((((((uint)pcolorData[temp+5])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				blue=((((((uint)pcolorData[temp+4])*1000000)/(uint)pcolorData[temp+7])*255)/1000000);
				colorBlock16[13]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[13] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[13] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[52]=pcolorData[temp+4];
				colorBGRA8_16[53]=pcolorData[temp+5];
				colorBGRA8_16[54]=pcolorData[temp+6];
				colorBGRA8_16[55]=pcolorData[temp+7];

				red=((((((uint)pcolorData[temp+10])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				green=((((((uint)pcolorData[temp+9])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				blue=((((((uint)pcolorData[temp+8])*1000000)/(uint)pcolorData[temp+11])*255)/1000000);
				colorBlock16[14]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[14] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[14] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[56]=pcolorData[temp+8];
				colorBGRA8_16[57]=pcolorData[temp+9];
				colorBGRA8_16[58]=pcolorData[temp+10];
				colorBGRA8_16[59]=pcolorData[temp+11];

				red=((((((uint)pcolorData[temp+14])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				green=((((((uint)pcolorData[temp+13])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				blue=((((((uint)pcolorData[temp+12])*1000000)/(uint)pcolorData[temp+15])*255)/1000000);
				colorBlock16[15]=(((ushort)red)<<8) & 0xF800;
				colorBlock16[15] |= (((ushort)green)<<3) & 0x07E0;
				colorBlock16[15] |= (((ushort)blue)>>3) & 0x001F;
				colorBGRA8_16[60]=pcolorData[temp+12];
				colorBGRA8_16[61]=pcolorData[temp+13];
				colorBGRA8_16[62]=pcolorData[temp+14];
				colorBGRA8_16[63]=pcolorData[temp+15];


				//According to DX9 SDK, a variety of methods could be used to generate the values
				//for the 4x4 alpha block. A simple way is to use the 4 upper bits.
				pcolorData2[index  ]=(((ushort)((alphaBlock[0] & 0xF0) | ((alphaBlock[1]>>4) & 0x0F)))<<8) 
					| ((ushort)((alphaBlock[2] & 0xF0) | ((alphaBlock[3]>>4) & 0x0F)));
				pcolorData2[index+1]=(((ushort)((alphaBlock[4] & 0xF0) | ((alphaBlock[5]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[6] & 0xF0) | ((alphaBlock[7]>>4) & 0x0F)));
				pcolorData2[index+2]=(((ushort)((alphaBlock[8] & 0xF0) | ((alphaBlock[9]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[10] & 0xF0) | ((alphaBlock[11]>>4) & 0x0F)));
				pcolorData2[index+3]=(((ushort)((alphaBlock[12] & 0xF0) | ((alphaBlock[13]>>4) & 0x0F)))<<8)
					| ((ushort)((alphaBlock[14] & 0xF0) | ((alphaBlock[15]>>4) & 0x0F)));
				
				ChooseEndPoints(colorBlock16, colorBGRA8_16, color_0, color_1);
				{	//Should we order the colors?
					if(color_0<color_1)
					{
						ushorttemp=color_0;
						color_0=color_1;
						color_1=ushorttemp;
					}
				}
				pcolorData2[index+4]=color_0;
				pcolorData2[index+5]=color_1;
				interpolatedColors=GenerateBitMask(color_0, color_1, 4, colorBlock16, colorBGRA8_16);
				
				memcpy(&pcolorData2[index+6], &interpolatedColors, 4);	//4bytes

				index+=8;		//128 bits written for each block for DXT2-3 compression
			}		
		}
	}
}

//PURPOSE:	The data in pcolorData is BGRA8 and not compressed. Compress it for DXT2 or DXT3
//			and allocate mem for colorData and write data into it.
//			Before calling this, make sure width and height are divisible by 4.
//			It's required for DXT compression.
sint ConvertBGRA8_To_DXT2OrDXT3(sint imageType, sint width, sint height, sint depth, uchar *pcolorData, sint *totalCompressedColorData, uchar *&colorData, bool isDXT2OrDXT3)
{
	/*if(imageType==IMAGETYPE_2DIMAGE)
	{
		sint size=width*height;
		if((colorData=new uchar[size])==NULL)
			return 0;

		ConvertBGRA8_To_DXT2OrDXT3_2D(width, height, pcolorData, colorData, isDXT2OrDXT3);

		if(totalCompressedColorData)
			*totalCompressedColorData=size;

		return 1;
	}
	else if(imageType==IMAGETYPE_CUBEMAPIMAGE)
	{
		sint size=width*height;
		if((colorData=new uchar[size*6])==NULL)
			return 0;

		sint i;
		sint size2=width*height*4;
		for(i=0; i<6; i++)
		{
			ConvertBGRA8_To_DXT2OrDXT3_2D(width, height, &pcolorData[i*size2], &colorData[i*size], isDXT2OrDXT3);
		}

		if(totalCompressedColorData)
			*totalCompressedColorData=size;

		return 1;
	}
	else if(imageType==IMAGETYPE_3DIMAGE)
	{
		sint size=width*height;
		if((colorData=new uchar[size*depth])==NULL)
			return 0;

		sint i;
		sint size2=width*height*4;
		for(i=0; i<depth; i++)
		{
			ConvertBGRA8_To_DXT2OrDXT3_2D(width, height, &pcolorData[i*size2], &colorData[i*size], isDXT2OrDXT3);
		}

		if(totalCompressedColorData)
			*totalCompressedColorData=size;

		return 1;
	}*/

	return -1;
}

//PURPOSE:	Read a graphics file.
//			Only supports DDS files for now.
//			The decomperssed data (pixels) are stored with dataalignment == 4
//			Supports 2D, 3D and cubemap textures.
//
//PARAMETERS:
//			[in]
//
//RETURN:
//			1 for success.
//			-1 if could not open the file for reading.
//			Returns 0 if out of memory, 2 if something goes wrong, 1 if all is OK.
//			All textures are promoted to 24 bpp, if they are less.
//			Bitmaps can be 2D texture, cube map or 3D texture.
//			Everything will be stored in pColorData and pCurrentColorData[] will point to the
//			current image.
//			Mipmaps are not stored, if they are present in the DDS.
sint glhReadGraphicsFile(char *errorMessage, const char *pfilePath, sint *width, sint *height, sint *depth, GLHPIXELFORMAT *pixelFormat, uint type, void *decompressedPixels, void *compressedPixels)
{
	DDSHeader	header;
	sint newWidth, newHeight, newDepth;
	sint size;
	uchar *colorData;
	FILE *hfile;

	DeallocateMemoryBuffer();
	DeallocateMemoryForImage();
	DeallocateMemoryForCompressedImage();

	hfile=fopen(pfilePath, "rb");
	if(hfile==0)
	{
		CopyString(errorMessage, (schar *)"The file ");
		ConcatenateString(errorMessage, pfilePath);
		ConcatenateString(errorMessage, (schar *)" does not exist or cannot be read.\nPlease check the file name and access privileges.");
		return -1;
	}

	//Read the header
	size=sizeof(DDSHeader);
	if((sint)fread(&header, 1, size, hfile)<size)
	{
		strcpy(ErrorMessage, "Header size is invalid for this DDS file. (1)");
		goto ExitOnError2_MY;
	}

	//Check to see if all the values are correct (or most of them)
	//Magic number and struct size?
	if((header.MagicNumber!=DWORDFROMCHARS('D', 'D', 'S', ' '))||(header.DDSurfaceDesc2.dwSize!=124))
	{
		strcpy(ErrorMessage, "Not a DSS file. Bad header. (2)");
		goto ExitOnError2_MY;
	}
	//Some flags must always appear
	if(((header.DDSurfaceDesc2.dwFlags & DDSD_CAPS) ==0)||((header.DDSurfaceDesc2.dwFlags & DDSD_PIXELFORMAT) ==0)||
		((header.DDSurfaceDesc2.dwFlags & DDSD_WIDTH) ==0)||((header.DDSurfaceDesc2.dwFlags & DDSD_HEIGHT) ==0))
	{
		strcpy(ErrorMessage, "Not a DSS file. Bad header. Flags missing. (3)");
		goto ExitOnError2_MY;
	}

	//Get width and height
	Width=(header.DDSurfaceDesc2.dwWidth<0) ? -(sint)(header.DDSurfaceDesc2.dwWidth) : header.DDSurfaceDesc2.dwWidth;
	Height=(header.DDSurfaceDesc2.dwHeight<0) ? -(sint)(header.DDSurfaceDesc2.dwHeight) : header.DDSurfaceDesc2.dwHeight;

	//Make sure all sides are power of 2 (DDS is more for textures than anything else)
	//Forget about RECT textures.
	ReturnVal=glhLowerPowerOfTwo(Width, (sint *)&newWidth, Height, (sint *)&newHeight);
	if(ReturnVal==0)
	{
		strcpy(ErrorMessage, "Width or height is not power of 2.\nPower of 2 dimensions preferred. (3.1)");
		goto ExitOnError2_MY;
	}
	
	//Get size of scanline or entire size
	if(header.DDSurfaceDesc2.dwFlags & DDSD_PITCH)	//Non-compressed format
	{
		DDS_IsCompressed=FALSE;
		ScanLinePadded=header.DDSurfaceDesc2.dwPitchOrLinearSize;
	}
	else if(header.DDSurfaceDesc2.dwFlags & DDSD_LINEARSIZE)	//Compressed format
	{
		DDS_IsCompressed=TRUE;
		ScanLinePadded=header.DDSurfaceDesc2.dwPitchOrLinearSize;	//This is the entire image size
	}
	else
	{	//It's not a requirement to have thses flags, so let DDS_IsCompressed==-1 for now
		DDS_IsCompressed=-1;			//It will be changed later
		//strcpy(ErrorMessage, "Not a DSS file. Bad header. Flags missing. (4)");
		//goto ExitOnError2_MY;
	}

	//Is depth present?
	if(header.DDSurfaceDesc2.dwFlags & DDSD_DEPTH)	//Is it a 3D texture
		Depth=header.DDSurfaceDesc2.dwDepth;
	else
		Depth=0;		//0 is power of 2

	if(Depth!=0)
	{
		ReturnVal=glhLowerPowerOfTwo2(Depth, (sint *)&newDepth);
		if(ReturnVal==0)
		{
			strcpy(ErrorMessage, "Depth is not power of 2.\nPower of 2 dimensions preferred. (3.2)");
			goto ExitOnError2_MY;
		}
	}

	//Any mipmaps? How many levels?
	if(header.DDSurfaceDesc2.dwFlags & DDSD_MIPMAPCOUNT)
		DDS_MipmapLevelCount=header.DDSurfaceDesc2.dwMipMapCount;
	else
		DDS_MipmapLevelCount=0;

	if(header.DDSurfaceDesc2.ddpfPixelFormat.dwSize!=32)	//Must be 32 bytes
	{
		strcpy(ErrorMessage, "Not a DSS file. Bad header. (5)");
		goto ExitOnError2_MY;
	}

	//Check to see if the flags in this part make sense.
	//If before we set DDS_IsCompressed=FALSE and now it says TRUE here
	//it doesn't make sense and vice-versa.
	if(DDS_IsCompressed!=-1)		//Only if the flags were present
	{
		if(((header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_RGB)&&(DDS_IsCompressed==TRUE))||
			((header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_FOURCC)&&(DDS_IsCompressed==FALSE)))
		{
			strcpy(ErrorMessage, "Not sure if this DSS file is compressed or not.\nConflicting information present in header. (6)");
			goto ExitOnError2_MY;
		}
	}

	//**** There are 17 formats in DDS (for now) ****
	if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_FOURCC)	//Compressed can be DXT compressed
	{
		DDS_IsCompressed=TRUE;

		if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==DWORDFROMCHARS('D', 'X', 'T', '1'))
			DDS_CompressionMethod=DDS_COMPRESSION_DXT1;
		else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==DWORDFROMCHARS('D', 'X', 'T', '2'))
			DDS_CompressionMethod=DDS_COMPRESSION_DXT2;
		else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==DWORDFROMCHARS('D', 'X', 'T', '3'))
			DDS_CompressionMethod=DDS_COMPRESSION_DXT3;
		else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==DWORDFROMCHARS('D', 'X', 'T', '4'))
			DDS_CompressionMethod=DDS_COMPRESSION_DXT4;
		else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==DWORDFROMCHARS('D', 'X', 'T', '5'))
			DDS_CompressionMethod=DDS_COMPRESSION_DXT5;
		else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC==0x00000024)
		{
			//For some reason, when I save a texture as RGBA16 with DxTex,
			//it sets
			//header.DDSurfaceDesc2.ddpfPixelFormat.dwFourCC=0x00000024
			//header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBBitCount==0
			//Let's correct the error...
			//Unfortunatly the masks are useless and should be already zero.
			DDS_IsCompressed=FALSE;
			header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBBitCount=64;
			header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask=0;
			header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask=0;
			header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask=0;
			header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask=0;
			BitsPerPixel=64;
		}

		//We must decompress the texture
	}
	else if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_RGB)	//Non-compressed
	{
		DDS_IsCompressed=FALSE;

		//Usually, this is 16, 24, 32bit but there are many other formats
		//For the A16B16G16R16 = 64 bit format, I will make an exception.
		//For now, we convert A16B16G16R16 to RGBA8 losing a lot of precesion
		//but in the future I might change this.
		BitsPerPixel=header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBBitCount;
		if((BitsPerPixel!=16)&&(BitsPerPixel!=24)&&(BitsPerPixel!=32)&&(BitsPerPixel!=64))
		{
			sprintf(ErrorMessage, "Bits per pixel for this DDS file is not 16 or 24 or 32 or 64. (6.1)\nBits per pixel = %d", BitsPerPixel);
			goto ExitOnError2_MY;
		}
	}
	else
	{
		strcpy(ErrorMessage, "Not a DSS file. Bad header. Flags missing. (7)");
		goto ExitOnError2_MY;
	}

	//Now for the TDDCAPS2 structure
	if((header.DDSurfaceDesc2.ddsCaps.dwCaps1 & DDSCAPS_TEXTURE) ==0)	//Must be present
	{
		strcpy(ErrorMessage, "Not a DSS file. Bad header. (8)");
		goto ExitOnError2_MY;
	}
	
	//We don't care for mipmaps.
	if(header.DDSurfaceDesc2.ddsCaps.dwCaps1 & DDSCAPS_MIPMAP)
	{	//Don't load mipmaps
		i=0;
	}
	//For DDS that contain multiple surfaces like Cubemap and 3D
	//PS: It looks like DXTex des not set this flag for 3D textures but
	//as sint as Depth!=0, it is a 3D texture.
	if(header.DDSurfaceDesc2.ddsCaps.dwCaps1 & DDSCAPS_COMPLEX)
	{	//This flag is useless
		i=1;
	}
	//If a cubemap
	if(header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP)
	{
		ImageType=IMAGETYPE_CUBEMAPIMAGE;

		//Make sure all 6 faces are present
		if(((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEX) ==0)||
			((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEX) ==0)||
			((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEY) ==0)||
			((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEY) ==0)||
			((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_POSITIVEZ) ==0)||
			((header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_CUBEMAP_NEGATIVEZ) ==0))
		{
			strcpy(ErrorMessage, "This DDS file contains a cubemap, but it looks like not all faces are present,\ntherefore this is an invalid file. (103)");
			goto ExitOnError2_MY;
		}

		//Compress or not? Compute size and load it in. Decompress if it was compressed.
		if(DDS_IsCompressed==FALSE)
		{
			BytesPerPixel=BitsPerPixel/8;

			//Check the masks to see where the BGRA values are
			//Just handle the typical arrangements else flag an error
			if(BitsPerPixel==32)
			{
				//I'm guessing for X formats, this flag is not present, but who cares...
				//if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)

				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
					//(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask		//Assume alpha is at 0xFF000000
				{
					//For A8R8G8B8 and X8R8G8B8
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					
					//Read the color data, the faces should be in sequence :
					//PosX, NegX, PosY, NegY, PosZ, NegZ
					//If there are mipmaps, they will be place right after the faces so we should
					//skip them
					if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
						goto ExitOnError0_MY;

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						//Get max mipmap count
						//2D and not GL_TEXTURE_CUBE_MAP because it is just for 1 face
						glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
						//Get amount of mem required for the mipmaps except for base level
						glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_RGBA8, 4, Width, Height, 0, 1, tempo-1);
					}

					//Read PosX
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1138)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegX
					if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1150)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosY
					if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1162)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegY
					if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1174)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosZ
					if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1186)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegZ
					if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1198)");
						goto ExitOnError2_MY;
					}
				}
				else if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x000003FF)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000FFC00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x3FF00000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0xC0000000))
				{
					//For A2R10G10B10
					//This is an odd format. Our TGraphicsFile doesn't support manipulation
					//of this kind of format. Convert it to BGRA8.
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
						goto ExitOnError0_MY;

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						//Get max mipmap count
						glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
						//Get amount of mem required for the mipmaps except for base level
						glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_RGBA8, 4, Width, Height, 0, 1, tempo-1);
					}

					//Read PosX
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1138)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegX
					if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1150)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosY
					if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1162)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegY
					if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1174)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosZ
					if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1186)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegZ
					if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1198)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A2B10G10R10_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (443)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==16)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00007C00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000003E0)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x0000001F))
				//	(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0x00008000))
				{
					//For A1R5G5B5 and X1R5G5B5
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
						goto ExitOnError0_MY;
					
					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						//Get max mipmap count
						glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
						//Get amount of mem required for the mipmaps except for base level
						glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_RGB5_A1, 4, Width, Height, 0, 1, tempo-1);
					}

					//Read PosX
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1138)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegX
					if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1150)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosY
					if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1162)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegY
					if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1174)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosZ
					if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1186)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegZ
					if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1198)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A1R5G5B5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (1289)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==24)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
				{
					//For R8G8B8
					TotalColorData=Width*BytesPerPixel*Height;
					ScanLinePadded=((Width*BytesPerPixel+3)/4)*4;
					TotalColorDataWithPadding=ScanLinePadded*Height;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
						goto ExitOnError0_MY;
					
					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						//Get max mipmap count
						glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
						//Get amount of mem required for the mipmaps except for base level
						glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_RGB8, 4, Width, Height, 0, 1, tempo-1);
					}

					//Read PosX
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1138)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegX
					if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1150)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosY
					if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1162)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegY
					if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1174)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosZ
					if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1186)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegZ
					if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1198)");
						goto ExitOnError2_MY;
					}

					BitsPerPixel=24;
					BytesPerPixel=3;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (526)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==64)
			{
				//This is a RGBA16 format.
				//The masks in header.DDSurfaceDesc2.ddpfPixelFormat are all 0.
				{
					//For RGBA16
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
						goto ExitOnError0_MY;
					
					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						//Get max mipmap count
						glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
						//Get amount of mem required for the mipmaps except for base level
						glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_RGBA16, 4, Width, Height, 0, 1, tempo-1);
					}

					//Read PosX
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1138)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegX
					if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1150)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosY
					if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1162)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegY
					if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1174)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read PosZ
					if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1186)");
						goto ExitOnError2_MY;
					}

					if(DDS_MipmapLevelCount>1)	//Skip mipmaps
					{
						fseek(hfile, tempo2, SEEK_CUR);
					}

					//Read NegZ
					if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1198)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A16B16G16R16_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
			}
		}
		else //if(DDS_IsCompressed==TRUE)
		{	//DX1 to 5 compression method used
			//In each case, calculate size of the compressed image, read the bytes,
			//then call appropriate decompressor to convert to BGRA8 format.
			//
			//size = max(1, width ÷ 4) x max(1, height ÷ 4) x 8(DXT1) or 16(DXT2-5)
			switch(DDS_CompressionMethod)
			{
			case DDS_COMPRESSION_DXT1:
				//TotalColorData is for the compresse size
				TotalColorData=(Width*Height*4)/8;		//(Width*Height) is number of pixels, (Width*Height*4) is bit count
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
					goto ExitOnError0_MY;

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					//Get max mipmap count
					glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
					//Get amount of mem required for the mipmaps except for base level
					glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, 4, Width, Height, 0, 1, tempo-1);
				}

				//Read PosX
				if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1828)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegX
				if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1840)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosY
				if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1852)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegY
				if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1864)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosZ
				if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1876)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegZ
				if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1888)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				colorData=NULL;
				if(ConvertDXT1_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT2:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
					goto ExitOnError0_MY;

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					//Get max mipmap count
					glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
					//Get amount of mem required for the mipmaps except for base level
					glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 4, Width, Height, 0, 1, tempo-1);
				}

				//Read PosX
				if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1828)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegX
				if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1840)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosY
				if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1852)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegY
				if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1864)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosZ
				if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1876)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegZ
				if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1888)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT3:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
					goto ExitOnError0_MY;

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					//Get max mipmap count
					glhGetMaxMipmapCount((sint *)&tempo, GL_TEXTURE_2D, Width, Height, 0);
					//Get amount of mem required for the mipmaps except for base level
					glhMemoryUsageWithMipmaps((sint *)&tempo2, GL_TEXTURE_2D, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, 4, Width, Height, 0, 1, tempo-1);
				}

				//Read PosX
				if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1828)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegX
				if((size=fread(&pColorData[TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1840)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosY
				if((size=fread(&pColorData[2*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1852)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegY
				if((size=fread(&pColorData[3*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1864)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read PosZ
				if((size=fread(&pColorData[4*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1876)");
					goto ExitOnError2_MY;
				}

				if(DDS_MipmapLevelCount>1)	//Skip mipmaps
				{
					fseek(hfile, tempo2, SEEK_CUR);
				}

				//Read NegZ
				if((size=fread(&pColorData[5*TotalColorDataWithPadding], 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1888)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT4:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				BitsPerPixel=32;
				BytesPerPixel=4;
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (150)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT5:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				BitsPerPixel=32;
				BytesPerPixel=4;
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*6])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (160)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			}
		}
	}
	//If 3D
	else if(header.DDSurfaceDesc2.ddsCaps.dwCaps2 & DDSCAPS2_VOLUME)
	{
		//Depth must not be 0
		if(Depth==0)
		{
			strcpy(ErrorMessage, "This is a 3D texture, but depth == 0. (10)");
			goto ExitOnError2_MY;
		}

		ImageType=IMAGETYPE_3DIMAGE;

		if(DDS_IsCompressed==FALSE)
		{
			BytesPerPixel=BitsPerPixel/8;

			//Could be
			//A8R8G8B8 = 32								<---
			//A1R5G5B5 = 16								<---
			//A4R4G4B4 = 16
			//R8G8B8 = 24								<---
			//R5G6B5 = 16
			//A2R10G10B10 = 32							<---
			//X8R8G8B8 = 32  (Same as A8R8G8B8)			<---
			//X1R5G5B5 = 16   (Same as A1R5G5B5)		<---
			//R3G3B2 = 8 (WTF???)
			//A8R3G3B2 = 16 (WTF???)
			//X4R4G4B4 = 16 (Same as A4R4G4B4)
			//A16R16G16B16 = 64							<---
			//The others are the DXT formats.

			//Check the masks to see where the BGRA values are
			//Just handle the typical arrangements else flag an error
			if(BitsPerPixel==32)
			{
				//I'm guessing for X formats, this flag is not present, but who cares...
				//if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)

				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
					//(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask		//Assume alpha is at 0xFF000000
				{
					//For A8R8G8B8 and X8R8G8B8
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
						goto ExitOnError0_MY;

					//All the slices are together, followed by 3D mipmaps if present
					if((size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile))!=TotalColorDataWithPadding*Depth)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (2222)");
						goto ExitOnError2_MY;
					}
				}
				else if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x000003FF)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000FFC00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x3FF00000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0xC0000000))
				{
					//For A2R10G10B10
					//This is an odd format. Our TGraphicsFile doesn't support manipulation
					//of this kind of format. Convert it to BGRA8.
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;

					//All the slices are together, followed by 3D mipmaps if present
					if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
						goto ExitOnError0_MY;

					if((size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile))!=TotalColorDataWithPadding*Depth)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (2243)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A2B10G10R10_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (2254)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==16)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00007C00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000003E0)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x0000001F))
				//	(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0x00008000))
				{
					//For A1R5G5B5 and X1R5G5B5
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					
					//All the slices are together, followed by 3D mipmaps if present
					if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile))!=TotalColorDataWithPadding*Depth)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (2276)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A1R5G5B5_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (2290)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==24)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
				{
					//For R8G8B8
					TotalColorData=Width*BytesPerPixel*Height;
					ScanLinePadded=((Width*BytesPerPixel+3)/4)*4;
					TotalColorDataWithPadding=ScanLinePadded*Height;

					//All the slices are together, followed by 3D mipmaps if present
					if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile))!=TotalColorDataWithPadding*Depth)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (2277)");
						goto ExitOnError2_MY;
					}

					BitsPerPixel=24;
					BytesPerPixel=3;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (2286)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==64)
			{
				//This is a RGBA16 format.
				//The masks in header.DDSurfaceDesc2.ddpfPixelFormat are all 0.
				{
					//For RGBA16
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;

					//All the slices are together, followed by 3D mipmaps if present
					if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile))!=TotalColorDataWithPadding*Depth)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (2339)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A16B16G16R16_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
			}
		}
		else //if(DDS_IsCompressed==TRUE)
		{	//DX1 to 5 compression method used
			//In each case, calculate size of the compressed image, read the bytes,
			//then call appropriate decompressor to convert to BGRA8 format.
			//
			//size = max(1, width ÷ 4) x max(1, height ÷ 4) x 8(DXT1) or 16(DXT2-5)
			switch(DDS_CompressionMethod)
			{
			case DDS_COMPRESSION_DXT1:
				//TotalColorData is for the compresse size
				TotalColorData=(Width*Height*4)/8;		//(Width*Height) is number of pixels, (Width*Height*4) is bit count
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile);
				if(size!=TotalColorDataWithPadding*Depth)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (120)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				colorData=NULL;
				if(ConvertDXT1_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT2:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile);
				if(size!=TotalColorDataWithPadding*Depth)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (130)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT3:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding*Depth, hfile);
				if(size!=TotalColorDataWithPadding*Depth)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (140)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT4:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				BitsPerPixel=32;
				BytesPerPixel=4;
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (150)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT5:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				BitsPerPixel=32;
				BytesPerPixel=4;
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding*Depth])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (160)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, Depth, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			}
		}
	}
	//If 2D
	else
	{
		/*ImageType=IMAGETYPE_2DIMAGE;*/

		if(DDS_IsCompressed==FALSE)
		{
			BytesPerPixel=BitsPerPixel/8;

			//Could be
			//A8R8G8B8 = 32								<---
			//A1R5G5B5 = 16								<---
			//A4R4G4B4 = 16
			//R8G8B8 = 24								<---
			//R5G6B5 = 16
			//A2R10G10B10 = 32							<---
			//X8R8G8B8 = 32  (Same as A8R8G8B8)			<---
			//X1R5G5B5 = 16   (Same as A1R5G5B5)		<---
			//R3G3B2 = 8 (WTF???)
			//A8R3G3B2 = 16 (WTF???)
			//X4R4G4B4 = 16 (Same as A4R4G4B4)
			//A16R16G16B16 = 64							<---
			//The others are the DXT formats.

			//Check the masks to see where the BGRA values are
			//Just handle the typical arrangements else flag an error
			if(BitsPerPixel==32)
			{
				//I'm guessing for X formats, this flag is not present, but who cares...
				//if(header.DDSurfaceDesc2.ddpfPixelFormat.dwFlags & DDPF_ALPHAPIXELS)

				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
					//(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask		//Assume alpha is at 0xFF000000
				{
					//For A8R8G8B8 and X8R8G8B8
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
						goto ExitOnError0_MY;

					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (100)");
						goto ExitOnError2_MY;
					}
				}
				else if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x000003FF)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000FFC00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x3FF00000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0xC0000000))
				{
					//For A2R10G10B10
					//This is an odd format. Our TGraphicsFile doesn't support manipulation
					//of this kind of format. Convert it to BGRA8.
					//PS: We consider this as BGRA, not ARGB. Same thing basically.
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
						goto ExitOnError0_MY;

					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (101)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A2B10G10R10_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (443)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==16)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00007C00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x000003E0)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x0000001F))
				//	(header.DDSurfaceDesc2.ddpfPixelFormat.dwRGBAlphaBitMask == 0x00008000))
				{
					//For A1R5G5B5 and X1R5G5B5
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (100)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A1R5G5B5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (1289)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==24)
			{
				if((header.DDSurfaceDesc2.ddpfPixelFormat.dwRBitMask == 0x00FF0000)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwGBitMask == 0x0000FF00)&&
					(header.DDSurfaceDesc2.ddpfPixelFormat.dwBBitMask == 0x000000FF))
				{
					//For R8G8B8
					TotalColorData=Width*BytesPerPixel*Height;
					ScanLinePadded=((Width*BytesPerPixel+3)/4)*4;
					TotalColorDataWithPadding=ScanLinePadded*Height;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (100)");
						goto ExitOnError2_MY;
					}

					BitsPerPixel=24;
					BytesPerPixel=3;
				}
				else
				{
					strcpy(ErrorMessage, "Data format is not recognized. (526)");
					goto ExitOnError2_MY;
				}
			}
			else if(BitsPerPixel==64)
			{
				//This is a RGBA16 format.
				//The masks in header.DDSurfaceDesc2.ddpfPixelFormat are all 0.
				{
					//For RGBA16
					TotalColorData=Width*BytesPerPixel*Height;
					TotalColorDataWithPadding=TotalColorData;
					//Read the color data
					if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
						goto ExitOnError0_MY;
					
					if((size=fread(pColorData, 1, TotalColorDataWithPadding, hfile))!=TotalColorDataWithPadding)
					{
						strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (1110)");
						goto ExitOnError2_MY;
					}

					colorData=NULL;
					Convert_A16B16G16R16_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData);
					delete [] pColorData;
					pColorData=colorData;
					BitsPerPixel=32;
					BytesPerPixel=4;
					TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				}
			}
		}
		else if(DDS_IsCompressed==TRUE)
		{	//DX1 to 5 compression method used
			//In each case, calculate size of the compressed image, read the bytes,
			//then call appropriate decompressor to convert to BGRA8 format.
			//
			//size = max(1, width ÷ 4) x max(1, height ÷ 4) x 8(DXT1) or 16(DXT2-5)
			switch(DDS_CompressionMethod)
			{
			case DDS_COMPRESSION_DXT1:
				//TotalColorData is for the compresse size
				TotalColorData=(Width*Height*4)/8;		//(Width*Height) is number of pixels, (Width*Height*4) is bit count
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (120)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				colorData=NULL;
				if(ConvertDXT1_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT2:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (130)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT3:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (140)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT2_DXT3_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT4:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (3317)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, true)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			case DDS_COMPRESSION_DXT5:
				//TotalColorData is for the compresse size
				TotalColorData=(Width/4*Height/4)*16;		// /4 amd *16 cancel out
				TotalColorDataWithPadding=TotalColorData;
				
				//Read the color data
				if((pColorData=new uchar[TotalColorDataWithPadding])==NULL)
					goto ExitOnError0_MY;

				size=fread(pColorData, 1, TotalColorDataWithPadding, hfile);
				if(size!=TotalColorDataWithPadding)
				{
					strcpy(ErrorMessage, "Could not read all of the data section of this DDS file.\nFile may be corrupt. (3355)");
					goto ExitOnError2_MY;
				}

				//Decompress pColorData into colorData.
				if(ConvertDXT4_DXT5_To_BGRA8(ImageType, Width, Height, 0, pColorData, colorData, false)==-1)
				{
					goto ExitOnError2_MY;
				}

				//Should we keep the compressed version?
				if(IsKeepDDSCompressedFormat)
				{
					TotalColorDataCompressed=TotalColorDataWithPadding;
					pColorDataCompressed=pColorData;
					pColorData=colorData;
				}
				else
				{
					delete [] pColorData;
					pColorData=colorData;
				}
				BitsPerPixel=32;
				BytesPerPixel=4;
				TotalColorDataWithPadding=TotalColorData=Width*Height*BytesPerPixel;
				break;
			}
		}
	}

	ScanLinePadded=((Width*BytesPerPixel+3)/4)*4;
	PaddingBytes=ScanLinePadded-Width*BytesPerPixel;
	CurrentFrame=0;
	if(ScanLinePadded%4==0)
		IsImageDWORDaligned=TRUE;
	else
		IsImageDWORDaligned=FALSE;
					
	if(IsImageRGBOrBGR==TRUE)
		GeneralizedSwapRedAndBlueComponent();
	if(IsFlipImageOn==TRUE)
		FlipTopDownWiseTheImage();

	i=DataAlignment;		//What user wants
	DataAlignment=4;		//Currently
	ReturnVal=SetDataAlignmentOfColorData(1, i);
	if(ReturnVal==0)
		goto ExitOnError0;

	fclose(hfile);
	return 1;
	ExitOnError2_MY:
	fclose(hfile);
	return 2;
	ExitOnError0_MY:
	strcpy(ErrorMessage, "A memory error occured. (RFD 999)");
	fclose(hfile);
	return 0;
}