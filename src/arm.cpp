#include "arm.hpp"
#include "graphics.hpp"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Calculate the hammer's world position (for the arm simulation and collision detection)
vec3 calculateHammerPosition(float baseRotation, float bicepAngle, float forearmAngle,
                             float bicepLength, float forearmLength)
{
    mat4 baseRotationMatrix = rotate(mat4(1.0f), radians(baseRotation), vec3(0.0f, 1.0f, 0.0f));

    float jointHeight = bicepLength * cos(radians(bicepAngle));
    float jointHorizontal = bicepLength * sin(radians(bicepAngle));
    float totalAngle = forearmAngle + bicepAngle;

    vec3 hammerLocalPos = vec3(0.0f, forearmLength * cos(radians(totalAngle)) + jointHeight, jointHorizontal + forearmLength * sin(radians(totalAngle)));
    vec3 hammerWorldPos = vec3(baseRotationMatrix * vec4(hammerLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);

    return hammerWorldPos;
}

// Render all arm components (base, bicep, joint, forearm, hammer)
void renderArmComponents(GLuint cementTopTextureID, GLuint cementBaseTextureID,
                         GLuint woodTextureID, GLuint metalTextureID,
                         int texturedShaderProgram, float baseRotation,
                         float bicepAngle, float forearmAngle,
                         float bicepLength, float forearmLength,
                         GLuint cubeVAO, GLuint sphereVAO, int sphereVertices,
                         GLuint cylinderVAO, int cylinderVertices,
                         int cubeVertices)
{
    // Binding Sphere VAO for all sphere components and cube VAO for all cube components
    glBindVertexArray(sphereVAO);

    // Bicep base (sphere)
    glBindTexture(GL_TEXTURE_2D, cementTopTextureID);
    mat4 baseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) *
                      rotate(mat4(1.0f), radians(baseRotation + 180.0f), vec3(0.0f, 1.0f, 0.0f)) *
                      scale(mat4(1.0f), vec3(0.55f, 0.5f, 0.55f));
    setWorldMatrix(texturedShaderProgram, baseMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices);

    // Larger fixed base (cube)
     glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, cementBaseTextureID);
    mat4 bigBaseMatrix = translate(mat4(1.0f), vec3(5.0f, 0.55f, 5.0f)) *
                         scale(mat4(1.0f), vec3(7.1f, 1.3f, 7.1f));
    setWorldMatrix(texturedShaderProgram, bigBaseMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices);

    // Create base rotation matrix for all arm components
    mat4 baseRotationMatrix = rotate(mat4(1.0f), radians(baseRotation), vec3(0.0f, 1.0f, 0.0f));

    // Create bicep (cylinder)
    glBindVertexArray(cylinderVAO);
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    vec3 bicepLocalPos = vec3(0.0f, bicepLength / 2 * cos(radians(bicepAngle)), bicepLength / 2 * sin(radians(bicepAngle)));
    vec3 bicepWorldPos = vec3(baseRotationMatrix * vec4(bicepLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 bicepMatrix = translate(mat4(1.0f), bicepWorldPos) *
                       baseRotationMatrix *
                       rotate(mat4(1.0f), radians(bicepAngle), vec3(1.0f, 0.0f, 0.0f)) *
                       translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f)) *
                       scale(mat4(1.0f), vec3(0.5f, bicepLength/4.0f, 0.5f));
    setWorldMatrix(texturedShaderProgram, bicepMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertices);

    // Create joint (sphere)
    glBindVertexArray(sphereVAO);
    glBindTexture(GL_TEXTURE_2D, metalTextureID);
    float jointHeight = bicepLength * cos(radians(bicepAngle));
    float jointHorizontal = bicepLength * sin(radians(bicepAngle));
    vec3 jointLocalPos = vec3(0.0f, jointHeight, jointHorizontal);
    vec3 jointWorldPos = vec3(baseRotationMatrix * vec4(jointLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 hingeMatrix = translate(mat4(1.0f), jointWorldPos) *
                       baseRotationMatrix *
                       rotate(mat4(1.0f), radians(forearmAngle + bicepAngle), vec3(1.0f, 0.0f, 0.0f)) *
                       scale(mat4(1.0f), vec3(0.35f, 0.35f, 0.35f));
    setWorldMatrix(texturedShaderProgram, hingeMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices);

    // Create forearm (cylinder)
    glBindVertexArray(cylinderVAO);
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    float totalAngle = forearmAngle + bicepAngle;
    vec3 forearmLocalPos = vec3(0.0f, forearmLength / 2 * cos(radians(totalAngle)) + jointHeight,
                                jointHorizontal + forearmLength / 2 * sin(radians(totalAngle)));
    vec3 forearmWorldPos = vec3(baseRotationMatrix * vec4(forearmLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 foreArmMatrix = translate(mat4(1.0f), forearmWorldPos) *
                         baseRotationMatrix *
                         rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) *
                         translate(mat4(1.0f), vec3(0.0f, -1.0f, 0.0f)) *
                         scale(mat4(1.0f), vec3(0.45f, forearmLength/4.0f, 0.45f));
    setWorldMatrix(texturedShaderProgram, foreArmMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cylinderVertices);

    // Create Hammer (cube)
    glBindVertexArray(cubeVAO);
    glBindTexture(GL_TEXTURE_2D, metalTextureID);
    vec3 hammerLocalPos = vec3(0.0f, forearmLength * cos(radians(totalAngle)) + jointHeight,
                               jointHorizontal + forearmLength * sin(radians(totalAngle)));
    vec3 hammerWorldPos = vec3(baseRotationMatrix * vec4(hammerLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 hammerMatrix = translate(mat4(1.0f), hammerWorldPos) *
                        baseRotationMatrix *
                        rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) *
                        scale(mat4(1.0f), vec3(3.0f, 3.0f, 8.0f));
    setWorldMatrix(texturedShaderProgram, hammerMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices);
}
