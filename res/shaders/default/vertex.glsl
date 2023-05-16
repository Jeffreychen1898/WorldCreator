#version 330 core

layout(location=1) in vec3 a_position;
layout(location=2) in vec4 a_color;
layout(location=3) in vec2 a_texCoord;

uniform mat4 u_projection;

varying vec4 v_color;
varying vec2 v_texCoord;

void main()
{
	gl_Position = u_projection * vec4(a_position, 1.0);
	v_color = a_color;
	v_texCoord = a_texCoord;
}
