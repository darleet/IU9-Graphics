#include "main.h"
#include <cmath>

int width = 900;
int height = 900;
float alpha = 0;
float beta = 0;
bool fill = true;

float theta = 0.61557763;
float phi = 0.785398;
float fz = 0;

const float x = 0.7;
const float y = 0.7;

int main() {
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(width, height, "Lab2",
                                          nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    while(!glfwWindowShouldClose(window)) {
        display(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        if (key == GLFW_KEY_RIGHT) {
            beta += 0.1;
        } else if (key == GLFW_KEY_LEFT) {
            beta -= 0.1;
        } else if (key == GLFW_KEY_UP) {
            alpha += 0.1;
        } else if (key == GLFW_KEY_DOWN) {
            alpha -= 0.1;
        } else if (key == GLFW_KEY_F) {
            fill = !fill;
            if (fill) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            }
        }
    }
}

GLfloat projMatrix[] = {
        0.87, 0, 0.5, 0,
        -0.09, 0.98, 0.15, 0,
        0.98, 0.35, -1.7, 1,
        0.49, 0.17, -0.85, 2
};

void cube(float p) {
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(-p/2, -p/2, -p/2);
    glVertex3f(-p/2,  p/2, -p/2);
    glVertex3f(-p/2,  p/2,  p/2);
    glVertex3f(-p/2, -p/2,  p/2);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f( p/2, -p/2, -p/2);
    glVertex3f( p/2, -p/2,  p/2);
    glVertex3f( p/2,  p/2,  p/2);
    glVertex3f( p/2,  p/2, -p/2);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-p/2, -p/2, -p/2);
    glVertex3f(-p/2, -p/2,  p/2);
    glVertex3f( p/2, -p/2,  p/2);
    glVertex3f( p/2, -p/2, -p/2);
    glColor3f(1.0, 1.0, 0.0);
    glVertex3f(-p/2, p/2, -p/2);
    glVertex3f(-p/2, p/2,  p/2);
    glVertex3f( p/2, p/2,  p/2);
    glVertex3f( p/2, p/2, -p/2);
    glColor3f(0.0, 1.0, 1.0);
    glVertex3f(-p/2, -p/2, -p/2);
    glVertex3f( p/2, -p/2, -p/2);
    glVertex3f( p/2,  p/2, -p/2);
    glVertex3f(-p/2,  p/2, -p/2);
    glColor3f(1.0, 0.0, 1.0);
    glVertex3f(-p/2, -p/2,  p/2);
    glVertex3f( p/2, -p/2,  p/2);
    glVertex3f( p/2,  p/2,  p/2);
    glVertex3f(-p/2,  p/2,  p/2);
    glEnd();
}

void display(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glViewport(0, 0, width/2, height/2);
    glMultMatrixf(projMatrix);
    GLfloat transMatrix[] = {
            std::cos(phi), std::sin(phi)*std::sin(theta), std::sin(theta)*std::cos(theta), 0,
            0, std::cos(theta), -std::sin(theta), 0,
            std::sin(phi), -std::cos(phi)*std::sin(theta), -std::cos(phi)*std::cos(theta), 0,
            0, 0, 0, 1
    };
    glMultMatrixf(transMatrix);
    cube(1);

    glViewport(width/2, height/2, width/2, height/2);
    glLoadIdentity();
    fz = std::sqrt(x*x+y*y);
    theta = asin(fz/sqrt(2)) + alpha;
    phi = asin(fz/sqrt(2-fz*fz)) + beta;
    glMultMatrixf(transMatrix);
    cube(1);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
