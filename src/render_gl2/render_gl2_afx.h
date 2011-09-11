#pragma once

//***************************************************************************
// Precompiled GL2Renderer headers.
//***************************************************************************
#include "engine/engine_afx.h"


//***************************************************************************
// DLL export/import helper.
//***************************************************************************
#ifdef RENDER_GL2_EXPORTS
#  define R_API			__declspec( dllexport )
#else
#  define R_API			__declspec( dllimport )
#endif
