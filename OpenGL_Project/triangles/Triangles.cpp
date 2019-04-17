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

  // ImGui 窗口
  {
    // 工具栏
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

  // OpenGL 渲染
  // 背景颜色
  glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
  glClear(GL_COLOR_BUFFER_BIT);

  // 画三个彩色三角形
  if (show_color_triangle) {
    trianglesShader->Use(); // 使用着色器
    glBindVertexArray(VAO_color_triangles);  // 使用预先设定的VAO
    // glDrawArrays(GL_TRIANGLES, 0, 3); // 画三角形
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // 解除绑定
  }

  // 画一个奇形怪状三角形
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

  // 画一个木头纹理三角形
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

  // 画三个彩色三角形
  this->VAO_color_triangles = drawColorTriangles();
  // 着色器
  this->trianglesShader = new Shader("triangles/glsl/tri.vs.glsl", "triangles/glsl/tri.fs.glsl");

  // 画一个倒三角
  this->VAO_a_triangles = drawATriangles();
  this->VAO_line = drawLine();
  this->changeColorShader = new Shader("triangles/glsl/color_tri.vs.glsl", "triangles/glsl/color_tri.fs.glsl");

  // 画一个木头三角
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
  // 顶点
  float vertices[] = {
    // 位置               // 颜色
    -0.8f, 0.2f, 0.0f,  0.0f, 1.0f, 0.0f, // 左下
    -0.2f, 0.2f, 0.0f,  0.0f, 0.0f, 1.0f, // 右下
    -0.5f, 0.8f, 0.0f,  1.0f, 0.0f, 0.0f,  // 上

    0.0f, -0.2f, 0.0f,  1.0f, 0.0f, 0.0f,

    0.2f, 0.2f, 0.0f,   0.0f, 1.0f, 0.0f, // 左下
    0.8f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f, // 右下
    0.5f, 0.8f, 0.0f,   1.0f, 0.0f, 0.0f,  // 上
  };

  unsigned int indices[] = {
    0, 1, 2,
    1, 3, 4,
    4, 5, 6
  };

  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 索引缓冲对象
  unsigned int EBO;
  glGenBuffers(1, &EBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  // 解析顶点数据
  // 数据的索引：0, 一组数据数量：3, 数据类型：float, 格式化：false, 一组数据的长度：6, 开始偏移量：0
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 解析颜色数据
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawWoodTriangles() {
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
  // load image, create texture and generate mipmaps
  // 读取纹理图片
  int width, height, nrChannels;
  unsigned char* data = stbi_load("triangles/assets/wood.jpg", &width, &height, &nrChannels, 0);
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
    // 位置              // 纹理
    0.8f, -0.2f, 0.0f,  1.0f, 1.0f, // 右上
    0.2f, -0.2f, 0.0f,  0.0f, 1.0f, // 左上
    0.5f, -0.6f, 0.0f,  0.5f, 0.0f  // 下
  };
  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindTexture(GL_TEXTURE_2D, texture);
  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 解析纹理数据
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawATriangles() {
  // 顶点
  float vertices[] = {
    // 位置              
    -0.8f, -0.2f, 0.0f, // 左下
    -0.2f, -0.2f, 0.0f, // 右下
    -0.5f, -0.6f, 0.0f, // 上
  };
  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}

unsigned int Triangles::drawLine() {
  // 顶点
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
  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return VAO;
}
