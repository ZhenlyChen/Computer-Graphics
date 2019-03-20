#pragma once

#include "../application.h"

class Points : public Application {
public:
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();

  void drawLine(int start_x, int start_y, int end_x, int end_y);
  unsigned int drawCircle();

  std::string title = "Points";

private:
  int drawType;
  int vertices[401];
  float finalVertices[401];

  unsigned int VAO_line;
  int triangles_vertices[6];

  ImVec4 bg_color;

  Shader* line_shader;
};