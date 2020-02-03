#include <iostream>

#include "PRL_handler.h"

int PRL_Handler :: handlerCount = 0;

int PRL_Handler :: getCount() noexcept
{
	return handlerCount;
}

PRL_Handler :: PRL_Handler()
{
    time.stopwatch.start();
    PRL_Displayer disp0;

    PRL_Collider col0;

    for (size_t i(0); i < PRL_PLAN_MAX; ++i)
    {
        display.displayer.push_back(disp0);
        collision.collider.push_back(col0);
    }

    handlerCount++;
}

PRL_Handler :: ~PRL_Handler()
{
    freeall();
    handlerCount--;
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

PRL_Animated* PRL_Handler :: createAnimated(PRL_Animation* anim, int plan)
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
        display.displayer[0].add(animd); cout << "DEBUG HANDLER (plan) " + to_string(plan) << endl;
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

PRL_Sprite* PRL_Handler :: createSprite(PRL_Image* image, int plan)
{
	plan ++; // plan is not used yet
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

PRL_TextLabel* PRL_Handler :: createTextLabel(std::string const& text, PRL_Font const& font)
{
	PRL_TextLabel* textLabel(nullptr);
	try
	{
		textLabel = new PRL_TextLabel(text, font);
	}
	catch (std::exception e)
	{
		return nullptr;
	}

	display.textLabel.push_back(textLabel);
	textLabel->renderText();
	display.displayer[0].add(textLabel);
	return textLabel;
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
	// text update here?

    for (size_t i(0); i < display.renderer.size(); ++i)
    {
        SDL_RenderClear(display.renderer[i]);
    }

	for (size_t i(0); i < display.displayer.size(); ++i)
	{
		display.displayer[i].display();
	}

	for (size_t i(0); i < display.renderer.size(); ++i)
    {
        SDL_RenderPresent(display.renderer[i]);
    }

    for (size_t i(0); i < display.textLabel.size(); ++i)
	{
		if (display.textLabel[i]->hasChanged())
			display.textLabel[i]->renderText();
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
