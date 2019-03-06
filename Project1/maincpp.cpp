#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
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
  // 初始化GLFW OpenGL4.5
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 初始化窗体
  GLFWwindow* window = glfwCreateWindow(800, 600, "HelloWorld", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // 初始化GLAD
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // 初始化视口
  glViewport(0, 0, 800, 600);

  // 顶点
  float vertices[] = {
    // 位置             // 颜色
    -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 左下
    1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f, // 右下
    0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 0.0f  // 上
  };
  // 顶点缓冲对象
  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // 绑定缓冲
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // 复制数据
  // GL_STATIC_DRAW ：数据不会或几乎不会改变。
  // GL_DYNAMIC_DRAW：数据会被改变很多。
  // GL_STREAM_DRAW ：数据每次绘制时都会改变。
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // 解析顶点数据
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // 解析颜色数据
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // 解除绑定
  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // 顶点着色器
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, GLSLCode(loadFile("vertices.glsl")), NULL);
  glCompileShader(vertexShader);
  checkStatus(vertexShader, GL_COMPILE_STATUS);

  // 片段着色器
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, GLSLCode(loadFile("fragment.glsl")), NULL);
  glCompileShader(fragmentShader);
  checkStatus(fragmentShader, GL_COMPILE_STATUS);

  // 着色器程序
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);
  checkStatus(shaderProgram, GL_LINK_STATUS);
  // 使用着色器
  glUseProgram(shaderProgram);

  // 删除着色器
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);



  while (!glfwWindowShouldClose(window)) {
    processInput(window);

    // 渲染
    // 背景颜色
    // glClearColor(0.2f, 0.5f, 0.8f, 1.0f);
    // glClear(GL_COLOR_BUFFER_BIT);
    // 画三角形
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

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