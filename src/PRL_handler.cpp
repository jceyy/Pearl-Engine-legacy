#include <iostream>

#include "PRL_handler.h"


PRL_Handler :: PRL_Handler()
{
    time.stopwatch.start();
}

PRL_Handler :: ~PRL_Handler()
{
    freeall();
}

void PRL_Handler :: freeall()
{
	;
}

PRL_Animation* PRL_Handler :: loadAnimation(const std::string& file_path)
{
	PRL_Animation* anim(nullptr);
	anim = new PRL_Animation(file_path, display.renderer[0]);

	if (anim != nullptr)
		display.animation.push_back(anim);

	return anim;
}

PRL_Animated* PRL_Handler :: createAnimated(PRL_Animation* anim)
{
	if (anim == nullptr)
	{
		SDL_SetError("Invalid animation");
		return nullptr;
	}

	PRL_Animated* animd(nullptr);
	animd = new PRL_Animated();

	if (animd != nullptr)
	{
		display.animated.push_back(animd);
		animd->setAnim(anim);
	}

	return animd;
}

PRL_Animated* PRL_Handler :: createAnimated()
{
	PRL_Animated* animd(nullptr);
	animd = new PRL_Animated();

	if (animd != nullptr)
		display.animated.push_back(animd);

	return animd;
}
