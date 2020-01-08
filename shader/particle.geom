#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 V;
uniform mat4 VP;
uniform vec3 systemColor;

out vec4 color;

void main() {
	vec3 right = vec3(V[0][0], V[1][0], V[2][0]);

	vec3 up = vec3(V[0][1], V[1][1], V[2][1]);
	
	float TTL = gl_in[0].gl_Position.w;
	float size = 0.2 * TTL;

    vec3 P = gl_in[0].gl_Position.xyz;

	gl_Position = VP * vec4(P - (right + up) * size, 1.0);
	color = vec4(systemColor, TTL / 5);
	EmitVertex();
	
	gl_Position = VP * vec4(P + (right - up) * size, 1.0);
	color = vec4(systemColor, TTL / 5);
	EmitVertex();

	gl_Position = VP * vec4(P - (right - up) * size, 1.0);
	color = vec4(systemColor, TTL / 5);
	EmitVertex();

	gl_Position = VP * vec4(P + (right + up) * size, 1.0);
	color = vec4(systemColor, TTL / 5);
	EmitVertex();
	
	EndPrimitive();  
}