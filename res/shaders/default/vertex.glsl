#version 330 core

layout(location=1) in vec3 a_position;
layout(location=2) in vec3 a_color;

varying vec3 v_color;

void main()
{
	gl_Position = vec4(a_position, 1.0);
	v_color = a_color;
}
