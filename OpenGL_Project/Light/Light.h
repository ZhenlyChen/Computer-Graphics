#pragma once

#include "../application.h"
#include "../lib/camera/Camera.h"

class LightApp : public Application {
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
  unsigned int lightVAO;

  Shader *phongShader;
  Shader *gouraudShader;
  Shader *lightingShader;

  float eyeValue[3];
  float lightValue[3];

  // Ðý×ª¹âÔ´
  bool rotateLight = false;

  // camera
  Camera* camera;
  int viewType = 0;
  int lightType = 0;

  float ambientStrength = 0.1f;
  float specularStrength = 0.5f;
  float diffuseStrength = 1.0f;
  int shininess = 32;
};