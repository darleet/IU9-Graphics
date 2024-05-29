#include <iostream>
#include <chrono>
#include "GL/gl.h"
#include "GLFW/glfw3.h"
#include "cmath"

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

void ViewCube(){
  auto end = std::chrono::steady_clock::now();

  auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_ti);
  auto full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);

  if (is_falling and delta_y < -0.7) {
      is_jumping = true;
      is_falling = false;
    begin_time = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
    full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);
  }

  if (is_jumping and delta_y > 0) {
      is_falling = true;
      is_jumping = false;
    begin_time = std::chrono::steady_clock::now();
    end = std::chrono::steady_clock::now();
    full_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin_time);
  }

  if (is_falling and elapsed_ms > std::chrono::milliseconds(1)) {
    begin_ti = end;
    delta_y = -9.8 * pow((full_ms.count() / 1000.0), 2) / 2;
    current_vel = 9.8 * (full_ms.count() / 1000.0);
  } else if (is_jumping and elapsed_ms > std::chrono::milliseconds(10)) {
    begin_ti = end;
    delta_y = current_vel * (full_ms.count() / 1000.0) -0.7 - 9.8 * pow((full_ms.count() / 1000.0), 2) / 2;
  }


  glBegin(GL_POLYGON);
  // Нижняя грань
  glColor3f(1, 0, 0);
  glNormal3f(0, -1, 0);

  glTexCoord2f( 1, 0 );
  glVertex3f(0.3, -0.3 + delta_y, 0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(0.3, -0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 1 );
  glVertex3f(-0.3, -0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 0 );
  glVertex3f(-0.3, -0.3 + delta_y, 0.3);
  glEnd();

  // перед
  glBegin(GL_POLYGON);
  glColor3f(0, 1, 0);
  glNormal3f(0, 0, -1);
  glTexCoord2f( 0, 0 );
  glVertex3f(-0.3, -0.3 + delta_y, 0.3);
  glTexCoord2f( 1, 0 );
  glVertex3f(0.3, -0.3 + delta_y, 0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(0.3, 0.3 + delta_y, 0.3);
  glTexCoord2f(0, 1);
  glVertex3f(-0.3, 0.3 + delta_y, 0.3);
  glEnd();

  // Правая грань
  glBegin(GL_POLYGON);
  glColor3f(0, 0, 1);
  glNormal3f(-1, 0, 0);

  glTexCoord2f( 0, 0 );
  glVertex3f(0.3, -0.3 + delta_y, 0.3);
  glTexCoord2f( 1, 0 );
  glVertex3f(0.3, -0.3 + delta_y, -0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 1 );
  glVertex3f(0.3, 0.3 + delta_y, 0.3);

  glEnd();

  // Верхняя грань
  glBegin(GL_POLYGON);
  glColor3f(0, 1, 1);
  glNormal3f(0, -1, 0);

  glTexCoord2f( 0, 1 );
  glVertex3f(0.3, 0.3 + delta_y, 0.3);
  glTexCoord2f( 0, 0 );
  glVertex3f(0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 1, 0 );
  glVertex3f(-0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(-0.3, 0.3 + delta_y, 0.3);
  glEnd();

  // Задняя грань
  glBegin(GL_POLYGON);
  glColor3f(1, 0, 1);
  glNormal3f(0, 0, -1);

  glTexCoord2f( 1, 0 );
  glVertex3f(0.3, -0.3 + delta_y, -0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 1 );
  glVertex3f(-0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 0 );
  glVertex3f(-0.3, -0.3 + delta_y, -0.3);
  glEnd();

  // Левая грань
  glBegin(GL_POLYGON);
  glColor3f(1, 1, 0.4);
  glNormal3f(1, 0, 0);

  glTexCoord2f( 1, 0 );
  glVertex3f(-0.3, -0.3 + delta_y, 0.3);
  glTexCoord2f( 1, 1 );
  glVertex3f(-0.3, 0.3 + delta_y, 0.3);
  glTexCoord2f( 0, 1 );
  glVertex3f(-0.3, 0.3 + delta_y, -0.3);
  glTexCoord2f( 0, 0 );
  glVertex3f(-0.3, -0.3 + delta_y, -0.3);
  glEnd();
}

void EnableLight() {
  GLfloat light0_diffuse[] = {1.0, 1.0, 1.0};
  GLfloat light0_direction[] = {-0.5, 0, -1, 1.0};
  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_direction);
}


GLuint LoadBMP(const char* image) {
  unsigned char header[54];
  unsigned int dataPos;
  unsigned int width, height;
  unsigned int imageSize;
  unsigned char * data;

  FILE * file = fopen(image, "rb");
  if (!file) {
    std::cout << "Файл не найден" << std::endl;
    return 0;
  }

  if ( fread(header, 1, 54, file) != 54 ) {
    std::cout << "Некорректный BMP-файл" << std::endl;
    return 0;
  }

  if ( header[0]!='B' || header[1]!='M' ){
    std::cout << "Некорректный BMP-файл" << std::endl;
    return 0;
  }

  imageSize  = *(int*)&(header[0x22]); // Размер изображения в байтах
  width      = *(int*)&(header[0x12]); // Ширина
  height     = *(int*)&(header[0x16]); // Высота

  if (imageSize==0)    imageSize=width*height*3;

  data = new unsigned char [imageSize];

  fread(data,1,imageSize,file);

  fclose(file);

  GLuint textureID;
  glGenTextures(1, &textureID);

  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,
               GL_BGR, GL_UNSIGNED_BYTE, data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  return textureID;
}


void DisplayWindow(GLFWwindow *window) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glEnable(GL_DEPTH_TEST);

  GLfloat material_diffuse[] = {0.4, 0.6, 0.3, 1.0};
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_diffuse);

  glClearColor(0.6, 0.2, 0.6, 0.0);


  GLfloat rotateCubeX[] = {
      1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, static_cast<GLfloat>(cos(anglex * M_PI / 180)), -static_cast<GLfloat>(sin(anglex * M_PI / 180)), 0.0f,
      0.0f, static_cast<GLfloat>(sin(anglex * M_PI / 180)), static_cast<GLfloat>(cos(anglex * M_PI / 180)), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  GLfloat rotateCubeY[] = {
      static_cast<GLfloat>(cos(angley * M_PI / 180)), 0.0f, static_cast<GLfloat>(sin(angley * M_PI / 180)), 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      -static_cast<GLfloat>(sin(angley * M_PI / 180)), 0.0f, static_cast<GLfloat>(cos(angley * M_PI / 180)), 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  GLfloat rotateCubeZ[] = {
      static_cast<GLfloat>(cos(anglez * M_PI / 180)), -static_cast<GLfloat>(sin(anglez * M_PI / 180)), 0.0f, 0.0f,
      static_cast<GLfloat>(sin(anglez * M_PI / 180)), static_cast<GLfloat>(cos(anglez * M_PI / 180)), 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f
  };

  glPushMatrix();
  glMatrixMode(GL_MODELVIEW);

  glMultMatrixf(rotateCubeX);
  glMultMatrixf(rotateCubeY);
  glMultMatrixf(rotateCubeZ);

  ViewCube();

  glPopMatrix();
  EnableLight();
  glFlush();
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

int main() {
  if (!glfwInit()) {
    return 1;
  }

  GLFWwindow *window = glfwCreateWindow(window_width, window_height, "Lab 6",
                            nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return 1;
  }

  glfwMakeContextCurrent(window);

  glEnable(GL_LIGHTING);
  glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
  glEnable(GL_NORMALIZE);

  GLuint texture = LoadBMP("../texture.bmp");
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

  glfwSetKeyCallback(window, CallbackKeys);

  while (!glfwWindowShouldClose(window)) {
    DisplayWindow(window);
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}