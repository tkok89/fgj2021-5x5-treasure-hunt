#version 120

uniform sampler2D mapTex;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 mapPx = texture2D(mapTex, uv);

	gl_FragColor = vec4(mapPx.r, mapPx.g, mapPx.b, 1);
}
