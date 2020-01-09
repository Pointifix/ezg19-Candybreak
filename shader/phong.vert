#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aOffset;

in int gl_InstanceID;
out int instanceID;

out VS_OUT {
	out vec3 FragPos;
	out vec3 Normal;
	out vec2 TexCoords;
	out vec4 FragPosLightSpace;
} vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightView;
uniform mat4 lightProjection;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos + aOffset, 1.0));
    vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
    vs_out.TexCoords = aTexCoords;
	vs_out.FragPosLightSpace = lightProjection * lightView * vec4(vs_out.FragPos, 1.0);
    instanceID = gl_InstanceID;

    gl_Position = projection * view * vec4(vs_out.FragPos, 1.0);
}