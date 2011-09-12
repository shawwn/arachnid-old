#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class CImage_impl;
class CFileHandle;

//===========================================================================
// Image
//	A class to represent a BGRA 2D image.
//===========================================================================
class CImage
{
	CImage();
	~CImage();
public:
	static void		Release( CImage*& pImage );
	static CImage*	CreateImage( PxU32 uiWidth, PxU32 uiHeight );
	static CImage*	LoadImageFromFile( CFileHandle& cFile );

	PxU32			GetWidth();
	PxU32			GetHeight();
	PxU32			GetBitsPerPixel();
	byte*			GetPixels();

private:
	E_IMPL( CImage );
};