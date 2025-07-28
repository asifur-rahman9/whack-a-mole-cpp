#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

// Vertex structure for textured colored vertices
struct TexturedColoredVertex
{
    TexturedColoredVertex(vec3 _position, vec3 _color, vec2 _uv)
        : position(_position), color(_color), uv(_uv) {}

    vec3 position;
    vec3 color;
    vec2 uv;
};

// Textured Cube model with position, color, and UV coordinates
extern const TexturedColoredVertex texturedCubeVertexArray[];

// Graphics function declarations
GLuint loadTexture(const char *filename);
const char *getVertexShaderSource();
const char *getFragmentShaderSource();
const char *getTexturedVertexShaderSource();
const char *getTexturedFragmentShaderSource();

int compileAndLinkShaders(const char *vertexShaderSource, const char *fragmentShaderSource);
int createTexturedCubeVertexArrayObject();
void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix);
void setViewMatrix(int shaderProgram, mat4 viewMatrix);
void setWorldMatrix(int shaderProgram, mat4 worldMatrix);
