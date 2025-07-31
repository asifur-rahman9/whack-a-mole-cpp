#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

// Game logic and rendering functions
void updateGameLogic(bool &newCube, float &cubeRad, float &cubeRot, float &cubeY,
                     unsigned long millis, unsigned long &baseTime, unsigned long reset);

void renderScene(GLuint grassTextureID, GLuint cementTopTextureID, GLuint cementBaseTextureID,
                 GLuint woodTextureID, GLuint metalTextureID, int texturedShaderProgram, int vao,
                 float baseRotation, float bicepAngle, float forearmAngle, float bicepLength,
                 float forearmLength, float cubeX, float cubeY, float cubeRad, float cubeRot);

bool checkCollision(vec3 hammerWorldPos, vec3 cubePosition, float baseRotation);

void renderLights(int lightVAO, int lightShadingProgram, int sphereVertices, vec3 lightPos);