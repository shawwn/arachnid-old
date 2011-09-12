#ifndef _DDSFILESTUFF_H_
#define _DDSFILESTUFF_H_





//For DDS files, my code, for DDS_CompressionMethod
#define DDS_COMPRESSION_DXT1		1
#define DDS_COMPRESSION_DXT2		2
#define DDS_COMPRESSION_DXT3		3
#define DDS_COMPRESSION_DXT4		4
#define DDS_COMPRESSION_DXT5		5

#define DDSD_CAPS 0x00000001 
#define DDSD_HEIGHT 0x00000002 
#define DDSD_WIDTH 0x00000004 
#define DDSD_PITCH 0x00000008 
#define DDSD_PIXELFORMAT 0x00001000 
#define DDSD_MIPMAPCOUNT 0x00020000 
#define DDSD_LINEARSIZE 0x00080000 
#define DDSD_DEPTH 0x00800000 

#define DDPF_ALPHAPIXELS 0x00000001 
#define DDPF_FOURCC 0x00000004 
#define DDPF_RGB 0x00000040

#define DDSCAPS_COMPLEX 0x00000008 
#define DDSCAPS_TEXTURE 0x00001000 
#define DDSCAPS_MIPMAP 0x00400000 



#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE

#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX

#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ

#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )

#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME
#define DDSD_DEPTH 0x00800000 

#define DDSCAPS2_CUBEMAP 0x00000200 
#define DDSCAPS2_CUBEMAP_POSITIVEX 0x00000400 
#define DDSCAPS2_CUBEMAP_NEGATIVEX 0x00000800 
#define DDSCAPS2_CUBEMAP_POSITIVEY 0x00001000 
#define DDSCAPS2_CUBEMAP_NEGATIVEY 0x00002000 
#define DDSCAPS2_CUBEMAP_POSITIVEZ 0x00004000 
#define DDSCAPS2_CUBEMAP_NEGATIVEZ 0x00008000 
#define DDSCAPS2_VOLUME 0x00200000 


struct DDPIXELFORMAT
{
	DWORD	dwSize;		//Size of structure. This member must be set to 32.
	DWORD	dwFlags;	//Flags to indicate valid fields. Uncompressed formats will usually
						//use DDPF_RGB to indicate an RGB format, while compressed formats
						//will use DDPF_FOURCC with a four-character code.
	DWORD	dwFourCC;	//This is the four-character code for compressed formats. dwFlags
						//should include DDPF_FOURCC in this case. For DXTn compression,
						//this is set to "DXT1", "DXT2", "DXT3", "DXT4", or "DXT5".
	DWORD	dwRGBBitCount;	//For RGB formats, this is the total number of bits in the format.
							//dwFlags should include DDPF_RGB in this case. This value is
							//usually 16, 24, or 32. For A8R8G8B8, this value would be 32.
	DWORD	dwRBitMask;		//For RGB formats, these three fields contain the masks for the
							//red, green, and blue channels. For A8R8G8B8, these values would
							//be 0x00ff0000, 0x0000ff00, and 0x000000ff respectively
	DWORD	dwGBitMask;
	DWORD	dwBBitMask;
	DWORD	dwRGBAlphaBitMask;	//For RGB formats, this contains the mask for the alpha
								//channel, if any. dwFlags should include DDPF_ALPHAPIXELS
								//in this case. For A8R8G8B8, this value would be 0xff000000.
};

struct DDCAPS2
{
	DWORD	dwCaps1;		//DDS files should always include DDSCAPS_TEXTURE. If the file
							//contains mipmaps, DDSCAPS_MIPMAP should be set. For any .dds
							//file with more than one main surface, such as a mipmaps, cubic
							//environment map, or volume texture, DDSCAPS_COMPLEX should also be set.
	DWORD	dwCaps2;		//For cubic environment maps, DDSCAPS2_CUBEMAP should be included
							//as well as one or more faces of the map (DDSCAPS2_CUBEMAP_POSITIVEX,
							//DDSCAPS2_CUBEMAP_NEGATIVEX, DDSCAPS2_CUBEMAP_POSITIVEY,
							//DDSCAPS2_CUBEMAP_NEGATIVEY, DDSCAPS2_CUBEMAP_POSITIVEZ,
							//DDSCAPS2_CUBEMAP_NEGATIVEZ). For volume textures,
							//DDSCAPS2_VOLUME should be included.
	DWORD	Reserved[2];	//Unused
};

struct DDSURFACEDESC2
{
	DWORD	dwSize;		//Size of structure. This member must be set to 124.
	DWORD	dwFlags;	//Flags to indicate valid fields. Always include DDSD_CAPS,
						//DDSD_PIXELFORMAT, DDSD_WIDTH, DDSD_HEIGHT.
	DWORD	dwHeight;	//Height of the main image in pixels
	DWORD	dwWidth;	//Width of the main image in pixels
	DWORD	dwPitchOrLinearSize;	//For uncompressed formats, this is the number
									//of bytes per scan line (DWORD> aligned) for the main
									//image. dwFlags should include DDSD_PITCH in this case.
									//For compressed formats, this is the total number of bytes
									//for the main image. dwFlags should be include DDSD_LINEARSIZE
									//in this case.
	DWORD	dwDepth;				//For volume textures, this is the depth of the volume.
									//dwFlags should include DDSD_DEPTH in this case.
	DWORD	dwMipMapCount;			//For items with mipmap levels, this is the total number
									//of levels in the mipmap chain of the main image.
									//dwFlags should include DDSD_MIPMAPCOUNT in this case.
	DWORD dwReserved1[11];			//Unused
	DDPIXELFORMAT	ddpfPixelFormat;	//32-byte value that specifies the pixel format structure.
	DDCAPS2	ddsCaps;				//16-byte value that specifies the capabilities structure
	DWORD	dwReserved2;			//Unused
};

struct DDSHeader
{
	DWORD	MagicNumber;
	DDSURFACEDESC2	DDSurfaceDesc2;
};

struct DXTColBlock
{
	ushort	Color0;
	ushort	Color1;

	//No bit fields - use bytes
	uchar	Row[4];
};

struct DXTAlphaBlockExplicit
{
	ushort row[4];
};

struct DXTAlphaBlock3BitLinear
{
	uchar	alpha0;
	uchar	alpha1;
	uchar	stuff[6];
};

#define DWORDFROMCHARS(a, b, c, d) (a | (b << 8) | (c << 16) | (d << 24))






#endif		//_DDSFILESTUFF_H_

