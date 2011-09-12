#include "engine_afx.h"
#include "c_filemanager.h"

#include "c_file.h"

SINGLETON_INIT( CFileManager );

//***************************************************************************
// Declarations
//***************************************************************************
typedef map<string,	CFileHandle>	PathToFileMap;

//===========================================================================
// FileFind - a 3rd party snippet to find a file by wildcard.
//===========================================================================
#ifdef WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# define MAXNAME 512
#endif

class FileFind
{
public:
	FileFind::FileFind(const char *dirname,const char *spec)
	{
#ifdef WIN32
		bPending = 0;
		hFindNext = INVALID_HANDLE_VALUE;
#endif

		if ( dirname && strlen(dirname) )
			sprintf(mSearchName,"%s\\%s",dirname,spec);
		else
			sprintf(mSearchName,"%s",spec);
	}

	FileFind::~FileFind(void)
	{
	}


	bool FindFirst(char *name)
	{
		bool ret=false;

#ifdef WIN32
		hFindNext = FindFirstFileA(mSearchName, &finddata);
		if ( hFindNext == INVALID_HANDLE_VALUE )
			ret =  false;
		else
		{
			bPending = 1; // have an initial file to check.
			ret =  FindNext(name);
		}
#endif
#ifdef LINUX_GENERIC
		mDir = opendir(".");
		ret = FindNext(name);
#endif
		return ret;
	}

	bool FindNext(char *name)
	{
		bool ret = false;

#ifdef WIN32
		if ( hFindNext != INVALID_HANDLE_VALUE )
		{
			if ( bPending )
			{
				bPending = 0;
				if ( IsValid( finddata ) )
				{
					strncpy(name,finddata.cFileName,MAXNAME);
					ret = true;
				}
			}

			if ( !bPending )
			{
				if ( FindNextFileA(hFindNext, &finddata) )
				{
					bPending = 1;
				}
				else
				{
					FindClose(hFindNext);
					hFindNext = INVALID_HANDLE_VALUE;
				}
			}
		}
#endif

#ifdef LINUX_GENERIC

		if ( mDir )
		{
			while ( 1 )
			{

				struct direct *di = readdir( mDir );

				if ( !di )
				{
					closedir( mDir );
					mDir = 0;
					ret = false;
					break;
				}

				if ( strcmp(di->d_name,".") == 0 || strcmp(di->d_name,"..") == 0 )
				{
					// skip it!
				}
				else
				{
					strncpy(name,di->d_name,MAXNAME);
					ret = true;
					break;
				}
			}
		}
#endif
		return ret;
	}

private:

#ifdef WIN32
	bool IsValid( const WIN32_FIND_DATAA& found )
	{
		E_UNREF_PARAM(found);
		if ( (finddata.cFileName[0] != '.') && !(finddata.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) )
			return true;
		else
			return false;
	}
#endif

	char mSearchName[MAXNAME];

#ifdef WIN32
	WIN32_FIND_DATAA finddata;
	HANDLE hFindNext;
	PxI32 bPending;
#endif
#ifdef LINUX_GENERIC
	DIR      *mDir;
#endif
};

//===========================================================================
// FileManager - Private State
//===========================================================================
class CFileManager_impl
{
public:
	~CFileManager_impl();
	CFileManager_impl();

	string	sPathToRootDir;
	string	sWorkingDir;
	string	sExeDir;

	PathToFileMap	mapFiles;
};

//---------------------------------------------------------------------------
CFileManager_impl::~CFileManager_impl()
{
}

//---------------------------------------------------------------------------
CFileManager_impl::CFileManager_impl()
{
}


//===========================================================================
// FileManager
//===========================================================================

//---------------------------------------------------------------------------
CFileManager::CFileManager()
: E_IMPL_NEW(CFileManager)
{
}

//---------------------------------------------------------------------------
CFileManager::~CFileManager()
{
	E_IMPL_DEL(CFileManager);
}

//---------------------------------------------------------------------------
CFileHandle&
CFileManager::OpenFile( const string& sDirtyPath, const char* sMode /*= "rb" */ )
{
	// normalize the path.
	string sPath;
	NormalizePath( sPath, sDirtyPath );

	// if we've already opened that file, then return a handle to it.
	{
		PathToFileMap::iterator itFind( m.mapFiles.find(sPath) );
		if ( itFind != m.mapFiles.end() )
		{
			CFileHandle& hFile( itFind->second );
			{
				// hmm, not sure about this.  For now, don't store FileHandles persistently,
				// or access them concurrently.
				hFile.Seek(0);
			}
			return hFile;
		}
	}

	// try to open the file.
	FILE* fp;
	{
		// build the final pathname for the OS...
		string sSystemPath;
		GetSystemPath( sSystemPath, sPath );

		// ... and open it.
		fp = fopen( sSystemPath.c_str(), sMode );
		if ( !fp )
		{
			E_ASSERT( !"Failed to open file." );
			return FNULL;
		}
	}

	// determine the file's size.
	PxU64 uiFileSize;
	{
		fseek( fp, 0, SEEK_END );
		uiFileSize = ftell( fp );
		fseek( fp, 0, SEEK_SET );
	}

	// if the file is over 4GB, don't allow it to be opned for now.
	if ( uiFileSize > 0xFFFFFFFF )
	{
		E_ASSERT( !"File too large." );
		fclose( fp );
		return FNULL;
	}
	
	// Note that the file is appended with zeroes in order to automatically 
	// NULL-terminate it, as a convenience.
	//
	// (4 bytes ensures that it could not possibly ever behave strangely
	// regardless of whether it's UTF-8, UTF-16, or UTF-32.)
	const PxU32 kNullTerminatorSize( 4 );

	// read the entire file into memory, for now.
	char* pMem = (char*)MemAlloc( (size_t)(uiFileSize + kNullTerminatorSize) );
	size_t uiBytesRead = fread( pMem, 1, (size_t)uiFileSize, fp );
	E_ASSERT( uiBytesRead == (size_t)uiFileSize );  E_UNREF_PARAM( uiBytesRead );

	// close it.
	fclose(fp);

	// NULL-terminate it.
	MemZero( &pMem[uiFileSize], kNullTerminatorSize );

	// wrap it.
	CFile* pFile( CFile::NewFromMemory( sPath, pMem, uiFileSize, true ) );

	// return a handle to it.
	CFileHandle& hFile( m.mapFiles[sPath] );
	E_ASSERT( hFile.m_pFile == NULL );
	hFile.m_pFile = pFile;
	return hFile;
}

//---------------------------------------------------------------------------
void
CFileManager::CloseFile( CFile*& pFile )
{
	if ( !pFile )
		return;

	E_DELETE( pFile );
	pFile = NULL;
}

//---------------------------------------------------------------------------
PxU32
CFileManager::FindFiles( StringList& vResults, const string& sDirtyPath, const string& sPattern )
{
	size_t uiPrevResultsCount( vResults.size() );

	// sanitize + normalize the directory.
	string sPath;
	GetSystemPath( sPath, sDirtyPath );

	// search for the files in directory 'sPath' which match the wildcard pattern 'sPattern'
	FileFind ff( sPath.c_str(), sPattern.c_str() );
	{
		// for each result...
		char sName[MAXNAME];
		if ( ff.FindFirst( sName ) )
		{
			do 
			{
				// normalize and append it to the results list.
				vResults.push_back(sName);
				NormalizePath( vResults.back() );

				// then search for the next file.
			} while ( ff.FindNext(sName) );
		}
	}

	if ( vResults.size() <= uiPrevResultsCount )
		return 0;
	else
		return (PxU32)(vResults.size() - uiPrevResultsCount);
}

//---------------------------------------------------------------------------
void
CFileManager::NormalizePath( string& sOutPath, const string& sPath )
{
	sOutPath.clear();

	if ( sPath.empty() )
		return;

	// determine whether we end with a slash.
	bool bEndsWithSlash(false);
	if ( (*sPath.rbegin() == '/') || (*sPath.rbegin() == '\\') )
		bEndsWithSlash = true;

	// determine the starting protocol.
	string sProtocol;
	{
		const bool IGNORE_CASE(true);
		if ( sPath[0] == '/' || sPath[0] == '\\' )
			sProtocol = '/';
		else if ( StartsWith( sPath, "http://", IGNORE_CASE ) )
			sProtocol = "http://";
		else if ( StartsWith( sPath, "ftp://", IGNORE_CASE ) )
			sProtocol = "ftp://";
		else if ( (sPath.length() >= 3) && (sPath[1] == ':') && (sPath[2] == '/' || sPath[2] == '\\') )
		{
			sProtocol = sPath.substr(0, 3);
			sProtocol[0] = (char)tolower(sProtocol[0]);
			sProtocol[2] = '/';
		}
	}

	StringList vSubdirs;
	PxU32 uiSubdirsCount = SplitBy( vSubdirs, sPath, "/\\", (PxU32)sProtocol.size() );

	sOutPath = sProtocol;
	for ( PxU32 i = 0; i < uiSubdirsCount; ++i )
	{
		if ( i > 0 )
			sOutPath += "/";
		sOutPath += vSubdirs[i];
	}
	if ( bEndsWithSlash )
		sOutPath += "/";
}

//---------------------------------------------------------------------------
string
CFileManager::NormalizePath( const string& sPath )
{
	string sResult;
	NormalizePath( sResult, sPath );
	return sResult;
}

//---------------------------------------------------------------------------
void
CFileManager::SetWorkingDir( const string& sWorkingDir )
{
	m.sWorkingDir = sWorkingDir;
	if ( m.sWorkingDir.empty() )
		return;

	// if working dir is not empty, then it should be relative to root,
	// and
}

//---------------------------------------------------------------------------
void
CFileManager::SetExeDir( const string& sExeDir )
{
	m.sExeDir = sExeDir;
}

//---------------------------------------------------------------------------
void
CFileManager::SetPathToRootDir( const string& sPathToRootDir )
{
	m.sPathToRootDir = sPathToRootDir;

	// ensure that it doesn't end with a slash.
	if ( EndsWith(m.sPathToRootDir, "/") )
	{
		m.sPathToRootDir = m.sPathToRootDir.substr(0, (m.sPathToRootDir.size() - 1));
	}
}

//---------------------------------------------------------------------------
const string&
CFileManager::GetWorkingDir() const
{
	return m.sWorkingDir;
}

//---------------------------------------------------------------------------
const string&
CFileManager::GetExeDir() const
{
	return m.sExeDir;
}

//---------------------------------------------------------------------------
const string&
CFileManager::GetPathToRootDir() const
{
	return m.sPathToRootDir;
}

//---------------------------------------------------------------------------
void
CFileManager::GetSystemPath( string& sOutPath, const string& sNormalizedPath )
{
	if ( sNormalizedPath.empty() )
		return;

	// determine whether this is an absolute path, such as "c:/foo/asdf.txt"
	//	or "http://www.xkcd.com"
	bool bAbsolute = false;
	{
		const bool IGNORE_CASE (true);

		if ( sOutPath[0] == '/' || sOutPath[0] == '\\' )
			bAbsolute = false;
		else if ( StartsWith( sOutPath, "http://", IGNORE_CASE ) )
			bAbsolute = true;
		else if ( StartsWith( sOutPath, "ftp://", IGNORE_CASE ) )
			bAbsolute = true;
		else if ( (sOutPath.length() >= 3) && (sOutPath[1] == ':') && (sOutPath[2] == '/' || sOutPath[2] == '\\') )
			bAbsolute = true;
	}

	//
	// if we're not an absolute path, then assume the OS's current directory is
	// our exe's directory.  And since we want the path to be relative to our root,
	// we need to prepend our 'sPathToRootDir' path.
	//
	// example: let's say...
	//	- 'sPath' is "/game/sounds/shatter.wav"
	//	- our exe dir (and hence the OS current directory) is "/bin/win32/"
	//
	// so, "/game/sounds/shatter.wav" is relative to our root game dir, but the OS is
	// relative to "/bin/win32/"; therefore we prepend our 'sPathToRootDir' path
	// to the incoming path, resulting in our return value:
	//
	//		"../../game/sounds/shatter.wav"
	//
	// (again, to re-iterate: since the operating system is relative to e.g.
	// "c:/arachnid/bin/win32/" then the OS will concatenate them and resolve
	// to our final, desired path: "c:/arachnid/game/sounds/shatter.wav")
	//
	if ( !bAbsolute )
	{
		sOutPath = GetPathToRootDir() + sNormalizedPath;
	}
}
