
//----------------------------------------------------------
// vertex inputs
varying vec2 v_TexCoord;

//----------------------------------------------------------
// samplers
uniform sampler2D s_DiffuseTex;
uniform sampler2D s_NormalTex;

//----------------------------------------------------------
// main
void main()
{
	vec4 diffuse = texture2D( s_DiffuseTex, v_TexCoord );

	vec3 normal = texture2D( s_NormalTex, v_TexCoord ).rgb;
	normal = normalize( 2.0*normal - 1.0 );

	gl_FragColor = vec4(0.0);
  gl_FragColor.rgb = diffuse.xyz;	
	//gl_FragColor.rgb = normal.xyz;
}

