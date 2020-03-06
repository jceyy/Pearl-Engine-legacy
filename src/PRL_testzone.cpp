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

	PRL_Animation* animMario = handler.loadAnimation("data/mario/mario.prl"); cout << "Mario load\n";
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

	background->setPos(100*1000.0, 100.0);
	PRL_FPoint mpos(0.0, 0.0);
	PRL_FPoint wowpos(0.0, 0.0);

	// Textbox test
	SDL_Color color_text = {255, 128, 255, 127};
	int font_size = 180;
    PRL_Font test_font("data/fonts/cooper.ttf", font_size, TTF_STYLE_ITALIC, color_text);
	PRL_TextLabel* textLabelTest = handler.createTextLabel("Text box test", test_font);
	textLabelTest->setPos(0, 0);


	/// WORLD BUILDING
	PRL_FPoint conceptionRenderer(3840, 2160); // all positions conceived for 4K
	PRL_FPoint posRatio(conceptionRenderer.x/handler.config.renderResolution.x,
                     conceptionRenderer.y/handler.config.renderResolution.y);
	//PRL_Image *im_sky = handler.loadImage("data/world_asset/sky.prl");
	PRL_Image *im_sun = handler.loadImage("data/world_asset/sun.prl"); cout << "Sun load\n";
	PRL_Image *im_flow1 = handler.loadImage("data/world_asset/flower1.prl"); cout << "Flower load\n";
	//PRL_Image *im_flow2 = handler.loadImage("data/world_asset/flower2.prl");
	//PRL_Image *im_flow3 = handler.loadImage("data/world_asset/flower3.prl");
	PRL_Image *im_grd1 = handler.loadImage("data/world_asset/ground1.prl");
	PRL_Image *im_grd2 = handler.loadImage("data/world_asset/ground2.prl");
	PRL_Image *im_grd3 = handler.loadImage("data/world_asset/ground3.prl"); cout << "Ground3 load\n";

	PRL_Sprite *sun = handler.createSprite(im_sun);
	sun->setPos(1920, 270);

	std::vector <PRL_Sprite*> ground(18);
	for (size_t i(0); i < ground.size() / 3; ++i)
    {
        float x0(150.0f + 3.0f*i*im_grd3->display.getSize().x), y0(2000.0f);
        ground[i] = handler.createSprite(im_grd1);
        ground[i + 1] = handler.createSprite(im_grd2);
        ground[i + 2] = handler.createSprite(im_grd3);

        ground[i]->setPos(x0, y0);
        ground[i + 1]->setPos(x0 + im_grd1->display.getSize().x, y0);
        ground[i + 2]->setPos(x0 + 2.0 * im_grd2->display.getSize().x, y0);
    }

    std::vector <PRL_Sprite*> flower(4);
    for (size_t i(0); i < flower.size(); ++i)
    {
        flower[i] = handler.createSprite(im_flow1);
        flower[i]->setPos(ground[i]->getDstRect().x, ground[i]->getDstRect().y - im_flow1->display.getSize().y);
    }

	handler.printClassDiagnostics();
    size_t temp_index(0);

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

        if (handler.input.isKeyPressedAndReset(SDL_SCANCODE_C)) // Change color of boxes
        {
            temp_index = (temp_index + 1) % 2;
            // "temp" : temporary function, will be removed as collisions get completely implemented
            background->tempSetIndex(temp_index);
            mario->tempSetIndex(temp_index);
            wowGuy->tempSetIndex(temp_index);
            textLabelTest->tempSetIndex(temp_index);
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
		wowGuy->setPos(wowpos);

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
