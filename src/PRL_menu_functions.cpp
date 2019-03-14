#include "PRL.h"
#include "PRL_input.h"
#include "PRL_display.h"
#include "PRL_animation.h"

#include <iostream>

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int PRL_TestZone()
{
	PRL_Animation* animMario = handler.loadAnimation("data/mario/mario.anim");

	if (animMario == nullptr)
		cout << "Loading failed\n";

	PRL_Animated* mario = handler.createAnimated(animMario);

	if (mario == nullptr) cout << "Main error here" << endl;


	PRL_Displayer displayer;
	displayer.add(mario);

	//displayer.display();
	//SDL_RenderPresent(*handler.display.renderer[0]);
	PRL_Delay(1000000);


	//*renderer_GLOBAL=handler.renderer[0];
    //SDL_Texture *texture = SDL_CreateTexture(handler.renderer[0], SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 256, 256);
    //SDL_Loc
    /*string output("");
    config_GLOBAL.getText("tobySaysHello", output);

    cout << "DEBUGGING: " << output << endl << endl;*/

    /*SDL_SetRenderDrawColor(handler.renderer[0],0, 0, 0, 0); // add to configuration?
    #if PRL_USE_TARGET_TEXTURE == 1
        SDL_Rect posTarget={0};
        SDL_QueryTexture(targetTexture[0], NULL, NULL, &(posTarget.w), &(posTarget.h));
    #endif // PRL_USE_TARGET_TEXTURE

    PRL_Displayer *displayer = new PRL_Displayer();

    PRL_Input *input = new PRL_Input();

    // Animation Simple
    PRL_AnimationSimple* animSimple = new PRL_AnimationSimple();
    animSimple->load("data/explode1/animtest.anim");
    //animSimple->load("data/mario/mario.anim");

    int switcher(0), i(0);
    //int FPS = 60+0*animSimple->getFrameRate(), i(0);

    // Animated Simple
    PRL_AnimatedSimple *animatedSimple1 = new PRL_AnimatedSimple();
    //animatedSimple1->add(animSimple2);
    animatedSimple1->add(animSimple);
    animatedSimple1->startAnim();

    PRL_AnimationVideo* videoTest = new PRL_AnimationVideo(5);
    videoTest->load("data/animtest/animtest.anim");
    videoTest->setPos(200, 200);
    videoTest->fillBufferAtUpdate(true);
    videoTest->startAnim();

    // Vertical Scroll Bar
    PRL_VScrollBar *scrollBar1 = new PRL_VScrollBar(input, 1080/2);
    scrollBar1->setPos(1280/6, 720/8);
    if (scrollBar1->load("data/scrollbar1/scrollbar1.hud") != 0) return PRL_ERROR;

    //Horizontal Scroll Bar
    PRL_HScrollBar *scrollBar2 = new PRL_HScrollBar(input, 450);
    scrollBar2->setPos(1280/4, 720/5);
    if (scrollBar2->load("data/default/hsbar1.hud") != 0) return PRL_ERROR;

    // Text Box Simple
    PRL_TextBoxSimple *textBoxSimple = new PRL_TextBoxSimple;
    textBoxSimple->setReferenceRenderer(1920, 1080);
    textBoxSimple->setFont("data/fonts/arial_narrow_7.ttf", 50, 1500);
    textBoxSimple->setText("Testing of the first ever implemented PEARL text box! #great");
    textBoxSimple->useBackground({128, 128, 128, 128}, {255, 255, 255, 192}, 5);
    textBoxSimple->startTextInputTracking(input);
    textBoxSimple->update();
    textBoxSimple->allowDisplacement(true);
    textBoxSimple->setPos(10, 1080/2);

    // Text Label
    PRL_TextLabel *textLabel = new PRL_TextLabel;
    textLabel->setReferenceRenderer(1920, 1080);
    textLabel->setFont("data/fonts/arial_narrow_7.ttf", 50);
    textLabel->setText("Wooooooooh! A working text label!");
    textLabel->useBackground({128, 128, 128, 128}, {255, 255, 255, 192}, 5);
    textLabel->update();
    textLabel->setInput(input);
    textLabel->allowDisplacement(true);
    textLabel->setPos(1920/2, 1080*3/4);


    displayer->add(animatedSimple1);
    displayer->add(scrollBar1);
    displayer->add(scrollBar2);
    displayer->add(textBoxSimple);
    displayer->add(textLabel);
    //displayer->add(videoTest);
    //displayer->display(false);

    long long time(timer_GLOBAL.updateAndGetTime_us());
    bool quit = false;

    while (!quit)
    {
        input->update();
        if (input->isKeyPressed(SDL_SCANCODE_ESCAPE) || input->quitEvent())
        {
            quit = true;
        }
        if (0&&input->isKeyPressedAndReset(SDL_SCANCODE_KP_1))
        {
            if (animatedSimple1->isAnimPaused()) animatedSimple1->startAnim();
            else animatedSimple1->pauseAnim();
            input->resetKey(SDL_SCANCODE_SPACE);
            printf("Animation paused at frame %d/%d\n", animatedSimple1->getCurrentFrame()+1, animatedSimple1->getFramesNumber());
        }
        else if(input->isKeyPressedAndReset(SDL_SCANCODE_SPACE))
        {
            if (switcher == 0)
            {
                animatedSimple1->makeAnimActive(1);
                switcher = 1;
            }
            else
            {
                animatedSimple1->makeAnimActive(0);
                switcher = 0;
            }
            cout << switcher << endl;

        }
        if (input->isKeyPressed(SDL_SCANCODE_RETURN))
        {
            ;
        }

        animatedSimple1->updateAnim();
        //videoTest->updateAnim();
        scrollBar1->update();
        scrollBar2->update();
        textBoxSimple->update();
        textLabel->update();

        //if (scrollBar1->hasMoved() || scrollBar2->hasMoved()) printf("BAR 1: %f     Bar 2: %f\n", scrollBar1->getBarOutput(), scrollBar2->getBarOutput());

        SDL_RenderClear(*renderer_GLOBAL);
        displayer->display();
        SDL_RenderPresent(*renderer_GLOBAL);
        //videoTest->fillBuffer(1);
        i++;
        if (i == animSimple->getFramesNumber()) i=0;
        if (config_GLOBAL.cpuRelieve)
        {
            //long long temppp = ((long long) (1000000/config_GLOBAL.fps*0.99)-(timer_GLOBAL.updateAndGetTime_us() - time))/1000; cout << temppp << endl;
            //if (temppp > 0) SDL_Delay((Uint32)temppp);
            PRL_Delay((long long) (1000000/config_GLOBAL.fps*0.95)-(timer_GLOBAL.updateAndGetTime_us() - time)); // set the calculation in a constant
            time = timer_GLOBAL.us();
            //time = timer_GLOBAL.ms();
        }
    }
    //delete animSimple;
    ///delete animatedSimple1; // BUGGY
    ///delete videoTest; // BUGGY
    delete scrollBar1;
    delete scrollBar2;
    delete textBoxSimple;
    delete textLabel;*/

    return 0;
}
