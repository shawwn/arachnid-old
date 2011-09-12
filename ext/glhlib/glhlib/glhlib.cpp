// glhlib.cpp : Defines the entry point for the DLL application.
//
//LOG
//Version 1.00 was the first release and only contained glhScaleImage_asm386()
//Version 1.10 glhGetString along with other varieties of glhScaleImage_asm386()
//Version 1.20 All the image scaling functions had a bug with the FPU control registers
//             and was corrected. Transfered some functionality from my other libraries
//             to this one. I added "Depth Of Field" effects function, FSAA function.
//Version 1.30 Added my own version of gluBuild2DMipmaps
//Version 1.40 Added support for DataAlignment=4 in glhScaleImage_asm386
//             Changed glhBuild2DMipmaps
//             Added glhBuildCubeMapsMipmaps
//             Added glhBuild3DMipmaps
//             MUST FIX texsubimage function
//Version 1.41 Fixed glhUnProjectDOUBLE_1, glhUnProjectFLOAT_1
//             hmmm, I tought I had fixed these functions already.
//Version 1.5  Added Block 16 functions
//Version 1.51 Added Block 17 functions
//Version 1.52 Added glhMultMatrixf2 and glhMultMatrixd2 to Block 12
//             Added glhMultiplyMatrixByMatrix4by4f_1 to Block 16 (double version not added)
//             Added glhMultiplyMatrixByMatrix4by4f_2 to Block 16 (double version not added)
//
//             Renamed all functions that have the word FLOAT or DOUBLE in them
//             to something more consistent with OpenGL's style, which uses "f" or "d" :
//
//             Renamed glhAccumFrustum_FLOAT to glhAccumFrustumf for GL style consistency.
//             Renamed glhAccumPerspective_FLOAT glhAccumPerspectivef for GL style consistency.
//             Renamed glhRender_DOF_SceneAA_FLOAT to glhRender_DOF_SceneAAf for GL style consistency.
//             Renamed glhComputeTangentAndBinormalVectors_v1_FLOAT to glhComputeTangentAndBinormalVectors_v1_f for GL style consistency.
//             Renamed glhComputeTangentAndBinormalVectors_v1_FLOAT_2 to glhComputeTangentAndBinormalVectors_v1_f_2 for GL style consistency.
//             Renamed glhBuild2DNormalMipmaps_FLOAT to glhBuild2DNormalMipmapsf for GL style consistency.
//             Renamed glhBuildCubeMapNormalMipmaps_FLOAT to glhBuildCubeMapNormalMipmapsf for GL style consistency.
//             Renamed glhBuildNormalizationCubeMapf to glhBuildNormalizationCubeMap_FLOAT for GL style consistency.
//             Renamed glhProjectFLOAT_1 to glhProjectf for GL style consistency.
//             Renamed glhProjectDOUBLE_1 to glhProjectd for GL style consistency.
//             Renamed glhUnProjectFLOAT_1 to glhUnProjectf for GL style consistency.
//             Renamed glhUnProjectDOUBLE_1 to glhUnProjectd for GL style consistency.
//             Renamed glhProjectFLOAT_2 to glhProjectf_2 for GL style consistency.
//             Renamed glhProjectFLOAT_3 to glhProjectf_3 for GL style consistency.
//             Renamed glhUnProjectFLOAT_2 to glhUnProjectf_2 for GL style consistency.
//             Renamed glhProjectFLOAT_SSE_Aligned_2 to glhProjectf_SSE_Aligned_2 for GL style consistency.
//             Renamed glhProjectFLOAT_SSE_Aligned_WarmCache_2 to glhProjectf_SSE_Aligned_WarmCache_2 for GL style consistency.
//             Renamed glhProjectFLOAT_SSE_Unaligned_2 to glhProjectf_SSE_Unaligned_2 for GL style consistency.
//             Renamed glhUnProjectFLOAT_SSE_Aligned_2" to glhUnProjectf_SSE_Aligned_2 for GL style consistency.
//             Renamed glhUnProjectFLOAT_SSE_Aligned_WarmCache_2 to glhUnProjectf_SSE_Aligned_WarmCache_2 for GL style consistency.
//             Renamed glhUnProjectFLOAT_SSE_Unaligned_2 to glhUnProjectf_SSE_Unaligned_2 for GL style consistency.
//             Renamed glhMultiplyMatrixByVector4by4FLOAT_1 to glhMultiplyMatrixByVector4by4f_1 for GL style consistency.
//             Renamed glhMultiplyMatrixByVector4by4FLOAT_2 to glhMultiplyMatrixByVector4by4f_2 for GL style consistency.
//             Renamed glhMultiplyMatrixByVector4by4FLOAT_SSE_Aligned_1 to glhMultiplyMatrixByVector4by4f_SSE_Aligned_1 for GL style consistency.
//             Renamed glhMultiplyMatrixByVector4by4FLOAT_SSE_Aligned_WarmCache_1 to glhMultiplyMatrixByVector4by4f_SSE_Aligned_WarmCache_1 for GL style consistency.
//
//Version 1.53
//             Changed to use Multithreaded DLL, so there is a dependence on msvcrt.dll
//             Make sure you compile your EXE with Multithreaded DLL (In VC++, Project Settings ->
//             C/C++ Tab. Select Code generation from Category list.
//             Use run-time library)
//             glhlib is compiled with VC++6 SP5, for Pentium Pro, Maximize Speed.
//
//             Added the following functions to section Block 12
//             Block 12 is all about Software matrix manipulation.
//             glhRotateAboutXd2
//             glhRotateAboutXf2
//             glhRotateAboutYd2
//             glhRotateAboutYf2
//             glhRotateAboutZd2
//             glhRotateAboutZf2
//
//             Added the following functions to section Block 19
//             Block 19 is all about Quaternions.
//             glhQuaternionIdentityf
//             glhQuaternionIsIdentityf
//             glhQuaternionIsPuref
//             glhQuaternionConjugatef
//             glhQuaternionMagnitudef
//             glhQuaternionMagnitudeSquaredf
//             glhQuaternionDotProductf
//             glhQuaternionExpf
//             glhQuaternionLnf
//             glhQuaternionNormalizef
//             glhQuaternionMultiplyf
//             glhQuaternionQuatToRotationMatrixf
//             glhQuaternionRotationMatrixToQuatf
//             glhQuaternionQuatToAxisAndAnglef
//             glhQuaternionAxisAndAngleToQuatf
//             glhQuaternionEulerAnglesToQuatf
//             glhQuaternionSLERPf
//
//             Added the following functions to section 20
//             Block 20 is all about Floating point number manipulation.
//             glhPackFloatIntoUByte
//             glhUnpackUByteIntoFloat
//
//             Added glhShadowMatrixd and glhShadowMatrixf to Block 14
//
//             Added glhShadowMatrixd2 and glhShadowMatrixf2 to Block 12
//
//             Added glhMultMatrixSwappedd2 and glhMultMatrixSwappedf2 to Block 12
//
//				Changed the way glhGetError works. There is now a an error stack, 100 elements deep.
//
//				Added glhMultiplyMatrixByVector4by4f_3 to Block 16
//
//             Moved glhQuickInvertMatrixdv_2 and glhQuickInvertMatrixfv_2
//             from Block 17 to Block 12, where the software matrix functions are located.
//             glhQuickInvertMatrixdv_2 renamed to glhQuickInvertMatrixd2 for consistency with it's "brother" functions.
//             glhQuickInvertMatrixfv_2 renamed to glhQuickInvertMatrixf2 for consistency with it's "brother" functions.
//
//             Moved glhTransposeMatrixdv and glhTransposeMatrixfv
//             from Block 6 to Block 12, where the software matrix functions are located.
//             glhTransposeMatrixdv renamed to glhTransposeMatrixd2 for consistency with it's "brother" functions.
//             glhTransposeMatrixfv renamed to glhTransposeMatrixf2 for consistency with it's "brother" functions.
//
//             Moved glhInvertMatrixdv_2 and glhInvertMatrixfv_2
//             from Block 9 to Block 12, where the software matrix functions are located.
//             glhInvertMatrixdv_2 renamed to glhInvertMatrixd2 for consistency with it's "brother" functions.
//             glhInvertMatrixfv_2 renamed to glhInvertMatrixf2 for consistency with it's "brother" functions.
//
//             Moved glhPerspectived and glhPerspectivef from
//             Block 12 to Block 14.
//
//				Moved glhGetOpenGLMainVersion from
//				Block 8 to Block 1.
//
//				Moved glhGetOpenGLMainVersionAsString from
//				Block 8 to Block 1.
//
//				Moved glhGetGLUMainVersion from
//				Block 8 to Block 1.
//				Moved glhGetOpenGLMainVersionAsString from
//
//				Moved glhGetOpenGLSubVersionAsString from
//				Block 8 to Block 1.
//
//				Moved glhGetOpenGLDriverVersionAsString from
//				Block 8 to Block 1.
//
//				Moved glhIsRunningInHardwareMode from
//				Block 8 to Block 1.
//
//				Added glhGetGLDeviceCaps_GL_1_5 to Block 1.
//
//				Added glhGetGLDeviceCaps_ARB_vp_1_0 to Block 1.
//
//				Added glhGetGLDeviceCaps_ARB_fp_1_0 to Block 1.
//
//				Added glhGetGLSLMainVersion and glhGetGLSLMainVersionAsString to Block 1
//
//				Added glhDoesProcessorSupportSSE2, glhDoesProcessorSupport3DNow, glhDoesProcessorSupport3DNowExtended to Block 16.
//
//             BUG fixes :
//             Small error in glhMultMatrixf2 and glhMultMatrixd2. Instead of 
//             leftMatrix * rightMatrix it was previouly rightMatrix * leftMatrix
//
//             Function glhMultiplyMatrixByMatrix4by4f_1 was defined as :
//             void glhMultiplyMatrixByMatrix4by4f_1(sint countMatrix, GLfloat *leftMatrix, GLfloat *rightMatrix, GLfloat *result)
//
//             countMatrix was removed because this version only does one matrix multiplication.
//             glhMultiplyMatrixByMatrix4by4f_2 is correct.
//
//				Updated glhCopyOpenGLErrorInto_cBuffer : Error code added for GL_EXT_framebuffer_object
//
//				Removing the following functions :
/*glhLoadIdentityProjection
glhLoadIdentityModelview
glhLoadIdentityTexture
glhTranslateProjectionf
glhTranslateProjectionfv
glhTranslateModelviewf
glhTranslateModelviewfv
glhTranslateTexturef
glhTranslateTexturefv
glhRotateProjectionf
glhRotateProjectionfv
glhRotateModelviewf
glhRotateModelviewfv
glhRotateTexturef
glhRotateTexturefv
glhScaleProjectionf
glhScaleProjectionfv
glhScaleModelviewf
glhScaleModelviewfv
glhScaleTexturef
glhScaleTexturefv
glhPushProjection
glhPopProjection
glhPushModelview
glhPopModelview
glhPushTexture
glhPopTexture
glhLoadMatrixProjectionfv
glhMultMatrixProjectionfv
glhLoadMatrixModelviewfv
glhMultMatrixModelviewfv
glhLoadMatrixTexturefv
glhMultMatrixTexturefv*/
//

/*Version 1.54
Moved glhInitLibrary and glhShutDownLibrary from Block 17 to Block 0

Adding to Block 0 :
Added glhGetGLDeviceCaps_GL_2_0 for getting GL 2.0 related max values.
Added glhGetGLDeviceCaps_GL2_0_GLSL for getting GLSL related version number and max values.

Adding to Block 12 :
glhConstantSizeMatrixf2 (double version not added)
glhExtractLookAtFromMatrixd2
glhExtractLookAtFromMatrixf2

Adding to Block 14 :
glhConstantSizeMatrixf

Adding to Block 16 :
glhMultiplyMatrixByMatrix4by4f_SSE_Aligned_WarmCache_1
glhMultiplyMatrixByMatrix4by4f_3DNow


Adding to Block 22 :
This block is about color space conversion.
glhComputeRGBWithHSVf
glhComputeBGRAWithHSVfub
glhComputeRGBAWithHSVfub
glhComputeHSVWithBGRAubf
glhComputeHSVWithRGBAubf


Moved glhDoesRayCrossTrianglef from Block 18 to Block 23
Block 23 is now the "Intersection Testing"

New addition to struct glhSphereObject :
GLfloat ScaleFactorS[8], ScaleFactorT[8];

Updated glhConvertHeightMapToNormalMap for glh error code support.
Now you can call glhGetError

Updated glhCreateSpheref so that when Sphere.GenerateTangentsAndBinormals

Bug Fix :

Bug in glhCreateCylinderf where texcoordinate was not generated.
Bug in glhCreateSpheref where texcoordinate was not generated.

glhConvertHeightMapToNormalMap, when format was GL_BGR, the function would generate an error.

When glhShutDownLibrary was called, it might crash. This has been fixed.

glhComputeTangentAndBinormalVectors_v1_f and glhComputeTangentAndBinormalVectors_v1_f_2
updated so that all number of components work. Previously, only {numCompVertex==3, numCompNormals==3,
numCompTexCoord==2, numCompTangent==3, numCompBinormal==3} worked.

Fixed and added code to glhCreateCubef, glhCreateCylinderf, glhCreateSpheref so that tangents and binormals
and related variables are proper.

Fixed glhGetOpenGLMainVersion where it would crash if a space character is not found at the proper location.
Fixed glhGetGLSLMainVersion where it would crash if a space character is not found at the proper location.

Fixed glhUnProjectf_2 where the w component of objectVertex4D was not beeing 1.0 as documented in the header file.
Instead, the z component was receiving 1.0 due to a copy and paste error. The w component was beeing untouched.

Fixed glhGetGLDeviceCaps_GL_1_5 and glhGetGLDeviceCaps_GL_2_0
This was beeing called
glGetIntegerv(GL_RED, &pcaps->RedBitsi);
glGetIntegerv(GL_GREEN, &pcaps->GreenBitsi);
glGetIntegerv(GL_BLUE, &pcaps->BlueBitsi);
glGetIntegerv(GL_ALPHA, &pcaps->AlphaBitsi);

instead of
glGetIntegerv(GL_RED_BITS, &pcaps->RedBitsi);
glGetIntegerv(GL_GREEN_BITS, &pcaps->GreenBitsi);
glGetIntegerv(GL_BLUE_BITS, &pcaps->BlueBitsi);
glGetIntegerv(GL_ALPHA_BITS, &pcaps->AlphaBitsi);

which gave GL errors (GL_INVALID_ENUM)


Version 1.55
Adding to Block 1
glhGetFloat
glhGetGLUMainVersionAsString
glhIsGLExtensionSupported
glhIsWGLExtensionSupported
glhIdentifyVendor
glhGetShaderModelAsString
glhGetShaderModel

Moved these
glhDoesProcessorSupportMMX
glhDoesProcessorSupportSSE
glhDoesProcessorSupportSSE2
glhDoesProcessorSupport3DNow
glhDoesProcessorSupport3DNowExtended
glhDoesOSSupportSSE

from Block 16 to Block 24
Block 24 is now called the "Processor Section"

Added to Block 24
glhGetProcessorInfo

BUG:
None found.


Version 1.56

Moved some functions around to better organize. Created file "All Functions.rtf"
for better documentation, for quick view of available glh functions.

Added support for float formats to glhMemoryUsageWithMipmaps

Added to Block 12
glhConstantSizeMatrixf

Added to Block 17
glhComputeFrameBufferUsage

Added to Block 21
glhCreatePartialSpheref
glhDeletePartialSpheref

Added Block 25
Block 25 is called "Shader code related"

Added to Block 25
glhTranslateShader

Small changes to glhExtractLookAtFromMatrixf2/glhExtractLookAtFromMatrixd2
to avoid divide by zero

I did not like how glhCreateCubef, glhCreateCylinderf, glhCreateSpheref and glhCreatePartialSpheref worked, so
I am deleting them and replacing them with
glhCreateCubef2, glhCreateCylinderf2, glhCreateSpheref2 and glhCreatePartialSpheref
and renaming changing glhDeleteCubef, glhDeleteCylinderf, glhDeleteSpheref and glhDeletePartialSpheref
to
glhDeleteCubef2, glhDeleteCylinderf2, glhDeleteSpheref2 and glhDeletePartialSpheref2

BUG:
glhQuaternionIsPuref was returning TRUE instead of FALSE and FALSE instead of TRUE

Version 1.57

In Block 1
Added supported for SM 4.0 detection for glhGetShaderModel, glhGetShaderModelAsString

Added
glhMultiplyVectorByMatrix4by4f_1
glhMultiplyVectorByMatrix4by4f_2
glhMultiplyVectorByMatrix4by4f_3

Version 1.58
Added glhQuaternionQuatToRotationMatrixTf

Version 1.59
Added glhFloatToHalf
From glhCommonObject2, removed PolygonNumber.
For glhCubeObject2, added WidthDivision, HeightDivision, LengthDivision
Getting rid of all functions in Block 17. It was rather useless.

Broken code in glhIsWGLExtensionSupported:
The problem is that ATI drivers return WGL_EXT_extensions_string when you call glGetString(GL_EXTENSIONS)
but they don't return WGL_ARB_extensions_string.
nVidia doesn't return either.
I had to modify my glhIsWGLExtensionSupported in order to compensate for this bizareness.

Version 1.60
Deleting :
glhGetFloat
glhGetOpenGLMainVersion
glhGetGLSLMainVersion
glhGetShaderModel
glhIsRunningInHardwareMode
====Use glhGetIntegerv instead

Deleting :
glhGetOpenGLMainVersionAsString
glhGetGLSLMainVersionAsString
glhGetGLUMainVersionAsString
glhGetOpenGLSubVersionAsString
glhGetOpenGLDriverVersionAsString
glhGetShaderModelAsString
====Use glhGetString instead

Version 1.61
Bug fix in glhCreateSpheref2. In class TSphere, GenerateVertices() and GenerateVertices2() were
not computing the vertices correctly. The z values wasn't taking the radius into account, so when the sphere
radius is 1.0, then it looks ok, else the sphere looks deformed.
Bug fix in when you called glhGetIntegerv(GLH_OPENGL_VERSION, params) where the minor number returned was incorrect.

Version 1.62
Deleting :
glhPerspectived
glhPerspectivef
and several other functions (Block 13)

Deleting :
glhAccumFrustumf, glhAccumPerspectivef, glhRender_DOF_SceneAAf

Version 1.63
Added glhCopyOpenGLErrorInto_cBuffer2

Bug : fix in glhProjectf and the other project functions

Version 1.64
Updated : glhConvertHeightMapToNormalMap

Version 1.65
Added glhPickMatrixd2 and glhPickMatrixf2

Version 2.0
Corrected some things in the header file (glhlib.h).
Added glhCreatePartialSphereStackf2, glhDeletePartialSphereStackf2.
Added glhCreateTorusf2, glhDeleteTorusf2.
Added glhCreateTubef2, glhDeleteTubef2.
Made changes in glhCreateCylinderf2 so that porting to 64 bit will be easier in the future.

Version 2.1
Added glhCreateChamferCylinderf2, glhDeleteChamferCylinderf2.
Added glhD3DOrthoLHd2, glhD3DOrthoLHf2, glhD3DOrthoRHd2, glhD3DOrthoRHf2.
Changed the header file to get rid of include gl.h (changed types such as GLfloat to float).
Added glhEvaluateCatmullRomCurvef to section 27.
*/



#include "stdafx.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include "glhlib.h"
#include <time.h>
#include "global.h"
#include "StringFunctions.h"



#pragma warning(disable: 4244)	//Shut up about double to sreal casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'sreal'



//THE FOLLOWING IS GARBAGE CODE FOR REFERENCE ONLY
// This class is exported from the glhlib.dll
/*class GLHLIB_API CGlhlib {
public:
	CGlhlib(void);
	// TODO: add your methods here.
};*/

//extern GLHLIB_API sint nGlhlib;

//GLHLIB_API sint fnGlhlib(void);

sint	GlobalRandom[2];

sreal	*FArray128;									//Reserved 128 sreal for SSE calculations
sint	MemoryAlignmentOnAllocation;				//Default : 16. For SSE (memory align)


ushort	_eLut[512];

//Defined in glhlib4.cpp
void InitPerlinNoise();



BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	sint i;

	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			GlobalRandom[0]=time(NULL);
			GlobalRandom[1]=(GlobalRandom[0]%57+GlobalRandom[0])*GlobalRandom[0];
			///////////////////////////
			InitPerlinNoise();

			for(i=0; i<0x100; i++)
			{
				sint e=(i & 0x0ff)-(127-15);

				if((e<=0)||(e>=30))
				{
					//
					// Special case
					//

					_eLut[i]=0;
					_eLut[i | 0x100]=0;
				}
				else
				{
					//Common case - normalized half, no exponent overflow possible
					_eLut[i]=(e << 10);
					_eLut[i | 0x100]=((e << 10) | 0x8000);
				}
			}
			break;
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
	}
	return TRUE;
}


// This is an example of an exported variable
//GLHLIB_API sint nGlhlib=0;

// This is an example of an exported function.
/*GLHLIB_API sint fnGlhlib(void)
{
	return 42;
}*/

// This is the constructor of a class that has been exported.
// see glhlib.h for the class definition
/*CGlhlib::CGlhlib()
{ 
	return; 
}*/



const schar glhVersion[]="2.1";
const schar glhURL[]="http://glhlib.sourceforge.net";
const schar glhDescribe[]="Graphics Library Helper aka glhlib";
schar cTempBuffer[1024];			//A buffer that is used as a temporary location


#define MAJOR_VERSION	2
#define MINOR_VERSION	1


const schar* glhGetString(uint language, uint token)
{
	schar *version;
	sint i=0, j=0, pointfound=0, spacefound=0;

	switch(language)
	{
	case GLH_ENGLISH:
		switch(token)
		{
		case GLH_GLHLIB_VERSION:
			return glhVersion;
		case GLH_GLHLIB_URL:
			return glhURL;
		case GLH_GLHLIB_DESCRIBE:
			return glhDescribe;
		case GLH_OPENGL_VERSION:
			version=(schar *)glGetString(GL_VERSION);
			if(version==NULL)
			{
				cTempBuffer[0]='\0';
				return cTempBuffer;
			}

			while(version[i]!=0)
			{
				if(pointfound==0)
				{
					if(IsCharacterANumber(version[i])==1)
						cTempBuffer[i]=version[i];
					else if(version[i]=='.')
					{
						cTempBuffer[i]='.';
						pointfound=1;
					}
					else
						break;
				}
				else
				{
					if(IsCharacterANumber(version[i])==1)
						cTempBuffer[i]=version[i];
					else
						break;
				}
				i++;
			}
			cTempBuffer[i]='\0';
			return cTempBuffer;
		case GLH_OPENGL_SUBVERSION:
			version=(schar *)glGetString(GL_VERSION);
			if(version==NULL)
			{
				cTempBuffer[0]='\0';
				return cTempBuffer;
			}

			while(version[i]!=0)
			{
				if(pointfound<2)
				{
					if(version[i]=='.')
						pointfound++;
					else if(IsCharacterANumber(version[i])==0)
						break;
				}
				else
				{
					if(IsCharacterANumberOrAPoint(version[i])==1)
						cTempBuffer[j++]=version[i];
					else
						break;
				}
				i++;
			}
			cTempBuffer[j]='\0';
			return cTempBuffer;
		case GLH_OPENGL_DRIVERVERSION:
			version=(schar *)glGetString(GL_VERSION);
			if(version==NULL)
			{
				cTempBuffer[0]='\0';
				return cTempBuffer;
			}

			while(version[i]!=0)
			{
				if(spacefound==0)
				{
					if(version[i]==' ')
						spacefound=1;
				}
				else
				{
					cTempBuffer[j++]=version[i];
				}
				i++;
			}
			cTempBuffer[j]='\0';
			return cTempBuffer;
		case GLH_GLSL_VERSION:
			version=(schar *)glGetString(GL_SHADING_LANGUAGE_VERSION);
			if(version==NULL)
			{
				cTempBuffer[0]='\0';
				return cTempBuffer;
			}

			while(version[i]!=0)
			{
				if(pointfound==0)
				{
					if(IsCharacterANumber(version[i])==1)
						cTempBuffer[i]=version[i];
					else if(version[i]=='.')
					{
						cTempBuffer[i]='.';
						pointfound=1;
					}
					else
						break;
				}
				else
				{
					if(IsCharacterANumber(version[i])==1)
						cTempBuffer[i]=version[i];
					else
						break;
				}
				i++;
			}
			cTempBuffer[i]='\0';
			return cTempBuffer;
		case GLH_GPU_SHADERMODEL:
			if(glhIsGLExtensionSupported("GL_EXT_gpu_shader4")==TRUE)
			{	//NV_vertex_program4, NV_fragment_program4, NV_gpu_program4
				strcpy(cTempBuffer, "4.0");
			}
			else if(glhIsGLExtensionSupported("GL_NV_vertex_program3")==TRUE)
			{
				strcpy(cTempBuffer, "3.0");
			}
			else if(glhIsGLExtensionSupported("GL_ATI_shader_texture_lod")==TRUE)
			{
				strcpy(cTempBuffer, "3.0");
			}
			else
				strcpy(cTempBuffer, "2.0");

			return cTempBuffer;
		default:
			return NULL;
		}
	default:
		return NULL;
	}
	return NULL;	//Should not reach here
}


void glhGetIntegerv(uint pname, sint *params)
{
	schar *cPointer;
	sint i, j, checkPoint=0;
	schar buffer[64], buffer2[64];		
	schar *ccc;

	switch(pname)
	{
	case GLH_GLHLIB_VERSION:
		//Return the GLH lib major and minor version
		params[0]=MAJOR_VERSION;
		params[1]=MINOR_VERSION;
		break;
	case GLH_OPENGL_HARDWAREACCELERATION:
		//Return TRUE or FALSE
		cPointer=(schar*)glGetString(GL_RENDERER);
		if(cPointer)
		{
			if(strlen(cPointer)>3)
			{
				if(strstr(cPointer, "GDI")!=0)
					params[0]=0;		//GDI Microsoft, software
				else if(strstr(cPointer, "Mesa")!=0)
					params[0]=0;		//MESA, software
				else if(strstr(cPointer, "MESA")!=0)
					params[0]=0;		//MESA, software
				else if(strstr(cPointer, "Direct3D")!=0)
					params[0]=0;		//Direct3D, software
				else if(strstr(cPointer, "Direct 3D")!=0)
					params[0]=0;		//Direct3D, software
				else
					params[0]=1;		//Hardware
			}
			else
				params[0]=0;			//Software
		}
		else
			params[0]=0;				//Software, should not reach this point
		break;
	case GLH_OPENGL_VERSION:
		//Return OpenGL major and minor version
		cPointer=(schar *)glGetString(GL_VERSION);
		if(cPointer)
		{
			for(i=j=0; cPointer[i]!='\0'; i++, j++)
			{
				if(cPointer[i]==' ')
				{
					buffer2[j]='\0';
					break;
				}
				else if(cPointer[i]=='.')
				{
					if(checkPoint==0)
					{
						buffer[j]='\0';
						j=-1;
					}

					checkPoint++;
					if(checkPoint==2)
					{
						buffer2[j]='\0';
						break;
					}
				}
				else if(checkPoint==0)
					buffer[j]=cPointer[i];
				else if(checkPoint==1)
					buffer2[j]=cPointer[i];
			}

			params[0]=ConvertStringIntoSINT(buffer);
			params[1]=ConvertStringIntoSINT(buffer2);
		}
		else
		{
			params[0]=1;		//Assume we have 1.1 at least
			params[1]=1;
		}
		break;
	case GLH_GLSL_VERSION:
		cPointer=(schar *)glGetString(GL_SHADING_LANGUAGE_VERSION);

		if(cPointer)
		{
			for(i=j=0; cPointer[i]!='\0'; i++, j++)
			{
				if(cPointer[i]==' ')
				{
					buffer2[j]='\0';
					break;
				}
				else if(cPointer[i]=='.')
				{
					if(checkPoint==0)
					{
						buffer[j]='\0';
						j=-1;
					}

					checkPoint++;
					if(checkPoint==2)
					{
						buffer2[j]='\0';
						break;
					}
				}
				else if(checkPoint==0)
					buffer[j]=cPointer[i];
				else if(checkPoint==1)
					buffer2[j]=cPointer[i];
			}

			params[0]=ConvertStringIntoSINT(buffer);
			params[1]=ConvertStringIntoSINT(buffer2);
		}
		else
		{
			params[0]=1;		//Assume we have 1.0 at least
			params[1]=0;
		}
		break;
	case GLH_GPU_SHADERMODEL:
		if(glhIsGLExtensionSupported("GL_EXT_gpu_shader4")==TRUE)
		{	//NV_vertex_program4, NV_fragment_program4, NV_gpu_program4
			//SM 4.0
			params[0]=4;
			params[1]=0;
		}
		else if(glhIsGLExtensionSupported("GL_NV_vertex_program3")==TRUE)
		{	//SM 3.0
			params[0]=3;
			params[1]=0;
		}
		else if(glhIsGLExtensionSupported("GL_ATI_shader_texture_lod")==TRUE)
		{	//SM 3.0
			params[0]=3;
			params[1]=0;
		}
		else
		{	//SM 2.0
			params[0]=2;
			params[1]=0;
		}
		break;
	case GLH_VENDOR:
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
		ccc=(schar *)glGetString(GL_VENDOR);

		if(CompareTwoStrings(ccc, (schar *)"ATI Technologies", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_ATI;
		else if(CompareTwoStrings(ccc, (schar *)"AMD", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_ATI;
		else if(CompareTwoStrings(ccc, (schar *)"NVIDIA", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_NVIDIA;
		else if(CompareTwoStrings(ccc, (schar *)"INTEL", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_INTEL;
		else if(CompareTwoStrings(ccc, (schar *)"SIS", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_SIS;
		else if(CompareTwoStrings(ccc, (schar *)"SiS", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_SIS;
		else if(CompareTwoStrings(ccc, (schar *)"3Dfx", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_3DFX;
		else if(CompareTwoStrings(ccc, (schar *)"3DLABS", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_3DLABS;
		else if(CompareTwoStrings(ccc, (schar *)"Diamond Multimedia", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_DIAMONDMULTIMEDIA;
		else if(CompareTwoStrings(ccc, (schar *)"ELSA", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_ELSA;
		else if(CompareTwoStrings(ccc, (schar *)"Imagination Technologies", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_IMAGINATIONTECHNOLOGIES;
		else if(CompareTwoStrings(ccc, (schar *)"Matrox", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_MATROX;
		else if(CompareTwoStrings(ccc, (schar *)"Microsoft", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_MICROSOFT;
		else if(CompareTwoStrings(ccc, (schar *)"S3 Graphics", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_S3;
		else if(CompareTwoStrings(ccc, (schar *)"S3 Incorporated", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_S3;
		else if(CompareTwoStrings(ccc, (schar *)"XGI Technology", FALSE, 0, -1, 0, -1)==1)
			params[0]=VENDOR_XGI;
		break;
	}
}

#if USE_INLINE_ASM
//PURPOSE: This function will not verify if values being passed to function are valid.
//         format must be RGB8 or RGBA8 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8)
//         dataAlignment must be 1 or 4
//         Returns 1 if success. 0 for failure, -5 for not yet implemented
sint glhScaleImage_asm386(uint format, uint isUseLinearOrPointSample,
		sint dataAlignment,
		sint widthin, sint heightin,
		uchar *datain,
		sint widthout, sint heightout,
		uchar *dataout)
{
	sint components;		//Bits per pixel (BPP)
	sint i, j, tempo;
	//sreal *tempin, *tempout;
	sreal sx, sy;
	sreal one=1.0;
	//sint sizein, sizeout;
	//sint rowstride;
	//uchar *ubptr;
	sint ii, jj;
	////////////////////////
	/*switch(format)
	{
	case GL_RGB:
		components=3;
		break;
	case GL_RGBA:
		components=4;
		break;
	default:
		return -5;
	}*/
	__asm
	{
		mov		edx, format
		cmp		edx, GL_RGB
		je		Address_GL_RGB
		cmp		edx, GL_RGB8
		je		Address_GL_RGB
		cmp		edx, GL_RGBA
		je		Address_GL_RGBA
		cmp		edx, GL_RGBA8
		je		Address_GL_RGBA
	}
	return -5;
	__asm
	{
		Address_GL_RGB:
		mov		components, 3
		jmp		FormatExit
		Address_GL_RGBA:
		mov		components, 4
		//jmp		FormatExit
		FormatExit:
	}
	
	/*if(widthout>1)
		sx=sreal(widthin-1)/sreal(widthout-1);
	else
		sx=sreal(widthin-1);
	if(heightout>1)
		sy=sreal(heightin-1)/sreal(heightout-1);
	else
		sy=sreal(heightin-1);*/
	__asm
	{
		//Need some temporary sint here.
		//First if statement
		mov		ecx, widthin
		dec		ecx
		mov		i, ecx
		mov		edx, widthout
		cmp		edx, 1
		jle		JUMPPOINT_a_q22bx
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fstp	sx				; store into sx
		jmp		JUMPPOINT_a_q22bxsup
		JUMPPOINT_a_q22bx:
		fild	i				; have the FPU translate integer to sreal
		fstp	sx				; store into sx
		JUMPPOINT_a_q22bxsup:
		//Second if statement
		mov		ecx, heightin
		dec		ecx
		mov		i, ecx
		mov		edx, heightout
		cmp		edx, 1
		jle		JUMPPOINT_b_yy789
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fstp	sy				; store into sy
		jmp		JUMPPOINT_b_yy789sup
		JUMPPOINT_b_yy789:
		fild	i				; have the FPU translate integer to sreal
		fstp	sy				; store into sy
		JUMPPOINT_b_yy789sup:
	}
	/////////////////////////////

	//The FPU's default behavior on Intel is to round or even the value which
	//causes a problem for this algorithm since we need truncation, so we change the FPU
	//control register bit 11 and 10 from 00 to 11
	short old_FPU_Flag, new_FPU_Flag;	//Must be 16 bit (word)
	__asm
	{
		fstcw	old_FPU_Flag	; store control word
		mov		ax, old_FPU_Flag
		or		ax, 0x0C00		; set bit 11 and bit 10 to 11, which means truncate
		mov		new_FPU_Flag, ax
		fldcw	new_FPU_Flag	; load control word
	}

	if(dataAlignment==1)
	{
		if(isUseLinearOrPointSample==FALSE)
		{
			//Gone use point sampling
			////////// DEBUG start /////////////////////////
			//Memory bounds check
			/*uchar *checking001, *checking002;
			checking001=datain+widthin*heightin*3;
			checking002=dataout+widthout*heightout*3;

			for(i=0; i<heightout; i++)
			{
				ii=i*sy;
				for(j=0; j<widthout; j++)
				{
					jj=j*sx;
					uchar *src=datain+(ii*widthin+jj)*components;
					uchar *dst=dataout+(i*widthout+j)*components;
					for(k=0; k<components; k++)
					{
						if(dst>=checking002)
						{
							MessageBox("Memory bound error (009zx)", "DEBUG", MB_OK);
							return 0;
						}
						if(src>=checking001)
						{
							MessageBox("Memory bound error (010zx)", "DEBUG", MB_OK);
							return 0;
						}
						*dst=*src;
						dst++;
						src++;
					}
				}
			}
			return 1;
			////////// DEBUG end /////////////////////////*/
			//Gone use point sampling
			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					ii=sint(sreal(i)*sy);
					for(j=0; j<widthout; j++)
					{
						jj=sint(sreal(j)*sx);
						uchar *src=datain+(ii*widthin+jj)*3;
						uchar *dst=dataout+(i*widthout+j)*3;
						//For each component
						dst[0]=src[0];
						dst[1]=src[1];
						dst[2]=src[2];
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					ii=sint(sreal(i)*sy);
					for(j=0; j<widthout; j++)
					{
						jj=sint(sreal(j)*sx);
						uchar *src=datain+(ii*widthin+jj)*4;
						uchar *dst=dataout+(i*widthout+j)*4;
						//For each component
						dst[0]=src[0];
						dst[1]=src[1];
						dst[2]=src[2];
						dst[3]=src[3];
					}
				}
			}*/
			__asm
			{
				//ebx is dataout
				//ecx as i
				//edx as j
				mov		ebx, dataout
				cmp		components, 3
				jne		ThirtyTwoBits
				//This is the 24 bit version
				dec		heightout		; the last line of the image will be processed differently, this value will not be restored
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoops
				EnterLoop1:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop1
				EnterLoop2:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, widthin
				add		esi, jj
				imul	esi, 3
				add		esi, datain
				mov		edi, i
				imul	edi, widthout
				add		edi, edx		; edx contains j
				imul	edi, 3
				add		edi, ebx
				//mov		ax, [esi]		; copy red and green component
				//mov		[edi], ax
				//mov		al, [esi+2]		; copy blue component
				//mov		[edi+2], al
				mov		eax, [esi]
				mov		[edi], eax			; copy 4 bytes
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2
				ExitLoop1:
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1
				//This is for the last line of the 24 bit image
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoops
				EnterLoop2_spk:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, widthin
				add		esi, jj
				imul	esi, 3
				add		esi, datain
				mov		edi, i
				imul	edi, widthout
				add		edi, edx		; edx contains j
				imul	edi, 3
				add		edi, ebx
				mov		ax, [esi]		; copy red and green component
				mov		[edi], ax
				mov		al, [esi+2]		; copy blue component
				mov		[edi+2], al
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_spk
				jmp		ExitLoops		; skip 32 bit section
				ThirtyTwoBits:
				//This is the 32 bit version
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoops
				EnterLoop1b:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop1b
				EnterLoop2b:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, widthin
				add		esi, jj
				imul	esi, 4
				add		esi, datain
				mov		edi, ecx		; contains i
				imul	edi, widthout
				add		edi, edx		; contains j
				imul	edi, 4
				add		edi, ebx
				mov		eax, [esi]		; copy color
				mov		[edi], eax
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2b
				ExitLoop1b:
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1b
				ExitLoops:
			}
			
			//return 1;			//Success
			goto EndOf_glhScaleImage_asm386;
		}
		else //if(isUseLinearOrPointSample==TRUE)
		{
			__asm
			{
				//if((sx<1.0)&&(sy<1.0))
				fld		sx
				fcomp	one
				fnstsw	ax
				test	ah, 5
				jp		SkipIf_98
				fld		sy
				fcomp	one
				fnstsw	ax
				test	ah, 5
				jp		SkipIf_98
			}
			//Magnify both width and height:  use weighted sample of 4 pixels
			//**** In this code, floating point is used to do some of the calculations, but I will avoid
			//**** using the FPU and will stick to integer computation with some limited precision.
			sint i0, i1, j0, j1, sum, sum2, sum3, sum4;
			//sint alpha, beta;		//These are suppose to be sreal in MESA code!
			sreal falpha, fbeta, foneminusalpha, foneminusbeta;//, s1, s2;
			uchar *src00, *src01, *src10, *src11;
			//uchar *check=dataout+widthout*heightout*components;
			//uchar *check2=datain+widthin*heightin*components;
			__asm
			{
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_66_78
				//24 bit section
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj:
				mov		i, ecx
				fild	i
				fmul	sy				; i0=sint(sreal(i)*sy);
				fist	i0				; keep the value in FPU register for later 
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b
				SkipIfStatement_firett_a:
				mov		i1, eax
				SkipIfStatement_firett_b:
				fisub	i0					; this is falpha in st(0) - falpha=sreal(i)*sy-sreal(i0);
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy
				EnterLoop2_sippy:
				mov		j, edx
				fild	j
				fmul	sx				; sreal(j)*sx
				fist	j0				; keep this in FPU register for later
				//if(j0+1>=widthin)
				//	j1=widthin-1;
				//else
				//	j1=j0+1;
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d
				SkipIfStatement_firett_c:
				mov		j1, eax
				SkipIfStatement_firett_d:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				//push	edx					; for safekeeping
				//push	ecx					; for safekeeping
				/*src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;*/
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 3
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 3
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 3
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 3
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*3;
				imul	eax, widthout
				add		eax, edx			; edx is j in this case
				imul	eax, 3
				add		eax, dataout
				mov		edx, eax			; now we have address in dst or in this case, edx
				mov		src00, esi
				mov		src01, edi
				mov		src10, ebx
				mov		src11, ecx
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				//////
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy
				ExitLoop2_sippy:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj
				jmp		ExitSuperly
				//32 bit section
				ThirtyTwoBits_66_78:
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj_b:
				mov		i, ecx
				fild	i
				fmul	sy
				fist	i0				; keep the value in FPU register for later
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a_b
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b_b
				SkipIfStatement_firett_a_b:
				mov		i1, eax
				SkipIfStatement_firett_b_b:
				fisub	i0
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy_b
				EnterLoop2_sippy_b:
				mov		j, edx
				fild	j
				fmul	sx
				fist	j0				; keep this in FPU register for later
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c_b
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d_b
				SkipIfStatement_firett_c_b:
				mov		j1, eax
				SkipIfStatement_firett_d_b:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 4				; 4 bytes per pixel
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 4				; 4 bytes per pixel
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 4				; 4 bytes per pixel
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 4				; 4 bytes per pixel
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*4;
				imul	eax, widthout
				add		eax, edx			; edx is j
				imul	eax, 4				; 4 bytes per pixel
				add		eax, dataout
				mov		edx, eax			; now we have address in dst
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;				
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				//dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
				//			(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
				mov		al, [esi+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[3]*foneminusbeta
				mov		al, [edi+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[3]*foneminusbeta
				mov		al, [ecx+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+3], al	; dst[3] contains result now
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy_b
				ExitLoop2_sippy_b:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj_b
				ExitSuperly:
				fstp	st(0)
				//finit			; a single pop would suffice here

				jmp		Skip_Bunch_11
			}
			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					//alpha=i*intsy-i0*5000;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						if(j0+1>=widthin)
							j1=widthin-1;
						else
							j1=j0+1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//beta=j*intsx-j0*5000;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;
						//For each component	
						//s1=src00[0]*(1.0-fbeta)+src01[0]*fbeta;
						//s2=src10[0]*(1.0-fbeta)+src11[0]*fbeta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(5000-beta)+sint(src01[0])*beta)*(5000-alpha)+
						//                      (sint(src10[0])*(5000-beta)+sint(src11[0])*beta)*alpha)/25000000);
						
						//ERROR CHECKING CODE
						//if((sint(dst[0])<sint(c)-5)&&(sint(dst[0])>sint(c)+5))
						//	k++;
						//s1=src00[1]*(1.0-fbeta)+src01[1]*fbeta;
						//s2=src10[1]*(1.0-fbeta)+src11[1]*fbeta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(5000-beta)+sint(src01[1])*beta)*(5000-alpha)+
						//                      (sint(src10[1])*(5000-beta)+sint(src11[1])*beta)*alpha)/25000000);
						
						//s1=src00[2]*(1.0-fbeta)+src01[2]*fbeta;
						//s2=src10[2]*(1.0-fbeta)+src11[2]*fbeta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(5000-beta)+sint(src01[2])*beta)*(5000-alpha)+
						//                      (sint(src10[2])*(5000-beta)+sint(src11[2])*beta)*alpha)/25000000);
						
					}
				}
				//schar b[200];
				//sprintf(b, "Number of errors is %d", k);
				//MessageBox(NULL, b, "DEBUG", MB_OK);
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*4;
						src01=datain+(i0*widthin+j1)*4;
						src10=datain+(i1*widthin+j0)*4;
						src11=datain+(i1*widthin+j1)*4;
						dst=dataout+(i*widthout+j)*4;
						//For each component	
						//s1=src00[0]*(1.0-beta)+src01[0]*beta;
						//s2=src10[0]*(1.0-beta)+src11[0]*beta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(10000-beta)+sint(src01[0])*beta)*(10000-alpha)+
						//                      (sint(src10[0])*(10000-beta)+sint(src11[0])*beta)*alpha)/10000);
						//s1=src00[1]*(1.0-beta)+src01[1]*beta;
						//s2=src10[1]*(1.0-beta)+src11[1]*beta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(10000-beta)+sint(src01[1])*beta)*(10000-alpha)+
						//                     (sint(src10[1])*(10000-beta)+sint(src11[1])*beta)*alpha)/10000);
						//s1=src00[2]*(1.0-beta)+src01[2]*beta;
						//s2=src10[2]*(1.0-beta)+src11[2]*beta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(10000-beta)+sint(src01[2])*beta)*(10000-alpha)+
						//	(sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);
						//s1=src00[3]*(1.0-beta)+src01[3]*beta;
						//s2=src10[3]*(1.0-beta)+src11[3]*beta;
						dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
							(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
						//dst[3]=uchar(((sint(src00[3])*(10000-beta)+sint(src01[3])*beta)*(10000-alpha)+
						//                      (sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);

					}
				}
			}*/
		
			//Shrink width and/or height:  use an unweighted box filter
			_asm
			{
				SkipIf_98:
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_xcd99
				//24 bit section
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz
				EnterLoop1_a_stuzz:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2
				Skipper_tyui:
				mov		i1, eax
				Skipper_tyui2:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178
				Enter_Loop2_c178:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2
				Skipper_tyui_zz:
				mov		j1, eax
				Skipper_tyui_zz2:
				mov		edi, i
				imul	edi, widthout
				add		edi, edx		; edx contains j
				imul	edi, 3
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		sum2, 0
				mov		sum3, 0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1
				Enter_rtt1:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2
				Enter_rtt2:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3));
				add		sum, ebx
				mov		bl, [esi+1]
				add		sum2, ebx
				mov		bl, [esi+2]
				add		sum3, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2
				Exit_rtt2:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1
				Exit_rtt1:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				mov		eax, sum2
				cdq
				idiv	ecx
				mov		[edi+1], al
				mov		eax, sum3
				cdq
				idiv	ecx
				mov		[edi+2], al
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178
				ExitLoop2_c178:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz
				ExitLoop1_a_stuzz:
				jmp		CLEAR_IF_1023
				//32 bit section
				/*for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						sum2=0;
						sum3=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4));
								sum2+=sint(*(datain+(ii*widthin+jj)*4+1));
								sum3+=sint(*(datain+(ii*widthin+jj)*4+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						sum2/=(j1-j0+1)*(i1-i0+1);
						sum3/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						dst[1]=(uchar)sum2;
						dst[2]=(uchar)sum3;
					}
				}*/
				ThirtyTwoBits_xcd99:
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz_32
				EnterLoop1_a_stuzz_32:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui_32
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2_32
				Skipper_tyui_32:
				mov		i1, eax
				Skipper_tyui2_32:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178_32
				Enter_Loop2_c178_32:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz_32
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2_32
				Skipper_tyui_zz_32:
				mov		j1, eax
				Skipper_tyui_zz2_32:
				mov		edi, i
				imul	edi, widthout
				add		edi, edx		; edx contains j
				imul	edi, 4
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		sum2, 0
				mov		sum3, 0
				mov		sum4, 0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_32
				Enter_rtt1_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_32
				Enter_rtt2_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4));
				add		sum, ebx
				mov		bl, [esi+1]
				add		sum2, ebx
				mov		bl, [esi+2]
				add		sum3, ebx
				mov		bl, [esi+3]
				add		sum4, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_32
				Exit_rtt2_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_32
				Exit_rtt1_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				mov		eax, sum2
				cdq
				idiv	ecx
				mov		[edi+1], al
				mov		eax, sum3
				cdq
				idiv	ecx
				mov		[edi+2], al
				mov		eax, sum4
				cdq
				idiv	ecx
				mov		[edi+3], al
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178_32
				ExitLoop2_c178_32:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz_32
				ExitLoop1_a_stuzz_32:
				CLEAR_IF_1023:

				Skip_Bunch_11:
			}

			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*3;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						sum2=0;
						sum3=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3));
								sum2+=sint(*(datain+(ii*widthin+jj)*3+1));
								sum3+=sint(*(datain+(ii*widthin+jj)*3+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						sum2/=(j1-j0+1)*(i1-i0+1);
						sum3/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						dst[1]=(uchar)sum2;
						dst[2]=(uchar)sum3;
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						sum2=0;
						sum3=0;
						sum4=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4));
								sum2+=sint(*(datain+(ii*widthin+jj)*4+1));
								sum3+=sint(*(datain+(ii*widthin+jj)*4+2));
								sum4+=sint(*(datain+(ii*widthin+jj)*4+3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						sum2/=(j1-j0+1)*(i1-i0+1);
						sum3/=(j1-j0+1)*(i1-i0+1);
						sum4/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						dst[1]=(uchar)sum2;
						dst[2]=(uchar)sum3
						dst[3]=(uchar)sum4
					}
				}
			}*/
		}
	}	//if(dataAlignment==1)
	else if(dataAlignment==4)
	{
		//If widthin is not divisible by 4, then padding bytes will be used
		sint scanLinePaddedIn;
		//If widthout is not divisible by 4, then padding bytes will be used
		sint scanLinePaddedOut;
		if(isUseLinearOrPointSample==FALSE)
		{
			//Gone use point sampling
			////////// DEBUG start /////////////////////////
			//Memory bounds check
			/*uchar *checking001, *checking002;
			checking001=datain+widthin*heightin*3;
			checking002=dataout+widthout*heightout*3;

			for(i=0; i<heightout; i++)
			{
				ii=i*sy;
				for(j=0; j<widthout; j++)
				{
					jj=j*sx;
					uchar *src=datain+(ii*widthin+jj)*components;
					uchar *dst=dataout+(i*widthout+j)*components;
					for(k=0; k<components; k++)
					{
						if(dst>=checking002)
						{
							MessageBox("Memory bound error (009zx)", "DEBUG", MB_OK);
							return 0;
						}
						if(src>=checking001)
						{
							MessageBox("Memory bound error (010zx)", "DEBUG", MB_OK);
							return 0;
						}
						*dst=*src;
						dst++;
						src++;
					}
				}
			}
			return 1;
			////////// DEBUG end /////////////////////////*/
			//Gone use point sampling
			/*if(components==3)
			{
				scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				for(i=0; i<heightout; i++)
				{
					ii=sint(sreal(i)*sy);
					for(j=0; j<widthout; j++)
					{
						jj=sint(sreal(j)*sx);
						
						uchar *src=datain+ii*scanLinePaddedIn+jj*3;
						uchar *dst=dataout+i*scanLinePaddedOut+j*3;
						//For each component
						dst[0]=src[0];
						dst[1]=src[1];
						dst[2]=src[2];
					}
				}
			}
			else if(components==4)
			{
				scanLinePaddedIn=((widthin*4+dataAlignment-1)/dataAlignment)*dataAlignment;
				scanLinePaddedOut=((widthout*4+dataAlignment-1)/dataAlignment)*dataAlignment;
				for(i=0; i<heightout; i++)
				{
					ii=sint(sreal(i)*sy);
					for(j=0; j<widthout; j++)
					{
						jj=sint(sreal(j)*sx);
						uchar *src=datain+ii*scanLinePaddedIn+jj*4;
						uchar *dst=dataout+i*scanLinePaddedOut+j*4;
						//For each component
						dst[0]=src[0];
						dst[1]=src[1];
						dst[2]=src[2];
						dst[3]=src[3];
					}
				}
			}*/

			__asm
			{
				//ebx is dataout
				//ecx as i
				//edx as j
				mov		ebx, dataout
				cmp		components, 3
				jne		ThirtyTwoBits_Secondary5000
				//This is the 24 bit version
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				//Do 32 bit copying except for last line where we do 16 + 8 bit to
				//avoid boundary over flow
				mov		eax, heightout
				dec		eax
				mov		heightout, eax

				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoops_Secondary5000
				EnterLoop1_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoops_Secondary5000
				EnterLoop2_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, scanLinePaddedIn
				mov		eax, jj
				imul	eax, 3
				add		esi, eax
				add		esi, datain
				mov		edi, i
				imul	edi, scanLinePaddedOut
				mov		eax, edx		; edx contains j
				imul	eax, 3
				add		edi, eax		
				add		edi, ebx
				mov		eax, [esi]
				mov		[edi], eax			; copy 4 bytes, 32 bits
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_Secondary5000
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_Secondary5000

				//Process the last line of this 24 bit image
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoops_Secondary5000
				EnterLoop2_Secondary5000_bbnn:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, scanLinePaddedIn
				mov		eax, jj
				imul	eax, 3
				add		esi, eax
				add		esi, datain
				mov		edi, i
				imul	edi, scanLinePaddedOut
				mov		eax, edx		; edx contains j
				imul	eax, 3
				add		edi, eax		
				add		edi, ebx
				mov		ax, word ptr[esi]
				mov		word ptr[edi], ax			; copy 4 bytes, 32 bits
				mov		al, byte ptr[esi+2]
				mov		byte ptr[edi+2], al
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_Secondary5000_bbnn

				jmp		ExitLoops_Secondary5000		; skip 32 bit section

				ThirtyTwoBits_Secondary5000:
				//This is the 32 bit version
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoops_Secondary5000
				EnterLoop1b_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	ii
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop1b_Secondary5000
				EnterLoop2b_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	jj
				mov		esi, ii
				imul	esi, scanLinePaddedIn
				mov		eax, jj
				imul	eax, 4
				add		esi, eax
				add		esi, datain
				mov		edi, ecx		; contains i
				imul	edi, scanLinePaddedOut
				mov		eax, edx		; contains j
				imul	eax, 4
				add		edi, eax		
				add		edi, ebx
				mov		eax, [esi]		; copy color, 32 bit move
				mov		[edi], eax
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2b_Secondary5000
				ExitLoop1b_Secondary5000:
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1b_Secondary5000
				ExitLoops_Secondary5000:
			}
			
			//return 1;			//Success
			goto EndOf_glhScaleImage_asm386;
		}
		else //if(isUseLinearOrPointSample==TRUE)
		{
			//Magnify both width and height:  use weighted sample of 4 pixels
			//**** In this code, floating point is used to do some of the calculations, but I will avoid
			//**** using the FPU and will stick to integer computation with some limited precision.
			sint i0, i1, j0, j1, sum, sum2, sum3, sum4;
			//sint alpha, beta;		//These are suppose to be sreal in MESA code!
			sreal falpha, fbeta, foneminusalpha, foneminusbeta;//, s1, s2;
			uchar *src00, *src01, *src10, *src11;
			//uchar *check=dataout+widthout*heightout*components;
			//uchar *check2=datain+widthin*heightin*components;

			__asm
			{
				//if((sx<1.0)&&(sy<1.0))
				fld		sx
				fcomp	one
				fnstsw	ax
				test	ah, 5
				jp		SkipIf_98_Secondary5000
				fld		sy
				fcomp	one
				fnstsw	ax
				test	ah, 5
				jp		SkipIf_98_Secondary5000
			}

			__asm
			{
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_66_78_Secondary5000
				//24 bit section
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly_Secondary5000
				EnterLoop1_dashj_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy				; i0=sint(sreal(i)*sy);
				fist	i0				; keep the value in FPU register for later 
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a_Secondary5000
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b_Secondary5000
				SkipIfStatement_firett_a_Secondary5000:
				mov		i1, eax
				SkipIfStatement_firett_b_Secondary5000:
				fisub	i0					; this is falpha in st(0) - falpha=sreal(i)*sy-sreal(i0);
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy_Secondary5000
				EnterLoop2_sippy_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx				; sreal(j)*sx
				fist	j0				; keep this in FPU register for later
				//if(j0+1>=widthin)
				//	j1=widthin-1;
				//else
				//	j1=j0+1;
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c_Secondary5000
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d_Secondary5000
				SkipIfStatement_firett_c_Secondary5000:
				mov		j1, eax
				SkipIfStatement_firett_d_Secondary5000:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				//push	edx					; for safekeeping
				//push	ecx					; for safekeeping
				/*src00=datain+i0*scanLinePaddedIn+j0*3;
				src01=datain+i0*scanLinePaddedIn+j1*3;
				src10=datain+i1*scanLinePaddedIn+j0*3;
				src11=datain+i1*scanLinePaddedIn+j1*3;
				dst=dataout+i*scanLinePaddedOut+j*3;*/
				mov		eax, j0				; not enough registers
				imul	eax, 3
				mov		j0, eax
				mov		eax, j1				; not emough registers
				imul	eax, 3
				mov		j1, eax
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				imul	esi, scanLinePaddedIn
				mov		edi, esi			; make a copy of i0*scanLinePaddedIn for efficiency
				add		esi, j0
				add		esi, eax
				add		edi, j1
				add		edi, eax
				mov		ebx, i1
				imul	ebx, scanLinePaddedIn
				mov		ecx, ebx			; make a copy of i1*scanLinePaddedIn for efficiency
				add		ebx, j0
				add		ebx, eax
				add		ecx, j1
				add		ecx, eax
				mov		eax, i				; dst=dataout+i*scabLinePaddedOut+j*3;
				imul	eax, scanLinePaddedOut
				imul	edx, 3
				add		eax, edx			; edx is j in this case (j is a backup)
				add		eax, dataout
				mov		edx, eax			; now we have address in dst or in this case, edx
				mov		src00, esi
				mov		src01, edi
				mov		src10, ebx
				mov		src11, ecx
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				///////////
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy_Secondary5000
				ExitLoop2_sippy_Secondary5000:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj_Secondary5000
				jmp		ExitSuperly_Secondary5000
				//32 bit section
				ThirtyTwoBits_66_78_Secondary5000:
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly_Secondary5000
				EnterLoop1_dashj_b_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy
				fist	i0				; keep the value in FPU register for later
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a_b_Secondary5000
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b_b_Secondary5000
				SkipIfStatement_firett_a_b_Secondary5000:
				mov		i1, eax
				SkipIfStatement_firett_b_b_Secondary5000:
				fisub	i0					; this is falpha in st(0)
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy_b_Secondary5000
				EnterLoop2_sippy_b_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx
				fist	j0				; keep this in FPU register for later
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c_b_Secondary5000
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d_b_Secondary5000
				SkipIfStatement_firett_c_b_Secondary5000:
				mov		j1, eax
				SkipIfStatement_firett_d_b_Secondary5000:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				mov		eax, j0				; not enough registers
				imul	eax, 4
				mov		j0, eax
				mov		eax, j1				; not enough registers
				imul	eax, 4
				mov		j1, eax
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				imul	esi, scanLinePaddedIn
				mov		edi, esi			; make a copy of i0*scanLinePaddedIn for efficiency
				add		esi, j0
				add		esi, eax
				add		edi, j1
				add		edi, eax
				mov		ebx, i1
				imul	ebx, scanLinePaddedIn
				mov		ecx, ebx			; make a copy if i1*sanLinePaddedIn for efficiency
				add		ebx, j0
				add		ebx, eax
				add		ecx, j1
				add		ecx, eax
				mov		eax, i				; dst=dataout+i*scanLinePaddedOut+j*4;
				imul	eax, scanLinePaddedOut
				imul	edx, 4
				add		eax, edx			; edx is j
				add		eax, dataout
				mov		edx, eax			; now we have address in dst
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;				
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				//dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
				//			(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
				mov		al, [esi+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[3]*foneminusbeta
				mov		al, [edi+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[3]*foneminusbeta
				mov		al, [ecx+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+3], al	; dst[3] contains result now
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy_b_Secondary5000
				ExitLoop2_sippy_b_Secondary5000:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj_b_Secondary5000
				ExitSuperly_Secondary5000:
				fstp	st(0)
				//finit			; a single pop would suffice here

				jmp		Skip_Bunch_11_Secondary5000
			}
			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						if(j0+1>=widthin)
							j1=widthin-1;
						else
							j1=j0+1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+i0*scanLinePaddedIn+j0*3;
						src01=datain+i0*scanLinePaddedIn+j1*3;
						src10=datain+i1*scanLinePaddedIn+j0*3;
						src11=datain+i1*scanLinePaddedIn+j1*3;
						dst=dataout+i*scanLinePaddedOut+j*3;
						//For each component	
						
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+i0*scanLinePaddedIn+j0*4;
						src01=datain+i0*scanLinePaddedIn+j1*4;
						src10=datain+i1*scanLinePaddedIn+j0*4;
						src11=datain+i1*scanLinePaddedIn+j1*4;
						dst=dataout+i*scanLinePaddedOut+j*4;
						//For each component	
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						
						dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
							(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
					}
				}
			}*/
			//Shrink width and/or height:  use an unweighted box filter
			_asm
			{
				SkipIf_98_Secondary5000:
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_xcd99_Secondary5000
				//24 bit section
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 3
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz_Secondary5000
				EnterLoop1_a_stuzz_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui_Secondary5000
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2_Secondary5000
				Skipper_tyui_Secondary5000:
				mov		i1, eax
				Skipper_tyui2_Secondary5000:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178_Secondary5000
				Enter_Loop2_c178_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz_Secondary5000
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2_Secondary5000
				Skipper_tyui_zz_Secondary5000:
				mov		j1, eax
				Skipper_tyui_zz2_Secondary5000:
				mov		edi, i
				imul	edi, scanLinePaddedOut
				mov		eax, edx		; edx contains j
				imul	eax, 3
				add		edi, eax
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		sum2, 0
				mov		sum3, 0
				mov		sum4, 0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_Secondary5000
				Enter_rtt1_Secondary5000:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_Secondary5000
				Enter_rtt2_Secondary5000:
				mov		esi, ecx
				imul	esi, scanLinePaddedIn
				mov		eax, edx
				imul	eax, 3
				add		esi, eax
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+ii*scanLinePaddedIn+jj*3));
				add		sum, ebx
				mov		bl, [esi+1]
				add		sum2, ebx
				mov		bl, [esi+2]
				add		sum3, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_Secondary5000
				Exit_rtt2_Secondary5000:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_Secondary5000
				Exit_rtt1_Secondary5000:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				mov		eax, sum2
				cdq
				idiv	ecx
				mov		[edi+1], al
				mov		eax, sum3
				cdq
				idiv	ecx
				mov		[edi+2], al
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178_Secondary5000
				ExitLoop2_c178_Secondary5000:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz_Secondary5000
				ExitLoop1_a_stuzz_Secondary5000:
				jmp		CLEAR_IF_1023_Secondary5000
				//32 bit section
				/*for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+i*scanLinePaddedOut+j*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4+1));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[1]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4+3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
					}
				}*/
				ThirtyTwoBits_xcd99_Secondary5000:
				//scanLinePaddedIn=((widthin*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthin
				mov		ecx, dataAlignment
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx					; result is in eax
				imul	eax, ecx
				mov		scanLinePaddedIn, eax
				//scanLinePaddedOut=((widthout*3+dataAlignment-1)/dataAlignment)*dataAlignment;
				mov		eax, widthout
				imul	eax, 4
				add		eax, ecx
				dec		eax
				cdq
				idiv	ecx
				imul	eax, ecx
				mov		scanLinePaddedOut, eax

				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz_32_Secondary5000
				EnterLoop1_a_stuzz_32_Secondary5000:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui_32_Secondary5000
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2_32_Secondary5000
				Skipper_tyui_32_Secondary5000:
				mov		i1, eax
				Skipper_tyui2_32_Secondary5000:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178_32_Secondary5000
				Enter_Loop2_c178_32_Secondary5000:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz_32_Secondary5000
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2_32_Secondary5000
				Skipper_tyui_zz_32_Secondary5000:
				mov		j1, eax
				Skipper_tyui_zz2_32_Secondary5000:
				mov		edi, i
				imul	edi, scanLinePaddedOut
				mov		eax, edx		; edx contains j
				imul	eax, 4
				add		edi, eax
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		sum2, 0
				mov		sum3, 0
				mov		sum4, 0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_32_Secondary5000
				Enter_rtt1_32_Secondary5000:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_32_Secondary5000
				Enter_rtt2_32_Secondary5000:
				mov		esi, ecx
				imul	esi, scanLinePaddedIn
				mov		eax, edx
				imul	eax, 4
				add		esi, eax
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4));
				add		sum, ebx
				mov		bl, [esi+1]
				add		sum2, ebx
				mov		bl, [esi+2]
				add		sum3, ebx
				mov		bl, [esi+3]
				add		sum4, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_32_Secondary5000
				Exit_rtt2_32_Secondary5000:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_32_Secondary5000
				Exit_rtt1_32_Secondary5000:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				mov		eax, sum2
				cdq
				idiv	ecx
				mov		[edi+1], al
				mov		eax, sum3
				cdq
				idiv	ecx
				mov		[edi+2], al
				mov		eax, sum4
				cdq
				idiv	ecx
				mov		[edi+3], al
				
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178_32_Secondary5000
				ExitLoop2_c178_32_Secondary5000:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz_32_Secondary5000
				ExitLoop1_a_stuzz_32_Secondary5000:
				CLEAR_IF_1023_Secondary5000:

				Skip_Bunch_11_Secondary5000:
			}

			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+i*scanLinePaddedOut+j*3;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						sum2=0;
						sum3=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*3));
								sum2+=sint(*(datain+ii*scanLinePaddedIn+jj*3+1));
								sum3+=sint(*(datain+ii*scanLinePaddedIn+jj*3+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						sum2/=(j1-j0+1)*(i1-i0+1);
						sum3/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						dst[1]=(uchar)sum2;
						dst[2]=(uchar)sum3;
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+i*scanLinePaddedOut+j*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						sum2=0;
						sum3=0;
						sum4=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+ii*scanLinePaddedIn+jj*4));
								sum2+=sint(*(datain+ii*scanLinePaddedIn+jj*4+1));
								sum3+=sint(*(datain+ii*scanLinePaddedIn+jj*4+2));
								sum4+=sint(*(datain+ii*scanLinePaddedIn+jj*4+3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						sum2/=(j1-j0+1)*(i1-i0+1);
						sum3/=(j1-j0+1)*(i1-i0+1);
						sum4/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						dst[1]=(uchar)sum2;
						dst[2]=(uchar)sum3;
						dst[3]=(uchar)sum4;
					}
				}
			}*/
		}
	}	//if(dataAlignment==4)
	else
	{
		//Don't forget to restore FPU state completly (the control register)
		__asm
		{
			fldcw	old_FPU_Flag	; load control word
		}
		return -5;		//Other data alignment are not implemented
	}

	EndOf_glhScaleImage_asm386:

	//Don't forget to restore FPU state completly (the control register)
	__asm
	{
		fldcw	old_FPU_Flag	; load control word
	}
	return 1;	//Success
}

//This function will not verify if values being passed to function are valid.
//format must be RGB8 or RGBA8 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8)
//dataAlignment must be 1
//Returns 1 if success. 0 for failure, -5 for not yet implemented
sint glhScaleImage_asm386_MMX(uint format, uint isUseLinearOrPointSample,
		sint dataAlignment,
		sint widthin, sint heightin,
		uchar *datain,
		sint widthout, sint heightout,
		uchar *dataout)
{
	sint components;		//Bits per pixel (BPP)
	sint i, j, tempo;
	//sreal *tempin, *tempout;
	sreal sx, sy;
	//sint sizein, sizeout;
	//sint rowstride;
	//uchar *ubptr;
	sint ii[2], jj[2], mem2[4];
	sreal one=1.0;
	////////////////////////
	/*switch(format)
	{
	case GL_RGB:
		components=3;
		break;
	case GL_RGBA:
		components=4;
		break;
	default:
		return -5;
	}*/
	__asm
	{
		mov		edx, format
		cmp		edx, GL_RGB
		je		Address_GL_RGB
		cmp		edx, GL_RGB8
		je		Address_GL_RGB
		cmp		edx, GL_RGBA
		je		Address_GL_RGBA
		cmp		edx, GL_RGBA8
		je		Address_GL_RGBA
	}
	return -5;
	__asm
	{
		Address_GL_RGB:
		mov		components, 3
		jmp		FormatExit
		Address_GL_RGBA:
		mov		components, 4
		//jmp		FormatExit
		FormatExit:
	}
	if(dataAlignment==1)
	{
	/*if(widthout>1)
		sx=sreal(widthin-1)/sreal(widthout-1);
	else
		sx=sreal(widthin-1);
	if(heightout>1)
		sy=sreal(heightin-1)/sreal(heightout-1);
	else
		sy=sreal(heightin-1);*/
	__asm
	{
		//Need some temporary sint here.
		//First if statement
		mov		ecx, widthin
		dec		ecx
		mov		i, ecx
		mov		edx, widthout
		cmp		edx, 1
		jle		JUMPPOINT_a_q22bx
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fstp	sx				; store into sx
		jmp		JUMPPOINT_a_q22bxsup
		JUMPPOINT_a_q22bx:
		fild	i				; have the FPU translate integer to sreal
		fstp	sx				; store into sx
		JUMPPOINT_a_q22bxsup:
		//Second if statement
		mov		ecx, heightin
		dec		ecx
		mov		i, ecx
		mov		edx, heightout
		cmp		edx, 1
		jle		JUMPPOINT_b_yy789
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fstp	sy				; store into sy
		jmp		JUMPPOINT_b_yy789sup
		JUMPPOINT_b_yy789:
		fild	i				; have the FPU translate integer to sreal
		fstp	sy				; store into sy
		JUMPPOINT_b_yy789sup:
	}
	if(isUseLinearOrPointSample==FALSE)
	{
		//Gone use point sampling
		////////// DEBUG start /////////////////////////
		//Memory bounds check
		/*uchar *checking001, *checking002;
		checking001=datain+widthin*heightin*3;
		checking002=dataout+widthout*heightout*3;

		for(ii[1]=0; ii[1]<heightout; ii[1]++)
		{
			ii[0]=i[1]*sy;
			for(jj[1]=0; jj[1]<widthout; jj[1]++)
			{
				jj[0]=jj[1]*sx;
				uchar *src=datain+(ii[0]*widthin+jj[0])*components;
				uchar *dst=dataout+(ii[1]*widthout+jj[1])*components;
				for(k=0; k<components; k++)
				{
					if(dst>=checking002)
					{
						MessageBox("Memory bound error (009zx)", "DEBUG", MB_OK);
						return 0;
					}
					if(src>=checking001)
					{
						MessageBox("Memory bound error (010zx)", "DEBUG", MB_OK);
						return 0;
					}
					*dst=*src;
					dst++;
					src++;
				}
			}
		}
		return 1;
		////////// DEBUG end /////////////////////////*/
		//Gone use point sampling
		/*if(components==3)
		{
			for(ii[1]=0; ii[1]<heightout; ii[1]++)
			{
				ii[0]=sint(sreal(ii[1])*sy);
				for(jj[1]=0; jj[1]<widthout; jj[1]++)
				{
					jj[0]=sint(sreal(jj[1])*sx);
					uchar *src=datain+(ii[0]*widthin+jj[0])*3;
					uchar *dst=dataout+(ii[1]*widthout+jj[1])*3;
					//For each component
					dst[0]=src[0];
					dst[1]=src[1];
					dst[2]=src[2];
				}
			}
		}
		else if(components==4)
		{
			for(ii[1]=0; ii[1]<heightout; ii[1]++)
			{
				ii[0]=sint(sreal(ii[1])*sy);
				for(jj[1]=0; jj[1]<widthout; jj[1]++)
				{
					jj[0]=sint(sreal(jj[1])*sx);
					uchar *src=datain+(ii[0]*widthin+jj[0])*4;
					uchar *dst=dataout+(ii[1]*widthout+jj[1])*4;
					//For each component
					dst[0]=src[0];
					dst[1]=src[1];
					dst[2]=src[2];
					dst[3]=src[3];
				}
			}
		}*/
		__asm
		{
			//ebx is dataout
			//ecx as ii[1]
			//edx as jj[1]
			mov		eax, datain
			mov		[mem2+8], eax
			mov		eax, dataout
			mov		[mem2+12], eax		; need these as a straight 64 bit quantity for MMX unit
			mov		ebx, dataout
			cmp		components, 3
			jne		ThirtyTwoBits
			//This is the 24 bit version
			dec		heightout		; the last line of the image will be processed differently, this value will not be restored
			xor		ecx, ecx
			cmp		ecx, heightout
			jge		ExitLoops
			EnterLoop1:
			mov		[ii+4], ecx
			fild	[ii+4]
			fmul	sy
			fistp	[ii]
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoop1
			EnterLoop2:
			mov		[jj+4], edx
			fild	[jj+4]
			fmul	sx
			fistp	[jj]
			//datain+(ii[0]*widthin+jj[0])*3;
			//dataout+(ii[1]*widthout+jj[1])*3;
			//previously 10 instructions, now it is 23
			mov		esi, [ii]
			imul	esi, widthin
			mov		edi, ecx		; ecx contains ii[1]
			imul	edi, widthout
			mov		[mem2], esi
			mov		[mem2+4], edi
			movq	mm0, [mem2]
			movq	mm1, [jj]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		eax, [mem2]
			imul	eax, 3
			mov		[mem2], eax
			mov		eax, [mem2+8]
			imul	eax, 3
			mov		[mem2+8], eax
			movq	mm0, [mem2]
			movq	mm1, [mem2+8]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		esi, [mem2]
			mov		edi, [mem2+8]
			emms
			//mov		ax, [esi]		; copy red and green component
			//mov		[edi], ax
			//mov		al, [esi+2]		; copy blue component
			//mov		[edi+2], al
			mov		eax, [esi]			; copy 4 bytes
			mov		[edi], eax
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2
			ExitLoop1:
			inc		ecx
			cmp		ecx, heightout
			jl		EnterLoop1
			//This is for the last line of the 24 bit image
			mov		[ii+4], ecx
			fild	[ii+4]
			fmul	sy
			fistp	[ii]
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoops
			EnterLoop2_spk:
			mov		[jj+4], edx
			fild	[jj+4]
			fmul	sx
			fistp	[jj]
			//datain+(ii[0]*widthin+jj[0])*3;
			//dataout+(ii[1]*widthout+jj[1])*3;
			//previously 10 instructions, now it is 23
			mov		esi, [ii]
			imul	esi, widthin
			mov		edi, ecx		; ecx contains ii[1]
			imul	edi, widthout
			mov		[mem2], esi
			mov		[mem2+4], edi
			movq	mm0, [mem2]
			movq	mm1, [jj]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		eax, [mem2]
			imul	eax, 3
			mov		[mem2], eax
			mov		eax, [mem2+8]
			imul	eax, 3
			mov		[mem2+8], eax
			movq	mm0, [mem2]
			movq	mm1, [mem2+8]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		esi, [mem2]
			mov		edi, [mem2+8]
			emms
			mov		ax, [esi]		; copy red and green component
			mov		[edi], ax
			mov		al, [esi+2]		; copy blue component
			mov		[edi+2], al
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2_spk
			jmp		ExitLoops		; skip 32 bit section
			ThirtyTwoBits:
			//This is the 32 bit version
			xor		ecx, ecx
			cmp		ecx, heightout
			jge		ExitLoops
			EnterLoop1b:
			mov		i, ecx
			fild	i
			fmul	sy
			fistp	ii
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoop1b
			EnterLoop2b:
			mov		j, edx
			fild	j
			fmul	sx
			fistp	jj
			//datain+(ii[0]*widthin+jj[0])*3;
			//dataout+(ii[1]*widthout+jj[1])*3;
			//previously 10 instructions, now it is 23
			mov		esi, [ii]
			imul	esi, widthin
			mov		edi, ecx		; ecx contains ii[1]
			imul	edi, widthout
			mov		[mem2], esi
			mov		[mem2+4], edi
			movq	mm0, [mem2]
			movq	mm1, [jj]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		eax, [mem2]
			imul	eax, 3
			mov		[mem2], eax
			mov		eax, [mem2+8]
			imul	eax, 3
			mov		[mem2+8], eax
			movq	mm0, [mem2]
			movq	mm1, [mem2+8]
			paddd	mm0, mm1
			movq	[mem2], mm0
			mov		esi, [mem2]
			mov		edi, [mem2+8]
			emms
			mov		eax, [esi]		; copy color
			mov		[edi], eax
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2b
			ExitLoop1b:
			inc		ecx
			cmp		ecx, heightout
			jl		EnterLoop1b
			ExitLoops:
		}
		return 1;			//Success
	}
	else //if(isUseLinearOrPointSample==TRUE)
	{
		//The FPU's default behavior on Intel is to round or even the value which
		//causes a problem for this algorithm since we need truncation, so we change the FPU
		//control register bit 11 and 10 from 00 to 11
		/*short old_FPU_Flag, new_FPU_Flag;	//Must be 16 bit (word)
		__asm
		{
			fstcw	old_FPU_Flag	; store control word
			mov		ax, old_FPU_Flag
			and		ax, 0xF7FF		; clear bits 11 and 10
			or		ax, 0x0C00		; set them to 1
			mov		new_FPU_Flag, ax
			fldcw	new_FPU_Flag	; load control word
			//if((sx<1.0)&&(sy<1.0))
			fld		sx
			fcomp	one
			fnstsw	ax
			test	ah, 5
			jp		SkipIf_98
			fld		sy
			fcomp	one
			fnstsw	ax
			test	ah, 5
			jp		SkipIf_98
		}*/
			//Magnify both width and height:  use weighted sample of 4 pixels
			//**** In this code, floating point is used to do some of the calculations, but I will avoid
			//**** using the FPU and will stick to integer computation with some limited precision.
			sint i0, i1, j0, j1, sum;
			//sint alpha, beta;		//These are suppose to be sreal in MESA code!
			sreal falpha, fbeta, foneminusalpha, foneminusbeta;//, s1, s2;
			uchar *src00, *src01, *src10, *src11;
			//uchar *check=dataout+widthout*heightout*components;
			//uchar *check2=datain+widthin*heightin*components;
			__asm
			{
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_66_78
				//24 bit section
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj:
				mov		i, ecx
				fild	i
				fmul	sy				; i0=sint(sreal(i)*sy);
				fist	i0				; keep the value in FPU register for later 
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b
				SkipIfStatement_firett_a:
				mov		i1, eax
				SkipIfStatement_firett_b:
				fisub	i0					; this is falpha in st(0) - falpha=sreal(i)*sy-sreal(i0);
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy
				EnterLoop2_sippy:
				mov		j, edx
				fild	j
				fmul	sx				; sreal(j)*sx
				fist	j0				; keep this in FPU register for later
				//if(j0+1>=widthin)
				//	j1=widthin-1;
				//else
				//	j1=j0+1;
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d
				SkipIfStatement_firett_c:
				mov		j1, eax
				SkipIfStatement_firett_d:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				//push	edx					; for safekeeping
				//push	ecx					; for safekeeping
				/*src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;*/
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 3
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 3
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 3
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 3
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*3;
				imul	eax, widthout
				add		eax, edx			; edx is j in this case
				imul	eax, 3
				add		eax, dataout
				mov		edx, eax			; now we have address in dst or in this case, edx
				mov		src00, esi
				mov		src01, edi
				mov		src10, ebx
				mov		src11, ecx
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				////////////
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy
				ExitLoop2_sippy:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj
				jmp		ExitSuperly
				//32 bit section
				ThirtyTwoBits_66_78:
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj_b:
				mov		i, ecx
				fild	i
				fld		sy
				fmulp	st(1), st(0)
				fist	i0				; keep the value in FPU register for later
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a_b
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b_b
				SkipIfStatement_firett_a_b:
				mov		i1, eax
				SkipIfStatement_firett_b_b:
				fild	i0
				fsubp	st(1), st(0)		; this is falpha in st(0)
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy_b
				EnterLoop2_sippy_b:
				mov		j, edx
				fild	j
				fld		sx
				fmulp	st(1), st(0)
				fist	j0				; keep this in FPU register for later
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c_b
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d_b
				SkipIfStatement_firett_c_b:
				mov		j1, eax
				SkipIfStatement_firett_d_b:
				fld		j0
				fsubp	st(1), st(0)		; this is fbeta in st(0)
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 4				; 4 bytes per pixel
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 4				; 4 bytes per pixel
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 4				; 4 bytes per pixel
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 4				; 4 bytes per pixel
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*3;
				imul	eax, widthout
				add		eax, edx			; edx is j
				imul	eax, 4				; 4 bytes per pixel
				add		eax, dataout
				mov		edx, eax			; now we have address in dst
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;				
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				//dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
				//			(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
				mov		al, [esi+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[3]*foneminusbeta
				mov		al, [edi+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[3]*foneminusbeta
				mov		al, [ecx+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+3], al	; dst[3] contains result now
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy_b
				ExitLoop2_sippy_b:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj_b
				ExitSuperly:
				fstp	st(0)
				//finit			; a single pop would suffice here

				jmp		Skip_Bunch_11
			}
			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					//alpha=i*intsy-i0*5000;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						if(j0+1>=widthin)
							j1=widthin-1;
						else
							j1=j0+1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//beta=j*intsx-j0*5000;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;
						//For each component	
						//s1=src00[0]*(1.0-fbeta)+src01[0]*fbeta;
						//s2=src10[0]*(1.0-fbeta)+src11[0]*fbeta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(5000-beta)+sint(src01[0])*beta)*(5000-alpha)+
						//                      (sint(src10[0])*(5000-beta)+sint(src11[0])*beta)*alpha)/25000000);
						
						//ERROR CHECKING CODE
						//if((sint(dst[0])<sint(c)-5)&&(sint(dst[0])>sint(c)+5))
						//	k++;
						//s1=src00[1]*(1.0-fbeta)+src01[1]*fbeta;
						//s2=src10[1]*(1.0-fbeta)+src11[1]*fbeta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(5000-beta)+sint(src01[1])*beta)*(5000-alpha)+
						//                      (sint(src10[1])*(5000-beta)+sint(src11[1])*beta)*alpha)/25000000);
						
						//s1=src00[2]*(1.0-fbeta)+src01[2]*fbeta;
						//s2=src10[2]*(1.0-fbeta)+src11[2]*fbeta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(5000-beta)+sint(src01[2])*beta)*(5000-alpha)+
						//                      (sint(src10[2])*(5000-beta)+sint(src11[2])*beta)*alpha)/25000000);
						
					}
				}
				//schar b[200];
				//sprintf(b, "Number of errors is %d", k);
				//MessageBox(NULL, b, "DEBUG", MB_OK);
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*4;
						src01=datain+(i0*widthin+j1)*4;
						src10=datain+(i1*widthin+j0)*4;
						src11=datain+(i1*widthin+j1)*4;
						dst=dataout+(i*widthout+j)*4;
						//For each component	
						//s1=src00[0]*(1.0-beta)+src01[0]*beta;
						//s2=src10[0]*(1.0-beta)+src11[0]*beta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(10000-beta)+sint(src01[0])*beta)*(10000-alpha)+
						//                      (sint(src10[0])*(10000-beta)+sint(src11[0])*beta)*alpha)/10000);
						//s1=src00[1]*(1.0-beta)+src01[1]*beta;
						//s2=src10[1]*(1.0-beta)+src11[1]*beta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(10000-beta)+sint(src01[1])*beta)*(10000-alpha)+
						//                     (sint(src10[1])*(10000-beta)+sint(src11[1])*beta)*alpha)/10000);
						//s1=src00[2]*(1.0-beta)+src01[2]*beta;
						//s2=src10[2]*(1.0-beta)+src11[2]*beta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(10000-beta)+sint(src01[2])*beta)*(10000-alpha)+
						//	(sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);
						//s1=src00[3]*(1.0-beta)+src01[3]*beta;
						//s2=src10[3]*(1.0-beta)+src11[3]*beta;
						dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
							(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
						//dst[3]=uchar(((sint(src00[3])*(10000-beta)+sint(src01[3])*beta)*(10000-alpha)+
						//                      (sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);

					}
				}
			}*/
			//Shrink width and/or height:  use an unweighted box filter
			_asm
			{
				//SkipIf_98:
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_xcd99
				//24 bit section
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz
				EnterLoop1_a_stuzz:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2
				Skipper_tyui:
				mov		i1, eax
				Skipper_tyui2:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178
				Enter_Loop2_c178:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2
				Skipper_tyui_zz:
				mov		j1, eax
				Skipper_tyui_zz2:
				mov		edi, i
				imul	edi, widthout
				add		edi, j
				imul	edi, 3
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1
				Enter_rtt1:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2
				Enter_rtt2:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2
				Exit_rtt2:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1
				Exit_rtt1:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_b
				Enter_rtt1_b:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_b
				Enter_rtt2_b:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				inc		esi
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3+1));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_b
				Exit_rtt2_b:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_b
				Exit_rtt1_b:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+1], al		; dst[1]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_c
				Enter_rtt1_c:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_c
				Enter_rtt2_c:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				add		esi, 2
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3+2));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_c
				Exit_rtt2_c:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_c
				Exit_rtt1_c:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+2], al		; dst[2]=(uchar)sum; (lower part of AX register)
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178
				ExitLoop2_c178:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz
				ExitLoop1_a_stuzz:
				jmp		CLEAR_IF_1023
				//32 bit section
				ThirtyTwoBits_xcd99:
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz_32
				EnterLoop1_a_stuzz_32:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui_32
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2_32
				Skipper_tyui_32:
				mov		i1, eax
				Skipper_tyui2_32:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178_32
				Enter_Loop2_c178_32:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz_32
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2_32
				Skipper_tyui_zz_32:
				mov		j1, eax
				Skipper_tyui_zz2_32:
				mov		edi, i
				imul	edi, widthout
				add		edi, j
				imul	edi, 4
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_32
				Enter_rtt1_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_32
				Enter_rtt2_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_32
				Exit_rtt2_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_32
				Exit_rtt1_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_b_32
				Enter_rtt1_b_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_b_32
				Enter_rtt2_b_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				inc		esi
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+1));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_b_32
				Exit_rtt2_b_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_b_32
				Exit_rtt1_b_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+1], al		; dst[1]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_c_32
				Enter_rtt1_c_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_c_32
				Enter_rtt2_c_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, 2
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+2));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_c_32
				Exit_rtt2_c_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_c_32
				Exit_rtt1_c_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+2], al		; dst[2]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_d_32
				Enter_rtt1_d_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_d_32
				Enter_rtt2_d_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, 3
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+3));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_d_32
				Exit_rtt2_d_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_d_32
				Exit_rtt1_d_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+3], al		; dst[3]=(uchar)sum; (lower part of AX register)
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178_32
				ExitLoop2_c178_32:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz_32
				ExitLoop1_a_stuzz_32:
				CLEAR_IF_1023:

				Skip_Bunch_11:
			}

			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*3;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3+1));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[1]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+1));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[1]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
					}
				}
			}*/
	}
	}	//dataAlignment==1
	else
	{
		//Don't forget to restore FPU state completly (the control register)
		/*__asm
		{
			fldcw	old_FPU_Flag	; load control word
		}*/
		return -5;		//Other data alignment are not implemented
	}

	//Don't forget to restore FPU state completly (the control register)
	/*__asm
	{
		fldcw	old_FPU_Flag	; load control word
	}*/
	return 1;	//Success
}

//This function will not verify if values being passed to function are valid.
//format must be RGB8 or RGBA8 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8)
//dataAlignment must be 1
//Returns 1 if success. 0 for failure, -5 for not yet implemented
sint glhScaleImage2_asm386(uint format, uint isUseLinearOrPointSample,
		sint dataAlignment,
		sint widthin, sint heightin,
		uchar *datain,
		sint widthout, sint heightout,
		uchar *dataout)
{
	sint components;		//Bits per pixel (BPP)
	sint i, j, tempo;
	//sreal *tempin, *tempout;
	sreal sx, sy, generalval;
	sint intsx, intsy;
	sreal one=1.0;
	//sint sizein, sizeout;
	//sint rowstride;
	//uchar *ubptr;
	sint ii;
	////////////////////////
	/*switch(format)
	{
	case GL_RGB:
		components=3;
		break;
	case GL_RGBA:
		components=4;
		break;
	default:
		return -5;
	}*/
	__asm
	{
		mov		edx, format
		cmp		edx, GL_RGB
		je		Address_GL_RGB
		cmp		edx, GL_RGB8
		je		Address_GL_RGB
		cmp		edx, GL_RGBA
		je		Address_GL_RGBA
		cmp		edx, GL_RGBA8
		je		Address_GL_RGBA
	}
	return -5;
	__asm
	{
		Address_GL_RGB:
		mov		components, 3
		jmp		FormatExit
		Address_GL_RGBA:
		mov		components, 4
		//jmp		FormatExit
		FormatExit:
	}
	if(dataAlignment==1)
	{
	/*if(widthout>1)
		sx=sreal(widthin-1)/sreal(widthout-1);
	else
		sx=sreal(widthin-1);
	if(heightout>1)
		sy=sreal(heightin-1)/sreal(heightout-1);
	else
		sy=sreal(heightin-1);*/
	__asm
	{
		mov		generalval, 461C4000h	; copy 10000.0
		//Need some temporary sint here.
		//First if statement
		mov		ecx, widthin
		dec		ecx
		mov		i, ecx
		mov		edx, widthout
		cmp		edx, 1
		jle		JUMPPOINT_a_q22bx
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fst		sx				; store into sx
		fmul	generalval
		fistp	intsx
		jmp		JUMPPOINT_a_q22bxsup
		JUMPPOINT_a_q22bx:
		fild	i				; have the FPU translate integer to sreal
		fst		sx				; store into sx
		fmul	generalval
		fistp	intsx
		JUMPPOINT_a_q22bxsup:
		//Second if statement
		mov		ecx, heightin
		dec		ecx
		mov		i, ecx
		mov		edx, heightout
		cmp		edx, 1
		jle		JUMPPOINT_b_yy789
		dec		edx
		mov		j, edx
		fild	i
		fidiv	j
		fst		sy				; store into sy
		fmul	generalval
		fistp	intsy
		jmp		JUMPPOINT_b_yy789sup
		JUMPPOINT_b_yy789:
		fild	i				; have the FPU translate integer to sreal
		fst		sy				; store into sy
		fmul	generalval
		fistp	intsy
		JUMPPOINT_b_yy789sup:
	}
	if(isUseLinearOrPointSample==FALSE)
	{
		//Gone use point sampling
		////////// DEBUG start /////////////////////////
		//Memory bounds check
		/*uchar *checking001, *checking002;
		checking001=datain+widthin*heightin*3;
		checking002=dataout+widthout*heightout*3;

		for(i=0; i<heightout; i++)
		{
			ii=i*sy;
			for(j=0; j<widthout; j++)
			{
				jj=j*sx;
				uchar *src=datain+(ii*widthin+jj)*components;
				uchar *dst=dataout+(i*widthout+j)*components;
				for(k=0; k<components; k++)
				{
					if(dst>=checking002)
					{
						MessageBox("Memory bound error (009zx)", "DEBUG", MB_OK);
						return 0;
					}
					if(src>=checking001)
					{
						MessageBox("Memory bound error (010zx)", "DEBUG", MB_OK);
						return 0;
					}
					*dst=*src;
					dst++;
					src++;
				}
			}
		}
		return 1;
		////////// DEBUG end /////////////////////////*/
		//Gone use point sampling
		/*if(components==3)
		{
			for(i=0; i<heightout; i++)
			{
				ii=sint(sreal(i)*sy);
				for(j=0; j<widthout; j++)
				{
					jj=sint(sreal(j)*sx);
					uchar *src=datain+(ii*widthin+jj)*3;
					uchar *dst=dataout+(i*widthout+j)*3;
					//For each component
					dst[0]=src[0];
					dst[1]=src[1];
					dst[2]=src[2];
				}
			}
		}
		else if(components==4)
		{
			for(i=0; i<heightout; i++)
			{
				ii=sint(sreal(i)*sy);
				for(j=0; j<widthout; j++)
				{
					jj=sint(sreal(j)*sx);
					uchar *src=datain+(ii*widthin+jj)*4;
					uchar *dst=dataout+(i*widthout+j)*4;
					//For each component
					dst[0]=src[0];
					dst[1]=src[1];
					dst[2]=src[2];
					dst[3]=src[3];
				}
			}
		}*/
		__asm
		{
			//ebx is dataout
			//ecx as i
			//edx as j
			//mov		tempo, 10		; need this later for truncation calculation
			mov		ebx, dataout
			cmp		components, 3
			jne		ThirtyTwoBits
			//This is the 24 bit version
			dec		heightout		; the last line of the image will be processed differently, this value will not be restored
			xor		ecx, ecx
			cmp		ecx, heightout
			jge		ExitLoops
			EnterLoop1:
			mov		i, ecx
			//fild	i
			//fmul	sy
			//fistp	ii
			mov		eax, intsy		; gone use truncation technic
			imul	eax, ecx		; contains i
			add		eax, 9999		; (i*intsy+9999)/10000 approximates ceilf(i*sy)
			cdq
			mov		ecx, 10000
			idiv	ecx
			mov		ii, eax
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoop1
			EnterLoop2:
			mov		j, edx
			//fild	j
			//fmul	sx
			//fistp	jj
			mov		eax, intsx		; gone use truncation technic
			imul	eax, edx		; edx contains j
			add		eax, 9999		; (j*intsx+9999)/10000 approximates ceilf(j*sx)
			cdq
			idiv	ecx				; destroys info in edx
			mov		edx, j
			//mov		jj, eax
			mov		esi, ii
			imul	esi, widthin
			add		esi, eax		; eax contains jj
			imul	esi, 3
			add		esi, datain
			mov		edi, i
			imul	edi, widthout
			add		edi, edx		; edx contains j
			imul	edi, 3
			add		edi, ebx
			//mov		ax, [esi]		; copy red and green component
			//mov		[edi], ax
			//mov		al, [esi+2]		; copy blue component
			//mov		[edi+2], al
			mov		eax, [esi]
			mov		[edi], eax			; copy 4 bytes
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2
			ExitLoop1:
			mov		ecx, i
			inc		ecx
			cmp		ecx, heightout
			jl		EnterLoop1
			//This is for the last line of the 24 bit image
			mov		i, ecx
			//fild	i
			//fmul	sy
			//fistp	ii
			mov		eax, intsy		; gone use truncation technic
			imul	eax, ecx		; ecx contains i
			add		eax, 9999		; (i*intsy+9999)/10000 approximates ceilf(i*sy)
			cdq
			mov		ecx, 10000
			idiv	ecx
			mov		ii, eax
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoops
			EnterLoop2_spk:
			mov		j, edx
			//fild	j
			//fmul	sx
			//fistp	jj
			mov		eax, intsx		; gone use truncation technic
			imul	eax, edx		; edx contains j
			add		eax, 9999		; (j*intsx+9999)/10000 approximates ceilf(j*sx)
			cdq
			idiv	ecx				; destroys info in edx
			mov		edx, j
			//mov		jj, eax
			mov		esi, ii
			imul	esi, widthin
			add		esi, eax		; eax contains jj
			imul	esi, 3
			add		esi, datain
			mov		edi, i
			imul	edi, widthout
			add		edi, edx		; edx contains j
			imul	edi, 3
			add		edi, ebx
			mov		ax, [esi]		; copy red and green component
			mov		[edi], ax
			mov		al, [esi+2]		; copy blue component
			mov		[edi+2], al
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2_spk
			jmp		ExitLoops		; skip 32 bit section
			ThirtyTwoBits:
			//This is the 32 bit version
			xor		ecx, ecx
			cmp		ecx, heightout
			jge		ExitLoops
			EnterLoop1b:
			mov		i, ecx
			//fild	i
			//fmul	sy
			//fistp	ii
			mov		eax, intsy		; gone use truncation technic
			imul	eax, ecx		; ecx contains i
			add		eax, 9999		; (i*intsy+9999)/10000 approximates ceilf(i*sy)
			cdq
			mov		ecx, 10000
			idiv	ecx
			mov		ii, eax
			xor		edx, edx
			cmp		edx, widthout
			jge		ExitLoop1b
			EnterLoop2b:
			mov		j, edx
			//fild	j
			//fmul	sx
			//fistp	jj
			mov		eax, intsy		; gone use truncation technic
			imul	eax, edx		; edx contains j
			add		eax, 9999		; (j*intsx+9999)/10000 approximates ceilf(j*sx)
			cdq
			idiv	ecx				; destroys info in edx
			mov		edx, j
			//mov		jj, eax
			mov		esi, ii
			imul	esi, widthin
			add		esi, eax		; eax contains jj
			imul	esi, 4
			add		esi, datain
			mov		edi, ecx		; contains i
			imul	edi, widthout
			add		edi, edx		; contains j
			imul	edi, 4
			add		edi, ebx
			mov		eax, [esi]		; copy color
			mov		[edi], eax
			inc		edx
			cmp		edx, widthout
			jl		EnterLoop2b
			ExitLoop1b:
			mov		ecx, i
			inc		ecx
			cmp		ecx, heightout
			jl		EnterLoop1b
			ExitLoops:
		}
		return 1;			//Success
	}
	else //if(isUseLinearOrPointSample==TRUE)
	{
		//The FPU's default behavior on Intel is to round or even the value which
		//causes a problem for this algorithm since we need truncation, so we change the FPU
		//control register bit 11 and 10 from 00 to 11
		/*short old_FPU_Flag, new_FPU_Flag;	//Must be 16 bit (word)
		__asm
		{
			fstcw	old_FPU_Flag	; store control word
			mov		ax, old_FPU_Flag
			and		ax, 0xF7FF		; clear bits 11 and 10
			or		ax, 0x0C00		; set them to 1
			mov		new_FPU_Flag, ax
			fldcw	new_FPU_Flag	; load control word
			//if((sx<1.0)&&(sy<1.0))
			fld		sx
			fcomp	one
			fnstsw	ax
			test	ah, 5
			jp		SkipIf_98
			fld		sy
			fcomp	one
			fnstsw	ax
			test	ah, 5
			jp		SkipIf_98
		}*/
			//Magnify both width and height:  use weighted sample of 4 pixels
			//**** In this code, floating point is used to do some of the calculations, but I will avoid
			//**** using the FPU and will stick to integer computation with some limited precision.
			sint i0, i1, j0, j1, sum;
			//sint alpha, beta;		//These are suppose to be sreal in MESA code!
			sreal falpha, fbeta, foneminusalpha, foneminusbeta;//, s1, s2;
			uchar *src00, *src01, *src10, *src11;
			//uchar *check=dataout+widthout*heightout*components;
			//uchar *check2=datain+widthin*heightin*components;
			__asm
			{
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_66_78
				//24 bit section
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj:
				mov		i, ecx
				fild	i
				fmul	sy				; i0=sint(sreal(i)*sy);
				fist	i0				; keep the value in FPU register for later 
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b
				SkipIfStatement_firett_a:
				mov		i1, eax
				SkipIfStatement_firett_b:
				fisub	i0					; this is falpha in st(0) - falpha=sreal(i)*sy-sreal(i0);
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy
				EnterLoop2_sippy:
				mov		j, edx
				fild	j
				fmul	sx				; sreal(j)*sx
				fist	j0				; keep this in FPU register for later
				//if(j0+1>=widthin)
				//	j1=widthin-1;
				//else
				//	j1=j0+1;
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d
				SkipIfStatement_firett_c:
				mov		j1, eax
				SkipIfStatement_firett_d:
				fisub	j0
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				//push	edx					; for safekeeping
				//push	ecx					; for safekeeping
				/*src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;*/
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 3
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 3
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 3
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 3
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*3;
				imul	eax, widthout
				add		eax, edx			; edx is j in this case
				imul	eax, 3
				add		eax, dataout
				mov		edx, eax			; now we have address in dst or in this case, edx
				mov		src00, esi
				mov		src01, edi
				mov		src10, ebx
				mov		src11, ecx
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				///////////
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy
				ExitLoop2_sippy:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj
				jmp		ExitSuperly
				//32 bit section
				ThirtyTwoBits_66_78:
				fld		one				; need this for later (1.0-falpha and 1.0 fbeta)
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitSuperly
				EnterLoop1_dashj_b:
				mov		i, ecx
				fild	i
				fld		sy
				fmulp	st(1), st(0)
				fist	i0				; keep the value in FPU register for later
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_a_b
				dec		ebx
				mov		i1, ebx
				jmp		SkipIfStatement_firett_b_b
				SkipIfStatement_firett_a_b:
				mov		i1, eax
				SkipIfStatement_firett_b_b:
				fild	i0
				fsubp	st(1), st(0)		; this is falpha in st(0)
				fst		falpha
				fsub	st(0), st(1)		; this is foneminusalpha in st(0)
				fchs						; must change the sign
				fstp	foneminusalpha
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_sippy_b
				EnterLoop2_sippy_b:
				mov		j, edx
				fild	j
				fld		sx
				fmulp	st(1), st(0)
				fist	j0				; keep this in FPU register for later
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		SkipIfStatement_firett_c_b
				dec		ebx
				mov		j1, ebx
				jmp		SkipIfStatement_firett_d_b
				SkipIfStatement_firett_c_b:
				mov		j1, eax
				SkipIfStatement_firett_d_b:
				fld		j0
				fsubp	st(1), st(0)		; this is fbeta in st(0)
				fst		fbeta
				fsub	st(0), st(1)		; this is foneminusbeta in st(0)
				fchs						; must change the sign
				fstp	foneminusbeta
				//Now esi is src00
				//Now edi is src01
				//Now ebx is src10
				//Now ecx is src11
				mov		eax, datain			; temporary (not enough registers to make this algorithm efficient)
				mov		esi, i0
				mov		edi, esi			; make a copy of i0 for efficiency
				imul	esi, widthin
				add		esi, j0
				imul	esi, 4				; 4 bytes per pixel
				add		esi, eax
				imul	edi, widthin
				add		edi, j1
				imul	edi, 4				; 4 bytes per pixel
				add		edi, eax
				mov		ebx, i1
				mov		ecx, ebx			; make a copy if i1 for efficiency
				imul	ebx, widthin
				add		ebx, j0
				imul	ebx, 4				; 4 bytes per pixel
				add		ebx, eax
				imul	ecx, widthin
				add		ecx, j1
				imul	ecx, 4				; 4 bytes per pixel
				add		ecx, eax
				mov		eax, i				; dst=dataout+(i*widthout+j)*3;
				imul	eax, widthout
				add		eax, edx			; edx is j
				imul	eax, 4				; 4 bytes per pixel
				add		eax, dataout
				mov		edx, eax			; now we have address in dst
				//dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
				//			(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
				xor		eax, eax
				mov		al, [esi]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[0]*foneminusbeta
				mov		al, [edi]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[0]*foneminusbeta
				mov		al, [ecx]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[0]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx], al	; dst[0] contains result now
				//dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
				//			(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;				
				mov		al, [esi+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[1]*foneminusbeta
				mov		al, [edi+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+1]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[1]*foneminusbeta
				mov		al, [ecx+1]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[1]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				mov		byte ptr[edx+1], al	; dst[1] contains result now
				//dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
				//			(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
				mov		al, [esi+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[2]*foneminusbeta
				mov		al, [edi+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+2]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[2]*foneminusbeta
				mov		al, [ecx+2]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[2]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+2], al	; dst[2] contains result now
				//dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
				//			(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
				mov		al, [esi+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src00[3]*foneminusbeta
				mov		al, [edi+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src01[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	foneminusalpha		; *foneminusalpha
				mov		al, [ebx+3]
				mov		tempo, eax
				fild	tempo
				fmul	foneminusbeta		; src10[3]*foneminusbeta
				mov		al, [ecx+3]
				mov		tempo, eax
				fild	tempo
				fmul	fbeta				; src11[3]*fbeta
				faddp	st(1), st(0)		; add them together
				fmul	falpha				; *falpha
				faddp	st(1), st(0)		; finally, add both parts to finish
				fistp	tempo				; store result in 32 bit integer
				mov		eax, tempo
				mov		byte ptr[edx+3], al	; dst[3] contains result now
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		EnterLoop2_sippy_b
				ExitLoop2_sippy_b:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_dashj_b
				ExitSuperly:
				fstp	st(0)
				//finit			; a single pop would suffice here

				jmp		Skip_Bunch_11
			}
			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					//alpha=i*intsy-i0*5000;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						if(j0+1>=widthin)
							j1=widthin-1;
						else
							j1=j0+1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//beta=j*intsx-j0*5000;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*3;
						src01=datain+(i0*widthin+j1)*3;
						src10=datain+(i1*widthin+j0)*3;
						src11=datain+(i1*widthin+j1)*3;
						dst=dataout+(i*widthout+j)*3;
						//For each component	
						//s1=src00[0]*(1.0-fbeta)+src01[0]*fbeta;
						//s2=src10[0]*(1.0-fbeta)+src11[0]*fbeta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(5000-beta)+sint(src01[0])*beta)*(5000-alpha)+
						//                      (sint(src10[0])*(5000-beta)+sint(src11[0])*beta)*alpha)/25000000);
						
						//ERROR CHECKING CODE
						//if((sint(dst[0])<sint(c)-5)&&(sint(dst[0])>sint(c)+5))
						//	k++;
						//s1=src00[1]*(1.0-fbeta)+src01[1]*fbeta;
						//s2=src10[1]*(1.0-fbeta)+src11[1]*fbeta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(5000-beta)+sint(src01[1])*beta)*(5000-alpha)+
						//                      (sint(src10[1])*(5000-beta)+sint(src11[1])*beta)*alpha)/25000000);
						
						//s1=src00[2]*(1.0-fbeta)+src01[2]*fbeta;
						//s2=src10[2]*(1.0-fbeta)+src11[2]*fbeta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(5000-beta)+sint(src01[2])*beta)*(5000-alpha)+
						//                      (sint(src10[2])*(5000-beta)+sint(src11[2])*beta)*alpha)/25000000);
						
					}
				}
				//schar b[200];
				//sprintf(b, "Number of errors is %d", k);
				//MessageBox(NULL, b, "DEBUG", MB_OK);
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					falpha=sreal(i)*sy-sreal(i0);
					foneminusalpha=1.0-falpha;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						fbeta=sreal(j)*sx-sreal(j0);
						foneminusbeta=1.0-fbeta;
						//Compute weighted average of pixels in rect (i0,j0)-(i1,j1)
						src00=datain+(i0*widthin+j0)*4;
						src01=datain+(i0*widthin+j1)*4;
						src10=datain+(i1*widthin+j0)*4;
						src11=datain+(i1*widthin+j1)*4;
						dst=dataout+(i*widthout+j)*4;
						//For each component	
						//s1=src00[0]*(1.0-beta)+src01[0]*beta;
						//s2=src10[0]*(1.0-beta)+src11[0]*beta;
						dst[0]=(src00[0]*foneminusbeta+src01[0]*fbeta)*foneminusalpha+
							(src10[0]*foneminusbeta+src11[0]*fbeta)*falpha;
						//dst[0]=uchar(((sint(src00[0])*(10000-beta)+sint(src01[0])*beta)*(10000-alpha)+
						//                      (sint(src10[0])*(10000-beta)+sint(src11[0])*beta)*alpha)/10000);
						//s1=src00[1]*(1.0-beta)+src01[1]*beta;
						//s2=src10[1]*(1.0-beta)+src11[1]*beta;
						dst[1]=(src00[1]*foneminusbeta+src01[1]*fbeta)*foneminusalpha+
							(src10[1]*foneminusbeta+src11[1]*fbeta)*falpha;
						//dst[1]=uchar(((sint(src00[1])*(10000-beta)+sint(src01[1])*beta)*(10000-alpha)+
						//                     (sint(src10[1])*(10000-beta)+sint(src11[1])*beta)*alpha)/10000);
						//s1=src00[2]*(1.0-beta)+src01[2]*beta;
						//s2=src10[2]*(1.0-beta)+src11[2]*beta;
						dst[2]=(src00[2]*foneminusbeta+src01[2]*fbeta)*foneminusalpha+
							(src10[2]*foneminusbeta+src11[2]*fbeta)*falpha;
						//dst[2]=uchar(((sint(src00[2])*(10000-beta)+sint(src01[2])*beta)*(10000-alpha)+
						//	(sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);
						//s1=src00[3]*(1.0-beta)+src01[3]*beta;
						//s2=src10[3]*(1.0-beta)+src11[3]*beta;
						dst[3]=(src00[3]*foneminusbeta+src01[3]*fbeta)*foneminusalpha+
							(src10[3]*foneminusbeta+src11[3]*fbeta)*falpha;
						//dst[3]=uchar(((sint(src00[3])*(10000-beta)+sint(src01[3])*beta)*(10000-alpha)+
						//                      (sint(src10[2])*(10000-beta)+sint(src11[2])*beta)*alpha)/10000);

					}
				}
			}*/
		
			//Shrink width and/or height:  use an unweighted box filter
			_asm
			{
				//SkipIf_98:
				//ecx is i
				//edx is j
				cmp		components, 3
				jne		ThirtyTwoBits_xcd99
				//24 bit section
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz
				EnterLoop1_a_stuzz:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2
				Skipper_tyui:
				mov		i1, eax
				Skipper_tyui2:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178
				Enter_Loop2_c178:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2
				Skipper_tyui_zz:
				mov		j1, eax
				Skipper_tyui_zz2:
				mov		edi, i
				imul	edi, widthout
				add		edi, j
				imul	edi, 3
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1
				Enter_rtt1:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2
				Enter_rtt2:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2
				Exit_rtt2:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1
				Exit_rtt1:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_b
				Enter_rtt1_b:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_b
				Enter_rtt2_b:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				inc		esi
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3+1));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_b
				Exit_rtt2_b:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_b
				Exit_rtt1_b:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+1], al		; dst[1]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_c
				Enter_rtt1_c:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_c
				Enter_rtt2_c:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 3
				add		esi, 2
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*3+2));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_c
				Exit_rtt2_c:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_c
				Exit_rtt1_c:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+2], al		; dst[2]=(uchar)sum; (lower part of AX register)
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178
				ExitLoop2_c178:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz
				ExitLoop1_a_stuzz:
				jmp		CLEAR_IF_1023
				//32 bit section
				ThirtyTwoBits_xcd99:
				xor		ecx, ecx
				cmp		ecx, heightout
				jge		ExitLoop1_a_stuzz_32
				EnterLoop1_a_stuzz_32:
				mov		i, ecx
				fild	i
				fmul	sy
				fistp	i0			; store as integer
				mov		eax, i0
				inc		eax
				mov		ebx, heightin
				cmp		eax, ebx
				jl		Skipper_tyui_32
				dec		ebx
				mov		i1, ebx
				jmp		Skipper_tyui2_32
				Skipper_tyui_32:
				mov		i1, eax
				Skipper_tyui2_32:
				xor		edx, edx
				cmp		edx, widthout
				jge		ExitLoop2_c178_32
				Enter_Loop2_c178_32:
				mov		j, edx
				fild	j
				fmul	sx
				fistp	j0
				mov		eax, j0
				inc		eax
				mov		ebx, widthin
				cmp		eax, ebx
				jl		Skipper_tyui_zz_32
				dec		ebx
				mov		j1, ebx
				jmp		Skipper_tyui_zz2_32
				Skipper_tyui_zz_32:
				mov		j1, eax
				Skipper_tyui_zz2_32:
				mov		edi, i
				imul	edi, widthout
				add		edi, j
				imul	edi, 4
				add		edi, dataout
				//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
				//push	ecx				; need this for looping and is ii
				//push	edx				; need this for looping and is jj
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_32
				Enter_rtt1_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_32
				Enter_rtt2_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_32
				Exit_rtt2_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_32
				Exit_rtt1_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi], al		; dst[0]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_b_32
				Enter_rtt1_b_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_b_32
				Enter_rtt2_b_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				inc		esi
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+1));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_b_32
				Exit_rtt2_b_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_b_32
				Exit_rtt1_b_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+1], al		; dst[1]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_c_32
				Enter_rtt1_c_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_c_32
				Enter_rtt2_c_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, 2
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+2));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_c_32
				Exit_rtt2_c_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_c_32
				Exit_rtt1_c_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+2], al		; dst[2]=(uchar)sum; (lower part of AX register)
				///////////////////////////REPEAT the above
				xor		ebx, ebx		; need for computing sum
				mov		sum, 0			; sum=0
				mov		ecx, i0
				cmp		ecx, i1
				jg		Exit_rtt1_d_32
				Enter_rtt1_d_32:
				mov		edx, j0
				cmp		edx, j1
				jg		Exit_rtt2_d_32
				Enter_rtt2_d_32:
				mov		esi, ecx
				imul	esi, widthin
				add		esi, edx
				imul	esi, 4
				add		esi, 3
				add		esi, datain
				mov		bl, [esi]				; sum+=sint(*(datain+(ii*widthin+jj)*4+2));
				add		sum, ebx
				inc		edx
				cmp		edx, j1
				jle		Enter_rtt2_d_32
				Exit_rtt2_d_32:
				inc		ecx
				cmp		ecx, i1
				jle		Enter_rtt1_d_32
				Exit_rtt1_d_32:
				mov		eax, sum		; sum/=(j1-j0+1)*(i1-i0+1);
				mov		ebx, j1
				sub		ebx, j0
				inc		ebx
				mov		ecx, i1
				sub		ecx, i0
				inc		ecx
				imul	ecx, ebx
				cdq
				idiv	ecx				; result is in eax
				mov		[edi+3], al		; dst[3]=(uchar)sum; (lower part of AX register)
				////////////////////////////
				//pop		edx				; don't forget to pop
				//pop		ecx				; don't forget to pop
				mov		edx, j
				inc		edx
				cmp		edx, widthout
				jl		Enter_Loop2_c178_32
				ExitLoop2_c178_32:
				mov		ecx, i
				inc		ecx
				cmp		ecx, heightout
				jl		EnterLoop1_a_stuzz_32
				ExitLoop1_a_stuzz_32:
				CLEAR_IF_1023:

				Skip_Bunch_11:
			}

			/*if(components==3)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i0+1>=heightin)
						i1=heightin-1;
					else
						i1=i0+1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*3;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3+1));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[1]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*3+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
					}
				}
			}
			else if(components==4)
			{
				for(i=0; i<heightout; i++)
				{
					i0=sint(sreal(i)*sy);
					i1=i0+1;
					if(i1>=heightin)
						i1=heightin-1;
					//i1=(i+1)*sy-EPSILON;
					for(j=0; j<widthout; j++)
					{
						j0=sint(sreal(j)*sx);
						j1=j0+1;
						if(j1>=widthin)
							j1=widthin-1;
						//j1=(j+1)*sx-EPSILON;
						dst=dataout+(i*widthout+j)*4;
						//Compute average of pixels in the rectangle (i0,j0)-(i1,j1)
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[0]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+1));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[1]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+2));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
						sum=0;
						for(ii=i0; ii<=i1; ii++)
						{
							for(jj=j0; jj<=j1; jj++)
							{
								sum+=sint(*(datain+(ii*widthin+jj)*4+3));
							}
						}
						sum/=(j1-j0+1)*(i1-i0+1);
						dst[2]=(uchar)sum;
					}
				}
			}*/
		//Don't forget to restore FPU state completly (the control register)
		/*__asm
		{
			fldcw	old_FPU_Flag	; load control word
		}*/
	}
	return 1;		//Success
	}	//dataAlignment==1

	return -5;		//Other data alignment are not implemented
}
#endif // #if USE_INLINE_ASM
