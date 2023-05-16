#version 330 core

varying vec4 v_color;
varying vec2 v_texCoord;

uniform sampler2D u_texture;

void main()
{
	vec4 texel = texture(u_texture, v_texCoord);
	gl_FragColor = texel * v_color;
}
