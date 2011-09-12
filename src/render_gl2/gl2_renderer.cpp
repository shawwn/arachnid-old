#include "render_gl2_afx.h"
#include "gl2_renderer.h"

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

// gl2 headers.
#include "gl2_texture2d.h"

// EditLib headers.
#include "editutil/editutil.h"
#include "editutil/ed_mesh.h"

//===========================================================================
// GL2Renderer - Private state
//===========================================================================
class CGL2Renderer_impl
{
public:
	CGL2Renderer_impl();
	~CGL2Renderer_impl();

	// Freeglut callback functions.
	static bool				TryStartupGL( PxU32 uiScreenW, PxU32 uiScreenH );
	static void				BindGlutCallbacks();
	static void				OnRedraw();
	static void				OnClose();
	static void				OnKeyDown( unsigned char c, int, int );
	static void				OnKeyUp( unsigned char c, int, int );

	CGL2Renderer*			pRenderer;
	PxU32					uiScreenW;
	PxU32					uiScreenH;
	GrImage*				pTestImage;
	GL2Texture2D*			pTestTex;
};

// our instance.
static CGL2Renderer_impl*	g_pRenderer;


//---------------------------------------------------------------------------
CGL2Renderer_impl::CGL2Renderer_impl()
: pRenderer( NULL )
, uiScreenW( 512 )
, uiScreenH( 512 )
{
	E_ASSERT( g_pRenderer == NULL );
	g_pRenderer = this;
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

	// register freeglut callbacks.
	CGL2Renderer_impl::BindGlutCallbacks();

	g_pRenderer->uiScreenW = uiScreenW;
	g_pRenderer->uiScreenH = uiScreenH;
	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnRedraw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

	glEnable( GL_TEXTURE_2D );
	g_pRenderer->pTestTex->Bind(0);

	glBegin( GL_QUADS );
	{
		glTexCoord2f( 0.0F, 0.0F );  glVertex2f( -1.0F, -1.0F );
		glTexCoord2f( 0.0F, 1.0F );  glVertex2f( -1.0F,  1.0F );
		glTexCoord2f( 1.0F, 1.0F );  glVertex2f(  1.0F,  1.0F );
		glTexCoord2f( 1.0F, 0.0F );  glVertex2f(  1.0F, -1.0F );
	}
	glEnd();

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

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::BindGlutCallbacks()
{
	glutDisplayFunc( OnRedraw );
	glutCloseFunc( OnClose );
	glutKeyboardFunc( OnKeyDown );
	glutKeyboardUpFunc( OnKeyUp );
}

//---------------------------------------------------------------------------
CGL2Renderer_impl::~CGL2Renderer_impl()
{
	E_DELETE( pTestImage );

	E_ASSERT( g_pRenderer == this );
	g_pRenderer = NULL;
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
	 m.pRenderer = this;
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
		// textures.
		{
			m.pTestImage = GrImage::LoadImageFromFile( FileManager.OpenFile("/media/props/human_head/human_head_d.jpg") );
			m.pTestTex = GL2Texture2D::LoadFromImage( m.pTestImage );
			m.pTestTex->Bind(0);
		}

		// mesh.
		{
			EdMesh* pMesh = EdMesh::LoadFromFile( FileManager.OpenFile("/media/props/human_head/human_head.obj" ) );
			E_DELETE(pMesh);
		}
	}

	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Shutdown()
{
	// free test assets.
	E_DELETE( m.pTestImage );
	E_DELETE( m.pTestTex );

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
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( cCamera.GetProjMat().GetData() );
	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixf( cCamera.GetViewMat().GetData() );
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Frame( PxU32 uiTotalTime, PxF32 fDeltaTime )
{
	E_UNREF_PARAM2( uiTotalTime, fDeltaTime );
	printf( "clock: %d\t\tdt: %f\n", uiTotalTime, fDeltaTime );
	glutMainLoopEvent();
}
