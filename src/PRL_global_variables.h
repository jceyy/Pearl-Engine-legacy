#ifndef PRL_GLOBAL_VARIABLES_H_INCLUDED
#define PRL_GLOBAL_VARIABLES_H_INCLUDED

#include <SDL2/SDL.h>
#include "PRL_configuration.h"
#include "PRL_timer.h"
#include "PRL_camera.h"
#include "PRL_display.h"
#include "PRL_defines.h"
#include "PRL_handler.h"

extern SDL_Texture *targetTexture_GLOBAL[PRL_MAX_RENDERER_GLOBAL];
extern SDL_Renderer *renderer_GLOBAL[PRL_MAX_RENDERER_GLOBAL];
extern SDL_Window *window_GLOBAL[PRL_MAX_WINDOWS_GLOBAL];
extern PRL_Timer timer_GLOBAL;
extern PRL_Camera camera_GLOBAL[PRL_MAX_CAMERAS_GLOBAL];
extern PRL_Displayer displayer_GLOBAL[PRL_MAX_DISPLAYERS_GLOBAL];
extern PRL_Config config_GLOBAL;
extern PRL_Handler handler;

#endif // PRL_GLOBAL_VARIABLES_H_INCLUDED
