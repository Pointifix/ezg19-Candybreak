#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 texCoord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform int pass;

void main()
{
	texCoord = aTexCoords;

	if (pass == 0)
	{
		gl_Position = projection * view * model * vec4(aPos, 1.0);
	}
    else
	{
		gl_Position = model * vec4(aPos.y, aPos.x, 0.1, 1.0);
	}
}