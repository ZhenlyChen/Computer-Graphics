#pragma once

#include "../application.h"
#include "../lib/camera/Camera.h"

class CameraApp : public Application {
public:
  void init(GLFWwindow* window);
  void input(GLFWwindow* window);
  void render();
  void clear();
  void prepare();

  std::string title = "Camera";
private:
  GLFWwindow* window;
  ImVec4 clear_color;
  unsigned int VBO, VAO, EBO;

  unsigned int VBOCenter, VAOCenter, EBOCenter;
  Shader* cudeShader;

  int projectionType;

  float orthographicValue[6];
  float perspectiveValue[4];

  float eyeValue[3];

  // camera
  Camera* camera;
  int viewType = 0;
};