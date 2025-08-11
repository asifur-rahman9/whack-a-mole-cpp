#include "game.hpp"
#include "graphics.hpp"
#include "constants.hpp"
#include "arm.hpp"
#include "shaders.hpp"
#include "camera.hpp"
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
                 float forearmLength, float cubeX, float cubeY, float cubeRad, float cubeRot,
                 GLuint sphereVAO, int sphereVertices,
                 GLuint cylinderVAO, int cylinderVertices,
                 int cubeVertices,
                 GLuint grass1VAO, int grass1Vertices,
                 GLuint grass2VAO, int grass2Vertices,
                 GLuint treeVAO, int treeVertices)
{
    // Enable our texture shader program, set the texture location, bind the texture

    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation = glGetUniformLocation(texturedShaderProgram, "textureSampler");
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glUniform1i(textureLocation, 0); // Set our Texture sampler to user Texture Unit 0

    // Bind the appropriate VAO
    glBindVertexArray(vao);

    // Draw ground
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "ambientStrength"), 0.1f);
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "specularStrength"), 0.1f);
    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(1000.0f, 0.02f, 1000.0f));
    setWorldMatrix(texturedShaderProgram, groundWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices);

    // Render all arm components using the dedicated arm module
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "ambientStrength"), 0.2f);
    glUniform1f(glGetUniformLocation(texturedShaderProgram, "specularStrength"), 0.5f);
    renderArmComponents(cementTopTextureID, cementBaseTextureID, woodTextureID, metalTextureID,
                        texturedShaderProgram, baseRotation, bicepAngle, forearmAngle,
                        bicepLength, forearmLength, vao, sphereVAO, sphereVertices,
                        cylinderVAO, cylinderVertices, cubeVertices);

    // draw cube
    vec3 cubePosition = vec3(cubeX + (cubeRad * sin(cubeRot)), cubeY, cubeRad * cos(cubeRot));
    mat4 cubeMatrix = translate(mat4(1.0f), cubePosition) * scale(mat4(1.0f), vec3(2.0f, 2.0f, 2.0f));
    setWorldMatrix(texturedShaderProgram, cubeMatrix);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertices);

    // Render grass
    renderGrass(grassTextureID, texturedShaderProgram, grass1VAO, grass1Vertices, grass2VAO, grass2Vertices);

    // Render trees
    renderTrees(grassTextureID, texturedShaderProgram, treeVAO, treeVertices);

    glUniform1f(glGetUniformLocation(texturedShaderProgram, "ambientStrength"), 0.1f);



}

void renderLights(int lightVAO, int lightShadingProgram, int sphereVertices, vec3 lightPos){
    // Bind the appropriate VAO

    glBindVertexArray(lightVAO);

    // Draw each of the spheres
    glUniform1f(glGetUniformLocation(lightShadingProgram, "alpha"), 0.4f);
    mat4 lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.3f, 0.3f, 0.3f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

    // Draw each of the spheres
    glUniform1f(glGetUniformLocation(lightShadingProgram, "alpha"), 0.4f);
    lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.35f, 0.35f, 0.35f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

    // Draw each of the spheres
    glUniform1f(glGetUniformLocation(lightShadingProgram, "alpha"), 0.4f);
    lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.4f, 0.4f, 0.4f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

     // Draw each of the spheres
    glUniform1f(glGetUniformLocation(lightShadingProgram, "alpha"), 0.3f);
    lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.45f, 0.45f, 0.45f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices

    // Draw each of the spheres
    glUniform1f(glGetUniformLocation(lightShadingProgram, "alpha"), 0.2f);
    lightWorldMatrix = translate(mat4(1.0f), lightPos) * scale(mat4(1.0f), vec3(0.5f, 0.5f, 0.5f));
    setWorldMatrix(lightShadingProgram, lightWorldMatrix);
    glDrawArrays(GL_TRIANGLES, 0, sphereVertices); // counting the sphere vertices




}

vec3* setLights(int LIGHT_NUMBR, int texturedShaderProgram, Shader textureShader, unsigned long millis, int lightingShaderProgram){
            //update lighting parameters
        glUseProgram(texturedShaderProgram);
        //const int LIGHT_NUMBR = 5;
        textureShader.setInt("lightNo", LIGHT_NUMBR);
        float lightRad[LIGHT_NUMBR];
        float val =  (float)((millis / 10) % 3600);
        lightRad[0] = radians(val);
        lightRad[1] = radians(2.0 * val + 45);
        lightRad[2] = radians(1.2 * val + 90);
        lightRad[3] = radians(0.8 * val);
        lightRad[4] = radians(1.3 * val);


        // set the position of each light
        vec3* lightPos = new vec3[LIGHT_NUMBR];
        lightPos[0] = vec3(40.f * sin(lightRad[0]), 58.f, 40.f* cos(lightRad[0]));
        lightPos[1] = vec3(45.f* sin(lightRad[1]), 70.f, 45.f* cos(lightRad[1]));
        lightPos[2] = vec3(40.f* sin(lightRad[2]), 50.f, 20.f* cos(lightRad[2]));
        lightPos[3] = vec3(50.f* sin(lightRad[3]), 75.f, 50.f* cos(lightRad[3]));
        lightPos[4] = vec3(25.f* sin(lightRad[4]), 55.f, 40.f* cos(lightRad[4]));

        //render each light and set its position in the texture shader
        textureShader.setVec3("lightColor", 0.7f, 0.7f, 0.7f);



        return lightPos;
}

void drawLights(int LIGHT_NUMBR, int lightingShaderProgram, GLuint sphereVAO, int sphereVertices, vec3* lightPos, int texturedShaderProgram, Shader textureShader){

    for(int i = 0; i < LIGHT_NUMBR; i++){
            glUseProgram(lightingShaderProgram);
            renderLights(sphereVAO, lightingShaderProgram, sphereVertices, lightPos[i]);

            string lightNo = "lightPos[" + to_string(i) + "]";
            glUseProgram(texturedShaderProgram);
            textureShader.setVec3(lightNo, lightPos[i]);
        }

}

void renderSkybox(Camera camera, int skyboxShaderProgram, Shader skyboxShader, glm::mat4 projectionMatrix, unsigned int skyboxVAO, unsigned int cubemapTexture){
    glm::mat4 skyView = glm::mat4(glm::mat3(camera.getViewMatrix()));
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        glUseProgram(skyboxShaderProgram);
        skyboxShader.setMat4("view", skyView);
        skyboxShader.setMat4("projection", projectionMatrix);
        skyboxShader.setInt("skybox", 0);

        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

}

// Check collision between hammer and cube
// bool checkCollision(vec3 hammerWorldPos, vec3 cubePosition, float baseRotation)
// {
//     float distanceX = abs(hammerWorldPos.x - cubePosition.x);
//     float distanceY = abs(hammerWorldPos.y - cubePosition.y);
//     float distanceZ = abs(hammerWorldPos.z - cubePosition.z);

//     return (distanceX < (4.0f * sin(baseRotation) + 1) && distanceY < 4.0f && distanceZ < (4.0f * cos(baseRotation) + 1));
// }

bool checkCatch(vec3 cameraWorld, vec3 lookAt, vec3 cubePosition){
    vec3 viewPoint = cameraWorld - lookAt;

    float distanceX = abs(viewPoint.x - cubePosition.x);
    float distanceY = abs(viewPoint.y - cubePosition.y);
    float distanceZ = abs(viewPoint.z - cubePosition.z);

    bool xGood = distanceX < 4;
    bool yGood = distanceY < 4;
    bool zGood = distanceZ < 4;


    return xGood & yGood & zGood;


}

// Check if hammer hits the player
bool checkHammerHitsPlayer(vec3 hammerWorldPos, vec3 playerPosition){
    float distance = length(hammerWorldPos - playerPosition);
    return distance < 5.0f;
}


void renderGrass(GLuint grassTextureID, int texturedShaderProgram,
                 GLuint grass1VAO, int grass1Vertices,
                 GLuint grass2VAO, int grass2Vertices)
{
    if (grass1VAO == 0) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);

    for (int x = 0; x < 80; x++) {
        for (int z = 0; z < 80; z++) {

            // Skip some randomly
            if ((x * 37 + z * 23) % 5 == 0) continue;

            float posX = (x - 40) * 1.5f + ((x * 17 + z * 13) % 20 - 10) * 0.1f;
            float posZ = (z - 40) * 1.5f + ((x * 13 + z * 17) % 20 - 10) * 0.1f;

            // Skip arm area
            if ((posX - 5) * (posX - 5) + (posZ - 5) * (posZ - 5) < 36) continue;

            // choice of grass type based on position
            if (x % 3 == 0) {
                glBindVertexArray(grass2VAO);
                setWorldMatrix(texturedShaderProgram, translate(mat4(1.0f), vec3(posX, 0.0f, posZ)));
                glDrawArrays(GL_TRIANGLES, 0, grass2Vertices);
            } else {
                glBindVertexArray(grass1VAO);
                setWorldMatrix(texturedShaderProgram, translate(mat4(1.0f), vec3(posX, 0.0f, posZ)));
                glDrawArrays(GL_TRIANGLES, 0, grass1Vertices);
            }
        }
    }
}

void renderTrees(GLuint grassTextureID, int texturedShaderProgram, GLuint treeVAO, int treeVertices)
{
    if (treeVAO == 0) return;

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, grassTextureID);
    glBindVertexArray(treeVAO);

    for (int x = 0; x < 15; x++) {
        for (int z = 0; z < 15; z++) {

            // Skip some randomly
            if ((x * 7 + z * 11) % 13 != 0) continue;

            float posX = (x - 7) * 8.0f + ((x * 23 + z * 17) % 40 - 20) * 0.2f;
            float posZ = (z - 7) * 8.0f + ((x * 19 + z * 29) % 40 - 20) * 0.2f;

            // Skip arm area
            if ((posX - 5) * (posX - 5) + (posZ - 5) * (posZ - 5) < 49) continue;

            // Varied sizes
            float scale = 2.5f + ((x * 13 + z * 7) % 15) * 0.1f;

            mat4 treeMatrix = translate(mat4(1.0f), vec3(posX, 0.0f, posZ)) *
                             glm::scale(mat4(1.0f), vec3(scale));
            setWorldMatrix(texturedShaderProgram, treeMatrix);
            glDrawArrays(GL_TRIANGLES, 0, treeVertices);
        }
    }
}
