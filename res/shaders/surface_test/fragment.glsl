#version 330 core

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int Id;

void main()
{
	FragColor = vec4(1.0, 0.0, 1.0, 1.0);
	Id = 1;
}
