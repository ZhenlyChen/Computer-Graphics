#pragma once

#include "Points.h"
#include <math.h>

void Points::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Points::prepare() {
  // 初始化背景颜色
  bg_color = ImVec4(0.19f, 0.55f, 0.60f, 1.00f);
  // 初始化着色器
  line_shader = new Shader("Points/glsl/line.vs.glsl", "Points/glsl/line.fs.glsl");

  triangles_vertices[0] = 10;
  triangles_vertices[1] = 30;
  triangles_vertices[2] = -25;
  triangles_vertices[3] = 70;
  triangles_vertices[4] = 33;
  triangles_vertices[5] = 78;

  drawType = 0;

}

void Points::render() {

  // 背景颜色
  glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  // ImGui 窗口
  {
    // 工具栏
    ImGui::Begin("Draw");
    ImGui::ColorEdit3("Background color", (float*)& bg_color);

    ImGui::RadioButton("Triangles", &drawType, 0);
    ImGui::Columns(3, "triangles");
    ImGui::Separator();

    ImGui::Text("Point 1: ");
    ImGui::SliderInt("X1", &triangles_vertices[0], -100, 100);
    ImGui::SliderInt("Y1", &triangles_vertices[1], -100, 100);
    ImGui::NextColumn();

    ImGui::Text("Point 2: ");
    ImGui::SliderInt("X2", &triangles_vertices[2], -100, 100);
    ImGui::SliderInt("Y2", &triangles_vertices[3], -100, 100);
    ImGui::NextColumn();

    ImGui::Text("Point 3: ");
    ImGui::SliderInt("X3", &triangles_vertices[4], -100, 100);
    ImGui::SliderInt("Y3", &triangles_vertices[5], -100, 100);
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::Separator();

    if (drawType == 0) {
      line_shader->Use();
      drawLine(triangles_vertices[0], triangles_vertices[1], triangles_vertices[2], triangles_vertices[3]);
      drawLine(triangles_vertices[0], triangles_vertices[1], triangles_vertices[4], triangles_vertices[5]);
      drawLine(triangles_vertices[4], triangles_vertices[5], triangles_vertices[2], triangles_vertices[3]);
    }
    ImGui::RadioButton("Circle", &drawType, 1);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }


}

void Points::clear() {
  glDeleteVertexArrays(1, &VAO_line);
}

void swap(int& a, int& b) {
  int t = a;
  a = b;
  b = t;
}

void Points::drawLine(int start_x, int start_y, int end_x, int end_y) {
  // 处理斜率大于1
  bool reverse = std::abs(end_y - start_y) > std::abs(end_x - start_x);
  if (reverse) {
    swap(start_x, start_y);
    swap(end_x, end_y);
  }
  // 处理反方向
  if (end_x < start_x) {
    swap(start_x, end_x);
    swap(start_y, end_y);
  }
  // 处理斜率为负数
  int step = start_y < end_y ? 1 : -1;
  
  vertices[0] = reverse ? start_y : start_x;
  vertices[1] = reverse ? start_x : start_y;

  int y = start_y;
  int dx = end_x - start_x;
  int dy2 = 2 * std::abs(end_y - start_y);
  int dy2dx2 = dy2 - 2 * dx;

  int p = dy2 - dx;

  int i = 2;
  for (int x = start_x + 1; x <= end_x; x++) {
    if (p <= 0) {
      p += dy2;
    } else {
      y += step;
      p += dy2dx2;
    }
    vertices[i] = reverse ? y : x;
    vertices[i + 1] = reverse ? x : y;
    i += 2;
  }

  // 缩放为-1到1
  for (int j = 0; j < i; j++) {
    finalVertices[j] = vertices[j] * 0.01;
  }
  // 顶点数组对象
  glGenVertexArrays(1, &VAO_line);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO_line);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 点的大小
  glPointSize(4.0);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(finalVertices), finalVertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  // 解除绑定
  //glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(VAO_line);
  glDrawArrays(GL_POINTS, 0, i / 2);
  glBindVertexArray(0);

  glDeleteVertexArrays(1, &VAO_line);
  glDeleteBuffers(1, &VBO);

}

unsigned int Points::drawCircle() {
  return 0;
}