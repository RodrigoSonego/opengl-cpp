#pragma once
#include <glm/glm.hpp>
#include <SDL.h>

class Camera
{
public:
	Camera(glm::vec3 initialPos);
	
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	glm::vec3 cameraPos;

	glm::mat4 getView();
	glm::mat4 getPerspective(float ratio);
	void processMouseInput(SDL_Event event);
	void moveWithKeyboard(float deltaTime, float speedModifier = 4.f);


private:
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float fov = 45.0f;
	
	float pitch;
	float yaw = -90.0f;

	float lastMouseX = 400;
	float lastMouseY = 300;
};

