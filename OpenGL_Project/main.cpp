#include <iostream>
using std::string;

#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui_impl_glfw.h"
#include "lib/imgui/imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "application.h"
#include "triangles/triangles.h"
#include "Points/Points.h"
#include "Transformation/Transformation.h"
#include "Camera/CameraApp.h"

void framebuffer_size_callback(GLFWwindow* window, int w, int h);
GLFWwindow* initWindow(Application* app, int height, int width);

int main() {
  // 初始化
  // Application *app = new Triangles();
  // Application* app = new Points();
  // Application* app = new Transformation();
  Application* app = new CameraApp();

  GLFWwindow* window = initWindow(app, app->defaultHeight, app->defaultWidth);
  if (window == NULL) return -1;

  app->prepare();

  while (!glfwWindowShouldClose(window)) {
    app->input(window);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    app->render();

    // ImGui::ShowDemoWindow();

    // ImGui 渲染
    ImGui::Render();
    glfwMakeContextCurrent(window);
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  app->clear();

  glfwTerminate();
  return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int w, int h) {
  glViewport(0, 0, w, h);
}

GLFWwindow* initWindow(Application* app, int height, int width) {
  // 初始化GLFW
  glfwInit();
  // OpenGL 4.5
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // 初始化窗体
  GLFWwindow* window = glfwCreateWindow(width, height, app->title.c_str(), NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return NULL;
  }
  glfwMakeContextCurrent(window); 
  glfwSwapInterval(1); // Enable vsync
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glfwSetWindowSizeLimits(window, width, height, width, height);

  // 初始化GLAD
  if (!gladLoadGL()) {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return NULL;
  }

  // 初始化视口
  glViewport(0, 0, width, height);

  // 初始化ImGui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 450");
  // glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

  app->init(window);

  return window;
}