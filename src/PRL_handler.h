#ifndef PRL_HANDLER_H_INCLUDED
#define PRL_HANDLER_H_INCLUDED

class PRL_Handler
{
public:

	PRL_Handler();
	~PRL_Handler();

	PRL_Config config;
	

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

	std::vector <SDL_Renderer*> renderer;
	std::vector <SDL_Window*> window;

// Threads/ networking, out screen update

//


};

#endif // PRL_HANDLER_H_INCLUDED
