#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class GrCamera_impl;
class GrProjection;
class MVec3;
class MMat4x4;

//===========================================================================
// GrCamera
//===========================================================================
class GrCamera
{
public:
	// copyable.
	GrCamera&				operator = ( const GrCamera& rhs );
	GrCamera( const GrCamera& rhs );

	// constructors.
	~GrCamera();
	GrCamera();
	GrCamera( const MVec3& pos, const MVec3& lookAt, const MVec3& upDir, const GrProjection& proj );


	void					SetPos( const MVec3& pos );
	void					SetUpDir( const MVec3& up );
	void					SetLookDir( const MVec3& look );
	void					SetProj( const GrProjection& proj );

	void					LookAt( const MVec3& lookAt );

	const MVec3&			GetPos() const;

	MVec3					CalcSideDir() const;
	const MVec3&			GetUpDir() const;
	const MVec3&			GetLookDir() const;

	const GrProjection&		GetProj() const;
	const MMat4x4&			GetProjMat() const;
	const MMat4x4&			GetViewMat() const;


private:
	void					RecomputeMatrix() const;

	E_IMPL( GrCamera );
};
