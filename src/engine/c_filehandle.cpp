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
: m_pFile( NULL )
{
	Acquire( rhs );
}

//---------------------------------------------------------------------------
CFileHandle&
CFileHandle::operator=( const CFileHandle& rhs )
{
	Acquire( rhs );
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
	return (m_pFile != NULL) && (m_pFile->IsOpen());
}

//---------------------------------------------------------------------------
void
CFileHandle::Close() const
{
	if ( m_pFile )
	{
		m_pFile->Close();
		m_pFile = NULL;
	}
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
CFileHandle::GetSize() const
{
	if ( !IsOpen() )
		return 0;

	return m_pFile->GetSize();
}

//---------------------------------------------------------------------------
PxU64
CFileHandle::GetPos() const
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return 0;

	return m_pFile->GetPos();
}

//---------------------------------------------------------------------------
void
CFileHandle::Seek( PxU64 uiPos ) const
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	m_pFile->Seek( uiPos );
}

//---------------------------------------------------------------------------
void
CFileHandle::Read( void* pDst, PxU32 uiNumBytes ) const
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	return m_pFile->Read( pDst, uiNumBytes );
}

//---------------------------------------------------------------------------
void
CFileHandle::Write( const void* pSrc, PxU32 uiNumBytes ) const
{
	E_ASSERT( IsOpen() );
	if ( !IsOpen() )
		return;

	return m_pFile->Write( pSrc, uiNumBytes );
}

//---------------------------------------------------------------------------
void
CFileHandle::Acquire( const CFileHandle& rhs ) const
{
	// don't acquire from ourselves!
	if ( rhs.m_pFile == m_pFile )
		return;

	// transfer ownership from rhs.
	{
		if ( m_pFile != NULL )
			Close();

		m_pFile = rhs.m_pFile;
		rhs.m_pFile = NULL;
	}
}
