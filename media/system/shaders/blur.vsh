
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

#if (VIRTUAL_TAPS != 3)
	varying vec2	v_CenterTap;
#endif

#if (VIRTUAL_TAPS == 3)
	varying vec2	v_TapsR[3];
	varying vec2	v_TapsG[3];
	varying vec2	v_TapsB[3];
#elif (VIRTUAL_TAPS == 5)
	varying vec2	v_TapsR[4];
	varying vec2	v_TapsG[4];
	varying vec2	v_TapsB[4];
#elif (VIRTUAL_TAPS == 7)
	varying vec2	v_PositiveTapsR[3];
	varying vec2	v_PositiveTapsG[3];
	varying vec2	v_PositiveTapsB[3];
	varying vec2	v_NegativeTapsR[3];
	varying vec2	v_NegativeTapsG[3];
	varying vec2	v_NegativeTapsB[3];
#elif (VIRTUAL_TAPS == 9)
	varying vec4	v_PositiveTapsR[2];
	varying vec4	v_PositiveTapsG[2];
	varying vec4	v_PositiveTapsB[2];
	varying vec4	v_NegativeTapsR[2];
	varying vec4	v_NegativeTapsG[2];
	varying vec4	v_NegativeTapsB[2];
#endif

//==========================================================
// uniforms.
//==========================================================
uniform mat4 u_ModelViewProjectionMatrix;

uniform vec4 u_BlurDirection;					//	(1,0,undef,undef) or (0,1,undef,undef)
uniform vec4 u_BlurOffsetsR;
uniform vec4 u_BlurOffsetsG;
uniform vec4 u_BlurOffsetsB;

//==========================================================
// main.
//==========================================================
void main()
{
	// calculate the outgoing position.
	gl_Position = u_ModelViewProjectionMatrix * vec4( a_Position.xyz, 1.0 );

	// output the texcoord.
	v_TexCoord = a_TexCoord;

	float texelSize = 1.0; // (we're using rectangular textures.)

	// output our blur info.
#if (VIRTUAL_TAPS != 3)
	//v_CenterTap = a_TexCoord + (texelSize / 2);
#endif
	
	for ( int i = 0; (i < LOOPS); ++i )
	{
		float tapOffsetR;
		float tapOffsetG;
		float tapOffsetB;
		#if VIRTUAL_TAPS == 3
			tapOffsetR = 1.5*float(i) - 1.5 + u_BlurOffsetsR[i]; // { -1.5, 0, 1.5 }
			tapOffsetG = 1.5*float(i) - 1.5 + u_BlurOffsetsG[i]; // { -1.5, 0, 1.5 }
			tapOffsetB = 1.5*float(i) - 1.5 + u_BlurOffsetsB[i]; // { -1.5, 0, 1.5 }
			v_TapsR[i] = tapOffsetR * u_BlurDirection.xy * texelSize;
			v_TapsG[i] = tapOffsetG * u_BlurDirection.xy * texelSize;
			v_TapsB[i] = tapOffsetB * u_BlurDirection.xy * texelSize;

		#elif VIRTUAL_TAPS == 5
			tapOffsetR = 2.0*float(i) - 3.5 + ((i > 1) ? 1.0 : 0.0) + u_BlurOffsetsR[i]; // { -3.5, -1.5, 1.5, 3.5 }
			tapOffsetG = 2.0*float(i) - 3.5 + ((i > 1) ? 1.0 : 0.0) + u_BlurOffsetsG[i]; // { -3.5, -1.5, 1.5, 3.5 }
			tapOffsetB = 2.0*float(i) - 3.5 + ((i > 1) ? 1.0 : 0.0) + u_BlurOffsetsB[i]; // { -3.5, -1.5, 1.5, 3.5 }
			v_TapsR[i] = tapOffsetR * u_BlurDirection.xy * texelSize;
			v_TapsG[i] = tapOffsetG * u_BlurDirection.xy * texelSize;
			v_TapsB[i] = tapOffsetB * u_BlurDirection.xy * texelSize;

		#elif VIRTUAL_TAPS == 7
			tapOffsetR = float(2*i) + 1.5 + u_BlurOffsetsR[i]; // { 1.5, 3.5, 5.5 }
			tapOffsetG = float(2*i) + 1.5 + u_BlurOffsetsG[i]; // { 1.5, 3.5, 5.5 }
			tapOffsetB = float(2*i) + 1.5 + u_BlurOffsetsB[i]; // { 1.5, 3.5, 5.5 }
			v_PositiveTapsR[i] = tapOffsetR * u_BlurDirection.xy * texelSize;
			v_PositiveTapsG[i] = tapOffsetG * u_BlurDirection.xy * texelSize;
			v_PositiveTapsB[i] = tapOffsetB * u_BlurDirection.xy * texelSize;
			v_NegativeTapsR[i] = -tapOffsetR * u_BlurDirection.xy * texelSize;
			v_NegativeTapsG[i] = -tapOffsetG * u_BlurDirection.xy * texelSize;
			v_NegativeTapsB[i] = -tapOffsetB * u_BlurDirection.xy * texelSize;

		#elif VIRTUAL_TAPS == 9
			tapOffsetR = float(4*i) + 1.5 + u_BlurOffsetsR[i * 2]; // { 1.5, 5.5 }
			tapOffsetG = float(4*i) + 1.5 + u_BlurOffsetsG[i * 2]; // { 1.5, 5.5 }
			tapOffsetB = float(4*i) + 1.5 + u_BlurOffsetsB[i * 2]; // { 1.5, 5.5 }
			v_PositiveTapsR[i].xy = tapOffset * u_BlurDirection.xy * texelSize;
			v_PositiveTapsG[i].xy = tapOffset * u_BlurDirection.xy * texelSize;
			v_PositiveTapsB[i].xy = tapOffset * u_BlurDirection.xy * texelSize;
			v_NegativeTapsR[i].xy = -tapOffset * u_BlurDirection.xy * texelSize;
			v_NegativeTapsG[i].xy = -tapOffset * u_BlurDirection.xy * texelSize;
			v_NegativeTapsB[i].xy = -tapOffset * u_BlurDirection.xy * texelSize;

			tapOffsetR = (float(2*i) + 1.0) * 2.0 + 1.5 + u_BlurOffsetsR[i * 2 + 1]; // { 3.5, 7.5 }
			tapOffsetG = (float(2*i) + 1.0) * 2.0 + 1.5 + u_BlurOffsetsG[i * 2 + 1]; // { 3.5, 7.5 }
			tapOffsetB = (float(2*i) + 1.0) * 2.0 + 1.5 + u_BlurOffsetsB[i * 2 + 1]; // { 3.5, 7.5 }
			v_PositiveTapsR[i].zw = tapOffset * u_BlurDirection.xy * texelSize;
			v_PositiveTapsG[i].zw = tapOffset * u_BlurDirection.xy * texelSize;
			v_PositiveTapsB[i].zw = tapOffset * u_BlurDirection.xy * texelSize;
			v_NegativeTapsR[i].zw = -tapOffset * u_BlurDirection.xy * texelSize;			
			v_NegativeTapsG[i].zw = -tapOffset * u_BlurDirection.xy * texelSize;			
			v_NegativeTapsB[i].zw = -tapOffset * u_BlurDirection.xy * texelSize;			
		#endif
	}
}

