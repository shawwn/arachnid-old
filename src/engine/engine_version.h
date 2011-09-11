#pragma once

//***************************************************************************
// Engine Version
//***************************************************************************
#define	ENGINE_MAJOR_VERSION		0
#define	ENGINE_MINOR_VERSION		0
#define	ENGINE_MICRO_VERSION		1
#define	ENGINE_HOTFIX_VERSION		0

#define MAKE_VERSION_STRING2( major, minor, micro, hotfix )	\
	PP_EVAL(#major) "." PP_EVAL(#minor) "." PP_EVAL(#micro) "." PP_EVAL(#hotfix)

#define MAKE_VERSION_STRING( major, minor, micro, hotfix )	\
	MAKE_VERSION_STRING2( major, minor, micro, hotfix )

#define ENGINE_VERSION_STRING		\
	MAKE_VERSION_STRING(ENGINE_MAJOR_VERSION, ENGINE_MINOR_VERSION, ENGINE_MICRO_VERSION, ENGINE_HOTFIX_VERSION)
