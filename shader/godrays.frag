#version 460 core
out vec4 FragColor;

in vec2 texCoord;

uniform int pass;
uniform bool light;

const vec2 lightPositionOnScreen = vec2(0.5, 0.5);

uniform sampler2D firstPass;

const int NUM_SAMPLES = 100;

void main()
{
	if (pass == 0)
	{
		if (light)
		{
			FragColor = vec4(1.0, 1.0, 1.0, 1.0);
		}
		else
		{
			FragColor = vec4(0.0, 0.0, 0.0, 1.0);
		}
	}
	else
	{
		vec2 deltaTextCoord = vec2( texCoord - lightPositionOnScreen );
    	vec2 textCoo = texCoord;
    	deltaTextCoord *= 1.0 /  float(NUM_SAMPLES) * 0.84f;
    	float illuminationDecay = 1.0;
	
    	for(int i=0; i < NUM_SAMPLES ; i++)
        {
                 textCoo -= deltaTextCoord;
                 vec4 aSample = texture(firstPass, textCoo);
				
                 aSample *= illuminationDecay * 5.65f;

                 FragColor += aSample;

                 illuminationDecay *= 1.0f;
         }
         FragColor *= 0.0034f;

		 FragColor = texture2D(firstPass, texCoord);
	}
}