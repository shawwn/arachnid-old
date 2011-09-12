#pragma once

//***************************************************************************
// DLL export/import helper.
//***************************************************************************
#ifdef EDITUTIL_EXPORTS
#  define ED_API		__declspec( dllexport )
#else
#  define ED_API		__declspec( dllimport )
#endif

//===========================================================================
// EditUtil
//===========================================================================

ED_API bool	Ed_Startup();
ED_API void	Ed_Shutdown();
