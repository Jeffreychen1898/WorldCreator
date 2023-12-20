#version 330 core

layout(location=1) in vec3 a_position;

uniform mat4 u_projection;
uniform float u_gamma;
uniform float u_exposure;
uniform float u_intensity;
uniform vec3 u_sunColor;
uniform vec3 u_sunPosition;
uniform vec3 u_cameraPosition;

varying vec3 v_position;
varying vec3 v_normal;
varying float v_gamma;
varying float v_exposure;
varying float v_intensity;
varying vec3 v_sunColor;
varying vec3 v_sunPosition;
varying vec3 v_cameraPosition;

void main()
{
	gl_Position = u_projection * vec4(a_position, 1.0);
	v_normal = vec3(0.f, 1.f, 0.f);
	v_position = a_position;
	v_gamma = u_gamma;
	v_exposure = u_exposure;
	v_intensity = u_intensity;
	v_sunColor = u_sunColor;
	v_sunPosition = u_sunPosition;
	v_cameraPosition = u_cameraPosition;
}
