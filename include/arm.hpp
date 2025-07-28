#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

// Arm kinematics and positioning functions
vec3 calculateHammerPosition(float baseRotation, float bicepAngle, float forearmAngle,
                             float bicepLength, float forearmLength);

// Arm rendering functions
void renderArmComponents(GLuint cementTopTextureID, GLuint cementBaseTextureID,
                         GLuint woodTextureID, GLuint metalTextureID,
                         int texturedShaderProgram, float baseRotation,
                         float bicepAngle, float forearmAngle,
                         float bicepLength, float forearmLength);
