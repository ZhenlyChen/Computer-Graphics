#include <iostream>
using std::string;


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);
GLFWwindow* initWindow(const char* title);

// ���ڴ�С
const int window_height = 800;
const int window_width = 800;

unsigned int drawColorTriangles() {
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


unsigned int drawATriangles() {
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

unsigned int drawLine() {
  // ����
  float vertices[] = {
    -0.5f, -0.6f, 0.0f, // ��
    -0.5f, -0.8f, 0.0f
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

int main() {
  // ��ʼ��
  GLFWwindow* window = initWindow("Triangles");
  if (window == NULL) return -1;

  // ��������ɫ������
  unsigned int VAO_color_triangles = drawColorTriangles();
  // ��ɫ��
  Shader trianglesShader("tri.vs.glsl", "tri.fs.glsl");

  // ��һ��������
  unsigned int VAO_a_triangles = drawATriangles();
  unsigned int VAO_line = drawLine();
  Shader changeColorShader("color_tri.vs.glsl", "color_tri.fs.glsl");

  ImVec4 triangles_color = ImVec4(0.0, 1.0, 0.0, 1.0);
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  bool show_color_triangle = true;
  bool show_a_triangle = true;

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();


    // ImGui��Ⱦ
    {
      // ������
      ImGui::Begin("Change color");
      ImGui::Checkbox("Show color triangle", (bool*)& show_color_triangle);
      ImGui::Checkbox("Show a triangle", (bool*)& show_a_triangle);
      if (show_a_triangle) {
        ImGui::ColorEdit3("Triangles color", (float*)& triangles_color);
      }
      ImGui::ColorEdit3("Background color", (float*)& clear_color);
      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }
    // Rendering
    ImGui::Render();
    glfwMakeContextCurrent(window);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    // OpenGL��Ⱦ
    // ������ɫ
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

    if (show_color_triangle) {
      // ��������ɫ������
      trianglesShader.Use(); // ʹ����ɫ��
      glBindVertexArray(VAO_color_triangles);  // ʹ��Ԥ���趨��VAO
      // glDrawArrays(GL_TRIANGLES, 0, 3); // ��������
      glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0); // �����
    }

    if (show_a_triangle) {
      // ��һ��������
      changeColorShader.Use();
      changeColorShader.SetColor("triColor", triangles_color);
      glBindVertexArray(VAO_a_triangles);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);

      glBindVertexArray(VAO_line);
      glDrawArrays(GL_LINE, 0, 2);
      glBindVertexArray(0);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO_color_triangles);
  glDeleteVertexArrays(1, &VAO_a_triangles);
  // glDeleteBuffers(1, &VBO);
  // glDeleteBuffers(1, &EBO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow* window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, w, h);
}

GLFWwindow* initWindow(const char* title) {
  // ��ʼ��GLFW
  glfwInit();
  // OpenGL 4.5
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


  // ��ʼ������
  GLFWwindow* window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window); 
  glfwSwapInterval(1); // Enable vsync
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ��ʼ��GLAD
  if (!gladLoadGL()) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  // ��ʼ���ӿ�
  glViewport(0, 0, window_width, window_height);

  // ��ʼ��ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450");
  return window;
}