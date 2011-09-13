
//----------------------------------------------------------
// vertex inputs
varying vec2 v_TexCoord;

//----------------------------------------------------------
// samplers
uniform sampler2D s_DiffuseTex;

//----------------------------------------------------------
// main
void main()
{
	vec4 diffuse = texture2D( s_DiffuseTex, v_TexCoord );
	gl_FragColor = diffuse;
}

