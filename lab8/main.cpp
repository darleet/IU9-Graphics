#include <iostream>
#include <chrono>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>

int window_width = 900;
int window_height = 900;

int anglez = 0;
int anglex = 0;
int angley = 0;

float delta_y = 0;
bool is_falling = false;
bool is_jumping = false;
float current_vel = 0;
bool tex = true;

auto begin_ti = std::chrono::steady_clock::now();
auto begin_time = std::chrono::steady_clock::now();

GLuint shaderProgram;

const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    TexCoord = aTexCoord;
}
)";

const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

void main() {
    FragColor = texture(texture1, TexCoord);
}
)";

void checkCompileErrors(GLuint shader, std::string type) {
  GLint success;
  GLchar infoLog[1024];
  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "| ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog <<
      "\n -- --------------------------------------------------- -- " << std::endl;
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
      std::cout << "| ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog <<
      "\n -- --------------------------------------------------- -- " << std::endl;
    }
  }
}

GLuint LoadBMP(const char* image) {
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;
  unsigned char* data;

  FILE* file = fopen(image, "rb");
  if (!file) {
    std::cout << "BMP file not found" << std::endl;
    return 0;
  }

  if (fread(header, 1, 54, file) != 54) {
    std::cout << "Incorrect BMP file" << std::endl;
    return 0;
  }

  if (header[0] != 'B' || header[1] != 'M') {
    std::cout << "Incorrect BMP file" << std::endl;
    return 0;
  }

  imageSize = *(int*)&(header[0x22]);
  width = *(int*)&(header[0x12]);
  height = *(int*)&(header[0x16]);

  if (imageSize == 0) imageSize = width * height * 3;
  data = new unsigned char[imageSize];
  fread(data, 1, imageSize, file);
  fclose(file);

  GLuint textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR,
               GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  delete[] data;
  return textureID;
}

void ViewCube() {
  auto end = std::chrono::steady_clock::now();
  auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_ti);
  auto full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);

  if (is_falling && delta_y < -0.7) {
    is_jumping = true;
    is_falling = false;
    begin_time = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
    full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);
  }

  if (is_jumping && delta_y > 0) {
    is_falling = true;
    is_jumping = false;
    begin_time = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
    full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);
  }

  if (is_falling && elapsed_ms > std::chrono::milliseconds(1)) {
    begin_ti = end;
    delta_y = -9.8 * pow((full_ms.count() / 1000.0), 2) / 2;
    current_vel = 9.8 * (full_ms.count() / 1000.0);
  } else if (is_jumping && elapsed_ms > std::chrono::milliseconds(10)) {
    begin_ti = end;
    delta_y = current_vel * (full_ms.count() / 1000.0) - 0.7 - 9.8 * pow((full_ms.count() / 1000.0), 2) / 2;
  }

  GLfloat vertices[] = {
      // Positions          // Texture Coords
      0.3f, -0.3f + delta_y,  0.3f,  1.0f, 0.0f,
      0.3f, -0.3f + delta_y, -0.3f,  1.0f, 1.0f,
      -0.3f, -0.3f + delta_y, -0.3f,  0.0f, 1.0f,
      -0.3f, -0.3f + delta_y,  0.3f,  0.0f, 0.0f,

      -0.3f, -0.3f + delta_y,  0.3f,  0.0f, 0.0f,
      0.3f, -0.3f + delta_y,  0.3f,  1.0f, 0.0f,
      0.3f,  0.3f + delta_y,  0.3f,  1.0f, 1.0f,
      -0.3f,  0.3f + delta_y,  0.3f,  0.0f, 1.0f,

      0.3f, -0.3f + delta_y,  0.3f,  0.0f, 0.0f,
      0.3f, -0.3f + delta_y, -0.3f,  1.0f, 0.0f,
      0.3f,  0.3f + delta_y, -0.3f,  1.0f, 1.0f,
      0.3f,  0.3f + delta_y,  0.3f,  0.0f, 1.0f,

      0.3f,  0.3f + delta_y,  0.3f,  1.0f, 0.0f,
      0.3f,  0.3f + delta_y, -0.3f,  1.0f, 1.0f,
      -0.3f,  0.3f + delta_y, -0.3f,  0.0f, 1.0f,
      -0.3f,  0.3f + delta_y,  0.3f,  0.0f, 0.0f,

      0.3f, -0.3f + delta_y, -0.3f,  1.0f, 0.0f,
      0.3f,  0.3f + delta_y, -0.3f,  1.0f, 1.0f,
      -0.3f,  0.3f + delta_y, -0.3f,  0.0f, 1.0f,
      -0.3f, -0.3f + delta_y, -0.3f,  0.0f, 0.0f,

      -0.3f, -0.3f + delta_y,  0.3f,  1.0f, 0.0f,
      -0.3f, -0.3f + delta_y, -0.3f,  1.0f, 1.0f,
      -0.3f,  0.3f + delta_y, -0.3f,  0.0f, 1.0f,
      -0.3f,  0.3f + delta_y,  0.3f,  0.0f, 0.0f
  };

  GLuint indices[] = {
      0, 1, 3, 1, 2, 3,
      4, 5, 6, 4, 6, 7,
      8, 9, 10, 8, 10, 11,
      12, 13, 14, 12, 14, 15,
      16, 17, 18, 16, 18, 19,
      20, 21, 22, 20, 22, 23
  };

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)nullptr);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}

void DisplayWindow(GLFWwindow* window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glUseProgram(shaderProgram);

  auto model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians((float)anglex), glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians((float)angley), glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, glm::radians((float)anglez), glm::vec3(0.0f, 0.0f, 1.0f));

  auto view = glm::mat4(1.0f);
  view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

  auto projection = glm::mat4(1.0f);
  projection = glm::perspective(glm::radians(45.0f), (float)window_width / (
      float)window_height, 0.1f, 100.0f);

  GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
  GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
  GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  ViewCube();

  glfwSwapBuffers(window);
  glfwPollEvents();
}

void CallbackKeys(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    if (key == GLFW_KEY_RIGHT) {
      angley += 6;
    }
    if (key == GLFW_KEY_LEFT) {
      angley -= 6;
    }
    if (key == GLFW_KEY_UP) {
      anglex -= 6;
    }
    if (key == GLFW_KEY_DOWN) {
      anglex += 6;
    }
    if (key == GLFW_KEY_LEFT_CONTROL) {
      if (is_jumping) {
        is_jumping = false;
        delta_y = 0;
      } else {
        is_falling = !is_falling;
      }
      begin_ti = std::chrono::steady_clock::now();
      begin_time = std::chrono::steady_clock::now();
    }

    if (key == GLFW_KEY_LEFT_ALT) {
      anglez = 0;
      anglex = 0;
      angley = 0;
      delta_y = 0;
      is_falling = false;
      is_jumping = false;
      current_vel = 0;
    }

    if (key == GLFW_KEY_T) {
      if (tex) {
        glDisable(GL_TEXTURE_2D);
      } else {
        glEnable(GL_TEXTURE_2D);
      }
      tex = !tex;
    }

    if (key == GLFW_KEY_PAGE_UP) {
      anglez += 3;
    }
    if (key == GLFW_KEY_PAGE_DOWN) {
      anglez -= 3;
    }
  }
}

void setupShader() {
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
  glCompileShader(vertexShader);
  checkCompileErrors(vertexShader, "VERTEX");

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
  glCompileShader(fragmentShader);
  checkCompileErrors(fragmentShader, "FRAGMENT");

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkCompileErrors(shaderProgram, "PROGRAM");

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

int main() {
  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(window_width, window_height, "Lab 6",
                                        nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  if (glewInit() != GLEW_OK) {
    std::cout << "Failed to initialize GLEW" << std::endl;
    return -1;
  }

  setupShader();

  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_NORMALIZE);

  GLuint texture = LoadBMP("../texture.bmp");
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  glfwSetKeyCallback(window, CallbackKeys);

  while (!glfwWindowShouldClose(window)) {
    DisplayWindow(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
