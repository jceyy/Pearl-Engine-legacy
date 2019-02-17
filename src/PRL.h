#ifndef PEARL_H_INCLUDED
#define PEARL_H_INCLUDED

#ifdef _WIN32
   //define something for Windows (32-bit and 64-bit, this part is common)
   #ifdef _WIN64
      //define something for Windows (64-bit only)
   #else
      //define something for Windows (32-bit only)
   #endif
#elif __APPLE__
    #include "TargetConditionals.h"
    #if TARGET_IPHONE_SIMULATOR
         // iOS Simulator
    #elif TARGET_OS_IPHONE
        // iOS device
    #elif TARGET_OS_MAC
        // Other kinds of Mac OS
    #else
    #   error "Unknown Apple platform"
    #endif
#elif __linux__
    // linux
#elif __unix__ // all unices not caught above
    // Unix
#elif defined(_POSIX_VERSION)
    // POSIX
#else
#   error "Unknown compiler"
#endif // _WIN32

/*
#ifdef _WIN32 || WIN32 || _WIN64 || WIN64
//#define WINDOWS
//#include <windows.h>
#endif // _WIN32 || WIN32 || _WIN64 || WIN64
*/
#ifdef __unix__
#define LINUX
#include <pthread.h>
#endif // __unix__
/*
#include <stdio.h>
#include <stdlib.h>
//#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <chrono>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>*/
//#include <SDL2/SDL_mixer.h>


#include "PRL_animation.h"
#include "PRL_camera.h"
#include "PRL_collision.h"
#include "PRL_configuration.h"
#include "PRL_defines.h"
#include "PRL_display.h"
#include "PRL_game_functions.h"
#include "PRL_global_variables.h"
#include "PRL_input.h"
#include "PRL_math.h"
#include "PRL_screenshots.h"
#include "PRL_HUD.h"
#include "PRL_timer.h"
#include "PRL_tool_programs.h"
//#include "tools.h"


#if PRL_COMPILE_ANIM_TESTER == 1
#include "PRL_tool_programs.h"
#endif // PRL_COMPILE_ANIM_TESTER

#endif // PEARL_H_INCLUDED
