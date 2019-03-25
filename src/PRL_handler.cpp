#include <iostream>

#include "PRL_handler.h"


PRL_Handler :: PRL_Handler()
{
    time.stopwatch.start();
    PRL_Displayer disp0;
    display.displayer.push_back(disp0);
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
	return loadAnimation(file_path, display.renderer[0]);
}

PRL_Animation* PRL_Handler :: loadAnimation(const std::string& file_path, SDL_Renderer* render)
{
	if (render == nullptr)
	{
		PRL_SetError("Invalid renderer");
		return nullptr;
	}

	PRL_Animation* anim(nullptr);
	try
	{
		anim = new PRL_Animation(file_path, render);
	}
	catch (std::string e)
	{
		return nullptr;
	}

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
	try
	{
		animd = new PRL_Animated();
	}
	catch (std::string e)
	{
		return nullptr;
	}

	if (animd != nullptr)
	{
		animd->setAnim(anim);
		display.animated.push_back(animd);
        display.displayer[0].add(animd);
	}

	return animd;
}

// Time

long long PRL_Handler :: _time :: getTimeUpdated() const
{
	return timeUpdated;
}

void PRL_Handler :: _time :: update()
{
	timeUpdated = PRL_GetTicks();
}

void PRL_Handler :: update()
{
	input.update();
	time.update();
	for (size_t i(0); i < display.displayer.size(); ++i)
	{
		display.displayer[i].display();
		SDL_RenderPresent(display.renderer[i]);
		SDL_RenderClear(display.renderer[i]);
	}
}

/*PRL_Animated* PRL_Handler :: createAnimated()
{
	PRL_Animated* animd(nullptr);
	animd = new PRL_Animated();

	if (animd != nullptr)
		display.animated.push_back(animd);

	return animd;
}*/
