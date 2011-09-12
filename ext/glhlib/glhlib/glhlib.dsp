# Microsoft Developer Studio Project File - Name="glhlib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=glhlib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "glhlib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "glhlib.mak" CFG="glhlib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "glhlib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "glhlib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "glhlib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLHLIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /Gz /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLHLIB_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 opengl32.lib glu32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "glhlib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLHLIB_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /G6 /Gz /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "GLHLIB_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib opengl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "glhlib - Win32 Release"
# Name "glhlib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\3DGraphicsLibrarySmall.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib.def
# End Source File
# Begin Source File

SOURCE=.\glhlib.rc
# End Source File
# Begin Source File

SOURCE=.\glhlib10.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib2.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib3.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib4.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib5.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib6.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib8.cpp
# End Source File
# Begin Source File

SOURCE=.\glhlib9.cpp
# End Source File
# Begin Source File

SOURCE=.\MemoryManagement.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TBaseShape.cpp
# End Source File
# Begin Source File

SOURCE=.\TChamferCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\TCone.cpp
# End Source File
# Begin Source File

SOURCE=.\TCube.cpp
# End Source File
# Begin Source File

SOURCE=.\TCube2.cpp
# End Source File
# Begin Source File

SOURCE=.\TCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\TCylinder2.cpp
# End Source File
# Begin Source File

SOURCE=.\TSphere.cpp
# End Source File
# Begin Source File

SOURCE=.\TSphere2.cpp
# End Source File
# Begin Source File

SOURCE=.\TTorus.cpp
# End Source File
# Begin Source File

SOURCE=.\TTube.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\3DGraphicsLibrarySmall.h
# End Source File
# Begin Source File

SOURCE=.\ddsfilestuff.h
# End Source File
# Begin Source File

SOURCE=.\glhlib.h
# End Source File
# Begin Source File

SOURCE=.\global.h
# End Source File
# Begin Source File

SOURCE=.\GlobalsExternDeclarations.h
# End Source File
# Begin Source File

SOURCE=.\jitter.h
# End Source File
# Begin Source File

SOURCE=.\MathLibrary.h
# End Source File
# Begin Source File

SOURCE=.\MemoryManagement.h
# End Source File
# Begin Source File

SOURCE=.\ObjectsDefines.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringFunctions.h
# End Source File
# Begin Source File

SOURCE=.\TBaseShape.h
# End Source File
# Begin Source File

SOURCE=.\TChamferCylinder.h
# End Source File
# Begin Source File

SOURCE=.\TCone.h
# End Source File
# Begin Source File

SOURCE=.\TCube.h
# End Source File
# Begin Source File

SOURCE=.\TCylinder.h
# End Source File
# Begin Source File

SOURCE=.\TSphere.h
# End Source File
# Begin Source File

SOURCE=.\TTorus.h
# End Source File
# Begin Source File

SOURCE=.\TTube.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
