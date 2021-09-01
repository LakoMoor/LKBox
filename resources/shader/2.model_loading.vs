#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords1;

uniform mat4 model1;
uniform mat4 view1;
uniform mat4 projection1;

void main()
{
    TexCoords1 = aTexCoords;    
    gl_Position = projection1 * view1 * model1 * vec4(aPos, 1.0);
}