#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

/// PROGRAMMABLE DEFINES ///
// changeable by programmers!
#define PROGRAM_NAME "PEARL Engine (alpha)" //"PEARL Engine 00.49.537a"

#define DEBUGGING 1 // toggle the console / output.txt std::cout stream destination

#define PRL_COMPILE_ANIM_TESTER 0 // whether or not to compile the animation tester
#define PRL_USE_WARNINGS 1 // very useful when programming
#define PRL_AUTO_WRITE_ERRORS 1 // toggle auto export of the errors into error.txt

#define PRL_MULTITHREADING 0 // allow massive multi threading usage
#define PRL_ALLOW_FLOAT_FPS 1 // allow the usage of floating point FPS, like 24.97 for instance

// input
#define PRL_USE_SCANCODES 1 // if 0 use key-codes instead of scan-codes for the keyboard


// display
#define PRL_DISPLAYER_MAX_OBJECTS 300
#define PRL_DISPLAYABLE_ALLOW_GETTEXTURE 0

// Animation
#define PRL_ANIMATION_KEEP_ORIGINAL_RES 1 // keep or not the original resolution in animations


/// RETURN ///
#define __CERR_REF__ PRL_TimeStamp() << " " << __FILENAME__ << ", line " << __LINE__ << " "
#define __FILENAME__ (__builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__)

// not changeable by programmers
#define PRL_WARNING (-1)
#define PRL_ERROR (-2)
#define PRL_RETURN_QUIT_TO_DESKTOP (-3)
#define PRL_RETURN_ALL_RIGHT 0
#define PRL_RETURN_RESTART_PROGRAM 1970

/// CORE ///
#define PRL_FILE_SEPARATION_CHARACTER ':' // the character used to split informations in a file like .anim or .hud
#define PRL_MIN_TRIG_PRECISION 2 // from 0 to 5, 5 being the best
/// PROGRAMMATION ///
#define ERROR (-1)

/// CONFIGURATION ///
#define PRL_CONFIG_WINDOW_ALLWAYS_ON_THE_TOP 0
#define PRL_CONFIG_CAPTURE_MOUSE 0


/// LIMITS ///
#define PRL_MAX_RENDERER_GLOBAL 5
#define PRL_MAX_WINDOWS_GLOBAL 5
#define PRL_MAX_CAMERAS_GLOBAL 4
#define PRL_MAX_DISPLAYERS_GLOBAL 10
#define PRL_MAX_FPS 500.0

// changeable


/// FILE ///
#define WRITE_AT_THE_END__FILE_ReplaceLine 0



/// STR AND NB MANIPULATION ///
#define MAX_X_COORD_NUMBERS 6
#define MAX_Y_COORD_NUMBERS 6


//** ************************************************** **
// old other file
#define PRL_MAJOR_VERSION 0
#define PRL_MINOR_VERSION 95
#define PRL_PATCHLEVEL 179

/*!
\version {0.95.179}
*/



/// SETTINGS ///
#define PRL_USE_TARGET_TEXTURE 0
#define PRL_ENABLE_SLOWMO 0


/// FILE ///

#define FILE_MAX_LINE_LENGTH 500
#define FILE_MAX_LINES 1200
#define FILE_MAX_FILE_NAME_LENGTH 250
#define FILE_MAX_FILE_ADDRESS_LENGTH 500
#define PRL_CONFIG_FILE_PATH "config/settings.txt"
#define PRL_CONFIG_FOLDER "config"

#endif // DEFINES_H_INCLUDED
