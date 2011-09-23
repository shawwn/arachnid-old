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
// camera.
#define GL2_MODEL_VIEW_PROJECTION_MATRIX_NAME	"u_ModelViewProjectionMatrix"
#define GL2_VIEW_POS_NAME						"u_ViewPos"

// lighting.
#define GL2_LIGHT_POS_NAME						"u_LightPos"
#define GL2_LIGHT_COLOR_NAME					"u_LightColor"

// blurring.
#define GL2_BLUR_DIRECTION_NAME					"u_BlurDirection"
#define GL2_BLUR_WEIGHTS_NAME					"u_BlurWeights"
#define GL2_BLUR_OFFSETS_NAME					"u_BlurOffsets"
#define GL2_BLUR_CENTER_TAP_WEIGHT_NAME			"u_BlurCenterTapWeight"

// samplers
#define GL2_TEX_DIFFUSE							1
#define GL2_TEX_DIFFUSE_NAME					"s_DiffuseTex"
#define GL2_TEX_NORMAL							2
#define GL2_TEX_NORMAL_NAME						"s_NormalTex"
#define GL2_TEX_CURRENT							3
#define GL2_TEX_CURRENT_NAME					"s_CurrentTex"
#define GL2_TEX_DEPTH							4
#define GL2_TEX_DEPTH_NAME						"s_DepthTex"

