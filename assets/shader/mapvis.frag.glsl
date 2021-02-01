#version 120

uniform sampler2D mapTex;
uniform sampler2D mapSDFTex;
uniform vec2 playerPos;
uniform float maxStepLength;
uniform float time;

const vec4 pParam = vec4( 17.0*17.0, 34.0, 1.0, 7.0);
const float pi = 3.141;
const float pi2 = pi * 2;

// From noise from https://www.geeks3d.com/20110317/shader-library-simplex-noise-glsl-opengl/
float permute(float x0,vec3 p) { 
	float x1 = mod(x0 * p.y, p.x);
	return floor(  mod( (x1 + p.z) *x0, p.x ));
}
vec2 permute(vec2 x0,vec3 p) { 
	vec2 x1 = mod(x0 * p.y, p.x);
	return floor(  mod( (x1 + p.z) *x0, p.x ));
}
vec3 permute(vec3 x0,vec3 p) { 
	vec3 x1 = mod(x0 * p.y, p.x);
	return floor(  mod( (x1 + p.z) *x0, p.x ));
}
vec4 permute(vec4 x0,vec3 p) { 
	vec4 x1 = mod(x0 * p.y, p.x);
	return floor(  mod( (x1 + p.z) *x0, p.x ));
}

float simplexNoise2(vec2 v)
{
	const vec2 C = vec2(
		0.211324865405187134, // (3.0-sqrt(3.0))/6.;
		0.366025403784438597); // 0.5*(sqrt(3.0)-1.);
	const vec3 D = vec3( 0., 0.5, 2.0) * 3.14159265358979312;
	// First corner
	vec2 i  = floor(v + dot(v, C.yy) );
	vec2 x0 = v -   i + dot(i, C.xx);

	// Other corners
	vec2 i1  =  (x0.x > x0.y) ? vec2(1.,0.) : vec2(0.,1.) ;

	 //  x0 = x0 - 0. + 0. * C
	vec2 x1 = x0 - i1 + 1. * C.xx ;
	vec2 x2 = x0 - 1. + 2. * C.xx ;

	// Permutations
	i = mod(i, pParam.x);
	vec3 p = permute( permute( 
		i.y + vec3(0., i1.y, 1. ), pParam.xyz)
		+ i.x + vec3(0., i1.x, 1. ), pParam.xyz
	);

	#ifndef USE_CIRCLE
	// ( N points uniformly over a line, mapped onto a diamond.)
	vec3 x = fract(p / pParam.w) ;
	vec3 h = 0.5 - abs(x) ;

	vec3 sx = vec3(lessThan(x,D.xxx)) *2. -1.;
	vec3 sh = vec3(lessThan(h,D.xxx));

	vec3 a0 = x + sx*sh;
	vec2 p0 = vec2(a0.x,h.x);
	vec2 p1 = vec2(a0.y,h.y);
	vec2 p2 = vec2(a0.z,h.z);

	#ifdef NORMALISE_GRADIENTS
	p0 *= taylorInvSqrt(dot(p0,p0));
	p1 *= taylorInvSqrt(dot(p1,p1));
	p2 *= taylorInvSqrt(dot(p2,p2));
	#endif

	vec3 g = 2.0 * vec3( dot(p0, x0), dot(p1, x1), dot(p2, x2) );
	#else 
	// N points around a unit circle.
	vec3 phi = D.z * mod(p,pParam.w) /pParam.w ;
	vec4 a0 = sin(phi.xxyy+D.xyxy);
	vec2 a1 = sin(phi.zz  +D.xy);
	vec3 g = vec3( dot(a0.xy, x0), dot(a0.zw, x1), dot(a1.xy, x2) );
	#endif
	// mix
	vec3 m = max(0.5 - vec3(dot(x0,x0), dot(x1,x1), dot(x2,x2)), 0.);
	m = m*m ;
	return 1.66666* 70.*dot(m*m, g);
}

bool raymarch(vec2 fromPos, vec2 toPos, out vec2 curPos, out float minStep)
{
	const float epsilon = 0.00001;

	minStep = 3;
	curPos = fromPos;
	vec2 rayDir = normalize(toPos - fromPos);

	for (int i = 0; i < 50; ++i)
	{
		float stepLength = min((texture2D(mapSDFTex, curPos).x - 0.45f) * maxStepLength, maxStepLength);
		minStep = min(stepLength, minStep);

		if (length(toPos - curPos) < epsilon || dot(normalize(toPos - curPos), rayDir) < 0)
		{
			return true;
		}

		if (minStep < epsilon)
		{
			// did not reach target
			return false;
		}

		curPos += rayDir * stepLength;
	}

	return false;
}

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 mapPx = texture2D(mapTex, uv);

	float light = 0;
	float jitterCount = 16;
	for (float i = 0; i < jitterCount; ++i)
	{
		float jt = i / jitterCount;
		vec2 jitteredPlayerPos = playerPos + vec2(sin(jt * pi2 + time * 128), cos(jt * pi2 + time * 128)) * 0.002;

		vec2 hitPos;
		float minStep;
		bool reached = raymarch(uv, jitteredPlayerPos, hitPos, minStep);

		float rayLight = 0;
		if (reached)
		{
			float dist = length(uv - playerPos);
			rayLight = 1 - dist / 0.1;
			//light *= min(1, minStep / 0.0025);
		}

		light += rayLight / jitterCount;
	}
	
	float simplex = (simplexNoise2(uv * 512) + 1) / 2;
	vec4 colorOut = (mapPx + vec4(vec3(simplex / 3), 0)) * vec4(vec3(light), 1); //vec4(light, texture2D(mapSDFTex, playerPos).x, mapPx.r, 1);

	gl_FragColor =  colorOut;
}
