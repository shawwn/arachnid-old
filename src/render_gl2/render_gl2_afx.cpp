#include "render_gl2_afx.h"

//---------------------------------------------------------------------------
void
_DbgCheckGL( const char* file, long line )
{
	static GLenum err;
	err = glGetError();
	if ( err != 0 )
	{
		// turn the line number into a string.
		char lineBuf[ 20 ];
		MemSet( lineBuf, 0, sizeof( lineBuf ) );
		_ltoa_s( line, lineBuf, 20, 10 );

		// compile a more complete error message.
		const GLubyte *str = gluErrorString( err );
		string errorMsg = "OpenGL Error occured in file \"";
		errorMsg += file;
		errorMsg += ", line ";
		errorMsg += lineBuf;
		errorMsg += ": ";
		errorMsg += ( str != 0 ? ( char * )str : "No OpenGL error given!" );
//		MessageBox( NULL, errorMsg.c_str(), "OpenGL error...", MB_OK | MB_ICONERROR );

		E_ASSERT( !"opengl error" );
	}
}