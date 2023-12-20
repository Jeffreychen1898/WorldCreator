#version 330 core

layout(location=1) in vec3 a_position;

void main()
{
	gl_Position = u_projection * vec4(a_position, 1.0);
}
