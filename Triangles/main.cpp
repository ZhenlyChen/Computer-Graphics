#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include <imgui.h>
#define IMGUI_IMPL_OPENGL_LOADER_GLEW
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

using std::string;

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
void processInput(GLFWwindow* window);

string loadFile(const char* path);
void checkStatus(unsigned int shader, GLenum type);

struct GLSLCode {
  const char* p;
  GLSLCode(const std::string& s) : p(s.c_str()) {}
  operator const char** () { return &p; }
};

int main() {
  // ��ʼ��GLFW OpenGL4.5
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // ��ʼ������
  GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWorld", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // ��ʼ��GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // ��ʼ���ӿ�
  glViewport(0, 0, 800, 600);

  // ����
  float vertices[] = {
    // λ��             // ��ɫ
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // ����
    1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // ����
    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f  // ��
  };
  // ���㻺�����
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // �󶨻���
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // ��������
  // GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
  // GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
  // GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // ������������
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // ������ɫ����
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // �����
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // ������ɫ��
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, GLSLCode(loadFile("vertices.glsl")), NULL);
  glCompileShader(vertexShader);
  checkStatus(vertexShader, GL_COMPILE_STATUS);

  // Ƭ����ɫ��
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, GLSLCode(loadFile("fragment.glsl")), NULL);
  glCompileShader(fragmentShader);
  checkStatus(fragmentShader, GL_COMPILE_STATUS);

  // ��ɫ������
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkStatus(shaderProgram, GL_LINK_STATUS);
  // ʹ����ɫ��
  glUseProgram(shaderProgram);

  // ɾ����ɫ��
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

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


  ImVec4 triangles_color = ImVec4(0.95f, 0.25f, 0.30f, 1.00f);
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ImGui��Ⱦ
    {
      // ������
      ImGui::Begin("Change color");

      ImGui::ColorEdit3("Triangles color", (float*)&triangles_color);
      ImGui::ColorEdit3("Clear color", (float*)& clear_color);
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
    // ��������
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwMakeContextCurrent(window);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

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

string loadFile(const char* path) {
  string shaderCode;
  std::ifstream shaderStream(path);
  std::ostringstream buf;
  char ch;
  while (buf && shaderStream.get(ch)) {
    buf.put(ch);
  }
  return buf.str();
}

void checkStatus(unsigned int shader, GLenum type) {
  int success;
  glGetShaderiv(shader, type, &success);
  if (!success) {
    char infoLogs[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLogs);
    std::cout << "Error:\n" << infoLogs << std::endl;
  }
  else {
    // std::cout << "Load " << shader << " success" << std::endl;
  }
}