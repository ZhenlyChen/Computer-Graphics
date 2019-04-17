#include "CameraApp.h"
#include <functional>

#include "../lib/stb_image.h"

void CameraApp::init(GLFWwindow* window) {
  // tell GLFW to capture our mouse
  this->camera = new Camera(window, this->defaultHeight, this->defaultWidth);
  this->window = window;
}

void CameraApp::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  camera->ProcessInput(window);
}

void CameraApp::prepare() {

  this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  this->projectionType = 0;

  this->orthographicValue[0] = -5.0f;
  this->orthographicValue[1] = 5.0f;
  this->orthographicValue[2] = -5.0f;
  this->orthographicValue[3] = 5.0f;
  this->orthographicValue[4] = -30.0f;
  this->orthographicValue[5] = 30.0f;

  this->perspectiveValue[0] = 45.0f;
  this->perspectiveValue[1] = (float)this->defaultWidth / (float)this->defaultHeight;
  this->perspectiveValue[2] = 0.1f;
  this->perspectiveValue[3] = 100.0f;

  this->eyeValue[0] = 0.0f;
  this->eyeValue[1] = 5.0f;
  this->eyeValue[2] = -20.0f;

  auto centerPos = glm::vec3(-1.5f, 0.5f, -1.5f);
  float vertices[] = {
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,

    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,

    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.6f, 0.3f, 0.8f,

    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.8f, 0.1f, 0.7f,

    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.5f, 0.8f, 0.2f,

    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.1f, 0.4f, 0.2f,
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
  // texture coord attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  centerPos = glm::vec3(0.0f, 0.0f, 0.0f);
  float verticesCenter[] = {
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.2f, 0.4f, 0.6f,

    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.4f, 0.6f, 0.7f,

    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.6f, 0.3f, 0.8f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.6f, 0.3f, 0.8f,

    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.8f, 0.1f, 0.7f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.8f, 0.1f, 0.7f,

    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z - 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x + 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.5f, 0.8f, 0.2f,
    centerPos.x - 2.0f, centerPos.y - 2.0f, centerPos.z + 2.0f,  0.5f, 0.8f, 0.2f,

    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z - 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x + 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.1f, 0.4f, 0.2f,
    centerPos.x - 2.0f, centerPos.y + 2.0f, centerPos.z + 2.0f,  0.1f, 0.4f, 0.2f,
  };

  glGenVertexArrays(1, &VAOCenter);
  glGenBuffers(1, &VBOCenter);
  glGenBuffers(1, &EBOCenter);

  glBindVertexArray(VAOCenter);
  glBindBuffer(GL_ARRAY_BUFFER, VBOCenter);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCenter);

  glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCenter), verticesCenter, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



  cudeShader = new Shader("Camera/glsl/cube.vs.glsl", "Camera/glsl/cube.fs.glsl");
  cudeShader->Use();

}

void CameraApp::render() {
  this->camera->UpDateDeltaTime();
  this->camera->enableControl = (this->viewType == 1);

  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);

  // 坐标变换
  cudeShader->Use();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  // 基础旋转
  // model = glm::rotate(model, 1.0f, glm::vec3(1.0f, 0.5f, 0.4f));


  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));

  if (this->viewType == 0) {
    view = glm::lookAt(glm::vec3(eyeValue[0], eyeValue[1], eyeValue[2]),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
  } else if (this->viewType == 1) {
    view = camera->GetViewMatrix(); 
    this->perspectiveValue[0] = camera->Zoom;
  } else if (this->viewType == 2) {
    view = camera->GetCenterViewMatrix();
    this->perspectiveValue[0] = camera->Zoom;
    float clock = glfwGetTime();
    float camPosX = sin(clock / 1.0) * 15.0f;
    float camPosZ = cos(clock / 1.0) * 15.0f;
    this->camera->Position = glm::vec3(camPosX, 5.0f, camPosZ);
    this->camera->Front = glm::vec3(0.0f, 0.0f, 0.0f);
    this->camera->Yaw = -90.0f;
    this->camera->Pitch = 0.0f;
    this->camera->Up = glm::vec3(0.0f, 1.0f, 0.0f);
  }

  if (this->projectionType == 0) {
    projection = glm::perspective(
      glm::radians(this->perspectiveValue[0]), this->perspectiveValue[1],
      this->perspectiveValue[2], this->perspectiveValue[3]);
  }
  else {
    projection = glm::ortho(
      this->orthographicValue[0], this->orthographicValue[1], 
      this->orthographicValue[2], this->orthographicValue[3],
      this->orthographicValue[4], this->orthographicValue[5]);
  }

  cudeShader->SetMat4("model", model);
  cudeShader->SetMat4("view", view);
  cudeShader->SetMat4("projection", projection);

  if (this->viewType == 0) {
    glBindVertexArray(VAO);
  }
  else {
    glBindVertexArray(VAOCenter);
  }
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  {
    // 工具栏
    ImGui::Begin("Menu");
    if (ImGui::CollapsingHeader("Base")) {
      ImGui::ColorEdit3("Background color", (float*)& clear_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::RadioButton("No Camera", &this->viewType, 0); ImGui::SameLine();
    ImGui::RadioButton("Enable Camera", &this->viewType, 1); ImGui::SameLine();
    ImGui::RadioButton("Rotate", &this->viewType, 2);
    ImGui::RadioButton("Perspective", &this->projectionType, 0);
    if (this->projectionType == 0) {
      ImGui::SliderFloat("Perspective Fovy", &this->perspectiveValue[0], -360.0f, 360.0f);
      ImGui::SliderFloat("Perspective Aspect", &this->perspectiveValue[1], -10.0f, 10.0f);
      ImGui::SliderFloat("Perspective Near", &this->perspectiveValue[2], -100.0f, 100.0f);
      ImGui::SliderFloat("Perspective Far", &this->perspectiveValue[3], -100.0f, 100.0f);
    }
    ImGui::RadioButton("Orthographic", &this->projectionType, 1);
    if (this->projectionType == 1) {
      ImGui::SliderFloat4("Orthographic Value", this->orthographicValue, -30.0f, 30.0f);
      ImGui::SliderFloat("Orthographic Near", &this->orthographicValue[4], -30.0f, 30.0f);
      ImGui::SliderFloat("Orthographic Far", &this->orthographicValue[5], -30.0f, 30.0f);
    }
    ImGui::SliderFloat3("Eye ", this->eyeValue, -30.0f, 30.0f);
    ImGui::End();
  }

}

void CameraApp::clear() {
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
}
