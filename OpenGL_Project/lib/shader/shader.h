#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../imgui/imgui.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct GLSLCode {
  const char* p;
  GLSLCode(const std::string& s) : p(s.c_str()) {}
  operator const char** () { return &p; }
};

class Shader {
public:
  Shader(const char* vertexPath, const char* fragmentPath);
  void Use();
  unsigned int GetID();
  // uniform
  void SetBool(const std::string& name, bool value) const;
  void SetInt(const std::string& name, int value) const;
  void SetFloat(const std::string& name, float value) const;
  void SetColor(const std::string& name, ImVec4 value) const;
  void SetMat4(const std::string& name, glm::mat4& value) const;

private:
  unsigned int id;
  std::string loadFile(const char* path);
  void checkStatus(unsigned int shader, GLenum type);
};

#endif // !SHADER_H