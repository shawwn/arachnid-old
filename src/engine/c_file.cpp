#include "engine_afx.h"
#include "c_file.h"

#include "engine/c_filemanager.h"

//***************************************************************************
// Declarations
//***************************************************************************

//===========================================================================
// File - Private Implementation
//===========================================================================
class CFile_impl
{
public:
	~CFile_impl();
	CFile_impl();

	// the sanitized path name.
	string			sPath;

	// the size of the file.
	PxU64			uiSize;

	// the read/write position of the file.
	PxU64			uiPos;

	// the file, in memory for now.
	char*			pMem;
	bool			bOwnMem;
};

//---------------------------------------------------------------------------
CFile_impl::~CFile_impl()
{
	if ( bOwnMem )
		MemFree( pMem );
}

//---------------------------------------------------------------------------
CFile_impl::CFile_impl()
: uiSize( 0 )
, uiPos( 0 )
, pMem( NULL )
, bOwnMem( false )
{
}

//===========================================================================
// File
//===========================================================================

//---------------------------------------------------------------------------
CFile::~CFile()
{
	E_IMPL_DEL(CFile);
}

//---------------------------------------------------------------------------
CFile::CFile()
: E_IMPL_NEW(CFile)
{
}

//---------------------------------------------------------------------------
CFile*
CFile::NewFromMemory( const string& sCleanPath, char* pMem, PxU64 uiMemSize, bool bOwnMem )
{
	CFile* pFile = E_NEW(CFile);
	pFile->m.sPath = sCleanPath;
	pFile->m.pMem = pMem;
	pFile->m.uiSize = uiMemSize;
	pFile->m.bOwnMem = bOwnMem;
	return pFile;
}

//---------------------------------------------------------------------------
const string&
CFile::GetPath() const
{
	return m.sPath;
}

//---------------------------------------------------------------------------
char*
CFile::GetFileMem() const
{
	return m.pMem;
}

//---------------------------------------------------------------------------
PxU64
CFile::GetSize()
{
	return m.uiSize;
}

//---------------------------------------------------------------------------
PxU64
CFile::GetPos()
{
	return m.uiPos;
}

//---------------------------------------------------------------------------
void
CFile::Seek( PxU64 uiPos )
{
	E_ASSERT( uiPos <= GetSize() );
	m.uiPos = Min( uiPos, GetSize() );
}

//---------------------------------------------------------------------------
void
CFile::Read( void* pDst, PxU32 uiNumBytes )
{
	E_ASSERT( (GetPos() + uiNumBytes) <= GetSize() );
	E_ASSERT( GetPos() <= GetSize() );

	byte* pSrc		= (byte*)GetFileMem();
	PxU64 uiSrc		= Min( GetSize(), GetPos() );
	PxU64 uiSrcEnd	= Min( GetSize(), GetPos() + uiNumBytes );
	PxU64 uiCount	= (uiSrcEnd - uiSrc);
	E_ASSERT( uiSrcEnd >= uiSrc );

	// read the data.
	MemCopy( pDst, (pSrc + uiSrc), (size_t)uiCount );;

	// advance the read pointer.
	m.uiPos += uiCount;
}

//---------------------------------------------------------------------------
void
CFile::Write( const void* pSrc, PxU32 uiNumBytes )
{
	E_UNREF_PARAM2( pSrc, uiNumBytes );
	E_ASSERT( !"todo." );
}
