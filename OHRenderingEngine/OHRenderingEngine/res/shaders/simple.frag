#version 330 core
out vec4 FragColor;
in vec3 v_Color;  
void main()
{
    FragColor = vec4(v_Color, 1.0f);
}