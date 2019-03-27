#include "PRL.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

void printAnimDiagnostics(PRL_Animation* anim);

int PRL_TestZone()
{
	// To DO: add save-memory settings flag to reduce loaded textures then implement in load animation
	bool quit(false);
	PRL_Animation* animMario = handler.loadAnimation("data/mario/mario.anim");

	if (animMario == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	PRL_Animated* mario = handler.createAnimated(animMario);
    PRL_Animated* mario_erstatz = handler.createAnimated(animMario);
	if (mario == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	//printAnimDiagnostics(animMario);

    PRL_FPoint mpos({0.0,0.0});
    PRL_FPoint mepos({0.0,0.0});
    bool showmario = false;

    while(!quit)
	{
		if (handler.input.quitEvent())
		{
			quit = true;
		}

		if (handler.input.isKeyPressed(SDL_SCANCODE_UP))
        {
            mpos.y -= 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_DOWN))
        {
            mpos.y += 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_LEFT))
        {
            mpos.x -= 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_RIGHT))
        {
            mpos.x += 50;
        }

        mario->setPos(mpos);
		mario->update();
        mario_erstatz->setPos(mpos);
		mario_erstatz->update();
        showmario = false;

		if (mpos.y <= 0)
		{
            mepos.y = handler.config.getRenderResolution().y;
            showmario = true;
		}
        else if (mpos.y >= handler.config.getRenderResolution().y + animMario->display.getSize((size_t) mario->getCurrentFrame()).y)
        {
            mepos.y = -animMario->display.getSize((size_t) mario_erstatz->getCurrentFrame()).y;
            showmario = true;
        }
        if (mpos.x <= 0)
        {
            mepos.x = handler.config.getRenderResolution().x;
            showmario = true;
        }
        else if (mpos.x >= handler.config.getRenderResolution().x + animMario->display.getSize((size_t) mario->getCurrentFrame()).x)
        {
            mepos.x = -animMario->display.getSize((size_t) mario_erstatz->getCurrentFrame()).x;
            showmario = true;
        }

        mario_erstatz->makeActive(showmario);

		handler.update();
		PRL_Delay(10000);
	}
    return 0;
}

void printAnimDiagnostics(PRL_Animation* anim)
{
	PRL_Point p = anim->display.getRefRenderSize();
	cout << "Animation properties:" << endl << endl;
    cout << "Frames number: " << anim->display.getFramesNumber() << endl;
    cout << "Frame rate: " << anim->display.getFPS() << endl;
    cout << "Reference render size: " << p.x << "x" << p.y << endl << endl;
}
