#pragma once

#include <glm/glm.hpp>

// Forward declaration to avoid including GLFW in header
struct GLFWwindow;

using namespace glm;

// Camera class to encapsulate all camera functionality
class Camera
{
private:
    // Camera position and orientation
    vec3 position;
    vec3 lookAt;
    vec3 up;

    // Camera parameters
    float speed;
    float fastSpeedMultiplier;
    float horizontalAngle;
    float verticalAngle;
    bool firstPerson;

    // Mouse tracking
    double lastMousePosX;
    double lastMousePosY;

public:
    // Constructor
    Camera(vec3 initialPosition = vec3(-35.0f, 20.0f, -35.0f),
           vec3 initialLookAt = vec3(1.0f, -0.3f, 1.0f),
           vec3 initialUp = vec3(0.0f, 1.0f, 0.0f));

    // Camera control methods
    void handleInput(GLFWwindow *window, float deltaTime);
    void updateViewMatrix(int colorShaderProgram, int texturedShaderProgram, int lightingShaderProgram);

    // Getters
    vec3 getPosition() const { return position; }
    vec3 getLookAt() const { return lookAt; }
    vec3 getUp() const { return up; }
    mat4 getViewMatrix() const;

    // Setters
    void setSpeed(float newSpeed) { speed = newSpeed; }
    void setPosition(vec3 newPosition) { position = newPosition; }
    void setLookAt(vec3 newLookAt) { lookAt = newLookAt; }
};
