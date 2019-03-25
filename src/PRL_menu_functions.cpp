#include "PRL.h"
#include "PRL_input.h"
#include "PRL_display.h"
#include "PRL_animation.h"

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

	if (mario == nullptr) // error
	{
		cout << "Loading failed: " << PRL_GetError() << endl;
		return PRL_ERROR;
	}

	printAnimDiagnostics(animMario);

    mario->setPos(200.0, 150.0);

    while(!quit)
	{
		if (handler.input.quitEvent())
		{
			quit = true;
		}

		mario->update();

		handler.update();
		PRL_Delay(9000);
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
