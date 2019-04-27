#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// 摄像机移动方向
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// 默认参数
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.04f;
const float ZOOM = 45.0f;

class Camera {
public:
  // Camera 属性
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler 角度
  float Yaw;
  float Pitch;
  // 移动灵敏度
  float MovementSpeed;
  // 鼠标灵敏度
  float MouseSensitivity;
  // 滚轮缩放
  float Zoom;
  // 是否开启控制
  bool enableControl;

  // 初始化并绑定窗口控制
  Camera(GLFWwindow* window, int height, int width);
  // 获取自由视图矩阵
  glm::mat4 GetViewMatrix();
  // 获取中心视图矩阵
  glm::mat4 GetCenterViewMatrix();
  // 更新速度
  void UpDateDeltaTime();
  // 处理键盘输入
  void ProcessInput(GLFWwindow* window);
  // 处理鼠标移动
  static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
  // 处理滚轮滚动
  static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
  // 当前状态
  float lastX;
  float lastY;
  bool firstMouse = true;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
};
