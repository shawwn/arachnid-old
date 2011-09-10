#include "engine_afx.h"
#include "i_resource.h"

//===========================================================================
// Resource
//===========================================================================

//---------------------------------------------------------------------------
IResource::~IResource()
{
}

//---------------------------------------------------------------------------
IResource::IResource( EResourceType eType, const string& sPath, const string& sName )
: m_eType(eType)
, m_sPath(sPath)
, m_sName(sName)
, m_hRes(INVALID_RESOURCE)
{
}
