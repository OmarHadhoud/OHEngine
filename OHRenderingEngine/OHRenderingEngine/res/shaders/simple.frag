#version 330 core

out vec4 FragColor;
in vec3 v_Color;
in vec2 v_TexCoords;
in float v_TexNum;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
	if(int(v_TexNum)==1)		
	    FragColor = texture(texture0, v_TexCoords);
	else if(int(v_TexNum)==2)
	    FragColor = texture(texture1, v_TexCoords);

}