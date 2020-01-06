#version 460 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;

void main()
{
	FragColor = texture(image, TexCoords);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness < 2.0) FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}