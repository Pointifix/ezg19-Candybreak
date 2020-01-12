#version 460 core
layout(location = 0) in vec4 aPos;
layout(location = 1) in vec4 aRot;

out vec4 rotation;

uniform mat4 V;
uniform mat4 VP;

void main()
{
	rotation = aRot;
	gl_Position = aPos;
}