#include <iostream>
#include <stdio.h>
#include <SDL2/SDL.h>

#include "PRL.h"

/* Prototypes */

int PRL_TestZone();
int maingen(), mainbut(); // to disappear

using std::cout;
using std::cerr;
using std::endl;
using std::string;

int main(int argc, char *argv[])
{
	for (int i(0); i < argc; ++i)
	{
		cout << string("Argument ") + to_string(i) + ": " + argv[i] << endl;
	}
	cout << endl;

    int main_return(0);
    if (PRL_Init()!=0) /* Initialize all the things we need */
    {
        cerr << "Error: in main(), failed to initialize PEARL Engine properly, quiting the program" << endl;
        return PRL_ERROR;
    }

    //PRL_Logo();
    //PRL_LoadingScreen();
    main_return = PRL_TestZone();
    //PRL_Delay(500000);

    PRL_Quit();
    return main_return;
}


#define MAX_PARTICLES 2048

typedef struct Particle {
	int active;
	float x, y;
	float vx, vy;
	float ax, ay;
} Particle;

static struct {
	SDL_Window * handle;
	int width, height;
	SDL_Renderer * renderer;
	SDL_Texture * texture;
	Uint32 particle_count;
	Particle particle[MAX_PARTICLES];
	Uint32 last_update;
} Window;

static Uint32 xstate = 905309021;
static float xorshift()
{
	xstate ^= xstate << 13;
	xstate ^= xstate >> 17;
	xstate ^= xstate << 5;
	return (float)xstate / 4294967295.f;
}

static void update_particles(Uint32 ms)
{
	int i;
	float sec = (float)ms / 1000.f;

    for (i = 0; i < MAX_PARTICLES; i++) // particles update
    {
        if (Window.particle[i].active)
        {
            Window.particle[i].vx += Window.particle[i].ax * sec;
            Window.particle[i].vy += Window.particle[i].ay * sec;
            Window.particle[i].vx *= Window.particle[i].y;
            Window.particle[i].vy *= 0.9f;
            Window.particle[i].x += Window.particle[i].vx * sec;
            Window.particle[i].y += Window.particle[i].vy * sec;
            if (Window.particle[i].y < -0.05f) {
                Window.particle[i].active = 0;
                Window.particle_count--;
            }
        }
    }

    for (i = 0; Window.particle_count < MAX_PARTICLES && i < MAX_PARTICLES; i++) // particles initialization
    {
        if (!Window.particle[i].active) {
            Window.particle[i].vx = 0;
            Window.particle[i].vy = 0;
            Window.particle[i].ax = (xorshift() - .5f) * 40.f;
            Window.particle[i].ay = -(10.f + xorshift() * 5.f);
            Window.particle[i].x = .5f + (xorshift() - .5f) * .02f;
            Window.particle[i].y = 0.95f + (xorshift() - .5f) * .02f;
            Window.particle[i].active = 1;
            Window.particle_count++;
            ms--;
        }
    }
}

static void create_tt()
{
	SDL_Texture * tt = SDL_CreateTexture(Window.renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, Window.width / 2, Window.height / 2);
	if (Window.texture != NULL) {
		SDL_DestroyTexture(Window.texture);
	}
	SDL_SetTextureBlendMode(tt, SDL_BLENDMODE_BLEND);
	Window.texture = tt;
}

static void draw_particles()
{
	int i;
	SDL_Rect dst_rect;

    SDL_SetRenderTarget(Window.renderer, Window.texture);

    SDL_SetRenderDrawColor(Window.renderer, 0, 0, 0, 0);
    SDL_RenderClear(Window.renderer);

    SDL_SetRenderDrawColor(Window.renderer, 255, 255, 255, 255);
    for (i = 0; i < MAX_PARTICLES; i++) {
        if (Window.particle[i].active) {
            dst_rect.x = (int)((Window.width / 2) * Window.particle[i].x) - 4;
            dst_rect.y = (int)((Window.height / 2) * Window.particle[i].y) - 4;
            dst_rect.w = 8;
            dst_rect.h = 8;
            SDL_RenderFillRect(Window.renderer, &dst_rect);
        }
    }

    SDL_SetRenderTarget(Window.renderer, NULL);
}

static void draw_texture(Uint32 now)
{
	SDL_Rect dst_rect;

    SDL_SetRenderDrawColor(Window.renderer, 0, 0, 0, 255);
    SDL_RenderClear(Window.renderer);

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.w = Window.width / 2;
    dst_rect.h = Window.height / 2;

    SDL_SetRenderDrawColor(Window.renderer, 200, 0, 0, 255);
    SDL_RenderFillRect(Window.renderer, &dst_rect);

    SDL_SetTextureColorMod(Window.texture, 0, 255, 255);
    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &dst_rect);


    dst_rect.x = Window.width / 2;
    dst_rect.y = 0;

    SDL_SetRenderDrawColor(Window.renderer, 0, 200, 0, 255);
    SDL_RenderFillRect(Window.renderer, &dst_rect);

    SDL_SetTextureColorMod(Window.texture, 255, 0, 255);
    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &dst_rect);


    dst_rect.x = 0;
    dst_rect.y = Window.height / 2;

    SDL_SetRenderDrawColor(Window.renderer, 0, 0, 200, 255);
    SDL_RenderFillRect(Window.renderer, &dst_rect);

    SDL_SetTextureColorMod(Window.texture, 255, 255, 0);
    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &dst_rect);


    dst_rect.x = Window.width / 2;
    dst_rect.y = Window.height / 2;

    SDL_SetRenderDrawColor(Window.renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(Window.renderer, &dst_rect);

    SDL_SetTextureColorMod(Window.texture, now / 140 % 256, now / 150 % 256, now / 160 % 256);
    SDL_RenderCopy(Window.renderer, Window.texture, NULL, &dst_rect);

    SDL_RenderPresent(Window.renderer);
}

static int loop()
{
	SDL_Event e;
	Uint32 now;

    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return 0;
        }
        else if (e.type == SDL_KEYUP)
        {
            Uint32 sym = e.key.keysym.sym;
            if (sym == SDLK_ESCAPE)
            {
                return 0;
            }
            else if (sym == SDLK_f)
            {
                if (SDL_GetWindowFlags(Window.handle) & SDL_WINDOW_FULLSCREEN)
                {
                    SDL_SetWindowFullscreen(Window.handle, SDL_FALSE);
                } else
                {
                    SDL_SetWindowFullscreen(Window.handle, SDL_WINDOW_FULLSCREEN_DESKTOP);
                }
            }
        }
        else if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                SDL_GetWindowSize(Window.handle, &Window.width, &Window.height);
                create_tt();
            }
        }
    }

    now = SDL_GetTicks();
    update_particles(now - Window.last_update);
    Window.last_update = now;

    draw_particles();
    draw_texture(now);

    return 1;
}

int maingen()//int argc, char * argv[])
{
	int i;
	char title[256] = {0};
	SDL_RendererInfo info;

    Window.texture = NULL;
    Window.last_update = SDL_GetTicks();
    Window.particle_count = 0;
    for (i = 0; i < MAX_PARTICLES; i++)
    {
        Window.particle[i].active = 0;
    }

    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
    Window.handle = SDL_CreateWindow("Loading...",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 500, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
    Window.renderer = SDL_CreateRenderer(Window.handle, -1, 0);

    SDL_GetRendererInfo(Window.renderer, &info);
    SDL_strlcat(title, info.name, sizeof(title));
    SDL_Log("Renderer %s started.", info.name);
    if (!(info.flags & SDL_RENDERER_TARGETTEXTURE)) {
        SDL_Log(" Renderer has no target texture support!");
        SDL_strlcat(title, " (no target textures!)", sizeof(title));
    }
    SDL_strlcat(title, ": Target Texture Color Mod", sizeof(title));
    SDL_SetWindowTitle(Window.handle, title);

    SDL_GetWindowSize(Window.handle, &Window.width, &Window.height);
    create_tt();

    SDL_SetRenderDrawBlendMode(Window.renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(Window.renderer, 0, 0, 0, 0);
    SDL_RenderClear(Window.renderer);
    SDL_RenderPresent(Window.renderer);

    while (loop()) {
        SDL_Delay(3);
    }

    SDL_DestroyTexture(Window.texture);
    SDL_DestroyRenderer(Window.renderer);
    SDL_DestroyWindow(Window.handle);
    //SDL_Quit();

    return 0;
}


