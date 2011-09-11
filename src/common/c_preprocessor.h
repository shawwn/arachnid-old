#pragma once

//***************************************************************************
// Declarations
//***************************************************************************

#ifndef E_SWIG

/**
Compiler define
*/
#ifdef _MSC_VER 
#	define E_VC
#   if _MSC_VER >= 1500
#       define E_VC9
#	elif _MSC_VER >= 1400
#		define E_VC8
#	elif _MSC_VER >= 1300
#		define E_VC7
#	else
#		define E_VC6
#	endif
#elif __GNUC__ || __SNC__
#	define E_GNUC
#elif defined(__MWERKS__)
#	define	E_CW
#else
#	error "Unknown compiler"
#endif


/**
Platform define
*/
#ifdef E_VC
#	ifdef _M_IX86
#		define E_X86
#		define E_WINDOWS
#   elif defined(_M_X64)
#       define E_X64
#       define E_WINDOWS
#	elif defined(_M_PPC)
#		define E_PPC
#		define E_X360
#		define E_VMX
#	else
#		error "Unknown platform"
#	endif
#elif defined E_GNUC
#   ifdef __CELLOS_LV2__
#	define E_PS3
#   elif defined(__arm__)
#	define E_LINUX
#	define E_ARM
#   elif defined(__i386__)
#       define E_X86
#   elif defined(__x86_64__)
#       define E_X64
#   elif defined(__ppc__)
#       define E_PPC
#   elif defined(__ppc64__)
#       define E_PPC
#	define E_PPC64
#   else
#	error "Unknown platform"
#   endif
#	if defined(__linux__)
#   	define E_LINUX
#	elif defined(__APPLE__)
#   	define E_APPLE
#	elif defined(__CYGWIN__)
#   	define E_CYGWIN
#   	define E_LINUX
#	endif
#elif defined E_CW
#	if defined(__PPCGEKKO__)
#		if defined(RVL)
#			define E_WII
#		else
#			define E_GC
#		endif
#	else
#		error "Unknown platform"
#	endif
#endif


/**
DLL export macros
*/
#ifndef E_C_EXPORT
	#define E_C_EXPORT extern "C"
#endif

/**
Calling convention
*/
#ifndef E_CALL_CONV
#	if defined E_WINDOWS
#		define E_CALL_CONV __cdecl
#	else
#		define E_CALL_CONV
#	endif
#endif

/**
Pack macros
*/
#if defined(E_VC)
#	define E_PUSH_PACK_DEFAULT	__pragma( pack(push, 8) )
#	define E_POP_PACK			__pragma( pack(pop) )
#elif defined(E_GNUC)
#	define E_PUSH_PACK_DEFAULT	_Pragma("pack(push, 8)")
#	define E_POP_PACK			_Pragma("pack(pop)")
#else
#	define E_PUSH_PACK_DEFAULT
#	define E_POP_PACK
#endif

/**
Inline macro
*/
#if defined(E_WINDOWS) || defined(E_X360)
#	define E_INLINE inline
#	pragma inline_depth( 255 )
#else
#	define E_INLINE inline
#endif

/**
Force inline macro
*/
#if defined(E_VC)
	#define E_FORCE_INLINE __forceinline
#elif defined(E_LINUX) // Workaround; Fedora Core 3 do not agree with force inline 
	#define E_FORCE_INLINE inline
#elif defined(E_GNUC)
	#define E_FORCE_INLINE inline __attribute__((always_inline))
#else
	#define E_FORCE_INLINE inline
#endif

/**
Noinline macro
*/
#if defined E_WINDOWS
#	define E_NOINLINE __declspec(noinline)
#elif defined(E_GNUC)
#	define E_NOINLINE __attribute__ ((noinline))
#else
#	define E_NOINLINE 
#endif


/*! restrict macro */
#if defined(E_GNUC) || defined(E_VC)
#	define E_RESTRICT __restrict
#elif defined(E_CW) && __STDC_VERSION__ >= 199901L
#	define E_RESTRICT restrict
#else
#	define E_RESTRICT
#endif


/**
Alignment macros

E_ALIGN_PREFIX and E_ALIGN_SUFFIX can be used for type alignment instead of aligning individual variables as follows:
E_ALIGN_PREFIX(16)
struct A {
...
} E_ALIGN_SUFFIX(16);
This declaration style is parsed correctly by Visual Assist.

*/
#ifndef E_ALIGN
	#if defined(E_VC)
		#define E_ALIGN(alignment, decl) __declspec(align(alignment)) decl
		#define E_ALIGN_PREFIX(alignment) __declspec(align(alignment))
		#define E_ALIGN_SUFFIX(alignment)
	#elif defined(E_GNUC)
		#define E_ALIGN(alignment, decl) decl __attribute__ ((aligned(alignment)))
		#define E_ALIGN_PREFIX(alignment)
		#define E_ALIGN_SUFFIX(alignment) __attribute__ ((aligned(alignment)))
	#elif defined(E_CW)
		#define E_ALIGN(alignment, decl) decl __attribute__ ((aligned(alignment)))
		#define E_ALIGN_PREFIX(alignment)
		#define E_ALIGN_SUFFIX(alignment) __attribute__ ((aligned(alignment)))
	#else
		#define E_ALIGN(alignment, decl)
		#define E_ALIGN_PREFIX(alignment)
		#define E_ALIGN_SUFFIX(alignment)
	#endif
#endif

/**
Deprecated marco
*/
#if 0 // set to 1 to create warnings for deprecated functions
#	define E_DEPRECATED __declspec(deprecated)
#else 
#	define E_DEPRECATED
#endif

// VC6 no 'CASS_FUNCTION' workaround
#if defined E_VC6 && !defined CASS_FUNCTION
#	define CASS_FUNCTION	"Undefined"
#endif

/**
General defines
*/

// static assert
#define PP_JOIN_HELPER(X, Y)		X##Y
#define PP_JOIN(X, Y, Z)			PP_JOIN_HELPER(X, Y##Z)
#define PP_EVAL2(X)					X
#define PP_EVAL(X)					PP_EVAL2(X)
#define E_COMPILE_TIME_ASSERT(exp)	typedef char PP_JOIN( ECompileTimeAssert, __FILE__ , __COUNTER__ )[(exp) ? 1 : -1]

#ifdef E_GNUC
#define E_OFFSET_OF(X, Y) __builtin_offsetof(X, Y)
#else
#include <stddef.h>
#define E_OFFSET_OF(X, Y) offsetof(X, Y)
#endif

// avoid unreferenced parameter warning (why not just disable it?)
// PT: or why not just omit the parameter's name from the declaration????
#define E_FORCE_PARAMETER_REFERENCE(_P) (void)(_P);

// check that exactly one of NDEBUG and _DEBUG is defined
#if !(defined NDEBUG ^ defined _DEBUG)
	#error Exactly one of NDEBUG and _DEBUG needs to be defined by preprocessor
#endif

// make sure E_CHECKED is defined in all _DEBUG configurations as well
#if !defined(E_CHECKED) && _DEBUG
#define E_CHECKED
#endif

namespace PackValidation
{
	// Ensure that the application hasn't tweaked the pack value to less than 8, which would break
	// matching between the API headers and the binaries
	// This assert works on win32/win64/360/ps3, but may need further specialization on other platforms.
	// Some GCC compilers need the compiler flag -malign-double to be set.

	struct PackTest { char _; long long a; };
	E_COMPILE_TIME_ASSERT(E_OFFSET_OF(PackTest, a) == 8);
}

#endif
