#version 330 core

layout(location=1) in vec3 a_position;
layout(location=2) in vec3 a_color;

uniform mat4 u_projection;

varying vec3 v_color;

void main()
{
	gl_Position = u_projection * vec4(a_position, 1.0);
	v_color = a_color;
}
