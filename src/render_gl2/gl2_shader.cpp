#include "render_gl2_afx.h"
#include "gl2_shader.h"

// math headers.
#include "common/m_vec3.h"

// engine headers.
#include "engine/c_filemanager.h"

//***************************************************************************
// Definitions
//***************************************************************************
GL2Shader* GL2Shader::s_pCurShader;

//***************************************************************************
// Error Shader
//***************************************************************************
static const char kErrorShaderVS[] =
"attribute vec3 a_Position;\n"
"uniform mat4 u_ModelViewProjectionMatrix;\n"
"void main()\n"
"{\n"
"\tgl_Position = u_ModelViewProjectionMatrix * vec4( a_Position.xyz, 1.0 );\n"
"}\n"
;

static const char kErrorShaderFS[] =
"void main()\n"
"{\n"
"\tgl_FragColor = vec4( 1.0f, 0.0f, 0.0f, 1.0f );\n"
"}\n"
;

//***************************************************************************
// Utilities
//***************************************************************************
// tests whether the build was successful.
static bool	WasCompileSuccessful( GLuint hShader );
static bool	WasLinkSuccessful( GLuint hProgram );

// retrieves error / warning logs.
static void	GetShaderLog( string& sOutLog, GLuint hShader );
static void	GetProgramLog( string& sOutLog, GLuint hProgram );

// compiles and links a shader.
void		BuildProgram( GLuint& hProgram, GLuint& hVertShader, GLuint& hFragShader,
						 const char* sVertProgram, const char* sFragProgram ); 

// deletes a vertex+fragment program.
void		DeleteProgram( GLuint& hProgram, GLuint& hVertShader, GLuint& hFragShader );

//===========================================================================
// Shader - Private Implementation
//===========================================================================
class GL2Shader_impl
{
public:
	~GL2Shader_impl();
	GL2Shader_impl();

	// clears our state to be equal to freshly-constructed.  Deletes our program and shaders.
	void			Reset( bool bClearErrorLogs = true );

	// compiles and links the shader.  If there are any compilation errors,
	// it 1) fetches the logs; 2) deletes the old shader; 3) compiles and
	// links an "error" shader (at the top of this file, a bright red effect).
	void			BuildFromSource( const char* sVertShader, const char* sFragShader );

	// initializes our sampler / constant locations.
	void			InitEngineParam( GLuint hProgram, GLenum glParamType, GL2Shader::EPARAM eParam, const char* sParamName );
	void			InitEngineSampler( GLuint hProgram, GLenum glParamType, GLuint uiTexUnit, const char* sParamName );
	void			InitParams();

	// the error logs.
	string			sProgramLog;
	string			sVertShaderLog;
	string			sFragShaderLog;

	// the GL program handle.
	GLuint			hProgram;
	GLuint			hVertShader;
	GLuint			hFragShader;

	// the uniform constant locations. -1 means "that uniform isn't used by the shader."
	GLint			vUniformLocations[ GL2Shader::EP_COUNT ];

	// the samplers used by the shader.
	PxU32			uiSamplersUsed;

	bool			bFailed;
};

//---------------------------------------------------------------------------
GL2Shader_impl::~GL2Shader_impl()
{
}

//---------------------------------------------------------------------------
GL2Shader_impl::GL2Shader_impl()
: hProgram( NULL_GL_HANDLE )
, hVertShader( NULL_GL_HANDLE )
, hFragShader( NULL_GL_HANDLE )
, bFailed( false )
{
}

//---------------------------------------------------------------------------
void
GL2Shader_impl::Reset( bool bClearErrorLogs )
{
	// clear our 'failed' state.
	bFailed = false;

	if ( bClearErrorLogs )
	{
		// clear any error logs.
		sProgramLog.clear();
		sFragShaderLog.clear();
		sVertShaderLog.clear();
	}

	// delete our old program, if any.
	DeleteProgram( hProgram, hFragShader, hVertShader );

	// clear our "samplers used", and our uniform constants table.
	uiSamplersUsed = 0;
	for ( PxI32 i = 0; i < GL2Shader::EP_COUNT; ++i )
		vUniformLocations[i] = -1;
}

//---------------------------------------------------------------------------
void
GL2Shader_impl::BuildFromSource(  const char* sVertShader, const char* sFragShader )
{
	// verify GL is okay.
	CHECK_GL();

	// clear any previous state.
	Reset();

	// build and link the program.
	BuildProgram( hProgram, hVertShader, hFragShader,
		sVertShader, sFragShader );
	CHECK_GL();

	// have we successfully linked?  If so, then we're done!
	if ( WasLinkSuccessful(hProgram) )
	{
		// on second thought, print out the program log.
		GetShaderLog( sVertShaderLog, hVertShader );
		GetShaderLog( sFragShaderLog, hFragShader );
			GetProgramLog( sProgramLog, hProgram );
		printf("\nShader successfully built:\n%s\n%s\n%s\n", sVertShaderLog.c_str(), sFragShaderLog.c_str(), sProgramLog.c_str() ); 
		return;
	}

	// determine whether the problem was with the compilation, or the linking phase.
	{
		bool bVertSuccess = WasCompileSuccessful( hVertShader );
		bool bFragSuccess = WasCompileSuccessful( hFragShader );

		if ( bVertSuccess && bFragSuccess )
		{
			// if both shaders compiled successfully, then the problem was in the linking stage,
			// so grab the program log.
			GetProgramLog( sProgramLog, hProgram );
		}
		else
		{
			// otherwise, the problem was at the compilation stage; grab the shader logs.
			if ( !bVertSuccess )
				GetShaderLog( sVertShaderLog, hVertShader );
			if ( !bFragSuccess )
				GetShaderLog( sFragShaderLog, hFragShader );
		}
	}
	CHECK_GL();

	// delete the failed shader, and return the error shader.
	{
		// clear any previous state, except error logs.
		Reset( false );

		// build and link the error shader.
		BuildProgram( hProgram, hVertShader, hFragShader,
			kErrorShaderVS, kErrorShaderFS );
	}
	CHECK_GL();

	// indicate that it's an error shader.
	bFailed = true;

	// print the error log.
	printf("\nShader failed to build:\n%s\n%s\n%s\n", sVertShaderLog.c_str(), sFragShaderLog.c_str(), sProgramLog.c_str() ); 
}

//---------------------------------------------------------------------------
void
GL2Shader::SetEngineParam4x4fv( EPARAM param, const float* data )
{
	// ensure GL is okay.
	CHECK_GL();

	// if the parameter doesn't exist, abort.
	if ( m.vUniformLocations[param] == -1 )
		return;

	// our program must be bound in order to update our parameters!
	assert( GL2Shader::GetActiveHandle() == m.hProgram );

	// update the parameter's value.
	glUniformMatrix4fv( m.vUniformLocations[param], 1, GL_FALSE, data );
}

//---------------------------------------------------------------------------
void
GL2Shader::SetEngineParam4fv( EPARAM param, const float* data )
{
	// ensure GL is okay.
	CHECK_GL();

	// if the parameter doesn't exist, abort.
	if ( m.vUniformLocations[param] == -1 )
		return;

	// our program must be bound in order to update our parameters!
	assert( GL2Shader::GetActiveHandle() == m.hProgram );

	// update the parameter's value.
	glUniform4fv( m.vUniformLocations[param], 1, data );
}

//---------------------------------------------------------------------------
void
GL2Shader_impl::InitEngineParam( GLuint hProgram, GLenum glParamType, GL2Shader::EPARAM eParam, const char* sParamName )
{
	// fetch the location of the uniform parameter by name.
	GLint handle = glGetUniformLocation( hProgram, sParamName );

	// verify the param's type and size.
	if ( handle != -1 )
	{
		GLint uiSize = 0;
		GLenum uiType = 0;
		size_t uiNameLen = strlen( sParamName ) + 1;
		char* name = (char*)alloca( uiNameLen );
		glGetActiveUniform( hProgram, handle, (GLsizei)uiNameLen, NULL, &uiSize, &uiType, name );

		assert( uiType == glParamType );
		if ( uiType != glParamType )
			printf( "Invalid engine parameter type specified for uniform %s!\n", sParamName );

		assert( uiSize == 1 );
		if ( uiSize != 1 )
			printf( "Invalid uniform size encountered for uniform %s!\n", sParamName );
	}
	CHECK_GL();

	// store the location.
	vUniformLocations[eParam] = handle;
}

//---------------------------------------------------------------------------
void
GL2Shader_impl::InitEngineSampler( GLuint hProgram, GLenum glParamType, GLuint uiTexUnit, const char* sParamName )
{
	assert( uiTexUnit < 16 );

	GLint handle = glGetUniformLocation( hProgram, sParamName );
	if ( handle == -1 )
		return;

	{
		// check the param's type.
		GLint size = 0;
		GLenum type = 0;
		size_t nameLen = strlen( sParamName ) + 1;
		char* name = (char*)alloca( nameLen );
		glGetActiveUniform( hProgram, handle, (GLsizei)nameLen, NULL, &size, &type, name );

		if ( type != glParamType )
		{
			bool valid = false;
			if ( glParamType == GL_SAMPLER_2D && type == GL_SAMPLER_2D_RECT )
				valid = true;
			if ( !valid )
			{
				assert( type == glParamType );
				printf( "Invalid engine sampler type specified for uniform %s!\n", sParamName );
			}
		}

		assert( size == 1 );
		if ( size != 1 )
			printf( "Invalid uniform size encountered for uniform %s!\n", sParamName );

		// set the sampler's value.
		{
			// get the previous shader.
			GLuint prevShader = GL2Shader::GetActiveHandle();

			// bind the sampler.
			glUseProgram( hProgram );
			glUniform1i( handle, uiTexUnit );

			// restore the previous shader.
			glUseProgram( prevShader );
		}
	}
	CHECK_GL();

	// mark the sampler as used.
	uiSamplersUsed |= (1 << uiTexUnit);
}

//---------------------------------------------------------------------------
void
GL2Shader_impl::InitParams()
{
	GLuint program = this->hProgram;

	InitEngineParam( program, GL_FLOAT_MAT4, GL2Shader::EP_MODEL_VIEW_PROJECTION_MATRIX, GL2_MODEL_VIEW_PROJECTION_MATRIX_NAME );
	InitEngineParam( program, GL_FLOAT_VEC4, GL2Shader::EP_VIEW_POS, GL2_VIEW_POS_NAME );
	InitEngineParam( program, GL_FLOAT_VEC4, GL2Shader::EP_LIGHT_POS, GL2_LIGHT_POS_NAME );
#if 0
	vUniformLocations[ GL2Shader::EP_DEBUG0 ] = InitEngineParam( program, GL2_DEBUG0_NAME, GL_FLOAT_VEC4 );
	vUniformLocations[ GL2Shader::EP_DEBUG1 ] = InitEngineParam( program, GL2_DEBUG1_NAME, GL_FLOAT_VEC4 );
	vUniformLocations[ GL2Shader::EP_DEBUG2 ] = InitEngineParam( program, GL2_DEBUG2_NAME, GL_FLOAT_VEC4 );
	vUniformLocations[ GL2Shader::EP_DEBUG3 ] = InitEngineParam( program, GL2_DEBUG3_NAME, GL_FLOAT_VEC4 );
	vUniformLocations[ GL2Shader::EP_DEBUG4 ] = InitEngineParam( program, GL2_DEBUG4_NAME, GL_FLOAT_VEC4 );
	vUniformLocations[ GL2Shader::EP_DEBUG5 ] = InitEngineParam( program, GL2_DEBUG5_NAME, GL_FLOAT_MAT4 );
	vUniformLocations[ GL2Shader::EP_DEBUG6 ] = InitEngineParam( program, GL2_DEBUG6_NAME, GL_FLOAT_MAT4 );
	vUniformLocations[ GL2Shader::EP_DEBUG7 ] = InitEngineParam( program, GL2_DEBUG7_NAME, GL_FLOAT_MAT4 );
	vUniformLocations[ GL2Shader::EP_DEBUG8 ] = InitEngineParam( program, GL2_DEBUG8_NAME, GL_FLOAT_MAT4 );
	vUniformLocations[ GL2Shader::EP_DEBUG9 ] = InitEngineParam( program, GL2_DEBUG9_NAME, GL_FLOAT_MAT4 );
#endif

	uiSamplersUsed = 0;
	InitEngineSampler( program,	GL_SAMPLER_2D,	GL2_TEX_DIFFUSE,	GL2_TEX_DIFFUSE_NAME );
	InitEngineSampler( program,	GL_SAMPLER_2D,	GL2_TEX_NORMAL,		GL2_TEX_NORMAL_NAME );
}

//===========================================================================
// Shader
//===========================================================================

//---------------------------------------------------------------------------
GL2Shader::GL2Shader()
: E_IMPL_NEW(GL2Shader)
{
}

//---------------------------------------------------------------------------
GL2Shader::~GL2Shader()
{
	DeleteProgram( m.hProgram, m.hFragShader, m.hVertShader );
	E_IMPL_DEL(GL2Shader);
}

//---------------------------------------------------------------------------
GL2Shader*
GL2Shader::CreateShader( const char* sVertShader, const char* sFragShader )
{
	CHECK_GL();
	
	// create our result, which creates our shader handles.
	GL2Shader* pShader = E_NEW( GL2Shader );

	// on build failure, this fetches the error logs and internally switches
	// to an error shader.  So the result is always a valid shader.
	pShader->m.BuildFromSource( sVertShader, sFragShader );
	CHECK_GL();

	// initialize the shader. 
	{
		// fetch uniform locations.
		pShader->m.InitParams();
	}
	CHECK_GL();

	// we're done, so return the shader.
	return pShader;
}

//---------------------------------------------------------------------------
GL2Shader*
GL2Shader::CreateShaderFromFile( const string& sPath )
{
	printf( "Loading shader '%s'...\n", sPath.c_str() ); 
	// an example of 'sPath' might be:
	//
	//	/media/system/shaders/textured
	//

	// load the source code.
	CFileHandle hVertProgram( FileManager.OpenFile(sPath + ".vsh") );
	CFileHandle hFragProgram( FileManager.OpenFile(sPath + ".fsh") );

	// if either of them failed, then abort.
	if ( !hVertProgram.IsOpen() || !hFragProgram.IsOpen() )
	{
		printf( "Failed to locate shaders at '%s'\n", sPath.c_str() ); 
		E_ASSERT( !"Failed to locate shader on disk." );
		return NULL;
	}

	// build a shader from the file data.  (Note that the filesystem
	// automatically null-terminates the file data for us.)
	return CreateShader( hVertProgram.GetFileMem(), hFragProgram.GetFileMem() );
}

//---------------------------------------------------------------------------
GLuint
GL2Shader::GetGLHandle( GL2Shader* pShader )
{
	if ( pShader == NULL )
		return NULL_GL_HANDLE;

	return pShader->m.hProgram;
}

//---------------------------------------------------------------------------
void
GL2Shader::Bind( GL2Shader* pShader )
{
	if ( pShader != s_pCurShader )
	{
		glUseProgram( pShader ? pShader->m.hProgram : NULL_GL_HANDLE );
		CHECK_GL();
		s_pCurShader = pShader;
	}
}

//---------------------------------------------------------------------------
void
GL2Shader::Unbind()
{
	glUseProgram( NULL_GL_HANDLE );
	s_pCurShader = NULL_GL_HANDLE;
}

//---------------------------------------------------------------------------
void
GL2Shader::SetEngineParam4fv( EPARAM param, const SVec4& data )
{
	SetEngineParam4fv( param, data.GetData() );
}

//---------------------------------------------------------------------------
void
GL2Shader::SetEngineParam4fv( EPARAM param, const MVec3& data )
{
	SetEngineParam4fv( param, SVec4( data.GetX(), data.GetY(), data.GetZ(), 1.0F ) );
}

//***************************************************************************
// Utilities 
//***************************************************************************

//---------------------------------------------------------------------------
bool
WasCompileSuccessful( GLuint hShader )
{
	GLint bStatus = GL_FALSE;
	glGetShaderiv( hShader, GL_COMPILE_STATUS, &bStatus );
	return (bStatus == GL_TRUE);
}

//---------------------------------------------------------------------------
bool
WasLinkSuccessful( GLuint hProgram )
{
	GLint bStatus = GL_FALSE;
	glGetProgramiv( hProgram, GL_LINK_STATUS, &bStatus );
	return (bStatus == GL_TRUE);
}

//---------------------------------------------------------------------------
void
GetShaderLog( string& sOutLog, GLuint hShader )
{
	GLsizei iSize = 0;
	glGetShaderiv( hShader, GL_INFO_LOG_LENGTH, &iSize );
	if ( iSize <= 0 )
		return;

	sOutLog.clear();
	sOutLog.resize( iSize );
	glGetShaderInfoLog( hShader, iSize, NULL, &sOutLog[0] );
}

//---------------------------------------------------------------------------
void
GetProgramLog( string& sOutLog, GLuint hProgram )
{
	GLsizei iSize = 0;
	glGetProgramiv( hProgram, GL_INFO_LOG_LENGTH, &iSize );
	if ( iSize <= 0 )
		return;

	sOutLog.clear();
	sOutLog.resize( iSize );
	glGetProgramInfoLog( hProgram, iSize, NULL, &sOutLog[0] );
}

//---------------------------------------------------------------------------
void
BuildProgram( GLuint& hProgram, GLuint& hVertShader, GLuint& hFragShader,
			 const char* sVertProgram, const char* sFragProgram )
{
	E_ASSERT( !hProgram && !hVertShader && !hFragShader );

	// verify GL is okay.
	CHECK_GL();

	// create the program'n'shader handles.
	hVertShader = glCreateShader( GL_VERTEX_SHADER );
	hFragShader = glCreateShader( GL_FRAGMENT_SHADER );
	hProgram = glCreateProgram();
	CHECK_GL();

	// compile the shaders.
	glShaderSource( hVertShader, 1, &sVertProgram, NULL );
	glShaderSource( hFragShader, 1, &sFragProgram, NULL );
	glCompileShader( hVertShader );
	glCompileShader( hFragShader );
	CHECK_GL();

	if ( WasCompileSuccessful(hVertShader) && WasCompileSuccessful(hFragShader) )
	{
		// attach the shaders.
		glAttachShader( hProgram, hVertShader );
		glAttachShader( hProgram, hFragShader );

		// bind the vertex attributes.
		glBindAttribLocation( hProgram,	GL2_ATTRIB_POSITION_INDEX,	GL2_ATTRIB_POSITION_NAME );
		glBindAttribLocation( hProgram,	GL2_ATTRIB_TANGENT_INDEX,	GL2_ATTRIB_TANGENT_NAME );
		glBindAttribLocation( hProgram,	GL2_ATTRIB_BINORMAL_INDEX,	GL2_ATTRIB_BINORMAL_NAME );
		glBindAttribLocation( hProgram,	GL2_ATTRIB_NORMAL_INDEX,	GL2_ATTRIB_NORMAL_NAME );
		glBindAttribLocation( hProgram,	GL2_ATTRIB_TEXCOORD_INDEX,	GL2_ATTRIB_TEXCOORD_NAME );
		glBindAttribLocation( hProgram,	GL2_ATTRIB_COLOR_INDEX,		GL2_ATTRIB_COLOR_NAME );

		// link the program.
		glLinkProgram( hProgram );

		CHECK_GL();
	}
}

//---------------------------------------------------------------------------
void
DeleteProgram( GLuint& hProgram, GLuint& hVertShader, GLuint& hFragShader )
{
	glDeleteProgram( hProgram );
	glDeleteShader( hFragShader );
	glDeleteShader( hVertShader );

	hProgram = NULL_GL_HANDLE;
	hVertShader = NULL_GL_HANDLE;
	hFragShader = NULL_GL_HANDLE;
}
