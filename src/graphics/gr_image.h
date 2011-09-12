#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class GrImage_impl;
class CFileHandle;

//===========================================================================
// Image
//	A class to represent a BGRA 2D image.
//===========================================================================
class GrImage
{
	GrImage();
public:
	~GrImage();
	static GrImage*	CreateImage( PxU32 uiWidth, PxU32 uiHeight );
	static GrImage*	LoadImageFromFile( CFileHandle& cFile );

	PxU32			GetWidth();
	PxU32			GetHeight();
	PxU32			GetBitsPerPixel();
	byte*			GetPixels();

private:
	E_IMPL( GrImage );
};