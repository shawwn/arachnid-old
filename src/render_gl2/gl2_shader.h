#pragma once

//***************************************************************************
// Declarations
//***************************************************************************
class GL2Shader_impl;
class MVec3;

//===========================================================================
// Shader
//	A class to represent a GLSL Vertex Program & Fragment Program.
//===========================================================================
class GL2Shader
{
	GL2Shader();
public:
	~GL2Shader();

	static GLuint		GetGLHandle( GL2Shader* pShader );
	static GL2Shader*	CreateShader( const char* sVertProgram, const char* sFragProgram );
	static void			Bind( GL2Shader* pShader );
	static void			Unbind();
	static GL2Shader*	GetActive()							{	return s_pCurShader;				}
	static GLuint		GetActiveHandle()					{	return GetGLHandle(s_pCurShader);	}

	enum EPARAM
	{
		EP_MODEL_VIEW_PROJECTION_MATRIX,
		EP_VIEW_POS,
		EP_LIGHT_POS,

		EP_COUNT
	};

	void				SetEngineParam4x4fv( EPARAM param, const float* data );
	void				SetEngineParam4fv( EPARAM param, const float* data );
	void				SetEngineParam4fv( EPARAM param, const SVec4& data );
	void				SetEngineParam4fv( EPARAM param, const MVec3& data );

private:
	E_IMPL( GL2Shader );

	// the current shader, set via Bind()
	static GL2Shader*			s_pCurShader;
};
