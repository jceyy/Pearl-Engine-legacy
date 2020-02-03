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


#include "PRL_animation.h"
#include "PRL_camera.h"
#include "PRL_collision.h"
#include "PRL_configuration.h"
#include "PRL_defines.h"
#include "PRL_display.h"
#include "PRL_input.h"
#include "PRL_math.h"
#include "PRL_HUD.h"
#include "PRL_text.h"
#include "PRL_time.h"
#include "PRL_handler.h"


#endif // PEARL_H_INCLUDED
