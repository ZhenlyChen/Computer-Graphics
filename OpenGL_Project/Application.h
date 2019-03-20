#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Application {
public:
  virtual void prepare() = 0;

  virtual void render() = 0;

  virtual void clear() = 0;

  virtual void input(GLFWwindow* window) = 0;
  
  int defaultHeight = 600;

  int defaultWidth = 800;

  std::string title = "OpenGL";
};