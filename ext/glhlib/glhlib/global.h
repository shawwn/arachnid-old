#ifndef _GLOBAL_2374_
#define _GLOBAL_2374_


#ifdef _M_IX86
#  define USE_INLINE_ASM		1
#else
#  define USE_INLINE_ASM		0
#endif


#define SILENT_GLOBAL


#ifndef FALSE
#define FALSE		0
#endif
#ifndef TRUE
#define TRUE		1
#endif

#ifndef NO
#define NO			0
#endif
#ifndef YES
#define YES			1
#endif



//PURPOSE:	Sometimes, I would want to copy a pointer to a regular variabled, then do some
//			arithmetic and then do whatever with the result. It is very rare that I do this in my
//			code. I know that in my VBO manager class, I do it.
//
//			EXAMPLE:
//			uchar *pointer=__somthing_here_;
//			uint thing;
//
//			memcpy(&thing, &pointer, sizeof(uint));		//Is &pointer correct?
//			//THE PROBLEM is that on 64 bit systems, thing really needs to be a 64 bit integer.
//			//So, it is better to write code like this :
//
//			uchar *pointer=__somthing_here_;
//			PPOINTER thing;
//
//			memcpy(&thing, &pointer, sizeof(PPOINTER));		//Is &pointer correct?
//			//Also, I would need to modify change the definition of PPOINTER to
//			//__int64 or something more generic for 64 bit platforms.
#ifdef PPOINTER
	#ifndef SILENT_GLOBAL
	#pragma message("PPOINTER defined as unsigned int (global.h)")
	#endif
	typedef unsigned int PPOINTER;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("PPOINTER already defined (global.h)")
	#endif
#endif

#ifndef schar
	#ifndef SILENT_GLOBAL
	#pragma message("schar defined as char (global.h)")
	#endif
	typedef char schar;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("schar already defined (global.h)")
	#endif
#endif

#ifndef uchar
	#ifndef SILENT_GLOBAL
	#pragma message("uchar defined as unsigned char (global.h)")
	#endif
	typedef unsigned char uchar;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("uchar already defined (global.h)")
	#endif
#endif

#ifndef tbool
	#ifndef SILENT_GLOBAL
	#pragma message("tbool defined as unsigned int (global.h)")
	#endif
	typedef unsigned int tbool;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("tbool already defined (global.h)")
	#endif
#endif

#ifndef ushort
	#ifndef SILENT_GLOBAL
	#pragma message("ushort defined as unsigned short (global.h)")
	#endif
	typedef unsigned short ushort;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("ushort already defined (global.h)")
	#endif
#endif

#ifndef sshort		//Signed 16 bit int
	#ifndef SILENT_GLOBAL
	#pragma message("sshort defined as signed short (global.h)")
	#endif
	typedef signed short sshort;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("sshort already defined (global.h)")
	#endif
#endif

#ifndef uint
	#ifndef SILENT_GLOBAL
	#pragma message("uint defined as unsigned int (global.h)")
	#endif
	typedef unsigned int uint;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("uint already defined (global.h)")
	#endif
#endif

#ifndef sint		//Signed 32 bit int
	#ifndef SILENT_GLOBAL
	#pragma message("sint defined as signed int (global.h)")
	#endif
	typedef signed int sint;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("sint already defined (global.h)")
	#endif
#endif

#ifndef ulong
	#ifndef SILENT_GLOBAL
	#pragma message("ulong defined as unsigned int (global.h)")
	#endif
	typedef unsigned long ulong;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("ulong already defined (global.h)")
	#endif
#endif

#ifndef slong		//Signed 32 bit int
	#ifndef SILENT_GLOBAL
	#pragma message("slong defined as signed int (global.h)")
	#endif
	typedef signed long slong;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("slong already defined (global.h)")
	#endif
#endif

#ifndef sint64		//Signed 64 bit int
	#ifndef SILENT_GLOBAL
	#pragma message("sint64 defined as __int64 (global.h)")
	#endif
	typedef __int64 sint64;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("sint64 already defined (global.h)")
	#endif
#endif

#ifndef sreal
	#ifndef SILENT_GLOBAL
	#pragma message("sreal defined as float (global.h)")
	#endif
	typedef float sreal;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("sreal already defined (global.h)")
	#endif
#endif

#ifndef sreal2
	#ifndef SILENT_GLOBAL
	#pragma message("sreal2 defined as double (global.h)")
	#endif
	typedef double sreal2;
#else
	#ifndef SILENT_GLOBAL
	#pragma message("sreal2 already defined (global.h)")
	#endif
#endif

#endif		//_GLOBAL_2374_


