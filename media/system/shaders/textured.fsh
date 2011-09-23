
#extension GL_ARB_texture_rectangle : enable

//----------------------------------------------------------
// vertex inputs
varying vec2 v_TexCoord;

//----------------------------------------------------------
// samplers
uniform sampler2DRect s_DiffuseTex;
uniform sampler2D s_NormalTex;

//----------------------------------------------------------
vec4 tap1( vec2 uv )
{
	vec4 color =	texture2DRect( s_DiffuseTex, gl_FragCoord.xy + uv);
	color.rgb += color.a;
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
	vec4 diffuse = texture2DRect( s_DiffuseTex, gl_FragCoord.xy );
	gl_FragColor = diffuse;
	gl_FragColor.rgb += diffuse.a;
#endif
#if 1
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
	//gl_FragColor.rgb += color.a;
#endif
#if 0
	vec4 diffuse = texture2D( s_DiffuseTex, v_TexCoord );

	vec3 normal = texture2D( s_NormalTex, v_TexCoord ).rgb;
	normal = normalize( 2.0*normal - 1.0 );

	gl_FragColor = vec4(0.0);
	gl_FragColor.rgb = diffuse.rgb;	

	float s = diffuse.a;

/*
	s -= 0.05;
	if (s > 0.0)
		s = 1-exp(-2.0*s);
	s += 0.05;

	s -= 0.4;
	if (s > 0.0)
		s = 1-exp(-0.25*s);
	s += 0.4;
*/


/*
	//diffuse.rgb = s;
	float t = 0.5;
	float ts = 0.6;
	diffuse.rgb =	mix( diffuse.rgb,	1.0*(1.0-exp(-1.0*diffuse.rgb)), t );
	diffuse.rgb +=	mix( s,			0.9*(1.0-exp(-4.0*s)), ts );
	//diffuse.rgb += s;
*/

	gl_FragColor.rgb = diffuse.rgb;

	//gl_FragColor.rgb = normal.xyz;
	//gl_FragColor.rgb = vec3(v_TexCoord.xy, 0.0);
#endif
}

