#include "main.h"
#include <cmath>

int width = 750;
int height = 750;
float alpha = 0.7f;
float beta = 0.8f;
bool fill = true;

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

void draw_cube(float p) {
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

GLfloat proj[] = {
        1, 0, 0, 0.8,
        0, 1, 0, 0.8,
        0, 0, 1, 0.8,
        0, 0, 0, 1
};

GLfloat trans1[] = {
        1, 0, 0, 0,
        0, 1, 0,0,
        0, 0, 1, 0,
        -0.5, -0.5, 0, 1
};

GLfloat trans2[] = {
        1, 0, 0, 0,
        0, 1, 0,0,
        0, 0, 1, 0,
        0.5, 0.5, 0, 1
};

void display(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);

    glMultMatrixf(trans1);
    glMultMatrixf(proj);
    GLfloat rotX1[] = {
            1, 0, 0, 0,
            0, cos(alpha), -sin(alpha), 0,
            0, sin(alpha), cos(alpha), 0,
            0, 0, 0, 1
    };
    GLfloat rotY1[] = {
            cos(beta), 0, sin(beta), 0,
            0, 1, 0, 0,
            -sin(beta), 0, cos(beta), 0,
            0, 0, 0, 1
    };
    glMultMatrixf(rotX1);
    glMultMatrixf(rotY1);
    draw_cube(0.3);
    glPopMatrix();

    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    glMultMatrixf(trans2);
    GLfloat rotX2[] = {
            1, 0, 0, 0,
            0, cos(0.7f), -sin(0.7f), 0,
            0, sin(0.7f), cos(0.7f), 0,
            0, 0, 0, 1
    };
    GLfloat rotY2[] = {
            cos(0.7f), 0, sin(0.7f), 0,
            0, 1, 0, 0,
            -sin(0.7f), 0, cos(0.7f), 0,
            0, 0, 0, 1
    };
    glMultMatrixf(rotX2);
    glMultMatrixf(rotY2);
    draw_cube(0.3);
    glPopMatrix();

    glfwSwapBuffers(window);
    glfwPollEvents();
}
