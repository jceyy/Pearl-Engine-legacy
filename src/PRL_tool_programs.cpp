#include "PRL.h"
#include "PRL_input.h"
#include "PRL_display.h"
#include "PRL_animation.h"

#define WINDOW_NAME "PRL Animation Tester v1.2"
#define KEYS_STRING "   KEYS - A+Z (or Q+W): zoom, BACKSPACE: zoom reset, SPACE: pause-play, ENTER: hide keys from title"
#define TITLE_LENGTH 350
#define MAX_ZOOM 20.0


void updateTitle(const int FPS, const int currentFrame, const int totalFrames, const float zoom, SDL_Rect ref_rend, bool keys)
{
    char title[TITLE_LENGTH], temp[]=KEYS_STRING;
    strcpy(title, "");
    if (!keys) strcpy(temp, "");
    sprintf(title, "%s %4s Frame: %03d/%03d  FPS: %03d  Zoom: %.2f/%.2f  Cycle time: %.3f s   Reference renderer: %dx%d   Current renderer: %dx%d%s",
            WINDOW_NAME, " ", currentFrame, totalFrames-1, FPS, zoom, MAX_ZOOM, 1.0*totalFrames/FPS, ref_rend.x, ref_rend.y, config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y, temp); // -1 because the start is at nb = 0 like an array
    SDL_SetWindowTitle(*window_GLOBAL, title);
}


int PRL_Tool_AnimationTester()
{
    #if PRL_COMPILE_ANIM_TESTER == 1
    SDL_Texture *previous = NULL;
    int framesNumber(test->getFramesNumber()), i(0);
    #endif // PRL_COMPILE_ANIM_TESTER

    /*PRL_Input input;

    PRL_AnimationSimple* test = new PRL_AnimationSimple;
    test->load("anim/animtest.anim");
    int FPS = test->getFrameRate();
    SDL_Rect rect = test->getSize();

    PRL_AnimatedSimple object1;
    object1.add(test);
    object1.startAnim();

    displayer_GLOBAL[0].addObject(&object1);

    int x, y, w = rect.w, h = rect.h;
    float cpt = 1.0;

    long long time(0);
    bool quit = false;
    bool toggle_keys(true);

    while (!quit)
    {
        time = timer_GLOBAL.updateAndGetTime_micros();
        input.update();
        if (input.isKeyPressed(SDL_SCANCODE_ESCAPE) || input.quitEvent())
        {
            quit = true;
        }
        if (input.isKeyPressed(SDL_SCANCODE_SPACE))//||input.isMouseButtonPressed(1))
        {
            if (object1.isPaused()) object1.startAnim();
            else object1.pauseAnim();
            input.resetKey(SDL_SCANCODE_SPACE);
        }
        if (input.isKeyPressed(SDL_SCANCODE_A))
        {
            cpt-=0.2; if (cpt < 0.2) cpt = 0.2;
            rect.w = w * cpt;
            rect.h = h * cpt;
        }
        if (input.isKeyPressed(SDL_SCANCODE_Z))
        {
            cpt+=0.2; if (cpt > MAX_ZOOM) cpt = MAX_ZOOM;
            rect.w = w * cpt;
            rect.h = h * cpt;
        }
        if (input.isKeyPressedAndReset(SDL_SCANCODE_BACKSPACE))
        {
            cpt = 1.0;
            rect.w = w * cpt;
            rect.h = h * cpt;
        }
        if (input.isKeyPressedAndReset(SDL_SCANCODE_RETURN))
        {
            toggle_keys = !toggle_keys;
        }
        if (input.mouseHasMoved()||1)
        {
            input.queryMouse(&x, &y);
            rect.x = x - rect.w/2;
            rect.y = y - rect.h/2;
        }

        object1.updateAnim();

        SDL_RenderClear(*renderer_GLOBAL);
        displayer_GLOBAL[0].display(false);
        //SDL_RenderCopy(*renderer_GLOBAL, object1.getDisplayTexture(), NULL, &rect);
        SDL_RenderPresent(*renderer_GLOBAL);

        #if PRL_COMPILE_ANIM_TESTER == 1
        if (previous != object1.getDisplayTexture())
        {
            previous = object1.getDisplayTexture();
            i++;
            if (i == test->getFramesNumber()) i=0;
            updateTitle(FPS, i, framesNumber,  cpt, test->getReferenceRendererSize(), toggle_keys);
        }
        #endif // PRL_COMPILE_ANIM_TESTER

        PRL_Delay(1000000/FPS-(timer_GLOBAL.updateAndGetTime_micros()-time));
    }
    delete test;*/
    return 0;
}

