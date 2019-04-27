#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// ������ƶ�����
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

// Ĭ�ϲ���
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.04f;
const float ZOOM = 45.0f;

class Camera {
public:
  // Camera ����
  glm::vec3 Position;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  // Euler �Ƕ�
  float Yaw;
  float Pitch;
  // �ƶ�������
  float MovementSpeed;
  // ���������
  float MouseSensitivity;
  // ��������
  float Zoom;
  // �Ƿ�������
  bool enableControl;

  // ��ʼ�����󶨴��ڿ���
  Camera(GLFWwindow* window, int height, int width);
  // ��ȡ������ͼ����
  glm::mat4 GetViewMatrix();
  // ��ȡ������ͼ����
  glm::mat4 GetCenterViewMatrix();
  // �����ٶ�
  void UpDateDeltaTime();
  // �����������
  void ProcessInput(GLFWwindow* window);
  // ��������ƶ�
  static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
  // ������ֹ���
  static void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

private:
  // ��ǰ״̬
  float lastX;
  float lastY;
  bool firstMouse = true;
  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
};
