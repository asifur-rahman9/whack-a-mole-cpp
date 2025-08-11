#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "camera.hpp"
#include "shaders.hpp"

using namespace glm;

// Game logic and rendering functions
void updateGameLogic(bool &newCube, float &cubeRad, float &cubeRot, float &cubeY,
                     unsigned long millis, unsigned long &baseTime, unsigned long reset);

void renderScene(GLuint grassTextureID, GLuint cementTopTextureID, GLuint cementBaseTextureID,
                 GLuint woodTextureID, GLuint metalTextureID, int texturedShaderProgram, int vao,
                 float baseRotation, float bicepAngle, float forearmAngle, float bicepLength,
                 float forearmLength, float cubeX, float cubeY, float cubeRad, float cubeRot,
                 GLuint sphereVAO, int sphereVertices,
                 GLuint cylinderVAO, int cylinderVertices,
                 int cubeVertices,
                 GLuint grass1VAO, int grass1Vertices,
                 GLuint grass2VAO, int grass2Vertices,
                 GLuint treeVAO, int treeVertices);

bool checkCatch(vec3 cameraWorld, vec3 lookAt, vec3 cubePosition);

bool checkHammerHitsPlayer(vec3 hammerWorldPos, vec3 playerPosition);

void renderLights(int lightVAO, int lightShadingProgram, int sphereVertices, vec3 lightPos);

vec3* setLights(int LIGHT_NUMBR, int texturedShaderProgram, Shader textureShader, unsigned long millis, int lightingShaderProgram);

void drawLights(int LIGHT_NUMBR, int lightingShaderProgram, GLuint sphereVAO, int sphereVertices, vec3* lightPos, int texturedShaderProgram, Shader textureShader);

void renderSkybox(Camera camera, int skyboxShaderProgram, Shader skyboxShader, glm::mat4 projectionMatrix, unsigned int skyboxVAO, unsigned int cubemapTexture);

void renderGrass(GLuint grassTextureID, int texturedShaderProgram,
                 GLuint grass1VAO, int grass1Vertices,
                 GLuint grass2VAO, int grass2Vertices);

void renderTrees(GLuint grassTextureID, int texturedShaderProgram, GLuint treeVAO, int treeVertices);
