#pragma once

#include "engine/c_filehandle.h"

//***************************************************************************
// Declarations
//***************************************************************************
class CFileManager_impl;

//===========================================================================
// FileManager
//===========================================================================
class E_API CFileManager
{
	SINGLETON_DEF( CFileManager );
public:
	~CFileManager();

	CFileHandle&	OpenFile( const string& sPath, const char* sMode = "rb" ); // todo: refactor 'sMode' into bitflags.
	void			CloseFile( CFile*& pFile );

	PxU32			FindFiles( StringList& vResults, const string& sPath, const string& sPattern );

	void			NormalizePath( string& sOutPath, const string& sPath );
	string			NormalizePath( const string& sPath );

	// Relative to root.
	void			SetWorkingDir( const string& sWorkingDir );
	// Relative to root.  (E.g. "/bin/win32/")
	void			SetExeDir( const string& sExeDir );
	// The root dir is relative to the operating system's current directory.
	//	(E.g. if the exe is located at "/bin/win32/arachnid.exe" then our
	//	root dir should be set to "../../")
	void			SetPathToRootDir( const string& sPathToRootDir ); 

	const string&	GetWorkingDir() const;
	const string&	GetExeDir() const;
	const string&	GetPathToRootDir() const;

private:
	// debating whether to make this public; hidden for now.
	friend class CSystem;
	void			GetSystemPath( string& sOutPath, const string& sNormalizedPath );

	// implementation is hidden from the header file, to avoid excessive recompilation.
	E_IMPL( CFileManager );
};

#define FileManager	(*CFileManager::GetInstance())
