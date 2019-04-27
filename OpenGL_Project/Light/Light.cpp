#include "Light.h"
#include <functional>

#include "../lib/stb_image.h"

void LightApp::init(GLFWwindow* window) {
  // tell GLFW to capture our mouse
  this->camera = new Camera(window, this->defaultHeight, this->defaultWidth);
  this->window = window;
}

void LightApp::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  camera->ProcessInput(window);
}

void LightApp::prepare() {

  this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  this->eyeValue[0] = 3.0f;
  this->eyeValue[1] = 3.0f;
  this->eyeValue[2] = 5.0f;

  this->lightValue[0] = -2.0f;
  this->lightValue[1] = 2.0f;
  this->lightValue[2] = -2.0f;

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f
  };

  unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
  };

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // 创建光照VAO
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


  phongShader = new Shader("Light/glsl/phongCube.vs.glsl", "Light/glsl/phongCube.fs.glsl");
  gouraudShader = new Shader("Light/glsl/gouraudCube.vs.glsl", "Light/glsl/gouraudCube.fs.glsl");

  lightingShader = new Shader("Light/glsl/light.vs.glsl", "Light/glsl/light.fs.glsl");
}

void LightApp::render() {
  this->camera->UpDateDeltaTime();
  this->camera->enableControl = (this->viewType == 1);

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  glm::vec3 lightPos = glm::vec3(this->lightValue[0], this->lightValue[1], this->lightValue[2]);
  float zoom = 45.0f;

  Shader* cubeShader;
  if (this->lightType == 0) {
    cubeShader = this->phongShader;
  }
  else {
    cubeShader = this->gouraudShader;
  }

  cubeShader->Use();
  if (this->viewType == 0) {
    view = glm::lookAt(glm::vec3(eyeValue[0], eyeValue[1], eyeValue[2]),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
    cubeShader->SetVec3("viewPos", glm::vec3(eyeValue[0], eyeValue[1], eyeValue[2]));
  }
  else if (this->viewType == 1) {
    view = camera->GetViewMatrix();
    zoom = camera->Zoom;
    cubeShader->SetVec3("viewPos", camera->Position);
  }

  projection = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 100.0f);

  cubeShader->SetMat4("model", model);
  cubeShader->SetMat4("view", view);
  cubeShader->SetMat4("projection", projection);

  cubeShader->SetVec3("objectColor", 0.4f, 0.7f, 0.3f); // 物体颜色
  cubeShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f); // 光照颜色
  cubeShader->SetFloat("ambientStrength", this->ambientStrength); // 环境光强度
  cubeShader->SetFloat("specularStrength", this->specularStrength); // 镜面强度
  cubeShader->SetFloat("diffuseStrength", this->diffuseStrength); // 漫反射强度
  cubeShader->SetInt("shininess", this->shininess); // 反光度

  model = glm::mat4(1.0f);
  // 旋转光源
  if (rotateLight) {
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));

  glm::vec4 location = model * glm::vec4(1.0f);

  cubeShader->SetVec3("lightPos", location.x, location.y, location.z); // 光照位置

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  
  // 光照
  lightingShader->Use();
  lightingShader->SetMat4("model", model);
  lightingShader->SetMat4("view", view);
  lightingShader->SetMat4("projection", projection);

  glBindVertexArray(lightVAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  {
    // 工具栏
    ImGui::Begin("Menu");
    if (ImGui::CollapsingHeader("Base")) {
      ImGui::ColorEdit3("Background color", (float*)& clear_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::RadioButton("No Camera", &this->viewType, 0); ImGui::SameLine();
    ImGui::RadioButton("Enable Camera", &this->viewType, 1);
    ImGui::SliderFloat3("Eye ", this->eyeValue, -30.0f, 30.0f);

    ImGui::RadioButton("Phong Shading", &this->lightType, 0); ImGui::SameLine();
    ImGui::RadioButton("Gouraud Shading", &this->lightType, 1);

    ImGui::Checkbox("Rotate light", &this->rotateLight);

    ImGui::SliderFloat("ambientStrength", &this->ambientStrength, 0.0f, 2.0f);
    ImGui::SliderFloat("diffuseStrength", &this->diffuseStrength, 0.0f, 2.0f);
    ImGui::SliderFloat("specularStrength", &this->specularStrength, 0.0f, 2.0f);
    ImGui::SliderInt("shininess", &this->shininess, 1, 256);

    ImGui::SliderFloat3("lightPostion", this->lightValue, -10.0f, 10.0f);
    ImGui::End();
  }

}

void LightApp::clear() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteVertexArrays(1, &lightVAO);
  glDeleteBuffers(1, &VBO);
}
