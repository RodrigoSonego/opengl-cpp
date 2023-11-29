#pragma once
#include <glm/glm.hpp>
#include <SDL.h>

class Camera
{
public:
	Camera(glm::vec3 initialPos);
	
	const float nearPlane = 0.1f;
	const float farPlane = 100.f;

	glm::vec3 cameraPosition;

	glm::mat4 getView();
	glm::mat4 getPerspective(float ratio);

	/// <summary>
	/// Process and calculate camera direction values based on mouse movement
	/// </summary>
	/// <param name="event">SDL event of the active window</param>
	void processMouseInput(SDL_Event event);

	/// <summary>
	/// Tests for keyboard input (W,A,S,D) and move camera based on it
	/// </summary>
	/// <param name="deltaTime">Time since last frame</param>
	/// <param name="speedModifier">Speed the camera will move</param>
	void moveWithKeyboard(float deltaTime, float speedModifier = 4.f);


private:
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	float fov = 45.0f;
	
	float pitch = 0.0f; // Equivalent to rotation on the X axis on Unity
	float yaw = -90.0f;

	float lastMouseX = 400;
	float lastMouseY = 300;

	const glm::vec3 cameraMaxPos = glm::vec3(5, 2.5, 6);
	const glm::vec3 cameraMinPos = glm::vec3(-5, -2, -6);

	void updateCameraFront();
};

