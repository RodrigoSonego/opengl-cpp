#include "Animator.h"

void Animator::play(SubTexture* subTex, int numberOfFrames, float deltaTime, bool willLoop, bool playBackwards)
{
	if (canPlay == false) { return; }

	if (currentFrame == numberOfFrames - 1) {
		if (willLoop)
			currentFrame = 0;
		else {
			return;
		}
	}

	float delayBtweenFrames = 0.2f;

	Texture* tex = subTex->getTexture();

	timeElapsed += deltaTime;
	if (timeElapsed >= delayBtweenFrames) {
		timeElapsed = 0;

		float offset = playBackwards ? -1.0f : 1.0f;

		glm::vec2 next = glm::vec2(subTex->getSpritePosition().x + offset, subTex->getSpritePosition().y);
		subTex->updateSpritePosition(next);

		++currentFrame;
	}
}

void Animator::stopAllAnimations()
{
	canPlay = false;

	currentFrame = 0;
	timeElapsed = 0.0f;
}

void Animator::enablePlaying()
{
	canPlay = true;
}
