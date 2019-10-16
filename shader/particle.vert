#version 460 core
layout(location = 0) in vec4 aPos;

uniform mat4 V;
uniform mat4 VP;

void main()
{
	gl_Position = aPos;
}