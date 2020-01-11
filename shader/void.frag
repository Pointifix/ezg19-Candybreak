#version 460 core
out vec4 FragColor;

uniform sampler2D aTexture;
uniform vec2 screenSize;

void main()
{
	FragColor = texture(aTexture, gl_FragCoord.xy / screenSize);
}