#pragma once

//===========================================================================
// Vertex Attributes
//===========================================================================
#define GR_ATTRIB_POSITION_NAME					"a_Position"
#define GR_ATTRIB_TANGENT_NAME					"a_Tangent"
#define GR_ATTRIB_BINORMAL_NAME					"a_Binormal"
#define GR_ATTRIB_NORMAL_NAME					"a_Normal"
#define GR_ATTRIB_TEXCOORD_NAME					"a_TexCoord"
#define GR_ATTRIB_COLOR_NAME					"a_Color"

#define GR_ATTRIB_POSITION_INDEX				0
#define GR_ATTRIB_TANGENT_INDEX					1
#define GR_ATTRIB_BINORMAL_INDEX				2
#define GR_ATTRIB_NORMAL_INDEX					3
#define GR_ATTRIB_TEXCOORD_INDEX				4
#define GR_ATTRIB_COLOR_INDEX					5
#define GR_ATTRIB_COUNT							6

#define GR_ATTRIB_POSITION_MASK					(1<<0)
#define GR_ATTRIB_TANGENT_MASK					(1<<1)
#define GR_ATTRIB_BINORMAL_MASK					(1<<2)
#define GR_ATTRIB_NORMAL_MASK					(1<<3)
#define GR_ATTRIB_TEXCOORD_MASK					(1<<4)
#define GR_ATTRIB_COLOR_MASK					(1<<5)

//===========================================================================
// Shader Parameters
//===========================================================================
#define GR_MODEL_VIEW_PROJECTION_MATRIX_NAME	"u_ModelViewProjectionMatrix"
#define GR_VIEW_POS_NAME						"u_ViewPos"
#define GR_LIGHT_POS_NAME						"u_LightPos"

#define GR_TEX_DIFFUSE							1
#define GR_TEX_DIFFUSE_NAME						"s_DiffuseTex"

#define GR_TEX_NORMAL							2
#define GR_TEX_NORMAL_NAME						"s_NormalTex"

/*
#define GR_TEX_SKIN0							3
#define GR_TEX_SKIN1							(GR_TEX_SKIN0 + 1)
#define GR_TEX_SKIN2							(GR_TEX_SKIN0 + 2)
#define GR_TEX_SKIN3							(GR_TEX_SKIN0 + 3)
#define GR_TEX_SKIN4							(GR_TEX_SKIN0 + 4)
#define GR_TEX_SKIN5							(GR_TEX_SKIN0 + 5)
#define GR_TEX_SKIN6							(GR_TEX_SKIN0 + 6)
#define GR_TEX_SKIN0_NAME						"s_Skin0Tex"
#define GR_TEX_SKIN1_NAME						"s_Skin1Tex"
#define GR_TEX_SKIN2_NAME						"s_Skin2Tex"
#define GR_TEX_SKIN3_NAME						"s_Skin3Tex"
#define GR_TEX_SKIN4_NAME						"s_Skin4Tex"
#define GR_TEX_SKIN5_NAME						"s_Skin5Tex"
#define GR_TEX_SKIN6_NAME						"s_Skin6Tex"
*/

