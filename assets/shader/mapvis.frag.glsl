#version 120

uniform sampler2D mapTex;
uniform sampler2D mapSDFTex;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 mapPx = texture2D(mapTex, uv);
	vec4 sdfPx = texture2D(mapSDFTex, uv);

	vec4 colorOut = mapPx * vec4(sin(sdfPx.x * ), 0, 0, 0);

	gl_FragColor =  colorOut;
}
