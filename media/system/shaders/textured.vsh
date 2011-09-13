
// ensure positional invariance between passes.
//invariant gl_Position;

//==========================================================
// vertex attributes.
//==========================================================
attribute vec3 a_Position;
attribute vec3 a_Tangent;
attribute vec3 a_Binormal;
attribute vec3 a_Normal;
attribute vec2 a_TexCoord;

//==========================================================
// vertex outputs.
//==========================================================
varying vec2 v_TexCoord;

//==========================================================
// uniforms.
//==========================================================
uniform mat4 u_ModelViewProjectionMatrix;

//==========================================================
// main.
//==========================================================
void main()
{
	// calculate the outgoing position.
	gl_Position = u_ModelViewProjectionMatrix * vec4( a_Position.xyz, 1.0 );

	// output the texcoord.
	v_TexCoord = a_TexCoord;
}

