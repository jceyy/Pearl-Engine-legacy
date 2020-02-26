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
/// Define to control throwing exceptions on error or only returns
// To DO: add save-memory settings flag to reduce loaded textures then implement in load animation

int PRL_TestZone()
{
	bool quit(false);

	PRL_Animation* animMario = handler.loadAnimation("data/mario/mario.prl");
	PRL_Animation* animWow = handler.loadAnimation("data/wow/wow.prl");
	PRL_Image* image = handler.loadImage("data/imgtest/img.prl");
	if (animMario == nullptr || image == nullptr || animWow == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	PRL_Sprite* background = handler.createSprite(image);
	PRL_Animated* wowGuy = handler.createAnimated(animWow);
    PRL_Animated* mario = handler.createAnimated(animMario);
	if (mario == nullptr || background == nullptr || wowGuy == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	background->setPos(1000.0, 100.0);
	PRL_FPoint mpos(0.0, 0.0);
	PRL_FPoint wowpos(0.0, 0.0);

	// Textbox test
	SDL_Color color_white = {255, 128, 255, 64};
	int font_size = 180;
    PRL_Font test_font("data/fonts/cooper.ttf", font_size, TTF_STYLE_ITALIC, color_white);
	PRL_TextLabel* textLabelTest = handler.createTextLabel("Text box test", test_font);
	textLabelTest->setPos(400, 500);

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

        if (handler.input.isKeyPressedAndReset(SDL_SCANCODE_SPACE)) // hide or show object
        {
            background->toggleDisplay();
            mario->toggleDisplay();
            wowGuy->toggleDisplay();
            textLabelTest->toggleDisplay();
        }

        if (handler.input.isKeyPressedAndReset(SDL_SCANCODE_B)) // hide or show box around object
        {
            background->toggleBoxDisplay();
            mario->toggleBoxDisplay();
            wowGuy->toggleBoxDisplay();
            textLabelTest->toggleBoxDisplay();
        }

		if (handler.input.isKeyPressed(SDL_SCANCODE_UP))
        {
            mpos.y -= 50;
            textLabelTest->setRenderStyle(PRL_TEXTSTYLE_BLENDED);
            textLabelTest->setText("Mangez des pommes up!");
        }
		if (handler.input.isKeyPressed(SDL_SCANCODE_DOWN))
        {
            mpos.y += 50;
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
		wowGuy->setPos(wowpos);
        wowGuy->update();

		handler.update();
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
