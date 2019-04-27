
#include"Camera.h"

static Camera* CameraInst;


Camera::Camera(GLFWwindow* window, int height, int width) {
  CameraInst = this;

  this->Position = glm::vec3(0.0f, 3.0f, 10.0f);
  this->Front = glm::vec3(0.0f, 0.0f, 0.0f);
  this->WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
  this->Up = glm::vec3(0.0f, 1.0f, 0.0f);
  this->Yaw = YAW;
  this->Pitch = PITCH;
  this->MovementSpeed = SPEED;
  this->MouseSensitivity = SENSITIVITY;
  this->Zoom = ZOOM;
  this->enableControl = false;
  this->lastX = width / 2.0f;
  this->lastY = height / 2.0f;

  glfwSetCursorPosCallback(window, this->MouseCallback);
  glfwSetScrollCallback(window, this->ScrollCallback);
}

glm::mat4 Camera::GetViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

glm::mat4 Camera::GetCenterViewMatrix() {
  return glm::lookAt(Position, Front, Up);
}

void Camera::UpDateDeltaTime() {
  float currentFrame = glfwGetTime();
  deltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
}

void Camera::ProcessInput(GLFWwindow* window) {

  float velocity = MovementSpeed * deltaTime;

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    Position += Front * velocity;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    Position -= Front * velocity;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    Position -= Right * velocity;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    Position += Right * velocity;
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    Position += Up * velocity;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    Position -= Up * velocity;


  // 捕获鼠标
  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

  // 释放鼠标
  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Camera::MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  if (!CameraInst->enableControl) return;
  if (CameraInst->firstMouse) {
    CameraInst->lastX = xpos;
    CameraInst->lastY = ypos;
    CameraInst->firstMouse = false;
  }

  float xoffset = xpos - CameraInst->lastX;
  float yoffset = CameraInst->lastY - ypos;

  CameraInst->lastX = xpos;
  CameraInst->lastY = ypos;

  xoffset *= CameraInst->MouseSensitivity;
  yoffset *= CameraInst->MouseSensitivity;

  CameraInst->Yaw += xoffset;
  CameraInst->Pitch += yoffset;

  if (CameraInst->Pitch > 89.0f)
    CameraInst->Pitch = 89.0f;
  if (CameraInst->Pitch < -89.0f)
    CameraInst->Pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(CameraInst->Yaw)) * cos(glm::radians(CameraInst->Pitch));
  front.y = sin(glm::radians(CameraInst->Pitch));
  front.z = sin(glm::radians(CameraInst->Yaw)) * cos(glm::radians(CameraInst->Pitch));
  CameraInst->Front = glm::normalize(front);
  CameraInst->Right = glm::normalize(glm::cross(CameraInst->Front, CameraInst->WorldUp));
  CameraInst->Up = glm::normalize(glm::cross(CameraInst->Right, CameraInst->Front));
}

void Camera::ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
  if (!CameraInst->enableControl) return;
  if (CameraInst->Zoom >= 1.0f && CameraInst->Zoom <= 45.0f)
    CameraInst->Zoom -= yoffset;
  if (CameraInst->Zoom <= 1.0f)
    CameraInst->Zoom = 1.0f;
  if (CameraInst->Zoom >= 45.0f)
    CameraInst->Zoom = 45.0f;
}