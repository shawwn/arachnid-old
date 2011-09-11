#pragma once

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

	void			NormalizePath( string& sOutPath, const string& sPath );
	string			NormalizePath( const string& sPath );

	void			GetSystemPath( string& sOutPath, const string& sPath );
	string			GetSystemPath( const string& sPath );

	PxU32			FindFiles( StringList& vResults, const string& sPath, const string& sPattern );

private:
	// implementation is hidden from the header file, to avoid excessive recompilation.
	E_IMPL_DECL( CFileManager );
};

#define FileManager	(*CFileManager::GetInstance())
