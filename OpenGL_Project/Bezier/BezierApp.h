#pragma once
#include "../application.h"
#include "glm/glm.hpp"

class BezierApp : public Application {
public:
  BezierApp();
  void init(GLFWwindow* window);
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();
  void updateWindowSize(int h, int w);

private:
  void addPoint(glm::vec2 point);
  void drawPoint(int size = 10, bool withLine = false , bool clear = true);
  long long int getFactorial(int i);
  float bernstein(int i, int n, float t);

  int windowHeight;
  int windowWidth;

  glm::vec3 bgColor;
  glm::vec3 pointColor;
  glm::vec3 lineColor;

  glm::vec2 point[101];
  glm::vec2 newPoint[300];
  glm::vec2 mPoint[300];

  float vPoint[4002];
  int vCount = 0;

  float frameT;

  int pointCount;

  bool mouseLeft;
  bool mouseRight;

  Shader* shader;


  long long int factorialTmp[105];
  int computedFac = 1;
};