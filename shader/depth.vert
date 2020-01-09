#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 3) in vec3 aOffset;

in int gl_InstanceID;
out int instanceID;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

void main()
{
	instanceID = gl_InstanceID;
    gl_Position = projection * view * model * vec4(aPos + aOffset, 1.0);
}  