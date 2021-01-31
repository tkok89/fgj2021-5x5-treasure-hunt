#version 120

uniform sampler2D mapTex;
uniform sampler2D mapSDFTex;

uniform vec2 playerPos;
uniform float maxStepLength;

bool raymarch(vec2 fromPos, vec2 toPos, out vec2 curPos)
{
	const float epsilon = 0.0001;

	float minStep = 3;
	curPos = fromPos;
	vec2 rayDir = normalize(toPos - fromPos);

	for (int i = 0; i < 50; ++i)
	{
		float stepLength = min(texture2D(mapSDFTex, curPos).x * maxStepLength, maxStepLength);
		minStep = min(stepLength, minStep);

		if (length(toPos - curPos) < epsilon || dot(normalize(toPos - curPos), rayDir) < 0)
		{
			// Reached target
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

	vec2 hitPos;
	bool hit = raymarch(playerPos, uv, hitPos);

	float light = 1;
	if (hit)
	{
		light = 0;
	}

	vec4 colorOut = vec4(light, texture2D(mapSDFTex, playerPos).x, mapPx.r, 1);

	gl_FragColor =  colorOut;
}
