#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CFile_impl;

//===========================================================================
// File
//===========================================================================
class E_API CFile
{
	// to open/close files, use FileManager.
	friend class CFileManager;
	virtual ~CFile();
	CFile();
public:

	// returns the sanitized pathname.
	const string&	GetPath() const;

	// a pointer to the file, in memory, and NULL-terminated.
	char*			GetFileMem() const;
	PxU64			GetSize();

	// quick'n'dirty I/O.
	PxU64			GetPos();
	void			Seek( PxU64 uiPos );
	void			Read( void* pDst, PxU32 uiNumBytes );
	void			Write( const void* pSrc, PxU32 uiNumBytes );

private:
	// internal FileManager-only interface.
	static CFile*	NewFromMemory( const string& sCleanPath, char* pMem, PxU64 uiMemSize, bool bOwnMem );

	E_IMPL( CFile );
};
