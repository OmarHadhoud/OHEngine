#version 400 core

out vec4 FragColor;
in vec2 v_TexCoords;

uniform sampler2D tex;
uniform bool horizontal;


float kernel[5] = float[]
(
	 0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216
);


void main()
{
	vec2 offset = 1.0f / textureSize(tex,0);
	FragColor = vec4(0.0f,0.0f,0.0f,1.0f);
	FragColor = kernel[0] * texture(tex, v_TexCoords);
	for(int i = 1; i < 5; i++)
	{
		if(horizontal)
		{
			FragColor += kernel[i] * texture(tex, v_TexCoords+vec2(i*offset.x, 0));
			FragColor += kernel[i] * texture(tex, v_TexCoords+vec2(-i*offset.x, 0));
		}
		else
		{
			FragColor += kernel[i] * texture(tex, v_TexCoords+vec2(0,i*offset.y));
			FragColor += kernel[i] * texture(tex, v_TexCoords+vec2(0,-i*offset.y));
		}
	}
	FragColor.a = 1.0f;	
}