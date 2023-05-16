#version 330 core

varying float v_yCoord;

void main()
{
	gl_FragColor = vec4(vec3(v_yCoord), 1.0);
}
