#include "shader.h"

Shader::Shader(const char * vertexPath, const char* fragmentPath) {
  // 顶点着色器
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, GLSLCode(loadFile(vertexPath)), NULL);
  glCompileShader(vertexShader);
  checkStatus(vertexShader, GL_COMPILE_STATUS);

  // 片段着色器
  unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, GLSLCode(loadFile(fragmentPath)), NULL);
  glCompileShader(fragmentShader);
  checkStatus(fragmentShader, GL_COMPILE_STATUS);

  // 着色器程序
  this->id = glCreateProgram();
  glAttachShader(this->id, vertexShader);
  glAttachShader(this->id, fragmentShader);
  glLinkProgram(this->id);
  checkStatus(this->id, GL_LINK_STATUS);

  // 删除着色器
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

void Shader::Use() {
  // 使用着色器
  glUseProgram(this->id);
}

unsigned int Shader::GetID() {
  return this->id;
}
// uniform
void Shader::SetBool(const std::string& name, bool value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), (int)value);
}
void Shader::SetInt(const std::string& name, int value) const {
  glUniform1i(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::SetFloat(const std::string& name, float value) const {
  glUniform1f(glGetUniformLocation(this->id, name.c_str()), value);
}

void Shader::SetColor(const std::string& name, ImVec4 value) const {
  glUniform4f(glGetUniformLocation(this->id, name.c_str()), value.x,value.y, value.z, 1.0f);
}


void Shader::SetMat4(const std::string& name, glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(this->id, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

std::string Shader::loadFile(const char* path) {
  std::string shaderCode;
  std::ifstream shaderFile;
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  std::stringstream shaderStream;
  try {
    shaderFile.open(path);
    shaderStream << shaderFile.rdbuf();
    shaderFile.close();
  }
  catch (std::ifstream::failure e) {
    std::cout << "ERROR: Can't read " << path << std::endl;
  }
  return shaderStream.str();
}

void Shader::checkStatus(unsigned int shader, GLenum type) {
  int success;
  glGetShaderiv(shader, type, &success);
  if (!success) {
    char infoLogs[512];
    glGetShaderInfoLog(shader, 512, NULL, infoLogs);
    std::cout << "Error:\n" << infoLogs << std::endl;
  }
}
