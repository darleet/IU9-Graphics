#include <vector>
#include "main.hpp"

struct Pixel {
  int x;
  int y;
};

int window_width = 900;
int window_height = 900;
GLubyte bg_color[3] = {0, 0, 0};
GLubyte border_color[3] = {255, 0, 255};
GLubyte fill_color[3] = {255, 255, 0};
GLubyte *frame_buffer = new GLubyte[window_width * window_height * 3];

std::vector<Pixel> pixels = std::vector<Pixel>();

bool filled = false; // press F to change state
bool closed = false; // press C to change state
bool filtered = false; // press R to change state

void ClearBuffer() {
  for (int i = 0; i < window_height * window_width * 3; ++i) {
    frame_buffer[i] = 0;
  }
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    if (key == 'F') {
      filled = !filled;
    }
    if (key == 'C') {
      closed = !closed;
    }
    if (key == 'R') {
      filtered = !filtered;
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

void SetPixel(int x, int y, const GLubyte r, const GLubyte g, const GLubyte b) {
  int index = 3 * (x + y * window_width);
  frame_buffer[index] = r;
  frame_buffer[index + 1] = g;
  frame_buffer[index + 2] = b;
}

void Filter() {
  for (int x = 1; x < window_width - 1; x++) {
    for (int y = 1; y < window_height - 1; y++) {
      GLubyte r, g, b;
      r = g = b = 0;

      r += frame_buffer[3 * (x - 1 + (y - 1) * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x + (y - 1) * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x + 1 + (y - 1) * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x - 1 + y * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x + 1 + y * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x - 1 + (y + 1) * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x + (y + 1) * window_width)] * 1.0/16.0;
      r += frame_buffer[3 * (x + 1 + (y + 1) * window_width)] * 1.0/16.0;

      r += frame_buffer[3 * (x + y * window_width)] * 1.0/2.0;

      g += frame_buffer[3 * (x - 1 + (y - 1) * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x + (y - 1) * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x + 1 + (y - 1) * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x - 1 + y * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x + 1 + y * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x - 1 + (y + 1) * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x + (y + 1) * window_width) + 1] * 1.0/16.0;
      g += frame_buffer[3 * (x + 1 + (y + 1) * window_width) + 1] * 1.0/16.0;

      g += frame_buffer[3 * (x + y * window_width) + 1] * 1.0/2.0;

      b += frame_buffer[3 * (x - 1 + (y - 1) * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x + (y - 1) * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x + 1 + (y - 1) * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x - 1 + y * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x + 1 + y * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x - 1 + (y + 1) * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x + (y + 1) * window_width) + 2] * 1.0/16.0;
      b += frame_buffer[3 * (x + 1 + (y + 1) * window_width) + 2] * 1.0/16.0;

      b += frame_buffer[3 * (x + y * window_width) + 2] * 1.0/2.0;

      SetPixel(x, y, r, g, b);
    }
  }
}

bool IsPixelSet(int x, int y) {
  int index = 3 * (x + y * window_width);
  return frame_buffer[index] == border_color[0] &&
    frame_buffer[index + 1] == border_color[1] &&
    frame_buffer[index + 2] == border_color[2];
}

bool IsPixelBG(int x, int y) {
  int index = 3 * (x + y * window_width);
  return frame_buffer[index] == bg_color[0] &&
    frame_buffer[index + 1] == bg_color[1] &&
    frame_buffer[index + 2] == bg_color[2];
}

bool IsPixelFill(int x, int y) {
  int index = 3 * (x + y * window_width);
  return frame_buffer[index] == fill_color[0] &&
    frame_buffer[index + 1] == fill_color[1] &&
    frame_buffer[index + 2] == fill_color[2];
}

void FillRectangle(int width, int height) {
  bool is_inside, prev_border;
  for (int i = 1; i < height - 1; i++) {
    is_inside = false;
    prev_border = false;
    for (int j = 0; j < width; j++) {
      if (IsPixelSet(j, i) && !prev_border) {
        is_inside = is_inside ^ IsPixelSet(j, i);
      }
      if (is_inside && !IsPixelSet(j, i)) {
        SetPixel(j, i, fill_color[0], fill_color[1], fill_color[2]);
      }
      prev_border = IsPixelSet(j, i);
    }

    if (is_inside) {
      for (int j = 0; j < width; j++) {
        if (IsPixelBG(j, i) && (IsPixelFill(j, i - 1) || IsPixelFill(j, i + 1))) {
          SetPixel(j, i, fill_color[0], fill_color[1], fill_color[2]);
        } else if (IsPixelFill(j, i) && (IsPixelBG(j, i - 1) || IsPixelBG(j, i + 1))) {
          SetPixel(j, i, bg_color[0], bg_color[1], bg_color[2]);
        }
      }
    }
  }
}

void DrawLine(int x1, int y1, int x2, int y2) {
  const int delta_x = abs(x2 - x1);
  const int delta_y = abs(y2 - y1);
  const int sign_x = x1 < x2 ? 1 : -1;
  const int sign_y = y1 < y2 ? 1 : -1;
  int error = delta_x - delta_y;

  SetPixel(x2, y2, border_color[0], border_color[1], border_color[2]);

  int error2;
  while (x1 != x2 || y1 != y2) {
    SetPixel(x1, y1, border_color[0], border_color[1], border_color[2]);
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

  ClearBuffer();

  if (pixels.size() == 1) {
    SetPixel(pixels[0].x, pixels[0].y, border_color[0], border_color[1], border_color[2]);
  }

  for (int i = 1; i < pixels.size(); ++i) {
    DrawLine(pixels[i - 1].x, pixels[i - 1].y, pixels[i].x, pixels[i].y);
  }

  if (pixels.size() > 2 && closed) {
    DrawLine(pixels.rbegin()->x, pixels.rbegin()->y, pixels[0].x, pixels[0].y);
    if (filled) {
      FillRectangle(window_width, window_height);
    }
  }

  if (filtered) {
    Filter();
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

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Lab4",
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
  delete[] frame_buffer;
  return 0;
}
