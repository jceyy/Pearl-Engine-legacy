#ifndef PRL_HANDLER_H_INCLUDED
#define PRL_HANDLER_H_INCLUDED

#include "SDL2/SDL.h"

#include "PRL_time.h"
#include "PRL_configuration.h"
#include "PRL_input.h"
#include "PRL_animation.h"

class PRL_Handler
{
public:

	PRL_Handler();
	~PRL_Handler();

	PRL_Config config;
	PRL_Input input;

	struct _display
	{
		int empty0;
	};
	_display display;

	struct _time
	{
		PRL_Stopwatch stopwatch; // automatically started
	};
	_time time;

	struct collision
	{
		int empty1;
	};

	struct network
	{
		int empty2;
	};

	struct physics
	{
		int empty3;
	};


	void loadAnimation(const std::string& file_path);

	void loadSprite(const std::string& file_path);

	void free(); // Function overloading to cover multiple type of objects

	void freeall(); // Frees everything in the PRL_handler

	void createText();

	void createTextbox();

	void freeUnusedAnimations();

	void freeUnusedSprites();

	// Updater, Collider, Displayer, Physic system

	static int getHandlerCount();

private:
	static int handlerCount;

public:
	std::vector <SDL_Renderer*> renderer;
	std::vector <SDL_Window*> window;
	std::vector <PRL_Animation*> animation;
	//std::vector <PRL_Sprite*> sprite;

// Threads/ networking, out screen update

//


};

#endif // PRL_HANDLER_H_INCLUDED
