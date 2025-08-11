#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    FragColor = vec4(0.25, 0.2, 0.4, 0.4) * texture(skybox, TexCoords);
}