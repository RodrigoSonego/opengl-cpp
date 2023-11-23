#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>

#include <iostream>

Camera::Camera(glm::vec3 initialPos)
{
	cameraPos = initialPos;
}

glm::mat4 Camera::getView()
{
	return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);;
}

glm::mat4 Camera::getPerspective(float ratio)
{
	return glm::perspective(glm::radians(fov), ratio, nearPlane, farPlane);
}

void Camera::processMouseInput(SDL_Event ev)
{
	if (ev.type == SDL_MOUSEMOTION) {
		float xPos = ev.button.x;
		float yPos = ev.button.y;

		float xOffset = xPos - lastMouseX;
		float yOffset = lastMouseY - yPos;

		lastMouseX = xPos;
		lastMouseY = yPos;

		float sensitivity = 0.05f;
		xOffset *= sensitivity;
		yOffset *= sensitivity;

		yaw += xOffset;
		pitch += yOffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		static bool firstMouse = true;
		if (firstMouse)
		{
			firstMouse = false;
			lastMouseX = xPos;
			lastMouseY = yPos;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
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

void Camera::moveWithKeyboard(float deltaTime, float speedModifier)
{
	float cameraSpeed = speedModifier * deltaTime; // adjust accordingly
	const Uint8* keyState;
	keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W])
		cameraPos += cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_S])
		cameraPos -= cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_A])
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyState[SDL_SCANCODE_D])
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}


