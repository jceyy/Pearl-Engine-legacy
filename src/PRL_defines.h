#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

/// PROGRAMMABLE DEFINES ///
// changeable by programmers!
#define PROGRAM_NAME "Pearl Engine (alpha)"

#define DEBUGGING 1 // toggle the console / output.txt std::cout stream destination

#define PRL_USE_WARNINGS 1 // very useful when programming
#define PRL_AUTO_WRITE_ERRORS 1 // toggle auto export of the errors into error.txt

#define PRL_MULTITHREADING 0 // allow massive multi threading usage
#define PRL_ALLOW_FLOAT_FPS 1 // allow the usage of floating point FPS, like 24.97 for instance

// input
#define PRL_USE_SCANCODES 1 // if 0 use key-codes instead of scan-codes for the keyboard

#define PRL_MAX_FPS 200

/// RETURN ///
#define __CERR_REF__ PRL_TimeStamp() << " " << __FILENAME__ << ", line " << __LINE__ << " "
#define __FILENAME__ (__builtin_strrchr(__FILE__, '\\') ? __builtin_strrchr(__FILE__, '\\') + 1 : __FILE__)

// not changeable by programmers
#define PRL_WARNING (-1)
#define PRL_ERROR (-2)
#define PRL_RETURN_QUIT_TO_DESKTOP (-3)
#define PRL_RETURN_ALL_RIGHT 0
#define PRL_RETURN_RESTART_PROGRAM 1970


#define PRL_MAJOR_VERSION 0
#define PRL_MINOR_VERSION 98
#define PRL_PATCHLEVEL 3
/*!
\version {0.98.3}
*/



/// FILE ///
#define PRL_CONFIG_FILE_PATH "config/settings.txt"
#define PRL_CONFIG_FOLDER "config"

#endif // DEFINES_H_INCLUDED
