#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class EdMesh_impl;
class CFileHandle;
namespace NVSHARE { class MeshSystem; }

//===========================================================================
// EdMesh
//===========================================================================
class ED_API EdMesh
{
	EdMesh();
public:
	~EdMesh();

	static EdMesh*			LoadFromFile( CFileHandle& hFile );

	NVSHARE::MeshSystem*	GetMesh();

private:
	E_IMPL( EdMesh );
};
