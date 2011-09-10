#pragma once

//***************************************************************************
// Precompiled Engine headers.
//***************************************************************************
#include "common/common_afx.h"

// declare the reshandle here, so that we don't have to include "c_resourcecommon.h"
// at this point.  (If we included it, then the whole project would need to be 
// rebuilt each time someone added a new type of resource.)
typedef PxU32			ResHandle;
const ResHandle			INVALID_RESOURCE(0);

//***************************************************************************
// DLL export/import helper.
//***************************************************************************
#ifdef ENGINE_EXPORTS
#  define E_API			__declspec( dllexport )
#else
#  define E_API			__declspec( dllimport )
#endif
