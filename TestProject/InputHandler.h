#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void processInput();
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouseCallback(GLFWwindow* window, double xpos, double ypos);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);