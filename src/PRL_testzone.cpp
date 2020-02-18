#include "PRL_testzone.h"

#include <iostream>
#include <cmath>

using std::cout;
using std::cerr;
using std::endl;
using std::string;


/// PRIORITIES
/// Collisions (rect at least)
/// Motion (instead of += cst for positions)
/// UI Elements

int PRL_TestZone()
{
	// To DO: add save-memory settings flag to reduce loaded textures then implement in load animation
	bool quit(false);

	PRL_Animation* animMario = handler.loadAnimation("data/mario/mario.prl");
	PRL_Animation* animWow = handler.loadAnimation("data/wow/wow.prl");
	PRL_Image* image = handler.loadImage("data/imgtest/img.prl");
	if (animMario == nullptr || image == nullptr || animWow == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	PRL_Sprite* sprite = handler.createSprite(image);
	PRL_Animated* wowGuy = handler.createAnimated(animWow);
	PRL_Animated* mario_erstatz = handler.createAnimated(animMario);
    PRL_Animated* mario = handler.createAnimated(animMario);

	if (mario == nullptr || mario_erstatz == nullptr || sprite == nullptr || wowGuy == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	sprite->setPos(1000.0, 100.0);

	//printAnimDiagnostics(animMario);

	PRL_FPoint mpos(0.0, 0.0);
	PRL_FPoint mepos(0.0, 0.0);
	PRL_FPoint wowpos(0.0, 0.0);
	bool showmario = false;

	// Textbox test
	SDL_Color color_white = {255, 128, 255, 0};
	int font_size = 180;
    PRL_Font test_font("data/fonts/cooper.ttf", font_size, TTF_STYLE_ITALIC, color_white);
	PRL_TextLabel* textLabelTest = handler.createTextLabel("Mangez des pommes", test_font);
	textLabelTest->setPos(400, 500);

	//long long cpt(0);
	handler.printClassDiagnostics();
	while(!quit)
	{
		if (handler.input.quitEvent())
		{
			quit = true;
		}
		if (handler.input.isKeyPressed(SDL_SCANCODE_ESCAPE))
        {
            quit = true;
        }

		if (handler.input.isKeyPressed(SDL_SCANCODE_UP))
        {
            mpos.y -= 50;

            font_size += 2;
            test_font.setSize(font_size);
            textLabelTest->setRenderStyle(PRL_TEXTSTYLE_BLENDED);
            textLabelTest->setText("Mangez des pommes up!");
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_DOWN))
        {
            mpos.y += 50;

            font_size -= 2;
            test_font.setSize(font_size);
            textLabelTest->setRenderStyle(PRL_TEXTSTYLE_SOLID);
            textLabelTest->setText("Mangez des pommes down!");
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_LEFT))
        {
            mpos.x -= 50;
            test_font.setStyle(TTF_STYLE_UNDERLINE|TTF_STYLE_ITALIC);
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_RIGHT))
        {
            mpos.x += 50;
            test_font.setStyle(TTF_STYLE_STRIKETHROUGH);
        }

        if (handler.input.isKeyPressed(SDL_SCANCODE_I))
        {
            wowpos.y -= 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_K))
        {
            wowpos.y += 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_J))
        {
            wowpos.x -= 50;
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_L))
        {
            wowpos.x += 50;
        }

        mario->setPos(mpos);
		mario->update();
        mario->setRotAngle((double)sqrt(mpos.x*mpos.x+mpos.y*mpos.y)/100);
		wowGuy->setPos(wowpos);
		wowGuy->setRotAngle((double)sqrt(wowpos.x*wowpos.x+wowpos.y*wowpos.y)/10);
        wowGuy->update();

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

        mario_erstatz->disableDisplay();
		showmario = !showmario;

		handler.update();

		/*if (wowGuy->isColliding() && mario->isColliding())
		{
			cpt++;
			cout << "Collision detected! " + to_string(cpt) << endl;
		}*/
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

