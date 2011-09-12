#include "editutil_afx.h"
#include "editutil.h"

// define the MeshImport global.
#include "MeshImport/MeshImport.h"
namespace NVSHARE { MeshImport *gMeshImport; }

//===========================================================================
// EditUtil
//===========================================================================

//---------------------------------------------------------------------------
bool
Ed_Startup()
{
	if ( NVSHARE::gMeshImport != NULL )
	{
		E_ASSERT( !"Already started." );
		return false;
	}

	NVSHARE::gMeshImport = NVSHARE::loadMeshImporters(NULL);
	if ( !NVSHARE::gMeshImport )
	{
		E_ASSERT( !"Failed to load mesh importers." );
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
void
Ed_Shutdown()
{
	if ( !NVSHARE::gMeshImport )
		return;

	// TODO: currently there is no clean way to shutdown meshimport.

	NVSHARE::gMeshImport = NULL;
}
