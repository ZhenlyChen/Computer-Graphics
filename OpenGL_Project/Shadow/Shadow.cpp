#include "Shadow.h"
#include <functional>

#include "../lib/stb_image.h"

void ShadowApp::init(GLFWwindow* window) {
  // tell GLFW to capture our mouse
  this->camera = new Camera(window, this->defaultHeight, this->defaultWidth);
  this->window = window;
}

void ShadowApp::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    this->viewType = 0;
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    this->viewType = 1;
  camera->ProcessInput(window);
}

void ShadowApp::prepare() {
  glEnable(GL_DEPTH_TEST);

  this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  this->eyeValue[0] = 3.0f;
  this->eyeValue[1] = 3.0f;
  this->eyeValue[2] = 5.0f;

  this->lightPosValue[0] = -2.0f;
  this->lightPosValue[1] = 2.0f;
  this->lightPosValue[2] = -2.0f;

  // ���������
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
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // ������ɫ��
  objectShader = new Shader("Shadow/glsl/lightObject.vs.glsl", "Shadow/glsl/lightObject.fs.glsl");
  lightingShader = new Shader("Shadow/glsl/light.vs.glsl", "Shadow/glsl/light.fs.glsl");
  depthShader = new Shader("Shadow/glsl/depth.vs.glsl", "Shadow/glsl/depth.fs.glsl");

  // ��Ӱ����

  // ��Ӱ ����2D����
  glGenFramebuffers(1, &depthMapFBO);
  glGenTextures(1, &depthMap);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  // ����Ȼ���
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void ShadowApp::randerCube(const Shader& shader) {
  glm::mat4 model = glm::mat4(1.0f);
  shader.SetMat4("model", model);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void ShadowApp::randerPlane(const Shader& shader) {
  glm::mat4  model = glm::mat4(1.0f);
  // ����ƽ��
  model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
  // ���
  model = glm::scale(model, glm::vec3(10.0f, 0.1f, 10.0f));
  shader.SetMat4("model", model);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void ShadowApp::render() {
  this->camera->UpDateDeltaTime();
  this->camera->enableControl = (this->viewType == 1);

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 view;
  glm::mat4 projection;
  glm::mat4 model;
  glm::vec3 lightPos = glm::vec3(this->lightPosValue[0], this->lightPosValue[1], this->lightPosValue[2]);

  // �����Դλ��
  model = glm::mat4(1.0f);
  // ��ת��Դ
  if (rotateLight) {
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
  }
  model = glm::translate(model, lightPos);
  model = glm::scale(model, glm::vec3(0.2f));
  glm::vec4 lightLoc = model * glm::vec4(1.0f);
  lightingShader->Use();
  lightingShader->SetMat4("model", model);

  // ������Ӱ����
  if (!this->preProject) {
    projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
  } else {
    projection = glm::perspective((float)glm::radians(45.0f), 1.0f, near_plane, far_plane);

  }
  view = glm::lookAt(glm::vec3(lightLoc), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  depthShader->Use();
  depthShader->SetMat4("view", view);
  depthShader->SetMat4("projection", projection);
  glm::mat4 lightSpaceMatrix = projection * view;
  glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
  glClear(GL_DEPTH_BUFFER_BIT);
  glCullFace(GL_FRONT);
  // ������Ӱ
  this->randerCube(*depthShader);
  this->randerPlane(*depthShader);
  glCullFace(GL_BACK);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // �����ӿ�
  glViewport(0, 0, this->defaultWidth, this->defaultHeight);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  // ��Ⱦ����
  // ������ͼ����
  float zoom = 45.0f;
  if (this->viewType == 0) {
    view = glm::lookAt(glm::vec3(eyeValue[0], eyeValue[1], eyeValue[2]),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f));
  }
  else {
    view = camera->GetViewMatrix();
    zoom = camera->Zoom;
  }
  // ����ͶӰ����
  projection = glm::perspective((float)glm::radians(zoom), 1.0f, 0.1f, 100.0f);

  // ���ƹ�Դ
  lightingShader->Use();
  lightingShader->SetMat4("view", view);
  lightingShader->SetMat4("projection", projection);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  // ���������
  objectShader->Use();
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, depthMap);
  objectShader->SetMat4("lightSpaceMatrix", lightSpaceMatrix); // ���ձ任
  objectShader->SetInt("shadowMap", 0); // ��Ӱ���ͼ

  // ����������
  model = glm::mat4(1.0f);
  if (this->viewType == 0) {
    objectShader->SetVec3("viewPos", glm::vec3(eyeValue[0], eyeValue[1], eyeValue[2]));
  }
  else if (this->viewType == 1) {
    objectShader->SetVec3("viewPos", camera->Position);
  }
  objectShader->SetMat4("view", view);
  objectShader->SetMat4("projection", projection);
  objectShader->SetVec3("objectColor", 0.2f, 0.6f, 0.2f); // ������ɫ
  objectShader->SetVec3("lightColor", 1.0f, 1.0f, 1.0f); // ������ɫ
  objectShader->SetFloat("ambientStrength", this->ambientStrength); // ������ǿ��
  objectShader->SetFloat("specularStrength", this->specularStrength); // ����ǿ��
  objectShader->SetFloat("diffuseStrength", this->diffuseStrength); // ������ǿ��
  objectShader->SetInt("shininess", this->shininess); // �����
  objectShader->SetVec3("lightPos", lightLoc.x, lightLoc.y, lightLoc.z); // ����λ��
  this->randerCube(*objectShader);
  // ���Ƶذ�
  objectShader->SetVec3("objectColor", 0.4f, 0.7f, 0.3f); // ������ɫ
  this->randerPlane(*objectShader);




  // ������
  {
    ImGui::Begin("Menu");
    if (ImGui::CollapsingHeader("Base")) {
      ImGui::ColorEdit3("Background color", (float*)& clear_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::RadioButton("No Camera", &this->viewType, 0); ImGui::SameLine();
    ImGui::RadioButton("Enable Camera", &this->viewType, 1);
    ImGui::SliderFloat3("Eye ", this->eyeValue, -30.0f, 30.0f);

    ImGui::Checkbox("Rotate light", &this->rotateLight); ImGui::SameLine();
    ImGui::Checkbox("Perspective Procjet", &this->preProject);

    ImGui::SliderFloat("ambientStrength", &this->ambientStrength, 0.0f, 2.0f);
    ImGui::SliderFloat("diffuseStrength", &this->diffuseStrength, 0.0f, 2.0f);
    ImGui::SliderFloat("specularStrength", &this->specularStrength, 0.0f, 2.0f);
    ImGui::SliderInt("shininess", &this->shininess, 1, 256);

    ImGui::SliderFloat3("lightPostion", this->lightPosValue, -10.0f, 10.0f);
    ImGui::End();
  } 
}

void ShadowApp::clear() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
}
