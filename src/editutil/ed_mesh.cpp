#include "editutil_afx.h"
#include "ed_mesh.h"

// engine headers.
#include "engine/c_filehandle.h"

// MeshImport headers.
#include "MeshImport/MeshImport.h"
using namespace NVSHARE;

//===========================================================================
// EdMesh - Private state.
//===========================================================================
class EdMesh_impl
{
public:
	EdMesh_impl();
	~EdMesh_impl();

	MeshSystemContainer*	pMeshContainer;
	MeshSystem*				pMesh;
};

//---------------------------------------------------------------------------
EdMesh_impl::EdMesh_impl()
: pMeshContainer( NULL )
, pMesh( NULL )
{
}

//---------------------------------------------------------------------------
EdMesh_impl::~EdMesh_impl()
{
	// release the mesh.
	if ( pMeshContainer )
		gMeshImport->releaseMeshSystemContainer( pMeshContainer );
}

//===========================================================================
// EdMesh
//===========================================================================

//---------------------------------------------------------------------------
EdMesh::EdMesh()
: E_IMPL_NEW( EdMesh )
{

}

//---------------------------------------------------------------------------
EdMesh::~EdMesh()
{
	E_IMPL_DEL( EdMesh );
}

//---------------------------------------------------------------------------
EdMesh*
EdMesh::LoadFromFile( const CFileHandle& hFile )
{
	// ensure that the mesh importer is loaded.
	E_ASSERT( gMeshImport );
	if ( !gMeshImport )
		return NULL;

	MeshSystemContainer* pMeshContainer = gMeshImport->createMeshSystemContainer( hFile.GetPath().c_str(), (const void*)hFile.GetFileMem(), (NxU32)hFile.GetSize(), NULL );
	hFile.Close();

	if ( !pMeshContainer )
	{
		E_ASSERT( !"failed to load mesh." );
		return NULL;
	}

	MeshSystem* pMesh = gMeshImport->getMeshSystem( pMeshContainer );
	if ( !pMesh )
	{
		E_ASSERT( !"failed to load mesh system." );
		gMeshImport->releaseMeshSystemContainer( pMeshContainer );
		return NULL;
	}

	EdMesh* pRet			= E_NEW(EdMesh);
	pRet->m.pMeshContainer	= pMeshContainer;
	pRet->m.pMesh			= pMesh;
	return pRet;
}

//---------------------------------------------------------------------------
NVSHARE::MeshSystem*
EdMesh::GetMesh()
{
	return m.pMesh;
}
