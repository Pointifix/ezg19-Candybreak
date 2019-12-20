#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2DMS image;

void main()
{
	FragColor = texelFetch(image, ivec2(TexCoords * textureSize(image)), 0);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness < 0.8) FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}