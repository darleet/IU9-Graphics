#include <cmath>
#include "main.hpp"

float phi = 0.0f;
float theta = 0.0f;

void DrawEllipsoid(int sector_count, int stack_count) {
  float a = 0.7f;
  float b = 0.5f;
  float c = 0.5f;

  GLfloat x, y, z;

  float sector_step = 2.0f * M_PI / sector_count;
  float stack_step = M_PI / stack_count;
  float alpha, beta, next_alpha, next_beta;

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(3.0f);
  for(int i = 0; i <= stack_count; ++i)
  {
    alpha = M_PI / 2 - static_cast<float>(i) * stack_step;
    next_alpha = M_PI / 2 - static_cast<float>(i + 1) * stack_step;

    glBegin(GL_TRIANGLE_STRIP);
    for(int j = 0; j <= sector_count; ++j)
    {
      glColor3f(
          1.0f,
          static_cast<float>(abs(sector_count / 2 - j)) / static_cast<float>(sector_count),
          static_cast<float>(i) / static_cast<float>(stack_count));
      beta = static_cast<float>(j) * sector_step;
      next_beta = static_cast<float>(j + 1) * sector_step;

      // Vertex position (x, y, z)
      x = a * cosf(alpha) * cosf(beta);      // r * cos(u) * cos(v)
      y = b * cosf(alpha) * sinf(beta);      // r * cos(u) * sin(v)
      z = c * sinf(alpha);                   // r * sin(u)
      glVertex3d(x, y, z);

      x = a * cosf(next_alpha) * cosf(next_beta);
      y = b * cosf(next_alpha) * sinf(next_beta);
      z = c * sinf(next_alpha);
      glVertex3d(x, y, z);
    }
    glEnd();
  }
}

void DisplayWindow(GLFWwindow* window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);
  glPushMatrix();

  GLfloat rotate_x[] = {
      1, 0, 0, 0,
      0, cosf(phi), -sinf(phi), 0,
      0, sinf(phi), cosf(phi), 0,
      0, 0, 0, 1
  };
  GLfloat rotate_y[] = {
      cosf(theta), 0, sinf(theta), 0,
      0, 1, 0, 0,
      -sinf(theta), 0, cosf(theta), 0,
      0, 0, 0, 1
  };

  glMatrixMode(GL_MODELVIEW);
  glMultMatrixf(rotate_x);
  glMultMatrixf(rotate_y);

  DrawEllipsoid(50, 15);

  glPopMatrix();
  glFlush();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_RIGHT) {
      theta += 0.1;
    } else if (key == GLFW_KEY_LEFT) {
      theta -= 0.1;
    } else if (key == GLFW_KEY_UP) {
      phi += 0.1;
    } else if (key == GLFW_KEY_DOWN) {
      phi -= 0.1;
    }
  }
}

int main() {
  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(750, 750, "Lab2",
                                        nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, KeyCallback);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  while(!glfwWindowShouldClose(window)) {
    DisplayWindow(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
