#pragma once

#include "../application.h"

class Transformation : public Application {
public:
  void init(GLFWwindow* window) {

  }
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();

  std::string title = "Transformation";
private:
  ImVec4 clear_color;

  bool en_translation;
  bool en_scale;
  bool en_rotate;
  bool en_other;

  bool auto_translation;
  bool auto_scale;
  bool auto_rotate;

  bool depth_test;

  unsigned int VBO, VAO, EBO;
  Shader* cudeShader;

  glm::vec3 rotate;
  glm::vec3 translation;
  int dir_t_x = 0;
  int dir_t_y = 0;
  int dir_t_z = 0;
  glm::vec3 scale;
  int dir_s_x = 0;
  int dir_s_y = 0;
  int dir_s_z = 0;

  glm::vec3* cubePositions;
};