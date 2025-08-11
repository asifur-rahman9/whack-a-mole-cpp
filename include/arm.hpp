#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace glm;

struct HammerController {
    bool isActive = false;
    float detectionRadius = 40.0f;
};

// Arm kinematics and positioning functions
vec3 calculateHammerPosition(float baseRotation, float bicepAngle, float forearmAngle,
                             float bicepLength, float forearmLength);

// Hammer automation function
void updateHammerMovement(HammerController& controller, vec3 cameraPosition,
                         float& baseRotation, float& bicepAngle, float& forearmAngle,
                         float deltaTime);

// Arm rendering functions
void renderArmComponents(GLuint cementTopTextureID, GLuint cementBaseTextureID,
                         GLuint woodTextureID, GLuint metalTextureID,
                         int texturedShaderProgram, float baseRotation,
                         float bicepAngle, float forearmAngle,
                         float bicepLength, float forearmLength,
                         GLuint cubeVAO, GLuint sphereVAO, int sphereVertices,
                         GLuint cylinderVAO, int cylinderVertices,
                         int cubeVertices);
