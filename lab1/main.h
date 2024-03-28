#ifndef PROGRAM_MAIN_H
#define PROGRAM_MAIN_H

#include <cstdlib>
#include "GLFW/glfw3.h"

int main();
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
void DisplayWindow(GLFWwindow *window);

#endif //PROGRAM_MAIN_H