#version 330 core
out vec4 FragColor;

in vec2 TexCoords1;

uniform sampler2D texture_diffuse11;

void main()
{    
    FragColor = texture(texture_diffuse11, TexCoords1);
}