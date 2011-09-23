#version 120

#extension GL_ARB_texture_rectangle : enable

//==========================================================
// vertex outputs.
//==========================================================
varying vec2 v_TexCoord;

//==========================================================
// samplers.
//==========================================================
uniform sampler2DRect	s_CurrentTex;
uniform sampler2DRect	s_DepthTex;

//==========================================================
// uniforms.
//==========================================================

//----------------------------------------------------------
vec4 tap1( vec2 uv )
{
	vec4 color =	texture2DRect( s_DepthTex, gl_FragCoord.xy + uv);
	//color.rgb += color.a;
	return color;
}

//----------------------------------------------------------
vec4 tap( vec2 uv )
{
	int N = 16;
	int Stride = 2;

	vec4 color = vec4(0.0); 
	for ( int i = 0; i < N; i+=Stride)
		color += tap1( float(i+1)*(uv) );

	color *= float(Stride)/float(N);
	return color;
}

//----------------------------------------------------------
// main
void main()
{
#if 0
	vec4 color =	tap( vec2( 0.0, 0.0 ) );
	color +=		tap( vec2( 1.0, 0.0 ) );
	color +=		tap( vec2(-1.0, 0.0 ) );
	color +=		tap( vec2( 0.0, 1.0 ) );
	color +=		tap( vec2( 0.0,-1.0 ) );
	color +=		tap( vec2( 1.0, 1.0 ) );
	color +=		tap( vec2(-1.0, 1.0 ) );
	color +=		tap( vec2( 1.0,-1.0 ) );
	color +=		tap( vec2(-1.0,-1.0 ) );
	color /= 9.0;
	gl_FragColor = color;

#elif 1
	vec4 color = ( 0.5*tap1( vec2( 0.0, 0.0 ) ) - 0.2 );
	color = (color / (color + 1.0));
	gl_FragColor = color;
	gl_FragColor.rgb += color.a;

#elif 1

#endif
}

