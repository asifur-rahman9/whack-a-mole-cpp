#version 330 core
in vec3 vertexColor;
out vec4 FragColor;

uniform float alpha = 0.6f;

void main()
{
    FragColor = vec4(0.95, 0.95, 0.9, alpha);
}