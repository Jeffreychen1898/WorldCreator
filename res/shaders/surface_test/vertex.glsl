#version 330 core

layout(location=1) in vec3 a_position;

uniform mat4 u_projection;

varying float v_yCoord;

void main()
{
	gl_Position = u_projection * vec4(a_position, 1.0);
	v_yCoord = a_position.y / 400.0;
}
