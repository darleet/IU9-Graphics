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

using std::sin, std::cos;

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
        1, 0, 0, 0.8,
        0, 1, 0, 0.8,
        0, 0, 1, 1.5,
        0, 0, 0, 1
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
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);

    GLfloat transMatrix[] = {
            1, 0, 0, 0,
            0, 1, 0,0,
            0, 0, 1, 0,
            -0.5, -0.5, 0, 1
    };
    glMultMatrixf(transMatrix);
    glMultMatrixf(projMatrix);
    GLfloat rotX[] = {
            1, 0, 0, 0,
            0, cos(alpha), -sin(alpha), 0,
            0, sin(alpha), cos(alpha), 0,
            0, 0, 0, 1
    };
    GLfloat rotY[] = {
            cos(beta), 0, sin(beta), 0,
            0, 1, 0, 0,
            -sin(beta), 0, cos(beta), 0,
            0, 0, 0, 1
    };
    glMultMatrixf(rotX);
    glMultMatrixf(rotY);
    cube(0.2);
    glPopMatrix();

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    fz = std::sqrt(x*x+y*y);
    theta = asin(fz/sqrt(2)) + alpha;
    phi = asin(fz/sqrt(2-fz*fz)) + beta;
    GLfloat transMatrix2[] = {
            1, 0, 0, 0,
            0, 1, 0,0,
            0, 0, 1, 0,
            0.5, 0.5, 0, 1
    };
    glMultMatrixf(transMatrix2);
    cube(0.2);

    glfwSwapBuffers(window);
    glfwPollEvents();
}
