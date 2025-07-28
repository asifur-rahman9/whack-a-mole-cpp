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
                         float bicepLength, float forearmLength)
{
    // Bicep base
    glBindTexture(GL_TEXTURE_2D, cementTopTextureID);
    mat4 baseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) *
                      rotate(mat4(1.0f), radians(baseRotation + 180.0f), vec3(0.0f, 1.0f, 0.0f)) *
                      scale(mat4(1.0f), vec3(4.1f, 2.4f, 4.1f));
    setWorldMatrix(texturedShaderProgram, baseMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Larger fixed base
    glBindTexture(GL_TEXTURE_2D, cementBaseTextureID);
    mat4 bigBaseMatrix = translate(mat4(1.0f), vec3(5.0f, 1.f, 5.0f)) *
                         scale(mat4(1.0f), vec3(6.1f, 1.6f, 6.1f));
    setWorldMatrix(texturedShaderProgram, bigBaseMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Create base rotation matrix for all arm components
    mat4 baseRotationMatrix = rotate(mat4(1.0f), radians(baseRotation), vec3(0.0f, 1.0f, 0.0f));

    // Create bicep
    glBindTexture(GL_TEXTURE_2D, woodTextureID);
    vec3 bicepLocalPos = vec3(0.0f, bicepLength / 2 * cos(radians(bicepAngle)), bicepLength / 2 * sin(radians(bicepAngle)));
    vec3 bicepWorldPos = vec3(baseRotationMatrix * vec4(bicepLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 bicepMatrix = translate(mat4(1.0f), bicepWorldPos) *
                       baseRotationMatrix *
                       rotate(mat4(1.0f), radians(bicepAngle), vec3(1.0f, 0.0f, 0.0f)) *
                       scale(mat4(1.0f), vec3(1.0f, bicepLength, 1.0f));
    setWorldMatrix(texturedShaderProgram, bicepMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Create joint
    float jointHeight = bicepLength * cos(radians(bicepAngle));
    float jointHorizontal = bicepLength * sin(radians(bicepAngle));
    vec3 jointLocalPos = vec3(0.0f, jointHeight, jointHorizontal);
    vec3 jointWorldPos = vec3(baseRotationMatrix * vec4(jointLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 hingeMatrix = translate(mat4(1.0f), jointWorldPos) *
                       baseRotationMatrix *
                       rotate(mat4(1.0f), radians(forearmAngle + bicepAngle), vec3(1.0f, 0.0f, 0.0f)) *
                       scale(mat4(1.0f), vec3(2.1f, 2.1f, 2.1f));
    setWorldMatrix(texturedShaderProgram, hingeMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Create forearm
    float totalAngle = forearmAngle + bicepAngle;
    vec3 forearmLocalPos = vec3(0.0f, forearmLength / 2 * cos(radians(totalAngle)) + jointHeight,
                                jointHorizontal + forearmLength / 2 * sin(radians(totalAngle)));
    vec3 forearmWorldPos = vec3(baseRotationMatrix * vec4(forearmLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 foreArmMatrix = translate(mat4(1.0f), forearmWorldPos) *
                         baseRotationMatrix *
                         rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) *
                         scale(mat4(1.0f), vec3(1.0f, forearmLength, 1.0f));
    setWorldMatrix(texturedShaderProgram, foreArmMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Create Hammer
    glBindTexture(GL_TEXTURE_2D, metalTextureID);
    vec3 hammerLocalPos = vec3(0.0f, forearmLength * cos(radians(totalAngle)) + jointHeight,
                               jointHorizontal + forearmLength * sin(radians(totalAngle)));
    vec3 hammerWorldPos = vec3(baseRotationMatrix * vec4(hammerLocalPos, 1.0f)) + vec3(5.0f, 0.0f, 5.0f);
    mat4 hammerMatrix = translate(mat4(1.0f), hammerWorldPos) *
                        baseRotationMatrix *
                        rotate(mat4(1.0f), radians(totalAngle), vec3(1.0f, 0.0f, 0.0f)) *
                        scale(mat4(1.0f), vec3(2.0f, 2.0f, 8.0f));
    setWorldMatrix(texturedShaderProgram, hammerMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}
