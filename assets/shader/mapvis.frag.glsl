#version 330

//uniform sampler2D mapDataTex;
//uniform vec2 playerPos;

// in vec4 gl_FragCoord;
// in bool gl_FrontFacing;
// in vec2 gl_PointCoord;

out vec4 outColor;

void main()
{
	outColor = vec4(1, gl_PointCoord.x, 1, 1);
}
