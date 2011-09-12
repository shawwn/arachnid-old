// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the GLHLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// GLHLIB_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef GLHLIB_EXPORTS
#define GLHLIB_API __declspec(dllexport)
#else
#define GLHLIB_API __declspec(dllimport)
#endif

#ifndef _GLHLIB_H_
#define _GLHLIB_H_

/*This is version 2.1*/




//Don't put this in the released glhlib.h
#include "global.h"



#pragma warning(disable: 4244)	//Shut up about double to float casting
#pragma warning(disable: 4305)	//'=' : truncation from 'const double' to 'float'


#define GLH_ALIGN16 __declspec(align(16))















//Pi value related - don't include this in release version of .h
#define PI_FULL_DEF			6.283185307179586476925286766559
#define PI_DEF				3.1415926535897932384626433832795
#define PI_2_DEF			1.5707963267948966192313216916398
#define PI_4_DEF			0.78539816339744830961566084581988

/*Global stuff*/
#ifndef NULL
#define NULL		0
#endif

#ifdef __cplusplus
extern "C"{
#endif

/*Defines -start*/

/*Part 1*/
#define GLH_ENGLISH							0x000186A0

#define GLH_GLHLIB_VERSION					0x000186A0
#define GLH_GLHLIB_URL						0x000186A2
#define GLH_GLHLIB_DESCRIBE					0x000186A3
#define GLH_OPENGL_VERSION					0x000186A4
#define GLH_OPENGL_SUBVERSION				0x000186A5
#define GLH_OPENGL_DRIVERVERSION			0x000186A6
#define GLH_GLSL_VERSION					0x000186A7
#define GLH_OPENGL_HARDWAREACCELERATION		0x000186A8
#define GLH_GPU_SHADERMODEL					0x000186A9
#define GLH_VENDOR							0x000186AA


//glhGetIntegerv(GLH_VENDOR, params) returns one of these in params
#define VENDOR_UNKNOWN						0x000000000
#define VENDOR_ATI							0x000000001			//ATI and AMD are the same
#define VENDOR_NVIDIA						0x000000002
#define VENDOR_INTEL						0x000000003
#define VENDOR_SIS							0x000000004
#define VENDOR_3DFX							0x000000005
#define VENDOR_3DLABS						0x000000006
#define VENDOR_MESA							0x000000007
#define VENDOR_DIAMONDMULTIMEDIA			0x000000008
#define VENDOR_XGI							0x000000009
#define VENDOR_ELSA							0x00000000A
#define VENDOR_IMAGINATIONTECHNOLOGIES		0x00000000B
#define VENDOR_MATROX						0x00000000C
#define VENDOR_MICROSOFT					0x00000000D
#define VENDOR_S3							0x00000000E

/*Part 2*/
#define GLH_ERROR_NONE						0xF0000000
#define GLH_ERROR_OUT_OF_MEMORY				0xF0000001
#define GLH_ERROR_INVALID_PARAM				0xF0000002
#define GLH_ERROR_INVALID_INDEX				0xF0000003
#define GLH_ERROR_INVALID_ID				0xF0000004
#define GLH_ERROR_INVALID_VALUE				0xF0000005
#define GLH_ERROR_STACK_OVERFLOW			0xF0000006
#define GLH_ERROR_STACK_UNDERFLOW			0xF0000007
#define GLH_ERROR_INTERNALERROR				0xF0000008


#define GLH_MODE_NONE						0x01000000
#define GLH_MODE_STANDARD					0x0100000F
#define GLH_MODE_ALTERNATE					0x010000F0
#define GLH_MODE_ALTERNATE2					0x010000F1
#define GLH_MODE_ALTERNATE3					0x010000F2
#define GLH_MODE_ALTERNATE4					0x010000F3
#define GLH_INTERPOLATIONQUALITY_LINEAR		0xF0000001
#define GLH_INTERPOLATIONQUALITY_COS		0xF0000002

#define GLH_OPTIMIZE_NONE					0x00100000
#define GLH_OPTIMIZE_MMX_SSE				0x0010000F


//Direct3D : pixels shader
#define	GLH_SHADERTYPE_PS_1_1			0x02000001
#define	GLH_SHADERTYPE_PS_1_2			0x02000002
#define	GLH_SHADERTYPE_PS_1_3			0x02000003
#define	GLH_SHADERTYPE_PS_1_4			0x02000004
#define	GLH_SHADERTYPE_PS_2_0			0x0200000A
#define	GLH_SHADERTYPE_PS_2_X			0x0200000B
#define	GLH_SHADERTYPE_PS_2_A			0x0200000C
#define	GLH_SHADERTYPE_PS_2_SW			0x0200000D
#define	GLH_SHADERTYPE_PS_3_0			0x02000020
#define	GLH_SHADERTYPE_PS_3_SW			0x02000021

//Direct3D : vertex shader
#define	GLH_SHADERTYPE_VS_1_1			0x03000001
#define	GLH_SHADERTYPE_VS_1_2			0x03000002
#define	GLH_SHADERTYPE_VS_1_3			0x03000003
#define	GLH_SHADERTYPE_VS_1_4			0x03000004
#define	GLH_SHADERTYPE_VS_2_0			0x0300000A
#define	GLH_SHADERTYPE_VS_2_X			0x0300000B
#define	GLH_SHADERTYPE_VS_2_A			0x0300000C
#define	GLH_SHADERTYPE_VS_2_SW			0x0300000D
#define	GLH_SHADERTYPE_VS_3_0			0x03000020
#define	GLH_SHADERTYPE_VS_3_SW			0x03000021

//OpenGL : ASM vertex program
#define GLH_SHADERTYPE_ARBVP_1_0		0x03000010

//OpenGL : ASM fragment program
#define GLH_SHADERTYPE_ARBFP_1_0		0x02000010

//OpenGL : GLSL vertex shader
#define GLH_SHADERTYPE_GLSL_VS_1_00		0x04000000

//OpenGL : GLSL fragment shader
#define GLH_SHADERTYPE_GLSL_FS_1_00		0x04000001



#define GLH_INDEX_OF_REFRACTION_VACUUM				1.0
#define GLH_INDEX_OF_REFRACTION_AIR					1.000293
#define GLH_INDEX_OF_REFRACTION_ICE					1.31
#define GLH_INDEX_OF_REFRACTION_WATER				1.333333
#define GLH_INDEX_OF_REFRACTION_ETHYLALCOHOL		1.36
#define GLH_INDEX_OF_REFRACTION_FLUORITE			1.43
#define GLH_INDEX_OF_REFRACTION_POPPYSEEDOIL		1.469
#define GLH_INDEX_OF_REFRACTION_OLIVEOIL			1.47
#define GLH_INDEX_OF_REFRACTION_LINSEEDOIL			1.478
#define GLH_INDEX_OF_REFRACTION_PLEXIGLASS			1.51
#define GLH_INDEX_OF_REFRACTION_IMMERSIONOIL		1.515
#define GLH_INDEX_OF_REFRACTION_CROWNGLASS			1.52
#define GLH_INDEX_OF_REFRACTION_QUARTZ				1.54
#define GLH_INDEX_OF_REFRACTION_SODIUMCHLORIDE		1.54
#define GLH_INDEX_OF_REFRACTION_LIGHTFLINTGLASS		1.58
#define GLH_INDEX_OF_REFRACTION_DENSEFLINTGLASS		1.66
#define GLH_INDEX_OF_REFRACTION_TOURMALINE			1.62
#define GLH_INDEX_OF_REFRACTION_GARNET				1.73
#define GLH_INDEX_OF_REFRACTION_ZIRCON				1.923
#define GLH_INDEX_OF_REFRACTION_CUBICZIRCONIA		2.14
#define GLH_INDEX_OF_REFRACTION_DIAMOND				2.417
#define GLH_INDEX_OF_REFRACTION_RUTILE				2.907
#define GLH_INDEX_OF_REFRACTION_GALLIUMPHOSPHIDE	3.5





#define GLH_UP										0x00000001
#define GLH_DOWN									0x00000002
#define GLH_LEFT									0x00000004
#define GLH_RIGHT									0x00000008



//PURPOSE:	For indices, if we want unsigned short or unsigned int
enum GLH_INDEXFORMAT
{
	GLH_INDEXFORMAT_16BIT,			//unsigned short
	GLH_INDEXFORMAT_32BIT			//unsigned int
};

//PURPOSE:	Some predefined vertex formats and following it are some struct that go along with it
enum GLH_VERTEXFORMAT
{
	GLH_VERTEXFORMAT_NULL,
	GLH_VERTEXFORMAT_V,				//Only vertex (xyz)
	GLH_VERTEXFORMAT_VT,			//vertex and texcoord (xyz, st)
	GLH_VERTEXFORMAT_VNT,			//vertex, normal, texcoord (xyz, nx-ny-nz, st)
	GLH_VERTEXFORMAT_VNT3,			//vertex, normal, texcoord but the r component is not used (xyz, nx-ny-nz, str)
									//You could write your own texcoords here for cubemap access.
	GLH_VERTEXFORMAT_VNTT3T3,		//xyz, nx-ny-nz, st, tangent (xyz), binormal (xyz)
	GLH_VERTEXFORMAT_VNTT3			//xyz, nx-ny-nz, st, tangent (xyz) but no binormal
									//In this case, you would compute the binormal in your shader
									//by doing binormal = cross(normal, tangent)
									//WARNING : You should have texcoords generated since these are using to compute
									//          tangent and binormals.
									//          Example : Cube.TexCoordStyle[0]=1;
									//                    Setting that to 1 will have the class Cube generate texcoords.
};


//Vertex
//SIZE : 4+4+4 = 4*3 = 12 bytes
//It's better to make it multiple of 32
//32-12 = 20 bytes (of garbage should be added)
//20/4 = 5 floats should be added
struct GLHVertex_V
{
	float	x, y, z;
	float	garbage[5];
};


//Vertex, tex0
//
//SIZE : 4+4+4 +4+4 = 4*6 = 20 bytes
//It's better to make it multiple of 32
//32-20 = 12 bytes (of garbage should be added)
//12/4 = 3 floats should be added
struct GLHVertex_VT
{
	float	x, y, z;
	float	s0, t0;
	float	garbage[3];
};

//Vertex, normal, tex0
//
//SIZE : 4+4+4 +4+4+4 +4+4 = 4*8 = 32 bytes
struct GLHVertex_VNT
{
	float	x, y, z;
	float	nx, ny, nz;
	float	s0, t0;
};

//Vertex, normal, tex0
//
//SIZE : 4+4+4 +4+4+4 +4+4+4 = 4*9 = 36 bytes
//It's better to make it multiple of 32
//32+32 = 64 bytes
//64-36 = 28 bytes (of garbage should be added)
//28/4 = 7 floats should be added
struct GLHVertex_VNT3
{
	float	x, y, z;
	float	nx, ny, nz;
	float	s0, t0, r0;
	float	garbage[7];
};


//Vertex, normal, tex0, tex1, tex2
//
//SIZE : 4+4+4 +4+4+4 +4+4 +4+4+4 +4+4+4 = 4*14 = 56 bytes
//It's better to make it multiple of 32
//32+32 = 64
//64-56 = 8 bytes (of garbage should be added)
//8/4 = 2 floats should be added
struct GLHVertex_VNTT3T3
{
	float	x, y, z;
	float	nx, ny, nz;
	float	s0, t0;
	float	s1, t1, r1;
	float	s2, t2, r2;
	float	garbage[2];
};

//Vertex, normal, tex0, tex1
//
//SIZE : 4+4+4 +4+4+4 +4+4 +4+4+4 = 4*11 = 44 bytes
//It's better to make it multiple of 32
//32+32 = 64
//64-44 = 20 bytes (of garbage should be added)
//20/4 = 5 floats should be added
struct GLHVertex_VNTT3
{
	float	x, y, z;
	float	nx, ny, nz;
	float	s0, t0;
	float	s1, t1, r1;
	float	garbage[5];
};




//Don't put this in the released glhlib.h
sint CheckForParameterErrors_Geometry(tbool check1, float width, tbool check2, float height, tbool check3, float length,
	GLH_INDEXFORMAT indexFormat);




//PURPOSE:	Get GL 1.5 capabilities.
//			See function glhGetGLDeviceCaps_GL_1_5 on this page.
struct GLCAPS_GL_1_5
{
	int	Size;							//The size of this struct

	//Buffers
	unsigned int	RGBAModeb;
	unsigned int	DoubleBufferedb;
	int		AuxBufferCounti;
	unsigned int	Stereob;
	int		RedBitsi;				//Main plain (back buffer in case of double buffering or front buffer in case of single buffering)
	int		GreenBitsi;
	int		BlueBitsi;
	int		AlphaBitsi;
	int		DepthBitsi;
	int		StencilBitsi;
	int		AccumRedBitsi;
	int		AccumGreenBitsi;
	int		AccumBlueBitsi;
	int		AccumAlphaBitsi;

	//Clip plane
	int	MaxClipPlanesi;

	//Matrices and stacks
	int	MaxModelviewStackDepthi;
	int	MaxProjectionStackDepthi;
	int	MaxTextureStackDepthi;
	int	MaxNameStackDepthi;
	int	MaxListNestingi;
	int	MaxEvalOrderi;
	int	MaxAttribStackDepthi;
	int	MaxClientAttribStackDepthi;

	//Pixel and fragments
	int	SubPixelBitsi;
	int	Samplesi;						//Coverage mask size
	int	SampleBuffersi;					//Number if multisample buffers
	int	QueryCounterBitsi;

	//Textures
	int	MaxTexture1DSizei;				//One value for width
	int	MaxTexture2DSizei;				//One value for both width and height
	int	MaxTexture3DSizei;				//One value for both width and height and depth
	int	MaxTextureCubeSizei;			//One value for both width and height
	int	MaxTextureLODBiasi;
	int	MaxTextureUnitsi;
	int	CompressedTextureFormatsi;
	int	NumberOfCompressedTextureFormatsi;

	//Viewport
	int	MaxViewportDimensionsi[2];		//Width, height

	//Primitives
	float	AliasedPointSizeRangef[2];		//Smallest and largest
	float	SmoothPointSizeRangef[2];		//Smallest and largest
	float	AliasedLineWidthRangef[2];		//Smallest and largest
	float	SmoothLineWidthRangef[2];		//Smallest and largest

	//Arrays
	int	MaxElementsIndicesi;
	int	MaxElementsVerticesi;

	//Lights
	int	MaxLightsi;
};


//PURPOSE:	Get GL 2.0 capabilities.
//			See function glhGetGLDeviceCaps_GL_2_0 on this page.
struct GLCAPS_GL_2_0 : public GLCAPS_GL_1_5
{
	//Vertex related
	int	MaxVertexAttribsi;

	//Texture related
	int	MaxCombinedTextureImageUnitsi;
	int	MaxTextureImageUnitsi;
	int	MaxTextureCoordsi;
};


//PURPOSE:	Get information about GL_ARB_vertex_program
//			See function glhGetGLDeviceCaps_ARB_vp_1_0 on this page.
struct GLCAPS_ARB_vp_1_0
{
	int	Size;							//The size of this struct

	int	MaxProgramInstructionsVPi;
	int	MaxProgramNativeInstructionsVPi;
	int	MaxProgramTemporariesVPi;
	int	MaxProgramNativeTemporariesVPi;
	int	MaxProgramParametersVPi;
	int	MaxProgramNativeParametersVPi;
	int	MaxProgramAttribsVPi;
	int	MaxProgramNativeAttribsVPi;
	int	MaxProgramAddressRegistersVPi;
	int	MaxProgramNativeAddressRegistersVPi;
	int	MaxProgramLocalParametersVPi;
	int	MaxProgramEnvParametersVPi;
	int	MaxVertexAttribsi;
	int	MaxProgramMatricesi;
	int	MaxProgramMatrixStackDepthi;
};

//PURPOSE:	Get information about GL_ARB_fragment_program
//			See function glhGetGLDeviceCaps_ARB_fp_1_0 on this page.
struct GLCAPS_ARB_fp_1_0
{
	int	Size;							//The size of this struct

	int	MaxProgramInstructionsFPi;
	int	MaxProgramNativeInstructionsFPi;
	int	MaxProgramTemporariesFPi;
	int	MaxProgramNativeTemporariesFPi;
	int	MaxProgramParametersFPi;
	int	MaxProgramNativeParametersFPi;
	int	MaxProgramAttributesFPi;
	int	MaxProgramNativeAttributesFPi;
	int	MaxProgramEnvParametersFPi;
	int	MaxProgramALUInstructionsFPi;
	int	MaxProgramNativeALUInstructionsFPi;
	int	MaxProgramTEXInstructionsFPi;
	int	MaxProgramNativeTEXInstructionsFPi;
	int	MaxProgramTEXIndirectionsFPi;
	int	MaxProgramNativeTEXIndirectionsFPi;
	int	MaxProgramMatricesi;
	int	MaxProgramMatrixStackDepthi;
	int	MaxTextureCoordsi;
	int	MaxTextureImageUnitsi;
};


//PURPOSE:	Get information about GL 2.0 (and above) GLSL
//			See function glhGetGLDeviceCaps_GL2_0_GLSL on this page.
struct GLCAPS_GL_2_0_GLSL
{
	int	Size;

	char	ShadingLanguageVersionc[64];	//String GLSL version returned by glGetString
	float	ShadingLanguageVersionf;		//Float GLSL version. Same value as returned by glhGetGLSLMainVersion

	int	MaxVertexUniformComponentsi;
	int	MaxVertexTextureImageUnitsi;
	int	MaxFragmentUniformComponentsi;
	int	MaxVaryingFloatsi;
	
};

/*Defines -end*/

/*Block 0 -start*/

//PURPOSE:	With the new features added in glh, we need a init, shutdown and geterror functions.
//glhGetError and glhCopyErrorInto_cBuffer appear in Block 14
GLHLIB_API void glhInitLibrary();
GLHLIB_API void glhShutDownLibrary();

/*Block 0 -end*/


/*Block 1 -start*/
//****************************************************************************
//THIS IS THE INFORMATION BLOCK.
//Collect information on glh, glu, GL, GL ES.
//The glh project will either be compiled for GL or GL ES.

//PURPOSE:	Get some info about the glh library. See section Defines Part 1
//
//			**** Note : glhGetOpenGLMainVersionAsString has been deleted. Use glhGetString(GLH_ENGLISH, GLH_OPENGL_VERSION)
//			            glhGetOpenGLSubVersionAsString has been deleted. Use glhGetString(GLH_ENGLISH, GLH_OPENGL_SUBVERSION)
//			            glhGetOpenGLDriverVersionAsString has been deleted. Use glhGetString(GLH_ENGLISH, GLH_OPENGL_DRIVERVERSION)
//			            glhGetGLSLMainVersionAsString has been deleted. Use glhGetString(GLH_ENGLISH, GLH_GLSL_VERSION)
//
//PARAMETERS:
//			[in] language: GLH_ENGLISH
//			[in] token:	GLH_GLHLIB_VERSION, GLH_GLHLIB_URL, GLH_GLHLIB_DESCRIBE
//			            GLH_OPENGL_VERSION : If glGetString(GL_VERSION) returns "1.3.1", this will return "1.3"
//			            GLH_OPENGL_SUBVERSION : If glGetString(GL_VERSION) returns "1.3.1", this function will return "1"
//			            GLH_OPENGL_DRIVERVERSION : Gets the OpenGL driver version and copies into buffer.
//			            buffer is a NULL string if something goes wrong.
//			            This function calls glGetString(GL_VERSION)
//			            and expects the version number to come after the OpenGL
//			            version number. Example :  1.2.1 34.56.12 so driver version is 34.56.12
//			            Some vendors do not provide the driver version so this will return
//			            an empty string in those cases.
//			            GLH_GLSL_VERSION : If glGetString(GL_SHADING_LANGUAGE_VERSION) returns "1.3.1", this function will return "1.3"
//			            GLH_GPU_SHADERMODEL : This is a hack and might change in the future. Most GL programmers want to ID the GPU
//			            and I think it's a good idea to be able to do so.
//			            This function tries to guess the Shader Model (SM) supported by the GPU.
//			            It checks for the presence of the extension GL_NV_vertex_program3, which would
//			            mean SM 3.0 for NV hw.
//			            It checks for the presence of GL_ATI_shader_texture_lod, which would mean SM 3.0 for ATI hw.
//			            If those extensions are not found, the code assumes you have SM 2.0
//			            It does not check to see if GL 2.0 is supported.
//			            It does not check for GL_ARB_shading_language_100 and the others.
//			            **** You might also be interested in glhGetIntegerv(GLH_VENDOR, params) ****
//
//			Update : The Geforce 8800 introduced a whole set of extensions, so if GL_EXT_gpu_shader4
//			         is found, you have a SM 4.0 GPU
//
//
//RETURN:
//			Pointer to constant string if valid language and token, else NULL.
GLHLIB_API const char* glhGetString(unsigned int language, unsigned int token);


//PURPOSE:	Get some info about the glh library. See section Defines Part 1
//			**** With some of the pnames, you get a single value, in others major and minor number.
//
//			**** Note : glhGetOpenGLMainVersion has been deleted. Use glhGetIntegerv(GLH_OPENGL_VERSION, params)
//			            glhGetGLSLMainVersion has been deleted. Use glhGetIntegerv(GLH_GLSL_VERSION, params)
//			            glhGetShaderModel has been deleted. Use glhGetIntegerv(GLH_GPU_SHADERMODEL, params)
//			            glhIsRunningInHardwareMode has been deleted. Use glhGetIntegerv(GLH_OPENGL_HARDWAREACCELERATION, params)
//
//PARAMETERS:
//			[in] pname:	GLH_GLHLIB_VERSION, then params will be the major and minor version
//
//			            GLH_OPENGL_HARDWAREACCELERATION : To find out if we are running in software or hardware mode.
//			            In other words, are we running using an ICD or MCD mechanism
//			            or Windows GDI. It works by checking if the first 3 letters is GDI.
//			            It searches for the word MESA (Mesa), since MESA is software on Windows.
//			            It searches for the word Direct3D (Direct 3D), since Direct3D is considered
//			            a software solution.
//			            Returns 1 for YES and 0 for NO
//			            GLH_OPENGL_VERSION returns major and minor GL version
//			            GLH_GLSL_VERSION returns major and minor GLSL version
//			            This is a hack and might change in the future. Most GL programmers want to ID the GPU
//			            and I think it's a good idea to be able to do so.
//			            GLH_GPU_SHADERMODEL : returns major and minor version.
//			            Tries to guess the Shader Model (SM) supported by the GPU.
//			            It checks for the presence of the extension GL_NV_vertex_program3, which would
//			            mean SM 3.0 for NV hw.
//			            It checks for the presence of GL_ATI_shader_texture_lod, which would mean SM 3.0 for ATI hw.
//			            If those extensions are not found, the code assumes you have SM 2.0
//			            It does not check to see if GL 2.0 is supported.
//			            It does not check for GL_ARB_shading_language_100 and the others.
//			            **** You might also be interested in glhGetIntegerv(GLH_VENDOR, params) ****
//
//			            Update : The Geforce 8800 introduced a whole set of extensions, so if GL_EXT_gpu_shader4
//			            is found, you have a SM 4.0 GPU
//
//			            GLH_VENDOR : returns some ID like VENDOR_UNKNOWN, VENDOR_ATI, VENDOR_NVIDIA, VENDOR_INTEL
//
//			[out] params : returns integers based on what you supply to pname
GLHLIB_API void glhGetIntegerv(unsigned int pname, int *params);



//PURPOSE:	Call this function to get GL 1.5 device capabilities
//
//PARAMETERS:
//			[out] errorMessage : If error occurs, a message is written here. Can be NULL.
//			                     Should be at least 512 bytes large.
//			[in] adapterCount : unused
//			[in] deviceType : unused
//			[in, out] pcaps : pointer to GLCAPS_GL_1_5
//			                  pcaps->Size must be sizeof(GLCAPS_GL_1_5)
//
//RETURN:
//			1 for success
//			-1 for error plus message.
GLHLIB_API int glhGetGLDeviceCaps_GL_1_5(char *errorMessage, int adapterCount, int deviceType, GLCAPS_GL_1_5 *pcaps);

//PURPOSE:	Call this function to get GL 2.0 device capabilities
//			You might also want to call glhGetGLDeviceCaps_GL2_0_GLSL to get GLSL related variables.
//
//PARAMETERS:
//			[out] errorMessage : If error occurs, a message is written here. Can be NULL.
//			                     Should be at least 512 bytes large.
//			[in] adapterCount : unused
//			[in] deviceType : unused
//			[in, out] pcaps : pointer to GLCAPS_GL_2_0
//			                  pcaps->Size must be sizeof(GLCAPS_GL_2_0)
//
//RETURN:
//			1 for success
//			-1 for error plus message.
GLHLIB_API int glhGetGLDeviceCaps_GL_2_0(char *errorMessage, int adapterCount, int deviceType, GLCAPS_GL_2_0 *pcaps);


//PURPOSE:	In the same spirit as glhGetGLDeviceCaps_GL_1_5, this gets specific information
//			about the GL_ARB_vertex_program extension.
//			**** This will call wglGetProcAddress to get pointer to glGetProgramivARB ****
//
//PARAMETERS:
//			[out] errorMessage : If error occurs, a message is written here. Can be NULL.
//			                     Should be at least 512 bytes large.
//			[in] adapterCount : unused
//			[in] deviceType : unused
//			[in, out] pcaps : pointer to GLCAPS_ARB_vp_1_0
//			                  pcaps->Size must be sizeof(GLCAPS_ARB_vp_1_0)
//
//RETURN:
//			1 for success
//			-1 for error plus message.
GLHLIB_API int glhGetGLDeviceCaps_ARB_vp_1_0(char *errorMessage, int adapterCount, int deviceType, GLCAPS_ARB_vp_1_0 *pcaps);


//PURPOSE:	In the same spirit as glhGetGLDeviceCaps_GL_1_5, this gets specific information
//			about the GL_ARB_fragment_program extension.
//			**** This will call wglGetProcAddress to get pointer to glGetProgramivARB ****
//
//PARAMETERS:
//			[out] errorMessage : If error occurs, a message is written here. Can be NULL.
//			                     Should be at least 512 bytes large.
//			[in] adapterCount : unused
//			[in] deviceType : unused
//			[in, out] pcaps : pointer to GLCAPS_ARB_fp_1_0
//			                  pcaps->Size must be sizeof(GLCAPS_ARB_fp_1_0)
//
//RETURN:
//			1 for success
//			-1 for error plus message.
GLHLIB_API int glhGetGLDeviceCaps_ARB_fp_1_0(char *errorMessage, int adapterCount, int deviceType, GLCAPS_ARB_fp_1_0 *pcaps);


//PURPOSE:	Make sure GL 2.0 and above is supported before calling this.
//
//PARAMETERS:
//			[out] errorMessage : If error occurs, a message is written here. Can be NULL.
//			                     Should be at least 512 bytes large.
//			[in] adapterCount : unused
//			[in] deviceType : unused
//			[in, out] pcaps : pointer to GLCAPS_GL_2_0_GLSL
//			                  pcaps->Size must be sizeof(GLCAPS_GL_2_0_GLSL)
//
//RETURN:
//			1 for success
//			-1 for error plus message.
GLHLIB_API int glhGetGLDeviceCaps_GL2_0_GLSL(char *errorMessage, int adapterCount, int deviceType, GLCAPS_GL_2_0_GLSL *pcaps);


//PURPOSE:	Check to see if an extension is present in glGetString(GL_EXTENSIONS)
//
//PARAMETERS:
//			extensionName : Something like GL_ARB_multitexture
//
//RETURN:
//			TRUE or FALSE
//			-1 if glGetString(GL_EXTENSIONS) returns NULL.
GLHLIB_API int glhIsGLExtensionSupported(const char *extensionName);

//PURPOSE:	Check to see if an extension is present in wglGetExtensionsStringARB(hdc) or wglGetExtensionsStringEXT(hdc)
//			Uses the ARB version if present, and failing that, uses EXT version, and failing that, it returns -1.
//
//PARAMETERS:
//			extensionName : Something like WGL_EXT_swap_control
//
//RETURN:
//			TRUE or FALSE
//			-1 if wglGetExtensionsStringARB(hdc) or wglGetExtensionsStringEXT(hdc) returns NULL.
//			-1 if neither function is present.
GLHLIB_API int glhIsWGLExtensionSupported(HDC hdc, const char *extensionName);




//PURPOSE:	Try to get the OpenGL error and copy it into the pstring.
//
//PARAMETERS:
//			[out] pstring : If this function returns 1, this would be something like "GL_OUT_OF_MEMORY"
//			                If -1, then this gets GL_NO_ERROR
//
//RETURN:
//			1 if pstring==NULL
//			1 if no errors found
//			-1 if GL error found and pstring received a string
//
//NOTE:		Even if there is an error, pstring may be strlen(pstring)==0 if
//			error was not identified.
//UPDATE:	Sunday, March 13, 2005 : Error code added for GL_EXT_framebuffer_object (GL_INVALID_FRAMEBUFFER_OPERATION_EXT)
GLHLIB_API int glhCopyOpenGLErrorInto_cBuffer(char *pstring);

//PURPOSE:	Try to get the OpenGL error and copy it into the pstring.
//
//PARAMETERS:
//			[out] pstring : If this function returns 1, this would be something like "GL_OUT_OF_MEMORY"
//			                If -1, then this gets GL_NO_ERROR
//			[out] glerror : If this function returns 1, this would receive the actual GL error ID
//
//RETURN:
//			1 if pstring==NULL
//			1 if no errors found
//			-1 if GL error found and pstring received a string
//
//NOTE:		Even if there is an error, pstring may be strlen(pstring)==0 if
//			error was not identified.
//UPDATE:	Sunday, March 13, 2005 : Error code added for GL_EXT_framebuffer_object (GL_INVALID_FRAMEBUFFER_OPERATION_EXT)
GLHLIB_API int glhCopyOpenGLErrorInto_cBuffer2(char *pstring, unsigned int &glerror);

/*Block 1 -end*/

/*Block 2 -start*/
#if USE_INLINE_ASM
//****************************************************************************
//TEXTURE SCALING OPERATIONS. DONE IN SOFTWARE.

//PURPOSE:	This function will not verify if values being passed are valid.
//			format must be RGB888 or RGBA8888 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8 are all accepted)
//			If isUseLinearOrPointSample=GL_TRUE then linear filtering is used else if GL_FALSE
//			point sampling is used.
//			dataAlignment must be 1 or 4.
//			Returns 1 if success. 0 for failure, -5 for not yet implemented.
//MESSAGE:	Only use glhScaleImage_asm386 in your important apps. The MMX version was a test
//			version and is slower (Intel didn't design MMX intelligently).
//			glhScaleImage2_asm386 is a test version and has bugs.
GLHLIB_API int glhScaleImage_asm386(unsigned int format, unsigned int isUseLinearOrPointSample,
		int dataAlignment,
		int widthin, int heightin,
		unsigned char *datain,
		int widthout, int heightout,
		unsigned char *dataout);

//PURPOSE:	Similar to glhScaleImage_asm386, except that we also scale in the z-direction (r-direction)
//			if needed. This function makes extensive use of glhScaleImage_asm386.
//
//			This function will not verify if values being passed are valid.
//			format must be RGB888 or RGBA8888 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8 are all accepted)
//			If isUseLinearOrPointSample=GL_TRUE then linear filtering is used else if GL_FALSE
//			point sampling is used.
//			dataAlignment must be 1 or 4.
//			Returns 1 if success. 0 for failure, -5 for not yet implemented.
GLHLIB_API int glhScaleImage3D_asm386(unsigned int format, unsigned int isUseLinearOrPointSample,
		int dataAlignment,
		int widthin, int heightin, int depthin,
		unsigned char *datain,
		int widthout, int heightout, int depthout,
		unsigned char *dataout);

//MESSAGE:	Only use glhScaleImage_asm386 in your important apps. The MMX version was a test
//			version and is slower (Intel didn't design MMX intelligently).
//			glhScaleImage2_asm386 is a test version and has bugs.
GLHLIB_API int glhScaleImage_asm386_MMX(unsigned int format, unsigned int isUseLinearOrPointSample,
		int dataAlignment,
		int widthin, int heightin,
		unsigned char *datain,
		int widthout, int heightout,
		unsigned char *dataout);

//PURPOSE:	This function will not verify if values being passed are valid.
//			format must be RGB888 or RGBA8888 (GL_RGB, GL_RGB8, GL_RGBA, GL_RGBA8 are all accepted)
//			If isUseLinearOrPointSample=GL_TRUE then linear filtering is used else if GL_FALSE
//			point sampling is used.
//			dataAlignment must be 1.
//			Returns 1 if success. 0 for failure, -5 for not yet implemented.
//MESSAGE:	Only use glhScaleImage_asm386 in your important apps. The MMX version was a test
//			version and is slower (Intel didn't design MMX intelligently).
//			glhScaleImage2_asm386 is a test version and has bugs.
GLHLIB_API int glhScaleImage2_asm386(unsigned int format, unsigned int isUseLinearOrPointSample,
		int dataAlignment,
		int widthin, int heightin,
		unsigned char *datain,
		int widthout, int heightout,
		unsigned char *dataout);
#endif

/*Block 2 -end*/

/*Block 3 -start*/
//****************************************************************************
//TEXTURE MIPMAP GENERATION AND UPLOAD T OPENGL
//Some functions that are similar to gluBuild2DMipmaps and extra things.

#if USE_INLINE_ASM
//PURPOSE:	This function will copy the pixels pointed to by pixels into the level=0 and
//			will use glhScaleImage_asm386() to scale the pixels and use them to update all the
//			other mipmaps. The application is responsable for knowing if mipmapping is used.
//			textureWidth and textureHeight are the width and height of the entire texture.
//			The other parameters are the same as glTexSubImage2D().
//			format must be GL_RGB or GLRGB8 or GL_RGBA or GL_RGBA8. type must be GL_UNSIGNED_BYTE.
//			Make sure glPixelStorei(GL_UNPACK_ALIGNMENT, 1 or 4); before calling this.
//			and dataAlignment must be 1 or 4
GLHLIB_API void glhTexSubImage2DMipmaps(int xoffset,
								int yoffset, int width, int height,
								int textureWidth, int textureHeight,
								unsigned int format,
								unsigned int type, const void *pixels);

//PURPOSE:	This function will build all the mipmaps using glhScaleImage_asm386() available
//			in this library.
//			components must be the same as for glTexImage2D.
//			dataAlignment must be 1 or 4, so be sure to call glPixelStorei(GL_UNPACK_ALIGNMENT, 1 or 4)
//			before using this function.
//			width and height must be positive, this function will not check for that.
//			If isPromoteToHigherPowerOn==GL_TRUE, then width and height will be bumped up to
//			next power of 2 number if not already power of 2, else if GL_FALSE they get bumped down
//			to a power of 2 number if not already power of 2.
//			format must be GL_RGB for RGB888 or GL_RGBA for RGBA8888 (or GL_BGR or GL_BGRA or GL_ABGR_EXT)
//			data must be of type unsigned char (unsigned char)
//			Returns 0 for memory error, 1 for success, -5 for not yet implemented, -1 for misuse
//WARNING:	The contents of data will be changed, but the pointer will remain the same.
//			**** That used to be true for version 1.3 and below, but no longer ****
GLHLIB_API int glhBuild2DMipmaps(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *data);

//PURPOSE:	This function will build all the mipmaps using glhScaleImage_asm386()
//			and will upload it to OpenGL, very much like gluBuild2DMipmaps for 2D textures.
//			This function does it for cubemaps
//
//PARAMETERS:
//			[in] components : must be the same as for glTexImage2D.
//			[in] dataAlignment : must be 1 or 4, so be sure to call glPixelStorei(GL_UNPACK_ALIGNMENT, 1 or 4)
//			                     before using this function.
//			[in] width : must be positive, this function will not check for that.
//			[in] height : must be positive, this function will not check for that.
//			[in] isPromoteToHigherPowerOn : if GL_TRUE, then width and height will be bumped up to
//			                                next power of 2 number if not already power of 2, else if GL_FALSE
//			                                they get bumped down to a power of 2 number if not already power of 2.
//			[in] format : must be GL_RGB for RGB888 or GL_RGBA for RGBA8888 (or GL_BGR or GL_BGRA or GL_ABGR_EXT)
//			[in] data : must be of type unsigned char (unsigned char). All the faces of the cubemap must be in sequence
//			            POSX, NEGX, POSY, NEGY, POSZ, NEGZ
//
//RETURN:
//			0 for memory error
//			1 for success
//			-5 for not yet implemented
//			-1 for misuse
GLHLIB_API int glhBuildCubeMapMipmaps(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *data);

//PURPOSE:	This function will build all the mipmaps using glhScaleImage_asm386()
//			in this library.
//			The rest of the parameters are the same as for glhBuildCubeMapMipmaps
//			except for depth, which is required for 3D textures.
//NOTE:		data points to an area of memory containing all the slices for the 3D texture.
//GLHLIB_API int glhBuild3DMipmaps(int components, int dataAlignment, int width, int height, int depth,
//				unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *data);
#endif

//PURPOSE:	For mipmaping normal maps.
//			The standard mipmapping algorithm is not suitable.
//			This will normalize each texel after doing the box filter.
//			It will upload each texture including the base texture to OpenGL
//			so be sure to call glBindTexture(GL_TEXTURE_2D, textureID);
//			before calling this.
//			Returns 0 for memory error, 1 for success, -1 for misuse.
//
//			components must be the same as for glTexImage2D.
//			dataAlignment must be 1 or 4, so be sure to call glPixelStorei(GL_UNPACK_ALIGNMENT, 1 or 4)
//			before using this function.
//			width and height must be power of 2
//			isPromoteToHigherPowerOn is ignored.
//			format must be GL_RGBA for RGBA8888 (or GL_BGRA or GL_ABGR_EXT)
//					we consider R as x, G as y, B as z, A as length of vector
//			data must be of type unsigned char (unsigned char)
//			The level 0 mipmap must be normalized (the RGB) and alpha must be 255 everywhere.
GLHLIB_API int glhBuild2DNormalMipmaps(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *data);
//PURPOSE:	Similar to glhBuild2DNormalMipmaps, except we have float instead of unsigned char, in
//			which case for format == GL_RGBA, it is considered as RGBA32-32-32-32.
GLHLIB_API int glhBuild2DNormalMipmapsf(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const float *data);


//PURPOSE:	Similar to glhBuild2DNormalMipmaps() but data points to 6 faces (similar requirements
//			to glhBuildCubeMapMipmaps)
//			The only difference is that you should call glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
GLHLIB_API int glhBuildCubeMapNormalMipmaps(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *data);
GLHLIB_API int glhBuildCubeMapNormalMipmapsf(int components, int dataAlignment, int width, int height,
				unsigned int isPromoteToHigherPowerOn, unsigned int format, const float *data);

//PURPOSE:	This will contruct a cubemap and will write the data into data.
//			After calling this, you might need to call glhBuildCubeMapNormalMipmaps
//			to upload the cubemap to OpenGL and make all the needed mipmaps as well.
//			dataAlignement must be 1 or 4.
//			width and height must be divisible by 2
//			format must be GL_RGBA for RGBA8888 (or GL_BGRA or GL_ABGR_EXT)
//			data must be allocated externally
//			Returns 1 for success, -1 for misuse.
GLHLIB_API int glhBuildNormalizationCubeMap(int dataAlignment, int width, int height, unsigned int format, unsigned char *data);
GLHLIB_API int glhBuildNormalizationCubeMapf(int dataAlignment, int width, int height, unsigned int format, float *data);


/*Block 3 -end*/

/*Block 4 -start*/
//****************************************************************************
//MISCELLANEOUS FUNCTIONS


//PURPOSE:	Compute the Fresnel term depending on incident vector and material index (relative material index)
//			If cosAngle < 0.0, then result receives 0.0 else the fresnel term will be computed.
//			Send an array of values and the output will be an array of values. count is the number of values
//			in cosAngle and the array size of result
//DEFINITION:
//			The Fresnel term is used to compensate for the fact that at high angle, the reflection becomes more
//			pronounced. At 90 degrees, reflection becomes 100% and no light enters the material.
//
//This approximates the fresnel term with the following equation :
//R0 = pow(1 - refractionIndex, 2) / pow(1+refractionIndex, 2)
//
//and
//return R0 + (1-R0) * pow(1-cosAngle, 5);
GLHLIB_API void glhFastFresnelTerm(float *result, float *cosAngle, float refractionIndex, int count);

/*Block 4 -end*/

/*Block 5 -start*/
//****************************************************************************
//PERLIN NOISE RELATED

//PURPOSE:	glh stores a table of random number dedicated to the perlin noise
//			generator. If you want new numbers in the table, call this.
GLHLIB_API void glhPerlinRegenerateTable();

//PURPOSE:	Generate a 1D noise function, based on persistence which
//			is defined as
//			frequency = 2^i
//			amplitude = persistence^i
//			in other words ...
//			ln(amplitude) = i*ln(persistence)
//			ln(amplitude)/ln(persistence) = i
//			frequency = 2^ln(amplitude)/ln(persistence)  <----
//
//			The numberOfOctaves which controls how many frequencies to sum up
//			boolOctave is an array the size of numberOfOctaves, with GL_TRUE and GL_FALSE
//			to control which ones to sum up.
//			interpolationQuality should be GLH_INTERPOLATIONQUALITY_LINEAR for low quality
//				GLH_INTERPOLATIONQUALITY_COS for high
//			mode can be GLH_MODE_STANDARD, GLH_MODE_ALTERNATE, GLH_MODE_ALTERNATE2, GLH_MODE_ALTERNATE3, GLH_MODE_ALTERNATE4
//
//			optimize can be GLH_OPTIMIZE_NONE
//
//			posx is 1 float that describes a position in 1D
//
//			The result is a single float and is written to *result.
//
//			Returns 1 for sucess, -1 if bad parameter passed		
GLHLIB_API int glhPerlinNoise1D(float *result, int interpolationQuality, int mode, int optimize, float posx, float persistence, int numberOfOctaves, int *boolOctave);

//PURPOSE:	Generate a 2D noise function, based on persistence which
//			is defined as
//			frequency = 2^i
//			amplitude = persistence^i
//			in other words ...
//			ln(amplitude) = i*ln(persistence)
//			ln(amplitude)/ln(persistence) = i
//			frequency = 2^ln(amplitude)/ln(persistence)  <----
//
//			The numberOfOctaves which controls how many frequencies to sum up
//			boolOctave is an array the size of numberOfOctaves, with GL_TRUE and GL_FALSE
//			to control which ones to sum up.
//			interpolationQuality should be GLH_INTERPOLATIONQUALITY_LINEAR for low quality
//				GLH_INTERPOLATIONQUALITY_COS for high
//			mode can be GLH_MODE_STANDARD, GLH_MODE_ALTERNATE, GLH_MODE_ALTERNATE2, GLH_MODE_ALTERNATE3, GLH_MODE_ALTERNATE4
//			optimize can be GLH_OPTIMIZE_NONE or GLH_OPTIMIZE_MMX_SSE
//
//			pos is 2 floats that describe a position in 2D when optimize == GLH_OPTIMIZE_NONE
//			when optimize == GLH_OPTIMIZE_MMX_SSE, pos is 2*4 floats (xy, xy, xy, xy)
//
//			The result is a single float and is written to *result when optimize == GLH_OPTIMIZE_NONE
//			when optimize == GLH_OPTIMIZE_MMX_SSE, result is 4 floats.
//
//			Returns 1 for sucess, -1 if bad parameter passed
//NOTE:		The only part that uses MMX and SSE when optimize == GLH_OPTIMIZE_MMX_SSE
//			is (interpolationQuality == GLH_INTERPOLATIONQUALITY_LINEAR) && (mode == GLH_MODE_STANDARD)
//			Writing in assembly is too time consuming.
//NOTE:		When optimize == GLH_OPTIMIZE_MMX_SSE, alignment is not necessary.
GLHLIB_API int glhPerlinNoise2D(float *result, int interpolationQuality, int mode, int optimize, float *pos, float persistence, int numberOfOctaves, int *boolOctave);

//PURPOSE:	Generate a 3D noise function, based on persistence which
//			is defined as
//			frequency = 2^i
//			amplitude = persistence^i
//			in other words ...
//			ln(amplitude) = i*ln(persistence)
//			ln(amplitude)/ln(persistence) = i
//			frequency = 2^ln(amplitude)/ln(persistence)  <----
//
//			The numberOfOctaves which controls how many frequencies to sum up
//			boolOctave is an array the size of numberOfOctaves, with GL_TRUE and GL_FALSE
//			to control which ones to sum up.
//			interpolationQuality should be GLH_INTERPOLATIONQUALITY_LINEAR for low quality
//				GLH_INTERPOLATIONQUALITY_COS for high
//			mode can be GLH_MODE_STANDARD, GLH_MODE_ALTERNATE, GLH_MODE_ALTERNATE2, GLH_MODE_ALTERNATE3, GLH_MODE_ALTERNATE4
//
//			optimize can be GLH_OPTIMIZE_NONE
//
//			pos is 3 floats that describe a position in 3D
//
//			The result is a single float and is written to *result.
//
//			Returns 1 for sucess, -1 if bad parameter passed
GLHLIB_API int glhPerlinNoise3D(float *result, int interpolationQuality, int mode, int optimize, float *pos, float persistence, int numberOfOctaves, int *boolOctave);

/*Block 5 -end*/

/*Block 6 -start*/
/*Block 6 -end*/

/*Block 7 -start*/
/*Block 7 -end*/

/*Block 8 -start*/

//PURPOSE:	Initialize OpenGL 1.1 hints.
GLHLIB_API void glhInitializeHints(unsigned int mode);

/*Block 8 -end*/

/*Block 9 -start*/
//****************************************************************************
//SOFTWARE TRANSFORM OPERATIONS. DOES NOT USE OPENGL.
//Multiply matrix by vertex. Multiply vertex by matrix. Multiply matrix by matrix.

//PURPOSE:	Returns 1 fo success, 0 for error (division by w=0.0)
//			windowCoordinate is (x, y, z)
GLHLIB_API int glhProjectd(double objx, double objy, double objz,
				double *modelview, double *projection,
				int *viewport,
				double *windowCoordinate);
GLHLIB_API int glhProjectf(float objx, float objy, float objz,
				float *modelview, float *projection,
				int *viewport,
				float *windowCoordinate);

//PURPOSE:	Singular matrix or division by w=0.0 return 0, else 1 for success.
//			objectCoordinates is (x, y, z)
GLHLIB_API int glhUnProjectd(double winx, double winy, double winz,
				double *modelview, double *projection,
				int *viewport,
				double *objectCoordinate);
GLHLIB_API int glhUnProjectf(float winx, float winy, float winz,
				float *modelview, float *projection,
				int *viewport,
				float *objectCoordinate);

//PURPOSE:	Returns 0 fo success, >0 for number of errors (division by w=0.0)
//			Example: If you have 5 vertices to process, then totalVertex4D=4*5
//			NOTE: the w of windowCoordinate4D won't be written to.
//			NOTE: we assume w component of objectVertex4D contains any value.
GLHLIB_API int glhProjectf_2(int totalVertex4D, float *objectVertex4D,
				float *modelview_projection,
				int *viewport,
				float *windowCoordinate4D);
//			Example: If you have 5 vertices to process, then totalVertex4D=3*5
GLHLIB_API int glhProjectf_3(int totalVertex3D, float *objectVertex3D,
				float *modelview_projection,
				int *viewport,
				float *windowCoordinate3D);

//PURPOSE:	Returns 0 for success, >0 for singular matrix or division by w=0.0
//			NOTE: objectVertex4D w component will contain 1.0
//			NOTE: we ignore w component of windowCoordinate4D
GLHLIB_API int glhUnProjectf_2(int totalVertex4D, float *windowCoordinate4D,
				float *inverse_modelview_projection,
				int *viewport,
				float *objectVertex4D);
GLHLIB_API int glhUnProjectf_3(int totalVertex3D, float *windowCoordinate3D,
				float *inverse_modelview_projection,
				int *viewport,
				float *objectVertex3D);

#if USE_INLINE_ASM
//PURPOSE:	Returns 0 for success, >0 for number of errors (division by w=0.0)
//			Memory access must be 16 byte aligned (128 bit) so objectVertex,
//			modelview_projection, windowCoordinate should
//			be a valid address, EXCEPT viewport, which doesn't matter.
//			Vertices are (x, y, z, w) windowCoordinate are (x, y, z, w)
//			For windowCoordinate, the result is x and y are window coordinates,
//			z is window space z
//			NOTE: we ignore w component of windowCoordinate4D
//			Example: If you have 5 vertices to process, then totalVertex4D=4*5
GLHLIB_API int glhProjectf_SSE_Aligned_2(int totalVertex4D, float *objectVertex4D,
				 float *modelview_projection,
				 int *viewport,
				 float *windowCoordinate4D);
//			The following might give a boost in performance. Pentium 3, Pentium 4, Athlon XP
GLHLIB_API int glhProjectf_SSE_Aligned_WarmCache_2(int totalVertex4D, float *objectVertex4D,
				 float *modelview_projection,
				 int *viewport,
				 float *windowCoordinate4D);
//			Same as above, but we don't care about alignment (lower performance)
GLHLIB_API int glhProjectf_SSE_Unaligned_2(int totalVertex4D, float *objectVertex4D,
				 float *modelview_projection,
				 int *viewport,
				 float *windowCoordinate4D);
//PURPOSE:	Returns 0 for success, >0 for singular matrix or division by w=0.0
//			Look at comment for glhProjectf_SSE_Aligned_2
GLHLIB_API int glhUnProjectf_SSE_Aligned_2(int totalVertex4D, float *windowCoordinate4D,
				float *inverse_modelview_projection,
				int *viewport,
				float *objectVertex4D);
//			The following might give a boost in performance. Pentium 3, Pentium 4, Athlon XP
GLHLIB_API int glhUnProjectf_SSE_Aligned_WarmCache_2(int totalVertex4D, float *windowCoordinate4D,
				float *inverse_modelview_projection,
				int *viewport,
				float *objectVertex4D);
//			Same as above, but we don't care about alignment (lower performance)
GLHLIB_API int glhUnProjectf_SSE_Unaligned_2(int totalVertex4D, float *windowCoordinate4D,
				float *inverse_modelview_projection,
				int *viewport,
				float *objectVertex4D);
#endif

//PURPOSE:	Do a 4x4 matrix multiplication on vector
//			NOTE: Assuming w of vector4D is any value.
//			Example: if we have 5 vertices, totalVertex4D=4*5
//			Returns always 0.
GLHLIB_API int glhMultiplyMatrixByVector4by4f_1(int totalVector4D, float *vector4D,
				float *matrix,
				float *result4D);
//			This similarly does a matrix multiplication and ignores w of result3D.
//			w of vector3D is assumed to be 1.0
GLHLIB_API int glhMultiplyMatrixByVector4by4f_2(int totalVector3D, float *vector3D,
				float *matrix,
				float *result3D);
//			This similarly does a matrix multiplication and ignores w of result3D.
//			w of vector3D is assumed to be 0.0.
//			This is suitable for transforming normals.
GLHLIB_API int glhMultiplyMatrixByVector4by4f_3(int totalVector3D, float *vector3D,
				float *matrix,
				float *result3D);

//PURPOSE:	Do a vector multiplication on a 4x4 matrix (UNLIKE glhMultiplyMatrixByVector4by4f_1
//			which is matrix * vector
//
//			NOTE: Assuming w of vector4D is any value.
//			Example: if we have 5 vertices, totalVertex4D=4*5
//			Returns always 0.
GLHLIB_API int glhMultiplyVectorByMatrix4by4f_1(int totalVector4D, float *vector4D,
				float *matrix,
				float *result4D);
//			This similarly does a matrix multiplication and ignores w of result3D.
//			w of vector3D is assumed to be 1.0
GLHLIB_API int glhMultiplyVectorByMatrix4by4f_2(int totalVector3D, float *vector3D,
				float *matrix,
				float *result3D);
//			This similarly does a matrix multiplication and ignores w of result3D.
//			w of vector3D is assumed to be 0.0.
//			This is suitable for transforming normals.
GLHLIB_API int glhMultiplyVectorByMatrix4by4f_3(int totalVector3D, float *vector3D,
				float *matrix,
				float *result3D);


#if USE_INLINE_ASM
//PURPOSE:	Do a 4x4 matrix multiplication on vector using SSE
//			All data must be 16 byte aligned.
//			It avoids cache pollution and reduces unecessary bus traffic.
//			NOTE: Assuming w of vector4D is any value.
//			Example: if we have 5 vertices, totalVertex4D=4*5
//			Returns always 0.
GLHLIB_API int glhMultiplyMatrixByVector4by4f_SSE_Aligned_1(int totalVector4D, float *vector4D,
				float *matrix,
				float *result4D);
//			The following might give a boost in performance. Pentium 3, Pentium 4, Athlon XP
//			This version warms the cache to reduce latency.
//			It avoids cache pollution and reduces unecessary bus traffic.
GLHLIB_API int glhMultiplyMatrixByVector4by4f_SSE_Aligned_WarmCache_1(int totalVector4D, float *vector4D,
				float *matrix,
				float *result4D);
#endif

//PURPOSE:	Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this only once. See glhMultiplyMatrixByMatrix4by4f_2.
GLHLIB_API void glhMultiplyMatrixByMatrix4by4f_1(float *leftMatrix, float *rightMatrix, float *result);
//PURPOSE:	Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this countMatrix number of times.
//			For example, for 2 matrix mult, countMatrix must be 2.
GLHLIB_API void glhMultiplyMatrixByMatrix4by4f_2(int countMatrix, float *leftMatrix, float *rightMatrix, float *result);

#if USE_INLINE_ASM
//PURPOSE:	**** SSE version of glhMultiplyMatrixByMatrix4by4f_1
//			Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this only once. See glhMultiplyMatrixByMatrix4by4f_2.
GLHLIB_API void glhMultiplyMatrixByMatrix4by4f_SSE_Aligned_WarmCache_1(float *leftMatrix, float *rightMatrix, float *result);

//PURPOSE:	**** 3DNow! version of glhMultiplyMatrixByMatrix4by4f_1
//			Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this only once. See glhMultiplyMatrixByMatrix4by4f_2.
GLHLIB_API void glhMultiplyMatrixByMatrix4by4f_3DNow(float *leftMatrix, float *rightMatrix, float *result);

//PURPOSE:	**** 3DNow! version of glhMultiplyMatrixByMatrix4by4f_1
//			Multiply leftMatrix[16] with rightMatrix[16] and write result to result[16]
//			It does this only once. See glhMultiplyMatrixByMatrix4by4f_2.
GLHLIB_API void glhMultiplyMatrixByMatrix4by4f_3DNow_1(float *leftMatrix, float *rightMatrix, float *result);
#endif

/*Block 9 -end*/

/*Block 10 -start*/
//**************************************************************************************************
//MISCELLANEOUS MATRIX OPERATIONS. DOES NOT USE OPENGL.
//ANGLES ARE IN RADIANS WITH SOME EXCEPTIONS.

//PURPOSE:	After calling
//			float matrix[16], eyePosition3D[3], center3D[3], upVector3D[3];
//			glhLoadIdentityf2(matrix);
//			glhLookAtf2(matrix, eyePosition3D, center3D, upVector3D);
//
//			you might want to retreive eyePosition3D, center3D and upVector3D directly from the matrix.
//			This function will attempt to do so.
//			**** WARNING : center3D will not be the original value you input because this information is lossed when
//			               you call glhLookAtf2
//			               center3D will be eyePosition3D + normalize(lookAtVector)
//
//			Hopefully, the matrix contains valid data else extracting will be a problem.
//			Even if you call glhRotatef2, glhTranslatef2 and other functions, this function will be able to extract
//			the new glhLookAt2 variables.
//
//PARAMETERS:
//			[in] matrix : Column major 4x4 matrix
//			[out] eyePosition3D : Position of the "eye" aka camera position
//			[out] center3D : The point at which we are looking at
//			[out] upVector3D : A normalized up vector
//
//RETURN:
//			1 for success (Most likely)
//			-1 for error. (Most likely the matrix contains some unexpected entries in the upper left 3x3 matrix.)
GLHLIB_API int glhExtractLookAtFromMatrixd2(double *matrix, double *eyePosition3D, double *center3D, double *upVector3D);
GLHLIB_API int glhExtractLookAtFromMatrixf2(float *matrix, float *eyePosition3D, float *center3D, float *upVector3D);

//PURPOSE:	Give a 4x4 matrix in OpenGL format, returns TRUE or FALSE
//			For double, the result must be accurate to 1.0e-12
//			For float, the result must be accurate to 1.0e-6
GLHLIB_API int glhIsMatrixRotationMatrixd(double *matrix);
GLHLIB_API int glhIsMatrixRotationMatrixf(float *matrix);
GLHLIB_API void glhExtractAnglesFromRotationMatrixd2(double *matrix, double *xAngleRadians, double *yAngleRadians, double *zAngleRadians);
GLHLIB_API void glhExtractAnglesFromRotationMatrixf2(float *matrix, float *xAngleRadians, float *yAngleRadians, float *zAngleRadians);

/*Block 10 -end*/

/*Block 11 -start*/

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by undercompensating.
//			if((width==*widthOpenGL)&&(height==*heightOpenGL)) return 1 else return 0.
GLHLIB_API int glhLowerPowerOfTwo(int width, int *widthOpenGL, int height, int *heightOpenGL);

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by overcompensating.
//			if((width==*widthOpenGL)&&(height==*heightOpenGL)) return 1 else return 0.
GLHLIB_API int glhHigherPowerOfTwo(int width, int *widthOpenGL, int height, int *heightOpenGL);

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by undercompensating.
//			if(value==*newValue) return 1 else return 0
GLHLIB_API int glhLowerPowerOfTwo2(int value, int *newValue);

//PURPOSE:	Used in OpenGL, for making image dimension power of 2 by overcompensating.
//			if(value==*newValue) return 1 else return 0
GLHLIB_API int glhHigherPowerOfTwo2(int value, int *newValue);

/*Block 11 -end*/

/*Block 12 -start*/
//**************************************************************************************************
//**************************************************************************************************
//SOFTWARE MATRIX OPERATIONS. DOES NOT USE OPENGL.
//ANGLES ARE IN RADIANS WITH SOME EXCEPTIONS.
//ALL OPENGL and GLU EQUIVALENT FUNCTIONS ARE HERE EXCEPT FOR glPushMatrix and glPopMatrix since
//**** glhlib doesn't emulate a matrix stack
//
//Certain functions have minor optimization such as glhRotateAboutXf2
//
//Certain functions are special such as glhMergedFrustumf2, glhFrustumInfiniteFarPlanef2,
//				glhShadowMatrixf2, glhInvertMatrixf2, glhQuickInvertMatrixf2, glhTransposeMatrixf2.
//
//Certain functions use SSE such as glhTranslatef2_SSE_Aligned



//PURPOSE:	For software transformation, so that a call to glGetFloatv() or glGetDoublev()
//			can be avoided. SSE and 3D Now! versions may be produced later on.
GLHLIB_API void glhLoadIdentityd2(double *matrix);
GLHLIB_API void glhLoadIdentityf2(float *matrix);
GLHLIB_API void glhTranslated2(double *matrix, double x, double y, double z);
GLHLIB_API void glhTranslatef2(float *matrix, float x, float y, float z);
GLHLIB_API void glhScaled2(double *matrix, double x, double y, double z);
GLHLIB_API void glhScalef2(float *matrix, float x, float y, float z);
GLHLIB_API void glhRotated2(double *matrix, double angleInRadians, double x, double y, double z);
GLHLIB_API void glhRotatef2(float *matrix, float angleInRadians, float x, float y, float z);
GLHLIB_API void glhRotateAboutXd2(double *matrix, double angleInRadians);
GLHLIB_API void glhRotateAboutXf2(float *matrix, float angleInRadians);
GLHLIB_API void glhRotateAboutYd2(double *matrix, double angleInRadians);
GLHLIB_API void glhRotateAboutYf2(float *matrix, float angleInRadians);
GLHLIB_API void glhRotateAboutZd2(double *matrix, double angleInRadians);
GLHLIB_API void glhRotateAboutZf2(float *matrix, float angleInRadians);
GLHLIB_API void glhPerspectived2(double *matrix, double fovyInDegrees, double aspectRatio, double znear, double zfar);
GLHLIB_API void glhPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);


//glhD3DPerspectiveLHd2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterLH
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DPerspectiveLHd2(double *matrix, double fovyInDegrees, double aspectRatio, double znear, double zfar);

//glhD3DPerspectiveLHf2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterLH
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DPerspectiveLHf2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);

//glhD3DPerspectiveRHd2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterRH
//The RH means Right Handed.
GLHLIB_API void glhD3DPerspectiveRHd2(double *matrix, double fovyInDegrees, double aspectRatio, double znear, double zfar);

//glhD3DPerspectiveRHf2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterRH
//The RH means Right Handed.
GLHLIB_API void glhD3DPerspectiveRHf2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);


GLHLIB_API void glhFrustumd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);
GLHLIB_API void glhFrustumf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);


//glhD3DFrustumLHd2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterLH
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DFrustumLHd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);

//glhD3DFrustumLHf2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterLH
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DFrustumLHf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

//glhD3DFrustumRHd2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterRH
//The RH means Right Handed.
GLHLIB_API void glhD3DFrustumRHd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);

//glhD3DFrustumRHf2 creates a matrix identical to D3DXMatrixPerspectiveOffCenterRH
//The RH means Right Handed.
GLHLIB_API void glhD3DFrustumRHf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);


GLHLIB_API void glhOrthod2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);
GLHLIB_API void glhOrthof2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);


//glhD3DOrthoLHd2 creates a matrix identical to D3DXMatrixOrthoOffCenterLH.
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DOrthoLHd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);

//glhD3DOrthoLHf2 creates a matrix identical to D3DXMatrixOrthoOffCenterLH.
//The LH means Left Handed.
//I recommend to stick with RH.
GLHLIB_API void glhD3DOrthoLHf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);

//glhD3DOrthoRHd2 creates a matrix identical to D3DXMatrixOrthoOffCenterRH.
//The RH means Right Handed.
GLHLIB_API void glhD3DOrthoRHd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);

//glhD3DOrthoRHf2 creates a matrix identical to D3DXMatrixOrthoOffCenterRH.
//The RH means Right Handed.
GLHLIB_API void glhD3DOrthoRHf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);


GLHLIB_API void glhMergedFrustumd2(double *matrix, double left, double right, double bottom, double top, double znear, double zfar);
GLHLIB_API void glhMergedFrustumf2(float *matrix, float left, float right, float bottom, float top, float znear, float zfar);
GLHLIB_API void glhMergedPerspectived2(double *matrix, double fovyInDegrees, double aspectRatio, double znear, double zfar);
GLHLIB_API void glhMergedPerspectivef2(float *matrix, float fovyInDegrees, float aspectRatio, float znear, float zfar);
GLHLIB_API void glhFrustumInfiniteFarPlaned2(double *matrix, double left, double right, double bottom, double top, double znear);
GLHLIB_API void glhFrustumInfiniteFarPlanef2(float *matrix, float left, float right, float bottom, float top, float znear);
GLHLIB_API void glhPerspectiveInfiniteFarPlaned2(double *matrix, double fovyInDegrees, double aspecRatio, double znear);
GLHLIB_API void glhPerspectiveInfiniteFarPlanef2(float *matrix, float fovyInDegrees, float aspectRatio, float znear);
GLHLIB_API void glhLookAtd2(double *matrix, double *eyePosition3D, double *center3D, double *upVector3D);
GLHLIB_API void glhLookAtf2(float *matrix, float *eyePosition3D, float *center3D, float *upVector3D);
GLHLIB_API void glhPickMatrixd2(double *matrix, double x, double y, double deltax, double deltay, int *viewport);
GLHLIB_API void glhPickMatrixf2(float *matrix, float x, float y, float deltax, float deltay, int *viewport);
GLHLIB_API void glhMultMatrixd2(double *result_leftMatrix, double *rightMatrix);
GLHLIB_API void glhMultMatrixf2(float *result_leftMatrix, float *rightMatrix);
GLHLIB_API void glhMultMatrixSwappedd2(double *result_rightMatrix, double *leftMatrix);
GLHLIB_API void glhMultMatrixSwappedf2(float *result_rightMatrix, float *leftMatrix);

//SSE versions of the above. Does the matrix multiplication with SSE
//Column major matrix goes wells with SSE.
//Cache warming is used where appropriate for more performance.
//**** A call to glhInitLibrary() is required. Don't forget to call glhShutDownLibrary() when shutting down.
//**** Because need to allocate some RAM and then deallocate
//xyzw in which w = 1.0
//Make sure all pointer to array is aligned 16 byte else you will get crash.
//Will not implement unaligned version.
GLHLIB_API void glhTranslatef2_SSE_Aligned(float *matrix, float *xyzw);
GLHLIB_API void glhScalef2_SSE_Aligned(float *matrix, float *xyzw);
GLHLIB_API void glhRotatef2_SSE_Aligned(float *matrix, float angleInRadians, float *xyzw);
//These don't give any benifits
//GLHLIB_API void glhRotateAboutXf2_SSE_Aligned(float *matrix, float angleInRadians);
//GLHLIB_API void glhRotateAboutYf2_SSE_Aligned(float *matrix, float angleInRadians);
//GLHLIB_API void glhRotateAboutZf2_SSE_Aligned(float *matrix, float angleInRadians);

//Most of the other ones aren't called so often so why bother optimizing. (Ex : glhPerspectivef2)

//Likely to get called often
GLHLIB_API void glhMultMatrixf2_SSE_Aligned(float *result_leftMatrix, float *rightMatrix);
GLHLIB_API void glhMultMatrixSwappedf2_SSE_Aligned(float *result_rightMatrix, float *leftMatrix);


//PURPOSE:	A fast way to get the inverse, compared to glhInvertMatrixdv_2 and glhInvertMatrixfv_2
//			matrix should not have any scale factor and matrix should be an orthogonal
//			matrix, and typically these are true for 3D graphics.
GLHLIB_API void glhQuickInvertMatrixd2(double *m, double *result);
GLHLIB_API void glhQuickInvertMatrixf2(float *m, float *result);

GLHLIB_API void glhTransposeMatrixd2(double *result, double *m);
GLHLIB_API void glhTransposeMatrixf2(float *result, float *m);

//PURPOSE:	Singular matrix return 0, else 1 for success.
GLHLIB_API int glhInvertMatrixd2(double *m, double *result);
GLHLIB_API int glhInvertMatrixf2(float *m, float *result);

//PURPOSE:	Constructs a shadow matrix. When this matrix is applied to the current GL
//			matrix (probably modelview), all vertices are mapped to a plane with equation
//			A*x + B*y + C*z + D = 0
//			All that is needed is the plane equation for the ground or what else have you,
//			and the light position.
//			planeEquation is the A B C D of the plane.
//			lightPosition is xyzw.
//HOW TO USE:	Apply this matrix after the camera matrix and before the world matrix.
//				The world matrix is for transforming the object to the "world". GL does not
//				have the notion of camera and world matrices but that's another issue.
//
//				glLoadMatrixf(CameraMatrix);
//				glhShadowMatrixf(planeEquation, lightPosition);
//				glMultMatrixf(WorldMatrix);
//
//				You may want to color the shadows as some dark color and perhaps use the stencil
//				to avoid double (or more) shadowing.
GLHLIB_API void glhShadowMatrixd2(double *matrix, double *planeEquation, double *lightPosition);
GLHLIB_API void glhShadowMatrixf2(float *matrix, float *planeEquation, float *lightPosition);


//PURPOSE:	In some software, some elements in the 3D scene (with perspective view of course)
//			certain object preserve their scale no matter from where you look at them.
//			glhConstantSizeMatrix will build a modelview matrix that does just that.
//			Why doesn't this function apply the matrix transform onto the GL matrix stack?
//			Keep in mind that this is a "2" function. It doesn't use or touch GL at all.
//			Also, see glhConstantSizeMatrixf, glhConstantSizeMatrixd
//HOW TO USE:	float newModelview[16];		//Column major matrix, 4x4
//				float objectCenter[4];		//Compute this yourself (XYZW where W = 1.0)
//				float modelview_projection[16];	//This is P * M
//				float modelview[16];			//The previous matrix on the stack.

//				glPushMatrix();		//Push old modelview matrix to preserve it
//				glhConstantSizeMatrixf2(newModelview, objectCenter, modelview_projection, modelview, viewport);
//				glLoadMatrixf(newModelview);
//				DrawMyObject();
//				glPopMatrix();
GLHLIB_API void glhConstantSizeMatrixf2(float *newModelview, float *objectCenter, float *modelview_projection, float *modelview, int *viewport);

//PURPOSE:	For creating a matrix for generalized planar reflection.
//			Typically, people reflect about the xz-plane or xy-plane or yz-plane using glScale(1.0, -1.0, 1.0)
//			or glScale(1.0, 1.0, -1.0) or glScale(-1.0, 1.0, 1.0) and then setup the stencil bits and render
//			everything with polygon orientation reversed (glFrontFace).
//			With this function, you can set an arbitrary reflection plane. Load into into the modelview matrix
//			and reverse polygon orientation, then render everything.
//			Be sure to reflect lights as well (glLightf).
//
//PARAMETERS:
//			[out] matrix : is an array of 16 floats (GL column matrix) and should contain identity matrix
//			               or previous transform. The resulting matrix will be stored here.
//			[in] ptransformArray : is an array of floats that contains (centerx of mirror, centery of mirror, centerz of
//			                       mirror, self-rotation about x, self-rotation about y, self-rotation about z).
//			                       Self rotation means rotating an object about its center point.
//			                       The rotation is for the mirror.
//			                       ***** The rotations must be in RADIANS as usual*****
//
//HOW TO USE:
//We assume the mirror is positioned in the world like this
//If your transform sequence is different than this, it won't work for you :
//glTranslatef(mirrorX, mirrorY, mirrorZ);
//glRotatef(mirror_Z_rotation, 0.0, 0.0, 1.0);
//glRotatef(mirror_Y_rotation, 0.0, 1.0, 0.0);
//glRotatef(mirror_X_rotation, 1.0, 0.0, 0.0);
//
//where mirror vertices are (positioned on the xy plane at z=0.0)
//{-1.0, -1.0, 0.0} {1.0, -1.0, 0.0} {-1.0, 1.0, 0.0} {1.0, 1.0, 0.0}
//
//When you call glhGeneralPlanarMirrorf_1 to generate the matrix, before you render the geometry of your stuff to be mirrored,
//call glMultMatrixf to upload (and multiply) to GL.
GLHLIB_API void glhGeneralPlanarMirrorf_1(float *matrix, float *ptransformArray);

//PURPOSE:	Same as glhGeneralPlanarMirrorf_1, but ptransformArray is split into ptranslate[3] and pselfRotate[3]
GLHLIB_API void glhGeneralPlanarMirrorf_2(float *matrix, float *ptranslate, float *pselfRotate);


//PURPOSE:	Assume you have an object that has already a rotation matrix applied to it. Call
//			the rotation R1. Let's say you want to apply another rotation to the object. Call it R2
//			but you no longer want to apply R1. The solution is to find the inverse of R1 and then
//			apply R2.
//			This functions will use glhQuickInvertMatrixf2 and then glhMultMatrixf2
GLHLIB_API void glhRotationMatrixFromRotationsf2(float *result_Matrix, float *R1, float *R2);

/*Block 12 -end*/

/*Block 13 -start*/
/*Block 13 -end*/

/*Block 14 -start*/
//**************************************************************************************************
//GLH ERROR DETECTION

//PURPOSE:	Get error code as int. See Block 0 for the #defines.
//			Once all errors are picked up by calling glhGetError, further calls will return GLH_ERROR_NONE.
//			The error stack is 100 elements deep.
//			Be sure to call glhInitLibrary and glhShutDownLibrary.
GLHLIB_API int glhGetError();

//PURPOSE:	Get the glh error and copy it into the pstring.
//			Returns 1 if pstring==NULL
//			Returns 1 if no errors found, else it returns -1.
GLHLIB_API int glhCopyErrorInto_cBuffer(char *pstring);
/*Block 14 -end*/

/*Block 15 -start*/
//**************************************************************************************************
//SOME NORMAL MAP AND HEIGHT MAP PROCESSING FUNCTIONS

//PURPOSE:	Using a height map, create a normal map.
//			The height map can be grayscale (8 bit), RGB8 (24 bit) or RGBA (32 bit)
//
//PARAMETERS:
//			[in] dataAlignment : must be 1 or 4.
//			[in] width and height : must be 2^n + 2*(border) where border is 0 or 1.
//			[in] border : 0 or 1
//			[in] scaleFactor : You should choose a good scale factor (4.0)
//			[in] isPromoteToHigherPowerOn : is ignored for now
//			[in] format : is for indata.
//			              It can be GL_LUMINANCE
//			              It can be GL_RGB (we will use red for height)
//			              It can be GL_BGR (we will use blue for height)
//			              It can be GL_RGBA (we will use red for height)
//			              It can be GL_BGRA (we will use blue for height)
//			              It can be GL_ABGR (we will use alpha for height)
//			[in] indata : Pointer to array of pixels
//			[in] outformat : must be GL_RGBA, GL_BGRA or GL_ABGR (always 32 bpp!)
//			[out] outdata : must be previously allocated, of width x height x 4 with 0 border.
//			[in] alphaHeight : If FALSE, then alpha will always be 255.
//			                   If TRUE, then alpha will be set to a "height" value.
//			[in] heightSource : Used when alphaHeight=TRUE.
//			                    If 0, then copy RED of indata to ALPHA of outdata
//			                    If 1, then copy GREEN of indata to ALPHA of outdata
//			                    If 2, then copy BLUE of indata to ALPHA of outdata
//			                    If GL_LUMINANCE is the format of indata, then it doesn't matter if you set 0, 1 or 2
//
//RETURN:
//			1 for success
//			-1 on failure
//
//UPDATE:	Updated to work with glhGetError. If int returns error, call glhGetError.
GLHLIB_API int glhConvertHeightMapToNormalMap(int dataAlignment, int width, int height, int border, float scaleFactor,
								unsigned int isPromoteToHigherPowerOn, unsigned int format, const unsigned char *indata, unsigned int outformat, unsigned char *outdata, unsigned int alphaHeight, int heightSource);


//PURPOSE:	Given a normal map, LERP normal with its neightbor :
//			new_normal = normal = (1-weightFactor) * neighbor_normal + weightFactor * normal
//
//			The effect is that it reduces the difference between neighboring normals.
//			If a rightmost pixel is reached, it's right neighbor is in column 0.
//			If a leftmost pixel is reached, it's left neightbor is the last column.
//			If a topmost pixel is reached, it's neighbor is on the bottom row of pixels.
//			If a bottommost pixel is reached, it's neighbor is on the top row of pixels.
//			Basically, this is a wraparound operation.
//
//PARAMETERS:
//			[in] dataAlignment : must be 1 or 4.
//			[in] width and height : must be 2^n + 2*(border) where border is 0 or 1.
//			[in] border : 0 or 1
//			[in] weightFactor : You should choose a value from 0.0 to 1.0. The code will not check if this is out of range.
//			[in] format : is for indata and outdata
//			              It can be GL_RGBA (we will use red for height)
//			              It can be GL_BGRA (we will use blue for height)
//			              It can be GL_ABGR (we will use alpha for height)
//			[in] indata : Pointer to array of pixels
//			[out] outdata : must be previously allocated, of width x height x 4 with 0 border.
//			[in] param : Use GLH_UP, GLH_DOWN, GLH_LEFT, GLH_RIGHT. You can OR them together.
//			             Example, if you specify GLH_UP, the neighbor pixel would be the pixel right above the current pixel.
//			             The only valid value is GLH_UP | GLH_DOWN | GLH_LEFT | GLH_RIGHT
//
//RETURN:
//			1 for success
//			-1 on failure
//
//UPDATE:	Updated to work with glhGetError. If int returns error, call glhGetError.
GLHLIB_API int glhNormalMapReducer(int dataAlignment, int width, int height, int border, float weightFactor, unsigned int format, const unsigned char *indata, unsigned char *outdata, int param);

/*Block 15 -end*/


/*Block 16 -start*/
//**************************************************************************************************
//NORMAL VECTOR, TANGENT VECTOR, BINORMAL VECTOR

//PURPOSE:	Compute tangent and binormal vectors.
//			index, vertex, normal and tex coords must be supplied and
//			memory for tangent and binormal vectors should be allocated externally.
//			**** There are 2 functions : One for unsigned int indices (32 bit), one for unsigned short indices (16 bit)
//
//PARAMETERS:
//			[in] totalIndices : The number of indices in pindices
//			[in] pindices : Array containing the indices
//			[in] totalVertex : The number of vertices
//			[in] numCompVertex : is the number of components per vertex. 3 or 4.
//			[in] pvertex : Array containing the vertices
//			[in] numCompNormals : is the number of components per normal. 3 or 4.
//			[in] pnormals : Array containing the normals
//			[in] numCompTexCoord : is the number of components per texcoord. 2 or 3 or 4.
//			[in] ptexCoord : Array containing the texcoords
//			[in] numCompTangent : is the number of components per tangent. 3 or 4.
//			[out] ptangent : Array containing the tangents
//			[in] numCompBinormal : is the number of components per binormals. 3 or 4.
//			[out] pbinormal : Array containing the binormals
//
//RETURN:
//			1 for success
//			-1 if bad values passed (0 for array size, components have an invalid value), GLH_ERROR_INVALID_PARAM
//			0 for memory error, GLH_ERROR_OUT_OF_MEMORY
//UPDATE:	Updated to work with glhGetError. If int returns error, call glhGetError.
GLHLIB_API int glhComputeTangentAndBinormalVectors_v1_f(int totalIndices, unsigned int *pindices, int totalVertex, int numCompVertex, float *pvertex, int numCompNormals, float *pnormals, int numCompTexCoord, float *ptexCoord, int numCompTangent, float *ptangent, int numCompBinormal, float *pbinormal);
GLHLIB_API int glhComputeTangentAndBinormalVectors_v1_f_2(int totalIndices, unsigned short *pindices, int totalVertex, int numCompVertex, float *pvertex, int numCompNormals, float *pnormals, int numCompTexCoord, float *ptexCoord, int numCompTangent, float *ptangent, int numCompBinormal, float *pbinormal);

/*Block 16 -end*/

/*Block 17 -start*/
/*Block 17 -end*/

/*Block 18 -start*/
//REFLECTION / REFRACTION

//PURPOSE: reflect = 2 * dot(N, I) * N - I
//       <  ^  >
//        \ | /
//         \|/  the left one is I (incident), the center one is N (normal), the right is reflected
//=====================
//N and I should be normalized.
//
//reflected is pointer to a sequence of 3D vectors
//pincidentVector is pointer to a sequence of 3D vectors
//pnormal is pointer to a sequence of 3D vectors
//count is number of vectors to be processed
GLHLIB_API void glhComputeReflectedVectorf(float *reflected, float *pincidentVector, float *pnormal, int count);

//PURPOSE:
//       <  ^   
//        \ |  
//         \|   the left one is I (incident), the center one is N (normal), the right is refracted
//			\
//			 \
//			  >
//=====================
//N and I should be normalized.
//eta is the ratio of the indices (from material / to be entered material)
//
//refracted is pointer to a sequence of 3D vectors
//pincidentVector is pointer to a sequence of 3D vectors
//pnormal is pointer to a sequence of 3D vectors
//eta is constant for the material and the wavelength
//count is number of vectors to be processed
//
//NOTE:	It might be a good idea to take total internal reflection into account
//		using external code.
GLHLIB_API void glhComputeRefractedVectorf(float *refracted, float *pincidentVector, float *pnormal, float eta, int count);

/*Block 18 -end*/

/*Block 19 -start*/
//****************************************************************************
//QUATERNION RELATED

//PURPOSE:	Set quat to {0, 0, 0, 1}
GLHLIB_API void glhQuaternionIdentityf(float *quat);

//PURPOSE:	If quat is {0, 0, 0, 1}, returns 1 (GL_TRUE), else 0 (GL_FALSE)
GLHLIB_API int glhQuaternionIsIdentityf(float *quat);

//PURPOSE:	If quat[3] == 1.0, returns 1 (GL_TRUE), else 0 (GL_FALSE)
GLHLIB_API int glhQuaternionIsPuref(float *quat);

//PURPOSE:	Calculate the conjugate of a quat.
//			xyz is simply negated. quat is xyzw
GLHLIB_API void glhQuaternionConjugatef(float *quat);

//PURPOSE:	Calculate the magnitude of a quat.
GLHLIB_API void glhQuaternionMagnitudef(float *magnitude, float *quat);

//PURPOSE:	Calculate the magnitude squared of a quat.
GLHLIB_API void glhQuaternionMagnitudeSquaredf(float *magnitude, float *quat);

//PURPOSE:	Compute dot product between 2 quats.
GLHLIB_API void glhQuaternionDotProductf(float *result, float *quat1, float *quat2);

//PURPOSE:	Calculates the exponential of a quat.
//			quat is expected to be a pure quaternion. See glhQuaternionIsPuref.
GLHLIB_API void glhQuaternionExpf(float *outquat, float *quat);

//PURPOSE:	Calculate the natural logarithm of a quat.
//			quat should be unit quat.
GLHLIB_API void glhQuaternionLnf(float *outquat, float *quat);


//PURPOSE:	Normalize quat. Some glhQuaternion***** require normalized quats for efficiency.
GLHLIB_API void glhQuaternionNormalizef(float *quat);

//PURPOSE:	outquat = quat1 * quat2
//			If quat1 and quat2 are normalized, outquat will be a unit quat.
GLHLIB_API void glhQuaternionMultiplyf(float *outquat, float *quat1, float *quat2);

//PURPOSE:	Build a 4x4 column major rotation matrix from a quat.
//			quat should be normalized. See glhQuaternionNormalizef.
//			You can use this in OpenGL.
//
//		    |       2     2                                   |
//		    | 1 - 2Y  - 2Z    2XY - 2ZW      2XZ + 2YW      0 |
//		    |                                                 |
//		    |                       2     2                   |
//		M = | 2XY + 2ZW       1 - 2X  - 2Z   2YZ - 2XW      0 |
//		    |                                                 |
//		    |                                      2     2    |
//		    | 2XZ - 2YW       2YZ + 2XW      1 - 2X  - 2Y   0 |
//		    |                                                 |
//		    |     0               0              0          1 |
//
GLHLIB_API void glhQuaternionQuatToRotationMatrixf(float *matrix, float *quat);

//Same as glhQuaternionQuatToRotationMatrixf except this matrix is transposed.
//You might need to submit this version to Direct3D.
GLHLIB_API void glhQuaternionQuatToRotationMatrixTf(float *matrix, float *quat);

//PURPOSE:	Build a quat from a 4x4 column major rotation matrix.
//			quat will be automatically normalized.
GLHLIB_API void glhQuaternionRotationMatrixToQuatf(float *quat, float *matrix);

//PURPOSE:	Convert quat to a axis of rotation and an angle in radians.
//			axis will receive xyz.
//			Quat should be normalized.
GLHLIB_API void glhQuaternionQuatToAxisAndAnglef(float *axis, float *angleInRadians, float *quat);

//PURPOSE:	Convert axis of rotation and an angle in radians to quat.
//			axis must be xyz. Should be unit vector.
GLHLIB_API void glhQuaternionAxisAndAngleToQuatf(float *quat, float *axis, float *angleInRadians);

//PURPOSE:	Convert Euler angles into a quat. Angles are in radians.
GLHLIB_API void glhQuaternionEulerAnglesToQuatf(float *quat, float roll, float pitch, float yaw);

//PURPOSE:	Interpolates between two quaternions, using spherical linear interpolation.
//			SLERP = Spherical Linear intERPolation
//			Interpolate 2 quats along smallest arc between them.
//			t is the interpolation factor where t = 0.0 gives outquat = quat1
//			and t = 1.0 gives outquat = quat2
//			t should be [0, 1]
//			You should make sure quat1 and quat2 are normalized.
GLHLIB_API void glhQuaternionSLERPf(float *outquat, float *quat1, float *quat2, float t);

/*Block 19 -end*/

/*Block 20 -start*/
//****************************************************************************
//FLOATING POINT NUMBER MANIPULATION

//PURPOSE:	Pack a 32 bit IEEE float into a array of 4 ubytes.
//			This works for numbers 0.0 to <1.0
//			Passing 1.0 gives 0.0
//			All numbers above and below this range are wrapped around.
//			This algorithm can be used in shaders where packing/unpacking may not be available.
//
//			The algorithm is :
//			float packFactors[] = { 1.0, 255.0, 65535.0, 16777215.0 };	//Better than { 1.0, 256.0, 65536.0, 16777216.0 }?
//			float Float[4];
//			Float[1]=Float[2]=Float[3]=Float[0]=inputNumber;	//Store copies
//			//MUL	float, float, packFactors;
//			Float[0]=Float[0]*packFactors[0];
//			Float[1]=Float[1]*packFactors[1];
//			Float[2]=Float[2]*packFactors[2];
//			Float[3]=Float[3]*packFactors[3];
//			//FRC	ub, Float;
//			outputNumber[0]=Float[0]-floor(Float[0]);
//			outputNumber[1]=Float[1]-floor(Float[1]);
//			outputNumber[2]=Float[2]-floor(Float[2]);
//			outputNumber[3]=Float[3]-floor(Float[3]);
GLHLIB_API void glhPackFloatIntoUByte(unsigned char *outputNumber, float inputNumber);

//PURPOSE:	Unpack a 32 bit IEEE float from an array of 3 ubytes to a single float.
//			This works for numbers 0.0 to <1.0
//			Passing 1.0 gives 0.0
//			All numbers above and below this range are wrapped around.
//			This algorithm can be used in shaders where packing/unpacking may not be available.
//
//			The algorithm is :
//			float extract[] = {1.0, 1.0/255.0, 1.0/65535.0, 1.0/16777215.0};	//Better than {1.0, 1.0/256.0, 1.0/65536.0, 1.0/16777216.0}?
//			//DP4	float, ub, extract;
//			*outputNumber=ub[0]*extract[0]+ub[1]*extract[1]+ub[2]*extract[2]+ub[3]*extract[3];
GLHLIB_API void glhUnpackUByteIntoFloat(float *outputNumber, const unsigned char *inputNumber);

//PURPOSE:	Given a 32 bit float, convert to 16 bit float.
//			Useful when you want to work with floats on the CPU, and then convert to 16 bit and send your
//			texture to OpenGL.
//			Or perhaps upload as a uniform for your shader.
GLHLIB_API void glhFloatToHalf(float infloat, unsigned short &outhalf);

/*Block 20 -end*/

/*Block 21 -start*/
//****************************************************************************
//GEOMETRY GENERATION

//PURPOSE:	A common object from which specialized structs are derived
//			The previous glhCommonObject has been deleted and we now have glhCommonObjectf2
//			**** The same applies to the derived stuff ****
class glhCommonObjectf2
{
public:
	unsigned int		AverageNormals;							//Used in some cases

	int			TexCoordStyle[8];						//Used in some cases

	GLH_INDEXFORMAT	IndexFormat;
	int			TotalIndex;
	unsigned short		*pIndex16Bit;							//Depends on IndexFormat, one or the other will be used
	unsigned int			*pIndex32Bit;							//Depends on IndexFormat, one or the other will be used

	GLH_VERTEXFORMAT	VertexFormat;						//One of the defines, see top of this file
	int			VertexCount;
	float			*pVertex;								//This receives the array of vertices
															//
};

//PURPOSE:	For use with function glhCreateCubef2 and glhDeleteCubef2
class glhCubeObjectf2 : public glhCommonObjectf2
{
public:
	float			Width, Height, Length;
	int			WidthDivision, HeightDivision, LengthDivision;

	unsigned int			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements
};

//PURPOSE:	For use with function glhCreateCylinderf2 and glhDeleteCylinderf2
class glhCylinderObjectf2 : public glhCommonObjectf2
{
public:
	unsigned int	IsThereATop, IsThereABottom;						//Render or not? If GL_FALSE, then it won't be computed and don't attempt to render.
	float		RadiusA, RadiusB, Height;							//RadiusA is top radius (+z), RadiusB is bottom radius (-z)

	int	Stacks;													//Number of divisions in the Theta direction
	int	Slices;													//Number of divisions in the Phi direction


	float			ScaleFactorS[8], ScaleFactorT[8];				//For texcoord

	//These 2 are for rendering the entire cylinder in one call to glDrawRangeElements
	unsigned int			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements


	//These are for rendering body, top, bottom separately
	unsigned int	Start_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_TopOfCylinder;						//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements

	//Index related
	int	StartIndexIn_pIndex_BodyOfCylinder, EndIndexIn_pIndex_BodyOfCylinder;		//End-1 gives the last index
	int	StartIndexIn_pIndex_TopOfCylinder, EndIndexIn_pIndex_TopOfCylinder;			//End-1 gives the last index
	int	StartIndexIn_pIndex_BottomOfCylinder, EndIndexIn_pIndex_BottomOfCylinder;	//End-1 gives the last index

	int	TotalIndex_BodyOfCylinder;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_TopOfCylinder;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BottomOfCylinder;								//Parameter to pass to glDrawRangeElements

	unsigned short	*pIndex16Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.

	unsigned int	*pIndex32Bit_BodyOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_TopOfCylinder;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BottomOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.

	//Vertex related
	int	StartIndexIn_pVertex_BodyOfCylinder, EndIndexIn_pVertex_BodyOfCylinder;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_TopOfCylinder, EndIndexIn_pVertex_TopOfCylinder;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BottomOfCylinder, EndIndexIn_pVertex_BottomOfCylinder;	//End-1 gives the last vertex


	int	TotalVertex_BodyOfCylinder;
	int	TotalVertex_TopOfCylinder;
	int	TotalVertex_BottomOfCylinder;

	float	*pVertex_BodyOfCylinder;								//Pointer to pVertex where body starts
	float	*pVertex_TopOfCylinder;									//Pointer to pVertex where top starts
	float	*pVertex_BottomOfCylinder;								//Pointer to pVertex where bottom starts
};

//PURPOSE:	For use with function glhCreateSpheref2 and glhDeleteSpheref2
class glhSphereObjectf2 : public glhCommonObjectf2
{
public:
	float			RadiusA, RadiusB, RadiusC;				//A is in the x direction, B is in the y direction, C is in the z direction
	int			Stacks;									//Number of divisions in the Theta direction
	int			Slices;									//Number of divisions in the Phi direction

	float			ScaleFactorS[8], ScaleFactorT[8];		//For texcoord


	unsigned int			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements
};

//PURPOSE:	For use with function glhCreatePartialSphereStackf2 and glhDeletePartialSphereStackf2
class glhPartialSphereStackObjectf2 : public glhSphereObjectf2
{
public:
	int	StartingStack;									//A value from 0 to Stacks-1. Should be <= to EndingStack
	int	EndingStack;									//A value from 0 to Stacks-1. Should be >= to StartingStack
};

//PURPOSE:	For use with function glhCreateTorusf2 and glhDeleteTorusf2
class glhTorusObjectf2 : public glhCommonObjectf2
{
public:
	float	RadiusOfTorus;			//The radius from the center to the outermost edge of the torus
	float	RadiusOfCircle;			//The radius of the cross section of a part of the torus

	int	Stacks;					//The slices along the wheel part of the torus. Minimum is 3.
	int	Slices;					//The slices of each cross section. Minimum is 3.

	float			ScaleFactorS[8], ScaleFactorT[8];		//For texcoord

	unsigned int			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements
};

//PURPOSE:	For use with function glhCreateConef2 and glhDeleteConef2
class glhConeObjectf2 : public glhCommonObjectf2
{
public:
	unsigned int	IsThereABottom;										//Render or not? If GL_FALSE, then it won't be computed and don't attempt to render.
	float		Radius, Height;

	int	Stacks;													//Number of divisions from bottom to the tip
	int	Slices;													//Number of divisions in the Phi direction, kind of like slices of a cake


	float			ScaleFactorS[8], ScaleFactorT[8];				//For texcoord

	//These 2 are for rendering the entire cone in one call to glDrawRangeElements
	unsigned int			Start_DrawRangeElements;						//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;							//Parameter to pass to glDrawRangeElements


	unsigned int	Start_DrawRangeElements_BodyOfCone;						//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BodyOfCone;						//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BottomOfCone;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BottomOfCone;						//Parameter to pass to glDrawRangeElements

	//Index related
	int	StartIndexIn_pIndex_BodyOfCone, EndIndexIn_pIndex_BodyOfCone;		//End-1 gives the last index
	int	StartIndexIn_pIndex_BottomOfCone, EndIndexIn_pIndex_BottomOfCone;	//End-1 gives the last index

	int	TotalIndex_BodyOfCone;										//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BottomOfCone;									//Parameter to pass to glDrawRangeElements

	unsigned short	*pIndex16Bit_BodyOfCone;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BottomOfCone;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.

	unsigned int	*pIndex32Bit_BodyOfCone;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BottomOfCone;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.

	//Vertex related
	int	StartIndexIn_pVertex_BodyOfCone, EndIndexIn_pVertex_BodyOfCone;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BottomOfCone, EndIndexIn_pVertex_BottomOfCone;	//End-1 gives the last vertex


	int	TotalVertex_BodyOfCone;
	int	TotalVertex_BottomOfCone;

	float	*pVertex_BodyOfCone;								//Pointer to pVertex where body starts
	float	*pVertex_BottomOfCone;								//Pointer to pVertex where bottom starts
};

//PURPOSE:	For use with function glhCreateTubef2 and glhDeleteTubef2
class glhTubeObjectf2 : public glhCommonObjectf2
{
public:
	unsigned int	IsThereATop, IsThereABottom;						//Render or not? If GL_FALSE, then it won't be computed and don't attempt to render.
	float		OuterRadiusA, OuterRadiusB;							//RadiusA is top radius (+z), RadiusB is bottom radius (-z)
	float		InnerRadiusA, InnerRadiusB;							//RadiusA is top radius (+z), RadiusB is bottom radius (-z)
	float		Height;

	int	Stacks;													//Number of divisions in the Theta direction
	int	Slices;													//Number of divisions in the Phi direction

	
	float			ScaleFactorS[8], ScaleFactorT[8];				//For texcoord

	//These 2 are for rendering the entire tube in one call to glDrawRangeElements
	unsigned int			Start_DrawRangeElements;				//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;					//Parameter to pass to glDrawRangeElements


	unsigned int	Start_DrawRangeElements_OuterBodyOfTube;			//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_OuterBodyOfTube;				//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_InnerBodyOfTube;			//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_InnerBodyOfTube;				//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_TopOfTube;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_TopOfTube;					//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BottomOfTube;				//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BottomOfTube;					//Parameter to pass to glDrawRangeElements

	//Index related
	int	StartIndexIn_pIndex_OuterBodyOfTube, EndIndexIn_pIndex_OuterBodyOfTube;		//End-1 gives the last index
	int	StartIndexIn_pIndex_InnerBodyOfTube, EndIndexIn_pIndex_InnerBodyOfTube;		//End-1 gives the last index
	int	StartIndexIn_pIndex_TopOfTube, EndIndexIn_pIndex_TopOfTube;					//End-1 gives the last index
	int	StartIndexIn_pIndex_BottomOfTube, EndIndexIn_pIndex_BottomOfTube;			//End-1 gives the last index

	int	TotalIndex_OuterBodyOfTube;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_InnerBodyOfTube;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_TopOfTube;										//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BottomOfTube;									//Parameter to pass to glDrawRangeElements

	unsigned short	*pIndex16Bit_OuterBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_InnerBodyOfTube;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_TopOfTube;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BottomOfTube;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.

	unsigned int	*pIndex32Bit_OuterBodyOfTube;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_InnerBodyOfTube;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_TopOfTube;										//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BottomOfTube;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.

	//Vertex related
	int	StartIndexIn_pVertex_OuterBodyOfTube, EndIndexIn_pVertex_OuterBodyOfTube;	//End-1 gives the last vertex
	int	StartIndexIn_pVertex_InnerBodyOfTube, EndIndexIn_pVertex_InnerBodyOfTube;	//End-1 gives the last vertex
	int	StartIndexIn_pVertex_TopOfTube, EndIndexIn_pVertex_TopOfTube;				//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BottomOfTube, EndIndexIn_pVertex_BottomOfTube;			//End-1 gives the last vertex
	
	
	int	TotalVertex_OuterBodyOfTube;
	int	TotalVertex_InnerBodyOfTube;
	int	TotalVertex_TopOfTube;
	int	TotalVertex_BottomOfTube;

	float	*pVertex_OuterBodyOfTube;							//Pointer to pVertex where body starts
	float	*pVertex_InnerBodyOfTube;							//Pointer to pVertex where body starts
	float	*pVertex_TopOfTube;									//Pointer to pVertex where top starts
	float	*pVertex_BottomOfTube;								//Pointer to pVertex where bottom starts
};


//PURPOSE:	For use with function glhCreateChamferCylinderf2 and glhDeleteChamferCylinderf2
class glhChamferCylinderObjectf2 : public glhCommonObjectf2
{
public:
	int	IsThereATop, IsThereABottom;						//Render or not? If GL_FALSE, then it won't be computed and don't attempt to render.
	float		BodyRadius, Height;									//Height is along the z-axis
	float		ChamferRadius;										//This controls how much chamfer is visible.
																	//It is the radius at the top of the cylinder and also the bottom of the cylinder.
																	//This should be less than BodyRadius.

	int	Stacks;													//Number of divisions in the Theta direction
	int	Slices;													//Number of divisions in the Phi direction
	int	ChamferStacks;											//There will be a chamfer at the top and one at the bottom
																	//This is the number of stacks for each. If this is 1 and Stacks is also
																	//some number like 5, then total stack count will be 1+5+1


	float			ScaleFactorS[8], ScaleFactorT[8];				//For texcoord

	//These 2 are for rendering the entire cylinder in one call to glDrawRangeElements
	unsigned int			Start_DrawRangeElements;						//Parameter to pass to glDrawRangeElements
	unsigned int			End_DrawRangeElements;							//Parameter to pass to glDrawRangeElements


	//These are for rendering top chamfer, body, bottom chamfer, top, bottom separately
	unsigned int	Start_DrawRangeElements_TopChamfer;						//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_TopChamfer;						//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BodyOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BottomChamfer;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BottomChamfer;					//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_TopOfCylinder;					//Parameter to pass to glDrawRangeElements
	unsigned int	Start_DrawRangeElements_BottomOfCylinder;				//Parameter to pass to glDrawRangeElements
	unsigned int	End_DrawRangeElements_BottomOfCylinder;					//Parameter to pass to glDrawRangeElements

	//Index related
	int	StartIndexIn_pIndex_TopChamfer, EndIndexIn_pIndex_TopChamfer;				//End-1 gives the last index
	int	StartIndexIn_pIndex_BodyOfCylinder, EndIndexIn_pIndex_BodyOfCylinder;		//End-1 gives the last index
	int	StartIndexIn_pIndex_BottomChamfer, EndIndexIn_pIndex_BottomChamfer;			//End-1 gives the last index
	int	StartIndexIn_pIndex_TopOfCylinder, EndIndexIn_pIndex_TopOfCylinder;			//End-1 gives the last index
	int	StartIndexIn_pIndex_BottomOfCylinder, EndIndexIn_pIndex_BottomOfCylinder;	//End-1 gives the last index

	int	TotalIndex_TopChamfer;										//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BodyOfCylinder;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BottomChamfer;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_TopOfCylinder;									//Parameter to pass to glDrawRangeElements
	int	TotalIndex_BottomOfCylinder;								//Parameter to pass to glDrawRangeElements

	unsigned short	*pIndex16Bit_TopChamfer;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BodyOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BottomChamfer;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_TopOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.
	unsigned short	*pIndex16Bit_BottomOfCylinder;							//Parameter to pass to glDrawRangeElements. Pointer to pIndex16Bit.

	unsigned int	*pIndex32Bit_TopChamfer;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BodyOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BottomChamfer;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_TopOfCylinder;									//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.
	unsigned int	*pIndex32Bit_BottomOfCylinder;								//Parameter to pass to glDrawRangeElements. Pointer to pIndex32Bit.

	//Vertex related
	int	StartIndexIn_pVertex_TopChamfer, EndIndexIn_pVertex_TopChamfer;				//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BodyOfCylinder, EndIndexIn_pVertex_BodyOfCylinder;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BottomChamfer, EndIndexIn_pVertex_BottomChamfer;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_TopOfCylinder, EndIndexIn_pVertex_TopOfCylinder;		//End-1 gives the last vertex
	int	StartIndexIn_pVertex_BottomOfCylinder, EndIndexIn_pVertex_BottomOfCylinder;	//End-1 gives the last vertex


	int	TotalVertex_TopChamfer;
	int	TotalVertex_BodyOfCylinder;
	int	TotalVertex_BottomChamfer;
	int	TotalVertex_TopOfCylinder;
	int	TotalVertex_BottomOfCylinder;

	float	*pVertex_TopChamfer;										//Pointer to pVertex where body starts
	float	*pVertex_BodyOfCylinder;									//Pointer to pVertex where body starts
	float	*pVertex_BottomChamfer;										//Pointer to pVertex where body starts
	float	*pVertex_TopOfCylinder;										//Pointer to pVertex where top starts
	float	*pVertex_BottomOfCylinder;									//Pointer to pVertex where bottom starts
};




//HOW TO USE :
//Read the comment for each function like glhCreateCubef2. Explanation for each function and code example is given.



//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a cube.
//			The cube will be centered at origin, with no rotation.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//
//PARAMETERS:
//			A glhCubeObjectf2 must be passed to this function.
//			Width : width of cube in x, >0.0
//			Height : height of cube in y >0.0
//			Length : length of cube in z >0.0
//			WidthDivision : the number of division in x, >0
//			HeightDivision : the number of division in y, >0
//			LengthDivision : the number of division in z, >0
//			AverageNormals : GL_TRUE or GL_FALSE. If GL_TRUE, normals are averaged, if GL_FALSE, it's as if cube has face normals.
//			                 ONLY GL_FALSE works.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int, but only 0 is valid for now.
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, then texcoords are computed for each vertex so that each face gets an image.
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhCubeObjectf2 Cube;
//memset(&Cube, 0, sizeof(glhCubeObjectf2));
//Cube.Width=1.0; Cube.Height=1.0; Cube.Length=1.0;
//Cube.WidthDivision=1; Cube.HeightDivision=1; Cube.LengthDivision=1;
//Cube.AverageNormals=GL_FALSE;
//Cube.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Cube.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Cube.TexCoordStyle[0]=1;
//
//glhCreateCubef2(&Cube);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Cube.pVertex);
//uint mypointer=(uint)Cube.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//
//glDrawRangeElements(GL_TRIANGLES, Cube.Start_DrawRangeElements, Cube.End_DrawRangeElements,
//    Cube.TotalIndex, GL_UNSIGNED_SHORT, Cube.pIndex16Bit);
//
//and to deallocate, call glhDeleteCubef2(&Cube);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Cube.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateCubef2(glhCubeObjectf2 *pcube);

//PURPOSE:	Delete the arrays allocated with glhCreateCubef2.
//
//RETURN:
//			Always 1.
GLHLIB_API int glhDeleteCubef2(glhCubeObjectf2 *pcube);


//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a cylinder.
//			The cylinder will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//
//PARAMETERS:
//			RadiusA : Top radius of cylinder, >0.0
//			RadiusB : Bottom radius of cylinder, >0.0
//			Height : Height of cylinder in z >0.0
//			Stacks : Think of it like layers of a cake. >=1
//			Slices : Think of it like sections of a cake. >=3
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around cylinder, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//			All other variables in glhCylinderObject are self explanatory. Here are some examples:
//			Start_DrawRangeElements_BodyOfCylinder : a variable to pass to glDrawRangeElements
//			                                         for rendering the body of the cylinder.
//			pVertex_BodyOfCylinder : A pointer to pVertex. This points to the start of the body.
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhCylinderObjectf2 Cylinder;
//memset(&Cylinder, 0, sizeof(glhCylinderObjectf2));
//Cylinder.IsThereATop=TRUE; Cylinder.IsThereABottom=TRUE;
//Cylinder.RadiusA=1.0; Cylinder.RadiusB=1.0; Cylinder.Height=1.0;
//Cylinder.Stacks=10; Cylinder.Slices=10;
//Cylinder.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Cylinder.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Cylinder.TexCoordStyle[0]=1;
//Cylinder.ScaleFactorS[0]=Cylinder.ScaleFactorT[0]=1.0;
//
//glhCreateCylinderf2(&Cylinder);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Cylinder.pVertex);
//uint mypointer=(uint)Cylinder.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//
//IF YOU WANT TO RENDER THE ENTIRE CYLINDER
//glDrawRangeElements(GL_TRIANGLES, Cylinder.Start_DrawRangeElements, Cylinder.End_DrawRangeElements,
//	Cylinder.TotalIndex, GL_UNSIGNED_SHORT, Cylinder.pIndex16Bit);
//
//IF YOU WANT TO RENDER THE BODY, THEN THE TOP, THEN BOTTOM, make 3 calls to glDrawRangeElements
//glDrawRangeElements(GL_TRIANGLES, Cylinder.Start_DrawRangeElements_BodyOfCylinder, Cylinder.End_DrawRangeElements_BodyOfCylinder,
//	Cylinder.TotalIndex_BodyOfCylinder, GL_UNSIGNED_SHORT, Cylinder.pIndex16Bit_BodyOfCylinder);
//
//glDrawRangeElements(GL_TRIANGLES, Cylinder.Start_DrawRangeElements_TopOfCylinder, Cylinder.End_DrawRangeElements_TopOfCylinder,
//	Cylinder.TotalIndex_TopOfCylinder, GL_UNSIGNED_SHORT, Cylinder.pIndex16Bit_TopOfCylinder);
//
//glDrawRangeElements(GL_TRIANGLES, Cylinder.Start_DrawRangeElements_BottomOfCylinder, Cylinder.End_DrawRangeElements_BottomOfCylinder,
//	Cylinder.TotalIndex_BottomOfCylinder, GL_UNSIGNED_SHORT, Cylinder.pIndex16Bit_BottomOfCylinder);
//
//
//and to deallocate, call glhDeleteCylinderf2(&Cylinder);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Cylinder.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateCylinderf2(glhCylinderObjectf2 *pcylinder);

//PURPOSE:	Delete the arrays allocated with glhCreateCylinderf2.
//
//RETURN:
//			Always 1.
GLHLIB_API int glhDeleteCylinderf2(glhCylinderObjectf2 *pcylinder);


//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a sphere.
//			The sphere will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//
//PARAMETERS:
//			RadiusA : In x direction, >0.0
//			RadiusB : In y direction, >0.0
//			RadiusC : In z direction, >0.0
//			Stacks : Think of it like layers of a cake. >=3
//			Slices : Think of it like sections of a cake. >=3
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around sphere, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhSphereObjectf2 Sphere;
//memset(&Sphere, 0, sizeof(glhSphereObjectf2));
//Sphere.RadiusA=1.0; Sphere.RadiusB=1.0; Sphere.RadiusC=1.0;
//Sphere.Stacks=10; Sphere.Slices=10;
//Sphere.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Sphere.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Sphere.TexCoordStyle[0]=1;
//Sphere.ScaleFactorS[0]=Sphere.ScaleFactorT[0]=1.0;
//
//glhCreateSpheref2(&Sphere);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Sphere.pVertex);
//uint mypointer=(uint)Sphere.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//glDrawRangeElements(GL_TRIANGLES, Sphere.Start_DrawRangeElements, Sphere.End_DrawRangeElements,
//	Sphere.TotalIndex, GL_UNSIGNED_SHORT, Sphere.pIndex16Bit);
//
//
//and to deallocate, call glhDeleteSpheref2(&Sphere);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Sphere.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateSpheref2(glhSphereObjectf2 *psphere);

//PURPOSE:	Delete the arrays allocated with glhCreateSpheref2.
//
//RETURN:
//			Always 1
GLHLIB_API int glhDeleteSpheref2(glhSphereObjectf2 *psphere);


//PURPOSE:	Same as glhCreateSpheref2 except that we have added members StartingStack and EndingStack
//			to control which stacks are generated.
//			The stacks are numbered 0 to Stacks-1
//
//PARAMETERS:
//			[in] StartingStack : 0 up to Stacks-1. Must be <= EndingStacks
//			[in] EndingStack : 0 up to Stacks-1. Must be >= StartingStack
//
//HOW TO USE:
//Add these :
//Sphere.StartingStack=0;
//Sphere.EndingStack=3;
//If you have 8 stacks (meaning that psphere.Stacks==8) and you want a dome (skybox or some other use)
//0 through 3 are the stacks to generate,
//
//Yes, you can have StartingStack=0 and Sphere.EndingStack=0 and this will generate the first stack.
//You can generate only the last stack by having StartingStack=7 and Sphere.EndingStack=7.
//
//Another example, StartingStack=0 and Sphere.EndingStack=1
//and that will generate stack #0 and stack #1 (2 in total)
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Sphere.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreatePartialSphereStackf2(glhPartialSphereStackObjectf2 *psphere);

//PURPOSE:	Delete the arrays allocated with glhCreatePartialSphereStackf2. They were allocated with "new []"
//			so this will use "delete []", which you can do yourself if you wish.
//
//RETURN:
//			Always 1
GLHLIB_API int glhDeletePartialSphereStackf2(glhPartialSphereStackObjectf2 *psphere);


//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a torus.
//			The torus will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			This means the "wheel" part of the torus will lie in the xy plane.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//
//PARAMETERS:
//			RadiusOfTorus : The radius from the center of the torus all the way out.
//			RadiusOfCircle : The radius of the cross section of a part of the torus
//			Stacks : The slices along the wheel part of the torus. Minimum is 3.
//			Slices : The slices of each cross section. Minimum is 3.
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhTorusObjectf2 Torus;
//memset(&Torus, 0, sizeof(glhTorusObjectf2));
//Torus.RadiusOfTorus=1.0;
//Torus.RadiusOfCircle=1.0;
//Torus.Stacks=10;
//Torus.Slices=10;
//Torus.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Torus.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Torus.TexCoordStyle[0]=1;
//Torus.ScaleFactorS[0]=Torus.ScaleFactorT[0]=1.0;
//
//glhCreateTorusf2(&Torus);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Torus.pVertex);
//uint mypointer=(uint)Torus.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//glDrawRangeElements(GL_TRIANGLES, Torus.Start_DrawRangeElements, Torus.End_DrawRangeElements,
//	Torus.TotalIndex, GL_UNSIGNED_SHORT, Torus.pIndex16Bit);
//
//
//and to deallocate, call glhDeleteTorusf2(&Torus);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Torus.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateTorusf2(glhTorusObjectf2 *ptorus);

//PURPOSE:	Delete the arrays allocated with glhCreateTorusf2. They were allocated with "new []"
//			so this will use "delete []", which you can do yourself if you wish.
//
//RETURN:
//			Always 1
GLHLIB_API int glhDeleteTorusf2(glhTorusObjectf2 *ptorus);

//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a cone.
//			The cone will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			The bottom of the cone is at {0.0, 0.0, -Height/2} and
//			the top point is at {0.0, 0.0, Height/2}.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//			NOTE: The top of the cone, there is a single vertex and the normal
//			will point to {0.0, 0.0, 1.0}. This might give undersireable lighting effect.
//
//PARAMETERS:
//			Radius : the bottom of the cone
//			Height : the height of the cone
//			Stacks : The slices along the wheel part of the torus. Minimum is 1.
//			Slices : The slices of each cross section, kind of like a cake. Minimum is 3.
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhConeObjectf2 Cone;
//memset(&Cone, 0, sizeof(glhConeObjectf2));
//Cone.IsThereABottom=TRUE;
//Cone.Radius=1.0;
//Cone.Height=2.0;
//Cone.Stacks=10;
//Cone.Slices=10;
//Cone.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Cone.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Cone.TexCoordStyle[0]=1;
//Cone.ScaleFactorS[0]=Cone.ScaleFactorT[0]=1.0;
//
//glhCreateConef2(&Cone);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//HOW TO RENDER THE ENTIRE CONE:
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Cone.pVertex);
//uint mypointer=(uint)Cone.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//glDrawRangeElements(GL_TRIANGLES, Cone.Start_DrawRangeElements, Cone.End_DrawRangeElements,
//	Cone.TotalIndex, GL_UNSIGNED_SHORT, Cone.pIndex16Bit);
//
//
//and to deallocate, call glhDeleteConef2(&Cone);
//
//
//HOW TO RENDER BODY OF CONE AND BOTTOM OF CONE SEPARATELY:
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID_Body);				//THIS PART IS DIFFERENT
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Cone.pVertex);
//uint mypointer=(uint)Cone.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//
//THE CALL TO glDrawRangeElements IS DIFFERENT
//glDrawRangeElements(GL_TRIANGLES, Cone.Start_DrawRangeElements_BodyOfCone, Cone.End_DrawRangeElements_BodyOfCone,
//	Cone.TotalIndex_BodyOfCone, GL_UNSIGNED_SHORT, Cone.pIndex16Bit_BodyOfCone);
//
//glBindTexture(GL_TEXTURE_2D, TextureID_Bottom);			//THIS PART IS DIFFERENT
//The pointers to glVertexPointer and such are fine.
//We need to call glDrawRangeElements and give the right index pointer = pIndex16Bit_BottomOfCone
//glDrawRangeElements(GL_TRIANGLES, Cone.Start_DrawRangeElements_BottomOfCone, Cone.End_DrawRangeElements_BottomOfCone,
//	Cone.TotalIndex_BottomOfCone, GL_UNSIGNED_SHORT, Cone.pIndex16Bit_BottomOfCone);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Cone.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateConef2(glhConeObjectf2 *pcone);

//PURPOSE:	Delete the arrays allocated with glhCreateConef2. They were allocated with "new []"
//			so this will use "delete []", which you can do yourself if you wish.
//
//RETURN:
//			Always 1
GLHLIB_API int glhDeleteConef2(glhConeObjectf2 *pcone);


//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a tube.
//			A tube is a cylinder except it is as if someone drilled a hole in it.
//
//			The tube will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//
//PARAMETERS:
//			OuterRadiusA : Top radius of tube, >0.0
//			OuterRadiusB : Bottom radius of tube, >0.0
//			InnerRadiusA : Top radius of tube, >0.0
//			InnerRadiusB : Bottom radius of tube, >0.0
//			Height : Height of cylinder in z >0.0
//			Stacks : Think of it like layers of a cake. >=1
//			Slices : Think of it like sections of a cake. >=3
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around cylinder, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//			All other variables in glhTubeObject are self explanatory. Here are some examples:
//			Start_DrawRangeElements_OuterBodyOfTube : a variable to pass to glDrawRangeElements
//			                                         for rendering the body of the tube.
//			pVertex_OuterBodyOfTube : A pointer to pVertex. This points to the start of the body.
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhTubeObjectf2 Tube;
//memset(&Tube, 0, sizeof(glhTubeObjectf2));
//Tube.IsThereATop=TRUE;
//Tube.IsThereABottom=TRUE;
//Tube.OuterRadiusA=2.0;
//Tube.OuterRadiusB=2.0;
//Tube.InnerRadiusA=1.0;
//Tube.InnerRadiusB=1.0;
//Tube.Height=1.0;
//Tube.Stacks=10; Tube.Slices=10;
//Tube.IndexFormat=GLH_INDEXFORMAT_16BIT;
//Tube.VertexFormat=GLH_VERTEXFORMAT_VNT;
//Tube.TexCoordStyle[0]=1;
//Tube.ScaleFactorS[0]=Tube.ScaleFactorT[0]=1.0;
//
//glhCreateTubef2(&Tube);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), Tube.pVertex);
//uint mypointer=(uint)Tube.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//
//IF YOU WANT TO RENDER THE ENTIRE TUBE
//glDrawRangeElements(GL_TRIANGLES, Tube.Start_DrawRangeElements, Tube.End_DrawRangeElements,
//	Tube.TotalIndex, GL_UNSIGNED_SHORT, Tube.pIndex16Bit);
//
//IF YOU WANT TO RENDER THE OUTER BODY, THEN THE INNER BODY, THEN THE TOP, THEN BOTTOM, make 3 calls to glDrawRangeElements
//glDrawRangeElements(GL_TRIANGLES, Tube.Start_DrawRangeElements_OuterBodyOfTube, Tube.End_DrawRangeElements_OuterBodyOfTube,
//	Tube.TotalIndex_OuterBodyOfTube, GL_UNSIGNED_SHORT, Tube.pIndex16Bit_OuterBodyOfTube);
//
//glDrawRangeElements(GL_TRIANGLES, Tube.Start_DrawRangeElements_InnerBodyOfTube, Tube.End_DrawRangeElements_InnerBodyOfTube,
//	Tube.TotalIndex_InnerBodyOfTube, GL_UNSIGNED_SHORT, Tube.pIndex16Bit_InnerBodyOfTube);
//
//glDrawRangeElements(GL_TRIANGLES, Tube.Start_DrawRangeElements_TopOfTube, Tube.End_DrawRangeElements_TopOfTube,
//	Tube.TotalIndex_TopOfTube, GL_UNSIGNED_SHORT, Tube.pIndex16Bit_TopOfTube);
//
//glDrawRangeElements(GL_TRIANGLES, Tube.Start_DrawRangeElements_BottomOfTube, Tube.End_DrawRangeElements_BottomOfTube,
//	Tube.TotalIndex_BottomOfTube, GL_UNSIGNED_SHORT, Tube.pIndex16Bit_BottomOfTube);
//
//
//and to deallocate, call glhDeleteTubef2(&Tube);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)Tube.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateTubef2(glhTubeObjectf2 *ptube);

//PURPOSE:	Delete the arrays allocated with glhCreateTubef2.
//
//RETURN:
//			Always 1.
GLHLIB_API int glhDeleteTubef2(glhTubeObjectf2 *ptube);

//PURPOSE:	Generate the index, vertex and possibly other attributes for rendering a chamfer cylinder.
//			The chamfer cylinder will be centered at origin, with no rotation, its main axis will be about the z-axis.
//			Each vertex can have its own normal, its own texcoord, its own tangents and binormals.
//			A chamfer cylinder has the sharp edges rounded. This shape is perfect for barrels and tires.
//			The normals are entirely smooth shaded.
//			There is no RadiusA and RadiusB as is the case for cylinder. This class only has 1 radius.
//
//PARAMETERS:
//			BodyRadius : Top radius of cylinder, >0.0
//			ChamferRadius : This is the radius of the circle at the top and also the bottom.
//			                This should be less than BodyRadius. It would create a tire-like object.
//			                If this is 0.0, then a pill shaped object will be created. Can't be less than 0.0.
//			Height : Height of cylinder in z >0.0
//			Stacks : Think of it like layers of a cake. >=1
//			Slices : Think of it like sections of a cake. >=3
//			AverageNormals : Not used for now.
//			TotalIndex : Will receive number of indices. Must not be NULL.
//			IndexFormat : GLH_INDEXFORMAT_16BIT or GLH_INDEXFORMAT_32BIT
//			pIndex16Bit : If indexFormat == GLH_INDEXFORMAT_16BIT
//			              Each triplet of indices makes a triangle.
//			              **** It's possible that if many indices are created (>2^16) then
//			              **** pindex32Bit will be used instead, but this is unlikely to happen.
//			pIndex32Bit : If indexFormat == GLH_INDEXFORMAT_32BIT
//			              Each triplet of indices makes a triangle.
//			VertexCount : Will receive the number of vertices.
//			pVertex : The vertices.
//			TexCoordStyle : Array of 8 int
//			                If texCoordStyle[X]==0, then memory is reserved for texcoords, nothing is written to the array.
//			                If texCoordStyle[X]==1, the default wrap around cylinder, with one on top and one on bottom centered.
//			                                        The body will have texture wrapped around once.
//			ScaleFactorS and ScaleFactorT : Array of 8 float. Scale factor in s and t direction.
//
//			All other variables in glhChamferCylinderObject are self explanatory. Here are some examples:
//			Start_DrawRangeElements_BodyOfCylinder : a variable to pass to glDrawRangeElements
//			                                         for rendering the body of the cylinder.
//			pVertex_BodyOfCylinder : A pointer to pVertex. This points to the start of the body.
//
//RETURN:
//			glhGetError returns GLH_ERROR_OUT_OF_MEMORY if it can't reserve memory with new[] operator.
//			glhGetError returns GLH_ERROR_INVALID_PARAM if one of the parameters is wrong.
//			-1 on error.
//			1 for success.
//
//HOW TO USE:
//glhChamferCylinderObjectf2 ChamferCylinder;
//memset(&ChamferCylinder, 0, sizeof(glhChamferCylinderObjectf2));
//ChamferCylinder.IsThereATop=TRUE; ChamferCylinder.IsThereABottom=TRUE;
//ChamferCylinder.BodyRadius=1.0;
//ChamferCylinder.ChamferRadius=0.9;
//ChamferCylinder.Height=1.0;
//ChamferCylinder.Stacks=10;
//ChamferCylinder.Slices=10;
//ChamferCylinder.ChamferStacks=2;
//ChamferCylinder.IndexFormat=GLH_INDEXFORMAT_16BIT;
//ChamferCylinder.VertexFormat=GLH_VERTEXFORMAT_VNT;
//ChamferCylinder.TexCoordStyle[0]=1;
//ChamferCylinder.ScaleFactorS[0]=ChamferCylinder.ScaleFactorT[0]=1.0;
//
//glhCreateChamferCylinderf2(&ChamferCylinder);
//
//HOW TO RENDER (You might want to use VBO, I'm just using VA here):
//glEnable(GL_TEXTURE_2D);
//glBindTexture(GL_TEXTURE_2D, TextureID);
//
//glVertexPointer(3, GL_FLOAT, sizeof(GLHVertex_VNT), ChamferCylinder.pVertex);
//uint mypointer=(uint)ChamferCylinder.pVertex;
//mypointer+=12;
//glNormalPointer(GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//mypointer+=12;
//glTexCoordPointer(2, GL_FLOAT, sizeof(GLHVertex_VNT), (uint *)mypointer);
//
//IF YOU WANT TO RENDER THE ENTIRE CYLINDER
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements, ChamferCylinder.End_DrawRangeElements,
//	ChamferCylinder.TotalIndex, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit);
//
//IF YOU WANT TO RENDER THE TOP CHAMFEER, BODY, BOTTOM CHAMFER, THEN THE TOP, THEN BOTTOM, make 5 calls to glDrawRangeElements
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements_TopChamfer, ChamferCylinder.End_DrawRangeElements_TopChamfer,
//	ChamferCylinder.TotalIndex_TopChamfer, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit_TopChamfer);
//
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements_BodyOfCylinder, ChamferCylinder.End_DrawRangeElements_BodyOfCylinder,
//	ChamferCylinder.TotalIndex_BodyOfCylinder, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit_BodyOfCylinder);
//
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements_BottomChamfer, ChamferCylinder.End_DrawRangeElements_BottomChamfer,
//	ChamferCylinder.TotalIndex_BottomChamfer, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit_BottomChamfer);
//
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements_TopOfCylinder, ChamferCylinder.End_DrawRangeElements_TopOfCylinder,
//	ChamferCylinder.TotalIndex_TopOfCylinder, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit_TopOfCylinder);
//
//glDrawRangeElements(GL_TRIANGLES, ChamferCylinder.Start_DrawRangeElements_BottomOfCylinder, ChamferCylinder.End_DrawRangeElements_BottomOfCylinder,
//	ChamferCylinder.TotalIndex_BottomOfCylinder, GL_UNSIGNED_SHORT, ChamferCylinder.pIndex16Bit_BottomOfCylinder);
//
//
//and to deallocate, call glhDeleteChamferCylinderf2(&ChamferCylinder);
//
//
//If you want to access the vertices, you would have to do something like this :
//Since we are using the VNT format, struct GLHVertex_VNT needs to be used. This is defined in this file.
//Just declare a variable :
//GLHVertex_VNT *these_are_my_vertices;
//these_are_my_vertices=(GLHVertex_VNT *)ChamferCylinder.pVertex;		//Do a cast
//
//Now you can access vertices
//this_is_x=these_are_my_vertices[0].x;
//this_is_y=these_are_my_vertices[0].y;
//this_is_z=these_are_my_vertices[0].z;
//this_is_nx=these_are_my_vertices[0].nx;					//The x component of the normal
//this_is_x=these_are_my_vertices[0].x;
//this_is_s0=these_are_my_vertices[0].s0;					//The s component of the texcoord
//
//but you will rarely need to do that. Perhaps if you want to change the position.
//Perhaps if you want to deform the object.
//Perhaps if you want to do some physics calculations and collision.
GLHLIB_API int glhCreateChamferCylinderf2(glhChamferCylinderObjectf2 *pchamferCylinder);

//PURPOSE:	Delete the arrays allocated with glhCreateChamferCylinderf2.
//
//RETURN:
//			Always 1.
GLHLIB_API int glhDeleteChamferCylinderf2(glhChamferCylinderObjectf2 *pchamferCylinder);



/*Block 21 -end*/

/*Block 22 -start*/
//****************************************************************************
//COLOR SPACE CONVERSION

//PURPOSE:	Using red, green and blue, compute hue, saturation and value.
GLHLIB_API void glhComputeHSVWithRGBf(float &hue, float &saturation, float &value, float red, float green, float blue);

//PURPOSE:	Using hue, saturation and value, compute red, green and blue.
GLHLIB_API void glhComputeRGBWithHSVf(float &red, float &green, float &blue, float hue, float saturation, float value);

//PURPOSE:	Using hue, saturation and value, compute red, green and blue.
//			Write it to a 32 bit uint. Format is 0xAARRGGBB.
GLHLIB_API void glhComputeBGRAWithHSVfub(unsigned int &bgraColor, float hue, float saturation, float value, float alpha);

//PURPOSE:	Using hue, saturation and value, compute red, green and blue.
//			Write it to a 32 bit uint. Format is 0xAABBGGRR.
GLHLIB_API void glhComputeRGBAWithHSVfub(unsigned int &rgbaColor, float hue, float saturation, float value, float alpha);

//PURPOSE:	Using red, green and blue, compute hue, saturation and value.
//			bgraColor is in 0xAARRGGBB format.
GLHLIB_API void glhComputeHSVWithBGRAubf(float &hue, float &saturation, float &value, float &alpha, unsigned int bgraColor);

//PURPOSE:	Using red, green and blue, compute hue, saturation and value.
//			bgraColor is in 0xAABBGGRR format.
GLHLIB_API void glhComputeHSVWithRGBAubf(float &hue, float &saturation, float &value, float &alpha, unsigned int rgbaColor);


/*Block 22 -end*/

/*Block 23 -start*/
//****************************************************************************
//INTERSECTION TESTING

//PURPOSE:	Test to see if a ray intersects a triangle.
//			Start point of ray is rayPoint1 (xyz)
//			rayDirection need no be normalized (xyz)
//			rayDirectionLengthSquared = rayDirection DOT rayDirection
//			trianglePoint1, trianglePoint2 are the first 2 vertices (xyz)
//			triangleVector1 is trianglePoint2-trianglePoint1 (xyz)
//			triangleVector2 is triangleVector3-triangleVector1 (xyz)
//			triangleVector1LengthSquared = triangleVector1 DOT triangleVector1
//			triangleVector2LengthSquared = triangleVector2 DOT triangleVector2
//			triangleVector1_DOT_triangleVector2 = triangleVector1 DOT triangleVector2
//			pointOfIntersection receives the intersection point (xyz)
//
//			Returns 1 if intersection detected, and pointOfIntersection receives the intersection point
//			else returns 0.
GLHLIB_API int glhDoesRayCrossTrianglef(float *rayPoint1, float *rayDirection, float rayDirectionLengthSquared,
												float *trianglePoint1, float *trianglePoint2,
												float *triangleVector1, float *triangleVector2,
												float triangleVector1LengthSquared, float triangleVector2LengthSquared,
												float triangleVector1_DOT_triangleVector2,
												float *pointOfIntersection);


/*Block 23 -end*/

/*Block 24 -start*/
//****************************************************************************
//PROCESSOR SECTION

//PURPOSE:	1 for yes, 0 for no.
//			Call glhDoesProcessorSupportSSE first, then call glhDoesProcessorSupportSSE and/or glhDoesProcessorSupportSSE2
GLHLIB_API int glhDoesProcessorSupportMMX();
GLHLIB_API int glhDoesProcessorSupportSSE();
GLHLIB_API int glhDoesProcessorSupportSSE2();
GLHLIB_API int glhDoesProcessorSupport3DNow();
GLHLIB_API int glhDoesProcessorSupport3DNowExtended();
GLHLIB_API int glhDoesOSSupportSSE();

//PURPOSE:	Get information on the CPU using the Windows registry
//			All pointers must be valid.
//			If error occurs while trying to get info from registry, then any of the parameters can be a empty string.
GLHLIB_API void glhGetProcessorInfo(char *pVendor, char *pName, char *pSpeedInMHz);

/*Block 24 -end*/

/*Block 25 -start*/
//****************************************************************************
//SHADER CODE RELATED

//PURPOSE:	Convert a shader from one format into another.
//			D3D pixel shader to GL fragment program
//			D3D vertex shader to GL vertex program
//			For example, from Direct3D ps2.0 to OpenGL ARBfp1.0
//			**** This function can be considered obsolete because for GL, it's own
//			**** high level shader is prefered, called GLSL
//			**** In the future, I may decide to add support for converting D3D low
//			**** level shader to GLSL and convert GL low level shaders to GLSL
//
//PARAMETERS:
//			shaderTypeIn  : should be one of the defines starting with GLH_SHADERTYPE_
//			                GLH_SHADERTYPE_PS_2_0
//			                GLH_SHADERTYPE_PS_1_4
//			                GLH_SHADERTYPE_VS_1_1
//			                GLH_SHADERTYPE_VS_2_0
//			pshaderTextIn : contains a NULL terminated ASCII text of the shader.
//			shaderTypeOut  : should be one of the defines starting with GLH_SHADERTYPE_
//			                 GLH_SHADERTYPE_ARBFP_1_0
//			                 GLH_SHADERTYPE_ARBVP_1_0
//			pshaderTextOut : You should allocate some space for this function to write the output string
//			                 Allocate twice the amount as pshaderTextIn
//			maxLengthOfShaderTextOut : The actual size of pshaderTextOut in bytes.
//
//RETURN:
//			1 for success
//			-1 if not yet implemented
GLHLIB_API int glhTranslateShader(int shaderTypeIn, const char *pshaderTextIn, int shaderTypeOut, char *pshaderTextOut, int maxLengthOfShaderTextOut);


/*Block 25 -end*/

/*Block 26 -start*/
//****************************************************************************
//MEMORY USAGE RELATED

//PURPOSE:	Get the max mipmap count based on :
//			Example : for GL_TEXTURE_2D with width=2, height=1, count will receive 2
//
//PARAMETERS:
//			[out] count : receives single int, which is the mipmap count
//			[in] target : GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
//			              RECT textures can't be mipmapped.
//			[in] width : for 1D, 2D, 3D, cube. >0
//			[in] height : for 2D, cube, otherwise ignored. >0
//			[in] depth : for 3D, otherwise ignored. >0
//
//RETURN:
//			1 for success
//			-1 if target is bad
//			If width, height or depth are 0 or less when they matter count receives 0
//			and function returns 1.
GLHLIB_API int glhGetMaxMipmapCount(int *count, unsigned int target, int width, int height, int depth);

//PURPOSE:	Computes memory consumption of a texture in bytes
//			See also glhGetMaxMipmapCount(int *count, unsigned int target, int width, int height, int depth)
//			Example :
//			int mipmapCount, totalBytes;
//			result=glhGetMaxMipmapCount(&mipmapCount, GL_TEXTURE_2D, width, height, depth);
//			result=glhMemoryUsageWithMipmaps(&totalBytes, GL_TEXTURE_2D, GL_RGBA8, 4, width, height, depth, 0, mipmapCount-1);
//			
//
//PARAMETERS:
//			[out] total : receives single int, count in bytes
//			[in] target : GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP
//			              RECT textures can't be mipmapped.
//			[in] components :
//					GL_ALPHA4
//					GL_ALPHA8
//					GL_ALPHA12
//					GL_ALPHA16
//					GL_LUMINANCE4
//					GL_LUMINANCE8
//					GL_LUMINANCE12
//					GL_LUMINANCE16
//					GL_LUMINANCE4_ALPHA4
//					GL_LUMINANCE6_ALPHA2
//					GL_LUMINANCE8_ALPHA8
//					GL_LUMINANCE12_ALPHA4
//					GL_LUMINANCE12_ALPHA12
//					GL_LUMINANCE16_ALPHA16
//					GL_INTENSITY4
//					GL_INTENSITY8
//					GL_INTENSITY12
//					GL_INTENSITY16
//					GL_R3_G3_B2
//					GL_RGB4
//					GL_RGB5
//					GL_RGB8
//					GL_RGB10
//					GL_RGB12
//					GL_RGB16
//					GL_RGBA2
//					GL_RGBA4
//					GL_RGB5_A1
//					GL_RGBA8
//					GL_RGB10_A2
//					GL_RGBA12
//					GL_RGBA16
//					GL_DEPTH_COMPONENT16
//					GL_DEPTH_COMPONENT24
//					GL_DEPTH_COMPONENT32
//					GL_COMPRESSED_RGB_S3TC_DXT1_EXT (See EXT_texture_compression_s3tc, same as MS DXTC)
//					GL_COMPRESSED_RGBA_S3TC_DXT1_EXT (See EXT_texture_compression_s3tc, same as MS DXTC)
//					GL_COMPRESSED_RGBA_S3TC_DXT3_EXT (See EXT_texture_compression_s3tc, same as MS DXTC)
//					GL_COMPRESSED_RGBA_S3TC_DXT5_EXT (See EXT_texture_compression_s3tc, same as MS DXTC)
//					GL_RGBA_FLOAT32_ATI same as GL_RGBA32F_ARB
//					GL_RGB_FLOAT32_ATI same as GL_RGB32F_ARB
//					GL_ALPHA_FLOAT32_ATI same as GL_ALPHA32F_ARB
//					GL_INTENSITY_FLOAT32_ATI same as GL_INTENSITY32F_ARB
//					GL_LUMINANCE_FLOAT32_ATI same as GL_LUMINANCE32F_ARB
//					GL_LUMINANCE_ALPHA_FLOAT32_ATI same as GL_LUMINANCE_ALPHA32F_ARB
//					GL_RGBA_FLOAT16_ATI same as GL_RGBA16F_ARB
//					GL_RGB_FLOAT16_ATI same as GL_RGB16F_ARB
//					GL_ALPHA_FLOAT16_ATI same as GL_ALPHA16F_ARB
//					GL_INTENSITY_FLOAT16_ATI same as GL_INTENSITY16F_ARB
//					GL_LUMINANCE_FLOAT16_ATI same as GL_LUMINANCE16F_ARB
//					GL_LUMINANCE_ALPHA_FLOAT16_ATI same as GL_LUMINANCE_ALPHA16F_ARB
//			[in] dataAlignment : should be 1, 2, 4, 8, .... (Typically 1 or 4)
//			[in] width : for 1D, 2D, 3D, cube. >0
//			[in] height : for 2D, cube, otherwise ignored. >0
//			[in] depth : for 3D, otherwise ignored. >0
//			[in] lowRange : starting from level 0 mipmap inclusive
//			[in] highRange : up to the highest mipmap level inclusive, counting from the base level=0
//			                 Use glhGetMaxMipmapCount to get the max number and pass count-1 to this function
//			                 to get all mipmap levels
//
//RETURN:
//			1 for success
//			-1 if target is bad
//			-2 if components is bad
//			If width, height or depth are 0 or less when they matter total receives 0
//			and function returns 1.
//
//ISSUE:	Certain formats like GL_LUMINANCE4 and others specify partial byte count
//			and this function counts bytes as integers.
//			I decided to ceil such formats.
//			Example : GL_LUMINANCE4 is 0.5 bytes and becomes 1 byte.
GLHLIB_API int glhMemoryUsageWithMipmaps(int *total, unsigned int target, int components, int dataAlignment, int width, int height, int depth, int lowRange, int highRange);

//PURPOSE:	Estimate how much VRAM the OpenGL surface is using
//			The algorithm used is :
//			if((depthBufferBits==24)&&(depthAndStencilTogether==FALSE))
//				otherbuffer1=32;
//			else
//				otherbuffer1=depthBufferBits;
//
//			if((depthBufferBits==24)&&(depthAndStencilTogether==TRUE)&&(stencilBufferBits==8))
//				otherbuffer2=0;
//			else
//				otherbuffer2=stencilBufferBits;
//
//			if(backBufferCount==0)
//				backBufferCount++;
//
//			sum = (color_buffer + otherbuffer1 + otherbuffer2) * backBufferCount;
//			if(includeFrontBuffer)
//				sum += bitsPerPixel_Color_FrontBuffer;
//
//PARAMETERS:
//			[out] memoryCount : VRAM usage, in bytes
//			[in] width : width of frame buffer
//			[in] height : height of frame buffer
//			[in] includeFrontBuffer : count the front buffer as well? TRUE or FALSE
//			[in] bitsPerPixel_Color_FrontBuffer : 16, 24, 32. If includeFrontBuffer=FALSE, this doesn't matter
//			[in] bitsPerPixel_Color : For the back buffer. There could be many back buffers. 16, 24, 32
//			[in] depthAndStencilTogether : Is it a merged z-buffer and stencil buffer? TRUE or FALSE
//			[in] depthBufferBits : 16, 24, 32
//			[in] stencilBufferBits : 0, 8, 16, 24, 32
//			[in] backBufferCount : How many backbuffers have been created?
//			                       0 and 1 mean the same. There is at least 1 back buffer.
//			                       2 means tripple buffering
//			                       3 and above is also possible but sure if any GPU supports it
//
//RETURN:
//			1 for success
//			-1 if an unexpectant value was passed
GLHLIB_API int glhComputeFrameBufferUsage(int *memoryCount, int width, int height, int includeFrontBuffer, int bitsPerPixel_Color_FrontBuffer, int bitsPerPixel_Color, int depthAndStencilTogether, int depthBufferBits, int stencilBufferBits, int backBufferCount);

/*Block 26 -end*/

/*Block 27 -start*/



//PURPOSE:	This function computes a k-th Order Catmull-Rom Spline.
//			Typically, the Catmull-Rom functions are limited to 4 points : P0, P1, P2, P3
//			and also, you vary a variable called t from 0.0 to 1.0 and you get a curve from P1 to P2.
//			This function doesn't have such limitations.
//			You can supply any number of points.
//			The curve is generated *starting* from the first control point *until* the last control point.
//
//PARAMETERS:
//			[in] numberOfComponentsForVertex : 1, 2, 3, 4
//			[out] pvertex : memory for pvertex must be allocated externally : new sreal[((numberOfControlPoints-1)*intervalCount+1)*numberOfComponentsForVertex]
//			                If numberOfComponentsForVertex==4, then w will always be set to 1.0.
//			[in] intervalCount : must be >0. This is the number of intervals between control points!!!
//			                     If you set this to 1, and you have 4 control points, then there will be 4 vertices generated.
//			                     If you set this to 2, and you have 4 control points, then there will be (4-1)*2+1=7 vertices generated.
//			                     If you set this to 3, and you have 4 control points, then there will be (4-1)*3+1=10 vertices generated.
//			                     If you set this to 4, and you have 4 control points, then there will be (4-1)*4+1=13 vertices generated.
//			                     If you set this to 5, and you have 4 control points, then there will be (4-1)*5+1=16 vertices generated.
//			                     If you set this to X, and you have 4 control points, then there will be (4-1)*X+1=Z vertices generated.
//			                     **** You would probably use a high value here so that the curve will have enough definition.
//			[in] controlPoints : the component count (xyz) must be the same as numberOfComponentsForVertex.
//			                     The array size must be [numberOfComponentsForVertex*numberOfControlPoints].
//			                     If numberOfComponentsForVertex==4, then you can set w to 1.0 if you want. It doesn't matter.
//			                     pvertex's w will be set to 1.0
//			[in] numberOfControlPoints : must be at least 4. 2 or 3 would be too little.
//			                             You need 4 if you don't want to cause a crash.
//
//EXAMPLE:
//Unfortunately, we are using immediate mode here. Feel free to convert to IBO/VBO if you want.
//
//int numberOfComponentsForVertex=3;
//int intervalCount=20;
//int numberOfControlPoints=4;
//int totalVertex=((numberOfControlPoints-1)*intervalCount+1)*numberOfComponentsForVertex;
//float *pvertex=new float[totalVertex];
//float *controlPoints=new float[numberOfComponentsForVertex*numberOfControlPoints];
////Let's setup the control points
//controlPoints[0]=0.0;
//controlPoints[1]=0.0;
//controlPoints[2]=0.0;
//controlPoints[3]=0.0;
//controlPoints[4]=1.0;
//controlPoints[5]=0.0;
//controlPoints[6]=1.0;
//controlPoints[7]=1.0;
//controlPoints[8]=0.0;
//controlPoints[9]=1.0;
//controlPoints[10]=0.0;
//controlPoints[11]=0.0;
////Let's evaluate the curve
//glhEvaluateCatmullRomCurvef(numberOfComponentsForVertex, pvertex, intervalCount, controlPoints, numberOfControlPoints);
//
//glDisable(GL_LIGHTING);
//glColor4f(1.0, 1.0, 1.0, 1.0);
//glBegin(GL_LINE_STRIP);
//for(i=0; i<totalVertex; i+=3)
//{
//	glVertex3fv(&pvertex[i]);
//}
//glEnd();
//glColor4f(1.0, 1.0, 0.0, 1.0);
//glPointSize(5.0);
//glBegin(GL_POINTS);
//for(i=0; i<numberOfComponentsForVertex*numberOfControlPoints; i+=3)
//{
//	glVertex3fv(&controlPoints[i]);
//}
//glEnd();
//delete [] pvertex;
//delete [] controlPoints;
GLHLIB_API void glhEvaluateCatmullRomCurvef(int numberOfComponentsForVertex, float *pvertex, int intervalCount, const float *controlPoints, int numberOfControlPoints);


/*Block 27 -end*/



//Don't put this in the released glhlib.h
void DeleteCommonObject(glhCommonObjectf2 *common);


#ifdef __cplusplus
}
#endif




#endif //_GLHLIB_H_
