#pragma once
#include "SubTexture.h"

class Animator
{
public:
	bool willLoop = false;

	void play(SubTexture* subTex, int numberOfFrames, float deltaTime, bool willLoop, bool playBackwards = false);

private:
	int currentFrame = 0;
	float timeElapsed = 0.0f;
	bool canPlay = true;

	void stopAllAnimations();
	void enablePlaying();
};

