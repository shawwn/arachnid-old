
//----------------------------------------------------------
// vertex inputs
varying vec3 v_Pos;
varying vec2 v_TexCoord;

//----------------------------------------------------------
// samplers
uniform sampler2D s_DiffuseTex;
uniform sampler2D s_NormalTex;

//----------------------------------------------------------
// uniforms
uniform vec4 u_ViewPos;			// X,Y,Z,undef
uniform vec4 u_LightPos;		// X,Y,Z,undef


//==========================================================
// Torrence-Sparrow
//==========================================================
vec3 FixNormal(vec3 vN, vec3 vV);
float toBeckmannParam(float n);
float toNPhong(float m);

#ifndef M_PI
	#define M_PI 3.1415926535897932384626433832795
#endif

float saturate(float v)
{
	return clamp(v, 0.0, 1.0);
}

// Schlick's Fresnel approximation
float fresnelReflectance( float VdotH, float F0 )
{
	float base = 1.0-VdotH;
	float exponential = pow(base, 5.0);	// replace this by 3 muls for C/C++
	return saturate(exponential + F0 * (1.0 - exponential));
}

#define FLT_EPSILON     1.192092896e-07        // smallest such that 1.0+FLT_EPSILON != 1.0
#define FLT_MAX         3.402823466e+38        // max value
#define FLT_MIN         1.175494351e-38        // min positive value

//----------------------------------------------------------
// The Torrance-Sparrow visibility factor, G,
// as described by Jim Blinn but divided by VdotN
// Note that this was carefully implemented such that division
// by zero problems and awkward discontinuities are avoided.
float VisibDiv(float LdotN, float VdotN, float VdotH, float HdotN)
{	
	// VdotH should never be zero. Only possible if
	// L and V end up in the same plane (unlikely).
	float denom = max( VdotH, FLT_EPSILON );	
										
	float numL = min(VdotN, LdotN);
	float numR = 2.0*HdotN;
	if((numL*numR)<=denom)	// min(x,1) = x
	{
		numL = numL == VdotN ? 1.0 : (LdotN / VdotN);	// VdotN is > 0 if this division is used
		return (numL*numR) / denom;
	}
	else					// min(x,1) = 1				this branch is taken when H and N are "close" (see fig. 3)
		return 1.0 / VdotN;
		// VdotN >= HdotN*VdotN >= HdotN*min(VdotN, LdotN) >= FLT_EPSILON/2
}

//----------------------------------------------------------
// this is a normalized Phong model used in the Torrance-Sparrow model
vec3 BRDF_ts_nphong(vec3 vN, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float n=32, float F0=0.2)
{
	// reflect hack when view vector is occluded
	// (might disable this, doesn't seem to be needed)
	//vN = FixNormal(vN, vV);

	// microfacet normal
	vec3 vH = normalize(vV+vL);

	// the various dot products
	float LdotN = saturate(dot(vL, vN));
	float VdotN = saturate(dot(vV, vN));
	float VdotH = saturate(dot(vV, vH));
	float HdotN = saturate(dot(vH, vN));
	
	// diffuse
	float fDiff = LdotN;

	// D is a surface distribution function and obeys:
	// D(vH)*HdotN is normalized (over half-spere)
	// Specifically, this is the normalized phong model
	float D = ((n+2.0)/(2.0*M_PI))*pow(HdotN, n);

	// torrance-sparrow visibility term divided by VdotN
	float fVdivDots = VisibDiv(LdotN, VdotN, VdotH, HdotN);
	
	// Schlick's approximation
	float fFres = fresnelReflectance(VdotH, F0);

	// torrance-sparrow:
	// (F * G * D) / (4 * LdotN * VdotN)
	// Division by VdotN is done in VisibDiv()
	// and division by LdotN is removed since 
	// outgoing radiance is determined by:
	// BRDF * LdotN * L()
	float fSpec = (fFres * fVdivDots * D) / 4.0;
	
	// sum up: diff + spec
	// technically fDiff should be divided by pi.
	// Instead, we choose here to scale Cs by pi
	// which makes the final result scaled by pi.
	// We do this to keep the output intensity range
	// at a level which is more "familiar".
	vec3 res = Cd * fDiff + M_PI * Cs * fSpec;
	return res;
}

//----------------------------------------------------------
// optional variant (self shadowing factor)
// vN is the shade normal (from a bump/normal map)
// vN2 represents the normalized interpolated vertex normal
vec3 BRDF2_ts_nphong(vec3 vN, vec3 vN2, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float n=32, float F0=0.2)
{
	vec3 res = BRDF_ts_nphong(vN, vL, vV, Cd, Cs, n, F0);
	return saturate(4.0*dot(vL, vN2)) * res;
}

//----------------------------------------------------------
// this is the Torrance-Sparrow model but using the Beckmann distribution
vec3 BRDF_ts_beckmann(vec3 vN, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float m=0.22, float F0=0.2)
{
	// reflect hack when view vector is occluded
	// (might disable this, doesn't seem to be needed)
	//vN = FixNormal(vN, vV);

	// microfacet normal
	vec3 vH = normalize(vV+vL);

	// the various dot products
	float LdotN = saturate(dot(vL, vN));
	float VdotN = saturate(dot(vV, vN));
	float VdotH = saturate(dot(vV, vH));
	float HdotN = saturate(dot(vH, vN));
	
	// diffuse
	float fDiff = LdotN;
	
	// D is a surface distribution function and obeys:
	// D(vH)*HdotN is normalized (over half-spere)
	// Specifically, this is the Beckmann surface distribution function
	// D = exp(-tan^2(\theta_h)/m^2) / (pi * m^2 * cos^4(\theta_h));
	// where \theta_h = acos(HdotN)
	float fSqCSnh = HdotN*HdotN;
	float fSqCSnh_m2 = fSqCSnh*m*m;
	//float numerator = exp(-pow(tan(acos(HdotN))/m,2));
	float numerator = exp(-((1.0-fSqCSnh)/max(fSqCSnh_m2, FLT_EPSILON)));		// faster than tan+acos
	float D = numerator / (M_PI*max(fSqCSnh_m2*fSqCSnh, FLT_EPSILON));

	// torrance-sparrow visibility term divided by VdotN
	float fVdivDots = VisibDiv(LdotN, VdotN, VdotH, HdotN);
	
	// Schlick's approximation
	float fFres = fresnelReflectance(VdotH, F0);

	// torrance-sparrow:
	// (F * G * D) / (4 * LdotN * VdotN)
	// Division by VdotN is done in VisibDiv()
	// and division by LdotN is removed since 
	// outgoing radiance is determined by:
	// BRDF * LdotN * L()
	float fSpec = (fFres * fVdivDots * D) / 4.0;
	
	// sum up: diff + spec
	// technically fDiff should be divided by pi.
	// Instead, we choose here to scale Cs by pi
	// which makes the final result scaled by pi.
	// We do this to keep the output intensity range
	// at a level which is more "familiar".
	vec3 res = Cd * fDiff + M_PI * Cs * fSpec;
	return res;
}

//----------------------------------------------------------
// optional variant (self shadowing factor)
// vN is the shade normal (from a bump/normal map)
// vN2 represents the normalized interpolated vertex normal
vec3 BRDF2_ts_beckmann(vec3 vN, vec3 vN2, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float m=0.22, float F0=0.2)
{
	vec3 res = BRDF_ts_beckmann(vN, vL, vV, Cd, Cs, m, F0);
	return saturate(4.0*dot(vL, vN2)) * res;
}

//----------------------------------------------------------
vec3 FixNormal(vec3 vN, vec3 vV)
{
	float VdotN = dot(vV,vN);
	if(VdotN<=0.0)
	{
		vN = vN - 2.0*vV * VdotN;
	}
	return vN;
}

//----------------------------------------------------------
float toBeckmannParam(float n)
{
	// remap to beckmann roughness parameter by matching
	// the normalization constants in the surface distribution functions.
	float m = sqrt(2.0 / (n+2.0));
	return m;
}

//----------------------------------------------------------
float toNPhong(float m)
{
	// remap to normalized phong roughness parameter by matching
	// the normalization constants in the surface distribution functions.
	float n = (2.0 / (m*m)) - 2.0;
	return n;
}

//-------------------------- Alternative Tilt BRDF

float CalcTiltNormalization(float brdfAng, int n);

// this is an alternative to the normalized Phong model used in the Torrance-Sparrow model.
// This model supports a tilted peak as opposed to the default nphong case where brdfAng=0
vec3 BRDF_ts_nphong_tilt(vec3 vN, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float brdfAng=1.0, int n=32, float F0=0.2)
{	
	vec3 vH = normalize(vV+vL);

	// reflect hack when view vector is occluded
	// (might disable this, doesn't seem to be needed)
	//vN = FixNormal(vN, vV);

	// the various dot products
	float LdotN = saturate(dot(vL, vN));
	float VdotN = saturate(dot(vV, vN));
	float VdotH = saturate(dot(vV, vH));
	float HdotN = saturate(dot(vH, vN));
	
	// diffuse
	float fDiff = LdotN;

	// D is a surface distribution function and obeys:
	// p(vH) = D(vH)*HdotN where p is normalized (over half-spere)
	// Specifically, this is a variant of the normalized phong model.
	// In this version p reaches its peak at the angle: brdfAng
	// if Vh is the angle between vN and vH then
	// p(vH) = fMultNorm * cos(Vh-brdfAng)^n
	// note that the integration domain remains the half-sphere
	// relative to the normal vN and that when
	// brdfAng is zero the result is identical to the regular nphong.
	
	int ninc = n + 1;		// this increment is to match nphong which
								// has the division by n_dot_h built into it
	
	// hopefully this is resolved at compilation time
	float fMultNorm = CalcTiltNormalization(brdfAng, ninc);
	float co2 = cos((brdfAng*M_PI)/180.0);
	float si2 = sin((brdfAng*M_PI)/180.0);
	
	// Evaluate pdf and D
	float co1 = dot(vH, vN);
	float si1 = sqrt(1.0-co1*co1);
	float H2dotN = saturate(co2*co1 + si2*si1);
	
	float p = fMultNorm*pow(H2dotN, float(ninc));
	float D = p / max(HdotN, FLT_EPSILON);

	// torrance-sparrow visibility term over VdotN
	float fVdivDots = VisibDiv(LdotN, VdotN, VdotH, HdotN);
	
	// Schlick's approximation
	float fFres = fresnelReflectance(VdotH, F0);

	// torrance-sparrow
	float fSpec = (fFres * fVdivDots * D) / 4.0;
	
	// sum up: diff + spec
	// technically fDiff should be divided by pi.
	// Instead, we choose here to scale Cs by pi
	// which makes the final result scaled by pi.
	// We do this to keep the output intensity range
	// at a level which is more "familiar".
	vec3 res = Cd * fDiff + M_PI * Cs * fSpec;
	return res;
}

// optional variant (self shadowing factor)
// vN is the shade normal (from a bump/normal map)
// vN2 represents the normalized interpolated vertex normal
vec3 BRDF2_ts_nphong_tilt(vec3 vN, vec3 vN2, vec3 vL, vec3 vV, vec3 Cd, vec3 Cs, float brdfAng=1, int n=32, float F0=0.2)
{
	vec3 res = BRDF_ts_nphong_tilt(vN, vL, vV, Cd, Cs, brdfAng, n, F0);
	return saturate(4.0*dot(vL, vN2)) * res;
}

float CalcTiltNormalization(float brdfAng, int n)
{
	// if \theta_h is the angle between vN and vH then the pdf is
	// p(vH) = fMultNorm * cos(\theta_h-brdfAng)^n
	// note that when brdfAng is zero this is the regular nphong
	
	// this function computes fMultNorm as the
	// reciprocal value of p integrated over the hemisphere
	// oriented relative to vN.
	float v0 = M_PI - ((brdfAng*M_PI)/180.0);
	
	// After substitution the integral can be expressed as:
	// area = 2 pi * ( sin(v0)* Int cos(u)^{n+1} du - cos(v0)* Int cos(u)^n * sin(u) du )
	// Both integrals are integrated over the domain: ]v0-pi; v0-(pi/2)[
	float fLterm = 0.0;
	int m = n+1;
	float factor = 1.0;
	
	float t1 = v0 - (M_PI/2.0);
	float t0 = v0 - M_PI;
	while(m>0)
	{	
		float R1 = pow( cos(t1), float(m-1))*sin(t1);
		float R0 = pow( cos(t0), float(m-1))*sin(t0);
		fLterm += (factor/float(m))*(R1-R0);	
		factor *= (float(m-1) / float(m));
		m -= 2;
	}
	if(m==0) fLterm += factor*(t1-t0);
	
	float fRterm = (-1.0/(float(n+1))) * ( pow( cos(t1), float(n+1)) - pow( cos(t0), float(n+1)) );
	float fNorm = (2.0*M_PI) * (sin(v0)*fLterm - cos(v0)*fRterm);
	
	return 1.0/fNorm;
}



//----------------------------------------------------------
vec4 texture2D_to_linear( sampler2D s, vec2 texCoord )
{
	vec4 r = texture2D( s, texCoord );
	return pow( r, vec4(2.2) );
}
vec4 linear_to_gamma( vec4 color, float ex=2.2 )
{
	return pow( color, vec4(1.0/ex) );
}

//----------------------------------------------------------
// main
void main()
{
	//vec4 diffuse = texture2D( s_DiffuseTex, v_TexCoord );
	vec4 diffuse = vec4(1.0);

	vec3 normal = texture2D( s_NormalTex, v_TexCoord ).rgb;
	vec3 N = normalize(2.0*normal - 1.0);

	vec3 L = normalize( u_LightPos.xyz - v_Pos );
	float NdotL = saturate(dot( N, L ));

	vec3 V = normalize( u_ViewPos.xyz - v_Pos );

	float desat = 2.0*(diffuse.r + diffuse.g + diffuse.b)/3.0;

	float kSpec = desat*0.18;
	float kRough = 0.3;
	float kShadow = 0.2;

	//diffuse = linear_to_gamma( diffuse, 1.4 );
	//diffuse *= 1.4;

	//diffuse = pow(diffuse, 2.2);

	vec4 spec = vec4(kSpec, kSpec, kSpec, 1.0);
	diffuse.rgb = BRDF_ts_beckmann(N, L, V, diffuse.rgb, spec.rgb, kRough, kShadow);

	//diffuse = pow(diffuse, 1.0/2.2);

	// output color.
	gl_FragColor = diffuse;
}

