
// ensure positional invariance between passes.
//invariant gl_Position;

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

#if (VIRTUAL_TAPS == 3)
	varying vec2	v_Taps[3];
#elif (VIRTUAL_TAPS == 5)
	varying vec2	v_Taps[4];
#elif (VIRTUAL_TAPS == 7)
	varying vec2	v_PositiveTaps[3];
	varying vec2	v_NegativeTaps[3];
#elif (VIRTUAL_TAPS == 9)
	varying vec4	v_PositiveTaps[2];
	varying vec4	v_NegativeTaps[2];
#endif

//==========================================================
// uniforms.
//==========================================================
uniform mat4 u_ModelViewProjectionMatrix;

uniform vec4 u_BlurDirection;					//	(1,0,undef,undef) or (0,1,undef,undef)
uniform vec4 u_BlurOffsets;

//==========================================================
// main.
//==========================================================
void main()
{
	// calculate the outgoing position.
	gl_Position = u_ModelViewProjectionMatrix * vec4( a_Position.xyz, 1.0 );

	// output the texcoord.
	v_TexCoord = a_TexCoord;
	
	for ( int i = 0; (i < LOOPS); ++i )
	{
		float tapOffset;
		#if VIRTUAL_TAPS == 3
			tapOffset = 1.5*float(i) - 1.5 + u_BlurOffsets[i]; // { -1.5, 0, 1.5 }
			v_Taps[i] = tapOffset * u_BlurDirection.xy;

		#elif VIRTUAL_TAPS == 5
			tapOffset = 2.0*float(i) - 3.5 + ((i > 1) ? 1.0 : 0.0) + u_BlurOffsets[i]; // { -3.5, -1.5, 1.5, 3.5 }
			v_Taps[i] = tapOffset * u_BlurDirection.xy;

		#elif VIRTUAL_TAPS == 7
			tapOffset = float(2*i) + 1.5 + u_BlurOffsets[i]; // { 1.5, 3.5, 5.5 }
			v_PositiveTaps[i] = tapOffset * u_BlurDirection.xy;
			v_NegativeTaps[i] = -tapOffset * u_BlurDirection.xy;

		#elif VIRTUAL_TAPS == 9
			tapOffset = float(4*i) + 1.5 + u_BlurOffsets[i * 2]; // { 1.5, 5.5 }
			v_PositiveTaps[i].xy = tapOffset * u_BlurDirection.xy;
			v_NegativeTaps[i].xy = -tapOffset * u_BlurDirection.xy;

			tapOffset = (float(2*i) + 1.0) * 2.0 + 1.5 + u_BlurOffsets[i * 2 + 1]; // { 3.5, 7.5 }
			v_PositiveTaps[i].zw = tapOffset * u_BlurDirection.xy;
			v_NegativeTaps[i].zw = -tapOffset * u_BlurDirection.xy;			
		#endif
	}
}

