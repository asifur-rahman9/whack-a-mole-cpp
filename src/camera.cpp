// OpenGL headers first (GLEW before GLFW)
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "camera.hpp"
#include "constants.hpp"
#include "graphics.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

using namespace std;

// Constructor
Camera::Camera(vec3 initialPosition, vec3 initialLookAt, vec3 initialUp)
    : position(initialPosition), lookAt(initialLookAt), up(initialUp)
{
    speed = CAMERA_SPEED;
    fastSpeedMultiplier = CAMERA_FAST_SPEED_MULTIPLIER;
    horizontalAngle = 45.0f;
    verticalAngle = -10.0f;
    firstPerson = true;

    // Initialize mouse position
    lastMousePosX = 0.0;
    lastMousePosY = 0.0;
    // Initialize gravity and velocity
    velocity = {0.0f, 0.0f, 0.0f};
    gravity = {0.0f, -10.0f, 0.0f};
    jumpSpeed = 10.0f;
    jump = false;

}

// Handle all camera input including movement and look around
void Camera::handleInput(GLFWwindow *window, float deltaTime)
{
    // Handle camera speed based on shift key
    bool fastCam = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
                   glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    float currentSpeed = fastCam ? (speed * fastSpeedMultiplier) : speed;

    // Handle mouse input for camera rotation
    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    double dx = mousePosX - lastMousePosX;
    double dy = mousePosY - lastMousePosY;

    lastMousePosX = mousePosX;
    lastMousePosY = mousePosY;

    // Apply mouse movement to camera angles
    const float lookSensitivity = 0.1f;
    if (firstPerson)
    {
        horizontalAngle += dx * lookSensitivity; 
        verticalAngle -= dy * lookSensitivity;

        // Clamp vertical angle to prevent flipping
        verticalAngle = glm::max(-85.0f, glm::min(85.0f, verticalAngle));

        // Update lookAt direction based on angles
        lookAt.x = cos(radians(verticalAngle)) * cos(radians(horizontalAngle));
        lookAt.y = sin(radians(verticalAngle));
        lookAt.z = cos(radians(verticalAngle)) * sin(radians(horizontalAngle));
        lookAt = normalize(lookAt);
    }

    // Handle keyboard input for camera movement
    vec3 sideVector = cross(lookAt, up);
    sideVector = normalize(sideVector);

    // WASD movement
    // if player is knocked back or jumping they can't change their velocity
    if(knockedBack == false){
        if(jump == false){
            if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                {position += vec3(lookAt.x, 0.0f, lookAt.z) * currentSpeed * deltaTime;
                keyVelocity = currentSpeed;
                velocity = vec3(0.0f);}

            else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                {position -= vec3(lookAt.x, 0.0f, lookAt.z) * currentSpeed * deltaTime;
                keyVelocity = -currentSpeed;
                velocity = vec3(0.0f);}
            else
                {keyVelocity = 0;}
        }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        position -= sideVector * currentSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        position += sideVector * currentSpeed * deltaTime;

    }


    if(position.y > PLAYER_HEIGHT){
        //they've jumped
        velocity = velocity + gravity * deltaTime;

    } else if(position.y <= PLAYER_HEIGHT){
        //they're on the ground
        if(jump == true)
            velocity = vec3(0.0f);
        velocity.y = 0.0f;
        position.y = PLAYER_HEIGHT;
        jump = false;
    }

    // space allows the player to jump
    if ((glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) && position.y == PLAYER_HEIGHT)
    {
        //add a verticle velocity componenet while mainting their horizontal movement
        velocity.y =  velocity.y + jumpSpeed;
        velocity = velocity + (vec3(lookAt.x, 0.0f, lookAt.z) * keyVelocity);
        jump = true;

    }

    // update position based on velocity, update velocity based on friction
    position = position + (velocity * deltaTime);
    velocity.x = 0.98 * velocity.x;
    velocity.z = 0.98 * velocity.z;

    // the player is not able to move until their knockback velocity has decreased
    if(knockedBack == true){
        if(length(velocity) < 3.0f){
            knockedBack = false;
        }
    }

}

// the player gets knocked away from the hammer if hit by the hammer
void Camera::knockBack(vec3 hammerPosition){
    if(knockedBack == false){
        vec3 knockbackDirection = normalize(position - hammerPosition);
        vec3 knockbackVelocity = vec3(knockbackDirection.x, 0.01f * knockbackDirection.y, knockbackDirection.z);
        knockbackVelocity = normalize(knockbackVelocity);
        knockbackVelocity *= 25.0f;

        velocity = knockbackVelocity;
        jump = false;
    }

    knockedBack = true;
}

// Update view matrix for both shader programs
void Camera::updateViewMatrix(int shaderProgram)
{
    mat4 viewMatrix = getViewMatrix();
    setViewMatrix(shaderProgram, viewMatrix);

}

// Get the current view matrix
mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, position + lookAt, up);
}
