#include "main.h"

GLfloat angle = 30.0;

int main() {
    if (!glfwInit()) {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow(900, 900, "Lab1",
                                          nullptr, nullptr);

    if (window == nullptr) {
        glfwTerminate();
        return 1;
    }
    
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);

    while(!glfwWindowShouldClose(window)) {
        display(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_RIGHT) {
            angle -= 15;
        }
        if (key == GLFW_KEY_LEFT) {
            angle += 15;
        }
        if (key == 'Q') {
            glfwTerminate();
            exit(0);
        }
    }
}

void display(GLFWwindow* window) {
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glPushMatrix();
    glRotatef(angle, 0, 0, 1);

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(-0.5f, -0.5f);

    glVertex2f(-0.5f, 0.3f);
    glVertex2f(-0.3f, 0.5f);

    glVertex2f(0.2f, 0.5f);
    glVertex2f(0.5f, 0.2f);

    glVertex2f(0.5f, -0.3f);
    glVertex2f(0.3f, -0.5f);
    glEnd();

    glPopMatrix();
    glfwSwapBuffers(window);
    glfwPollEvents();
}
