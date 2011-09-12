#include "engine_afx.h"
#include "c_filehandle.h"

#include "c_filemanager.h"
#include "c_file.h"

//***************************************************************************
// Definitions
//***************************************************************************
E_API CFileHandle	FNULL;

//===========================================================================
// FileHandle
//===========================================================================

//---------------------------------------------------------------------------
CFileHandle::CFileHandle()
: m_pFile( NULL )
{
}

//---------------------------------------------------------------------------
CFileHandle::CFileHandle( CFile* pFile )
: m_pFile( pFile )
{
}

//---------------------------------------------------------------------------
CFileHandle::CFileHandle( const CFileHandle& rhs )
: m_pFile( rhs.m_pFile )
{
}

//---------------------------------------------------------------------------
CFileHandle&
CFileHandle::operator=( const CFileHandle& rhs )
{
	m_pFile = rhs.m_pFile;
	return *this;
}

//---------------------------------------------------------------------------
CFileHandle::~CFileHandle()
{
	Close();
}

//---------------------------------------------------------------------------
bool
CFileHandle::IsOpen() const
{
	return (m_pFile != NULL);
}

//---------------------------------------------------------------------------
void
CFileHandle::Close()
{
	FileManager.CloseFile( m_pFile );
	m_pFile = NULL;
}

//---------------------------------------------------------------------------
CFile*
CFileHandle::GetFile() const
{
	return m_pFile;
}

//---------------------------------------------------------------------------
const string&
CFileHandle::GetPath() const
{
	if ( !IsOpen() )
		return SNULL;

	return m_pFile->GetPath();
}

//---------------------------------------------------------------------------
char*
CFileHandle::GetFileMem() const
{
	if ( !IsOpen() )
		return NULL;

	return m_pFile->GetFileMem();
}

//---------------------------------------------------------------------------
PxU64
CFileHandle::GetSize()
{
	if ( !IsOpen() )
		return 0;

	return m_pFile->GetSize();
}

//---------------------------------------------------------------------------
PxU64
CFileHandle::GetPos()
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return 0;

	return m_pFile->GetPos();
}

//---------------------------------------------------------------------------
void
CFileHandle::Seek( PxU64 uiPos )
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	m_pFile->Seek( uiPos );
}

//---------------------------------------------------------------------------
void
CFileHandle::Read( void* pDst, PxU32 uiNumBytes )
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	return m_pFile->Read( pDst, uiNumBytes );
}

//---------------------------------------------------------------------------
void
CFileHandle::Write( const void* pSrc, PxU32 uiNumBytes )
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	return m_pFile->Write( pSrc, uiNumBytes );
}
