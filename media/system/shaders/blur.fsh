#version 120

#extension GL_ARB_texture_rectangle : enable

//==========================================================
// constants.
//==========================================================
#define TAPS				13
#define VIRTUAL_TAPS		( 1 + ((TAPS - 1) / 2) )
#if VIRTUAL_TAPS == 3
#  define LOOPS				3
#else
#  define LOOPS				(6 - ((VIRTUAL_TAPS - 1) / 2))	// 4, 3, 2
#endif

//==========================================================
// vertex outputs.
//==========================================================
varying vec2 v_TexCoord;

#if (VIRTUAL_TAPS != 3)
	varying vec2	v_CenterTap;
#endif

#if (VIRTUAL_TAPS == 3)
	varying vec2	v_Taps[3];
#elif (VIRTUAL_TAPS == 5)
	varying vec2	v_Taps[4];
#elif (VIRTUAL_TAPS == 7)
	varying vec2	v_PositiveTaps[3];
	varying vec2	v_NegativeTaps[3];
#elif (VIRTUAL_TAPS == 9)
	varying vec2	v_PositiveTaps[3];
	varying vec2	v_NegativeTaps[3];
#endif

//==========================================================
// samplers.
//==========================================================
uniform sampler2DRect	s_CurrentTex;

//==========================================================
// uniforms.
//==========================================================
uniform vec4 u_BlurWeights;
uniform vec4 u_BlurOffsets;
#if VIRTUAL_TAPS != 3
uniform vec4 u_BlurCenterTapWeight;			// (X,undef,undef,undef)
#endif

//----------------------------------------------------------
vec4 tap1( vec2 uv )
{
	vec4 color =	texture2DRect( s_CurrentTex, gl_FragCoord.xy + uv);
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

#elif 0
	vec4 color = texture2DRect( s_CurrentTex, gl_FragCoord.xy );
	gl_FragColor = color;
	gl_FragColor.rgb += color.a;

#elif 1
	#if VIRTUAL_TAPS == 3
		mat3x4 samples;
	#elif VIRTUAL_TAPS == 5
		mat4x4 samples;
	#elif VIRTUAL_TAPS == 7
		mat3x4 positiveSamples;
		mat3x4 negativeSamples;
	#elif VIRTUAL_TAPS == 9
		mat4x4 positiveSamples;
		mat4x4 negativeSamples;
	#endif

	for(int i=0; i<LOOPS; i++)
	{
		#if VIRTUAL_TAPS == 3 || VIRTUAL_TAPS == 5
			samples[i]				= tap1( v_Taps[i] );
		#elif VIRTUAL_TAPS == 7
			positiveSamples[i]		= tap1( v_PositiveTaps[i] );
			negativeSamples[i]		= tap1( v_NegativeTaps[i] );
		#elif VIRTUAL_TAPS == 9
			positiveSamples[i*2]	= tap1( v_PositiveTaps[i].xy );
			negativeSamples[i*2]	= tap1( v_NegativeTaps[i].xy );
			positiveSamples[i*2+1]	= tap1( v_PositiveTaps[i].zw );
			negativeSamples[i*2+1]	= tap1( v_NegativeTaps[i].zw );
		#endif
	}
	
	#if VIRTUAL_TAPS != 3
		vec4 centerSample	= tap1( vec2(0,0) ) * u_BlurCenterTapWeight.x;
	#endif

	vec4 color = vec4( 0.0 );
	{
		#if VIRTUAL_TAPS != 3
			color += centerSample;
		#endif

		#if VIRTUAL_TAPS == 3
			color += (samples * u_BlurWeights.xyz);
		#elif VIRTUAL_TAPS == 5
			color += (samples * u_BlurWeights);
		#elif VIRTUAL_TAPS == 7
			color += (positiveSamples * u_BlurWeights.xyz);
			color += (negativeSamples * u_BlurWeights.xyz);
		#elif VIRTUAL_TAPS == 9
			color += (positiveSamples * u_BlurWeights);
			color += (negativeSamples * u_BlurWeights);
		#endif
	}
	gl_FragColor = color;

#endif
}

