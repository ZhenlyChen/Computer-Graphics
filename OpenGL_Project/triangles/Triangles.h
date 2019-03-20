#pragma once

#include "../application.h"

class Triangles : public Application {
public:
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();

  unsigned int drawColorTriangles();
  unsigned int drawWoodTriangles();
  unsigned int drawATriangles();
  unsigned int drawLine();

  std::string title = "Triangles";
private:
  unsigned int VAO_color_triangles;
  unsigned int VAO_a_triangles;
  unsigned int VAO_line;
  unsigned int VAO_wood_triangles;

  ImVec4 triangles_color;
  ImVec4 clear_color;

  bool show_color_triangle;
  bool show_a_triangle;
  bool show_wood_triangle;

  Shader* trianglesShader;
  Shader* changeColorShader;
  Shader* woodShader;
};