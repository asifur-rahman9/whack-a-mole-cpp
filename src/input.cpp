#include "input.hpp"
#include "constants.hpp"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;


// Handle general input (escape, mouse clicks)
bool handleGeneralInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        return true; // Signal to close window

    return false; // Continue running
}
