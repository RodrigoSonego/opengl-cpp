#pragma once
#include "SubTexture.h"
#include <string>

class Animator
{
public:
	bool willLoop = false;

	/// <summary>
	/// Plays animation taking a starter subtexture
	/// </summary>
	/// <param name="subTex">SubTexture to be animated</param>
	/// <param name="numberOfFrames">number of frames to be animated</param>
	/// <param name="animName">name of the animation. Only really used when an object uses multiple animations</param>
	void play(SubTexture* subTex, int numberOfFrames, float deltaTime,
			bool willLoop, bool playBackwards = false, std::string animName = "idle");
	/// <summary>
	/// Sets isFinished to true and resets current frame, stopping the animation
	/// </summary>
	void stopAnimating();
	
	bool isFinished = false;
	int getCurrentFrame();

private:
	int currentFrame = 0;
	float timeElapsed = 0.0f;
	glm::vec2 m_FirstSprite;

	std::string currentAnimName;
};

