#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#include "engine/i_resource.h"


//===========================================================================
// TextureResource
//===========================================================================
class CTextureResource : public IResource
{
public:
	virtual ~CTextureResource();
	CTextureResource( const string& sPath, const string& sName );
};