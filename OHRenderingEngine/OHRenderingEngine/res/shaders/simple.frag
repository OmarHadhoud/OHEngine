#version 330 core

out vec4 FragColor;
in vec3 v_Color;
in vec2 v_TexCoords;
uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, v_TexCoords);
}