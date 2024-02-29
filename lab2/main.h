#ifndef PROGRAM_MAIN_H
#define PROGRAM_MAIN_H

#include <cstdlib>
#include "GLFW/glfw3.h"

int main();
void key_callback(GLFWwindow*, int, int, int, int);
void display(GLFWwindow*);

#endif //PROGRAM_MAIN_H