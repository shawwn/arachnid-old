#include "render_gl2_afx.h"
#include "gl2_renderer.h"

#include "GL/freeglut.h"

#include "engine/c_system.h"

//===========================================================================
// GL2Renderer - Private state
//===========================================================================
class CGL2Renderer_impl
{
public:
	CGL2Renderer_impl();

	// Freeglut callback functions.
	static void				BindGlutCallbacks();
	static void				OnRedraw();
	static void				OnClose();
	static void				OnKeyDown( unsigned char c, int, int );
	static void				OnKeyUp( unsigned char c, int, int );

	CGL2Renderer*			pRenderer;
	PxU32					uiResW;
	PxU32					uiResH;
};

//---------------------------------------------------------------------------
CGL2Renderer_impl::CGL2Renderer_impl()
: pRenderer( NULL )
, uiResW( 512 )
, uiResH( 512 )
{
}

//---------------------------------------------------------------------------
void
CGL2Renderer_impl::OnRedraw()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
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
CGL2Renderer::Startup( PxU32 uiResolutionW, PxU32 uiResolutionH )
{
	m.uiResW = uiResolutionW;
	m.uiResH = uiResolutionH;

	// initialize freeglut.
	int argc = 0;
	char* argv = "";
	glutInit( &argc, &argv );
	glutInitDisplayMode( GLUT_RGBA | GLUT_DEPTH | GLUT_STENCIL );
	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize( uiResolutionW, uiResolutionH );
	glutSetOption( GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION );
	glutCreateWindow( "Arachnid" );

	// register the callbacks.
	CGL2Renderer_impl::BindGlutCallbacks();

	return true;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Shutdown()
{
	glutDestroyWindow( glutGetWindow() );
	glutMainLoopEvent();
	delete this;
}

//---------------------------------------------------------------------------
void
CGL2Renderer::Frame( PxU32 uiTotalTime, PxF32 fDeltaTime )
{
	E_UNREF_PARAM2( uiTotalTime, fDeltaTime );
	printf( "clock: %d\t\tdt: %f\n", uiTotalTime, fDeltaTime );
	glutMainLoopEvent();
}
