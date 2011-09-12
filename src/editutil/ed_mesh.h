#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class EdMesh_impl;
class CFileHandle;

//===========================================================================
// EdMesh
//===========================================================================
class ED_API EdMesh
{
	EdMesh();
public:
	~EdMesh();

	static EdMesh*		LoadFromFile( CFileHandle& hFile );

private:
	E_IMPL( EdMesh );
};
