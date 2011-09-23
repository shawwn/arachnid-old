#include "render_gl2_afx.h"
#include "gl2_renderer.h"

// math headers.
#include "common/m_mat4x4.h"
#include "common/m_gaussiankernel.h"

// engine headers.
#include "engine/c_system.h"
#include "engine/c_filemanager.h"

// graphics headers.
#include "graphics/gr_image.h"
#include "graphics/gr_camera.h"
#include "common/m_vec3.h"
#include "common/m_mat4x4.h"

// FreeGlut headers.
#include "GL/freeglut.h"

// UI headers.
#include "AntTweakBar.h"

// EditLib headers.
#include "editutil/editutil.h"
#include "editutil/ed_mesh.h"
#include "MeshImport/MeshImport.h"

// gl2 headers.
#include "gl2_texture2d.h"
#include "gl2_framebuffer.h"
#include "gl2_renderbuffer.h"
#include "gl2_shader.h"

// shader directory.
const char* kShaderPath = "/media/system/shaders/";

typedef map<string, GL2Shader*>	ShaderMap;

enum EVIEWSPACE
{
	VIEW_CAMERASPACE = 1,
	VIEW_SCREENSPACE
};

//***************************************************************************
// Declarations
//***************************************************************************
void DrawMesh( NVSHARE::MeshSystem* ms );
void DrawMesh( EdMesh* pMesh )
{
	if ( pMesh != NULL )
		DrawMesh( pMesh->GetMesh() );
}

static const PxF64	kBlurDeviationDefault( GaussianKernel_IdealStandardDeviation(13) );
static PxF64	g_fBlurDeviationR( kBlurDeviationDefault );
static PxF64	g_fBlurDeviationG( kBlurDeviationDefault );
static PxF64	g_fBlurDeviationB( kBlurDeviationDefault );

//===========================================================================
// GL2Renderer - Private state
//===========================================================================
class CGL2Renderer_impl
{
public:
	CGL2Renderer_impl();
	~CGL2Renderer_impl();

	// loads all shaders.
	void					ReloadShaders();
	void					UnloadShaders();

	// Freeglut callback functions.
	bool					TryStartupGL( PxU32 uiScreenW, PxU32 uiScreenH );
	void					SetupShader( GL2Shader* pShader, EVIEWSPACE eViewSpace = VIEW_CAMERASPACE );
	GL2Shader*				SetupShader( const string& sName, EVIEWSPACE eViewSpace = VIEW_CAMERASPACE );
	static void				OnRedraw();
	static void				OnClose();
	static void				OnKeyDown( unsigned char c, int, int );
	static void				OnKeyUp( unsigned char c, int, int );

	bool					bKeysDown[256];

	PxU32					uiScreenW;
	PxU32					uiScreenH;

	PxU32					uiFPS;
	PxU32					uiFPSCounter;
	PxU32					uiFPSSecond;

	GL2Framebuffer*			pFBScreenPing;
	GL2Framebuffer*			pFBScreenPong;
	GL2Texture2D*			pRBScreenColorPing;
	GL2Texture2D*			pRBScreenColorPong;
	//GL2Renderbuffer*		pRBScreenDepth;
	GL2Texture2D*			pRBScreenDepth;

	GL2Texture2D*			pTestDiffuseMap;
	GL2Texture2D*			pTestNormalMap;
	EdMesh*					pTestMesh;
	ShaderMap				mapShaders;

	GrCamera				cCamera;
};

// our singleton.
static CGL2Renderer_impl*	g_pRenderer;

//===========================================================================
// BlurRenderer
//===========================================================================
class GL2BlurRenderer
{
public:
	GL2BlurRenderer( PxU32 uiNumTaps = 13 );

	GL2Shader*		SetupShader( const SVec3d& vRGBDeviation );
	bool			SetupShader( GL2Shader* pShader,
								GL2Shader::EPARAM eBlurWeightWhich,
								GL2Shader::EPARAM eBlurOffsetWhich,
								PxF64 fDeviation,
								PxF32& fOutCenterTapWeight );
	GL2Shader*		SetupShader()				{	return SetupShader( SVec3d(GaussianKernel_IdealStandardDeviation( (int)GetNumTaps() ) ) );	}

	PxU32			GetNumTaps() const			{	return m_uiNumTaps;		}

private:
	PxU32			m_uiNumTaps;
};

// our singleton.
static GL2BlurRenderer		g_Blur;

//===========================================================================
// GL2Renderer - Definitions
//===========================================================================

//---------------------------------------------------------------------------
CGL2Renderer_impl::CGL2Renderer_impl()
: uiScreenW( 0 )
, uiScreenH( 0 )
, uiFPS( 0 )
, uiFPSCounter( 0 )
, uiFPSSecond( 0 )
, pFBScreenPing( NULL )
, pFBScreenPong( NULL )
, pRBScreenColorPing( NULL )
, pRBScreenColorPong( NULL )
, pRBScreenDepth( NULL )
, pTestDiffuseMap( NULL )
, pTestNormalMap( NULL )
, pTestMesh( NULL )
{
	E_ASSERT( g_pRenderer == NULL );
	g_pRenderer = this;

	for ( int i = 0; i < 256; ++i )
		bKeysDown[i] = false;
}

//---------------------------------------------------------------------------
CGL2Renderer_impl::~CGL2Renderer_impl()
{
	UnloadShaders();

	E_DELETE( pTestMesh );
	E_DELETE( pTestDiffuseMap );
	E_DELETE( pTestNormalMap );

	E_DELETE( pFBScreenPong );
	E_DELETE( pFBScreenPing );
	E_DELETE( pRBScreenDepth );
	E_DELETE( pRBScreenColorPong );
	E_DELETE( pRBScreenColorPing );

	E_ASSERT( g_pRenderer == this );
	g_pRenderer = NULL;
}

//---------------------------------------------------------------------------
bool
CGL2Renderer_impl::TryStartupGL( PxU32 uiScreenW, PxU32 uiScreenH )
{
	// initialize freeglut.
	int argc = 0;
	char* argv = "";
	glutInit( &argc, &argv );
	glutInitDisplayMode( GLUT_RGB | GLUT_DEPTH );
	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( uiScreenW, uiScreenH );
	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
	glutCreateWindow( "Arachnid" );

	// bind freeglut callbacks.
	glutDisplayFunc( OnRedraw );
	glutCloseFunc( OnClose );
	glutKeyboardFunc( OnKeyDown );
	glutKeyboardUpFunc( OnKeyUp );

	// initialize GLEW.
	{
		GLenum err = glewInit();
		if ( err != GLEW_OK )
		{
			// glewInit failed, something is seriously wrong.
			printf( "Error: %s\n", glewGetErrorString(err) );
			E_ASSERT( !"failed to initialize glew" );
			return false;
		}
	}


	// verify that we support various extensions.
	{
// helper macro.
#define CHECK_EXT( name )										\
		if ( !glewIsExtensionSupported( #name ) )				\
		{														\
			E_ASSERT( !"extension not supported" );				\
			printf( "Error: extension '%s' not supported!\n" ); \
			supported = false; \
		}

		bool supported = true;
		CHECK_EXT( GL_ARB_shader_objects );
		CHECK_EXT( GL_ARB_texture_float );
		CHECK_EXT( GL_ARB_texture_rectangle );
		CHECK_EXT( GL_ARB_framebuffer_object );

		// if we don't support a required extension, then abort.
		if ( !supported )
			return false;
#undef CHECK_EXT
	}

	// initialize GL.
	{
		glEnable( GL_TEXTURE_2D );
		glEnable( GL_TEXTURE_RECTANGLE_ARB );
		CHECK_GL();

		glPixelStorei( GL_PACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		CHECK_GL();
	}

	// disable vsync.
	{
#ifdef WIN32
		if ( wglewGetExtension( "WGL_EXT_swap_control" ) )
		{
			assert( wglSwapIntervalEXT != NULL );
			wglSwapIntervalEXT( 0 );
		}
#endif
	}
	CHECK_GL();

	// create our framebuffer.
	pFBScreenPing = GL2Framebuffer::CreateFramebuffer( uiScreenW, uiScreenH );
	{
		// color channel.
		pRBScreenColorPing = GL2Texture2D::CreateTexture2D( GL_RGBA16F, GL_RGBA, GL_UNSIGNED_SHORT, uiScreenW, uiScreenH, true );
		pFBScreenPing->Attach( GL2FramebufferAttachment( GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenColorPing ) );

		// depth channel.
		pRBScreenDepth = GL2Texture2D::CreateTexture2D( GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, uiScreenW, uiScreenH, true );
		pFBScreenPing->Attach( GL2FramebufferAttachment( GL_DEPTH_ATTACHMENT,  GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenDepth ) );
		pFBScreenPing->Attach( GL2FramebufferAttachment( GL_STENCIL_ATTACHMENT,  GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenDepth ) );
		//pRBScreenDepth = GL2Renderbuffer::CreateRenderbuffer( GL_DEPTH24_STENCIL8, uiScreenW, uiScreenH );
		//pFBScreenPing->Attach( GL2FramebufferAttachment( GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, GL2_TEXTYPE_RENDERBUFFER, pRBScreenDepth ) );
	}
	CHECK_GL();
	GL2Framebuffer::VerifyBind( pFBScreenPong );
	pFBScreenPong = GL2Framebuffer::CreateFramebuffer( uiScreenW, uiScreenH );
	{
		// color channel.
		pRBScreenColorPong = GL2Texture2D::CreateTexture2D( GL_RGBA16F, GL_RGBA, GL_UNSIGNED_SHORT, uiScreenW, uiScreenH, true );
		pFBScreenPong->Attach( GL2FramebufferAttachment( GL_COLOR_ATTACHMENT0, GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenColorPong ) );

		// depth channel.
		pRBScreenDepth = GL2Texture2D::CreateTexture2D( GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, uiScreenW, uiScreenH, true );
		pFBScreenPong->Attach( GL2FramebufferAttachment( GL_DEPTH_ATTACHMENT,  GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenDepth ) );
		pFBScreenPong->Attach( GL2FramebufferAttachment( GL_STENCIL_ATTACHMENT,  GL_TEXTURE_RECTANGLE_ARB, GL2_TEXTYPE_2D, pRBScreenDepth ) );
		//pRBScreenDepth = GL2Renderbuffer::CreateRenderbuffer( GL_DEPTH24_STENCIL8, uiScreenW, uiScreenH );
		//pFBScreenPong->Attach( GL2FramebufferAttachment( GL_DEPTH_ATTACHMENT,  GL_RENDERBUFFER, GL2_TEXTYPE_RENDERBUFFER, pRBScreenDepth ) );
	}
	CHECK_GL();
	GL2Framebuffer::VerifyBind( pFBScreenPong );
	GL2Framebuffer::Unbind();

	this->uiScreenW = uiScreenW;
	this->uiScreenH = uiScreenH;
	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::SetupShader( GL2Shader* pShader, EVIEWSPACE eViewSpace )
{
	E_ASSERT( pShader != NULL );
	if ( !pShader )
	{
		GL2Shader::Unbind();
		return;
	}

	// bind the shader.
	GL2Shader::Bind( pShader );

	// current camera viewpoint.
	GrCamera& cCam( g_pRenderer->cCamera );

	// camera parameters.
	{
		switch ( eViewSpace )
		{
		case VIEW_CAMERASPACE:
			{
				// worldviewproj matrix.
				MMat4x4 wvp( cCam.GetProjMat() * cCam.GetViewMat() );
				pShader->SetEngineParam4x4fv( GL2Shader::EP_MODEL_VIEW_PROJECTION_MATRIX, wvp.GetData() );
				break;
			}
		case VIEW_SCREENSPACE:
			{
				pShader->SetEngineParam4x4fv( GL2Shader::EP_MODEL_VIEW_PROJECTION_MATRIX, MMat4x4::Identity.GetData() );
				break;
			}
		default:
			E_ASSERT(false);
		}

		// view position.
		pShader->SetEngineParam4fv( GL2Shader::EP_VIEW_POS, cCam.GetPos() );
	}
}

//---------------------------------------------------------------------------
GL2Shader*
CGL2Renderer_impl::SetupShader( const string& sName, EVIEWSPACE eViewSpace )
{
	ShaderMap::iterator itFind( mapShaders.find(sName) );
	if ( itFind == mapShaders.end() )
		return false;

	GL2Shader* pShader = itFind->second;
	if ( pShader )
		SetupShader( pShader, eViewSpace );

	return pShader;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnRedraw()
{
	if ( !g_pRenderer )
		return;
	CGL2Renderer_impl& m( *g_pRenderer );

	glClearColor( 0.2F, 0.2F, 0.2F, 0.0F );
	glClearDepth( 1.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_TEXTURE_2D );
	glEnable( GL_TEXTURE_RECTANGLE_ARB );

	// render the scene to texture.
	GL2Framebuffer::VerifyBind( m.pFBScreenPing );
	{
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		// clear the framebuffer.

		//const char* kShaderName = "textured";
		const char* kShaderName = "torrence_sparrow";
		if ( GL2Shader* pShader = m.SetupShader( kShaderName ) )
		{
			// textures.
			GL2Texture2D::Bind( GL2_TEX_DIFFUSE, g_pRenderer->pTestDiffuseMap );
			GL2Texture2D::Bind( GL2_TEX_NORMAL, g_pRenderer->pTestNormalMap );

			// draw the lights.
			glEnable( GL_BLEND );
			{
				vector<MVec3> vLightPos;
				vector<SVec4> vLightColor;
				{
					// perfect!
					vLightPos.push_back( MVec3(-2.0F, 0.0F,-5.0F) ); vLightColor.push_back( SVec4( 1.0F, 1.0F, 1.0F, 1.0F) );
					vLightPos.push_back( MVec3(-2.0F, 0.0F,-2.0F) ); vLightColor.push_back( SVec4( 1.0F, 1.0F, 1.0F, 1.0F) );

					// good enough...
					vLightPos.push_back( MVec3(-1.0F, 0.0F, 2.0F) ); vLightColor.push_back( SVec4( 1.2F, 1.2F, 1.2F, 0.0F) );

					vLightPos.push_back( MVec3( 5.0F, 0.0F, 0.0F) ); vLightColor.push_back( SVec4( 1.0F, 1.0F, 1.0F, 1.0F) );
					vLightPos.push_back( MVec3( 2.0F, 0.0F, 0.0F) ); vLightColor.push_back( SVec4( 1.0F, 1.0F, 1.0F, 1.0F) );
				}

				// draw each light.
				for ( size_t iLight(0); (iLight < vLightPos.size()); ++iLight )
				{
					// setup blend states.
					GLenum srcblend = GL_ONE;
					GLenum dstblend = (iLight == 0) ? GL_ZERO : GL_ONE;
					glBlendFunc( srcblend, dstblend );


					// light params.
					{
						pShader->SetEngineParam4fv( GL2Shader::EP_LIGHT_POS, vLightPos[iLight] );
						pShader->SetEngineParam4fv( GL2Shader::EP_LIGHT_COLOR, vLightColor[iLight] );
					}

					// draw the mesh.
					DrawMesh( m.pTestMesh );
				}
			}
			glDisable( GL_BLEND );
		}
	}
	GL2Framebuffer::Unbind();
	CHECK_GL();

#if 0
	// render to screen, sourcing from that texture.
	if ( GL2Shader* pShader = m.SetupShader( "blur_13taps", VIEW_SCREENSPACE ) )
	{
		GL2Texture2D::Bind( GL2_TEX_CURRENT, g_pRenderer->pRBScreenColorPing );

		m.pRBScreenColorPing->DrawFullscreen();
	}
	CHECK_GL();
#else
	SVec3d vBlurDeviationRGB;
	vBlurDeviationRGB.x = g_fBlurDeviationR;
	vBlurDeviationRGB.y = g_fBlurDeviationG;
	vBlurDeviationRGB.z = g_fBlurDeviationB;
	if ( GL2Shader* pShader = g_Blur.SetupShader( vBlurDeviationRGB ) )
	{
		bool bTry( !m.bKeysDown['G'] );
		if ( bTry )
		{
			GL2Framebuffer::VerifyBind( m.pFBScreenPong );
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		}
		{
			// horizontal separation.
			GL2Texture2D::Bind( GL2_TEX_CURRENT, g_pRenderer->pRBScreenColorPing );
			pShader->SetEngineParam4fv( GL2Shader::EP_BLUR_DIRECTION, SVec4( 1,0,		0.0, 0.0) );
			m.pRBScreenColorPing->DrawFullscreen();
		}

		if ( bTry )
		{
			GL2Framebuffer::Unbind();
			{
				// vertical separation.
				GL2Texture2D::Bind( GL2_TEX_CURRENT, g_pRenderer->pRBScreenColorPong );
				pShader->SetEngineParam4fv( GL2Shader::EP_BLUR_DIRECTION, SVec4( 0,1,		0.0, 0.0) );
				m.pRBScreenColorPong->DrawFullscreen();
			}
		}
	}
	CHECK_GL();
#endif

	GL2Framebuffer::Unbind();
	GL2Shader::Unbind();
	glutSwapBuffers();
	glutPostRedisplay();
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnClose()
{
	System.Exit();
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnKeyDown( unsigned char c, int, int )
{
	c = (unsigned char)toupper(c);
	switch ( c )
	{
	//case 'P':
	case 0x1B:	// ESC key
	case 0x71:		// VK_F1
		// quit.
		OnClose();
		break;

	case 'R':
		// reload shaders.
		g_pRenderer->ReloadShaders();
		break;

	case 0x20: // VK_SPACE
		g_fBlurDeviationR = kBlurDeviationDefault;
		g_fBlurDeviationG = kBlurDeviationDefault;
		g_fBlurDeviationB = kBlurDeviationDefault;
		break;
	}

	g_pRenderer->bKeysDown[c] = true;
	c = (unsigned char)tolower(c);
	g_pRenderer->bKeysDown[c] = true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnKeyUp( unsigned char c, int, int )
{
	E_UNREF_PARAM(c);

	c = (unsigned char)toupper(c);
	g_pRenderer->bKeysDown[c] = false;
	c = (unsigned char)tolower(c);
	g_pRenderer->bKeysDown[c] = false;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::ReloadShaders()
{
	// unload our current shaders.
	UnloadShaders();

	// determine the list of shaders to load.
	StringList vShaderFiles;
	FileManager.FindFiles( vShaderFiles, kShaderPath, "*.vsh" );
	FileManager.FindFiles( vShaderFiles, kShaderPath, "*.fsh" );

	for ( size_t i(0); i < vShaderFiles.size(); ++i )
	{
		const string& sPath( vShaderFiles[i] );
		const string& sFilename( Path_GetFilename(sPath) );
		const string& sName( Lowercase( Path_GetFilenameName(sPath) ) );

		// if we've already loaded the shader at this point, then skip it.
		if ( mapShaders.find( sName ) != mapShaders.end() )
			continue;

		// load the shader and insert it into our map.
		GL2Shader* pShader = GL2Shader::CreateShaderFromFile( FileManager.Join(kShaderPath, sFilename) );
		E_ASSERT( pShader != NULL );
		if ( pShader != NULL )
		{
			mapShaders.insert( make_pair(sName, pShader) );
		}
	}
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::UnloadShaders()
{
	while ( !mapShaders.empty() )
	{
		const string&	sName( mapShaders.begin()->first );
		GL2Shader*		pShader( mapShaders.begin()->second );
		E_UNREF_PARAM2( sName, pShader );

		// unload the shader.
		E_DELETE( pShader );

		mapShaders.erase( mapShaders.begin() );
	}
}

//===========================================================================
// GL2Renderer
//===========================================================================

//---------------------------------------------------------------------------
CGL2Renderer::~CGL2Renderer()
{
	E_IMPL_DEL( CGL2Renderer );
}

//---------------------------------------------------------------------------
CGL2Renderer::CGL2Renderer()
: E_IMPL_NEW( CGL2Renderer )
{
}

//---------------------------------------------------------------------------
bool
CGL2Renderer::Startup( PxU32 uiScreenW, PxU32 uiScreenH )
{
	// initialize GL.
	if ( !m.TryStartupGL( uiScreenW, uiScreenH ) )
	{
		Shutdown();
		return false;
	}

	// initialize AntTweakBar UI library.
	if ( !TwInit( TW_OPENGL, NULL ) )
	{
        fprintf(stderr, "AntTweakBar initialization failed: %s\n", TwGetLastError());
		Shutdown();
		return false;
	}

	// initialize EditUtil library.
	if ( !Ed_Startup() )
	{
        E_ASSERT( !"EditUtil failed to initialize." );
		Shutdown();
		return false;
	}

	// load test assets.
	{
		// diffuse map.
		{
			m.pTestDiffuseMap = GL2Texture2D::LoadMipsFromFile( "/media/props/human_head/human_head_d" );
			//m.pTestDiffuseMap = GL2Texture2D::LoadMipsFromFile( "/media/system/textures/mips/mips" );
		}
		E_ASSERT( m.pTestDiffuseMap );

		// normal map.
		{
			m.pTestNormalMap = GL2Texture2D::LoadMipsFromFile( "/media/props/human_head/human_head_n" );
		}
		E_ASSERT( m.pTestNormalMap );

		// mesh.
		m.pTestMesh = EdMesh::LoadFromFile( FileManager.OpenFile("/media/props/human_head/human_head.obj" ) );

		// shaders.
		m.ReloadShaders();
	}

	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Shutdown()
{
	m.UnloadShaders();

	// free test assets.
	E_DELETE( m.pTestMesh );
	E_DELETE( m.pTestDiffuseMap );
	E_DELETE( m.pTestNormalMap );

	// shutdown EditLib.
	Ed_Shutdown();

	// shutdown UI library.
	TwTerminate();

	// shutdown GL.
	glutDestroyWindow( glutGetWindow() );
	glutMainLoopEvent();

	// we're outta here!  Peace!
	delete this;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::SetViewpoint( const GrCamera& cCamera )
{
	m.cCamera = cCamera;
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Frame( PxU32 uiTotalTime, PxF32 fDeltaTime )
{
	E_UNREF_PARAM2( uiTotalTime, fDeltaTime );
	//printf( "clock: %d\t\tdt: %f\n", uiTotalTime, fDeltaTime );

	// update our camera.
	{
		const PxF32 kSpeedZ( 0.125F );
		const PxF32 kSpeedY( 0.375F );
		const PxF32 kSpeedX( 0.250F );
		MVec3 v3Velocity;
		{
			MVec3 v3Impulse;
			if ( m.bKeysDown['W'] )
				v3Impulse += MVec3( 0, 0, 1);
			if ( m.bKeysDown['S'] )
				v3Impulse += MVec3( 0, 0,-1);
			if ( m.bKeysDown['D'] )
				v3Impulse += MVec3( 1, 0, 0);
			if ( m.bKeysDown['A'] )
				v3Impulse += MVec3(-1, 0, 0);
			if ( m.bKeysDown[0x20] ) // space bar
				v3Impulse += MVec3( 0, 1, 0);
			if ( m.bKeysDown['C'] )
				v3Impulse += MVec3( 0,-1, 0);
			v3Impulse.Normalize();
			v3Impulse *= MVec3( kSpeedX, kSpeedY, kSpeedZ );

			v3Velocity += (v3Impulse.GetX() * m.cCamera.CalcSideDir());
			v3Velocity += (v3Impulse.GetY() * m.cCamera.GetUpDir());
			v3Velocity += (v3Impulse.GetZ() * m.cCamera.GetLookDir());
		}
		m.cCamera.SetPos( m.cCamera.GetPos() + fDeltaTime*v3Velocity );
		m.cCamera.LookAt( MVec3(0,0,0) );
	}

	// update our blur deviation.
	{
		if ( m.bKeysDown['0'] ) g_fBlurDeviationR += fDeltaTime;
		if ( m.bKeysDown['P'] ) g_fBlurDeviationR -= fDeltaTime;
		if ( m.bKeysDown['9'] ) g_fBlurDeviationG += fDeltaTime;
		if ( m.bKeysDown['O'] ) g_fBlurDeviationG -= fDeltaTime;
		if ( m.bKeysDown['8'] ) g_fBlurDeviationB += fDeltaTime;
		if ( m.bKeysDown['I'] ) g_fBlurDeviationB -= fDeltaTime;
	}

	// update our FPS.
	{
		if ( m.uiFPSSecond != (uiTotalTime / 1000) )
		{
			m.uiFPSSecond = (uiTotalTime / 1000);
			m.uiFPS = m.uiFPSCounter;
			m.uiFPSCounter = 0;

			// draw some debug info to our title.
			char tmp[128];
			sprintf_s(tmp, 128, "FPS: %d  Blur RGB Deviation: (%2.4f, %2.4f, %2.4f)",
				m.uiFPS,
				g_fBlurDeviationR, g_fBlurDeviationG, g_fBlurDeviationB );
			glutSetWindowTitle(tmp);
		}
		++m.uiFPSCounter;
	}

	glutMainLoopEvent();
}

//***************************************************************************
// Definitions
//***************************************************************************
using namespace NVSHARE;

//---------------------------------------------------------------------------
void
DrawMesh( NVSHARE::MeshSystem* ms )
{
	E_ASSERT( ms );
	if ( !ms )
		return;

	glBegin( GL_TRIANGLES );

	for ( NxU32 iMesh = 0; iMesh < ms->mMeshCount; ++iMesh )
	{
		Mesh* m = ms->mMeshes[ iMesh ];

		for ( NxU32 iSub = 0; iSub < m->mSubMeshCount; ++iSub )
		{
			SubMesh* sm = m->mSubMeshes[ iSub ];

			for ( NxU32 iTri = 0; iTri < sm->mTriCount; ++iTri )
			{
				NxU32 i0 = sm->mIndices[ 3*iTri + 0 ];
				NxU32 i1 = sm->mIndices[ 3*iTri + 1 ];
				NxU32 i2 = sm->mIndices[ 3*iTri + 2 ];
				MeshVertex* v0 = &m->mVertices[ i0 ];
				MeshVertex* v1 = &m->mVertices[ i1 ];
				MeshVertex* v2 = &m->mVertices[ i2 ];

#if 0
				glTexCoord2fv( v0->mTexel1 ); glVertex3fv( v0->mPos );
				glTexCoord2fv( v1->mTexel1 ); glVertex3fv( v1->mPos );
				glTexCoord2fv( v2->mTexel1 ); glVertex3fv( v2->mPos );
#else
				glVertexAttrib2fv( GL2_ATTRIB_TEXCOORD_INDEX, v0->mTexel1 ); glVertexAttrib3fv( GL2_ATTRIB_POSITION_INDEX, v0->mPos );
				glVertexAttrib2fv( GL2_ATTRIB_TEXCOORD_INDEX, v1->mTexel1 ); glVertexAttrib3fv( GL2_ATTRIB_POSITION_INDEX, v1->mPos );
				glVertexAttrib2fv( GL2_ATTRIB_TEXCOORD_INDEX, v2->mTexel1 ); glVertexAttrib3fv( GL2_ATTRIB_POSITION_INDEX, v2->mPos );
#endif
			}
		}
	}

	glEnd();
}

//===========================================================================
// BlurRenderer - Definitions
//===========================================================================

//---------------------------------------------------------------------------
GL2BlurRenderer::GL2BlurRenderer( PxU32 uiNumTaps /*= 13 */ )
: m_uiNumTaps( uiNumTaps )
{
}

//---------------------------------------------------------------------------
GL2Shader*
GL2BlurRenderer::SetupShader( const SVec3d& vRGBDeviation )
{
	GL2Shader* pShader = g_pRenderer->SetupShader("blur", VIEW_SCREENSPACE);
	if ( pShader )
	{
		SVec4 vCenterTapWeightsRGB( 0.0F, 0.0F, 0.0F, 0.0F );
		SetupShader( pShader, GL2Shader::EP_BLUR_WEIGHTS, GL2Shader::EP_BLUR_OFFSETS, vRGBDeviation.x, vCenterTapWeightsRGB.x );
		pShader->SetEngineParam4fv( GL2Shader::EP_BLUR_CENTER_TAP_WEIGHT, vCenterTapWeightsRGB );
	}
	return pShader;
}

//---------------------------------------------------------------------------
bool
GL2BlurRenderer::SetupShader( GL2Shader* pShader,
							 GL2Shader::EPARAM eBlurWeightWhich,
							 GL2Shader::EPARAM eBlurOffsetWhich,
							 PxF64 fDeviation,
							 PxF32& fOutCenterTapWeight )
{
	E_ASSERT( pShader );

	// calculate the blur weights.
	{
        // Only 5, 9, 13 and 17 taps are supported in the fast shader
		switch ( m_uiNumTaps )
		{
		case 5:
		case 9:
		case 13:
		case 17:
			break;
		default:
			E_ASSERT( !"Invalid number of taps." );
			{
				GL2Shader::Unbind();
				return false;
			}
		}

        int iSampledTaps = 1 + (4 * (int)Ceil((m_uiNumTaps - 1) / 4.0));
		int iCount = GaussianKernel_GetFastWeights( iSampledTaps, fDeviation, NULL, 0, NULL, 0 );
		if ( iCount > 0 )
		{
			vector<PxF32>	vWeights;
			vector<PxF32>	vOffsets;
			vWeights.resize( (size_t)Max(8, iCount), 0.0F );
			vOffsets.resize( (size_t)Max(8, iCount), 0.0F );

			GaussianKernel_GetFastWeights( iSampledTaps, fDeviation,
				&vWeights[0], (int)vWeights.size(),
				&vOffsets[0], (int)vOffsets.size() );

			fOutCenterTapWeight = 0.0F;
			if ( iSampledTaps != 5 )
				fOutCenterTapWeight = vWeights[0];

#define BIND_weights(x,y,z,w)	pShader->SetEngineParam4fv(eBlurWeightWhich, SVec4(x,y,z,w))
#define BIND_offsets(x,y,z,w)	pShader->SetEngineParam4fv(eBlurOffsetWhich, SVec4(x,y,z,w))
			{
				switch ( iSampledTaps )
				{
				case 5:
					BIND_weights( vWeights[1],	vWeights[0],		vWeights[1],		0.0F );
					BIND_offsets( -vOffsets[1],	vOffsets[0],		vOffsets[1],		0.0F );
					break;
				case 9:
					BIND_weights( vWeights[2],	vWeights[1],		vWeights[1],		vWeights[2] );
					BIND_offsets( -vOffsets[2],	-vOffsets[1],		vOffsets[1],		vOffsets[2] );
					break;
				case 13:
					BIND_weights(  vWeights[1],	vWeights[2],		vWeights[3],		0.0F );
					BIND_offsets(  vOffsets[1],	vOffsets[2],		vOffsets[3],		0.0F );
					break;
				case 17:
					BIND_weights( vWeights[1],	vWeights[2],		vWeights[3],		vWeights[4] );
					BIND_offsets( vOffsets[1],	vOffsets[2],		vOffsets[3],		vOffsets[4] );
					break;
				}
			}
#undef BIND_weights
#undef BIND_offsets

			// we're ready to go!
			return true;
		}
	}
	return false;
}
