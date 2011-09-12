#pragma once

//===========================================================================
// Precompiled Headers
//===========================================================================
#include "common/common_afx.h"
#include "engine/engine_afx.h"


//***************************************************************************
// DLL export/import helper.
//***************************************************************************
#ifdef GRPAHICS_EXPORTS
#  define GR_API			__declspec( dllexport )
#else
#  define GR_API			__declspec( dllimport )
#endif
