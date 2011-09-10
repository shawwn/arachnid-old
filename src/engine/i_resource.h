#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
#include "engine/c_resourcecommon.h"


//***************************************************************************
// Resource
//***************************************************************************
class IResource
{
public:
	virtual ~IResource();
	IResource( EResourceType eType, const string& sPath, const string& sName );

	const string&	GetName() const						{	return m_sName;		}
	const string&	GetPath() const						{	return m_sPath;		}

	ResHandle		GetHandle() const					{	return m_hRes;		}
	void			SetHandle( ResHandle hRes )			{	m_hRes = hRes;		}

private:
	ResHandle		m_hRes;
	string			m_sPath;
	string			m_sName;
	EResourceType	m_eType;
};
