#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

using namespace glm;

// Input handling functions
void handleRobotArmInput(GLFWwindow *window, float &bicepAngle, float &forearmAngle, float &baseRotation);

bool handleGeneralInput(GLFWwindow *window);
