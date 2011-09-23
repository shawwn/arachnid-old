#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CFile;
class CFileHandle;

extern E_API CFileHandle	FNULL;

//===========================================================================
// FileHandle
//===========================================================================
class E_API CFileHandle
{
	// to open/close files, use FileManager.
	friend class CFileManager;
	CFileHandle( CFile* pFile );
public:
	~CFileHandle();
	CFileHandle();
	CFileHandle( const CFileHandle& rhs );
	CFileHandle&	operator = ( const CFileHandle& rhs );

	// returns whether we've opened successfully.
	bool			IsOpen() const;

	// closes the opened file.
	void			Close() const;

	// returns the underlying CFile pointer.
	CFile*			GetFile() const;

	// returns the sanitized pathname.
	const string&	GetPath() const;

	// a pointer to the file, in memory, and NULL-terminated.
	char*			GetFileMem() const;
	PxU64			GetSize() const;

	// quick'n'dirty I/O.
	PxU64			GetPos() const;
	void			Seek( PxU64 uiPos ) const;
	void			Read( void* pDst, PxU32 uiNumBytes ) const;
	void			Write( const void* pSrc, PxU32 uiNumBytes ) const;

private:
	void			Acquire( const CFileHandle& rhs ) const;

	mutable CFile*	m_pFile;
};

