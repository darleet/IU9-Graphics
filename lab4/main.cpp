#include <vector>
#include "main.hpp"

struct Pixel {
  int x;
  int y;
};

int window_width = 900;
int window_height = 900;
GLubyte border_color[3] = {255, 0, 255};
GLubyte fill_color[3] = {255, 255, 0};
GLubyte *frame_buffer = new GLubyte[window_width * window_height * 3];

std::vector<Pixel> pixels = std::vector<Pixel>();
Pixel inner_pixel;

bool filled = false; // press F to change state
bool closed = false; // press C to change state

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    if (key == 'F') {
      filled = !filled;
    }
    if (key == 'C') {
      closed = !closed;
    }
    if (key == 'Q') {
      glfwTerminate();
      exit(0);
    }
  }
}


void MouseButtonCallback(GLFWwindow *window, int btn, int action, int mods) {
  if (btn == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    pixels.push_back({static_cast<int>(x), static_cast<int>(window_height - y)});
  }
}

void ClearBuffer() {
  for (int i = 0; i < window_height * window_width * 3; ++i) {
    frame_buffer[i] = 0;
  }
}

void SetPixel(int x, int y, const GLubyte* color) {
  int index = 3 * (x + y * window_width);
  frame_buffer[index] = color[0];
  frame_buffer[index + 1] = color[1];
  frame_buffer[index + 2] = color[2];
}

void DrawLine(int x1, int y1, int x2, int y2) {
  const int delta_x = abs(x2 - x1);
  const int delta_y = abs(y2 - y1);
  const int sign_x = x1 < x2 ? 1 : -1;
  const int sign_y = y1 < y2 ? 1 : -1;
  int error = delta_x - delta_y;

  SetPixel(x2, y2, border_color);

  int error2;
  while (x1 != x2 || y1 != y2) {
    SetPixel(x1, y1, border_color);
    error2 = error * 2;

    if (error2 > -delta_y) {
      error -= delta_y;
      x1 += sign_x;
    }

    if (error2 < delta_x) {
      error += delta_x;
      y1 += sign_y;
    }

  }
}

void DisplayWindow(GLFWwindow* window) {
  glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
  glPushMatrix();

  glViewport(0, 0, window_width, window_height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glClear(GL_COLOR_BUFFER_BIT);

  ClearBuffer();

  if (pixels.size() == 1) {
    SetPixel(pixels[0].x, pixels[0].y, border_color);
  }

  for (int i = 1; i < pixels.size(); ++i) {
    DrawLine(pixels[i - 1].x, pixels[i - 1].y, pixels[i].x, pixels[i].y);
  }

  if (pixels.size() > 2 && closed) {
    DrawLine(pixels.rbegin()->x, pixels.rbegin()->y, pixels[0].x, pixels[0].y);
  }

  glRasterPos2i(-1, -1);
  glDrawPixels(window_width, window_height, GL_RGB, GL_UNSIGNED_BYTE,
               frame_buffer);

  glPopMatrix();
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int main() {
  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Lab1",
                                        nullptr, nullptr);

  if (window == nullptr) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSetKeyCallback(window, KeyCallback);
  glfwSetMouseButtonCallback(window, MouseButtonCallback);

  while(!glfwWindowShouldClose(window)) {
    DisplayWindow(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
