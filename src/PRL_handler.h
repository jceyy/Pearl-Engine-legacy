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
		friend PRL_Handler;

	public:
		std::vector <SDL_Renderer*> renderer;
		std::vector <SDL_Window*> window;
	private:
		std::vector <PRL_Animation*> animation;
		std::vector <PRL_Animated*> animated;

		PRL_Displayer hudDisplayer;
	};
	_display display;

	struct _time
	{
		friend PRL_Handler;
		friend PRL_Animated;

	public:
		PRL_Stopwatch stopwatch; // automatically started
		long long getTimeUpdated() const;
	private:
		long long timeUpdated;
	};
	_time time;

	struct collision
	{
	};

	struct network
	{
	};

	struct physics
	{
	};


	PRL_Animation* loadAnimation(const std::string& file_path);

	//PRL_Image* loadImage(const std::string& file_path);

	PRL_Animated* createAnimated();
    PRL_Animated* createAnimated(PRL_Animation* anim);


    //PRL_Sprite* createSprite();

	void free(); // Function overloading to cover multiple type of objects

	void freeall(); // Frees everything in the PRL_handler

	void createText();

	void createTextbox();

	void freeUnusedAnimations();

	void freeUnusedImages();

	// Updater, Collider, Displayer, Physic system

	static int getHandlerCount();

private:
	static int handlerCount;

	//std::vector <PRL_Sprite*> sprite;

	// Threads/ networking, out screen update

//


};

#endif // PRL_HANDLER_H_INCLUDED
