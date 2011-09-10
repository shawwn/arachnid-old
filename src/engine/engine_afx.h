#pragma once

#ifdef ENGINE_EXPORTS
#  define E_API			__declspec( dllexport )
#else
#  define E_API			__declspec( dllimport )
#endif
