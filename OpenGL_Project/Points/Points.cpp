#pragma once

#include "Points.h"
#include <math.h>

Points::Points() {
   this->title = "Points";
   // 初始化背景颜色
   bg_color = ImVec4(0.19f, 0.55f, 0.60f, 1.00f);
   // 初始化三角形顶点
   triangles_vertices[0] = 10;
   triangles_vertices[1] = 30;
   triangles_vertices[2] = -25;
   triangles_vertices[3] = 70;
   triangles_vertices[4] = 33;
   triangles_vertices[5] = 78;
   // 类型
   draw_type = 0;
   // 画布大小
   canvas_size = 100;
   // 圆
   circle_x = 0;
   circle_y = 0;
   circle_r = 10;
   // 栅格化
   rasterize = false;
   // 点大小
   point_size = 4.0;
}

void Points::input(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void Points::prepare() {
  // 初始化着色器
  point_shader = new Shader("Points/glsl/point.vs.glsl", "Points/glsl/point.fs.glsl");
}

void Points::render() {
  // 背景颜色
  glClearColor(bg_color.x, bg_color.y, bg_color.z, bg_color.w);
  glClear(GL_COLOR_BUFFER_BIT);
  // ImGui 窗口
  {
    // 工具栏
    ImGui::Begin("Draw");

    // 画布大小
    ImGui::SliderInt("Canvas Size", &canvas_size, 100, 400);

    // 点的大小
    ImGui::SliderFloat("Point Size", &point_size, 1.0, 10.0);

    // 自适应调节大小
    for (int i = 0; i < 6; i++) {
      if (triangles_vertices[i] > canvas_size) triangles_vertices[i] = canvas_size;
      if (triangles_vertices[i] < -canvas_size) triangles_vertices[i] = -canvas_size;
    }

    ImGui::RadioButton("Triangles", &draw_type, 0);
    // 使用着色器
    point_shader->Use();
    point_shader->SetColor("pointColor", shader_color);
   
    if (draw_type == 0) {
      ImGui::Checkbox("Rasterize", &rasterize);

      ImGui::Columns(3, "triangles");
      ImGui::Separator();

      ImGui::Text("Point 1: ");
      ImGui::SliderInt("X1", &triangles_vertices[0], -canvas_size, canvas_size);
      ImGui::SliderInt("Y1", &triangles_vertices[1], -canvas_size, canvas_size);
      ImGui::NextColumn();

      ImGui::Text("Point 2: ");
      ImGui::SliderInt("X2", &triangles_vertices[2], -canvas_size, canvas_size);
      ImGui::SliderInt("Y2", &triangles_vertices[3], -canvas_size, canvas_size);
      ImGui::NextColumn();

      ImGui::Text("Point 3: ");
      ImGui::SliderInt("X3", &triangles_vertices[4], -canvas_size, canvas_size);
      ImGui::SliderInt("Y3", &triangles_vertices[5], -canvas_size, canvas_size);
      ImGui::NextColumn();

      ImGui::Columns(1);
      ImGui::Separator();

      // 画边
      calcLine(triangles_vertices[0], triangles_vertices[1], triangles_vertices[2], triangles_vertices[3]);
      calcLine(triangles_vertices[0], triangles_vertices[1], triangles_vertices[4], triangles_vertices[5]);
      calcLine(triangles_vertices[4], triangles_vertices[5], triangles_vertices[2], triangles_vertices[3]);
      // 光栅化
      if (rasterize) {
        calcTriangle();
      }
      drawPoint();
    }

    ImGui::RadioButton("Circle", &draw_type, 1);

    if (draw_type == 1) {
      ImGui::PushItemWidth(120);
      ImGui::SliderInt("X", &circle_x, -canvas_size, canvas_size); ImGui::SameLine();
      ImGui::SliderInt("Y", &circle_y, -canvas_size, canvas_size); ImGui::SameLine();
      ImGui::SliderInt("R", &circle_r, 2, canvas_size);
      if (circle_r > canvas_size) circle_r = canvas_size;
      ImGui::PopItemWidth();
      // 画圆
      calcCircle(circle_x, circle_y, circle_r);
      drawPoint();
    }

    ImGui::ColorEdit3("Point color", (float*)& shader_color);
    ImGui::ColorEdit3("Background color", (float*)& bg_color);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
  }


}

void Points::clear() {
  // Null
}

inline void swap(int& a, int& b) {
  int t = a;
  a = b;
  b = t;
}

inline void Points::addPoint(int x, int y) {
  vertices[point_count++] = x;
  vertices[point_count++] = y;
}

void Points::drawPoint() {
  // 缩放为-1到1
  for (int j = 0; j < point_count; j++) {
    finalVertices[j] = vertices[j] * (1.0f / canvas_size);
  }
  // 顶点数组对象
  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  // 顶点缓冲对象
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 点的大小
  glPointSize(point_size);
  // 复制数据
  glBufferData(GL_ARRAY_BUFFER, sizeof(finalVertices), finalVertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
  glEnableVertexAttribArray(0);
  // 解除绑定
  //glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  //glBindVertexArray(VAO_line);
  glDrawArrays(GL_POINTS, 0, point_count / 2);
  glBindVertexArray(0);

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  point_count = 0;
}

void Points::calcLine(int start_x, int start_y, int end_x, int end_y) {
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
  int y = start_y;
  int dx = end_x - start_x;
  int dy2 = 2 * std::abs(end_y - start_y);
  int dy2dx2 = dy2 - 2 * dx;
  int p = -dx;
  for (int x = start_x; x <= end_x; x++) {
    if (p <= 0) {
      p += dy2;
    } else {
      y += step;
      p += dy2dx2;
    }
    reverse ? addPoint(y, x) : addPoint(x, y);
  }
}


void Points::calcCircle(int cx, int cy, int r) {
  int x = 0;
  int y = r;
  int p = 3 - 2 * r;
  addPoint(cx, cy + r);
  addPoint(cx, cy - r);
  addPoint(cx + r, cy);
  addPoint(cx - r, cy);
  for (x = 1; x <= y; x++) {
    if (p < 0) {
      p += 4 * x + 6;
    } else {
      p += 4 * x - 4 * y + 10;
      y--;
    }
    addPoint(cx + x, cy + y);
    addPoint(cx + y, cy + x);
    addPoint(cx - x, cy + y);
    addPoint(cx - y, cy + x);
    addPoint(cx + x, cy - y);
    addPoint(cx + y, cy - x);
    addPoint(cx - x, cy - y);
    addPoint(cx - y, cy - x);
  }
}

void Points::calcTriangle() {
  // 计算直线方程
  int parameter[9];
  parameter[0] = triangles_vertices[5] - triangles_vertices[3];
  parameter[1] = triangles_vertices[2] - triangles_vertices[4];
  parameter[2] = triangles_vertices[3] * triangles_vertices[4] - triangles_vertices[2] * triangles_vertices[5];

  parameter[3] = triangles_vertices[5] - triangles_vertices[1];
  parameter[4] = triangles_vertices[0] - triangles_vertices[4];
  parameter[5] = triangles_vertices[1] * triangles_vertices[4] - triangles_vertices[0] * triangles_vertices[5];

  parameter[6] = triangles_vertices[3] - triangles_vertices[1];
  parameter[7] = triangles_vertices[0] - triangles_vertices[2];
  parameter[8] = triangles_vertices[1] * triangles_vertices[2] - triangles_vertices[0] * triangles_vertices[3];

  // 中心化
  for (int i = 0; i < 3; i++) {
    if (parameter[i * 3] * triangles_vertices[i * 2] + parameter[i * 3 + 1] * triangles_vertices[i * 2 + 1] + parameter[i * 3 + 2] < 0) {
      for (int j = 0; j < 3; j++) {
        parameter[i * 3 + j] *= -1;
      }
    }
  }

  // 计算方块
  int max_x = triangles_vertices[0];
  int min_x = triangles_vertices[0];
  int max_y = triangles_vertices[1];
  int min_y = triangles_vertices[1];
  for (int i = 0; i < 3; i++) {
    if (triangles_vertices[i * 2] > max_x) max_x = triangles_vertices[i * 2];
    if (triangles_vertices[i * 2] < min_x) min_x = triangles_vertices[i * 2];
    if (triangles_vertices[i * 2 + 1] > max_y) max_y = triangles_vertices[i * 2 + 1];
    if (triangles_vertices[i * 2 + 1] < min_y) min_y = triangles_vertices[i * 2 + 1];
  }

  for (int x = min_x; x <= max_x; x++) {
    for (int y = min_y; y <= max_y; y++) {
      bool inside = true;
      for (int i = 0; i < 3; i++) {
        if (parameter[i * 3] * x + parameter[i * 3 + 1] * y + parameter[i * 3 + 2] < 0) {
          inside = false;
          break;
        }
      }
      if (inside) {
        addPoint(x, y);
      }
    }
  }
}