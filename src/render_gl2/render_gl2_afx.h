#pragma once

//***************************************************************************
// Precompiled GL2Renderer headers.
//***************************************************************************
#include "engine/engine_afx.h"

// precompile the GL headers (thus requiring windows.h).
#ifdef WIN32
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#endif
// glew headers.
#include <gl/glew.h>
#ifdef WIN32
#  include <gl/wglew.h>
#endif
// GL headers.
#include <GL/gl.h>
#include <GL/glext.h>
#ifdef WIN32
#include <GL/wglext.h>
#endif

// precompile our renderer headers.
#include "render_gl2/gl2_config.h"
#include "render_gl2/gl2_types.h"

//***************************************************************************
// Declarations
//***************************************************************************

// rather than passing 0 to GL functions, represent a "NULL GL handle" for clarity.
const GLuint	GLNULL(0);

//***************************************************************************
// Utilities
//***************************************************************************
extern void	_DbgCheckGL( const char* file, long line );

#ifdef _DEBUG
 #if 1
  #define CHECK_GL()		_DbgCheckGL( __FILE__, __LINE__ )
 #else
  #define CHECK_GL()
 #endif
#else
 #define CHECK_GL()
#endif

//***************************************************************************
// DLL export/import helper.
//***************************************************************************
#ifdef RENDER_GL2_EXPORTS
#  define R_API			__declspec( dllexport )
#else
#  define R_API			__declspec( dllimport )
#endif
