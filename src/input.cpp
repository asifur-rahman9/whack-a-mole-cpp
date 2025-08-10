#include "input.hpp"
#include "constants.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

// Handle robotic arm input controls
void handleRobotArmInput(GLFWwindow *window, float &bicepAngle, float &forearmAngle, float &baseRotation)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) // move robot bicep to the left
    {
        if (bicepAngle > -45.0)
            bicepAngle = bicepAngle - ARM_ROTATION_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) // move robot bicep to the right
    {
        if (bicepAngle < 45.0)
            bicepAngle = bicepAngle + ARM_ROTATION_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) // move forearm up/to the left
    {
        if (forearmAngle > -140)
            forearmAngle += -FOREARM_ROTATION_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) // move forearm down/to the right
    {
        if (forearmAngle < 140)
            forearmAngle += FOREARM_ROTATION_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // rotate base right
    {
        baseRotation -= BASE_ROTATION_SPEED;
        if (baseRotation < -360.0f)
            baseRotation += 360.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) // rotate base left
    {
        baseRotation += BASE_ROTATION_SPEED;
        if (baseRotation > 360.0f)
            baseRotation -= 360.0f;
    }
}

// Handle general input (escape, mouse clicks)
bool handleGeneralInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return true; // Signal to close window

    return false; // Continue running
}
