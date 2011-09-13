#pragma once

//===========================================================================
// Texture Unit Limits
//===========================================================================
#define GL2_TEXUNIT_COUNT						16
#define GL2_TEXUNIT(x)							static_cast<PxU32>(x)

//===========================================================================
// Vertex Attributes
//===========================================================================
#define GL2_ATTRIB_POSITION_NAME				"a_Position"
#define GL2_ATTRIB_TANGENT_NAME					"a_Tangent"
#define GL2_ATTRIB_BINORMAL_NAME				"a_Binormal"
#define GL2_ATTRIB_NORMAL_NAME					"a_Normal"
#define GL2_ATTRIB_TEXCOORD_NAME				"a_TexCoord"
#define GL2_ATTRIB_COLOR_NAME					"a_Color"

#define GL2_ATTRIB_POSITION_INDEX				0
#define GL2_ATTRIB_TANGENT_INDEX				1
#define GL2_ATTRIB_BINORMAL_INDEX				2
#define GL2_ATTRIB_NORMAL_INDEX					3
#define GL2_ATTRIB_TEXCOORD_INDEX				4
#define GL2_ATTRIB_COLOR_INDEX					5
#define GL2_ATTRIB_COUNT						6

#define GL2_ATTRIB_POSITION_MASK				(1<<0)
#define GL2_ATTRIB_TANGENT_MASK					(1<<1)
#define GL2_ATTRIB_BINORMAL_MASK				(1<<2)
#define GL2_ATTRIB_NORMAL_MASK					(1<<3)
#define GL2_ATTRIB_TEXCOORD_MASK				(1<<4)
#define GL2_ATTRIB_COLOR_MASK					(1<<5)

//===========================================================================
// Shader Parameters
//===========================================================================
#define GL2_MODEL_VIEW_PROJECTION_MATRIX_NAME	"u_ModelViewProjectionMatrix"
#define GL2_VIEW_POS_NAME						"u_ViewPos"
#define GL2_LIGHT_POS_NAME						"u_LightPos"

#define GL2_TEX_DIFFUSE							1
#define GL2_TEX_DIFFUSE_NAME					"s_DiffuseTex"

#define GL2_TEX_NORMAL							2
#define GL2_TEX_NORMAL_NAME						"s_NormalTex"

/*
#define GL2_TEX_SKIN0							3
#define GL2_TEX_SKIN1							(GL2_TEX_SKIN0 + 1)
#define GL2_TEX_SKIN2							(GL2_TEX_SKIN0 + 2)
#define GL2_TEX_SKIN3							(GL2_TEX_SKIN0 + 3)
#define GL2_TEX_SKIN4							(GL2_TEX_SKIN0 + 4)
#define GL2_TEX_SKIN5							(GL2_TEX_SKIN0 + 5)
#define GL2_TEX_SKIN6							(GL2_TEX_SKIN0 + 6)
#define GL2_TEX_SKIN0_NAME						"s_Skin0Tex"
#define GL2_TEX_SKIN1_NAME						"s_Skin1Tex"
#define GL2_TEX_SKIN2_NAME						"s_Skin2Tex"
#define GL2_TEX_SKIN3_NAME						"s_Skin3Tex"
#define GL2_TEX_SKIN4_NAME						"s_Skin4Tex"
#define GL2_TEX_SKIN5_NAME						"s_Skin5Tex"
#define GL2_TEX_SKIN6_NAME						"s_Skin6Tex"
*/

