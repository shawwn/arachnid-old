#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#include "engine/c_resourcecommon.h"

// forward declarations.
class IResource;
class CResourceLibrary;

//===========================================================================
// ResourceManager
//===========================================================================
class CResourceManager
{
public:
	virtual ~CResourceManager();
	SINGLETON_DEF(CResourceManager);

private:
};
#define ResourceManager		(*CResourceManager::GetInstance())

