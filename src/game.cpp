#include "game.hpp"
#include "graphics.hpp"
#include "constants.hpp"
#include "arm.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Update game logic including cube positioning and timing
void updateGameLogic(bool &newCube, float &cubeRad, float &cubeRot, float &cubeY,
                     unsigned long millis, unsigned long &baseTime, unsigned long reset)
{
    if (millis - baseTime > reset)
    {
        newCube = true;
        baseTime = millis;
        cout << "New cube position!" << endl;
    }

    if ((millis - baseTime) % 1000 < 15)
    {
        cout << "Time left: " << 10 - ((millis - baseTime) / 1000) << endl;
    }

    // calculate new cube position if necessary
    if (newCube == true)
    {
        // choose new cube position
        cubeRad = (rand() % (MAX_CUBE_RADIUS - MIN_CUBE_RADIUS + 1)) + MIN_CUBE_RADIUS;
        cubeRot = rand() % 360;
        cubeY = rand() % (MAX_CUBE_HEIGHT - MIN_CUBE_HEIGHT + 1) + MIN_CUBE_HEIGHT;
        newCube = false;
    }
}

// Render entire scene (including ground, arm components, and cube)
void renderScene(GLuint grassTextureID, GLuint cementTopTextureID, GLuint cementBaseTextureID,
                 GLuint woodTextureID, GLuint metalTextureID, int texturedShaderProgram, int vao,
                 float baseRotation, float bicepAngle, float forearmAngle, float bicepLength,
                 float forearmLength, float cubeX, float cubeY, float cubeRad, float cubeRot)
{
    // Enable our texture shader program, set the texture location, bind the texture
    
    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glUniform1i(textureLocation, 0); // Set our Texture sampler to user Texture Unit 0

    // Bind the appropriate VAO
    glBindVertexArray(vao);

    // Draw ground
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "specularStrength"), 0.2f);
    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
    setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

    // Render all arm components using the dedicated arm module
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "specularStrength"), 0.5f);
    renderArmComponents(cementTopTextureID, cementBaseTextureID, woodTextureID, metalTextureID,
                        texturedShaderProgram, baseRotation, bicepAngle, forearmAngle,
                        bicepLength, forearmLength);

    // draw cube
    vec3 cubePosition = vec3(cubeX + (cubeRad * sin(cubeRot)), cubeY, cubeRad * cos(cubeRot));
    mat4 cubeMatrix = translate(mat4(1.0f), cubePosition) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
    setWorldMatrix(texturedShaderProgram, cubeMatrix);
    glDrawArrays(GL_TRIANGLES, 0, 36);



}

void renderLights(int lightVAO, int lightShadingProgram, int sphereVertices, vec3 lightPos){
    // Bind the appropriate VAO
    
    glBindVertexArray(lightVAO);

    // Draw each of the spheres
    mat4 lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

    


}

// Check collision between hammer and cube
bool checkCollision(vec3 hammerWorldPos, vec3 cubePosition, float baseRotation)
{
    float distanceX = abs(hammerWorldPos.x - cubePosition.x);
    float distanceY = abs(hammerWorldPos.y - cubePosition.y);
    float distanceZ = abs(hammerWorldPos.z - cubePosition.z);

    return (distanceX < (4.0f * sin(baseRotation) + 1) && distanceY < 4.0f && distanceZ < (4.0f * cos(baseRotation) + 1));
}
