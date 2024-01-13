#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

#include <iostream>

Camera::Camera(glm::vec3 initialPos)
{
	transform.position = initialPos;
}

glm::mat4 Camera::getView()
{
	glm::mat4 modelMat = glm::translate(glm::mat4(1.0f), transform.position );
	return glm::inverse(modelMat);
}

glm::mat4 Camera::getPerspective(float ratio)
{
	return glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
}

// TODO: make this be able to rotate 360º horizontally
void Camera::processMouseInput(SDL_Event ev)
{
	if (ev.type == SDL_MOUSEMOTION) {

		float xOffset = ev.motion.xrel;
		float yOffset = -ev.motion.yrel;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		updateCameraFront();
	}

	if (ev.type == SDL_MOUSEWHEEL)
	{
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= ev.wheel.y;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
}

void Camera::updateCameraFront() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);
}

void Camera::moveWithKeyboard(float deltaTime, float speedModifier)
{
	//float cameraSpeed = speedModifier * deltaTime; // adjust accordingly
	//const Uint8* keyState;
	//keyState = SDL_GetKeyboardState(NULL);
	//if (keyState[SDL_SCANCODE_W])
	//	cameraPosition += cameraSpeed * glm::vec3(1, 0, 1) * cameraFront;
	//if (keyState[SDL_SCANCODE_S])
	//	cameraPosition -= cameraSpeed * glm::vec3(1, 0, 1) * cameraFront;
	//if (keyState[SDL_SCANCODE_A])
	//	cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * glm::vec3(1, 0, 1) * cameraSpeed;
	//if (keyState[SDL_SCANCODE_D])
	//	cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * glm::vec3(1, 0, 1) * cameraSpeed;

	//cameraPosition = glm::clamp(cameraPosition, cameraMinPos, cameraMaxPos);
}


