#ifndef PROGRAM_MAIN_HPP
#define PROGRAM_MAIN_HPP

#include <cstdlib>
#include "GLFW/glfw3.h"

int main();
void KeyCallback(GLFWwindow*, int, int, int, int);
void DrawEllipsoid(int, int);
void DisplayWindow(GLFWwindow*);

#endif //PROGRAM_MAIN_HPP