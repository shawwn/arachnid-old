#include "engine_afx.h"
#include "engine/c_system.h"

// Windows headers.
#pragma warning( disable: 4201 )
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <shlwapi.h>
#include <direct.h>
#pragma comment( lib, "winmm.lib" )

// engine headers.
#include "engine/c_filemanager.h"

// forward declarations.
void	Sys_ChangeToExeDir();
void	Sys_GetExePath( char* sOutPath, PxU32 uiMaxSize );
void	Sys_GetExeDir( char* sOutPath, PxU32 uiMaxSize );
bool	UTF8_to_UCS2( wstring& sOut, const string& pStr );
bool	UCS2_to_UTF8( string& sOut, const wstring& pStr );

//***************************************************************************
// Windows implementation of System.
//***************************************************************************
SINGLETON_INIT( CSystem );

struct SLibInfo;

typedef map<string, SLibInfo>					PathToLibMap;
typedef map<void*, PathToLibMap::iterator>		LibToPathMap;
typedef vector< pair<OnExitCallback, void*> >	OnExitCallbackList;

//===========================================================================
// SLibInfo - to track handles to DLLs we've loaded, including refcount.
//===========================================================================
struct SLibInfo
{
	SLibInfo();
	SLibInfo( void* hHandle, PxI32 iRefCount = 1 );

	void*	hHandle;
	PxI32	iRefCount;
};

//---------------------------------------------------------------------------

SLibInfo::SLibInfo()
: hHandle(NULL)
, iRefCount(0)
{
}

//---------------------------------------------------------------------------
SLibInfo::SLibInfo( void* hHandle, PxI32 iRefCount /*= 1 */ )
: hHandle( hHandle )
, iRefCount( iRefCount )
{

}

//===========================================================================
// System - Private State
//===========================================================================
class CSystem_impl
{
public:
	CSystem_impl();

	PathToLibMap		mapPathToLib;
	LibToPathMap		mapLibToPath;
	string				sAbsolutePathToRootDir;
	OnExitCallbackList	vOnExitCallbacks;
	bool				bInitialized;
	bool				bExiting;
};

//---------------------------------------------------------------------------
CSystem_impl::CSystem_impl()
: bInitialized( false )
, bExiting( false )
{
}

//===========================================================================
// System
//===========================================================================

//---------------------------------------------------------------------------
CSystem::CSystem()
: E_IMPL_NEW(CSystem)
{
}

//---------------------------------------------------------------------------
CSystem::~CSystem()
{
	E_IMPL_DEL(CSystem);
}

//---------------------------------------------------------------------------
void
CSystem::Init( int argc, char** argv )
{
	E_UNREF_PARAM2( argc, argv );

	// verify we haven't already been initialized.
	E_ASSERT( !m.bInitialized );
	if ( m.bInitialized )
		return;
	m.bInitialized = true;

	// switch the working directory to our exe's dir.
	Sys_ChangeToExeDir();

	// parse our exe directory and root directory.
	{
		char sExeDir[1024] = {0};
		Sys_GetExeDir( sExeDir, 1024 );
		char* sTail = strstr( sExeDir, "\\bin\\" );
		sTail = sTail ? sTail : strstr( sExeDir, "/bin/" );
		if ( sTail )
		{
			FileManager.SetPathToRootDir( "../../" );
			FileManager.SetExeDir( FileManager.NormalizePath( sTail ) );
			sTail[1] = '\0';
			m.sAbsolutePathToRootDir = FileManager.NormalizePath( sExeDir );
		}
	}
}

//---------------------------------------------------------------------------
void
CSystem::Exit( int iCode /*= 0 */ )
{
	// prevent recursive exiting.
	if ( m.bExiting )
		return;
	m.bExiting = true;

	// notify everyone that we're exiting.
	for ( PxU32 i = 0; i < (PxU32)m.vOnExitCallbacks.size(); ++i )
	{
		OnExitCallback	pCallback = m.vOnExitCallbacks[i].first;
		void*			pUserdata = m.vOnExitCallbacks[i].second;

		E_ASSERT( pCallback );
		if ( pCallback )
			pCallback( pUserdata );
	}

	// now exit.
	exit( iCode );
}

//---------------------------------------------------------------------------
void
CSystem::NotifyOnExit( OnExitCallback pCallback, void* pUserdata )
{
	if ( !pCallback )
		return;

	// if the callback is already in the list, then skip it.
	for ( PxU32 i = 0; i < (PxU32)m.vOnExitCallbacks.size(); ++i )
	{
		if ( pCallback == m.vOnExitCallbacks[i].first )
			return;
	}

	m.vOnExitCallbacks.push_back( make_pair( pCallback, pUserdata ) );
}

//---------------------------------------------------------------------------
void*
CSystem::LoadLib( const string& sDirtyPath )
{
	// sanitize the path.
	string sPath;
	FileManager.GetSystemPath( sPath, sDirtyPath );

	// try to load the DLL.
	void* hLib = NULL;
	{
		// convert to a windows-native string.
		wstring sUCS2Path;
		if ( UTF8_to_UCS2( sUCS2Path, sPath ) )
			hLib = LoadLibrary( sUCS2Path.c_str() );
	}

	// if the DLL failed to load, then abort.
	if ( !hLib )
		return NULL;

	// update our bookkeeping.
	PathToLibMap::iterator itExists( m.mapPathToLib.find(sPath) );
	if ( itExists == m.mapPathToLib.end() )
	{
		// freshly loaded; refcount will be 1.
		itExists = m.mapPathToLib.insert( make_pair(sPath, SLibInfo(hLib)) ).first;
	}
	else
	{
		// already loaded; increment our refcount
		SLibInfo& cInfo( itExists->second );
		cInfo.iRefCount += 1;
	}

	// index by library pointer, to map from (DLL handle) -> (DLL pathname).
	m.mapLibToPath[hLib] = itExists;

	// return the DLL handle.
	return hLib;
}

//---------------------------------------------------------------------------
void*
CSystem::FindLib( const string& sDirtyPath )
{
	// sanitize the path.
	string sPath;
	FileManager.GetSystemPath( sPath, sDirtyPath );

	// check whether it's already loaded.
	PathToLibMap::const_iterator itFind( m.mapPathToLib.find(sPath) );
	if ( itFind == m.mapPathToLib.end() )
		return NULL;

	// return its existing handle.
	const SLibInfo& cInfo( itFind->second );
	return cInfo.hHandle;
}

//---------------------------------------------------------------------------
void
CSystem::UnloadLib( void* hLib )
{
	if ( !hLib )
		return;

	// check whether it's already loaded.
	LibToPathMap::iterator itFind( m.mapLibToPath.find(hLib) );
	if ( itFind == m.mapLibToPath.end() )
	{
		E_ASSERT( !"WARNING: Tried to unload an unknown DLL pointer" );
		return;
	}

	// get the DLL info.
	const string&	sLibPath( (itFind->second)->first );
	SLibInfo&		cLibInfo( (itFind->second)->second );
	E_UNREF_PARAM2( sLibPath, cLibInfo );

	// free the library.
	FreeLibrary( (HMODULE)cLibInfo.hHandle );

	// decrement the lib's refcount.
	E_ASSERT( cLibInfo.iRefCount >= 1 );
	cLibInfo.iRefCount -= 1;

	// if the refcount just reached zero, then the above FreeLibrary
	// call has deleted the DLL from memory;  therefore we need to
	// delete it from our bookkeeping structures.
	if ( cLibInfo.iRefCount <= 0 )
	{
		m.mapPathToLib.erase( itFind->second );
		m.mapLibToPath.erase( itFind );
	}
}

//---------------------------------------------------------------------------
void*
CSystem::GetProcAddr( void* hLib, const char* sFuncName )
{
	void* pResult = ::GetProcAddress( (HMODULE)hLib, sFuncName );
	return pResult;
}

//===========================================================================
// Global static initialization.
//===========================================================================

//---------------------------------------------------------------------------
void	Sys_GlobalStaticInit()
{
	static bool bOnce = true;
	if ( !bOnce )
		return;
	bOnce = false;

	// do memory allocator initialization, etc.
}

//---------------------------------------------------------------------------
void	Sys_GetExePath( char* sOutPath, PxU32 uiMaxSize )
{
	GetModuleFileNameA( 0, sOutPath, uiMaxSize );
}

//---------------------------------------------------------------------------
void	Sys_GetExeDir( char* sOutPath, PxU32 uiMaxSize )
{
	GetModuleFileNameA( 0, sOutPath, uiMaxSize );
	PathRemoveFileSpecA( sOutPath );

	size_t uiOutLen = strlen(sOutPath);
	if ( uiOutLen > 0 )
	{
		char cLast = sOutPath[uiOutLen - 1];
		if ( cLast != '/' && cLast != '\\' )
		{
			sOutPath[uiOutLen] = '\\';
			sOutPath[uiOutLen + 1] = '\0';
		}
	}
}

//---------------------------------------------------------------------------
void	Sys_ChangeToExeDir()
{
	char vcExePath[1024] = {0};
	char vcExe[1024];

	GetModuleFileNameA( 0, vcExePath, sizeof( vcExePath ) );
	strcpy_s( vcExe, vcExePath );

	PathRemoveFileSpecA( vcExePath );

	size_t len = strlen( vcExePath );
	const char *exeName = &vcExe[len];
	if ( exeName[0] == '\\' || exeName[0] == '/' )
		exeName++;

	if( vcExePath[0] )
	{
		_chdir( vcExePath );
	}
}

//---------------------------------------------------------------------------
bool
UTF8_to_UCS2( wstring& sOut, const string& sInput )
{
	// handle empty string.
	if ( sInput.empty() )
	{
		sOut.clear();
		return true;
	}

	// determine the required number of characters (NOT including the null-terminator).
	int iRequiredCount = MultiByteToWideChar( CP_UTF8, 0,
											sInput.c_str(), (int)sInput.length(),
											NULL, 0 );

	// abort if we failed to convert.
	E_ASSERT( iRequiredCount > 0 );
	if ( iRequiredCount <= 0 )
		return false;

	// resize the string to the required size.
	sOut.resize( (size_t)iRequiredCount );

	// translate the string.
	iRequiredCount = MultiByteToWideChar( CP_UTF8, 0,
											sInput.c_str(), (int)sInput.length(),
											&sOut[0], (int)sOut.size() );

	E_ASSERT( iRequiredCount > 0 );
	return (iRequiredCount > 0);
}

//---------------------------------------------------------------------------
bool
UCS2_to_UTF8( string& sOut, const wstring& sInput )
{
	// handle empty string.
	if ( sInput.empty() )
	{
		sOut.clear();
		return true;
	}

	// determine the required number of characters (NOT including the null-terminator).
	int iRequiredCount = WideCharToMultiByte( CP_UTF8, 0,
											sInput.c_str(), (int)sInput.length(),
											NULL, 0,
											0, 0);

	// abort if we failed to convert.
	E_ASSERT( iRequiredCount > 0 );
	if ( iRequiredCount <= 0 )
		return false;

	// resize the string to the required size.
	sOut.resize( (size_t)iRequiredCount );

	// translate the string.
	iRequiredCount = WideCharToMultiByte( CP_UTF8, 0,
											sInput.c_str(), (int)sInput.length(),
											&sOut[0], (int)sOut.size(),
											0, 0 );

	E_ASSERT( iRequiredCount > 0 );
	return (iRequiredCount > 0);
}
