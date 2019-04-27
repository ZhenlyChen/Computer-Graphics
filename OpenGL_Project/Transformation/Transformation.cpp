#include "Transformation.h"

#include "../lib/stb_image.h"

void Transformation::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Transformation::prepare() {
  this->depth_test = true;
  this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  this->rotate = glm::vec3(0.0f, 1.0f, 0.0f);
  this->translation = glm::vec3(0.0f, 0.0f, 0.0f);
  this->scale = glm::vec3(0.5f, 0.5f, 0.5f);

  this->dir_t_x = 0;
  this->dir_t_y = 1;

  // 生成纹理
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;
  unsigned char* data = stbi_load("Transformation/assets/mmexport1476440125120.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    // 生成纹理
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Read textrues failed" << std::endl;
  }
  // 释放图片
  stbi_image_free(data);

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
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
  glBindTexture(GL_TEXTURE_2D, texture);

  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);



  cudeShader = new Shader("Transformation/glsl/cube.vs.glsl", "Transformation/glsl/cube.fs.glsl");
  cudeShader->Use();
  cudeShader->SetInt("woodTexture", 0);

}

void Transformation::render() {
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (depth_test) {
    glEnable(GL_DEPTH_TEST);
  }
  else {
    glDisable(GL_DEPTH_TEST);
  }

  // 坐标变换
  cudeShader->Use();

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  if (this->en_translation) { // 平移
    model = glm::translate(model, this->translation);
  }

  if (this->en_rotate) { // 旋转
    model = glm::rotate(model, (float)glfwGetTime(), this->rotate);
  }

  if (this->auto_translation) {
    if (dir_t_x == 0) {
      translation.x += 0.02f;
      if (translation.x > 0.95f) dir_t_x = 1;
    }
    else {
      translation.x -= 0.01f;
      if (translation.x < -0.95f) dir_t_x = 0;
    }
    if (dir_t_y == 0) {
      translation.y += 0.01f;
      if (translation.y > 0.95f) dir_t_y = 1;
    }
    else {
      translation.y -= 0.03f;
      if (translation.y < -0.95f) dir_t_y = 0;
    }
  }

  if (this->en_scale) { // 缩放
    model = glm::scale(model, this->scale);
  }


  if (this->auto_scale) {
    if (dir_s_x == 0) {
      scale.x += 0.02f;
      if (scale.x > 0.9f) dir_s_x = 1;
    }
    else {
      scale.x -= 0.01f;
      if (scale.x < 0.5f) dir_s_x = 0;
    }
    if (dir_s_y == 0) {
      scale.y += 0.01f;
      if (scale.y > 0.9f) dir_s_y = 1;
    }
    else {
      scale.y -= 0.03f;
      if (scale.y < 0.5f) dir_s_y = 0;
    }
    if (dir_s_z == 0) {
      scale.z += 0.02f;
      if (scale.z > 0.9f) dir_s_z = 1;
    }
    else {
      scale.z -= 0.03f;
      if (scale.z < 0.5f) dir_s_z = 0;
    }
  }



  // view = glm::translate(view, glm::vec3(0.0f, 0.0f, -6.0f));
  view = glm::lookAt(glm::vec3(0.0f, 2.0f, -6.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f));
  projection = glm::perspective(glm::radians(45.0f), (float)this->defaultWidth / (float)this->defaultHeight, 0.1f, 100.0f);

  cudeShader->SetMat4("model", model);
  cudeShader->SetMat4("view", view);
  cudeShader->SetMat4("projection", projection);

  glBindVertexArray(VAO);

  // glDrawArrays(GL_TRIANGLES, 0, 36);
  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


  if (en_other) {
    model = glm::mat4(1.0f);
    if (this->en_translation) { // 平移
      model = glm::translate(model, this->translation);
    }
    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    cudeShader->SetMat4("model", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
    model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
    cudeShader->SetMat4("model", model);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
  }





  // model = glm::translate(model, glm::vec3(0.5f, 0.5f, 0.5f));
  // cudeShader->SetMat4("model", model);
  // glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

  // 矩形
  /*for (unsigned int i = 0; i < 10; i++)
  {
    glm::mat4 model;
    model = glm::translate(model, cubePositions[i]);
    float angle = 20.0f * i;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    cudeShader->SetMat4("model", model);

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }*/

  // ImGui 窗口
  {
    // 工具栏
    ImGui::Begin("Menu");
    if (ImGui::CollapsingHeader("Base")) {
      ImGui::ColorEdit3("Background color", (float*)& clear_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::Checkbox("Enable depth test", &depth_test);
    ImGui::Checkbox("Translation", &en_translation);
    if (en_translation) {
      ImGui::SameLine();
      ImGui::Checkbox("Auto Translation", &auto_translation);
      ImGui::PushItemWidth(120);
      ImGui::SliderFloat("t x", &translation.x, -1.0f, 1.0f); ImGui::SameLine();
      ImGui::SliderFloat("t y", &translation.y, -1.0f, 1.0f); ImGui::SameLine();
      ImGui::SliderFloat("t z", &translation.z, -1.0f, 1.0f);
      ImGui::PopItemWidth();
    }
    ImGui::Checkbox("Rotation", &en_rotate);
    if (en_rotate) {
      ImGui::SameLine();
      ImGui::Checkbox("Auto Rotation", &auto_rotate);
      ImGui::PushItemWidth(120);
      ImGui::SliderFloat("r x", &rotate.x, -1.0f, 1.0f); ImGui::SameLine();
      ImGui::SliderFloat("r y", &rotate.y, -1.0f, 1.0f); ImGui::SameLine();
      ImGui::SliderFloat("r z", &rotate.z, -1.0f, 1.0f);
      ImGui::PopItemWidth();
    }
    ImGui::Checkbox("Scaling", &en_scale);
    if (en_scale) {
      ImGui::SameLine();
      ImGui::Checkbox("Auto_Scaling", &auto_scale);
      ImGui::PushItemWidth(120);
      ImGui::SliderFloat("s x", &scale.x, -3.0f, 3.0f); ImGui::SameLine();
      ImGui::SliderFloat("s y", &scale.y, -3.0f, 3.0f); ImGui::SameLine();
      ImGui::SliderFloat("s z", &scale.z, -3.0f, 3.0f);
      ImGui::PopItemWidth();
    }
    ImGui::Checkbox("More", &en_other);
    ImGui::End();
  }

  // OpenGL 渲染
  // 背景颜色

}

void Transformation::clear() {

}