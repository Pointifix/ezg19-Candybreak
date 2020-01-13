#version 460 core
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in vec4 rotation[];

uniform mat4 V;
uniform mat4 VP;
uniform vec3 systemColor;
uniform int mode;

out vec4 color;

mat4 rotationX( in float angle ) {
	return mat4(	1.0,		0,			0,			0,
			 		0, 	cos(angle),	-sin(angle),		0,
					0, 	sin(angle),	 cos(angle),		0,
					0, 			0,			  0, 		1);
}

mat4 rotationY( in float angle ) {
	return mat4(	cos(angle),		0,		sin(angle),	0,
			 				0,		1.0,			 0,	0,
					-sin(angle),	0,		cos(angle),	0,
							0, 		0,				0,	1);
}

mat4 rotationZ( in float angle ) {
	return mat4(	cos(angle),		-sin(angle),	0,	0,
			 		sin(angle),		cos(angle),		0,	0,
							0,				0,		1,	0,
							0,				0,		0,	1);
}

vec3 rotate( vec3 point, vec3 rotation ) {
	return vec3(vec4(point, 1.0) * rotationX(rotation.x) * rotationY(rotation.y) * rotationZ(rotation.z));
}

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
	float TTL = gl_in[0].gl_Position.w;
	float size = .2 * TTL;
	if (mode == 0) size *= 3;

	//vec3 right = vec3(V[0][0], V[1][0], V[2][0]) * size;
	//vec3 up = vec3(V[0][1], V[1][1], V[2][1]) * size;

	vec3 right = rotate(vec3(V[0][0], V[1][0], V[2][0]), rotation[0].xyz * TTL) * size;
	vec3 up = rotate(vec3(V[0][1], V[1][1], V[2][1]), rotation[0].xyz * TTL) * size;

	vec3 particleColor = systemColor;

	float randomNumber = rand(vec2(gl_PrimitiveIDIn));

	const vec4 colorarray[7] = {
		vec4(0.0, 0.0, 10.0, 1.0),
		vec4(0.0, 10.0, 0.0, 1.0),
		vec4(0.0, 10.0, 10.0, 1.0),
		vec4(10.0, 0.0, 0.0, 1.0),
		vec4(10.0, 0.0, 10.0, 1.0),
		vec4(10.0, 1.0, 0.0, 1.0),
		vec4(10.0, 0.0, 10.0, 1.0)
	};
	particleColor = vec3(colorarray[int(randomNumber * 7) % 7]);

    vec3 P = gl_in[0].gl_Position.xyz;

	gl_Position = VP * vec4(P + right + up, 1.0);
	color = vec4(particleColor, TTL / 5);
	EmitVertex();
	
	gl_Position = VP * vec4(P + right - up, 1.0);
	color = vec4(particleColor, TTL / 5);
	EmitVertex();

	gl_Position = VP * vec4(P - right + up, 1.0);
	color = vec4(particleColor, TTL / 5);
	EmitVertex();

	gl_Position = VP * vec4(P - right - up, 1.0);
	color = vec4(particleColor, TTL / 5);
	EmitVertex();
	
	EndPrimitive();  
}