#include <iostream>

#include "PRL_handler.h"


PRL_Handler :: PRL_Handler()
{
    time.stopwatch.start();
    PRL_Displayer disp0;
    display.displayer.push_back(disp0);

    PRL_Collider colr0;
    collision.collider.push_back(colr0);
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
	catch (std::exception e)
	{
		return nullptr;
	}

	if (anim != nullptr)
		display.animation.push_back(anim);
	else
		return nullptr;

	return anim;
}

PRL_Animated* PRL_Handler :: createAnimated(PRL_Animation* anim)
{
	if (anim == nullptr)
	{
		PRL_SetError("Invalid animation");
		return nullptr;
	}

	PRL_Animated* animd(nullptr);
	try
	{
		animd = new PRL_Animated();
	}
	catch (std::exception e)
	{
		return nullptr;
	}

	if (animd != nullptr)
	{
		animd->setAnim(anim);
		display.animated.push_back(animd);
        display.displayer[0].add(animd);
        collision.collider[0].add(animd);
	}
	else
		return nullptr;

	return animd;
}

PRL_Image* PRL_Handler :: loadImage(const std::string& file_path)
{
	return loadImage(file_path, display.renderer[0]);
}

PRL_Image* PRL_Handler :: loadImage(const std::string& file_path, SDL_Renderer* render)
{
	if (render == nullptr)
	{
		PRL_SetError("Invalid renderer");
		return nullptr;
	}

	PRL_Image* img(nullptr);
	try
	{
		img = new PRL_Image(file_path, render);
	}
	catch (std::exception e)
	{
		return nullptr;
	}

	if (img != nullptr)
		display.image.push_back(img);
	else
		return nullptr;

	return img;
}

PRL_Sprite* PRL_Handler :: createSprite(PRL_Image* image)
{
	if (image == nullptr)
	{
		PRL_SetError("Invalid animation");
		return nullptr;
	}

	PRL_Sprite* sprite(nullptr);
	try
	{
		sprite = new PRL_Sprite();
	}
	catch (std::exception e)
	{
		return nullptr;
	}

	if (sprite != nullptr)
	{
		sprite->setImage(image);
		display.sprite.push_back(sprite);
        display.displayer[0].add(sprite);
	}
	else
		return nullptr;

	return sprite;
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
	collision.collider[0].testCollisions();
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
