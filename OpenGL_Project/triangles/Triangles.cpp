#include "triangles.h"

#include "../lib/imgui/imgui.h"
#include "../lib/imgui/imgui_impl_glfw.h"
#include "../lib/imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../lib/shader/shader.h"


#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

void Triangles::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Triangles::render() {

  // ImGui ����
  {
    // ������
    ImGui::Begin("Change color");
    ImGui::ColorEdit3("Background color", (float*)& clear_color);
    ImGui::Checkbox("Show color triangle", (bool*)& show_color_triangle);
    ImGui::Checkbox("Show a triangle", (bool*)& show_a_triangle);
    if (show_a_triangle) {
      ImGui::ColorEdit3("Triangles color", (float*)& triangles_color);
    }
    ImGui::Checkbox("Show wood triangle", (bool*)& show_wood_triangle);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }

  // OpenGL ��Ⱦ
  // ������ɫ
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  // ��������ɫ������
  if (show_color_triangle) {
    trianglesShader->Use(); // ʹ����ɫ��
    glBindVertexArray(VAO_color_triangles);  // ʹ��Ԥ���趨��VAO
    // glDrawArrays(GL_TRIANGLES, 0, 3); // ��������
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // �����
  }

  // ��һ�����ι�״������
  if (show_a_triangle) {
    changeColorShader->Use();
    changeColorShader->SetColor("triColor", triangles_color);
    glBindVertexArray(VAO_a_triangles);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);

    glBindVertexArray(VAO_line);
    glDrawArrays(GL_LINE_STRIP, 0, 8);
    glBindVertexArray(0);
  }

  // ��һ��ľͷ����������
  if (show_wood_triangle) {
    woodShader->Use();
    glBindVertexArray(VAO_wood_triangles);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
  }
}

void Triangles::clear() {
  glDeleteVertexArrays(1, &VAO_color_triangles);
  glDeleteVertexArrays(1, &VAO_a_triangles);
  glDeleteVertexArrays(1, &VAO_line);
  glDeleteVertexArrays(1, &VAO_wood_triangles);
}

void Triangles::prepare() {

  // ��������ɫ������
  this->VAO_color_triangles = drawColorTriangles();
  // ��ɫ��
  this->trianglesShader = new Shader("triangles/glsl/tri.vs.glsl", "triangles/glsl/tri.fs.glsl");

  // ��һ��������
  this->VAO_a_triangles = drawATriangles();
  this->VAO_line = drawLine();
  this->changeColorShader = new Shader("triangles/glsl/color_tri.vs.glsl", "triangles/glsl/color_tri.fs.glsl");

  // ��һ��ľͷ����
  this->VAO_wood_triangles = drawWoodTriangles();
  this->woodShader = new Shader("triangles/glsl/textures.vs.glsl", "triangles/glsl/textures.fs.glsl");
  this->woodShader->Use();
  this->woodShader->SetInt("ourTexture", 0);


  this->triangles_color = ImVec4(0.0, 1.0, 0.0, 1.0);
  this->clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  this->show_color_triangle = true;
  this->show_a_triangle = true;
  this->show_wood_triangle = true;
}


unsigned int Triangles::drawColorTriangles() {
  // ����
  float vertices[] = {
    // λ��               // ��ɫ
    -0.8f, 0.2f, 0.0f,  0.0f, 1.0f, 0.0f, // ����
    -0.2f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f, // ����
    -0.5f, 0.8f, 0.0f,  1.0f, 0.0f, 0.0f,  // ��

    0.0f, -0.2f, 0.0f,  1.0f, 0.0f, 0.0f,

    0.2f, 0.2f, 0.0f,   0.0f, 1.0f, 0.0f, // ����
    0.8f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f, // ����
    0.5f, 0.8f, 0.0f,   1.0f, 0.0f, 0.0f,  // ��
  };

  unsigned int indices[] = {
    0, 1, 2,
    1, 3, 4,
    4, 5, 6
  };

  // �����������
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // ���㻺�����
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // �����������
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  // �󶨻���
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // ��������
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // ������������
  // ���ݵ�������0, һ������������3, �������ͣ�float, ��ʽ����false, һ�����ݵĳ��ȣ�6, ��ʼƫ������0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // ������ɫ����
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawWoodTriangles() {
  // ��������
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  // set the texture wrapping parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  // ��ȡ����ͼƬ
  int width, height, nrChannels;
  unsigned char* data = stbi_load("triangles/assets/wood.jpg", &width, &height, &nrChannels, 0);
  if (data) {
    // ��������
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cout << "Read textrues failed" << std::endl;
  }
  // �ͷ�ͼƬ
  stbi_image_free(data);


  float vertices[] = {
    // λ��              // ����
    0.8f, -0.2f, 0.0f,  1.0f, 1.0f, // ����
    0.2f, -0.2f, 0.0f,  0.0f, 1.0f, // ����
    0.5f, -0.6f, 0.0f,  0.5f, 0.0f  // ��
  };
  // �����������
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // ���㻺�����
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // �󶨻���
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ��������
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // ������������
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // ������������
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawATriangles() {
  // ����
  float vertices[] = {
    // λ��              
    -0.8f, -0.2f, 0.0f, // ����
    -0.2f, -0.2f, 0.0f, // ����
    -0.5f, -0.6f, 0.0f, // ��
  };
  // �����������
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // ���㻺�����
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // �󶨻���
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ��������
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // ������������
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawLine() {
  // ����
  float vertices[] = {
    -0.5f, -0.6f,
    -0.5f, -0.8f,
    -0.4f, -0.6f,
    -0.4f, -0.8f,
    -0.5f, -0.9f,
    -0.6f, -0.8f,
    -0.6f, -0.6f,
    -0.5f, -0.8f,
  };
  // �����������
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // ���㻺�����
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // �󶨻���
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ��������
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // ������������
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}
