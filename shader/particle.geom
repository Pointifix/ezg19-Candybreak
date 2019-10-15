#version 460 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform mat4 V;
uniform mat4 VP;

void main() {
	vec3 right = vec3(V[0][0], V[1][0], V[2][0]);

	vec3 up = vec3(V[0][1], V[1][1], V[2][1]);

	float size = 5;

    vec3 P = gl_in[0].gl_Position.xyz;

	gl_Position = VP * vec4(P - (right + up) * size, 1.0);
	EmitVertex();
	
	gl_Position = VP * vec4(P + (right - up) * size, 1.0);
	EmitVertex();

	gl_Position = VP * vec4(P - (right - up) * size, 1.0);
	EmitVertex();

	gl_Position = VP * vec4(P + (right + up) * size, 1.0);
	EmitVertex();
	
	EndPrimitive();  
}