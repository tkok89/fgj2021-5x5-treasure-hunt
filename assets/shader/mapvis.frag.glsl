#version 330

uniform sampler2D mapTex;
//uniform vec2 playerPos;

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

in vec2 uv;

out vec4 outColor;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;

	vec4 mapPx = texture2D(mapTex, uv);

	outColor = vec4(uv.x, uv.y, mapPx.r, 1);
}
