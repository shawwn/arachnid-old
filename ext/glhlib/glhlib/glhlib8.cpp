// glhlib8.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
#include <GL/gl.h>
//#include <GL/glext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"
#include "MemoryManagement.h"
#include <stdlib.h>



#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'


#define REGKEY_PROCESSOR		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"
#define REGKEY_PROCESSOR_NAMESTRING		"ProcessorNameString"
#define REGKEY_PROCESSOR_VENDOR			"VendorIdentifier"
#define REGKEY_PROCESSOR_SPEED			"~MHz"







//PURPOSE:	This function queries BOOL value in the registry using the
//			hkey and szSubKey as the registry key info.  If the value is not
//			found in the registry, it is added with a zero value.
//PARAMETERS:
//			[in] hkey : handle to a registry key
//			[in] pSubKey : pointer to a subkey string
//			[out] pValue
void RegQueryString(HKEY hkey, schar *pSubKey, schar *pValue)
{
	uint rc;
	ulong length;
	ulong dwType;
	uchar cbuffer1024[1024];
	length=1024;

	rc=(uint)RegQueryValueEx(hkey, pSubKey, NULL, &dwType, cbuffer1024, &length);
	if(rc==ERROR_SUCCESS)
	{
		strcpy(pValue, (const schar *)cbuffer1024);
	}
	else
		pValue[0]='\0';
}

void RegQuerySINT(HKEY hkey, schar *pSubKey, sint *pValue)
{
	uint rc;
	ulong length;
	ulong dwType;
	sint data=0;
	length=sizeof(sint);

	rc=(uint)RegQueryValueEx(hkey, pSubKey, NULL, &dwType, (uchar *)&data, &length);
	if(rc==ERROR_SUCCESS)
	{
		*pValue=data;
	}
	else
		*pValue=0;
}

void glhGetProcessorInfo(schar *pVendor, schar *pName, schar *pSpeedInMHz)
{
	uint rc;
	HKEY hKey;
	sint data;

	if((pVendor==NULL)&&(pName==NULL)&&(pSpeedInMHz==NULL))
		return;

	pVendor[0]=pName[0]=pSpeedInMHz[0]='\0';

	rc=(uint)RegOpenKeyEx(HKEY_LOCAL_MACHINE, REGKEY_PROCESSOR, 0, KEY_QUERY_VALUE, &hKey);
	if(rc==ERROR_SUCCESS)
	{
		if(pVendor)
			RegQueryString(hKey, REGKEY_PROCESSOR_VENDOR, pVendor);
		if(pName)
			RegQueryString(hKey, REGKEY_PROCESSOR_NAMESTRING, pName);
		if(pSpeedInMHz)
		{
			RegQuerySINT(hKey, REGKEY_PROCESSOR_SPEED, &data);
			ConvertSINTIntoString(data, pSpeedInMHz);
			strcat(pSpeedInMHz, " MHz");
		}
		RegCloseKey(hKey);
	}
}




sint glhIdentifyVendor()
{
	//Among others, these are some of them
 	//ATI Technologies Inc.
	//3Dfx Interactive Inc.
	//Matrox Graphics Inc.
	//NVIDIA Corporation
	//S3 Graphics
	//S3 Incorporated
	//SIS
	//SiS AGP
	//SiS DMA
	//Trident
	//XGI Technology Inc.
	//Diamond Multimedia

	//I think I don't need to make lower case
	//Companies rarely change their GL_VENDOR string
	const schar *ccc=(const schar *)glGetString(GL_VENDOR);

	if(CompareTwoStrings(ccc, (schar *)"ATI Technologies", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_ATI;
	if(CompareTwoStrings(ccc, (schar *)"NVIDIA", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_NVIDIA;
	if(CompareTwoStrings(ccc, (schar *)"INTEL", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_INTEL;
	if(CompareTwoStrings(ccc, (schar *)"SIS", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_SIS;
	if(CompareTwoStrings(ccc, (schar *)"SiS", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_SIS;
	if(CompareTwoStrings(ccc, (schar *)"3Dfx", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_3DFX;
	if(CompareTwoStrings(ccc, (schar *)"3DLABS", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_3DLABS;
	if(CompareTwoStrings(ccc, (schar *)"Diamond Multimedia", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_DIAMONDMULTIMEDIA;
	if(CompareTwoStrings(ccc, (schar *)"ELSA", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_ELSA;
	if(CompareTwoStrings(ccc, (schar *)"Imagination Technologies", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_IMAGINATIONTECHNOLOGIES;
	if(CompareTwoStrings(ccc, (schar *)"Matrox", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_MATROX;
	if(CompareTwoStrings(ccc, (schar *)"Microsoft", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_MICROSOFT;
	if(CompareTwoStrings(ccc, (schar *)"S3 Graphics", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_S3;
	if(CompareTwoStrings(ccc, (schar *)"S3 Incorporated", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_S3;
	if(CompareTwoStrings(ccc, (schar *)"XGI Technology", FALSE, 0, -1, 0, -1)==1)
		return VENDOR_XGI;

	return VENDOR_UNKNOWN;
}



sint glhComputeFrameBufferUsage(sint *memoryCount, sint width, sint height, sint includeFrontBuffer, sint bitsPerPixel_Color_FrontBuffer, sint bitsPerPixel_Color, sint depthAndStencilTogether, sint depthBufferBits, sint stencilBufferBits, sint backBufferCount)
{
	sint byteCount;
	sint otherbuffer1, otherbuffer2;

	if(includeFrontBuffer)
	{
		if((bitsPerPixel_Color_FrontBuffer!=16)&&(bitsPerPixel_Color_FrontBuffer!=24)&&(bitsPerPixel_Color_FrontBuffer!=32))
			return -1;
	}

	if((bitsPerPixel_Color!=16)&&(bitsPerPixel_Color!=24)&&(bitsPerPixel_Color!=32))
		return -1;

	if((depthBufferBits!=16)&&(depthBufferBits!=24)&&(depthBufferBits!=32))
		return -1;

	if((stencilBufferBits!=0)&&(stencilBufferBits!=8)&&(stencilBufferBits!=16)&&(stencilBufferBits!=24)&&(stencilBufferBits!=32))
		return -1;

	if((depthBufferBits==24)&&(depthAndStencilTogether==FALSE))
		otherbuffer1=32;
	else
		otherbuffer1=depthBufferBits;

	if((depthBufferBits==24)&&(depthAndStencilTogether==TRUE)&&(stencilBufferBits==8))
		otherbuffer2=0;
	else
		otherbuffer2=stencilBufferBits;

	if(backBufferCount==0)
		backBufferCount++;

	byteCount=(bitsPerPixel_Color+otherbuffer1+otherbuffer2)/8;
	*memoryCount=byteCount*width*height*backBufferCount;

	if(includeFrontBuffer)
		*memoryCount+=(bitsPerPixel_Color_FrontBuffer/8)*width*height;

	return 1;
}


