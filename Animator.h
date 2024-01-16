#pragma once
#include "SubTexture.h"
#include <string>

class Animator
{
public:
	bool willLoop = false;

	void play(SubTexture* subTex, int numberOfFrames, float deltaTime,
			bool willLoop, bool playBackwards = false, std::string animName = "idle");
	void stopAllAnimations();
	
	bool isFinished = false;
	int getCurrentFrame();

private:
	int currentFrame = 0;
	float timeElapsed = 0.0f;

	std::string currentAnimName;
};

