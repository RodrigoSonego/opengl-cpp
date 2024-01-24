#include "Animator.h"
#include <iostream>
void Animator::play(SubTexture* subTex, int numberOfFrames, float deltaTime, 
	bool willLoop, bool playBackwards, std::string animName)
{
	if (numberOfFrames == 0) { return; }

	if (isFinished && willLoop == false && currentAnimName == animName)
	{
		return;
	}

	// Different anim name means it's entering the animator for the first time
	if (currentAnimName != animName) {
		currentFrame = 0;
		m_FirstSprite = subTex->getSpritePosition();
	}

	if (currentFrame > numberOfFrames - 1) {
		if (willLoop) {		
			currentFrame = 0;
			subTex->updateSpritePosition(m_FirstSprite);
		}
		else {
			stopAllAnimations();
			return;
		}
	}

	currentAnimName = animName;
	isFinished = false;
	

	float delayBtweenFrames = 0.13f;

	Texture* tex = subTex->getTexture();

	timeElapsed += deltaTime;
	if (timeElapsed >= delayBtweenFrames) {
		timeElapsed = 0;

		float offset = playBackwards ? -1.0f : 1.0f;

		subTex->switchToNextSprite(playBackwards);

		++currentFrame;
	}
}

void Animator::stopAllAnimations()
{
	currentFrame = 0;
	timeElapsed = 0.0f;
	isFinished = true;
}

int Animator::getCurrentFrame()
{
	return currentFrame;
}

