// glhlib6.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/wglext.h>
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
#include "TTorus.h"
#include "TTube.h"
#include <stack>




#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'



using namespace std;



//INTERNAL VARIABLES, CLASSES, STRUCTS
//sint	ErrorState;			//Record error if one occurs. I will no longer use this. I will instead use a stack with max depth of 100
extern sint			TotalErrorStateStack;
extern stack<sint>	ErrorStateStack;




//PURPOSE:	Just a wrapper for my 3DGraphicsLibrary function.
sint glhDoesRayCrossTrianglef(sreal *rayPoint1, sreal *rayDirection, sreal rayDirectionLengthSquared,
												sreal *trianglePoint1, sreal *trianglePoint2,
												sreal *triangleVector1, sreal *triangleVector2,
												sreal triangleVector1LengthSquared, sreal triangleVector2LengthSquared,
												sreal triangleVector1_DOT_triangleVector2,
												sreal *pointOfIntersection)
{
	return DoesRayCrossTriangle_6_FLOAT(rayPoint1, rayDirection, rayDirectionLengthSquared,
												trianglePoint1, trianglePoint2,
												triangleVector1, triangleVector2,
												triangleVector1LengthSquared, triangleVector2LengthSquared,
												triangleVector1_DOT_triangleVector2,
												pointOfIntersection);
}



void glhComputeReflectedVectorf(sreal *reflected, sreal *pincidentVector, sreal *pnormal, sint count)
{
	sint i;
	count*=3;		//XYZ
	for(i=0; i<count; i+=3)
	{	//This should be inlined
		ComputeReflectedVector_1(&reflected[i], &pincidentVector[i], &pnormal[i]);
	}
}

void glhComputeRefractedVectorf(sreal *refracted, sreal *pincidentVector, sreal *pnormal, sreal eta, sint count)
{
	sint i;
	count*=3;		//XYZ
	for(i=0; i<count; i+=3)
	{	//This should be inlined
		ComputeRefractedVector_1(&refracted[i], &pincidentVector[i], &pnormal[i], eta);
	}
}

void glhGeneralPlanarMirrorf_1(sreal *matrix, sreal *ptransformArray)
{
	glhLoadIdentityf2(matrix);

	//Move to mirror center
	glhTranslatef2(matrix, ptransformArray[0], ptransformArray[1], ptransformArray[2]);

	//Rotate about mirror center z
	//glhRotatef2(matrix, ptransformArray[5], 0.0, 0.0, 1.0);	//Don't use this, it's slower
	glhRotateAboutZf2(matrix, ptransformArray[5]);
	//Rotate about mirror center y
	//glhRotatef2(matrix, ptransformArray[4], 0.0, 1.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutYf2(matrix, ptransformArray[4]);
	//Rotate about mirror center x
	//glhRotatef2(matrix, ptransformArray[3], 1.0, 0.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutXf2(matrix, ptransformArray[3]);

	//Mirror about the xy-plane
	glhScalef2(matrix, 1.0, 1.0, -1.0);
	

	//Negative rotation about mirror center x
	//glhRotatef2(matrix, ptransformArray[3], -1.0, 0.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutXf2(matrix, -ptransformArray[3]);
	//Negative rotation about mirror center y
	//glhRotatef2(matrix, ptransformArray[4], 0.0, -1.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutYf2(matrix, -ptransformArray[4]);
	//Negative rotation about mirror center z
	//glhRotatef2(matrix, ptransformArray[5], 0.0, 0.0, -1.0);	//Don't use this, it's slower
	glhRotateAboutZf2(matrix, -ptransformArray[5]);

	//Negative move to mirror center
	glhTranslatef2(matrix, -ptransformArray[0], -ptransformArray[1], -ptransformArray[2]);
}


void glhGeneralPlanarMirrorf_2(sreal *matrix, sreal *ptranslate, sreal *pselfRotate)
{
	glhLoadIdentityf2(matrix);

	//Move to mirror center
	glhTranslatef2(matrix, ptranslate[0], ptranslate[1], ptranslate[2]);

	//Rotate about mirror center z
	//glhRotatef2(matrix, ptransformArray[5], 0.0, 0.0, 1.0);	//Don't use this, it's slower
	glhRotateAboutZf2(matrix, pselfRotate[2]);
	//Rotate about mirror center y
	//glhRotatef2(matrix, ptransformArray[4], 0.0, 1.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutYf2(matrix, pselfRotate[1]);
	//Rotate about mirror center x
	//glhRotatef2(matrix, ptransformArray[3], 1.0, 0.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutXf2(matrix, pselfRotate[0]);

	//Mirror about the xy-plane
	glhScalef2(matrix, 1.0, 1.0, -1.0);
	

	//Negative rotation about mirror center x
	//glhRotatef2(matrix, ptransformArray[3], -1.0, 0.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutXf2(matrix, -pselfRotate[0]);
	//Negative rotation about mirror center y
	//glhRotatef2(matrix, ptransformArray[4], 0.0, -1.0, 0.0);	//Don't use this, it's slower
	glhRotateAboutYf2(matrix, -pselfRotate[1]);
	//Negative rotation about mirror center z
	//glhRotatef2(matrix, ptransformArray[5], 0.0, 0.0, -1.0);	//Don't use this, it's slower
	glhRotateAboutZf2(matrix, -pselfRotate[2]);

	//Negative move to mirror center
	glhTranslatef2(matrix, -ptranslate[0], -ptranslate[1], -ptranslate[2]);
}

void glhRotateAboutXd2(sreal2 *matrix, sreal2 angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal2 m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	//rotate[ 0]=1.0;	//Not needed
	//rotate[ 1]=0.0;	//Not needed
	//rotate[ 2]=0.0;	//Not needed
	//rotate[ 3]=0.0;	//Not needed

	//rotate[ 4]=0.0;	//Not needed
	rotate[ 5]=cos(angleInRadians);
	rotate[ 6]=sin(angleInRadians);
	//rotate[ 7]=0.0;	//Not needed

	//rotate[ 8]=0.0;	//Not needed
	rotate[ 9]=-rotate[6];
	rotate[10]=rotate[5];
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	//matrix[0]=m[0];		//No change

	matrix[4]=m[4]*rotate[5]+
		m[8]*rotate[6];

	matrix[8]=m[4]*rotate[9]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	//matrix[1]=m[1];		//No change

	matrix[5]=m[5]*rotate[5]+
		m[9]*rotate[6];

	matrix[9]=m[5]*rotate[9]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	//matrix[2]=m[2];		//No change

	matrix[6]=m[6]*rotate[5]+
		m[10]*rotate[6];

	matrix[10]=m[6]*rotate[9]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	//matrix[3]=m[3];	//No change

	matrix[7]=m[7]*rotate[5]+
		m[11]*rotate[6];

	matrix[11]=m[7]*rotate[9]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}

void glhRotateAboutXf2(sreal *matrix, sreal angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	//rotate[ 0]=1.0;	//Not needed
	//rotate[ 1]=0.0;	//Not needed
	//rotate[ 2]=0.0;	//Not needed
	//rotate[ 3]=0.0;	//Not needed

	//rotate[ 4]=0.0;	//Not needed
	rotate[ 5]=cosf(angleInRadians);
	rotate[ 6]=sinf(angleInRadians);
	//rotate[ 7]=0.0;	//Not needed

	//rotate[ 8]=0.0;	//Not needed
	rotate[ 9]=-rotate[6];
	rotate[10]=rotate[5];
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	//matrix[0]=m[0];		//No change

	matrix[4]=m[4]*rotate[5]+
		m[8]*rotate[6];

	matrix[8]=m[4]*rotate[9]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	//matrix[1]=m[1];		//No change

	matrix[5]=m[5]*rotate[5]+
		m[9]*rotate[6];

	matrix[9]=m[5]*rotate[9]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	//matrix[2]=m[2];		//No change

	matrix[6]=m[6]*rotate[5]+
		m[10]*rotate[6];

	matrix[10]=m[6]*rotate[9]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	//matrix[3]=m[3];	//No change

	matrix[7]=m[7]*rotate[5]+
		m[11]*rotate[6];

	matrix[11]=m[7]*rotate[9]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}

void glhRotateAboutYd2(sreal2 *matrix, sreal2 angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal2 m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	rotate[ 0]=cos(angleInRadians);
	//rotate[ 1]=0.0;	//Not needed
	//rotate[ 2]=-sin(angleInRadians);		//Done later
	//rotate[ 3]=0.0;	//Not needed

	//rotate[ 4]=0.0;	//Not needed
	//rotate[ 5]=1.0;	//Not needed
	//rotate[ 6]=0.0;	//Not needed
	//rotate[ 7]=0.0;	//Not needed

	rotate[ 8]=sin(angleInRadians);
	//rotate[ 9]=0.0;	//Not needed
	rotate[10]=rotate[0];
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	rotate[ 2]=-rotate[8];

	matrix[0]=m[0]*rotate[0]+
		m[8]*rotate[2];

	//matrix[4]=m[4];	//No change

	matrix[8]=m[0]*rotate[8]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[9]*rotate[2];

	matrix[5]=m[5];		//No change

	matrix[9]=m[1]*rotate[8]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[10]*rotate[2];

	matrix[6]=m[6];		//No change

	matrix[10]=m[2]*rotate[8]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[11]*rotate[2];

	matrix[7]=m[7];		//No change

	matrix[11]=m[3]*rotate[8]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}

void glhRotateAboutYf2(sreal *matrix, sreal angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	rotate[ 0]=cosf(angleInRadians);
	//rotate[ 1]=0.0;	//Not needed
	//rotate[ 2]=-sin(angleInRadians);		//Done later
	//rotate[ 3]=0.0;	//Not needed

	//rotate[ 4]=0.0;	//Not needed
	//rotate[ 5]=1.0;	//Not needed
	//rotate[ 6]=0.0;	//Not needed
	//rotate[ 7]=0.0;	//Not needed

	rotate[ 8]=sinf(angleInRadians);
	//rotate[ 9]=0.0;	//Not needed
	rotate[10]=rotate[0];
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	rotate[ 2]=-rotate[8];

	matrix[0]=m[0]*rotate[0]+
		m[8]*rotate[2];

	//matrix[4]=m[4];	//No change

	matrix[8]=m[0]*rotate[8]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[9]*rotate[2];

	matrix[5]=m[5];		//No change

	matrix[9]=m[1]*rotate[8]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[10]*rotate[2];

	matrix[6]=m[6];		//No change

	matrix[10]=m[2]*rotate[8]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[11]*rotate[2];

	matrix[7]=m[7];		//No change

	matrix[11]=m[3]*rotate[8]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}

void glhRotateAboutZd2(sreal2 *matrix, sreal2 angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal2 m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	rotate[ 0]=cos(angleInRadians);
	rotate[ 1]=sin(angleInRadians);
	//rotate[ 2]=0.0;	//Not needed
	//rotate[ 3]=0.0;	//Not needed

	rotate[ 4]=-rotate[1];
	rotate[ 5]=rotate[0];
	//rotate[ 6]=0.0;	//Not needed
	//rotate[ 7]=0.0;	//Not needed

	//rotate[ 8]=0.0;	//Not needed
	//rotate[ 9]=0.0;	//Not needed
	//rotate[10]=1.0;	//Not needed
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	
	matrix[0]=m[0]*rotate[0]+
		m[4]*rotate[1];

	matrix[4]=m[0]*rotate[4]+
		m[4]*rotate[5];

	//matrix[8]=m[8];	//No change

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[5]*rotate[1];

	matrix[5]=m[1]*rotate[4]+
		m[5]*rotate[5];

	//matrix[9]=m[9];	//No change

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[6]*rotate[1];

	matrix[6]=m[2]*rotate[4]+
		m[6]*rotate[5];

	//matrix[10]=m[10];		//No change

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[7]*rotate[1];

	matrix[7]=m[3]*rotate[4]+
		m[7]*rotate[5];

	//matrix[11]=m[11];		//No change

	//matrix[15]=matrix[15];
}

void glhRotateAboutZf2(sreal *matrix, sreal angleInRadians)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal m[16], rotate[16];
	
	//Make a copy
	m[0]=matrix[0];
	m[1]=matrix[1];
	m[2]=matrix[2];
	m[3]=matrix[3];
	m[4]=matrix[4];
	m[5]=matrix[5];
	m[6]=matrix[6];
	m[7]=matrix[7];
	m[8]=matrix[8];
	m[9]=matrix[9];
	m[10]=matrix[10];
	m[11]=matrix[11];
	//m[12]=matrix[12];
	//m[13]=matrix[13];
	//m[14]=matrix[14];
	//m[15]=matrix[15];

	rotate[ 0]=cosf(angleInRadians);
	rotate[ 1]=sinf(angleInRadians);
	//rotate[ 2]=0.0;	//Not needed
	//rotate[ 3]=0.0;	//Not needed

	rotate[ 4]=-rotate[1];
	rotate[ 5]=rotate[0];
	//rotate[ 6]=0.0;	//Not needed
	//rotate[ 7]=0.0;	//Not needed

	//rotate[ 8]=0.0;	//Not needed
	//rotate[ 9]=0.0;	//Not needed
	//rotate[10]=1.0;	//Not needed
	//rotate[11]=0.0;		//Not needed
	//The last column of rotate[] is {0 0 0 1}

	
	matrix[0]=m[0]*rotate[0]+
		m[4]*rotate[1];

	matrix[4]=m[0]*rotate[4]+
		m[4]*rotate[5];

	//matrix[8]=m[8];	//No change

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[5]*rotate[1];

	matrix[5]=m[1]*rotate[4]+
		m[5]*rotate[5];

	//matrix[9]=m[9];	//No change

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[6]*rotate[1];

	matrix[6]=m[2]*rotate[4]+
		m[6]*rotate[5];

	//matrix[10]=m[10];		//No change

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[7]*rotate[1];

	matrix[7]=m[3]*rotate[4]+
		m[7]*rotate[5];

	//matrix[11]=m[11];		//No change

	//matrix[15]=matrix[15];
}


sint GetGLErrorQuickly()
{
	uint error=glGetError();
	switch(error)
	{
	case GL_INVALID_ENUM:
		return -1;
	case GL_INVALID_VALUE:
		return -2;
	case GL_INVALID_OPERATION:
		return -3;
	case GL_STACK_OVERFLOW:
		return -4;
	case GL_STACK_UNDERFLOW:
		return -5;
	case GL_OUT_OF_MEMORY:
		return -6;
	default:
		return 1;
	}
}

//*************************************************************************
//*************************************************************************
//This function will never be released since it is not reliable.
//glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &resident); and
//glAreTexturesResident are not reliable
//and the technique I used here, detecting when rendering becomes slow due to texture swapping
//is not reliable. Memory usage may go into the gigabytes using this method so it is risky.
//*************************************************************************
//*************************************************************************
//*************************************************************************
//*************************************************************************
//PURPOSE:	Call this to get the amount of video memory available for storing textures.
//			This is just an estimate.
//			A GL context must be current.
//			The algorithm will create 2D textures of size 512x512 RGBA8
//			Each texture consumes 1 MB of memory which is a good way to do an estimate.
//
//NOTE:		This function pushs the modelview and projection stack once.
//			It calls glDisable(GL_DEPTH_TEST);
//			glGenTextures, glDeleteTextures and other texture related functions.
//			It calls glVertexPointer, glTexCoordPointer
//			glEnableClientState(GL_VERTEX_POINTER);
//			glEnableClientState(GL_TEXCOORD_POINTER);
//			!!!!!!!!!!and you should disable the other arrays!!!!!!!!
//			It calls glEnable(GL_TEXTURE_2D) and then glDisable(GL_TEXTURE_2D)
//			!!!!!!!!!Make sure texture unit 0 is the active unit!!!!!!!!
//
//RETURNS:
//			Size in bytes if successful
//			-1 for GL_INVALID_ENUM
//			-2 for GL_INVALID_VALUE
//			-3 for GL_INVALID_OPERATION
//			-4 for GL_STACK_OVERFLOW
//			-5 for GL_STACK_UNDERFLOW
//			-6 for GL_OUT_OF_MEMORY
/*uint glhGetTextureMemoryCount()
{
	sint errorCode;
	sint i, j, k, loopCounter, texCounter, resident;
	uchar *pixels=NULL;
	schar errorMessage[512];
	sint totaltimerStatistics=0;
	sreal timerStatistics[128];			//Only need 64 of these
	sreal timerDelta[128];
	sreal startTime, endTime;
	sreal FTempo;

	sint totalTextureID=0, usedTextureID=0, textureIDExcess=512;
	uint *ptextureID=NULL;

	//A quad
	ushort pIndex[6];
	sreal pVertexBuffer[20];	//{XYZ, ST} x 4 = 20
	sreal depthValue;

	//glGetTexParameteriv may not always work, so I will use the performance timer to detect slowdowns
	//and see if it can do a better job. Probably not cause AGP memory can be very fast.

	GeneralGameTimerStruct generalGameTimer;
	if(InitializeGameTimer(errorMessage, generalGameTimer)==0)
	{
		errorCode=0;
		goto ExitOnError_GLTexMemory;
	}

	//A quad
	depthValue=-0.1;

	pIndex[0]=0;
	pIndex[1]=1;
	pIndex[2]=2;
	pIndex[3]=2;
	pIndex[4]=1;
	pIndex[5]=3;

	i=0;
	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=-1.0;
	pVertexBuffer[i++]=depthValue;
	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=0.0;

	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=depthValue;
	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=1.0;

	pVertexBuffer[i++]=-1.0;
	pVertexBuffer[i++]=-1.0;
	pVertexBuffer[i++]=depthValue;
	pVertexBuffer[i++]=0.0;
	pVertexBuffer[i++]=0.0;

	pVertexBuffer[i++]=-1.0;
	pVertexBuffer[i++]=1.0;
	pVertexBuffer[i++]=depthValue;
	pVertexBuffer[i++]=0.0;
	pVertexBuffer[i++]=1.0;

	glVertexPointer(3, GL_FLOAT, sizeof(sreal)*5, pVertexBuffer);
	glTexCoordPointer(2, GL_FLOAT, sizeof(sreal)*5, &pVertexBuffer[3]);

	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;


	pixels=new uchar[512*512*4];		//RGBA8
	//No need to fill pixels

	//Allocate 8 texture (8 MB)
	//Disable depth testing
	//Render quads using each texture

	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;

	glEnable(GL_TEXTURE_2D);

	loopCounter=0;
	while(usedTextureID<512)	//Until 512 texture (512MB) are created
	{					//The loop will run up to 64 times
		//Create 8 more textures
		AllocateMemory_1(ptextureID, &totalTextureID, &usedTextureID, textureIDExcess, 8);

		glGenTextures(8, &ptextureID[usedTextureID]);

		for(j=0, k=usedTextureID; j<8; j++, k++)
		{
			glBindTexture(GL_TEXTURE_2D, ptextureID[k]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			if((errorCode=GetGLErrorQuickly())!=1)
				goto ExitOnError_GLTexMemory;
		}
		usedTextureID+=8;		//Increment

		//Render X amount of quads, with each texture.
		startTime=GetTimeWithGameTimer(generalGameTimer);
		for(i=0; i<usedTextureID; i++)
		{
			glBindTexture(GL_TEXTURE_2D, ptextureID[i]);
			if((errorCode=GetGLErrorQuickly())!=1)
				goto ExitOnError_GLTexMemory;
			glDrawElements(GL_TRIANGLES, 4, GL_UNSIGNED_SHORT, pIndex);
			if((errorCode=GetGLErrorQuickly())!=1)
				goto ExitOnError_GLTexMemory;
		}
		endTime=GetTimeWithGameTimer(generalGameTimer);
		if(loopCounter<5)		//Just record
		{
			timerStatistics[totaltimerStatistics]=endTime-startTime;
			totaltimerStatistics++;
		}
		else
		{
			timerStatistics[totaltimerStatistics]=endTime-startTime;
			//Compare with previous
			//Built a kind of graph
			//If there is a sudden increase in time, then things have slowed down, maybe using AGP or hard disk
			//swap file.
			for(i=1; i<=totaltimerStatistics; i++)
			{
				timerDelta[i-1]=fabsf(timerStatistics[i]-timerStatistics[i-1]);
			}

			for(i=1; i<totaltimerStatistics; i++)
			{
				FTempo=timerDelta[i]/timerDelta[i-1];
				if(FTempo>23.0)		//timerDelta[i] is too high
				{
					//Previous texCounter should have a good value.
					//if(texCounter<usedTextureID)
					goto ExitOnDone_GLTexMemory;		//Oh Oh! Some texture are not resident!
				}
			}


			totaltimerStatistics++;
		}

		//Check to see if all texture are video memory resident according to GL drivers
		for(i=texCounter=0; i<usedTextureID; i++)
		{
			//We can either use this for many textures or we can use glGetTexParameter for single texture
			//glAreTexturesResident
			glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_RESIDENT, &resident);
			if((errorCode=GetGLErrorQuickly())!=1)
				goto ExitOnError_GLTexMemory;
			if(resident==GL_TRUE)
				texCounter++;

			//OLD
			//if(glIsTextureResident(ptextureID[i])==GL_TRUE)
			//	texCounter++;
		}

		if(texCounter<usedTextureID)
			break;		//Oh Oh! Some texture are not resident!

		loopCounter++;
		if(loopCounter>=64)
			break;		//Loop 64 times is enough for 512MB of textures
	}

ExitOnDone_GLTexMemory:

	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;

	glBindTexture(GL_TEXTURE_2D, 0);
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;
	//Delete all the GL textures
	for(i=0; i<usedTextureID; i++)
	{
		glDeleteTextures(1, &ptextureID[i]);
	}
	if((errorCode=GetGLErrorQuickly())!=1)
		goto ExitOnError_GLTexMemory;
	
	if(pixels)
		delete [] pixels;
	if(ptextureID)
		delete [] ptextureID;

	//Return the number of bytes of video memory for texture
	return texCounter*512*512*4;
ExitOnError_GLTexMemory:
	if(pixels)
		delete [] pixels;
	if(ptextureID)
		delete [] ptextureID;
	return errorCode;
}*/


//***** Some of this comes from
//http://www.gamedev.net/reference/articles/article1095.asp

//PURPOSE:	Set quat to {0, 0, 0, 1}, xyzw (NOT wxyz)
void glhQuaternionIdentityf(sreal *quat)
{
	//Any quat multiplied by identity quat will be unchanged.
	//Also, {0, 0, 0, 0} is addition identity quat
	quat[0]=quat[1]=quat[2]=0.0;		//xyz is a vector
	quat[3]=1.0;
}

//PURPOSE:	If quat is {0, 0, 0, 1}, returns 1 (TRUE), else 0 (FALSE)
sint glhQuaternionIsIdentityf(sreal *quat)
{
	if((quat[0]==0.0)&&(quat[1]==0.0)&&(quat[2]==0.0)&&(quat[3]==1.0))
		return TRUE;
	else
		return FALSE;
}

//PURPOSE:	If quat[3] == 1.0, returns 1 (TRUE), else 0 (FALSE)
sint glhQuaternionIsPuref(sreal *quat)
{
	if(quat[3]==1.0)
		return TRUE;
	else
		return FALSE;
}

//PURPOSE:	Calculate the conjugate of a quat.
//			xyz is simply negated. quat is xyzw
void glhQuaternionConjugatef(sreal *quat)
{
	quat[0]=-quat[0];
	quat[1]=-quat[1];
	quat[2]=-quat[2];
}

//PURPOSE:	Calculate the magnitude of a quat.
void glhQuaternionMagnitudef(sreal *magnitude, sreal *quat)
{
	magnitude[0]=sqrtf(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
}

//PURPOSE:	Calculate the magnitude squared of a quat.
void glhQuaternionMagnitudeSquaredf(sreal *magnitude, sreal *quat)
{
	magnitude[0]=quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3];
}

//PURPOSE:	Compute dot product between 2 quats.
void glhQuaternionDotProductf(sreal *result, sreal *quat1, sreal *quat2)
{
	result[0]=quat1[0]*quat2[0]+quat1[1]*quat2[1]+quat1[2]*quat2[2]+quat1[3]*quat2[3];
}

//PURPOSE:	Calculates the exponential of a quat.
//			quat is expected to be a pure quaternion. See glhQuaternionIsPuref.
void glhQuaternionExpf(sreal *outquat, sreal *quat)
{
	//Normalize
	sreal n=1.0/sqrtf(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
	quat[0]*=n;
	quat[1]*=n;
	quat[2]*=n;
	quat[3]*=n;

	//Convert a quaternion to a rotation axis and angle
	sreal sin_a_2;
	sreal cos_a=quat[3];
	sreal angle=acosf(cos_a)*2.0;
	sreal sin_a=sqrtf(1.0-cos_a*cos_a);
	if(fabsf(sin_a)<0.0005)
		sin_a=sin_a_2=1.0;
	else
		sin_a_2=1.0/sin_a;

	sreal axis[3];
	axis[0]=quat[0]*sin_a_2;
	axis[1]=quat[1]*sin_a_2;
	axis[2]=quat[2]*sin_a_2;

	//From D3D docs :
	//Given a pure quaternion defined by:
	//q = (0, theta * v);
	//
	//This method calculates the exponential result.
	//exp(Q) = (cos(theta), sin(theta) * v)
	sreal theta=sinf(angle*0.5);
	outquat[3]=cosf(theta);
	theta=sinf(theta);
	outquat[0]=theta*axis[0];
	outquat[1]=theta*axis[1];
	outquat[2]=theta*axis[2];
}

//PURPOSE:	Calculate the natural logarithm of a quat.
//			quat should be unit quat.
void glhQuaternionLnf(sreal *outquat, sreal *quat)
{
	//From D3D docs:
	//A unit quaternion, is defined by:
	//Q == (cos(theta), sin(theta) * v) where |v| = 1
	//The natural logarithm of Q is, ln(Q) = (0, theta * v)
	sreal theta=acosf(quat[3]);
	sreal sin_theta_2=1.0/sinf(theta);
	outquat[0]=quat[0]*sin_theta_2*theta;
	outquat[1]=quat[1]*sin_theta_2*theta;
	outquat[2]=quat[2]*sin_theta_2*theta;
	outquat[3]=0.0;
}

//PURPOSE:	Normalize quat. Some glhQuaternion***** require normalized quats for efficiency.
void glhQuaternionNormalizef(sreal *quat)
{
	sreal n=1.0/sqrtf(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]+quat[3]*quat[3]);
	quat[0]*=n;
	quat[1]*=n;
	quat[2]*=n;
	quat[3]*=n;
}

//PURPOSE:	outquat = quat1 * quat2
//			If quat1 and quat2 are normalized, outquat will be a unit quat.
void glhQuaternionMultiplyf(sreal *outquat, sreal *quat1, sreal *quat2)
{
	//x = w1*x2 + w2*x1 + y1*z2 - z1*y2
	outquat[0]=quat1[3]*quat2[0]+quat2[3]*quat1[0]+quat1[1]*quat2[2]-quat1[2]*quat2[1];
	//y = w1*y2 + w2*y1 + z1*x2 - x1*z2
	outquat[1]=quat1[3]*quat2[1]+quat2[3]*quat1[1]+quat1[2]*quat2[0]-quat1[0]*quat2[2];
	//z = w1*z2 + w2*z1 + x1*y2 - y1*x2
	outquat[2]=quat1[3]*quat2[2]+quat2[3]*quat1[2]+quat1[0]*quat2[1]-quat1[1]*quat2[0];
	//w = w1*w2 - v1*v2
	outquat[3]=quat1[3]*quat2[3]-quat1[0]*quat2[0]-quat1[1]*quat2[1]-quat1[2]*quat2[2];
}

//PURPOSE:	Build a 4x4 column major rotation matrix from a quat.
//			quat should be normalized. See glhQuaternionNormalizef.
void glhQuaternionQuatToRotationMatrixf(sreal *matrix, sreal *quat)
{
	//    |       2     2                                         |
	//    | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW          0   |
	//    |                                                       |
	//    |                       2     2                         |
	//M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW          0   |
	//    |                                                       |
	//    |                                      2     2          |
	//    | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y       0   |
	//    |                                                       |
	//    |                                                       |
	//    |   0                0              0               1   |

	sreal twoXSquared=2.0*quat[0]*quat[0];
	sreal twoYSquared=2.0*quat[1]*quat[1];
	sreal twoZSquared=2.0*quat[2]*quat[2];
	
	sreal twoXY=2.0*quat[0]*quat[1];
	sreal twoXW=2.0*quat[0]*quat[3];
	sreal twoYW=2.0*quat[1]*quat[3];
	sreal twoZW=2.0*quat[2]*quat[3];
	sreal twoXZ=2.0*quat[0]*quat[2];
	sreal twoYZ=2.0*quat[1]*quat[2];
	

	matrix[0]=1.0-twoYSquared-twoZSquared;
	matrix[1]=twoXY+twoZW;
	matrix[2]=twoXZ-twoYW;
	matrix[3]=0.0;

	matrix[4]=twoXY-twoZW;
	matrix[5]=1.0-twoXSquared-twoZSquared;
	matrix[6]=twoYZ+twoXW;
	matrix[7]=0.0;

	matrix[8]=twoXZ+twoYW;
	matrix[9]=twoYZ-twoXW;
	matrix[10]=1.0-twoXSquared-twoYSquared;
	matrix[11]=0.0;

	matrix[12]=matrix[13]=matrix[14]=0.0;
	matrix[15]=1.0;
}

//PURPOSE:	Build a 4x4 column major rotation matrix from a quat.
//			quat should be normalized. See glhQuaternionNormalizef.
void glhQuaternionQuatToRotationMatrixTf(sreal *matrix, sreal *quat)
{
	//    |       2     2                                         |
	//    | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW          0   |
	//    |                                                       |
	//    |                       2     2                         |
	//M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW          0   |
	//    |                                                       |
	//    |                                      2     2          |
	//    | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y       0   |
	//    |                                                       |
	//    |                                                       |
	//    |   0                0              0               1   |

	sreal twoXSquared=2.0*quat[0]*quat[0];
	sreal twoYSquared=2.0*quat[1]*quat[1];
	sreal twoZSquared=2.0*quat[2]*quat[2];
	
	sreal twoXY=2.0*quat[0]*quat[1];
	sreal twoXW=2.0*quat[0]*quat[3];
	sreal twoYW=2.0*quat[1]*quat[3];
	sreal twoZW=2.0*quat[2]*quat[3];
	sreal twoXZ=2.0*quat[0]*quat[2];
	sreal twoYZ=2.0*quat[1]*quat[2];
	

	matrix[0]=1.0-twoYSquared-twoZSquared;
	matrix[1]=twoXY-twoZW;
	matrix[2]=twoXZ+twoYW;
	matrix[3]=0.0;

	matrix[4]=twoXY+twoZW;
	matrix[5]=1.0-twoXSquared-twoZSquared;
	matrix[6]=twoYZ-twoXW;
	matrix[7]=0.0;

	matrix[8]=twoXZ-twoYW;
	matrix[9]=twoYZ+twoXW;
	matrix[10]=1.0-twoXSquared-twoYSquared;
	matrix[11]=0.0;

	matrix[12]=matrix[13]=matrix[14]=0.0;
	matrix[15]=1.0;
}


//PURPOSE:	Build a quat from a 4x4 column major rotation matrix.
//			quat will be automatically normalized.
void glhQuaternionRotationMatrixToQuatf(sreal *quat, sreal *matrix)
{
	//This comes from matrixfaq.htm
	//It uses a row major format and matrix is linear (matrix[16])
	//I had to change the indices to be GL compatible
	sreal trace, S;

	/*//Calculate trace of matrix
	trace=matrix[0]+matrix[5]+matrix[10]+1.0;

	//If the trace of the matrix is greater than zero, then
	//perform an "instant" calculation.
	if(trace>0.0)
	{
		S=0.5/sqrtf(trace);
		quat[0]=(matrix[6]-matrix[9])*S;
		quat[1]=(matrix[8]-matrix[2])*S;
		quat[2]=(matrix[1]-matrix[4])*S;
		quat[3]=0.25/S;
	}
	else //if(trace<=0.0)
	{
		//Identify which major diagonal element has the greatest value.
		//The major diagonal of a matrix is the set of elements where the
		//row number is equal to the column number.
		//We only care about column 0, 1 and 2
		if(matrix[0]>matrix[5])
		{
			if(matrix[0]>matrix[10])
			{
				//matrix[0] is largest
				//Column 0
				S=1.0/sqrtf(1.0+matrix[0]-matrix[5]-matrix[10])*2.0;

				quat[0]=0.5/S;
				quat[1]=(matrix[1]+matrix[4])*S;
				quat[2]=(matrix[8]+matrix[2])*S;
				quat[3]=(matrix[6]+matrix[9])*S;
			}
			else
			{
				//matrix[10] is largest
				//Column 2
				S=1.0/sqrtf(1.0+matrix[10]-matrix[0]-matrix[5])*2.0;

				quat[0]=(matrix[8]+matrix[2])*S;
				quat[1]=(matrix[6]+matrix[9])*S;
				quat[2]=0.5/S;
				quat[3]=(matrix[1]+matrix[4])*S;
			}
		}
		else if(matrix[5]>matrix[10])
		{
			//matrix[5] is largest
			//Column 1
			S=1.0/sqrtf(1.0+matrix[5]-matrix[0]-matrix[10])*2.0;

			quat[0]=(matrix[1]+matrix[4])*S;
			quat[1]=0.5/S;
			quat[2]=(matrix[6]+matrix[9])*S;
			quat[3]=(matrix[8]+matrix[2])*S;
		}
		else
		{
			//matrix[10] is largest
			//Column 2
			S=1.0/sqrtf(1.0+matrix[10]-matrix[0]-matrix[5])*2.0;

			quat[0]=(matrix[8]+matrix[2])*S;
			quat[1]=(matrix[6]+matrix[9])*S;
			quat[2]=0.5/S;
			quat[3]=(matrix[1]+matrix[4])*S;
		}
	}*/

	//This is from http://www.gamasutra.com/features/19980703/quaternions_01.htm
	//which appears to give result consistent with D3DX library and
	//also I can confirm by using the inverse operation --> glhQuaternionQuatToRotationMatrixf

	//Calculate trace of matrix
	trace=matrix[0]+matrix[5]+matrix[10];

	//If the trace of the matrix is greater than zero, then
	//perform an "instant" calculation.
	if(trace>0.0)
	{
		S=0.5/sqrtf(trace+1.0);
		quat[0]=(matrix[6]-matrix[9])*S;
		quat[1]=(matrix[8]-matrix[2])*S;
		quat[2]=(matrix[1]-matrix[4])*S;
		quat[3]=0.25/S;
	}
	else //if(trace<=0.0)
	{
		//Identify which major diagonal element has the greatest value.
		//The major diagonal of a matrix is the set of elements where the
		//row number is equal to the column number.
		//We only care about column 0, 1 and 2

		// I'm gone recode this part for efficiency
		/*sint i=0, j, k;
		if(matrix[5]>matrix[0]) i=1;
		if(matrix[10]>matrix[i]) i=2;

		sint nxt[]={1, 2, 0};
		
		j=nxt[i];
		k=nxt[j];

		S=sqrtf(1.0+matrix[i*4+i]-matrix[j*4+j]-matrix[k*4+k]);

		sreal q[4];
		q[i]=S*0.5;
		
		if(S!=0.0) S=0.5/S;

		q[3]=(matrix[j*4+k]-matrix[k*4+j])*S;
		q[j]=(matrix[i*4+j]+matrix[j*4+i])*S;
		q[k]=(matrix[i*4+k]+matrix[k*4+i])*S;

		quat[0]=q[0];
		quat[1]=q[1];
		quat[2]=q[2];
		quat[3]=q[3];*/

		if(matrix[0]>matrix[5])
		{
			if(matrix[0]>matrix[10])
			{
				S=sqrtf(1.0+matrix[0]-matrix[5]-matrix[10]);

				quat[0]=S*0.5;
				if(S!=0.0) S=0.5/S;
				quat[1]=(matrix[1]+matrix[4])*S;
				quat[2]=(matrix[2]+matrix[8])*S;
				quat[3]=(matrix[6]-matrix[9])*S;
			}
			else
			{
				S=sqrtf(1.0+matrix[10]-matrix[0]-matrix[5]);

				quat[2]=S*0.5;
				if(S!=0.0) S=0.5/S;
				quat[0]=(matrix[8]+matrix[2])*S;
				quat[1]=(matrix[9]+matrix[6])*S;
				quat[3]=(matrix[1]-matrix[4])*S;
			}
		}
		else if(matrix[5]>matrix[10])
		{
			S=sqrtf(1.0+matrix[5]-matrix[10]-matrix[0]);

			quat[1]=S*0.5;
			if(S!=0.0) S=0.5/S;
			quat[3]=(matrix[8]-matrix[2])*S;
			quat[2]=(matrix[6]+matrix[9])*S;
			quat[0]=(matrix[4]+matrix[1])*S;
		}
		else
		{
			S=sqrtf(1.0+matrix[10]-matrix[0]-matrix[5]);

			quat[2]=S*0.5;
			if(S!=0.0) S=0.5/S;
			quat[0]=(matrix[8]+matrix[2])*S;
			quat[1]=(matrix[9]+matrix[6])*S;
			quat[3]=(matrix[1]-matrix[4])*S;
		}
	}
}


//PURPOSE:	Convert quat to a axis of rotation and an angle in radians.
//			axis will receive xyz.
//			Quat should be normalized.
void glhQuaternionQuatToAxisAndAnglef(sreal *axis, sreal *angleInRadians, sreal *quat)
{
	sreal scale=1.0/sqrtf(quat[0]*quat[0]+quat[1]*quat[1]+quat[2]*quat[2]);
	axis[0]=quat[0]*scale;
	axis[1]=quat[1]*scale;
	axis[2]=quat[2]*scale;

	angleInRadians[0]=2.0*acosf(quat[3]);
}

//PURPOSE:	Convert axis of rotation and an angle in radians to quat.
//			axis must be xyz. Should be unit vector.
void glhQuaternionAxisAndAngleToQuatf(sreal *quat, sreal *axis, sreal *angleInRadians)
{
	sreal thetaDivTwo=angleInRadians[0]*0.5;
	sreal sinThetaDivTwo=angleInRadians[0]*0.5;

	quat[0]=axis[0]*sinThetaDivTwo;
	quat[1]=axis[1]*sinThetaDivTwo;
	quat[2]=axis[2]*sinThetaDivTwo;
	quat[3]=cosf(thetaDivTwo);
}

//PURPOSE:	Convert Euler angles into a quat. Angles are in radians.
void glhQuaternionEulerAnglesToQuatf(sreal *quat, sreal roll, sreal pitch, sreal yaw)
{
	sreal cr, cp, cy, sr, sp, sy, cpcy, spsy;
	sreal roll_half, pitch_half, yaw_half;

	//Calculate trig identities
	roll_half=roll*0.5;
	yaw_half=yaw*0.5;
	pitch_half=pitch*0.5;

	cr=cosf(roll_half);
	cp=cosf(pitch_half);
	cy=cosf(yaw*0.5);

	sr=sinf(roll_half);
	sp=sinf(pitch_half);
	sy=sinf(yaw_half);
	
	cpcy=cp*cy;
	spsy=sp*sy;

	quat[3]=cr*cpcy + sr*spsy;
	quat[0]=sr*cpcy - cr*spsy;
	quat[1]=cr*sp*cy + sr*cp*sy;
	quat[2]=cr*cp*sy - sr*sp*cy;
}

//PURPOSE:	Interpolates between two quaternions, using spherical linear interpolation.
//			SLERP = Spherical Linear intERPolation
//			Interpolate 2 quats along smallest arc between them.
//			t is the interpolation factor where t = 0.0 gives outquat = quat1
//			and t = 1.0 gives outquat = quat2
//			t should be [0, 1]
//			You should make sure quat1 and quat2 are
void glhQuaternionSLERPf(sreal *outquat, sreal *quat1, sreal *quat2, sreal t)
{
	sreal omega, cosom, sinom, scale0, scale1;
	sreal cosom2;

	//Calc cosine. quat1 DOT quat2
	cosom=quat1[0]*quat2[0]+quat1[1]*quat2[1]+quat1[2]*quat2[2]+quat1[3]*quat2[3];

	//Adjust signs (if necessary)
	if(cosom<0.0)
	{
		cosom2=-cosom;
	}
	else
	{
		cosom2=cosom;
	}

	//Calculate coefficients
	if((1.0-cosom2)>0.00005)
	{
		//Standard case (slerp)
		omega=acosf(cosom2);
		sinom=sinf(omega);
		scale0=sinf((1.0-t)*omega)/sinom;
		scale1=sinf(t*omega)/sinom;
	}
	else
	{
		//"from" and "to" quaternions are very close 
		//... so we can do a linear interpolation
		scale0=1.0-t;
		scale1=t;
	}

	//Calculate final values
	if(cosom<0.0)
	{
		outquat[0]=scale0*quat1[0]-scale1*quat2[0];
		outquat[1]=scale0*quat1[1]-scale1*quat2[1];
		outquat[2]=scale0*quat1[2]-scale1*quat2[2];
		outquat[3]=scale0*quat1[3]-scale1*quat2[3];
	}
	else
	{
		outquat[0]=scale0*quat1[0]+scale1*quat2[0];
		outquat[1]=scale0*quat1[1]+scale1*quat2[1];
		outquat[2]=scale0*quat1[2]+scale1*quat2[2];
		outquat[3]=scale0*quat1[3]+scale1*quat2[3];
	}
}

void glhPackFloatIntoUByte(uchar *outputNumber, sreal inputNumber)
{
	sreal packFactors[]={1.0, 255.0, 65535.0, 16777215.0};	//Better than { 1.0, 256.0, 65536.0, 16777216.0 }?
	sreal Float[4];
	Float[1]=Float[2]=Float[3]=Float[0]=inputNumber;	//Store copies

	//MUL	float, float, packFactors;
	Float[0]=Float[0]*packFactors[0];
	Float[1]=Float[1]*packFactors[1];
	Float[2]=Float[2]*packFactors[2];
	Float[3]=Float[3]*packFactors[3];

	//FRC	ub, Float;
	outputNumber[0]=Float[0]-floorf(Float[0]);
	outputNumber[1]=Float[1]-floorf(Float[1]);
	outputNumber[2]=Float[2]-floorf(Float[2]);
	outputNumber[3]=Float[3]-floorf(Float[3]);
}

void glhUnpackUByteIntoFloat(sreal *outputNumber, const uchar *inputNumber)
{
	sreal extract[]={1.0, 1.0/255.0, 1.0/65535.0, 1.0/16777215.0};	//Better than {1.0, 1.0/256.0, 1.0/65536.0, 1.0/16777216.0}?
	
	//DP4	float, ub, extract;
	*outputNumber=inputNumber[0]*extract[0]+inputNumber[1]*extract[1]+inputNumber[2]*extract[2]+inputNumber[3]*extract[3];
}



void glhShadowMatrixd2(sreal2 *matrix, sreal2 *planeEquation, sreal2 *lightPosition)
{
	sreal2 resultMatrix[16];
	sreal2 mymatrix[16];
	sreal2 dot;
	
	//Find dot product between light position vector and plane normal
	dot=planeEquation[0]*lightPosition[0] +
			planeEquation[1]*lightPosition[1] +
			planeEquation[2]*lightPosition[2] +
			planeEquation[3]*lightPosition[3];

	mymatrix[0]=dot-lightPosition[0]*planeEquation[0];
	mymatrix[4]=-lightPosition[0]*planeEquation[1];
	mymatrix[8]=-lightPosition[0]*planeEquation[2];
	mymatrix[12]=-lightPosition[0]*planeEquation[3];

	mymatrix[1]=-lightPosition[1]*planeEquation[0];
	mymatrix[5]=dot-lightPosition[1]*planeEquation[1];
	mymatrix[9]=-lightPosition[1]*planeEquation[2];
	mymatrix[13]=-lightPosition[1]*planeEquation[3];

	mymatrix[2]=-lightPosition[2]*planeEquation[0];
	mymatrix[6]=-lightPosition[2]*planeEquation[1];
	mymatrix[10]=dot-lightPosition[2]*planeEquation[2];
	mymatrix[14]=-lightPosition[2]*planeEquation[3];

	mymatrix[3]=-lightPosition[3]*planeEquation[0];
	mymatrix[7]=-lightPosition[3]*planeEquation[1];
	mymatrix[11]=-lightPosition[3]*planeEquation[2];
	mymatrix[15]=dot-lightPosition[3]*planeEquation[3];

	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, mymatrix);
	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhShadowMatrixf2(sreal *matrix, sreal *planeEquation, sreal *lightPosition)
{
	sreal resultMatrix[16];
	sreal mymatrix[16];
	sreal dot;
	
	//Find dot product between light position vector and plane normal
	dot=planeEquation[0]*lightPosition[0] +
			planeEquation[1]*lightPosition[1] +
			planeEquation[2]*lightPosition[2] +
			planeEquation[3]*lightPosition[3];

	mymatrix[0]=dot-lightPosition[0]*planeEquation[0];
	mymatrix[4]=-lightPosition[0]*planeEquation[1];
	mymatrix[8]=-lightPosition[0]*planeEquation[2];
	mymatrix[12]=-lightPosition[0]*planeEquation[3];

	mymatrix[1]=-lightPosition[1]*planeEquation[0];
	mymatrix[5]=dot-lightPosition[1]*planeEquation[1];
	mymatrix[9]=-lightPosition[1]*planeEquation[2];
	mymatrix[13]=-lightPosition[1]*planeEquation[3];

	mymatrix[2]=-lightPosition[2]*planeEquation[0];
	mymatrix[6]=-lightPosition[2]*planeEquation[1];
	mymatrix[10]=dot-lightPosition[2]*planeEquation[2];
	mymatrix[14]=-lightPosition[2]*planeEquation[3];

	mymatrix[3]=-lightPosition[3]*planeEquation[0];
	mymatrix[7]=-lightPosition[3]*planeEquation[1];
	mymatrix[11]=-lightPosition[3]*planeEquation[2];
	mymatrix[15]=dot-lightPosition[3]*planeEquation[3];

	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, mymatrix);
	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}


sint CheckForParameterErrors_Geometry(tbool check1, sreal width, tbool check2, sreal height, tbool check3, sreal length,
	GLH_INDEXFORMAT indexFormat)
{
	if(check1)
	{
		if(width<=0.0)
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}
	if(check2)
	{
		if(height<=0.0)
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}
	if(check3)
	{
		if(length<=0.0)
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}
	if((indexFormat!=GLH_INDEXFORMAT_16BIT)&&(indexFormat!=GLH_INDEXFORMAT_32BIT))
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


sint glhCreateCubef2(glhCubeObjectf2 *pcube)
{
	sint i, returnVal;

	if(pcube==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, pcube->Width, TRUE, pcube->Height, TRUE, pcube->Length,
		pcube->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((pcube->TexCoordStyle[i]!=0)&&(pcube->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}


	TCube cube;

	returnVal=cube.Startup2(pcube->IndexFormat, pcube->VertexFormat, pcube->Width, pcube->Height, pcube->Length,
		pcube->WidthDivision, pcube->HeightDivision, pcube->LengthDivision, (tbool)pcube->AverageNormals, pcube->TexCoordStyle[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		cube.DeallocateAllMemory();
		return -1;
	}


	//Write stuff to pcube
	pcube->TotalIndex=cube.UsedIndex;
	pcube->pIndex16Bit=cube.pIndex16Bit;
	cube.pIndex16Bit=NULL;
	pcube->pIndex32Bit=cube.pIndex32Bit;
	cube.pIndex32Bit=NULL;
	pcube->VertexCount=cube.VertexCount;

	switch(cube.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		pcube->pVertex=(sreal *)cube.pVertex_V;
		cube.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		pcube->pVertex=(sreal *)cube.pVertex_VT;
		cube.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		pcube->pVertex=(sreal *)cube.pVertex_VNT;
		cube.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		pcube->pVertex=(sreal *)cube.pVertex_VNT3;
		cube.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		pcube->pVertex=(sreal *)cube.pVertex_VNTT3T3;
		cube.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		pcube->pVertex=(sreal *)cube.pVertex_VNTT3;
		cube.pVertex_VNTT3=NULL;
		break;
	}

	pcube->Start_DrawRangeElements=cube.Start_DrawRangeElements;
	pcube->End_DrawRangeElements=cube.End_DrawRangeElements;

	cube.DeallocateAllMemory();
	return 1;
}

void DeleteCommonObject(glhCommonObjectf2 *common)
{
	if(common->pIndex16Bit)
	{
		delete [] common->pIndex16Bit;
		common->pIndex16Bit=NULL;
	}
	if(common->pIndex32Bit)
	{
		delete [] common->pIndex32Bit;
		common->pIndex32Bit=NULL;
	}
	if(common->pVertex)
	{
		delete [] common->pVertex;
		common->pVertex=NULL;
	}
}

sint glhDeleteCubef2(glhCubeObjectf2 *pcube)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(pcube);
	DeleteCommonObject(common);
	return 1;
}


sint glhCreateCylinderf2(glhCylinderObjectf2 *pcylinder)
{
	sint i, returnVal;

	if(pcylinder==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, pcylinder->RadiusA, TRUE, pcylinder->RadiusB, TRUE, pcylinder->Height,
		pcylinder->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((pcylinder->TexCoordStyle[i]!=0)&&(pcylinder->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(pcylinder->Stacks<=0)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(pcylinder->Slices<=2)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TCylinder cylinder;

	returnVal=cylinder.StartupByTwoRadiusStacksAndSlices2(pcylinder->IndexFormat, pcylinder->VertexFormat,
		(pcylinder->IsThereATop) ? TRUE : FALSE, (pcylinder->IsThereABottom) ? TRUE : FALSE,
		pcylinder->RadiusA, pcylinder->RadiusB, pcylinder->Height, pcylinder->Stacks, pcylinder->Slices,
		pcylinder->TexCoordStyle[0], pcylinder->ScaleFactorS[0], pcylinder->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		cylinder.DeallocateAllMemory();
		return -1;
	}


	//Write stuff to pcylinder
	pcylinder->TotalIndex=cylinder.UsedIndex;
	pcylinder->pIndex16Bit=cylinder.pIndex16Bit;
	cylinder.pIndex16Bit=NULL;
	pcylinder->pIndex32Bit=cylinder.pIndex32Bit;
	cylinder.pIndex32Bit=NULL;
	pcylinder->VertexCount=cylinder.VertexCount;

	switch(cylinder.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_V;
		cylinder.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_VT;
		cylinder.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_VNT;
		cylinder.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_VNT3;
		cylinder.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_VNTT3T3;
		cylinder.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		pcylinder->pVertex=(sreal *)cylinder.pVertex_VNTT3;
		cylinder.pVertex_VNTT3=NULL;
		break;
	}


	pcylinder->Stacks=cylinder.Stacks;
	pcylinder->Slices=cylinder.Slices;

	//Copy the rest of it in a few shots
	pcylinder->Start_DrawRangeElements=cylinder.Start_DrawRangeElements;
	pcylinder->End_DrawRangeElements=cylinder.End_DrawRangeElements;

	pcylinder->Start_DrawRangeElements_BodyOfCylinder=cylinder.Start_DrawRangeElements_BodyOfCylinder;
	pcylinder->End_DrawRangeElements_BodyOfCylinder=cylinder.End_DrawRangeElements_BodyOfCylinder;
	pcylinder->Start_DrawRangeElements_TopOfCylinder=cylinder.Start_DrawRangeElements_TopOfCylinder;
	pcylinder->End_DrawRangeElements_TopOfCylinder=cylinder.End_DrawRangeElements_TopOfCylinder;
	pcylinder->Start_DrawRangeElements_BottomOfCylinder=cylinder.Start_DrawRangeElements_BottomOfCylinder;
	pcylinder->End_DrawRangeElements_BottomOfCylinder=cylinder.End_DrawRangeElements_BottomOfCylinder;

	pcylinder->StartIndexIn_pIndex_BodyOfCylinder=cylinder.StartIndexIn_pIndex_BodyOfCylinder;
	pcylinder->EndIndexIn_pIndex_BodyOfCylinder=cylinder.EndIndexIn_pIndex_BodyOfCylinder;
	pcylinder->StartIndexIn_pIndex_TopOfCylinder=cylinder.StartIndexIn_pIndex_TopOfCylinder;
	pcylinder->EndIndexIn_pIndex_TopOfCylinder=cylinder.EndIndexIn_pIndex_TopOfCylinder;
	pcylinder->StartIndexIn_pIndex_BottomOfCylinder=cylinder.StartIndexIn_pIndex_BottomOfCylinder;
	pcylinder->EndIndexIn_pIndex_BottomOfCylinder=cylinder.EndIndexIn_pIndex_BottomOfCylinder;

	pcylinder->TotalIndex_BodyOfCylinder=cylinder.TotalIndex_BodyOfCylinder;
	pcylinder->TotalIndex_TopOfCylinder=cylinder.TotalIndex_TopOfCylinder;
	pcylinder->TotalIndex_BottomOfCylinder=cylinder.TotalIndex_BottomOfCylinder;

	pcylinder->pIndex16Bit_BodyOfCylinder=cylinder.pIndex16Bit_BodyOfCylinder;
	pcylinder->pIndex16Bit_TopOfCylinder=cylinder.pIndex16Bit_TopOfCylinder;
	pcylinder->pIndex16Bit_BottomOfCylinder=cylinder.pIndex16Bit_BottomOfCylinder;

	pcylinder->pIndex32Bit_BodyOfCylinder=cylinder.pIndex32Bit_BodyOfCylinder;
	pcylinder->pIndex32Bit_TopOfCylinder=cylinder.pIndex32Bit_TopOfCylinder;
	pcylinder->pIndex32Bit_BottomOfCylinder=cylinder.pIndex32Bit_BottomOfCylinder;

	pcylinder->StartIndexIn_pVertex_BodyOfCylinder=cylinder.StartIndexIn_pVertex_BodyOfCylinder;
	pcylinder->EndIndexIn_pVertex_BodyOfCylinder=cylinder.EndIndexIn_pVertex_BodyOfCylinder;
	pcylinder->StartIndexIn_pVertex_TopOfCylinder=cylinder.StartIndexIn_pVertex_TopOfCylinder;
	pcylinder->EndIndexIn_pVertex_TopOfCylinder=cylinder.EndIndexIn_pVertex_TopOfCylinder;
	pcylinder->StartIndexIn_pVertex_BottomOfCylinder=cylinder.StartIndexIn_pVertex_BottomOfCylinder;
	pcylinder->EndIndexIn_pVertex_BottomOfCylinder=cylinder.EndIndexIn_pVertex_BottomOfCylinder;

	pcylinder->TotalVertex_BodyOfCylinder=cylinder.TotalVertex_BodyOfCylinder;
	pcylinder->TotalVertex_TopOfCylinder=cylinder.TotalVertex_TopOfCylinder;
	pcylinder->TotalVertex_BottomOfCylinder=cylinder.TotalVertex_BottomOfCylinder;

	pcylinder->pVertex_BodyOfCylinder=cylinder.pVertex_BodyOfCylinder;
	pcylinder->pVertex_TopOfCylinder=cylinder.pVertex_TopOfCylinder;
	pcylinder->pVertex_BottomOfCylinder=cylinder.pVertex_BottomOfCylinder;

	cylinder.DeallocateAllMemory();
	return 1;
}

sint glhDeleteCylinderf2(glhCylinderObjectf2 *pcylinder)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(pcylinder);
	DeleteCommonObject(common);
	return 1;
}

sint glhCreateSpheref2(glhSphereObjectf2 *psphere)
{
	sint i, returnVal;

	if(psphere==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, psphere->RadiusA, TRUE, psphere->RadiusB, TRUE, psphere->RadiusC,
		psphere->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((psphere->TexCoordStyle[i]!=0)&&(psphere->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(psphere->Stacks<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(psphere->Slices<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TSphere sphere;

	returnVal=sphere.StartupByABCStacksAndSlices2(psphere->IndexFormat, psphere->VertexFormat,
		psphere->RadiusA, psphere->RadiusB, psphere->RadiusC, psphere->Stacks, psphere->Slices,
		psphere->TexCoordStyle[0], psphere->ScaleFactorS[0], psphere->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		sphere.DeallocateAllMemory();
		return -1;
	}

	//Write stuff to psphere
	psphere->TotalIndex=sphere.UsedIndex;
	psphere->pIndex16Bit=sphere.pIndex16Bit;
	sphere.pIndex16Bit=NULL;
	psphere->pIndex32Bit=sphere.pIndex32Bit;
	sphere.pIndex32Bit=NULL;
	psphere->VertexCount=sphere.VertexCount;
	psphere->pVertex=sphere.pVertex;

	switch(sphere.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		psphere->pVertex=(sreal *)sphere.pVertex_V;
		sphere.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		psphere->pVertex=(sreal *)sphere.pVertex_VT;
		sphere.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		psphere->pVertex=(sreal *)sphere.pVertex_VNT;
		sphere.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNT3;
		sphere.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNTT3T3;
		sphere.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNTT3;
		sphere.pVertex_VNTT3=NULL;
		break;
	}


	psphere->RadiusA=sphere.RadiusA;
	psphere->RadiusB=sphere.RadiusB;
	psphere->RadiusC=sphere.RadiusC;
	psphere->Stacks=sphere.Stacks;
	psphere->Slices=sphere.Slices;

	psphere->Start_DrawRangeElements=sphere.Start_DrawRangeElements;
	psphere->End_DrawRangeElements=sphere.End_DrawRangeElements;

	sphere.DeallocateAllMemory();
	return 1;
}

sint glhDeleteSpheref2(glhSphereObjectf2 *psphere)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(psphere);
	DeleteCommonObject(common);
	return 1;
}


sint glhCreatePartialSphereStackf2(glhPartialSphereStackObjectf2 *psphere)
{
	sint i, returnVal;

	if(psphere==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, psphere->RadiusA, TRUE, psphere->RadiusB, TRUE, psphere->RadiusC,
		psphere->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((psphere->TexCoordStyle[i]!=0)&&(psphere->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(psphere->Stacks<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(psphere->Slices<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	//Start stack and end stack should make sense
	if(psphere->StartingStack<0)							//Stack count starts at 0 and go to Stacks-1
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(psphere->StartingStack>psphere->EndingStack)		//Must not be larger
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(psphere->EndingStack>psphere->Stacks-1)				//Must not be larger than the number of stacks we will create
	{														//Also, stack count starts at 0 and go to Stacks-1
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	//Yes, psphere->StartingStack can EQUAL psphere->EndingStack and that will generate 1 single stack.



	TSphere sphere;

	returnVal=sphere.StartupByABCStacksAndSlices_StackBound2(psphere->IndexFormat, psphere->VertexFormat,
		psphere->RadiusA, psphere->RadiusB, psphere->RadiusC, psphere->Stacks, psphere->Slices,
		psphere->TexCoordStyle[0], psphere->ScaleFactorS[0], psphere->ScaleFactorT[0],
		psphere->StartingStack, psphere->EndingStack);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		sphere.DeallocateAllMemory();
		return -1;
	}

	//Write stuff to psphere
	psphere->TotalIndex=sphere.UsedIndex;
	psphere->pIndex16Bit=sphere.pIndex16Bit;
	sphere.pIndex16Bit=NULL;
	psphere->pIndex32Bit=sphere.pIndex32Bit;
	sphere.pIndex32Bit=NULL;
	psphere->VertexCount=sphere.VertexCount;
	psphere->pVertex=sphere.pVertex;

	switch(sphere.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		psphere->pVertex=(sreal *)sphere.pVertex_V;
		sphere.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		psphere->pVertex=(sreal *)sphere.pVertex_VT;
		sphere.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		psphere->pVertex=(sreal *)sphere.pVertex_VNT;
		sphere.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNT3;
		sphere.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNTT3T3;
		sphere.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		psphere->pVertex=(sreal *)sphere.pVertex_VNTT3;
		sphere.pVertex_VNTT3=NULL;
		break;
	}


	psphere->RadiusA=sphere.RadiusA;
	psphere->RadiusB=sphere.RadiusB;
	psphere->RadiusC=sphere.RadiusC;
	psphere->Stacks=sphere.Stacks;
	psphere->Slices=sphere.Slices;

	psphere->Start_DrawRangeElements=sphere.Start_DrawRangeElements;
	psphere->End_DrawRangeElements=sphere.End_DrawRangeElements;

	sphere.DeallocateAllMemory();
	return 1;
}

sint glhDeletePartialSphereStackf2(glhPartialSphereStackObjectf2 *psphere)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(psphere);
	DeleteCommonObject(common);
	return 1;
}

sint glhCreateTorusf2(glhTorusObjectf2 *ptorus)
{
	sint i, returnVal;

	if(ptorus==NULL)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	returnVal=CheckForParameterErrors_Geometry(TRUE, ptorus->RadiusOfTorus, TRUE, ptorus->RadiusOfCircle, FALSE, ptorus->RadiusOfCircle,
		ptorus->IndexFormat);
	if(returnVal==-1)
	{
		return -1;
	}

	//Style must be 0 or 1, if texcoords are wanted
	for(i=0; i<NUMBEROFTEXTUREUNITS; i++)
	{
		if((ptorus->TexCoordStyle[i]!=0)&&(ptorus->TexCoordStyle[i]!=1))
		{
			if(TotalErrorStateStack<100)
			{
				ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
				TotalErrorStateStack++;
			}
			return -1;
		}
	}

	if(ptorus->Stacks<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}
	if(ptorus->Slices<3)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_INVALID_PARAM);
			TotalErrorStateStack++;
		}
		return -1;
	}

	TTorus torus;

	returnVal=torus.StartupByStacksAndSlices2(ptorus->IndexFormat, ptorus->VertexFormat,
		ptorus->RadiusOfTorus, ptorus->RadiusOfCircle, ptorus->Stacks, ptorus->Slices,
		ptorus->TexCoordStyle[0], ptorus->ScaleFactorS[0], ptorus->ScaleFactorT[0]);
	if(returnVal==-1)
	{
		if(TotalErrorStateStack<100)
		{
			ErrorStateStack.push(GLH_ERROR_OUT_OF_MEMORY);
			TotalErrorStateStack++;
		}
		torus.DeallocateAllMemory();
		return -1;
	}

	//Write stuff to ptorus
	ptorus->TotalIndex=torus.UsedIndex;
	ptorus->pIndex16Bit=torus.pIndex16Bit;
	torus.pIndex16Bit=NULL;
	ptorus->pIndex32Bit=torus.pIndex32Bit;
	torus.pIndex32Bit=NULL;
	ptorus->VertexCount=torus.VertexCount;
	ptorus->pVertex=torus.pVertex;

	switch(torus.VertexFormat)
	{
	case GLH_VERTEXFORMAT_V:
		ptorus->pVertex=(sreal *)torus.pVertex_V;
		torus.pVertex_V=NULL;
		break;
	case GLH_VERTEXFORMAT_VT:
		ptorus->pVertex=(sreal *)torus.pVertex_VT;
		torus.pVertex_VT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT:
		ptorus->pVertex=(sreal *)torus.pVertex_VNT;
		torus.pVertex_VNT=NULL;
		break;
	case GLH_VERTEXFORMAT_VNT3:
		ptorus->pVertex=(sreal *)torus.pVertex_VNT3;
		torus.pVertex_VNT3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3T3:
		ptorus->pVertex=(sreal *)torus.pVertex_VNTT3T3;
		torus.pVertex_VNTT3T3=NULL;
		break;
	case GLH_VERTEXFORMAT_VNTT3:
		ptorus->pVertex=(sreal *)torus.pVertex_VNTT3;
		torus.pVertex_VNTT3=NULL;
		break;
	}


	ptorus->RadiusOfTorus=torus.RadiusOfTorus;
	ptorus->RadiusOfCircle=torus.RadiusOfCircle;
	ptorus->Stacks=torus.Stacks;
	ptorus->Slices=torus.Slices;

	ptorus->Start_DrawRangeElements=torus.Start_DrawRangeElements;
	ptorus->End_DrawRangeElements=torus.End_DrawRangeElements;

	torus.DeallocateAllMemory();
	return 1;
}

sint glhDeleteTorusf2(glhTorusObjectf2 *ptorus)
{
	glhCommonObjectf2 *common=reinterpret_cast<glhCommonObjectf2 *>(ptorus);
	DeleteCommonObject(common);
	return 1;
}

sint glhGetGLDeviceCaps_GL_1_5(char *errorMessage, sint adapterCount, sint deviceType, GLCAPS_GL_1_5 *pcaps)
{
	if(pcaps==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "pcaps is NULL @ glhGetGLDeviceCaps_GL_1_5");
		return -1;
	}


	if(pcaps->Size!=sizeof(GLCAPS_GL_1_5))
	{
		if(errorMessage)
			strcpy(errorMessage, "Caps size is invalid @ glhGetGLDeviceCaps_GL_1_5");
		return -1;
	}

	//Init to zero
	memset(pcaps, 0, sizeof(GLCAPS_GL_1_5));
	pcaps->Size=sizeof(GLCAPS_GL_1_5);

	uchar temp;
	//Buffers
	glGetBooleanv(GL_RGBA_MODE, &temp);
	pcaps->RGBAModeb=(uint)temp;

	glGetBooleanv(GL_DOUBLEBUFFER, &temp);
	pcaps->DoubleBufferedb=(uint)temp;

	glGetIntegerv(GL_AUX_BUFFERS, &pcaps->AuxBufferCounti);

	glGetBooleanv(GL_STEREO, &temp);
	pcaps->Stereob=(uint)temp;

	glGetIntegerv(GL_RED_BITS, &pcaps->RedBitsi);
	glGetIntegerv(GL_GREEN_BITS, &pcaps->GreenBitsi);
	glGetIntegerv(GL_BLUE_BITS, &pcaps->BlueBitsi);
	glGetIntegerv(GL_ALPHA_BITS, &pcaps->AlphaBitsi);
	glGetIntegerv(GL_DEPTH_BITS, &pcaps->DepthBitsi);
	glGetIntegerv(GL_STENCIL_BITS, &pcaps->StencilBitsi);
	glGetIntegerv(GL_ACCUM_RED_BITS, &pcaps->AccumRedBitsi);
	glGetIntegerv(GL_ACCUM_GREEN_BITS, &pcaps->AccumGreenBitsi);
	glGetIntegerv(GL_ACCUM_BLUE_BITS, &pcaps->AccumBlueBitsi);
	glGetIntegerv(GL_ACCUM_ALPHA_BITS, &pcaps->AccumAlphaBitsi);

	//Clip plane
	glGetIntegerv(GL_MAX_CLIP_PLANES, &pcaps->MaxClipPlanesi);

	//Matrices and stacks
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &pcaps->MaxModelviewStackDepthi);
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &pcaps->MaxProjectionStackDepthi);
	glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &pcaps->MaxTextureStackDepthi);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &pcaps->MaxNameStackDepthi);
	glGetIntegerv(GL_MAX_LIST_NESTING, &pcaps->MaxListNestingi);
	glGetIntegerv(GL_MAX_EVAL_ORDER, &pcaps->MaxEvalOrderi);
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &pcaps->MaxAttribStackDepthi);
	glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &pcaps->MaxClientAttribStackDepthi);


	//Pixel and fragments
	glGetIntegerv(GL_SUBPIXEL_BITS, &pcaps->SubPixelBitsi);
	glGetIntegerv(GL_SAMPLE_BUFFERS, &pcaps->SampleBuffersi);
	glGetIntegerv(GL_SAMPLES, &pcaps->Samplesi);
	glGetIntegerv(GL_QUERY_COUNTER_BITS, &pcaps->QueryCounterBitsi);

	//Textures
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &pcaps->MaxTexture2DSizei);
	pcaps->MaxTexture1DSizei=pcaps->MaxTexture2DSizei;			//Same
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &pcaps->MaxTexture3DSizei);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &pcaps->MaxTextureCubeSizei);
	glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &pcaps->MaxTextureLODBiasi);
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &pcaps->MaxTextureUnitsi);
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &pcaps->CompressedTextureFormatsi);
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &pcaps->NumberOfCompressedTextureFormatsi);

	//Viewport
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, pcaps->MaxViewportDimensionsi);

	//Primitives
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pcaps->AliasedPointSizeRangef);
	glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, pcaps->SmoothPointSizeRangef);

	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, pcaps->AliasedLineWidthRangef);
	glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, pcaps->SmoothLineWidthRangef);

	//Arrays
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &pcaps->MaxElementsIndicesi);
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES , &pcaps->MaxElementsVerticesi);

	
	//Lights
	glGetIntegerv(GL_MAX_LIGHTS, &pcaps->MaxLightsi);

	//glGetIntegerv(, &pcaps->);

	return 1;
}

sint glhGetGLDeviceCaps_GL_2_0(char *errorMessage, sint adapterCount, sint deviceType, GLCAPS_GL_2_0 *pcaps)
{
	if(pcaps==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "pcaps is NULL @ glhGetGLDeviceCaps_GL_2_0");
		return -1;
	}


	if(pcaps->Size!=sizeof(GLCAPS_GL_2_0))
	{
		if(errorMessage)
			strcpy(errorMessage, "Caps size is invalid @ glhGetGLDeviceCaps_GL_2_0");
		return -1;
	}

	//Init to zero
	memset(pcaps, 0, sizeof(GLCAPS_GL_2_0));
	pcaps->Size=sizeof(GLCAPS_GL_2_0);

	uchar temp;

	//Buffers
	glGetBooleanv(GL_RGBA_MODE, &temp);
	pcaps->RGBAModeb=(uint)temp;

	glGetBooleanv(GL_DOUBLEBUFFER, &temp);
	pcaps->DoubleBufferedb=(uint)temp;

	glGetIntegerv(GL_AUX_BUFFERS, &pcaps->AuxBufferCounti);

	glGetBooleanv(GL_STEREO, &temp);
	pcaps->Stereob=(uint)temp;

	glGetIntegerv(GL_RED_BITS, &pcaps->RedBitsi);
	glGetIntegerv(GL_GREEN_BITS, &pcaps->GreenBitsi);
	glGetIntegerv(GL_BLUE_BITS, &pcaps->BlueBitsi);
	glGetIntegerv(GL_ALPHA_BITS, &pcaps->AlphaBitsi);
	glGetIntegerv(GL_DEPTH_BITS, &pcaps->DepthBitsi);
	glGetIntegerv(GL_STENCIL_BITS, &pcaps->StencilBitsi);
	glGetIntegerv(GL_ACCUM_RED_BITS, &pcaps->AccumRedBitsi);
	glGetIntegerv(GL_ACCUM_GREEN_BITS, &pcaps->AccumGreenBitsi);
	glGetIntegerv(GL_ACCUM_BLUE_BITS, &pcaps->AccumBlueBitsi);
	glGetIntegerv(GL_ACCUM_ALPHA_BITS, &pcaps->AccumAlphaBitsi);

	//Clip plane
	glGetIntegerv(GL_MAX_CLIP_PLANES, &pcaps->MaxClipPlanesi);

	//Matrices and stacks
	glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &pcaps->MaxModelviewStackDepthi);
	glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &pcaps->MaxProjectionStackDepthi);
	glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &pcaps->MaxTextureStackDepthi);
	glGetIntegerv(GL_MAX_NAME_STACK_DEPTH, &pcaps->MaxNameStackDepthi);
	glGetIntegerv(GL_MAX_LIST_NESTING, &pcaps->MaxListNestingi);
	glGetIntegerv(GL_MAX_EVAL_ORDER, &pcaps->MaxEvalOrderi);
	glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &pcaps->MaxAttribStackDepthi);
	glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH, &pcaps->MaxClientAttribStackDepthi);


	//Pixel and fragments
	glGetIntegerv(GL_SUBPIXEL_BITS, &pcaps->SubPixelBitsi);
	glGetIntegerv(GL_SAMPLE_BUFFERS, &pcaps->SampleBuffersi);
	glGetIntegerv(GL_SAMPLES, &pcaps->Samplesi);
	glGetIntegerv(GL_QUERY_COUNTER_BITS, &pcaps->QueryCounterBitsi);

	//Textures
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &pcaps->MaxTexture2DSizei);
	pcaps->MaxTexture1DSizei=pcaps->MaxTexture2DSizei;			//Same
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &pcaps->MaxTexture3DSizei);
	glGetIntegerv(GL_MAX_CUBE_MAP_TEXTURE_SIZE, &pcaps->MaxTextureCubeSizei);
	glGetIntegerv(GL_MAX_TEXTURE_LOD_BIAS, &pcaps->MaxTextureLODBiasi);
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &pcaps->MaxTextureUnitsi);
	glGetIntegerv(GL_COMPRESSED_TEXTURE_FORMATS, &pcaps->CompressedTextureFormatsi);
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &pcaps->NumberOfCompressedTextureFormatsi);

	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &pcaps->MaxCombinedTextureImageUnitsi);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &pcaps->MaxTextureImageUnitsi);
	glGetIntegerv(GL_MAX_TEXTURE_COORDS, &pcaps->MaxTextureCoordsi);

	//Viewport
	glGetIntegerv(GL_MAX_VIEWPORT_DIMS, pcaps->MaxViewportDimensionsi);

	//Primitives
	glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE, pcaps->AliasedPointSizeRangef);
	glGetFloatv(GL_SMOOTH_POINT_SIZE_RANGE, pcaps->SmoothPointSizeRangef);

	glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, pcaps->AliasedLineWidthRangef);
	glGetFloatv(GL_SMOOTH_LINE_WIDTH_RANGE, pcaps->SmoothLineWidthRangef);

	//Arrays
	glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &pcaps->MaxElementsIndicesi);
	glGetIntegerv(GL_MAX_ELEMENTS_VERTICES , &pcaps->MaxElementsVerticesi);

	
	//Lights
	glGetIntegerv(GL_MAX_LIGHTS, &pcaps->MaxLightsi);

	//Vertex related
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &pcaps->MaxVertexAttribsi);


	return 1;
}

sint glhGetGLDeviceCaps_ARB_vp_1_0(char *errorMessage, sint adapterCount, sint deviceType, GLCAPS_ARB_vp_1_0 *pcaps)
{
	if(pcaps==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "pcaps is NULL @ glhGetGLDeviceCaps_ARB_vp_1_0");
		return -1;
	}


	if(pcaps->Size!=sizeof(GLCAPS_ARB_vp_1_0))
	{
		if(errorMessage)
			strcpy(errorMessage, "Caps size is invalid @ glhGetGLDeviceCaps_ARB_vp_1_0");
		return -1;
	}

	//Init to zero
	memset(pcaps, 0, sizeof(GLCAPS_ARB_vp_1_0));
	pcaps->Size=sizeof(GLCAPS_ARB_vp_1_0);

	PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
	glGetProgramivARB=(PFNGLGETPROGRAMIVARBPROC)(wglGetProcAddress("glGetProgramivARB"));
	if(glGetProgramivARB==NULL)
		return 1;		//Should not reach here anyway

	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &pcaps->MaxProgramInstructionsVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &pcaps->MaxProgramNativeInstructionsVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &pcaps->MaxProgramTemporariesVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &pcaps->MaxProgramNativeTemporariesVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &pcaps->MaxProgramParametersVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &pcaps->MaxProgramNativeParametersVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &pcaps->MaxProgramAttribsVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &pcaps->MaxProgramNativeAttribsVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ADDRESS_REGISTERS_ARB, &pcaps->MaxProgramAddressRegistersVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ADDRESS_REGISTERS_ARB, &pcaps->MaxProgramNativeAddressRegistersVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_LOCAL_PARAMETERS_ARB, &pcaps->MaxProgramLocalParametersVPi);
	glGetProgramivARB(GL_VERTEX_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &pcaps->MaxProgramEnvParametersVPi);

	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS_ARB, &pcaps->MaxVertexAttribsi);
	glGetIntegerv(GL_MAX_PROGRAM_MATRICES_ARB, &pcaps->MaxProgramMatricesi);
	glGetIntegerv(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &pcaps->MaxProgramMatrixStackDepthi);

	return 1;
}

sint glhGetGLDeviceCaps_ARB_fp_1_0(char *errorMessage, sint adapterCount, sint deviceType, GLCAPS_ARB_fp_1_0 *pcaps)
{
	if(pcaps==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "pcaps is NULL @ glhGetGLDeviceCaps_ARB_fp_1_0");
		return -1;
	}


	if(pcaps->Size!=sizeof(GLCAPS_ARB_fp_1_0))
	{
		if(errorMessage)
			strcpy(errorMessage, "Caps size is invalid @ glhGetGLDeviceCaps_ARB_fp_1_0");
		return -1;
	}

	//Init to zero
	memset(pcaps, 0, sizeof(GLCAPS_ARB_fp_1_0));
	pcaps->Size=sizeof(GLCAPS_ARB_fp_1_0);

	PFNGLGETPROGRAMIVARBPROC glGetProgramivARB;
	glGetProgramivARB=(PFNGLGETPROGRAMIVARBPROC)(wglGetProcAddress("glGetProgramivARB"));
	if(glGetProgramivARB==NULL)
		return 1;		//Should not reach here anyway

	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_INSTRUCTIONS_ARB, &pcaps->MaxProgramInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_INSTRUCTIONS_ARB, &pcaps->MaxProgramNativeInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEMPORARIES_ARB, &pcaps->MaxProgramTemporariesFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEMPORARIES_ARB, &pcaps->MaxProgramNativeTemporariesFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_PARAMETERS_ARB, &pcaps->MaxProgramParametersFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_PARAMETERS_ARB, &pcaps->MaxProgramNativeParametersFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ATTRIBS_ARB, &pcaps->MaxProgramAttributesFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ATTRIBS_ARB, &pcaps->MaxProgramNativeAttributesFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ENV_PARAMETERS_ARB, &pcaps->MaxProgramEnvParametersFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_ALU_INSTRUCTIONS_ARB, &pcaps->MaxProgramALUInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_ALU_INSTRUCTIONS_ARB, &pcaps->MaxProgramNativeALUInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INSTRUCTIONS_ARB, &pcaps->MaxProgramTEXInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INSTRUCTIONS_ARB, &pcaps->MaxProgramNativeTEXInstructionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_TEX_INDIRECTIONS_ARB, &pcaps->MaxProgramTEXIndirectionsFPi);
	glGetProgramivARB(GL_FRAGMENT_PROGRAM_ARB, GL_MAX_PROGRAM_NATIVE_TEX_INDIRECTIONS_ARB, &pcaps->MaxProgramNativeTEXIndirectionsFPi);

	glGetIntegerv(GL_MAX_PROGRAM_MATRICES_ARB, &pcaps->MaxProgramMatricesi);
	glGetIntegerv(GL_MAX_PROGRAM_MATRIX_STACK_DEPTH_ARB, &pcaps->MaxProgramMatrixStackDepthi);
	glGetIntegerv(GL_MAX_TEXTURE_COORDS_ARB, &pcaps->MaxTextureCoordsi);
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS_ARB, &pcaps->MaxTextureImageUnitsi);

	return 1;
}

sint glhGetGLDeviceCaps_GL2_0_GLSL(char *errorMessage, sint adapterCount, sint deviceType, GLCAPS_GL_2_0_GLSL *pcaps)
{
	schar *version;
	sint i, pointfound, checkPoint, present;
	schar buffer[64];

	if(pcaps==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "pcaps is NULL @ glhGetGLDeviceCaps_GL2_0_GLSL");
		return -1;
	}


	if(pcaps->Size!=sizeof(GLCAPS_GL_2_0_GLSL))
	{
		if(errorMessage)
			strcpy(errorMessage, "Caps size is invalid @ glhGetGLDeviceCaps_GL2_0_GLSL");
		return -1;
	}

	//Init to zero
	memset(pcaps, 0, sizeof(GLCAPS_GL_2_0_GLSL));
	pcaps->Size=sizeof(GLCAPS_GL_2_0_GLSL);

	version=(schar *)glGetString(GL_SHADING_LANGUAGE_VERSION);
	if(version==NULL)
	{
		if(errorMessage)
			strcpy(errorMessage, "glGetString(GL_SHADING_LANGUAGE_VERSION) returned NULL @ glhGetGLDeviceCaps_GL2_0_GLSL");
		return -1;
	}
	i=pointfound=0;
	while(version[i]!=0)
	{
		if(pointfound==0)
		{
			if(IsCharacterANumber(version[i])==1)
				pcaps->ShadingLanguageVersionc[i]=version[i];
			else if(version[i]=='.')
			{
				pcaps->ShadingLanguageVersionc[i]='.';
				pointfound=1;
			}
			else
				break;
		}
		else
		{
			if(IsCharacterANumber(version[i])==1)
				pcaps->ShadingLanguageVersionc[i]=version[i];
			else
				break;
		}
		i++;
	}
	pcaps->ShadingLanguageVersionc[i]='\0';

	checkPoint=0;
	present=FALSE;
	for(i=0; version[i]!='\0'; i++)
	{
		if(version[i]=='.')
		{
			checkPoint++;
			if(checkPoint==2)
				break;
			buffer[i]=version[i];
		}
		else
			buffer[i]=version[i];

		if(buffer[i]==' ')
			present=TRUE;
	}
	buffer[i]='\0';

	if(present)
		CopyStringUntilCharacterIsMet(' ', buffer, version, 127, 1);
	else
		strcpy(buffer, version);
	pcaps->ShadingLanguageVersionf=ConvertStringIntoSREAL(buffer);

	glGetIntegerv(GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, &pcaps->MaxVertexTextureImageUnitsi);
	glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &pcaps->MaxVertexUniformComponentsi);
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &pcaps->MaxFragmentUniformComponentsi);
	glGetIntegerv(GL_MAX_VARYING_FLOATS, &pcaps->MaxVaryingFloatsi);

	return 1;
}


sint glhIsGLExtensionSupported(const schar *extensionName)
{
	schar *ext=(schar *)glGetString(GL_EXTENSIONS);
	if((ext!=NULL)&&(extensionName!=NULL))
	{
		if(strstr(ext, extensionName)!=NULL)
			return 1;
		else
			return 0;
	}
	else
		return -1;
}

sint glhIsWGLExtensionSupported(HDC hdc, const schar *extensionName)
{
	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB=NULL;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT=NULL;

	wglGetExtensionsStringARB=(PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	wglGetExtensionsStringEXT=(PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if((wglGetExtensionsStringARB)||(wglGetExtensionsStringEXT))
	{
		schar *ext;
		if(wglGetExtensionsStringARB)
			ext=(schar *)wglGetExtensionsStringARB(hdc);
		else
			ext=(schar *)wglGetExtensionsStringEXT();

		if((ext!=NULL)&&(extensionName!=NULL))
		{
			if(strstr(ext, extensionName)!=NULL)
				return 1;
			else
				return 0;
		}
		else
			return -1;
	}
	else
		return -1;

	//OLD CODE THAT DIDN'T WORK BECAUSE
	//The problem is that ATI drivers return WGL_EXT_extensions_string when you call glGetString(GL_EXTENSIONS)
	//but they don't return WGL_ARB_extensions_string.
	//nVidia doesn't return either.
	//I had to modify my glhIsWGLExtensionSupported in order to compensate for this bizareness.
	/*PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB=NULL;
	PFNWGLGETEXTENSIONSSTRINGEXTPROC wglGetExtensionsStringEXT=NULL;

	if(glhIsGLExtensionSupported("WGL_ARB_extensions_string"))
		wglGetExtensionsStringARB=(PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");

	if(glhIsGLExtensionSupported("WGL_EXT_extensions_string"))
		wglGetExtensionsStringEXT=(PFNWGLGETEXTENSIONSSTRINGEXTPROC)wglGetProcAddress("wglGetExtensionsStringEXT");

	if((wglGetExtensionsStringARB)||(wglGetExtensionsStringEXT))
	{
		schar *ext;
		if(wglGetExtensionsStringARB)
			ext=(schar *)wglGetExtensionsStringARB(hdc);
		else
			ext=(schar *)wglGetExtensionsStringEXT();

		if((ext!=NULL)&&(extensionName!=NULL))
		{
			if(strstr(ext, extensionName)!=NULL)
				return 1;
			else
				return 0;
		}
		else
			return -1;
	}
	else
		return -1;*/
}


void glhConstantSizeMatrixf2(sreal *newModelview, sreal *objectCenter, sreal *modelview_projection, sreal *inverse_modelview_projection, sreal *modelview, sint *viewport)
{
	sint j;
	sreal windowCoordinates[4];				//XYZW
	sreal squareBoundingBox[16];			//XYZW, 4 vertices. Right bottom, then CCW rotation.
	sreal objsquareBoundingBox[16];			//XYZW, 4 vertices. Right bottom, then CCW rotation.
											//Object space.

	sreal width, length;

	glhProjectf_2(4, objectCenter,
				modelview_projection,
				viewport,
				windowCoordinates);

	//Compute pixel bounding box (square box, like 50 by 50 pixels).
	j=0;
	squareBoundingBox[j++]=windowCoordinates[0]+25.0;
	squareBoundingBox[j++]=windowCoordinates[1]-25.0;
	squareBoundingBox[j++]=windowCoordinates[2];			//Depth doesn't change
	squareBoundingBox[j++]=1.0;

	squareBoundingBox[j++]=windowCoordinates[0]+25.0;
	squareBoundingBox[j++]=windowCoordinates[1]+25.0;
	squareBoundingBox[j++]=windowCoordinates[2];			//Depth doesn't change
	squareBoundingBox[j++]=1.0;

	squareBoundingBox[j++]=windowCoordinates[0]-25.0;
	squareBoundingBox[j++]=windowCoordinates[1]+25.0;
	squareBoundingBox[j++]=windowCoordinates[2];			//Depth doesn't change
	squareBoundingBox[j++]=1.0;

	squareBoundingBox[j++]=windowCoordinates[0]-25.0;
	squareBoundingBox[j++]=windowCoordinates[1]-25.0;
	squareBoundingBox[j++]=windowCoordinates[2];			//Depth doesn't change
	squareBoundingBox[j++]=1.0;

	//Transform square box from window space to object space.
	glhUnProjectf_2(16, squareBoundingBox, 
			inverse_modelview_projection,
			viewport,
			objsquareBoundingBox);


	//HACK. What is the width of the square? What is the height of the square?
	width=sqrtf((objsquareBoundingBox[8]-objsquareBoundingBox[4])*
		(objsquareBoundingBox[8]-objsquareBoundingBox[4])+
		(objsquareBoundingBox[9]-objsquareBoundingBox[5])*
		(objsquareBoundingBox[9]-objsquareBoundingBox[5])+
		(objsquareBoundingBox[10]-objsquareBoundingBox[6])*
		(objsquareBoundingBox[10]-objsquareBoundingBox[6]));
	length=sqrtf((objsquareBoundingBox[0]-objsquareBoundingBox[4])*
		(objsquareBoundingBox[0]-objsquareBoundingBox[4])+
		(objsquareBoundingBox[1]-objsquareBoundingBox[5])*
		(objsquareBoundingBox[1]-objsquareBoundingBox[5])+
		(objsquareBoundingBox[2]-objsquareBoundingBox[6])*
		(objsquareBoundingBox[2]-objsquareBoundingBox[6]));

	if(width<length)
		width=length;		//Choose largest.

	width=width/25.0;

	//Apply to current matrix
	memcpy(newModelview, modelview, sizeof(sreal)*16);
	glhTranslatef2(newModelview, objectCenter[0], objectCenter[1], objectCenter[2]);
	glhScalef2(newModelview, width, width, width);
	glhTranslatef2(newModelview, -objectCenter[0], -objectCenter[1], -objectCenter[2]);
}




sint glhExtractLookAtFromMatrixf2(sreal *matrix, sreal *eyePosition3D, sreal *center3D, sreal *upVector3D)
{
	sreal rightVector[3], newUpVector[3], lookAt[3];
	sreal ftemp;
	sreal factors[4], newRightVector[3];
	sreal newRightVector2[3];
	sreal newMatrix_12_13_14[3], newMatrix_12_13_14_2[3];
	
	sint is_eyePosition3D_Solved=FALSE;

	sint done[3];

	rightVector[0]=matrix[0];
	rightVector[1]=matrix[4];
	rightVector[2]=matrix[8];

	//We expect this to be normalized
	upVector3D[0]=matrix[1];
	upVector3D[1]=matrix[5];
	upVector3D[2]=matrix[9];

	//These were negated originally
	lookAt[0]=-matrix[2];
	lookAt[1]=-matrix[6];
	lookAt[2]=-matrix[10];

	//**** Try to solve a system of 3 equations ****
	//
	//[12] contains -(rightVector DOT eyePosition)
	//[13] contains -(upVector DOT eyePosition)
	//[14] contains -(lookVector DOT eye Position)
	//
	//The unknown is eyePosition. The others are already inside the matrix
	//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
	//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
	//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
	//
	if((rightVector[0]!=0.0)&&(upVector3D[0]!=0.0)&&(lookAt[0]!=0.0))		//Else we have problems!
	{
		ftemp=rightVector[0]/upVector3D[0];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=0.0;		//This becomes eliminated
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.x = 0.0 so
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate X from equation #2 using equation #3
		ftemp=upVector3D[0]/lookAt[0];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=0.0;		//This becomes eliminated
		newUpVector[1]=upVector3D[1]-factors[1];
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.y * Y + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[1]!=0.0)&&(newUpVector[1]!=0.0))
		{
			ftemp=newRightVector[1]/newUpVector[1];

			factors[1]=ftemp*newUpVector[1];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[1]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

			//Plug in Z into previous 2 variable equation to get Y
			eyePosition3D[1]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[1];

			//Now we have Y and Z. Solve for X using the original equation
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[0]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[1]*eyePosition3D[1]);

			is_eyePosition3D_Solved=TRUE;
		}
		else
			return -1;
	}
	else if((rightVector[1]!=0.0)&&(upVector3D[1]!=0.0)&&(lookAt[1]!=0.0))		//Else we have problems!
	{
		ftemp=rightVector[1]/upVector3D[1];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=0.0;		//This becomes eliminated
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.y = 0.0 so
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate Y from equation #2 using equation #3
		ftemp=upVector3D[1]/lookAt[1];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=0.0;		//This becomes eliminated
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

			//Plug in Z into previous 2 variable equation to get X
			eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[0];

			//Now we have X and Z. Solve for Y using the original equation
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[1]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[0]*eyePosition3D[0]);

			is_eyePosition3D_Solved=TRUE;
		}
		else
			return -1;
	}
	else if((rightVector[2]!=0.0)&&(upVector3D[2]!=0.0)&&(lookAt[2]!=0.0))		//Else we have problems!
	{
		ftemp=rightVector[2]/upVector3D[2];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.z = 0.0 so
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]

		//Now eliminate Z from equation #2 using equation #3
		ftemp=upVector3D[2]/lookAt[2];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=upVector3D[2]-factors[2];
		newUpVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.y * Y = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[1]=newUpVector[1]-factors[1];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.y * Y = newMatrix_12_13_14_2[0]
			//and it is easy to find Y
			eyePosition3D[1]=newMatrix_12_13_14_2[0]/newRightVector2[1];

			//Plug in Y into previous 2 variable equation to get X
			eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[1]*eyePosition3D[1]))/newRightVector[0];

			//Now we have X and Y. Solve for Z using the original equation
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[2]=(-matrix[12])-(rightVector[1]*eyePosition3D[1])-(rightVector[0]*eyePosition3D[0]);

			is_eyePosition3D_Solved=TRUE;
		}
		else
			return -1;
	}
	else
	{	//Some of the entries in the upper left 3x3 matrix are zero

		done[0]=done[1]=done[2]=FALSE;		//To know if all the components are filled

		//Try to find eyePosition3D
		if((matrix[0]==0.0)&&(matrix[4]==0.0)&&(matrix[8]!=0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[2]=-matrix[12]/matrix[8];
			done[2]=TRUE;
		}
		else if((matrix[0]==0.0)&&(matrix[4]!=0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[1]=-matrix[12]/matrix[4];
			done[1]=TRUE;
		}
		else if((matrix[0]!=0.0)&&(matrix[4]==0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[0]=-matrix[12]/matrix[0];
			done[0]=TRUE;
		}

		if((matrix[1]==0.0)&&(matrix[5]==0.0)&&(matrix[9]!=0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[2]=-matrix[13]/matrix[9];
			done[2]=TRUE;
		}
		else if((matrix[1]==0.0)&&(matrix[5]!=0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[1]=-matrix[13]/matrix[5];
			done[1]=TRUE;
		}
		else if((matrix[1]!=0.0)&&(matrix[5]==0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[0]=-matrix[13]/matrix[1];
			done[0]=TRUE;
		}

		if((matrix[2]==0.0)&&(matrix[6]==0.0)&&(matrix[10]!=0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[2]=-matrix[14]/matrix[10];
			done[2]=TRUE;
		}
		else if((matrix[2]==0.0)&&(matrix[6]!=0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[1]=-matrix[14]/matrix[6];
			done[1]=TRUE;
		}
		else if((matrix[2]!=0.0)&&(matrix[6]==0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[0]=-matrix[14]/matrix[2];
			done[0]=TRUE;
		}

		//Were all the components filled?
		if((done[0])&&(done[1])&&(done[2]))
			is_eyePosition3D_Solved=TRUE;
		else
			is_eyePosition3D_Solved=FALSE;
	}

	if(is_eyePosition3D_Solved)
	{
		//Compute center3D
		NormalizeVectorFLOAT_2(lookAt);

		center3D[0]=eyePosition3D[0]+lookAt[0];
		center3D[1]=eyePosition3D[1]+lookAt[1];
		center3D[2]=eyePosition3D[2]+lookAt[2];
		return 1;
	}
	else
	{
		return -1;
	}
	//NEW CODE, but doesn't work too well
	//I need to set aside some time and fix it
	/*sreal rightVector[3], newUpVector[3], lookAt[3];
	sreal ftemp;
	sreal factors[4], newRightVector[3];
	sreal newRightVector2[3];
	sreal newMatrix_12_13_14[3], newMatrix_12_13_14_2[3];
	
	sint is_eyePosition3D_Solved=FALSE;

	sint done[3];

	rightVector[0]=matrix[0];
	rightVector[1]=matrix[4];
	rightVector[2]=matrix[8];

	//We expect this to be normalized
	upVector3D[0]=matrix[1];
	upVector3D[1]=matrix[5];
	upVector3D[2]=matrix[9];

	//These were negated originally
	lookAt[0]=-matrix[2];
	lookAt[1]=-matrix[6];
	lookAt[2]=-matrix[10];

	//**** Try to solve a system of 3 equations ****
	//
	//[12] contains -(rightVector DOT eyePosition)
	//[13] contains -(upVector DOT eyePosition)
	//[14] contains -(lookVector DOT eye Position)
	//
	//The unknown is eyePosition. The others are already inside the matrix
	//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
	//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
	//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
	//
	if((rightVector[0]!=0.0)&&(upVector3D[0]!=0.0)&&(lookAt[0]!=0.0))		//Else we have problems!
	{
		ftemp=rightVector[0]/upVector3D[0];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=0.0;		//This becomes eliminated
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.x = 0.0 so
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate X from equation #2 using equation #3
		ftemp=upVector3D[0]/lookAt[0];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=0.0;		//This becomes eliminated
		newUpVector[1]=upVector3D[1]-factors[1];
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.y * Y + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[1]!=0.0)&&(newUpVector[1]!=0.0))
		{
			ftemp=newRightVector[1]/newUpVector[1];

			factors[1]=ftemp*newUpVector[1];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[1]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			if(newRightVector2[2]!=0.0)
			{
				eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

				//Plug in Z into previous 2 variable equation to get Y
				eyePosition3D[1]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[1];

				//Now we have Y and Z. Solve for X using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[0]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[1]*eyePosition3D[1]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}

	if((rightVector[1]!=0.0)&&(upVector3D[1]!=0.0)&&(lookAt[1]!=0.0)&&(is_eyePosition3D_Solved==FALSE))		//Else we have problems!
	{
		ftemp=rightVector[1]/upVector3D[1];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=0.0;		//This becomes eliminated
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.y = 0.0 so
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate Y from equation #2 using equation #3
		ftemp=upVector3D[1]/lookAt[1];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=0.0;		//This becomes eliminated
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			if(newRightVector2[2]!=0.0)
			{
				eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

				//Plug in Z into previous 2 variable equation to get X
				eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[0];

				//Now we have X and Z. Solve for Y using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[1]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[0]*eyePosition3D[0]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}


	if((rightVector[2]!=0.0)&&(upVector3D[2]!=0.0)&&(lookAt[2]!=0.0)&&(is_eyePosition3D_Solved==FALSE))		//Else we have problems!
	{
		ftemp=rightVector[2]/upVector3D[2];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.z = 0.0 so
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]

		//Now eliminate Z from equation #2 using equation #3
		ftemp=upVector3D[2]/lookAt[2];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=upVector3D[2]-factors[2];
		newUpVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.y * Y = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[1]=newUpVector[1]-factors[1];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.y * Y = newMatrix_12_13_14_2[0]
			//and it is easy to find Y
			if(newRightVector2[1]!=0.0)
			{
				eyePosition3D[1]=newMatrix_12_13_14_2[0]/newRightVector2[1];

				//Plug in Y into previous 2 variable equation to get X
				eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[1]*eyePosition3D[1]))/newRightVector[0];

				//Now we have X and Y. Solve for Z using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[2]=(-matrix[12])-(rightVector[1]*eyePosition3D[1])-(rightVector[0]*eyePosition3D[0]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}

	if(is_eyePosition3D_Solved==FALSE)
	{	//Some of the entries in the upper left 3x3 matrix are zero

		done[0]=done[1]=done[2]=FALSE;		//To know if all the components are filled

		//Try to find eyePosition3D
		if((matrix[0]==0.0)&&(matrix[4]==0.0)&&(matrix[8]!=0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[2]=-matrix[12]/matrix[8];
			done[2]=TRUE;
		}
		else if((matrix[0]==0.0)&&(matrix[4]!=0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[1]=-matrix[12]/matrix[4];
			done[1]=TRUE;
		}
		else if((matrix[0]!=0.0)&&(matrix[4]==0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[0]=-matrix[12]/matrix[0];
			done[0]=TRUE;
		}

		if((matrix[1]==0.0)&&(matrix[5]==0.0)&&(matrix[9]!=0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[2]=-matrix[13]/matrix[9];
			done[2]=TRUE;
		}
		else if((matrix[1]==0.0)&&(matrix[5]!=0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[1]=-matrix[13]/matrix[5];
			done[1]=TRUE;
		}
		else if((matrix[1]!=0.0)&&(matrix[5]==0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[0]=-matrix[13]/matrix[1];
			done[0]=TRUE;
		}

		if((matrix[2]==0.0)&&(matrix[6]==0.0)&&(matrix[10]!=0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[2]=-matrix[14]/matrix[10];
			done[2]=TRUE;
		}
		else if((matrix[2]==0.0)&&(matrix[6]!=0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[1]=-matrix[14]/matrix[6];
			done[1]=TRUE;
		}
		else if((matrix[2]!=0.0)&&(matrix[6]==0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[0]=-matrix[14]/matrix[2];
			done[0]=TRUE;
		}

		//Were all the components filled?
		if((done[0])&&(done[1])&&(done[2]))
			is_eyePosition3D_Solved=TRUE;
		else
			is_eyePosition3D_Solved=FALSE;
	}

	if(is_eyePosition3D_Solved)
	{
		//Compute center3D
		NormalizeVectorFLOAT_2(lookAt);

		center3D[0]=eyePosition3D[0]+lookAt[0];
		center3D[1]=eyePosition3D[1]+lookAt[1];
		center3D[2]=eyePosition3D[2]+lookAt[2];
		return 1;
	}
	else
		return -1;*/
}

sint glhExtractLookAtFromMatrixd2(sreal2 *matrix, sreal2 *eyePosition3D, sreal2 *center3D, sreal2 *upVector3D)
{
	sreal2 rightVector[3], newUpVector[3], lookAt[3];
	sreal2 ftemp;
	sreal2 factors[4], newRightVector[3];
	sreal2 newRightVector2[3];
	sreal2 newMatrix_12_13_14[3], newMatrix_12_13_14_2[3];
	
	sint is_eyePosition3D_Solved=FALSE;

	sint done[3];

	rightVector[0]=matrix[0];
	rightVector[1]=matrix[4];
	rightVector[2]=matrix[8];

	//We expect this to be normalized
	upVector3D[0]=matrix[1];
	upVector3D[1]=matrix[5];
	upVector3D[2]=matrix[9];

	//These were negated originally
	lookAt[0]=-matrix[2];
	lookAt[1]=-matrix[6];
	lookAt[2]=-matrix[10];

	//**** Try to solve a system of 3 equations ****
	//
	//[12] contains -(rightVector DOT eyePosition)
	//[13] contains -(upVector DOT eyePosition)
	//[14] contains -(lookVector DOT eye Position)
	//
	//The unknown is eyePosition. The others are already inside the matrix
	//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
	//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
	//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
	//
	if((rightVector[0]!=0.0)&&(upVector3D[0]!=0.0)&&(lookAt[0]!=0.0))		//Else we have problems!
	{
		ftemp=rightVector[0]/upVector3D[0];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=0.0;		//This becomes eliminated
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.x = 0.0 so
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate X from equation #2 using equation #3
		ftemp=upVector3D[0]/lookAt[0];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=0.0;		//This becomes eliminated
		newUpVector[1]=upVector3D[1]-factors[1];
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.y * Y + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[1]!=0.0)&&(newUpVector[1]!=0.0))
		{
			ftemp=newRightVector[1]/newUpVector[1];

			factors[1]=ftemp*newUpVector[1];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[1]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			if(newRightVector2[2]!=0.0)
			{
				eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

				//Plug in Z into previous 2 variable equation to get Y
				eyePosition3D[1]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[1];

				//Now we have Y and Z. Solve for X using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[0]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[1]*eyePosition3D[1]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}


	if((rightVector[1]!=0.0)&&(upVector3D[1]!=0.0)&&(lookAt[1]!=0.0)&&(is_eyePosition3D_Solved==FALSE))		//Else we have problems!
	{
		ftemp=rightVector[1]/upVector3D[1];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=0.0;		//This becomes eliminated
		newRightVector[2]=rightVector[2]-factors[2];

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.y = 0.0 so
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]

		//Now eliminate Y from equation #2 using equation #3
		ftemp=upVector3D[1]/lookAt[1];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=0.0;		//This becomes eliminated
		newUpVector[2]=upVector3D[2]-factors[2];

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.z * Z = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.z * Z = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[2]=newUpVector[2]-factors[2];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.z * Z = newMatrix_12_13_14_2[0]
			//and it is easy to find Z
			if(newRightVector2[2]!=0.0)
			{
				eyePosition3D[2]=newMatrix_12_13_14_2[0]/newRightVector2[2];

				//Plug in Z into previous 2 variable equation to get X
				eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[2]*eyePosition3D[2]))/newRightVector[0];

				//Now we have X and Z. Solve for Y using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[1]=(-matrix[12])-(rightVector[2]*eyePosition3D[2])-(rightVector[0]*eyePosition3D[0]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}


	if((rightVector[2]!=0.0)&&(upVector3D[2]!=0.0)&&(lookAt[2]!=0.0)&&(is_eyePosition3D_Solved==FALSE))		//Else we have problems!
	{
		ftemp=rightVector[2]/upVector3D[2];

		factors[0]=ftemp*upVector3D[0];
		factors[1]=ftemp*upVector3D[1];
		factors[2]=ftemp*upVector3D[2];
		factors[3]=ftemp*(-matrix[13]);

		newRightVector[0]=rightVector[0]-factors[0];
		newRightVector[1]=rightVector[1]-factors[1];
		newRightVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[0]=(-matrix[12])-factors[3];

		//We now have
		//newRightVector.x * X + newRightVector.y * Y + newRightVector.z * Z = newMatrix_12_13_14[0]
		//where newRightVector.z = 0.0 so
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]

		//Now eliminate Z from equation #2 using equation #3
		ftemp=upVector3D[2]/lookAt[2];

		factors[0]=ftemp*lookAt[0];
		factors[1]=ftemp*lookAt[1];
		factors[2]=ftemp*lookAt[2];
		factors[3]=ftemp*(-matrix[14]);

		newUpVector[0]=upVector3D[0]-factors[0];
		newUpVector[1]=upVector3D[2]-factors[2];
		newUpVector[2]=0.0;		//This becomes eliminated

		newMatrix_12_13_14[1]=(-matrix[13])-factors[3];

		//Now we have 2 equations with 2 unknowns
		//newRightVector.x * X + newRightVector.y * Y = newMatrix_12_13_14[0]
		//newUpVector.x * X + newUpVector.y * Y = newMatrix_12_13_14[1]

		if((newRightVector[0]!=0.0)&&(newUpVector[0]!=0.0))
		{
			ftemp=newRightVector[0]/newUpVector[0];

			factors[0]=ftemp*newUpVector[0];
			factors[2]=ftemp*newUpVector[2];
			factors[3]=ftemp*newMatrix_12_13_14[1];


			newRightVector2[0]=0.0;		//This becomes eliminated
			newRightVector2[1]=newUpVector[1]-factors[1];

			newMatrix_12_13_14_2[0]=newMatrix_12_13_14[0]-factors[3];

			//Now we have a single equation : the first one
			//newRightVector2.y * Y = newMatrix_12_13_14_2[0]
			//and it is easy to find Y
			if(newRightVector2[1]!=0.0)
			{
				eyePosition3D[1]=newMatrix_12_13_14_2[0]/newRightVector2[1];

				//Plug in Y into previous 2 variable equation to get X
				eyePosition3D[0]=(newMatrix_12_13_14[0]-(newRightVector[1]*eyePosition3D[1]))/newRightVector[0];

				//Now we have X and Y. Solve for Z using the original equation
				//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
				eyePosition3D[2]=(-matrix[12])-(rightVector[1]*eyePosition3D[1])-(rightVector[0]*eyePosition3D[0]);

				is_eyePosition3D_Solved=TRUE;
			}
		}
	}


	if(is_eyePosition3D_Solved==FALSE)
	{	//Some of the entries in the upper left 3x3 matrix are zero

		done[0]=done[1]=done[2]=FALSE;		//To know if all the components are filled

		//Try to find eyePosition3D
		if((matrix[0]==0.0)&&(matrix[4]==0.0)&&(matrix[8]!=0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[2]=-matrix[12]/matrix[8];
			done[2]=TRUE;
		}
		else if((matrix[0]==0.0)&&(matrix[4]!=0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[1]=-matrix[12]/matrix[4];
			done[1]=TRUE;
		}
		else if((matrix[0]!=0.0)&&(matrix[4]==0.0)&&(matrix[8]==0.0))
		{
			//rightVector.x * X + rightVector.y * Y + rightVector.z * Z = -matrix[12]
			eyePosition3D[0]=-matrix[12]/matrix[0];
			done[0]=TRUE;
		}

		if((matrix[1]==0.0)&&(matrix[5]==0.0)&&(matrix[9]!=0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[2]=-matrix[13]/matrix[9];
			done[2]=TRUE;
		}
		else if((matrix[1]==0.0)&&(matrix[5]!=0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[1]=-matrix[13]/matrix[5];
			done[1]=TRUE;
		}
		else if((matrix[1]!=0.0)&&(matrix[5]==0.0)&&(matrix[9]==0.0))
		{
			//upVector.x * X + upVector.y * Y + upVector.z * Z = -matrix[13]
			eyePosition3D[0]=-matrix[13]/matrix[1];
			done[0]=TRUE;
		}

		if((matrix[2]==0.0)&&(matrix[6]==0.0)&&(matrix[10]!=0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[2]=-matrix[14]/matrix[10];
			done[2]=TRUE;
		}
		else if((matrix[2]==0.0)&&(matrix[6]!=0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[1]=-matrix[14]/matrix[6];
			done[1]=TRUE;
		}
		else if((matrix[2]!=0.0)&&(matrix[6]==0.0)&&(matrix[10]==0.0))
		{
			//lookVector.x * X + lookVector.y * Y + lookVector.z * Z = -matrix[14]
			eyePosition3D[0]=-matrix[14]/matrix[2];
			done[0]=TRUE;
		}

		//Were all the components filled?
		if((done[0])&&(done[1])&&(done[2]))
			is_eyePosition3D_Solved=TRUE;
		else
			is_eyePosition3D_Solved=FALSE;
	}

	if(is_eyePosition3D_Solved)
	{
		//Compute center3D
		NormalizeVectorDOUBLE_2(lookAt);

		center3D[0]=eyePosition3D[0]+lookAt[0];
		center3D[1]=eyePosition3D[1]+lookAt[1];
		center3D[2]=eyePosition3D[2]+lookAt[2];
		return 1;
	}
	else
		return -1;
}

sreal ClampTo_0_To_1_FLOAT(sreal fvalue)
{
	if(fvalue<=0.0)
		return 0.0;
	else if(fvalue>=1.0)
		return 1.0;
	else
		return 
		fvalue;
}

sint ClampTo_0_To_255_INT(sreal fvalue)
{
	if(fvalue<=0.0)
		return 0;
	else if(fvalue>=255.0)
		return 255;
	else
		return (sint)fvalue;
}

void glhComputeHSVWithRGBf(sreal &hue, sreal &saturation, sreal &value, sreal red, sreal green, sreal blue)
{
	//We dont touch Alpha, just Hue, Saturation and Value
	if((red==green)&&(green==blue))
	{
		hue=0.0;
		saturation=0.0;
		value=red;
	}
	else
	{
		//One or two of them are higher then the other color channel
		//Use that information to find out in which quadrant we are in
		//and compute Hue, Saturation and Value from that
		sint i;
		sreal h, f;
		if((red>=green)&&(red>=blue))
		{
			//Quadrant 0 or 5
			if(green>=blue)
				i=0;
			else
				i=5;
		}
		else if((green>=red)&&(green>=blue))
		{
			//Quadrant 1 or 2
			if(red>=blue)
				i=1;
			else
				i=2;
		}
		else if((blue>=red)&&(blue>=green))
		{
			//Quadrant 3 or 4
			if(green>=red)
				i=3;
			else
				i=4;
		}

		switch(i)
		{
		case 0:
			value=red;
			//We need to solve for Saturation first
			//1.0-(1.0-Green/Value)/Saturation=f); 
			//Blue/Value=(1.0-Saturation);
			saturation=1.0-blue/value;
			f=1.0-(1.0-green/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		case 1:
			value=green;
			//(1.0-Red/Value)/Saturation=f; 
			//1.0-Blue/Value=Saturation;
			saturation=1.0-blue/value;
			f=(1.0-red/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		case 2:
			value=green;
			//1.0-Red/Value=Saturation;
			//1.0-(1.0-Blue/Value)/Saturation=f; 
			saturation=1.0-red/value;
			f=1.0-(1.0-blue/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		case 3:
			value=blue;
			//1.0-Red/Value=Saturation;
			//(1.0-Green/Value)/Saturation=f; 
			saturation=1.0-red/value;
			f=(1.0-green/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		case 4:
			value=blue;
			//1.0-(1.0-Red/Value)/Saturation=f; 
			//1.0-Green/Value=Saturation;
			saturation=1.0-green/value;
			f=1.0-(1.0-red/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		case 5:
			value=red;
			//1.0-Green/Value=Saturation;
			//(1.0-Blue/Value)/Saturation=f; 
			saturation=1.0-green/value;
			f=(1.0-blue/value)/saturation;
			h=f+(sreal)i;	//This should be between 0.0 and 360.0
			hue=(h*60.0)/360.0;
			break;
		}
	}

	hue=ClampTo_0_To_1_FLOAT(hue);
	saturation=ClampTo_0_To_1_FLOAT(saturation);
	value=ClampTo_0_To_1_FLOAT(value);
}

void glhComputeRGBWithHSVf(sreal &red, sreal &green, sreal &blue, sreal hue, sreal saturation, sreal value)
{
	//HSV to RGB color space conversion.  From pg. 593 of Foley & van Dam. 
	if(saturation==0.0)
	{
		red=value;
		green=value;
		blue=value;
	}
	else
	{
		//Hue is a value between 0.0 adn 1.0, but map it to 0 degree to 360 degree
		sreal i, h, f, p, q, t;
		h=hue*360.0;
		if(h>=360.0)
			h=0.0;
		h=h/60.0;
		i=(sreal)floorf(h);
		f=h-i;
		p=value*(1.0-saturation);
		q=value*(1.0-(saturation*f)); 
		t=value*(1.0-(saturation*(1.0-f))); 
		switch((sint)i)	//In which quadrant is it in the hexagon
		{
		case 0:
			red=value;
			green=t;
			blue=p;
			break;
		case 1:
			red=q;
			green=value;
			blue=p;
			break;
		case 2:
			red=p;
			green=value;
			blue=t;
			break;
		case 3:
			red=p;
			green=q;
			blue=value;
			break;
		case 4:
			red=t;
			green=p;
			blue=value;
			break;
		case 5:
			red=value;
			green=p;
			blue=q;
			break;
		}
	}

	red=ClampTo_0_To_1_FLOAT(red);
	green=ClampTo_0_To_1_FLOAT(green);
	blue=ClampTo_0_To_1_FLOAT(blue);
}

void glhComputeBGRAWithHSVfub(uint &bgraColor, sreal hue, sreal saturation, sreal value, sreal alpha)
{
	//HSV to RGB color space conversion.  From pg. 593 of Foley & van Dam. 
	//Use the RGB + ALPHA to create the HEX value 
	if(saturation==0.0)
	{
		sint red2=(sint)(255.0*value);
		sint green2=(sint)(255.0*value);
		sint blue2=(sint)(255.0*value);
		sint alpha2=(sint)(255.0*alpha);
		bgraColor=((alpha2<<24) | (red2<<16) | (green2<<8) | (blue2));
	}
	else
	{
		//Hue is a value between 0.0 adn 1.0, but map it to 0 degree to 360 degree
		sint red2, green2, blue2, alpha2;
		sreal i, h, f, p, q, t;
		h=hue*360.0;
		if(h>=360.0)
			h=0.0;
		h=h/60.0;
		i=(sreal)floorf(h);
		f=h-i;
		p=value*(1.0-saturation);
		q=value*(1.0-(saturation*f)); 
		t=value*(1.0-(saturation*(1.0-f))); 
		switch((sint)i)
		{
		case 0:
			red2=(sint)(255.0*value);
			green2=(sint)(255.0*t);
			blue2=(sint)(255.0*p);
			break;
		case 1:
			red2=(sint)(255.0*q);
			green2=(sint)(255.0*value);
			blue2=(sint)(255.0*p);
			break;
		case 2:
			red2=(sint)(255.0*p);
			green2=(sint)(255.0*value);
			blue2=(sint)(255.0*t);
			break;
		case 3:
			red2=(sint)(255.0*p);
			green2=(sint)(255.0*q);
			blue2=(sint)(255.0*value);
			break;
		case 4:
			red2=(sint)(255.0*t);
			green2=(sint)(255.0*p);
			blue2=(sint)(255.0*value);
			break;
		case 5:
			red2=(sint)(255.0*value);
			green2=(sint)(255.0*p);
			blue2=(sint)(255.0*q);
			break;
		}

		alpha2=(sint)(255.0*alpha);
		bgraColor=((alpha2<<24) | (red2<<16) | (green2<<8) | (blue2));
	}
}


void glhComputeRGBAWithHSVfub(uint &rgbaColor, sreal hue, sreal saturation, sreal value, sreal alpha)
{
	//HSV to RGB color space conversion.  From pg. 593 of Foley & van Dam. 
	//Use the RGB + ALPHA to create the HEX value 
	if(saturation==0.0)
	{
		sint red2=(sint)(255.0*value);
		sint green2=(sint)(255.0*value);
		sint blue2=(sint)(255.0*value);
		sint alpha2=(sint)(255.0*alpha);
		rgbaColor=((alpha2<<24) | (blue2<<16) | (green2<<8) | (red2));
	}
	else
	{
		//Hue is a value between 0.0 adn 1.0, but map it to 0 degree to 360 degree
		sint red2, green2, blue2, alpha2;
		sreal i, h, f, p, q, t;
		h=hue*360.0;
		if(h>=360.0)
			h=0.0;
		h=h/60.0;
		i=(sreal)floorf(h);
		f=h-i;
		p=value*(1.0-saturation);
		q=value*(1.0-(saturation*f)); 
		t=value*(1.0-(saturation*(1.0-f))); 
		switch((sint)i)
		{
		case 0:
			red2=(sint)(255.0*value);
			green2=(sint)(255.0*t);
			blue2=(sint)(255.0*p);
			break;
		case 1:
			red2=(sint)(255.0*q);
			green2=(sint)(255.0*value);
			blue2=(sint)(255.0*p);
			break;
		case 2:
			red2=(sint)(255.0*p);
			green2=(sint)(255.0*value);
			blue2=(sint)(255.0*t);
			break;
		case 3:
			red2=(sint)(255.0*p);
			green2=(sint)(255.0*q);
			blue2=(sint)(255.0*value);
			break;
		case 4:
			red2=(sint)(255.0*t);
			green2=(sint)(255.0*p);
			blue2=(sint)(255.0*value);
			break;
		case 5:
			red2=(sint)(255.0*value);
			green2=(sint)(255.0*p);
			blue2=(sint)(255.0*q);
			break;
		}

		alpha2=(sint)(255.0*alpha);
		rgbaColor=((alpha2<<24) | (blue2<<16) | (green2<<8) | (red2));
	}
}

void glhComputeHSVWithBGRAubf(sreal &hue, sreal &saturation, sreal &value, sreal &alpha, uint bgraColor)
{
	//Read AARRGGBB from hex and write to Red, Green, Blue, Alpha
	alpha=((sreal)((0xFF000000 & bgraColor)>>24))/255.0;
	sreal red=((sreal)((0x00FF0000 & bgraColor)>>16))/255.0;
	sreal green=((sreal)((0x0000FF00 & bgraColor)>>8))/255.0;
	sreal blue=((sreal)((0x000000FF & bgraColor)))/255.0;
	glhComputeHSVWithRGBf(hue, saturation, value, red, green, blue);
}

void glhComputeHSVWithRGBAubf(sreal &hue, sreal &saturation, sreal &value, sreal &alpha, uint rgbaColor)
{
	//Read AARRGGBB from hex and write to Red, Green, Blue, Alpha
	alpha=((sreal)((0xFF000000 & rgbaColor)>>24))/255.0;
	sreal blue=((sreal)((0x00FF0000 & rgbaColor)>>16))/255.0;
	sreal green=((sreal)((0x0000FF00 & rgbaColor)>>8))/255.0;
	sreal red=((sreal)((0x000000FF & rgbaColor)))/255.0;
	glhComputeHSVWithRGBf(hue, saturation, value, red, green, blue);
}

