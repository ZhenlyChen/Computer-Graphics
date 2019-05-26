#pragma once
#include "BezierApp.h"
#include <math.h>
#include "string.h"

double cursorX;
double cursorY;

bool buttonLeft;
bool buttonRight;

void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
  cursorX = xpos;
  cursorY = ypos;
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
  buttonLeft = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT;
  buttonRight = action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT;
}

BezierApp::BezierApp() {
  this->title = "Points";
  // 初始化背景颜色
  bgColor = glm::vec3(0.19f, 0.55f, 0.60f);
  pointColor = glm::vec3(0.4f, 0.35f, 0.80f);
  lineColor = glm::vec3(0.7f, 0.6f, 0.2f);
  this->windowHeight = this->defaultHeight;
  this->windowWidth = this->defaultWidth;

  this->factorialTmp[0] = 1;
}

void BezierApp::updateWindowSize(int h, int w) {
  this->windowHeight = h;
  this->windowWidth = w;
}

void BezierApp::init(GLFWwindow* window) {
  glfwSetMouseButtonCallback(window, mouseButtonCallback);
  glfwSetCursorPosCallback(window, cursorPosCallback);
}

void BezierApp::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void BezierApp::prepare() {
  // 初始化着色器
  shader = new Shader("GLSL/2D.vs.glsl", "GLSL/2D.fs.glsl");
}

void BezierApp::render() {
  // 背景颜色
  glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  // 左键按下
  if (this->mouseLeft == false && buttonLeft == true) {
    if (this->pointCount < 21) {
      this->point[pointCount] = glm::vec2(cursorX, cursorY);
      this->pointCount++;
    }
  }
  this->mouseLeft = buttonLeft;
  // 右键按下
  if (this->mouseRight == false && buttonRight == true) {
    if (this->pointCount > 0) this->pointCount--;
  }
  this->mouseRight = buttonRight;

  shader->Use();
  // 画点
  shader->SetVec3("drawColor", glm::vec3(0.4, 0.4, 0.4));
  for (int i = 0; i < this->pointCount; i++) {
    this->addPoint(this->point[i]);
  }
  this->drawPoint(5, true, false);
  shader->SetVec3("drawColor", pointColor);
  this->drawPoint();

  // 画线
  shader->SetVec3("drawColor", lineColor);
  if (this->pointCount > 1) {
    for (float t = 0; t < 1; t += 0.001) {
      glm::vec2 p = this->point[0] * this->bernstein(0, this->pointCount - 1, t);
      for (int i = 1; i < this->pointCount; i++) {
        p = p + this->point[i] * this->bernstein(i, this->pointCount - 1, t);
      }
      this->addPoint(p);
    }
    this->drawPoint(5);
  }

  // 辅助线
  if (this->pointCount > 1) {
    shader->SetVec3("drawColor", glm::vec3(1, 1, 1));
    frameT += 0.01;
    if (frameT > 1) frameT = 0;
    float t = this->frameT;
    int mCount = this->pointCount;
    for (int i = 0; i < this->pointCount; i++) {
      mPoint[i] = point[i];
    }
    while (mCount > 1) {
      int newCount = 0;
      for (int i = 0; i < mCount - 1; i++) {
        glm::vec2 p = this->mPoint[i] * (1 - t) + this->mPoint[i + 1] * t;
        newPoint[newCount] = p;
        newCount++;
        this->addPoint(p);
      }
      this->drawPoint(5, true);
      for (int i = 0; i < newCount; i++) {
        mPoint[i] = newPoint[i];
      }
      mCount = newCount;
    }
    shader->SetVec3("drawColor", pointColor);
    glm::vec2 p = this->point[0] * this->bernstein(0, this->pointCount - 1, t);
    for (int i = 1; i < this->pointCount; i++) {
      p = p + this->point[i] * this->bernstein(i, this->pointCount - 1, t);
    }
    this->addPoint(p);
    this->drawPoint();
  }

  //// ImGui 窗口
  //{
  //  // 工具栏
  //  ImGui::Begin("Draw");
  //  ImGui::Text((std::to_string(cursorX) + "," + std::to_string(cursorY)).c_str());
  //  ImGui::Text(std::to_string(this->pointCount).c_str());
  //  ImGui::ColorEdit3("Point color", (float*)& pointColor);
  //  ImGui::ColorEdit3("Line color", (float*)& lineColor);
  //  ImGui::ColorEdit3("Background color", (float*)& bgColor);
  //  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  //  ImGui::End();
  //}
}
// 计算阶乘
long long int BezierApp::getFactorial(int i) {
  if (i > 104) return 1;
  for (; computedFac <= i; computedFac++) {
    factorialTmp[computedFac] = factorialTmp[computedFac - 1] * computedFac;
  }
  return factorialTmp[i];
}
// 计算 bernstein 函数
float BezierApp::bernstein(int i, int n, float t) {
  long long int a = getFactorial(i) * getFactorial(n - i);
  float res = getFactorial(n) / a;
  res *= pow(t, i) * pow(1 - t, n - i);
  return res;
}

void BezierApp::clear() {
  // Null
}

// 添加点
void BezierApp::addPoint(glm::vec2 point) {
  this->vPoint[this->vCount * 2] = (point.x / this->windowWidth) * 2 - 1;
  this->vPoint[this->vCount * 2 + 1] = -((point.y / this->windowHeight) * 2 - 1);
  this->vCount++;
}

// 画点
void BezierApp::drawPoint(int size, bool withLine, bool clear) {
  if (this->vCount < 1) return;
  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 点的大小
  glPointSize(size);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->vCount * 2, this->vPoint, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  // 解除绑定
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDrawArrays(GL_POINTS, 0, this->vCount);
  if (withLine) glDrawArrays(GL_LINE_STRIP, 0, this->vCount);
  glBindVertexArray(0);
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  if (clear) this->vCount = 0;
}
