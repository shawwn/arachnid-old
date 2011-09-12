#include "graphics_afx.h"
#include "c_image.h"

#include "engine/c_filemanager.h"
#include "engine/c_file.h"

#include "FreeImage.h"

//***************************************************************************
// Declarations
//***************************************************************************

// FreeImage I/O callbacks.
inline unsigned DLL_CALLCONV _ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle); 
inline unsigned DLL_CALLCONV _WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle);
inline int DLL_CALLCONV _SeekProc(fi_handle handle, long offset, int origin);
inline long DLL_CALLCONV _TellProc(fi_handle handle);
void _OutputMessageFunction(FREE_IMAGE_FORMAT fif, const char *msg); 
void _stdcall _OutputMessageFunctionStdCall(FREE_IMAGE_FORMAT fif, const char *msg); 



//===========================================================================
// Image - Private Implementation
//===========================================================================
class CImage_impl
{
public:
	~CImage_impl();
	CImage_impl();

	PxU32			uiWidth;
	PxU32			uiHeight;
	PxU32			uiBitsPerPixel;
	void*			pPixels;

	// pointer to the FreeImage image.
	FIBITMAP*		pDib;
};

//---------------------------------------------------------------------------
CImage_impl::~CImage_impl()
{
	if ( pDib )
		FreeImage_Unload( pDib );
}

//---------------------------------------------------------------------------
CImage_impl::CImage_impl()
: uiWidth(0)
, uiHeight(0)
, uiBitsPerPixel(0)
, pPixels(NULL)
, pDib(NULL)
{
}

//===========================================================================
// Image
//===========================================================================

//---------------------------------------------------------------------------
CImage::~CImage()
{
	E_IMPL_DEL(CImage);
}

//---------------------------------------------------------------------------
CImage::CImage()
: E_IMPL_NEW(CImage)
{
}

//---------------------------------------------------------------------------
void
CImage::Release( CImage*& pImage )
{
	E_DELETE( pImage );
}

//---------------------------------------------------------------------------
CImage*
CImage::LoadImageFromFile( CFileHandle& cFile )
{
	E_ASSERT( cFile.IsOpen() );
	if ( !cFile.IsOpen() )
		return NULL;

	// TODO: refactor this to be threadsafe.
	FreeImageIO io;
	io.read_proc  = _ReadProc;
	io.write_proc = _WriteProc;
	io.tell_proc  = _TellProc;
	io.seek_proc  = _SeekProc;

	fi_handle hHandle = (fi_handle)cFile.GetFile();

	FreeImage_SetOutputMessage( _OutputMessageFunction );
	FreeImage_SetOutputMessageStdCall( _OutputMessageFunctionStdCall );

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to

	//check the file signature and deduce its format
	fif = FreeImage_GetFileTypeFromHandle(&io, hHandle);
	//if still unknown, try to guess the file format from the file extension
	if(fif == FIF_UNKNOWN) 
		fif = FreeImage_GetFIFFromFilename(cFile.GetPath().c_str());
	//if still unknown, return failure
	if(fif == FIF_UNKNOWN)
		return NULL;

	//check that the plugin has reading capabilities and load the file
	if(FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_LoadFromHandle(fif, &io, hHandle);
	//if the image failed to load, return failure
	if(!dib)
		return NULL;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if((bits == 0) || (width == 0) || (height == 0))
	{
		FreeImage_Unload(dib);
		return NULL;
	}

	CImage* pImg = E_NEW(CImage);
	pImg->m.uiWidth = width;
	pImg->m.uiHeight = height;
	pImg->m.uiBitsPerPixel = FreeImage_GetBPP(dib);
	pImg->m.pPixels = bits;
	pImg->m.pDib = dib;
	return pImg;
}

//---------------------------------------------------------------------------
PxU32
CImage::GetWidth()
{
	return m.uiWidth;
}

//---------------------------------------------------------------------------
PxU32
CImage::GetHeight()
{
	return m.uiHeight;
}

//---------------------------------------------------------------------------
PxU32
CImage::GetBitsPerPixel()
{
	return m.uiBitsPerPixel;
}

//---------------------------------------------------------------------------
byte*
CImage::GetPixels()
{
	return (byte*)m.pPixels;
}

//===========================================================================
// FreeImage I/O callbacks.
//===========================================================================

// ----------------------------------------------------------
inline unsigned DLL_CALLCONV
_ReadProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	CFile* pFile( (CFile*)handle );
	byte* pDst( (byte*)buffer );

	// determine how many bytes we can read.
	PxU64 uiSrc		= Min( pFile->GetSize(), pFile->GetPos() );
	PxU64 uiSrcEnd	= Min( pFile->GetSize(), pFile->GetPos() + (size*count) );
	PxU64 uiCount	= (uiSrcEnd - uiSrc);
	E_ASSERT( uiSrcEnd >= uiSrc );

	E_ASSERT( uiCount <= 0xFFFFFFFF );
	pFile->Read( pDst, (PxU32)uiCount );
	return (PxU32)uiCount;
}

// ----------------------------------------------------------
inline unsigned DLL_CALLCONV
_WriteProc(void *buffer, unsigned size, unsigned count, fi_handle handle)
{
	E_UNREF_PARAM3( handle, count, buffer );
	E_ASSERT( "todo." );
	return size*count;
}

// ----------------------------------------------------------
inline int DLL_CALLCONV
_SeekProc(fi_handle handle, long offset, int origin)
{
	CFile* pFile( (CFile*)handle );

	switch ( origin )
	{
	case SEEK_SET:
		pFile->Seek( offset );
		break;

	case SEEK_CUR:
		pFile->Seek( pFile->GetPos() + offset );
		break;

	case SEEK_END:
		{
			PxU64 uiFileSize( pFile->GetSize() );
			E_ASSERT( offset < uiFileSize );
			pFile->Seek( uiFileSize - Min((PxU64)offset, uiFileSize) );
		}
		break;

	default:
		E_ASSERT( !"Unknown origin" );
	}

	return 0;
}

// ----------------------------------------------------------
inline long DLL_CALLCONV
_TellProc(fi_handle handle)
{
	CFile* pFile( (CFile*)handle );
	return (long)pFile->GetPos();
}

//---------------------------------------------------------------------------
void
_OutputMessageFunction(FREE_IMAGE_FORMAT fif, const char *msg)
{
	printf( "FreeImage[0x%X]: %s", fif, msg );
}

//---------------------------------------------------------------------------
void _stdcall
_OutputMessageFunctionStdCall(FREE_IMAGE_FORMAT fif, const char *msg)
{
	printf( "FreeImage[0x%X]: %s", fif, msg );
}
