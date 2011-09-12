// glhlib2.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "glhlib.h"
#include "jitter.h"		//A file created by SGI (The Red Book 1.1 page 407 table 10-5)
						//jitter.h has been modified!!! Values from table10-5 have been added)
#include <GL/gl.h>
#include <GL/glext.h>
#include <math.h>
#include <string.h>
#include "StringFunctions.h"
#include "MathLibrary.h"


#pragma warning(disable: 4244)	//Shut up about sreal2 to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const sreal2' to 'sreal'

#ifndef M_PI
#define M_PI	3.14159265358979323846
#endif

#ifndef GL_INVALID_FRAMEBUFFER_OPERATION_EXT
#define GL_INVALID_FRAMEBUFFER_OPERATION_EXT		0x0506
#endif

//This is from jitter.h (jitter_point) and is used by
//RenderWith_DepthOfField_SceneAntialiased() for depth of field effect
//and scene antialiasing.
static jitter_point *jitter_values_array[]=
{
	{0},				//We start from 0! jitter_values_array[0]
	{0},
	{&j2[0]},
	{&j3[0]},
	{&j4[0]},
	{&j5[0]},
	{&j6[0]},
	{0},
	{&j8[0]},
	{&j9[0]},
	{0},
	{0},
	{&j12[0]},
	{0},
	{0},
	{&j15[0]},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{0},
	{&j24[0]},
	{&j66[0]}		//Need to add more NULL or 0 before j66
};


static sreal2 GlobalIdentityMatrixd[]=
	{1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0};

static sreal GlobalIdentityMatrixf[]=
	{1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0};

//////////////////////////////////////////////////////////////////








#if USE_INLINE_ASM

void glhTexSubImage2DMipmaps(sint xoffset,
								 sint yoffset, sint width, sint height,
								 sint textureWidth, sint textureHeight,
								 uint format,
								 uint type, const void *pixels)
{
	if(type!=GL_UNSIGNED_BYTE)
		return;
	sint components;
	switch(format)
	{
	case GL_RGB:
	case GL_RGB8:
		components=3;
		goto GoodJump;
		break;
	case GL_RGBA:
	case GL_RGBA8:
		components=4;
		goto GoodJump;
		break;
	default:
		return;
	}
	GoodJump:
	//For level=0
	glTexSubImage2D(GL_TEXTURE_2D, 0, xoffset, yoffset, width, height, format, type, pixels);
	if((textureWidth==1)&&(textureHeight==1))
		return;		//We are done, no other mipmaps
	//Now we need to figure out how many mipmap level there are.
	sint i, totalLevels;
	sint w, h;
	sint new_xoffset, new_yoffset;
	sint new_width, new_height;
	uchar *new_pixels;
	w=textureWidth;
	h=textureHeight;
	totalLevels=0;
	while(1)
	{
		if(w!=1)		//Must not be 1 otherwise it will become 0
			w/=2;
		if(h!=1)		//Must not be 1 otherwise it will become 0
			h/=2;
		totalLevels++;
		if((w==1)&&(h==1))
			break;
	}
	new_xoffset=xoffset;
	new_yoffset=yoffset;
	new_width=width;
	new_height=height;
	new_pixels=new uchar[textureWidth*textureHeight*components];
	if(new_pixels==0)		//Memory error
		return;
	for(i=1; i<=totalLevels; i++)
	{
		//Must rescale the "subpixels"
		new_xoffset/=2;
		new_yoffset/=2;
		new_width/=2;
		new_height/=2;
		glhScaleImage_asm386(format, TRUE, 1, width, height, (uchar *)pixels,
			new_width, new_height, (uchar *)new_pixels);
		glTexSubImage2D(GL_TEXTURE_2D, (sint)i, new_xoffset, new_yoffset,
			new_width, new_height, format, type, new_pixels);
	}
	delete [] new_pixels;
}
#endif // #if USE_INLINE_ASM


//PURPOSE: These 2 functions are for transposing a 4x4 matrix (sreal and sreal2)
void glhTransposeMatrixf2(sreal *result, sreal *m)
{
	result[0]=m[0];
	result[1]=m[4];
	result[2]=m[8];
	result[3]=m[12];
	result[4]=m[1];
	result[5]=m[5];
	result[6]=m[9];
	result[7]=m[13];
	result[8]=m[2];
	result[9]=m[6];
	result[10]=m[10];
	result[11]=m[14];
	result[12]=m[3];
	result[13]=m[7];
	result[14]=m[11];
	result[15]=m[15];
}
void glhTransposeMatrixd2(sreal2 *result, sreal2 *m)
{
	result[0]=m[0];
	result[1]=m[4];
	result[2]=m[8];
	result[3]=m[12];
	result[4]=m[1];
	result[5]=m[5];
	result[6]=m[9];
	result[7]=m[13];
	result[8]=m[2];
	result[9]=m[6];
	result[10]=m[10];
	result[11]=m[14];
	result[12]=m[3];
	result[13]=m[7];
	result[14]=m[11];
	result[15]=m[15];
}

//OBSOLETE: glhAccumFrustumf, glhAccumPerspectivef, glhRender_DOF_SceneAAf
//Keeping the code here for future reference. 
//PURPOSE: The following 2 functions are from the red book and can be used
//         for scene antialiasing and for "depth of field" effect.
//         Both are quite slow on hardware that don't have a hardware
//         support for the accumulation buffer.
//         This function will modify the projection and modelview matrices
//         in the end and will exit while being in modelview
//         matrix mode.
//         pixdx and pixdy are the jitter values (of less than 1 pixel).
//         eyedx eyedy focus are used for DOF effect and focus must be nonzero
//         or else a division by zero will occur.
//Both are from the Red Book (accpersp.c)
/*void glhAccumFrustumf(sreal left, sreal right, sreal bottom, sreal top,
						sreal zNear, sreal zFar, sreal pixdx, sreal pixdy,
						sreal eyedx, sreal eyedy, sreal focus, sint *viewport)
{
	sreal xwsize, ywsize;
	sreal dx, dy;
	sreal projectionMatrix[16];
	
	xwsize=right-left;
	ywsize=top-bottom;
	dx=-(pixdx*xwsize/(sreal)viewport[2]+eyedx*zNear/focus);
	dy=-(pixdy*ywsize/(sreal)viewport[3]+eyedy*zNear/focus);

	//glLoadIdentity();
	//glFrustum(sreal2(left+dx), sreal2(right+dx), sreal2(bottom+dy),
	//	sreal2(top+dy), zNear, zFar);

	glhLoadIdentityf2(projectionMatrix);
	glhFrustumf2(projectionMatrix, left+dx, right+dx, bottom+dy,
		top+dy, zNear, zFar);
	glLoadMatrixf(projectionMatrix);
}*/

//OBSOLETE: glhAccumFrustumf, glhAccumPerspectivef, glhRender_DOF_SceneAAf
//Keeping the code here for future reference
/*void glhAccumPerspectivef(sreal fovyInDegrees, sreal aspectRatio, sreal zNear, sreal zFar,
					  sreal pixdx, sreal pixdy, sreal eyedx, sreal eyedy,
					  sreal focus, sint *viewport)
{
	sreal left, right, bottom, top;
	//M_PI=3.14159265358979323846
	top=zNear*tanf((fovyInDegrees*M_PI)/360.0);
	bottom=-top;
	right=top*aspectRatio;
	left=-right;

	glhAccumFrustumf(left, right, bottom, top, zNear, zFar, pixdx, pixdy, eyedx, eyedy, focus, viewport);
}*/

//OBSOLETE: glhAccumFrustumf, glhAccumPerspectivef, glhRender_DOF_SceneAAf
//Keeping the code here for future reference
/*sint glhRender_DOF_SceneAAf(sreal fovyInDegrees, sreal aspectRatio,
					sreal zNear, sreal zFar, sreal pixdx, sreal pixdy,
					sreal focus, sint AccumQuality, sreal jitterFactor, sint &jitter, sint *viewport)
{
	if(jitter_values_array[AccumQuality]==0)
		return -1;
	if(jitter>0)
	{
		glAccum(GL_ACCUM, 1.0/(sreal)AccumQuality);
		if(jitter>=AccumQuality)
		{
			glAccum(GL_RETURN, 1.0);		//Copy to back buffer (if sreal2 buffer on)
			return 1;
		}
	}
	else
		glClear(GL_ACCUM_BUFFER_BIT);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glhAccumPerspectivef(fovyInDegrees, aspectRatio,
		zNear, zFar,
		pixdx, pixdy,
		jitterFactor*jitter_values_array[AccumQuality][jitter].x,
		jitterFactor*jitter_values_array[AccumQuality][jitter].y,
		focus, viewport);
	jitter++;
	return 2;
}*/





void glhInitializeHints(uint mode)
{
	glHint(GL_FOG_HINT, mode);
	glHint(GL_LINE_SMOOTH_HINT, mode);
	glHint(GL_POINT_SMOOTH_HINT, mode);
	glHint(GL_POLYGON_SMOOTH_HINT, mode);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, mode);
}




//glOrtho(0.0, Width, 0.0, Height, 1.0, -1.0);        <--- use this
//Set modelview to identity
//modelview is 16 elements, projection is 16, viewport is 4
//windowcoordinate is 3.
//This is only correct when glDepthRange(0.0, 1.0)
//Returns 1 if OK else 0.
sint glhProjectd(sreal2 objx, sreal2 objy, sreal2 objz,
				 sreal2 *modelview, sreal2 *projection,
				 sint *viewport,
				 sreal2 *windowCoordinate)
{
	//Transformation vectors
	sreal2 fTempo[8];
	//Modelview transform
	fTempo[0]=modelview[0]*objx+
		modelview[4]*objy+
		modelview[8]*objz+
		modelview[12];		//w is always 1
	fTempo[1]=modelview[1]*objx+
		modelview[5]*objy+
		modelview[9]*objz+
		modelview[13];
	fTempo[2]=modelview[2]*objx+
		modelview[6]*objy+
		modelview[10]*objz+
		modelview[14];
	fTempo[3]=modelview[3]*objx+
		modelview[7]*objy+
		modelview[11]*objz+
		modelview[15];
	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4]=projection[0]*fTempo[0]+	
		projection[4]*fTempo[1]+
		projection[8]*fTempo[2]+
		projection[12]*fTempo[3];
	fTempo[5]=projection[1]*fTempo[0]+	
		projection[5]*fTempo[1]+
		projection[9]*fTempo[2]+
		projection[13]*fTempo[3];
	fTempo[6]=projection[2]*fTempo[0]+	
		projection[6]*fTempo[1]+
		projection[10]*fTempo[2]+
		projection[14]*fTempo[3];
	fTempo[7]=-fTempo[2];

	//The result normalizes between -1 and 1
	if(fTempo[7]==0.0)	//The w value
		return 0;

	fTempo[7]=1.0/fTempo[7];

	//Perspective division
	fTempo[4]*=fTempo[7];
	fTempo[5]*=fTempo[7];
	fTempo[6]*=fTempo[7];

	//Window coordinates
	//Map x, y to range 0-1
	windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
	windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];

	//This is only correct when glDepthRange(0.0, 1.0)
	windowCoordinate[2]=(1.0+fTempo[6])*0.5;	//Between 0 and 1
	return 1;
}

//glOrtho(0.0, Width, 0.0, Height, 1.0, -1.0);  <--- use this
//Set modelview to identity
//This is only correct when glDepthRange(0.0, 1.0)
sint glhProjectf(sreal objx, sreal objy, sreal objz,
				 sreal *modelview, sreal *projection,
				 sint *viewport,
				 sreal *windowCoordinate)
{
	//Transformation vectors
	sreal fTempo[8];
	//Modelview transform
	fTempo[0]=modelview[0]*objx+
		modelview[4]*objy+
		modelview[8]*objz+
		modelview[12];		//w is always 1
	fTempo[1]=modelview[1]*objx+
		modelview[5]*objy+
		modelview[9]*objz+
		modelview[13];
	fTempo[2]=modelview[2]*objx+
		modelview[6]*objy+
		modelview[10]*objz+
		modelview[14];
	fTempo[3]=modelview[3]*objx+
		modelview[7]*objy+
		modelview[11]*objz+
		modelview[15];
	//Projection transform, the final row of projection matrix is always [0 0 -1 0]
	//so we optimize for that.
	fTempo[4]=projection[0]*fTempo[0]+	
		projection[4]*fTempo[1]+
		projection[8]*fTempo[2]+
		projection[12]*fTempo[3];
	fTempo[5]=projection[1]*fTempo[0]+	
		projection[5]*fTempo[1]+
		projection[9]*fTempo[2]+
		projection[13]*fTempo[3];
	fTempo[6]=projection[2]*fTempo[0]+	
		projection[6]*fTempo[1]+
		projection[10]*fTempo[2]+
		projection[14]*fTempo[3];
	fTempo[7]=-fTempo[2];

	//The result normalizes between -1 and 1
	if(fTempo[7]==0.0)	//The w value
		return 0;

	fTempo[7]=1.0/fTempo[7];

	//Perspective division
	fTempo[4]*=fTempo[7];
	fTempo[5]*=fTempo[7];
	fTempo[6]*=fTempo[7];

	//Window coordinates
	//Map x, y to range 0-1
	windowCoordinate[0]=(fTempo[4]*0.5+0.5)*viewport[2]+viewport[0];
	windowCoordinate[1]=(fTempo[5]*0.5+0.5)*viewport[3]+viewport[1];

	//This is only correct when glDepthRange(0.0, 1.0)
	windowCoordinate[2]=(1.0+fTempo[6])*0.5;	//Between 0 and 1
	return 1;
}

//modelview is 16 elements, projection is 16, viewport is 4
//objectCoordinates is 3.
//Returns 1 if OK else 0.
sint glhUnProjectd(sreal2 winx, sreal2 winy, sreal2 winz,
	     sreal2 *modelview, sreal2 *projection,
	     sint *viewport,
	     sreal2 *objectCoordinate)
{
	//Transformation matrices
	sreal2 m[16], A[16];
	sreal2 in[4], out[4];

	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	MultiplyMatrices4by4OpenGL_DOUBLE_3(A, projection, modelview);
	//Now compute the inverse of matrix A
	if(glhInvertMatrixd2(A, m)==0)
		return 0;

	//Transformation of normalized coordinates between -1 and 1
	in[0]=(winx-(sreal2)viewport[0])/(sreal2)viewport[2]*2.0-1.0;
	in[1]=(winy-(sreal2)viewport[1])/(sreal2)viewport[3]*2.0-1.0;
	in[2]=2.0*winz-1.0;
	in[3]=1.0;
	
	//Objects coordinates
	MultiplyMatrixByVector4by4OpenGL_DOUBLE(out, m, in);
	if(out[3]==0.0)
		return 0;
	out[3]=1.0/out[3];
	objectCoordinate[0]=out[0]*out[3];
	objectCoordinate[1]=out[1]*out[3];
	objectCoordinate[2]=out[2]*out[3];
	return 1;
}

sint glhUnProjectf(sreal winx, sreal winy, sreal winz,
	     sreal *modelview, sreal *projection,
	     sint *viewport,
	     sreal *objectCoordinate)
{
	//Transformation matrices
	sreal m[16], A[16];
	sreal in[4], out[4];

	//Calculation for inverting a matrix, compute projection x modelview
	//and store in A[16]
	MultiplyMatrices4by4OpenGL_FLOAT(A, projection, modelview);
	//Now compute the inverse of matrix A
	if(glhInvertMatrixf2(A, m)==0)
		return 0;

	//Transformation of normalized coordinates between -1 and 1
	in[0]=(winx-(sreal)viewport[0])/(sreal)viewport[2]*2.0-1.0;
	in[1]=(winy-(sreal)viewport[1])/(sreal)viewport[3]*2.0-1.0;
	in[2]=2.0*winz-1.0;
	in[3]=1.0;
	
	//Objects coordinates
	MultiplyMatrixByVector4by4OpenGL_FLOAT(out, m, in);
	if(out[3]==0.0)
		return 0;
	out[3]=1.0/out[3];
	objectCoordinate[0]=out[0]*out[3];
	objectCoordinate[1]=out[1]*out[3];
	objectCoordinate[2]=out[2]*out[3];
	return 1;
}


#define SWAP_ROWS_DOUBLE(a, b) { sreal2 *_tmp = a; (a)=(b); (b)=_tmp; }
#define SWAP_ROWS_FLOAT(a, b) { sreal *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,r,c) (m)[(c)*4+(r)]


//Compute inverse of 4x4 transformation matrix.
//Code contributed by Jacques Leroy jle@star.be
//Return 1 for success, 0 for failure (singular matrix)
//From MESA 3.2 GLU source
sint glhInvertMatrixd2(sreal2 *m, sreal2 *out)
{
	sreal2 wtmp[4][8];
	sreal2 m0, m1, m2, m3, s;
	sreal2 *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
      r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
      r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
      r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
      r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
      r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
      r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
      r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
      r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
      r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
      r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
      r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabs(r3[0]) > fabs(r2[0]))
      SWAP_ROWS_DOUBLE(r3, r2);
   if (fabs(r2[0]) > fabs(r1[0]))
      SWAP_ROWS_DOUBLE(r2, r1);
   if (fabs(r1[0]) > fabs(r0[0]))
      SWAP_ROWS_DOUBLE(r1, r0);
   if (0.0 == r0[0])
      return 0;

   /* eliminate first variable     */
   m1 = r1[0] / r0[0];
   m2 = r2[0] / r0[0];
   m3 = r3[0] / r0[0];
   s = r0[1];
   r1[1] -= m1 * s;
   r2[1] -= m2 * s;
   r3[1] -= m3 * s;
   s = r0[2];
   r1[2] -= m1 * s;
   r2[2] -= m2 * s;
   r3[2] -= m3 * s;
   s = r0[3];
   r1[3] -= m1 * s;
   r2[3] -= m2 * s;
   r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) {
      r1[4] -= m1 * s;
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r0[5];
   if (s != 0.0) {
      r1[5] -= m1 * s;
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r0[6];
   if (s != 0.0) {
      r1[6] -= m1 * s;
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r0[7];
   if (s != 0.0) {
      r1[7] -= m1 * s;
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabs(r3[1]) > fabs(r2[1]))
      SWAP_ROWS_DOUBLE(r3, r2);
   if (fabs(r2[1]) > fabs(r1[1]))
      SWAP_ROWS_DOUBLE(r2, r1);
   if (0.0 == r1[1])
      return 0;

   /* eliminate second variable */
   m2 = r2[1] / r1[1];
   m3 = r3[1] / r1[1];
   r2[2] -= m2 * r1[2];
   r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3];
   r3[3] -= m3 * r1[3];
   s = r1[4];
   if (0.0 != s) {
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r1[5];
   if (0.0 != s) {
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r1[6];
   if (0.0 != s) {
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r1[7];
   if (0.0 != s) {
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabs(r3[2]) > fabs(r2[2]))
      SWAP_ROWS_DOUBLE(r3, r2);
   if (0.0 == r2[2])
      return 0;

   /* eliminate third variable */
   m3 = r3[2] / r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
      r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3])
      return 0;

   s = 1.0 / r3[3];		/* now back substitute row 3 */
   r3[4] *= s;
   r3[5] *= s;
   r3[6] *= s;
   r3[7] *= s;

   m2 = r2[3];			/* now back substitute row 2 */
   s = 1.0 / r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
      r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
      r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
      r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

   m1 = r1[2];			/* now back substitute row 1 */
   s = 1.0 / r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
      r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
      r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

   m0 = r0[1];			/* now back substitute row 0 */
   s = 1.0 / r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
      r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(out, 0, 0) = r0[4];
   MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
   MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
   MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
   MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
   MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
   MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
   MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
   MAT(out, 3, 3) = r3[7];

   return 1;
}

sint glhInvertMatrixf2(sreal *m, sreal *out)
{
   sreal wtmp[4][8];
   sreal m0, m1, m2, m3, s;
   sreal *r0, *r1, *r2, *r3;

   r0 = wtmp[0], r1 = wtmp[1], r2 = wtmp[2], r3 = wtmp[3];

   r0[0] = MAT(m, 0, 0), r0[1] = MAT(m, 0, 1),
      r0[2] = MAT(m, 0, 2), r0[3] = MAT(m, 0, 3),
      r0[4] = 1.0, r0[5] = r0[6] = r0[7] = 0.0,
      r1[0] = MAT(m, 1, 0), r1[1] = MAT(m, 1, 1),
      r1[2] = MAT(m, 1, 2), r1[3] = MAT(m, 1, 3),
      r1[5] = 1.0, r1[4] = r1[6] = r1[7] = 0.0,
      r2[0] = MAT(m, 2, 0), r2[1] = MAT(m, 2, 1),
      r2[2] = MAT(m, 2, 2), r2[3] = MAT(m, 2, 3),
      r2[6] = 1.0, r2[4] = r2[5] = r2[7] = 0.0,
      r3[0] = MAT(m, 3, 0), r3[1] = MAT(m, 3, 1),
      r3[2] = MAT(m, 3, 2), r3[3] = MAT(m, 3, 3),
      r3[7] = 1.0, r3[4] = r3[5] = r3[6] = 0.0;

   /* choose pivot - or die */
   if (fabsf(r3[0]) > fabsf(r2[0]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (fabsf(r2[0]) > fabsf(r1[0]))
      SWAP_ROWS_FLOAT(r2, r1);
   if (fabsf(r1[0]) > fabsf(r0[0]))
      SWAP_ROWS_FLOAT(r1, r0);
   if (0.0 == r0[0])
      return 0;

   /* eliminate first variable     */
   m1 = r1[0] / r0[0];
   m2 = r2[0] / r0[0];
   m3 = r3[0] / r0[0];
   s = r0[1];
   r1[1] -= m1 * s;
   r2[1] -= m2 * s;
   r3[1] -= m3 * s;
   s = r0[2];
   r1[2] -= m1 * s;
   r2[2] -= m2 * s;
   r3[2] -= m3 * s;
   s = r0[3];
   r1[3] -= m1 * s;
   r2[3] -= m2 * s;
   r3[3] -= m3 * s;
   s = r0[4];
   if (s != 0.0) {
      r1[4] -= m1 * s;
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r0[5];
   if (s != 0.0) {
      r1[5] -= m1 * s;
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r0[6];
   if (s != 0.0) {
      r1[6] -= m1 * s;
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r0[7];
   if (s != 0.0) {
      r1[7] -= m1 * s;
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabsf(r3[1]) > fabsf(r2[1]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (fabsf(r2[1]) > fabsf(r1[1]))
      SWAP_ROWS_FLOAT(r2, r1);
   if (0.0 == r1[1])
      return 0;

   /* eliminate second variable */
   m2 = r2[1] / r1[1];
   m3 = r3[1] / r1[1];
   r2[2] -= m2 * r1[2];
   r3[2] -= m3 * r1[2];
   r2[3] -= m2 * r1[3];
   r3[3] -= m3 * r1[3];
   s = r1[4];
   if (0.0 != s) {
      r2[4] -= m2 * s;
      r3[4] -= m3 * s;
   }
   s = r1[5];
   if (0.0 != s) {
      r2[5] -= m2 * s;
      r3[5] -= m3 * s;
   }
   s = r1[6];
   if (0.0 != s) {
      r2[6] -= m2 * s;
      r3[6] -= m3 * s;
   }
   s = r1[7];
   if (0.0 != s) {
      r2[7] -= m2 * s;
      r3[7] -= m3 * s;
   }

   /* choose pivot - or die */
   if (fabsf(r3[2]) > fabsf(r2[2]))
      SWAP_ROWS_FLOAT(r3, r2);
   if (0.0 == r2[2])
      return 0;

   /* eliminate third variable */
   m3 = r3[2] / r2[2];
   r3[3] -= m3 * r2[3], r3[4] -= m3 * r2[4],
      r3[5] -= m3 * r2[5], r3[6] -= m3 * r2[6], r3[7] -= m3 * r2[7];

   /* last check */
   if (0.0 == r3[3])
      return 0;

   s = 1.0 / r3[3];		/* now back substitute row 3 */
   r3[4] *= s;
   r3[5] *= s;
   r3[6] *= s;
   r3[7] *= s;

   m2 = r2[3];			/* now back substitute row 2 */
   s = 1.0 / r2[2];
   r2[4] = s * (r2[4] - r3[4] * m2), r2[5] = s * (r2[5] - r3[5] * m2),
      r2[6] = s * (r2[6] - r3[6] * m2), r2[7] = s * (r2[7] - r3[7] * m2);
   m1 = r1[3];
   r1[4] -= r3[4] * m1, r1[5] -= r3[5] * m1,
      r1[6] -= r3[6] * m1, r1[7] -= r3[7] * m1;
   m0 = r0[3];
   r0[4] -= r3[4] * m0, r0[5] -= r3[5] * m0,
      r0[6] -= r3[6] * m0, r0[7] -= r3[7] * m0;

   m1 = r1[2];			/* now back substitute row 1 */
   s = 1.0 / r1[1];
   r1[4] = s * (r1[4] - r2[4] * m1), r1[5] = s * (r1[5] - r2[5] * m1),
      r1[6] = s * (r1[6] - r2[6] * m1), r1[7] = s * (r1[7] - r2[7] * m1);
   m0 = r0[2];
   r0[4] -= r2[4] * m0, r0[5] -= r2[5] * m0,
      r0[6] -= r2[6] * m0, r0[7] -= r2[7] * m0;

   m0 = r0[1];			/* now back substitute row 0 */
   s = 1.0 / r0[0];
   r0[4] = s * (r0[4] - r1[4] * m0), r0[5] = s * (r0[5] - r1[5] * m0),
      r0[6] = s * (r0[6] - r1[6] * m0), r0[7] = s * (r0[7] - r1[7] * m0);

   MAT(out, 0, 0) = r0[4];
   MAT(out, 0, 1) = r0[5], MAT(out, 0, 2) = r0[6];
   MAT(out, 0, 3) = r0[7], MAT(out, 1, 0) = r1[4];
   MAT(out, 1, 1) = r1[5], MAT(out, 1, 2) = r1[6];
   MAT(out, 1, 3) = r1[7], MAT(out, 2, 0) = r2[4];
   MAT(out, 2, 1) = r2[5], MAT(out, 2, 2) = r2[6];
   MAT(out, 2, 3) = r2[7], MAT(out, 3, 0) = r3[4];
   MAT(out, 3, 1) = r3[5], MAT(out, 3, 2) = r3[6];
   MAT(out, 3, 3) = r3[7];

   return 1;
}

sint glhCopyOpenGLErrorInto_cBuffer(schar *pstring)
{
	if(pstring==NULL)
		return 1;

	uint the_error=glGetError();
	if(the_error==GL_NO_ERROR)
	{
		strcpy(pstring, "GL_NO_ERROR");
		return 1;
	}
	else if(the_error==GL_INVALID_ENUM)
		strcpy(pstring, "GL_INVALID_ENUM");
	else if(the_error==GL_INVALID_VALUE)
		strcpy(pstring, "GL_INVALID_VALUE");
	else if(the_error==GL_INVALID_OPERATION)
		strcpy(pstring, "GL_INVALID_OPERATION");
	else if(the_error==GL_STACK_OVERFLOW)
		strcpy(pstring, "GL_STACK_OVERFLOW");
	else if(the_error==GL_STACK_UNDERFLOW)
		strcpy(pstring, "GL_STACK_UNDERFLOW");
	else if(the_error==GL_OUT_OF_MEMORY)
		strcpy(pstring, "GL_OUT_OF_MEMORY");
	else if(the_error==GL_INVALID_FRAMEBUFFER_OPERATION_EXT)	//Get rid of the _EXT in the future
		strcpy(pstring, "GL_INVALID_FRAMEBUFFER_OPERATION");	//At the time of this writting, glext.h was not updated
	else														//so I'm adding the define at the top of this file.
		pstring[0]='\0';

	return -1;
}

sint glhCopyOpenGLErrorInto_cBuffer2(schar *pstring, uint &glerror)
{
	if(pstring==NULL)
		return 1;

	glerror=glGetError();
	if(glerror==GL_NO_ERROR)
	{
		strcpy(pstring, "GL_NO_ERROR");
		return 1;
	}
	else if(glerror==GL_INVALID_ENUM)
		strcpy(pstring, "GL_INVALID_ENUM");
	else if(glerror==GL_INVALID_VALUE)
		strcpy(pstring, "GL_INVALID_VALUE");
	else if(glerror==GL_INVALID_OPERATION)
		strcpy(pstring, "GL_INVALID_OPERATION");
	else if(glerror==GL_STACK_OVERFLOW)
		strcpy(pstring, "GL_STACK_OVERFLOW");
	else if(glerror==GL_STACK_UNDERFLOW)
		strcpy(pstring, "GL_STACK_UNDERFLOW");
	else if(glerror==GL_OUT_OF_MEMORY)
		strcpy(pstring, "GL_OUT_OF_MEMORY");
	else if(glerror==GL_INVALID_FRAMEBUFFER_OPERATION_EXT)		//Get rid of the _EXT in the future
		strcpy(pstring, "GL_INVALID_FRAMEBUFFER_OPERATION");	//At the time of this writting, glext.h was not updated
	else														//so I'm adding the define at the top of this file.
		pstring[0]='\0';

	return -1;
}

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by undercompensating.
//			If not, returns 0, else 1.
//			If width == 0, *widthOpenGL ==1 and returns 1.
sint glhLowerPowerOfTwo(sint width, sint *widthOpenGL, sint height, sint *heightOpenGL)
{
	if(width==0)
	{
		*widthOpenGL=1;
		width=1;
	}
	else
	{
		sint start=1;
		while(start<width)
		{
			start<<=1;
		}
		if(start>width)
		{
			start>>=1;
			*widthOpenGL=start;
		}
		else //if(start==width)
			*widthOpenGL=start;
	}
	if(height==0)
	{
		*heightOpenGL=1;
		height=1;
	}
	else
	{
		sint start=1;
		while(start<height)
		{
			start<<=1;
		}
		if(start>height)
		{
			start>>=1;
			*heightOpenGL=start;
		}
		else //if(start==height)
			*heightOpenGL=start;
	}

	if((width==*widthOpenGL)&&(height==*heightOpenGL))
		return 1;
	else
		return 0;

	//OLD CODE - it can fail because of sreal precision (I noticed this in the Release version
	//but not in the DEBUG version for some reason
	/*sreal WidthPower2=logf((sreal)width)/(sreal)logf(2.0);
	*widthOpenGL=(sint)WidthPower2;
	WidthPower2=(sreal)*widthOpenGL;
	WidthPower2=powf(2.0, WidthPower2);
	*widthOpenGL=(sint)WidthPower2;
	
	sreal HeightPower2=logf((sreal)height)/(sreal)logf(2.0);
	*heightOpenGL=(sint)HeightPower2;
	HeightPower2=(sreal)*heightOpenGL; 
	HeightPower2=powf(2.0, HeightPower2);
	*heightOpenGL=(sint)HeightPower2;
	
	if((width==*widthOpenGL)&&(height==*heightOpenGL))
		return 1;
	else
		return 0;*/
}
//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by overcompensating.
//			If not, returns 0, else 1.
//			If width == 0, *widthOpenGL ==1 and returns 1.
sint glhHigherPowerOfTwo(sint width, sint *widthOpenGL, sint height, sint *heightOpenGL)
{
	if(width==0)
	{
		*widthOpenGL=1;
		width=1;
	}
	else
	{
		sint start=1;
		while(start<width)
		{
			start<<=1;
		}
		*widthOpenGL=start;
	}
	if(height==0)
	{
		*heightOpenGL=1;
		height=1;
	}
	else
	{
		sint start=1;
		while(start<height)
		{
			start<<=1;
		}
		*heightOpenGL=start;
	}

	if((width==*widthOpenGL)&&(height==*heightOpenGL))
		return 1;
	else
		return 0;
}






void glhTranslated2(sreal2 *matrix, sreal2 x, sreal2 y, sreal2 z)
{
	matrix[12]=matrix[0]*x+matrix[4]*y+matrix[8]*z+matrix[12];
	matrix[13]=matrix[1]*x+matrix[5]*y+matrix[9]*z+matrix[13];
	matrix[14]=matrix[2]*x+matrix[6]*y+matrix[10]*z+matrix[14];
	matrix[15]=matrix[3]*x+matrix[7]*y+matrix[11]*z+matrix[15];
}

void glhTranslatef2(sreal *matrix, sreal x, sreal y, sreal z)
{
	matrix[12]=matrix[0]*x+matrix[4]*y+matrix[8]*z+matrix[12];
	matrix[13]=matrix[1]*x+matrix[5]*y+matrix[9]*z+matrix[13];
	matrix[14]=matrix[2]*x+matrix[6]*y+matrix[10]*z+matrix[14];
	matrix[15]=matrix[3]*x+matrix[7]*y+matrix[11]*z+matrix[15];
}

void glhScaled2(sreal2 *matrix, sreal2 x, sreal2 y, sreal2 z)
{
	matrix[0]*=x;
	matrix[4]*=y;
	matrix[8]*=z;
	
	matrix[1]*=x;
	matrix[5]*=y;
	matrix[9]*=z;

	matrix[2]*=x;
	matrix[6]*=y;
	matrix[10]*=z;

	matrix[3]*=x;
	matrix[7]*=y;
	matrix[11]*=z;
}

void glhScalef2(sreal *matrix, sreal x, sreal y, sreal z)
{
	matrix[0]*=x;
	matrix[4]*=y;
	matrix[8]*=z;
	
	matrix[1]*=x;
	matrix[5]*=y;
	matrix[9]*=z;

	matrix[2]*=x;
	matrix[6]*=y;
	matrix[10]*=z;

	matrix[3]*=x;
	matrix[7]*=y;
	matrix[11]*=z;
}

void glhRotated2(sreal2 *matrix, sreal2 angleInRadians, sreal2 x, sreal2 y, sreal2 z)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal2 m[16], rotate[16];
	sreal2 OneMinusCosAngle, CosAngle, SinAngle;
	sreal2 A_OneMinusCosAngle, C_OneMinusCosAngle;
	CosAngle=cos(angleInRadians);			//Some stuff for optimizing code
	OneMinusCosAngle=1.0-CosAngle;
	SinAngle=sin(angleInRadians);
	A_OneMinusCosAngle=x*OneMinusCosAngle;
	C_OneMinusCosAngle=z*OneMinusCosAngle;
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
	m[12]=matrix[12];
	m[13]=matrix[13];
	m[14]=matrix[14];
	m[15]=matrix[15];

	rotate[ 0]=x*A_OneMinusCosAngle+CosAngle;
	rotate[ 1]=y*A_OneMinusCosAngle+z*SinAngle;
	rotate[ 2]=z*A_OneMinusCosAngle-y*SinAngle;
	rotate[ 3]=0.0;

	rotate[ 4]=y*A_OneMinusCosAngle-z*SinAngle;
	rotate[ 5]=y*y*OneMinusCosAngle+CosAngle;
	rotate[ 6]=y*C_OneMinusCosAngle+x*SinAngle;
	rotate[ 7]=0.0;

	rotate[ 8]=x*C_OneMinusCosAngle+y*SinAngle;
	rotate[ 9]=y*C_OneMinusCosAngle-x*SinAngle;
	rotate[10]=z*C_OneMinusCosAngle+CosAngle;
	rotate[11]=0.0;
	//The last column of rotate[] is {0 0 0 1}

	matrix[0]=m[0]*rotate[0]+
		m[4]*rotate[1]+
		m[8]*rotate[2];

	matrix[4]=m[0]*rotate[4]+
		m[4]*rotate[5]+
		m[8]*rotate[6];

	matrix[8]=m[0]*rotate[8]+
		m[4]*rotate[9]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[5]*rotate[1]+
		m[9]*rotate[2];

	matrix[5]=m[1]*rotate[4]+
		m[5]*rotate[5]+
		m[9]*rotate[6];

	matrix[9]=m[1]*rotate[8]+
		m[5]*rotate[9]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[6]*rotate[1]+
		m[10]*rotate[2];

	matrix[6]=m[2]*rotate[4]+
		m[6]*rotate[5]+
		m[10]*rotate[6];

	matrix[10]=m[2]*rotate[8]+
		m[6]*rotate[9]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[7]*rotate[1]+
		m[11]*rotate[2];

	matrix[7]=m[3]*rotate[4]+
		m[7]*rotate[5]+
		m[11]*rotate[6];

	matrix[11]=m[3]*rotate[8]+
		m[7]*rotate[9]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}

void glhRotatef2(sreal *matrix, sreal angleInRadians, sreal x, sreal y, sreal z)
{
	//See page 191 of Elementary Linear Algebra by Howard Anton
	sreal m[16], rotate[16];
	sreal OneMinusCosAngle, CosAngle, SinAngle;
	sreal A_OneMinusCosAngle, C_OneMinusCosAngle;
	CosAngle=cosf(angleInRadians);			//Some stuff for optimizing code
	OneMinusCosAngle=1.0-CosAngle;
	SinAngle=sinf(angleInRadians);
	A_OneMinusCosAngle=x*OneMinusCosAngle;
	C_OneMinusCosAngle=z*OneMinusCosAngle;
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
	m[12]=matrix[12];
	m[13]=matrix[13];
	m[14]=matrix[14];
	m[15]=matrix[15];

	rotate[ 0]=x*A_OneMinusCosAngle+CosAngle;
	rotate[ 1]=y*A_OneMinusCosAngle+z*SinAngle;
	rotate[ 2]=z*A_OneMinusCosAngle-y*SinAngle;
	rotate[ 3]=0.0;

	rotate[ 4]=y*A_OneMinusCosAngle-z*SinAngle;
	rotate[ 5]=y*y*OneMinusCosAngle+CosAngle;
	rotate[ 6]=y*C_OneMinusCosAngle+x*SinAngle;
	rotate[ 7]=0.0;

	rotate[ 8]=x*C_OneMinusCosAngle+y*SinAngle;
	rotate[ 9]=y*C_OneMinusCosAngle-x*SinAngle;
	rotate[10]=z*C_OneMinusCosAngle+CosAngle;
	rotate[11]=0.0;
	//The last column of rotate[] is {0 0 0 1}

	matrix[0]=m[0]*rotate[0]+
		m[4]*rotate[1]+
		m[8]*rotate[2];

	matrix[4]=m[0]*rotate[4]+
		m[4]*rotate[5]+
		m[8]*rotate[6];

	matrix[8]=m[0]*rotate[8]+
		m[4]*rotate[9]+
		m[8]*rotate[10];

	//matrix[12]=matrix[12];

	matrix[1]=m[1]*rotate[0]+
		m[5]*rotate[1]+
		m[9]*rotate[2];

	matrix[5]=m[1]*rotate[4]+
		m[5]*rotate[5]+
		m[9]*rotate[6];

	matrix[9]=m[1]*rotate[8]+
		m[5]*rotate[9]+
		m[9]*rotate[10];

	//matrix[13]=matrix[13];

	matrix[2]=m[2]*rotate[0]+
		m[6]*rotate[1]+
		m[10]*rotate[2];

	matrix[6]=m[2]*rotate[4]+
		m[6]*rotate[5]+
		m[10]*rotate[6];

	matrix[10]=m[2]*rotate[8]+
		m[6]*rotate[9]+
		m[10]*rotate[10];

	//matrix[14]=matrix[14];

	matrix[3]=m[3]*rotate[0]+
		m[7]*rotate[1]+
		m[11]*rotate[2];

	matrix[7]=m[3]*rotate[4]+
		m[7]*rotate[5]+
		m[11]*rotate[6];

	matrix[11]=m[3]*rotate[8]+
		m[7]*rotate[9]+
		m[11]*rotate[10];

	//matrix[15]=matrix[15];
}


void glhLoadIdentityd2(sreal2 *matrix)
{
	memcpy(matrix, GlobalIdentityMatrixd, sizeof(sreal2)*16);
	/*matrix[0]=1.0;
	matrix[1]=0.0;
	matrix[2]=0.0;
	matrix[3]=0.0;
	matrix[4]=0.0;
	matrix[5]=1.0;
	matrix[6]=0.0;
	matrix[7]=0.0;
	matrix[8]=0.0;
	matrix[9]=0.0;
	matrix[10]=1.0;
	matrix[11]=0.0;
	matrix[12]=0.0;
	matrix[13]=0.0;
	matrix[14]=0.0;
	matrix[15]=1.0;*/
}

void glhLoadIdentityf2(sreal *matrix)
{
	memcpy(matrix, GlobalIdentityMatrixf, sizeof(sreal)*16);
	/*matrix[0]=1.0;
	matrix[1]=0.0;
	matrix[2]=0.0;
	matrix[3]=0.0;
	matrix[4]=0.0;
	matrix[5]=1.0;
	matrix[6]=0.0;
	matrix[7]=0.0;
	matrix[8]=0.0;
	matrix[9]=0.0;
	matrix[10]=1.0;
	matrix[11]=0.0;
	matrix[12]=0.0;
	matrix[13]=0.0;
	matrix[14]=0.0;
	matrix[15]=1.0;*/
}

void glhPerspectived2(sreal2 *matrix, sreal2 fovyInDegrees, sreal2 aspectRatio, sreal2 znear, sreal2 zfar)
{
	sreal2 ymax, xmax;
	ymax=znear*tan(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhFrustumd2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhPerspectivef2(sreal *matrix, sreal fovyInDegrees, sreal aspectRatio, sreal znear, sreal zfar)
{
	sreal ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhD3DPerspectiveLHd2(sreal2 *matrix, sreal2 fovyInDegrees, sreal2 aspectRatio, sreal2 znear, sreal2 zfar)
{
	sreal2 ymax, xmax;
	ymax=znear*tan(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhD3DFrustumLHd2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhD3DPerspectiveLHf2(sreal *matrix, sreal fovyInDegrees, sreal aspectRatio, sreal znear, sreal zfar)
{
	sreal ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhD3DFrustumLHf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhD3DPerspectiveRHd2(sreal2 *matrix, sreal2 fovyInDegrees, sreal2 aspectRatio, sreal2 znear, sreal2 zfar)
{
	sreal2 ymax, xmax;
	ymax=znear*tan(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhD3DFrustumRHd2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhD3DPerspectiveRHf2(sreal *matrix, sreal fovyInDegrees, sreal aspectRatio, sreal znear, sreal zfar)
{
	sreal ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhD3DFrustumRHf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhFrustumd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=(-zfar-znear)/temp4;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(-temp*zfar)/temp4;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhFrustumf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=(-zfar-znear)/temp4;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(-temp*zfar)/temp4;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhD3DFrustumLHd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/(left-right);
	matrix2[9]=(top+bottom)/(bottom-top);
	matrix2[10]=zfar/temp4;
	matrix2[11]=1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(znear*zfar)/(znear-zfar);
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhD3DFrustumLHf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/(left-right);
	matrix2[9]=(top+bottom)/(bottom-top);
	matrix2[10]=zfar/temp4;
	matrix2[11]=1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(znear*zfar)/(znear-zfar);
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhD3DFrustumRHd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=zfar/(znear-zfar);
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(znear*zfar)/(znear-zfar);
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhD3DFrustumRHf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=zfar/(znear-zfar);
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(znear*zfar)/(znear-zfar);
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhOrthod2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=-2.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(-right-left)/temp2;
	matrix2[13]=(-top-bottom)/temp3;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhOrthof2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=-2.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(-right-left)/temp2;
	matrix2[13]=(-top-bottom)/temp3;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhD3DOrthoLHd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=1.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(right+left)/(left-right);
	matrix2[13]=(top+bottom)/(bottom-top);
	matrix2[14]=znear/(znear-zfar);
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhD3DOrthoLHf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=1.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(right+left)/(left-right);
	matrix2[13]=(top+bottom)/(bottom-top);
	matrix2[14]=znear/(znear-zfar);
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhD3DOrthoRHd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=znear-zfar;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=1.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(right+left)/(left-right);
	matrix2[13]=(top+bottom)/(bottom-top);
	matrix2[14]=znear/temp4;
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhD3DOrthoRHf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp2, temp3, temp4, resultMatrix[16];
	temp2=right-left;
	temp3=top-bottom;
	temp4=znear-zfar;
	matrix2[0]=2.0/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=2.0/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=0.0;
	matrix2[9]=0.0;
	matrix2[10]=1.0/temp4;
	matrix2[11]=0.0;
	matrix2[12]=(right+left)/(left-right);
	matrix2[13]=(top+bottom)/(bottom-top);
	matrix2[14]=znear/temp4;
	matrix2[15]=1.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhMergedFrustumd2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear, sreal2 zfar)
{
	sreal2 matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=-2.0/temp4;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhMergedFrustumf2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear, sreal zfar)
{
	sreal matrix2[16], temp, temp2, temp3, temp4, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	temp4=zfar-znear;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=-2.0/temp4;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=(-zfar-znear)/temp4;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhMergedPerspectived2(sreal2 *matrix, sreal2 fovyInDegrees, sreal2 aspectRatio, sreal2 znear, sreal2 zfar)
{
	sreal2 ymax, xmax;
	ymax=znear*tan(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhMergedFrustumd2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhMergedPerspectivef2(sreal *matrix, sreal fovyInDegrees, sreal aspectRatio, sreal znear, sreal zfar)
{
	sreal ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhMergedFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void glhFrustumInfiniteFarPlaned2(sreal2 *matrix, sreal2 left, sreal2 right, sreal2 bottom, sreal2 top, sreal2 znear)
{
	sreal2 matrix2[16], temp, temp2, temp3, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=-1.0;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=-2.0*znear;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhFrustumInfiniteFarPlanef2(sreal *matrix, sreal left, sreal right, sreal bottom, sreal top, sreal znear)
{
	sreal matrix2[16], temp, temp2, temp3, resultMatrix[16];
	temp=2.0*znear;
	temp2=right-left;
	temp3=top-bottom;
	matrix2[0]=temp/temp2;
	matrix2[1]=0.0;
	matrix2[2]=0.0;
	matrix2[3]=0.0;
	matrix2[4]=0.0;
	matrix2[5]=temp/temp3;
	matrix2[6]=0.0;
	matrix2[7]=0.0;
	matrix2[8]=(right+left)/temp2;
	matrix2[9]=(top+bottom)/temp3;
	matrix2[10]=-1.0;
	matrix2[11]=-1.0;
	matrix2[12]=0.0;
	matrix2[13]=0.0;
	matrix2[14]=-2.0*znear;
	matrix2[15]=0.0;
	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);

	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhPerspectiveInfiniteFarPlaned2(sreal2 *matrix, sreal2 fovyInDegrees, sreal2 aspectRatio, sreal2 znear)
{
	sreal2 ymax, xmax;
	ymax=znear*tan(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhFrustumInfiniteFarPlaned2(matrix, -xmax, xmax, -ymax, ymax, znear);
}

void glhPerspectiveInfiniteFarPlanef2(sreal *matrix, sreal fovyInDegrees, sreal aspectRatio, sreal znear)
{
	sreal ymax, xmax;
	ymax=znear*tanf(fovyInDegrees*M_PI/360.0);
	//ymin=-ymax;
	//xmin=-ymax*aspectRatio;
	xmax=ymax*aspectRatio;
	glhFrustumInfiniteFarPlanef2(matrix, -xmax, xmax, -ymax, ymax, znear);
}

void glhLookAtd2(sreal2 *matrix, sreal2 *eyePosition3D, sreal2 *center3D, sreal2 *upVector3D)
{
	sreal2 forward[3], side[3], up[3];
	sreal2 matrix2[16], resultMatrix[16];

	forward[0]=center3D[0]-eyePosition3D[0];
	forward[1]=center3D[1]-eyePosition3D[1];
	forward[2]=center3D[2]-eyePosition3D[2];
	NormalizeVectorDOUBLE_2(forward);

	//Side = forward x up
	ComputeNormalOfPlaneDOUBLE_2(side, forward, upVector3D);
	NormalizeVectorDOUBLE_2(side);

	//Recompute up as: up = side x forward
	ComputeNormalOfPlaneDOUBLE_2(up, side, forward);

	matrix2[0]=side[0];
	matrix2[4]=side[1];
	matrix2[8]=side[2];
	matrix2[12]=0.0;

	matrix2[1]=up[0];
	matrix2[5]=up[1];
	matrix2[9]=up[2];
	matrix2[13]=0.0;

	matrix2[2]=-forward[0];
	matrix2[6]=-forward[1];
	matrix2[10]=-forward[2];
	matrix2[14]=0.0;

	matrix2[3]=matrix2[7]=matrix2[11]=0.0;
	matrix2[15]=1.0;

	MultiplyMatrices4by4OpenGL_DOUBLE_3(resultMatrix, matrix, matrix2);
	glhTranslated2(resultMatrix, -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
	
	memcpy(matrix, resultMatrix, 16*sizeof(sreal2));
}

void glhLookAtf2(sreal *matrix, sreal *eyePosition3D, sreal *center3D, sreal *upVector3D)
{
	sreal forward[3], side[3], up[3];
	sreal matrix2[16], resultMatrix[16];

	forward[0]=center3D[0]-eyePosition3D[0];
	forward[1]=center3D[1]-eyePosition3D[1];
	forward[2]=center3D[2]-eyePosition3D[2];
	NormalizeVectorFLOAT_2(forward);

	//Side = forward x up
	ComputeNormalOfPlaneFLOAT_2(side, forward, upVector3D);
	NormalizeVectorFLOAT_2(side);

	//Recompute up as: up = side x forward
	ComputeNormalOfPlaneFLOAT_2(up, side, forward);

	matrix2[0]=side[0];
	matrix2[4]=side[1];
	matrix2[8]=side[2];
	matrix2[12]=0.0;

	matrix2[1]=up[0];
	matrix2[5]=up[1];
	matrix2[9]=up[2];
	matrix2[13]=0.0;

	matrix2[2]=-forward[0];
	matrix2[6]=-forward[1];
	matrix2[10]=-forward[2];
	matrix2[14]=0.0;

	matrix2[3]=matrix2[7]=matrix2[11]=0.0;
	matrix2[15]=1.0;

	MultiplyMatrices4by4OpenGL_FLOAT(resultMatrix, matrix, matrix2);
	glhTranslatef2(resultMatrix, -eyePosition3D[0], -eyePosition3D[1], -eyePosition3D[2]);
	
	memcpy(matrix, resultMatrix, 16*sizeof(sreal));
}

void glhMultMatrixd2(sreal2 *result_leftMatrix, sreal2 *rightMatrix)
{
	//result_leftMatrix = result_leftMatrix * rightMatrix
	sreal2 tempMatrix[16];
	memcpy(tempMatrix, result_leftMatrix, 16*sizeof(sreal2));

	MultiplyMatrices4by4OpenGL_DOUBLE_3(result_leftMatrix, tempMatrix, rightMatrix);	
}

void glhMultMatrixf2(sreal *result_leftMatrix, sreal *rightMatrix)
{
	//result_leftMatrix = result_leftMatrix * rightMatrix
	sreal tempMatrix[16];
	memcpy(tempMatrix, result_leftMatrix, 16*sizeof(sreal));

	MultiplyMatrices4by4OpenGL_FLOAT(result_leftMatrix, tempMatrix, rightMatrix);
}

void glhMultMatrixSwappedd2(sreal2 *result_rightMatrix, sreal2 *leftMatrix)
{
	//result_rightMatrix = leftMatrix * result_rightMatrix
	sreal2 tempMatrix[16];
	memcpy(tempMatrix, result_rightMatrix, 16*sizeof(sreal2));

	MultiplyMatrices4by4OpenGL_DOUBLE_3(result_rightMatrix, leftMatrix, tempMatrix);
}

void glhMultMatrixSwappedf2(sreal *result_rightMatrix, sreal *leftMatrix)
{
	//result_rightMatrix = leftMatrix * result_rightMatrix
	sreal tempMatrix[16];
	memcpy(tempMatrix, result_rightMatrix, 16*sizeof(sreal));

	MultiplyMatrices4by4OpenGL_FLOAT(result_rightMatrix, leftMatrix, tempMatrix);
}



//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by undercompensating.
//			If not, returns 0, else 1.
//			If width == 0, *widthOpenGL ==1 and returns 1.
sint glhLowerPowerOfTwo2(sint value, sint *newValue)
{
	if(value==0)
	{
		*newValue=1;
		value=1;
	}
	else
	{
		sint start=1;
		while(start<value)
		{
			start<<=1;
		}
		if(start>value)
		{
			start>>=1;
			*newValue=start;
		}
		else //if(start==value)
			*newValue=start;
	}

	if(value==*newValue)
		return 1;
	else
		return 0;
}

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by overcompensating.
//			If not, returns 0, else 1.
//			If width == 0, *widthOpenGL ==1 and returns 1.
sint glhHigherPowerOfTwo2(sint value, sint *newValue)
{
	if(value==0)
	{
		*newValue=1;
		value=1;
	}
	else
	{
		sint start=1;
		while(start<value)
		{
			start<<=1;
		}
		*newValue=start;
	}

	if(value==*newValue)
		return 1;
	else
		return 0;
}

#if USE_INLINE_ASM
sint glhBuild2DMipmaps(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const uchar *data)
{
	sint WidthOpenGL, HeightOpenGL, returnVal, swap;
	sint width2, height2;
	uint myformat;
	uchar *dataout, *data2;
	sint level;
	if(isPromoteToHigherPowerOn==TRUE)
	{
		glhHigherPowerOfTwo2(width, &WidthOpenGL);
		glhHigherPowerOfTwo2(height, &HeightOpenGL);
	}
	else
	{
		glhLowerPowerOfTwo2(width, &WidthOpenGL);
		glhLowerPowerOfTwo2(height, &HeightOpenGL);
	}
	
	sint scanLinePadded;
	if((format==GL_RGB)||(format==GL_BGR))
	{
		myformat=GL_RGB;
		sint BytesPerPixel=3;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL];
	}
	else if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR_EXT))
	{
		myformat=GL_RGBA;
		sint BytesPerPixel=4;
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
				returnVal=(sint)glhScaleImage_asm386(myformat, TRUE, dataAlignment, width2, height2,
					(uchar *)data2, WidthOpenGL, HeightOpenGL, dataout);
				if((returnVal==0)||(returnVal==-5))
				{
					delete [] dataout;
					return returnVal;
				}
			}
			
			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL,
				0, format, GL_UNSIGNED_BYTE, dataout);
		}
		else
		{
			swap=0;
			returnVal=(sint)glhScaleImage_asm386(myformat, TRUE, dataAlignment, width2, height2,
				dataout, WidthOpenGL, HeightOpenGL, (uchar *)data2);
			if((returnVal==0)||(returnVal==-5))
			{
				delete [] dataout;
				return returnVal;
			}

			glTexImage2D(GL_TEXTURE_2D, level, components, WidthOpenGL, HeightOpenGL,
				0, format, GL_UNSIGNED_BYTE, data2);
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

sint glhBuildCubeMapMipmaps(sint components, sint dataAlignment, sint width, sint height,
				unsigned int isPromoteToHigherPowerOn, uint format, const uchar *data)
{
	sint WidthOpenGL, HeightOpenGL, returnVal, swap;
	sint WidthOpenGL2, HeightOpenGL2;
	sint width2, height2;
	uint myformat;
	uchar *dataout, *data2;
	sint level;
	if(isPromoteToHigherPowerOn==TRUE)
	{
		glhHigherPowerOfTwo2(width, &WidthOpenGL);
		glhHigherPowerOfTwo2(height, &HeightOpenGL);
	}
	else
	{
		glhLowerPowerOfTwo2(width, &WidthOpenGL);
		glhLowerPowerOfTwo2(height, &HeightOpenGL);
	}
	
	sint scanLinePadded;
	sint BytesPerPixel;
	if((format==GL_RGB)||(format==GL_BGR))
	{
		myformat=GL_RGB;
		BytesPerPixel=3;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL];
	}
	else if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR_EXT))
	{
		myformat=GL_RGBA;
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
					returnVal=(sint)glhScaleImage_asm386(myformat, TRUE, dataAlignment, width2, height2,
						(uchar *)data2, WidthOpenGL2, HeightOpenGL2, dataout);
					if((returnVal==0)||(returnVal==-5))
					{
						delete [] dataout;
						return returnVal;
					}
				}
				
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2,
						0, format, GL_UNSIGNED_BYTE, dataout);
			}
			else
			{
				swap=0;
				returnVal=(sint)glhScaleImage_asm386(myformat, TRUE, dataAlignment, width2, height2,
					dataout, WidthOpenGL2, HeightOpenGL2, (uchar *)data2);
				if((returnVal==0)||(returnVal==-5))
				{
					delete [] dataout;
					return returnVal;
				}

				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+k, level, components, WidthOpenGL2, HeightOpenGL2,
					0, format, GL_UNSIGNED_BYTE, data2);
			}
			level++;
			width2=WidthOpenGL2;
			height2=HeightOpenGL2;
			if((WidthOpenGL2==1)&&(HeightOpenGL2==1))
				break;
			if(WidthOpenGL2>=2)		//If it turns into 1, do not divide anymore
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

//THIS FUNCTION MUST BE TESTED FOR BUGS
sint glhBuild3DMipmaps(sint components, sint dataAlignment, sint width, sint height, sint depth,
				unsigned int isPromoteToHigherPowerOn, uint format, const uchar *data)
{
	/*sint WidthOpenGL, HeightOpenGL, DepthOpenGL, returnVal, swap;
	sint width2, height2, depth2;
	uint myformat;
	uchar *dataout, *data2;
	sint level;
	
	if(isPromoteToHigherPowerOn==TRUE)
	{
		glhHigherPowerOfTwo2(width, &WidthOpenGL);
		glhHigherPowerOfTwo2(height, &HeightOpenGL);
		glhHigherPowerOfTwo2(depth, &DepthOpenGL);
	}
	else
	{
		glhLowerPowerOfTwo2(width, &WidthOpenGL);
		glhLowerPowerOfTwo2(height, &HeightOpenGL);
		glhLowerPowerOfTwo2(depth, &DepthOpenGL);
	}
	
	sint scanLinePadded;
	sint BytesPerPixel;
	if((format==GL_RGB)||(format==GL_BGR))
	{
		myformat=GL_RGB;
		BytesPerPixel=3;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height*depth];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL*DepthOpenGL];
	}
	else if((format==GL_RGBA)||(format==GL_BGRA)||(format==GL_ABGR_EXT))
	{
		myformat=GL_RGBA;
		BytesPerPixel=4;
		scanLinePadded=((width*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		data2=new uchar[scanLinePadded*height*depth];
		scanLinePadded=((WidthOpenGL*BytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
		dataout=new uchar[scanLinePadded*HeightOpenGL*DepthOpenGL];
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
	depth2=depth;
	while(1)
	{
		if(swap==0)
		{
			swap=1;
			if((level==0)&&(width==WidthOpenGL)&&(height==HeightOpenGL)&&(depth==DepthOpenGL))
			{
				//Make a copy of the original
				memcpy(dataout, data, sizeof(uchar)*scanLinePadded*height);	
			}
			else
			{
				returnVal=(sint)glhScaleImage3D_asm386(myformat, TRUE, dataAlignment, width2, height2, depth2,
					(uchar *)data2, WidthOpenGL, HeightOpenGL, DepthOpenGL, dataout);
				if((returnVal==0)||(returnVal==-5))
				{
					delete [] dataout;
					return returnVal;
				}
			}
			
			glTexImage3D(GL_TEXTURE_3D, level, components, WidthOpenGL2, HeightOpenGL2, DepthOpenGL2,
					0, format, GL_UNSIGNED_BYTE, dataout);
		}
		else
		{
			swap=0;
			returnVal=(sint)glhScaleImage3D_asm386(myformat, TRUE, dataAlignment, width2, height2, depth2,
				dataout, WidthOpenGL, HeightOpenGL, DepthOpenGL, (uchar *)data2);
			if((returnVal==0)||(returnVal==-5))
			{
				delete [] dataout;
				return returnVal;
			}
		
			glTexImage3D(GL_TEXTURE_3D, level, components, WidthOpenGL, HeightOpenGL, DepthOpenGL
				0, format, GL_UNSIGNED_BYTE, data2);
		}
		level++;
		width2=WidthOpenGL;
		height2=HeightOpenGL;
		depth2=DepthOpenGL;
		if((WidthOpenGL==1)&&(HeightOpenGL==1)&&(DepthOpenGL==1))
			break;
		if(WidthOpenGL>=2)		//If it turns into 1, do not divide anymore
			WidthOpenGL/=2;
		if(HeightOpenGL>=2)		//If it turns into 1, do not divide anymore
			HeightOpenGL/=2;
		if(DepthOpenGL>=2)		//If it turns into 1, do not divide anymore
			DepthOpenGL/=2;
	}

	delete [] dataout;
	delete [] data2;*/
	////////////////////////////
	return 1;
}

sint glhScaleImage3D_asm386(uint format, unsigned int isUseLinearOrPointSample,
		sint dataAlignment,
		sint widthin, sint heightin, sint depthin,
		uchar *datain,
		sint widthout, sint heightout, sint depthout,
		uchar *dataout)
{
	sint i, j, k, total, total2, total3;
	sint returnVal;
	sint bytesPerPixel;
	sint temp_scanLinePadded;
	uchar *ubtempo, *antislice, *antislice2;

	//1. First, scale each slice (xy-plane)
	//2. Second scale in the z-direction by creating some temporary memory space
	//   and copying the pixels from each slice. This is a very slow process.
	//   I will use the yz-plane
	
	if((format==GL_RGB)||(format==GL_RGB8))
		bytesPerPixel=3;
	else if((format==GL_RGBA)||(format==GL_RGBA8))
		bytesPerPixel=4;
	else
		return -1;

	temp_scanLinePadded=((widthout*bytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment;
	ubtempo=new uchar[temp_scanLinePadded*heightout*depthin];		//Allocate depthin number of slices
	if(ubtempo==NULL)
		return 0;

	
	//Step 1
	if((widthin!=widthout)||(heightin!=heightout))
	{
		total=((widthin*bytesPerPixel+dataAlignment-1)/dataAlignment)*dataAlignment*heightin;
		total2=temp_scanLinePadded*heightout;
		//Scale all the slices
		for(k=0; k<depthin; k++)
		{
			returnVal=glhScaleImage_asm386(format, isUseLinearOrPointSample, dataAlignment, widthin, heightin,
				&datain[k*total], widthout, heightout, &ubtempo[k*total2]);
			if((returnVal==0)||(returnVal==-5))
			{
				delete [] ubtempo;
				return returnVal;
			}
		}
	}
	else		//width and height are identical, just do a memcpy
	{
		total2=temp_scanLinePadded*heightout;
		memcpy(ubtempo, datain, total2*depthin);
	}

	//Step 2
	//Now scale in the z-direction. This involves reconstructing 2D images out of the anti-slices
	//which is very CPU intensive.

	if(depthin!=depthout)
	{
		//!!!!!!!! data alignment of the antislice is 1 !!!!!!!!!!!
		total3=depthin*heightout*bytesPerPixel;
		antislice=new uchar[total3];		//1 yz-plane
		if(antislice==NULL)
		{
			delete [] ubtempo;
			return 0;
		}

		//!!!!!!!! data alignment of the antislice is 1 !!!!!!!!!!!
		total3=depthout*heightout*bytesPerPixel;
		antislice2=new uchar[total3];		//1 yz-plane
		if(antislice2==NULL)
		{
			delete [] ubtempo;
			delete [] antislice;
			return 0;
		}

		if(bytesPerPixel==3)
		{
			sint depthinTimes3=depthin*3;		//*3 because of RGB
			for(k=0; k<widthout; k++)
			{
				for(j=0; j<heightout; j++)
				{
					for(i=0; i<depthin; i++)
					{
						antislice[j*depthinTimes3+i  ]=ubtempo[i*total2+k*3+j*temp_scanLinePadded  ];
						antislice[j*depthinTimes3+i+1]=ubtempo[i*total2+k*3+j*temp_scanLinePadded+1];
						antislice[j*depthinTimes3+i+2]=ubtempo[i*total2+k*3+j*temp_scanLinePadded+2];
					}
				}
				returnVal=glhScaleImage_asm386(format, isUseLinearOrPointSample, 1, depthin, heightout,
					antislice, depthout, heightout, antislice2);
				if((returnVal==0)||(returnVal==-5))
				{
					delete [] ubtempo;
					delete [] antislice;
					delete [] antislice2;
					return returnVal;
				}

				//Must copy the scaled antislice into the output buffer, dataout
				for(j=0; j<heightout; j++)
				{
					for(i=0; i<depthout; i++)
					{
						dataout[i*total2+k*3+j*temp_scanLinePadded  ]=antislice2[j*depthinTimes3+i  ];
						dataout[i*total2+k*3+j*temp_scanLinePadded+1]=antislice2[j*depthinTimes3+i+1];
						dataout[i*total2+k*3+j*temp_scanLinePadded+2]=antislice2[j*depthinTimes3+i+2];
					}
				}
			}
		}
		else if(bytesPerPixel==4)
		{
			sint depthinTimes4=depthin*4;		//*4 because of RGBA
			for(k=0; k<widthout; k++)
			{
				for(j=0; j<heightout; j++)
				{
					for(i=0; i<depthin; i++)
					{
						antislice[j*depthinTimes4+i  ]=ubtempo[i*total2+k*4+j*temp_scanLinePadded  ];
						antislice[j*depthinTimes4+i+1]=ubtempo[i*total2+k*4+j*temp_scanLinePadded+1];
						antislice[j*depthinTimes4+i+2]=ubtempo[i*total2+k*4+j*temp_scanLinePadded+2];
						antislice[j*depthinTimes4+i+3]=ubtempo[i*total2+k*4+j*temp_scanLinePadded+3];
					}
				}
				returnVal=glhScaleImage_asm386(format, isUseLinearOrPointSample, 1, depthin, heightout,
					antislice, depthout, heightout, antislice2);
				if((returnVal==0)||(returnVal==-5))
				{
					delete [] ubtempo;
					delete [] antislice;
					delete [] antislice2;
					return returnVal;
				}

				//Must copy the scaled antislice into the output buffer, dataout
				for(j=0; j<heightout; j++)
				{
					for(i=0; i<depthout; i++)
					{
						dataout[i*total2+k*4+j*temp_scanLinePadded  ]=antislice2[j*depthinTimes4+i  ];
						dataout[i*total2+k*4+j*temp_scanLinePadded+1]=antislice2[j*depthinTimes4+i+1];
						dataout[i*total2+k*4+j*temp_scanLinePadded+2]=antislice2[j*depthinTimes4+i+2];
						dataout[i*total2+k*4+j*temp_scanLinePadded+3]=antislice2[j*depthinTimes4+i+3];
					}
				}
			}
		}

		delete [] ubtempo;
		delete [] antislice;
		delete [] antislice2;
		//Success!
	}
	else //if(depthin==depthout)	//The result is in ubtempo, just memcpy to dataout
	{
		memcpy(dataout, ubtempo, temp_scanLinePadded*heightout*depthin);
		delete [] ubtempo;
		//Success!
	}

	
	return 1;
}
#endif // #if USE_INLINE_ASM



sint glhIsMatrixRotationMatrixd(sreal2 *matrix)
{
	sreal2 length;
	//The rows of the matrix or the columns of the vector have length =1
	length=matrix[0]*matrix[0]+matrix[1]*matrix[1]+matrix[2]*matrix[2]+matrix[3]*matrix[3];
	if(fabs(1.0-length)>1.0e-12)
		return 0;
	length=matrix[4]*matrix[4]+matrix[5]*matrix[5]+matrix[6]*matrix[6]+matrix[7]*matrix[7];
	if(fabs(1.0-length)>1.0e-12)
		return 0;
	length=matrix[8]*matrix[8]+matrix[9]*matrix[9]+matrix[10]*matrix[10]+matrix[11]*matrix[11];
	if(fabs(1.0-length)>1.0e-12)
		return 0;
	length=matrix[12]*matrix[12]+matrix[13]*matrix[13]+matrix[14]*matrix[14]+matrix[15]*matrix[15];
	if(fabs(1.0-length)>1.0e-12)
		return 0;
	return 1;
}

sint glhIsMatrixRotationMatrixf(sreal *matrix)
{
	sreal length;
	//The rows of the matrix or the columns of the vector have length =1
	length=matrix[0]*matrix[0]+matrix[1]*matrix[1]+matrix[2]*matrix[2]+matrix[3]*matrix[3];
	if(fabsf(1.0-length)>1.0e-06)
		return 0;
	length=matrix[4]*matrix[4]+matrix[5]*matrix[5]+matrix[6]*matrix[6]+matrix[7]*matrix[7];
	if(fabsf(1.0-length)>1.0e-06)
		return 0;
	length=matrix[8]*matrix[8]+matrix[9]*matrix[9]+matrix[10]*matrix[10]+matrix[11]*matrix[11];
	if(fabsf(1.0-length)>1.0e-06)
		return 0;
	length=matrix[12]*matrix[12]+matrix[13]*matrix[13]+matrix[14]*matrix[14]+matrix[15]*matrix[15];
	if(fabsf(1.0-length)>1.0e-06)
		return 0;
	return 1;
}

void glhExtractAnglesFromRotationMatrixd2(sreal2 *matrix, sreal2 *xAngleRadians, sreal2 *yAngleRadians, sreal2 *zAngleRadians)
{
	*yAngleRadians=asin(-matrix[2]);
	sreal2 ycos=cos(*yAngleRadians);	//This is 0 if angle is PI or 3/2*Pi
	if((ycos>1.0e-6)||(ycos<-1.0e-6))
	{
		sreal2 fval, fval2;
		fval=matrix[1]/ycos;
		fval2=matrix[0]/ycos;
		if(fval>=0.0)
		{
			if(fval2>=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*zAngleRadians=asin(fval);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*zAngleRadians=acos(fval2);
			}
		}
		else
		{
			if(fval2<=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*zAngleRadians=asin(fval);
				*zAngleRadians=*zAngleRadians-2.0*(1.570796327+*zAngleRadians);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*zAngleRadians=-acos(fval2);
			}
		}
		//If I do asin(matrix[6]/ycos) then I get a number between 90 and 0 and -90
		//So I have to check in which quadrant we are in
		fval=matrix[6]/ycos;
		fval2=matrix[10]/ycos;
		if(fval>=0.0)
		{
			if(fval2>=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*xAngleRadians=asin(fval);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*xAngleRadians=acos(fval2);
			}
		}
		else
		{
			if(fval2<=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*xAngleRadians=asin(fval);
				*xAngleRadians=*xAngleRadians-2.0*(1.570796327+*xAngleRadians);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*xAngleRadians=-acos(fval2);
			}
		}
	}
	else
	{	//ycos is zero, problem is a little more complex
		//If *yAngleRadians is 90 degrees, then the set of equations to solve is
		//1. -sin(Z)*cos(X) + cos(Z)*sin(X)
		//2. sin(Z)*sin(X) + cos(Z)*cos(X)
		//3. -cos(Z)*sin(X) + sin(Z)*cos(X)
		//Adding 1 and 3 gives zero
		//I will try to solve by starting from angle 0 and going to 360.
		//I will try to narrow down the numbers down to 0.1 degrees or
		//0.017453292 radians

		//and if *yAngleRadians is 270 degrees, then the set of equations to solve is
		//-sin(Z)*cos(X) - cos(Z)*sin(X)
		//sin(Z)*sin(X) - cos(Z)*cos(X)
		//-cos(Z)*sin(X) - sin(Z)*cos(X)
		//Adding 1 and 3 gives  -2*cos(Z)*sin(X) -2*sin(Z)*cos(X)

		if(matrix[2]==-1.0)
		{
			long i, j;
			sreal2 closest=100000.0, closest2=100000.0, closest3=100000.0, zangle, xangle, zangle2, xangle2, fval, fval2, fval3;
			*zAngleRadians=0.0;
			for(i=0; i<180; i++)
			{
				*xAngleRadians=0.0;
				for(j=0; j<180; j++)
				{
					fval=matrix[4]+sin(*zAngleRadians)*cos(*xAngleRadians)-cos(*zAngleRadians)*sin(*xAngleRadians);
					fval2=matrix[5]-sin(*zAngleRadians)*sin(*xAngleRadians)-cos(*zAngleRadians)*cos(*xAngleRadians);
					fval3=matrix[9]+cos(*zAngleRadians)*sin(*xAngleRadians)-sin(*zAngleRadians)*cos(*xAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle=*zAngleRadians;
						xangle=*xAngleRadians;
					}
					*xAngleRadians+=0.017453292;
				}
				*zAngleRadians+=0.017453292;
			}
			//Now try to narrow it down to a tenth of a degree
			zangle2=zangle;
			xangle2=xangle;
			*zAngleRadians=zangle-0.017453292;
			xangle-=0.017453292;
			for(i=0; i<20; i++)
			{
				*xAngleRadians=xangle;
				for(j=0; j<20; j++)
				{
					fval=matrix[4]+sin(*zAngleRadians)*cos(*xAngleRadians)-cos(*zAngleRadians)*sin(*xAngleRadians);
					fval2=matrix[5]-sin(*zAngleRadians)*sin(*xAngleRadians)-cos(*zAngleRadians)*cos(*xAngleRadians);
					fval3=matrix[9]+cos(*zAngleRadians)*sin(*xAngleRadians)-sin(*zAngleRadians)*cos(*xAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle2=*zAngleRadians;
						xangle2=*xAngleRadians;
					}
					*xAngleRadians+=0.0017453292;
				}
				*zAngleRadians+=0.0017453292;
			}

			//Hopefully, we have the best values now
			*zAngleRadians=zangle2;
			*xAngleRadians=xangle2;
		}
		else //if(matrix[2]==1.0)
		{
			long i, j;
			sreal2 closest=100000.0, closest2=100000.0, closest3=100000.0, zangle, xangle, zangle2, xangle2, fval, fval2, fval3;
			*zAngleRadians=0.0;
			for(i=0; i<180; i++)
			{
				*xAngleRadians=0.0;
				for(j=0; j<180; j++)
				{
					fval=matrix[4]+sin(*zAngleRadians)*cos(*xAngleRadians)+cos(*zAngleRadians)*sin(*xAngleRadians);
					fval2=matrix[5]-sin(*zAngleRadians)*sin(*xAngleRadians)+cos(*zAngleRadians)*cos(*xAngleRadians);
					fval3=matrix[9]+cos(*zAngleRadians)*sin(*xAngleRadians)+sin(*zAngleRadians)*cos(*xAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle=*zAngleRadians;
						xangle=*xAngleRadians;
					}
					*xAngleRadians+=0.017453292;
				}
				*zAngleRadians+=0.017453292;
			}
			//Now try to narrow it down to a tenth of a degree
			zangle2=zangle;
			xangle2=xangle;
			*zAngleRadians=zangle-0.017453292;
			xangle-=0.017453292;
			for(i=0; i<20; i++)
			{
				*xAngleRadians=xangle;
				for(j=0; j<20; j++)
				{
					fval=matrix[4]+sin(*zAngleRadians)*cos(*xAngleRadians)+cos(*zAngleRadians)*sin(*xAngleRadians);
					fval2=matrix[5]-sin(*zAngleRadians)*sin(*xAngleRadians)+cos(*zAngleRadians)*cos(*xAngleRadians);
					fval3=matrix[9]+cos(*zAngleRadians)*sin(*xAngleRadians)+sin(*zAngleRadians)*cos(*xAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle2=*zAngleRadians;
						xangle2=*xAngleRadians;
					}
					*xAngleRadians+=0.0017453292;
				}
				*zAngleRadians+=0.0017453292;
			}

			//Hopefully, we have the best values now
			*zAngleRadians=zangle2;
			*xAngleRadians=xangle2;
		}
	}
}

void glhExtractAnglesFromRotationMatrixf2(sreal *matrix, sreal *xAngleRadians, sreal *yAngleRadians, sreal *zAngleRadians)
{
	//UPDATE:	When I'm using acosf on certain values like -1.0, I'm getting a
	//			-1.#INDO  (a invalid number, 0xFFC00000), so I will use doubles here.
	sreal2 dyAngleRadians=asin((sreal2)(-matrix[2]));
	*yAngleRadians=(sreal)dyAngleRadians;
	sreal2 ycos=cos(dyAngleRadians);	//This is 0 if angle is PI or 3/2*Pi
	if((ycos>1.0e-6)||(ycos<-1.0e-6))
	{
		sreal2 fval, fval2;
		fval=(sreal2)(matrix[1])/ycos;
		fval2=(sreal2)(matrix[0])/ycos;
		if(fval>=0.0)
		{
			if(fval2>=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*zAngleRadians=(sreal)asin(fval);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*zAngleRadians=(sreal)acos(fval2);
			}
		}
		else
		{
			if(fval2<=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				sreal2 dzAngleRadians=asin(fval);
				*zAngleRadians=(sreal)(dzAngleRadians-2.0*(1.570796327+dzAngleRadians));
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*zAngleRadians=(sreal)(-acos(fval2));
			}
		}
		//If I do asin(matrix[6]/ycos) then I get a number between 90 and 0 and -90
		//So I have to check in which quadrant we are in
		fval=(sreal2)(matrix[6])/ycos;
		fval2=(sreal2)(matrix[10])/ycos;
		if(fval>=0.0)
		{
			if(fval2>=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				*xAngleRadians=(sreal)asin(fval);
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*xAngleRadians=(sreal)acos(fval2);
			}
		}
		else
		{
			if(fval2<=0.0)
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval<-1.0)
					fval=-1.0;
				else if(fval>1.0)
					fval=1.0;
				sreal2 dxAngleRadians=asin(fval);
				*xAngleRadians=(sreal)(dxAngleRadians-2.0*(1.570796327+dxAngleRadians));
			}
			else
			{
				//MAKE SURE VALUE IS BETWEEN -1.0 and 1.0 otherwise I get an invalid value
				if(fval2<-1.0)
					fval2=-1.0;
				else if(fval2>1.0)
					fval2=1.0;
				*xAngleRadians=(sreal)(-acosf(fval2));
			}
		}
	}
	else
	{	//ycos is zero, problem is a little more complex
		//If *yAngleRadians is 90 degrees, then the set of equations to solve is
		//1. -sin(Z)*cos(X) + cos(Z)*sin(X)
		//2. sin(Z)*sin(X) + cos(Z)*cos(X)
		//3. -cos(Z)*sin(X) + sin(Z)*cos(X)
		//Adding 1 and 3 gives zero
		//I will try to solve by starting from angle 0 and going to 360.
		//I will try to narrow down the numbers down to 0.1 degrees or
		//0.017453292 radians

		//and if *yAngleRadians is 270 degrees, then the set of equations to solve is
		//-sin(Z)*cos(X) - cos(Z)*sin(X)
		//sin(Z)*sin(X) - cos(Z)*cos(X)
		//-cos(Z)*sin(X) - sin(Z)*cos(X)
		//Adding 1 and 3 gives  -2*cos(Z)*sin(X) -2*sin(Z)*cos(X)

		if(matrix[2]==-1.0)
		{
			long i, j;
			sreal2 closest=100000.0, closest2=100000.0, closest3=100000.0, zangle, xangle, zangle2, xangle2, fval, fval2, fval3;
			sreal2 dzAngleRadians=0.0, dxAngleRadians;
			for(i=0; i<180; i++)
			{
				dxAngleRadians=0.0;
				for(j=0; j<180; j++)
				{
					fval=matrix[4]+sin(dzAngleRadians)*cos(dxAngleRadians)-cos(dzAngleRadians)*sin(dxAngleRadians);
					fval2=matrix[5]-sin(dzAngleRadians)*sin(dxAngleRadians)-cos(dzAngleRadians)*cos(dxAngleRadians);
					fval3=matrix[9]+cos(dzAngleRadians)*sin(dxAngleRadians)-sin(dzAngleRadians)*cos(dxAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle=dzAngleRadians;
						xangle=dxAngleRadians;
					}
					dxAngleRadians+=0.017453292;
				}
				dzAngleRadians+=0.017453292;
			}
			//Now try to narrow it down to a tenth of a degree
			zangle2=zangle;
			xangle2=xangle;
			dzAngleRadians=zangle-0.017453292;
			xangle-=0.017453292;
			for(i=0; i<20; i++)
			{
				dxAngleRadians=xangle;
				for(j=0; j<20; j++)
				{
					fval=matrix[4]+sin(dzAngleRadians)*cos(dxAngleRadians)-cos(dzAngleRadians)*sin(dxAngleRadians);
					fval2=matrix[5]-sin(dzAngleRadians)*sin(dxAngleRadians)-cos(dzAngleRadians)*cos(dxAngleRadians);
					fval3=matrix[9]+cos(dzAngleRadians)*sin(dxAngleRadians)-sin(dzAngleRadians)*cos(dxAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle2=dzAngleRadians;
						xangle2=dxAngleRadians;
					}
					dxAngleRadians+=0.0017453292;
				}
				dzAngleRadians+=0.0017453292;
			}

			//Hopefully, we have the best values now
			*zAngleRadians=(sreal)zangle2;
			*xAngleRadians=(sreal)xangle2;
		}
		else //if(matrix[2]==1.0)
		{
			long i, j;
			sreal2 closest=100000.0, closest2=100000.0, closest3=100000.0, zangle, xangle, zangle2, xangle2, fval, fval2, fval3;
			sreal2 dzAngleRadians=0.0, dxAngleRadians;
			for(i=0; i<180; i++)
			{
				dxAngleRadians=0.0;
				for(j=0; j<180; j++)
				{
					fval=matrix[4]+sin(dzAngleRadians)*cos(dxAngleRadians)+cos(dzAngleRadians)*sin(dxAngleRadians);
					fval2=matrix[5]-sin(dzAngleRadians)*sin(dxAngleRadians)+cos(dzAngleRadians)*cos(dxAngleRadians);
					fval3=matrix[9]+cos(dzAngleRadians)*sin(dxAngleRadians)+sin(dzAngleRadians)*cos(dxAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle=dzAngleRadians;
						xangle=dxAngleRadians;
					}
					dxAngleRadians+=0.017453292;
				}
				dzAngleRadians+=0.017453292;
			}
			//Now try to narrow it down to a tenth of a degree
			zangle2=zangle;
			xangle2=xangle;
			dzAngleRadians=zangle-0.017453292;
			xangle-=0.017453292;
			for(i=0; i<20; i++)
			{
				dxAngleRadians=xangle;
				for(j=0; j<20; j++)
				{
					fval=matrix[4]+sin(dzAngleRadians)*cos(dxAngleRadians)+cos(dzAngleRadians)*sin(dxAngleRadians);
					fval2=matrix[5]-sin(dzAngleRadians)*sin(dxAngleRadians)+cos(dzAngleRadians)*cos(dxAngleRadians);
					fval3=matrix[9]+cos(dzAngleRadians)*sin(dxAngleRadians)+sin(dzAngleRadians)*cos(dxAngleRadians);
					if((fabs(fval)<closest)&&(fabs(fval2)<closest2)&&(fabs(fval3)<closest3))
					{
						closest=fabs(fval);
						closest2=fabs(fval2);
						closest3=fabs(fval3);
						zangle2=dzAngleRadians;
						xangle2=dxAngleRadians;
					}
					dxAngleRadians+=0.0017453292;
				}
				dzAngleRadians+=0.0017453292;
			}

			//Hopefully, we have the best values now
			*zAngleRadians=(sreal)zangle2;
			*xAngleRadians=(sreal)xangle2;
		}
	}
}


/*void glhMemoryCopy_asm386_MMX(void *dest, void *src, unsigned sint byteSize)
{
	//***********************************************
	// Trim size to nearest multiple of 64
	//***********************************************
	unsigned sint byteSizeTrimmed=byteSize-(byteSize%64);

	__asm
	{
		mov		ecx, byteSizeTrimmed
		mov		edx, dest
		mov		ebx, src
		
		//***********************************************
		// Add destination to size
		//***********************************************
		add		ecx, edx

		//***********************************************
		// Loop until ecx = dest + size
		//***********************************************
		cpLoop:
		/*movq	mm0, [ebx][0x00]
		movq	mm1, [ebx][0x08]
		movq	mm2, [ebx][0x10]
		movq	mm3, [ebx][0x18]
		movq	mm4, [ebx][0x20]
		movq	mm5, [ebx][0x28]
		movq	mm6, [ebx][0x30]
		movq	mm7, [ebx][0x38]

		movq	[edx][0x00], mm0
		movq	[edx][0x08], mm1
		movq	[edx][0x10], mm2
		movq	[edx][0x18], mm3
		movq	[edx][0x20], mm4
		movq	[edx][0x28], mm5
		movq	[edx][0x30], mm6
		movq	[edx][0x38], mm7*/

/*		movq	mm0, [ebx][0x00]
		movq	[edx][0x00], mm0
		movq	mm1, [ebx][0x08]
		movq	[edx][0x08], mm1
		movq	mm2, [ebx][0x10]
		movq	[edx][0x10], mm2
		movq	mm3, [ebx][0x18]
		movq	[edx][0x18], mm3
		movq	mm4, [ebx][0x20]
		movq	[edx][0x20], mm4
		movq	mm5, [ebx][0x28]
		movq	[edx][0x28], mm5
		movq	mm6, [ebx][0x30]
		movq	[edx][0x30], mm6
		add		edx, 64
		movq	mm7, [ebx][0x38]
		add		ebx, 64
		movq	[edx][0x38], mm7
		
		cmp		ecx, edx

		ja		cpLoop
		//***********************************************
		// Empty MMX state
		//***********************************************
		emms
	}

	//***********************************************
	// Copy any leftover bytes MMX didn't cover
	//***********************************************
	{
		schar *src8=(schar *)src;
		schar *dest8=(schar *)dest;

		unsigned sint temp=byteSize-byteSizeTrimmed;
		for(unsigned sint v=0; v<temp; v++)
			dest8[byteSizeTrimmed+v]=src8[byteSizeTrimmed+v];
	}
}*/
