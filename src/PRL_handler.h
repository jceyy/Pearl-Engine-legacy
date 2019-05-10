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
		std::vector <PRL_Image*> image;
		std::vector <PRL_Sprite*> sprite;

		std::vector <PRL_Displayer> displayer;
		PRL_Displayer hudDisplayer;
	};
	_display display;

	struct _time
	{
		friend class PRL_Handler;
		friend class PRL_Animated;

	public:
		PRL_Stopwatch stopwatch; // automatically started
		long long getTimeUpdated() const;
	private:
		long long timeUpdated;

		void update();
	};
	_time time;

	struct _collision
	{
		friend class PRL_Collider;

	public:
		std::vector <PRL_Collider> collider;
	};
	_collision collision;

	struct network
	{
	};

	struct physics
	{
	};


	PRL_Animation* loadAnimation(const std::string& file_path);
	PRL_Animation* loadAnimation(const std::string& file_path, SDL_Renderer* render);

	PRL_Image* loadImage(const std::string& file_path);
	PRL_Image* loadImage(const std::string& file_path, SDL_Renderer* render);

	//
    PRL_Animated* createAnimated(PRL_Animation* anim, int plan = PRL_PLAN_MAX);
	PRL_Sprite* createSprite(PRL_Image* image, int plan = PRL_PLAN_MAX);

    void update();

	void free(); // Function overloading to cover multiple type of objects

	void freeall(); // Frees everything in the PRL_handler

	void createText();

	void createTextbox();

	void freeUnusedAnimations();

	void freeUnusedImages();
	// Current FPS function?

	// Updater, Collider, Displayer, Physic system
    // update animations here

	static int getHandlerCount();

private:
	static int handlerCount;


	// Threads/ networking, out screen update

//


};

extern PRL_Handler handler;

#endif // PRL_HANDLER_H_INCLUDED
