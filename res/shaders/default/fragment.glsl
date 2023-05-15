#version 330 core

varying vec3 v_color;

uniform sampler2D u_texture;

void main()
{
	vec4 texel = texture(u_texture, vec2(0.2, 0.2));
	gl_FragColor = texel * vec4(v_color, 1.0);
}
