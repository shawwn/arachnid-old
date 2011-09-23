#pragma once

//***************************************************************************
// Globally disable some very annoying Microsoft compiler warnings.
// Note: I have thought about this carefully, and have determined that it
// should be safe for our purposes.
//***************************************************************************
#pragma warning( disable: 4996 )
#pragma warning( disable: 4505 )

//***************************************************************************
// Globally-shared precompiled headers.  These are pulled into every namespace.
//***************************************************************************
#include "common/c_preprocessor.h"
#include "common/c_simpletypes.h"
#include "common/c_singleton.h"
#include "common/c_memmanager.h"
#include "common/c_idioms.h"
#include "common/c_utilities.h"

// precompile STL.
#include "common/c_stl.h"
#include "common/c_string.h"
