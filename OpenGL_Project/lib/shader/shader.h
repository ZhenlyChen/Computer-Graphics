#pragma once

#include <glad/glad.h>
#include "../imgui/imgui.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

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

private:
  unsigned int id;
  std::string loadFile(const char* path);
  void checkStatus(unsigned int shader, GLenum type);
};
