#include "render_gl2_afx.h"
#include "gl2_renderer.h"

// math headers.
#include "common/m_mat4x4.h"

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

//***************************************************************************
// Declarations
//***************************************************************************
void DrawMesh( NVSHARE::MeshSystem* ms );

//===========================================================================
// GL2Renderer - Private state
//===========================================================================
class CGL2Renderer_impl
{
public:
	CGL2Renderer_impl();
	~CGL2Renderer_impl();

	// Freeglut callback functions.
	bool					TryStartupGL( PxU32 uiScreenW, PxU32 uiScreenH );
	static void				OnRedraw();
	static void				OnClose();
	static void				OnKeyDown( unsigned char c, int, int );
	static void				OnKeyUp( unsigned char c, int, int );

	PxU32					uiScreenW;
	PxU32					uiScreenH;

	GL2Framebuffer*			pFBScreen;
	GL2Texture2D*			pRBScreenColor;
	GL2Renderbuffer*		pRBScreenDepth;

	GL2Texture2D*			pTestDiffuseMap;
	GL2Texture2D*			pTestNormalMap;
	EdMesh*					pTestMesh;
	GL2Shader*				pTestShader;

	GrCamera				cCamera;
};

// our singleton.
static CGL2Renderer_impl*	g_pRenderer;


//---------------------------------------------------------------------------
CGL2Renderer_impl::CGL2Renderer_impl()
: uiScreenW( 0 )
, uiScreenH( 0 )
, pFBScreen( NULL )
, pRBScreenColor( NULL )
, pRBScreenDepth( NULL )
, pTestDiffuseMap( NULL )
, pTestNormalMap( NULL )
, pTestMesh( NULL )
, pTestShader( NULL )
{
	E_ASSERT( g_pRenderer == NULL );
	g_pRenderer = this;
}

//---------------------------------------------------------------------------
CGL2Renderer_impl::~CGL2Renderer_impl()
{
	E_DELETE( pTestShader );
	E_DELETE( pTestMesh );
	E_DELETE( pTestDiffuseMap );
	E_DELETE( pTestNormalMap );

	E_DELETE( pFBScreen );
	E_DELETE( pRBScreenDepth );
	E_DELETE( pRBScreenColor );

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
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowPosition( 100, 100 );
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

	// ensure GL is okay.
	CHECK_GL();

	// create our framebuffers.
	{
		pFBScreen = GL2Framebuffer::CreateFramebuffer( uiScreenW, uiScreenH );
		pRBScreenColor = GL2Texture2D::CreateTexture2D( GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, uiScreenW, uiScreenH );
		pRBScreenDepth = GL2Renderbuffer::CreateRenderbuffer( GL_DEPTH_COMPONENT24, uiScreenW, uiScreenH );
		pFBScreen->Attach( GL2FramebufferAttachment( GL_COLOR_ATTACHMENT0, GL2_TEXTYPE_2D, pRBScreenColor ) );
		pFBScreen->Attach( GL2FramebufferAttachment( GL_DEPTH_ATTACHMENT,  GL2_TEXTYPE_RENDERBUFFER, pRBScreenDepth ) );
	}

	this->uiScreenW = uiScreenW;
	this->uiScreenH = uiScreenH;
	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnRedraw()
{
	if ( !g_pRenderer )
		return;

	// current camera viewpoint.
	GrCamera& cCam( g_pRenderer->cCamera );

	glClearColor( 0.2F, 0.2F, 0.2F, 0.0F );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_TEXTURE_2D );

	// render to texture.
	GL2Framebuffer::Bind( g_pRenderer->pFBScreen );
	GL2Shader::Bind( g_pRenderer->pTestShader );
	{
		// setup the shader.
		GL2Shader* pShader = g_pRenderer->pTestShader;
		{
			// camera.
			{
				// worldviewproj matrix.
				MMat4x4 wvp( cCam.GetProjMat() * cCam.GetViewMat() );
				pShader->SetEngineParam4x4fv( GL2Shader::EP_MODEL_VIEW_PROJECTION_MATRIX, wvp.GetData() );

				// view position.
				pShader->SetEngineParam4fv( GL2Shader::EP_VIEW_POS, cCam.GetPos() );
			}

			// light position.
			{
				MVec3 lightPos( cCam.GetPos() );
				lightPos.SetX( 0.0F );
				pShader->SetEngineParam4fv( GL2Shader::EP_LIGHT_POS, lightPos );
			}

			// textures.
			GL2Texture2D::Bind( GL2_TEX_DIFFUSE, g_pRenderer->pTestDiffuseMap );
			GL2Texture2D::Bind( GL2_TEX_NORMAL, g_pRenderer->pTestNormalMap );
		}

		// render mesh.
		EdMesh* pMesh = g_pRenderer->pTestMesh;
		DrawMesh( pMesh ? pMesh->GetMesh() : NULL );
	}
	GL2Shader::Unbind();
	GL2Framebuffer::Unbind();
	CHECK_GL();

	// render to screen, sourcing from that texture.
	glViewport( 0, 0, g_pRenderer->uiScreenW, g_pRenderer->uiScreenH );
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		g_pRenderer->pRBScreenColor->DrawFullscreen();
	}
	CHECK_GL();

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
	if ( c == 0x1B ) // ESC key
		OnClose();
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnKeyUp( unsigned char c, int, int )
{
	E_UNREF_PARAM(c);
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
			GrImage* pTestImage = GrImage::LoadImageFromFile( FileManager.OpenFile("/media/props/human_head/human_head_d.jpg") );
			m.pTestDiffuseMap = GL2Texture2D::LoadFromImage( pTestImage );
			E_DELETE( pTestImage );
		}

		// normal map.
		{
			GrImage* pTestImage = GrImage::LoadImageFromFile( FileManager.OpenFile("/media/props/human_head/human_head_n.jpg") );
			m.pTestNormalMap = GL2Texture2D::LoadFromImage( pTestImage );
			E_DELETE( pTestImage );
		}

		// mesh.
		m.pTestMesh = EdMesh::LoadFromFile( FileManager.OpenFile("/media/props/human_head/human_head.obj" ) );

		// shader.
		m.pTestShader = GL2Shader::CreateShaderFromFile( "/media/system/shaders/torrence_sparrow" );
	}

	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Shutdown()
{
	// free test assets.
	E_DELETE( m.pTestShader );
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
