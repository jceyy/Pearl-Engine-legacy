#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <cstring>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "PRL_timer.h"
#include "PRL_global_variables.h"

#include "PRL_configuration.h"
#include "PRL_math.h"
#include "PRL_tool_programs.h"

#include "PRL_input.h"
#include "PRL_defines.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;


// Global variables definition
SDL_Texture *targetTexture_GLOBAL[PRL_MAX_RENDERER_GLOBAL];
SDL_Renderer *renderer_GLOBAL[PRL_MAX_RENDERER_GLOBAL];
SDL_Window *window_GLOBAL[PRL_MAX_WINDOWS_GLOBAL];
PRL_Config config_GLOBAL;
PRL_Timer timer_GLOBAL;
PRL_Camera camera_GLOBAL[PRL_MAX_CAMERAS_GLOBAL];
PRL_Displayer displayer_GLOBAL[PRL_MAX_DISPLAYERS_GLOBAL];

// convert a bool into a char string
inline const char* const btoc(bool b)
{
    if (b == true)
        return "true";
    else
        return "false";
}

// convert an int into a char string
inline const char* const ito3scale(int n)
{
    if (n <= 0)
        return "low";
    else if (n == 1)
        return "normal";
    else
        return "high";
}

// convert an int into a char string
inline const char* const ito5scale(int n)
{
    if (n <= 0)
        return "verylow";
    else if (n == 1)
        return "low";
    else if (n == 2)
        return "normal";
    else if (n == 3)
        return "high";
    else if (n == 4)
        return "veryhigh";
    else
        return "ultra";

}

// returns a time stamp
std::string PRL_TimeStamp()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", timeinfo);
    std::string str(buffer);

    return str;
}

void PRL_SetError(std::string const& msg)
{
    PRL_ErrorHandling::setError(msg);
}

std::string PRL_GetError()
{
    return PRL_ErrorHandling::getError();
}

int PRL_ErrorCount()
{
    return PRL_ErrorHandling::getErrorCount();
}

std::string PRL_Config :: getCompilationTime()
{
    return std::string(__DATE__) + std::string(", ") + std::string(__TIME__);
}

void printLaunchDiag() // print the launch diagnosis
{
    int i(0), joy_num = SDL_NumJoysticks();
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer_GLOBAL[0], &info);
    SDL_DisplayMode current;
    const SDL_version *linkv = IMG_Linked_Version();

    cout << "Platform detection: " << SDL_GetPlatform() << endl << endl;

    cout << "--- Program compilation properties ---" << endl << endl;
    cout << "Program compiled on " << config_GLOBAL.getCompilationTime() << " using:" << endl;
    cout << "SDL version: " << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << endl;
    cout << "SDL_TTF version: " << TTF_MAJOR_VERSION << "." << TTF_MINOR_VERSION << "." << TTF_PATCHLEVEL << endl;
    cout << "SDL_IMG version: " << (int) linkv->major << "." << (int) linkv->minor << "." << (int) linkv->patch << endl;
    cout << "PEARL ENGINE version: " << PRL_MAJOR_VERSION << "." << PRL_MINOR_VERSION << "." << PRL_PATCHLEVEL << endl;
    cout << endl << endl;

    cout << "--- System settings ---" << endl << endl;
    cout << "Logical CPU cores: " << SDL_GetCPUCount() << endl;
    cout << "L1 cache line size: " << SDL_GetCPUCacheLineSize() << " bytes" << endl;
    cout << "System RAM: " << SDL_GetSystemRAM() << " MB" << endl;
    cout << endl << endl;

    cout << "--- Devices ---" << endl << endl;
    cout << "Available video displays: " << SDL_GetNumVideoDisplays() << endl;

    for(i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        if (SDL_GetCurrentDisplayMode(i, &current)!=0)
        {
            #if PRL_USE_WARNINGS == 1
            cout << __CERR_REF__ << "Could not get display mode for video display #" << i << ": " << SDL_GetError() << endl;
            #endif // PRL_USE_WARNINGS
        }
        else
        {
            cout << "Display #" << i+1 << " - Name: " << SDL_GetDisplayName(i) <<" - current display mode is " << current.w << " x ";
            cout << current.h << " @ " << current.refresh_rate << " Hz" << endl;
        }
    }

    SDL_Joystick *joystick[PRL_MAX_JOYSTICKS];
    cout << endl;
    cout << "Available controllers: " << joy_num << endl;
    if (joy_num > 0)
    {
        for (i=0; i < joy_num; i++)
        {
            joystick[i] = SDL_JoystickOpen(i);
            cout << "Controller #" << i+1 << " - ";
            cout << "Name: " << SDL_JoystickName(joystick[i]);
            cout << " - " << SDL_JoystickNumButtons(joystick[i]) << " buttons, " << SDL_JoystickNumAxes(joystick[i]) << " axis, " ;
            cout << SDL_JoystickNumHats(joystick[i]) << " hats, " << SDL_JoystickNumBalls(joystick[i]) << " balls." << endl;
            SDL_JoystickClose(joystick[i]);
        }
    }
    else
        cout << "No controller plugged in" << endl;

    cout << endl << endl;
    cout << "--- Loaded Settings ---" << endl << endl;
    cout << "Display resolution: " << config_GLOBAL.resolution.x << "x" << config_GLOBAL.resolution.y << endl;
    cout << "Renderer resolution: " << config_GLOBAL.renderResolution.x << "x" << config_GLOBAL.renderResolution.y << endl;
    cout << "Video driver: " << SDL_GetCurrentVideoDriver() << endl;
    cout << "Renderer name: " << info.name << endl;
    cout << "Renderer max. texture size: " << info.max_texture_width << " x " << info.max_texture_height << endl;
    cout << endl;

    cout << "Using high-DPI: " << btoc(config_GLOBAL.highDpi) << endl;
    cout << "Using vSync: " << btoc(config_GLOBAL.vsync) << endl;
    cout << "Using full-screen: " << btoc(config_GLOBAL.fullscreen) << endl;
    cout << "Using CPU-relieve: " << btoc(config_GLOBAL.cpuRelieve) << endl;
    cout << "Resizable window: " << btoc(config_GLOBAL.resizable) << endl;
    cout << "Borderless window: " << btoc(config_GLOBAL.borderless) << endl;
    cout << endl;

    cout << "Texture filtering: " << ito3scale(stoi(config_GLOBAL.textureFiltering)) << endl;
    cout << "Process priority: " << ito3scale((int)config_GLOBAL.processPriority) << endl;
    cout << "Trigonometric precision (the allowed min. being \"" << ito5scale(PRL_MIN_TRIG_PRECISION) << "\"): ";
    cout << ito5scale(config_GLOBAL.trigoPrecision) << endl;
    cout << endl;

    cout << "Frames per second: " << config_GLOBAL.fps << endl;
    cout << "Brightness: " << config_GLOBAL.brightness << endl;
    cout << "Music volume: " << config_GLOBAL.musicVol << endl;
    cout << "Effects volume: " << config_GLOBAL.effectsVol << endl;
    cout << "Voice volume: " << config_GLOBAL.voiceVol << endl;
    cout << "General volume: " << config_GLOBAL.generalVol << endl;
    cout << "Mouse sensitivity: " << config_GLOBAL.mouseSensitivity << endl;
    cout << endl;

    cout << "Language: " << config_GLOBAL.language << endl;
    cout << endl;

    /// controllers plugged in?
}

int PRL_CreateWindowAndRenderer()
{
    // SYSTEM
    if (SDL_SetThreadPriority(config_GLOBAL.processPriority) != 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to change the process priority: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to change the process priority: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    // WINDOW
    Uint32 flagw1, flagw2, flagw3, flagw4, flagw5, flagw6;
    flagw1 = flagw2 = flagw3 = flagw4 = flagw5 = flagw6 = SDL_WINDOW_SHOWN;

    if (config_GLOBAL.fullscreen == true)
        flagw1 = SDL_WINDOW_FULLSCREEN;

    if (config_GLOBAL.highDpi == true)
        flagw2 = SDL_WINDOW_ALLOW_HIGHDPI;

    if (config_GLOBAL.resizable == true)
        flagw3 = SDL_WINDOW_RESIZABLE;

    if (config_GLOBAL.borderless == true)
        flagw4 = SDL_WINDOW_BORDERLESS;

    if (PRL_CONFIG_WINDOW_ALLWAYS_ON_THE_TOP)
        flagw5 = SDL_WINDOW_ALWAYS_ON_TOP;

    if (PRL_CONFIG_CAPTURE_MOUSE)
        flagw6 = SDL_WINDOW_MOUSE_CAPTURE;

    Uint32 flagsw = flagw1|flagw2|flagw3|flagw4|flagw5|flagw6;

    window_GLOBAL[0] = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             config_GLOBAL.resolution.x, config_GLOBAL.resolution.y, flagsw);

    if (window_GLOBAL[0] == NULL)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to create the window: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to create the window: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }


    // RENDERER
    Uint32 flagr1(0), flagr2(0);

    if(config_GLOBAL.vsync == true)
        flagr1 = SDL_RENDERER_PRESENTVSYNC;

    if (config_GLOBAL.driver_name != "software")
        flagr2 = SDL_RENDERER_ACCELERATED;
    else
        flagr2 = SDL_RENDERER_SOFTWARE;

    Uint32 flags_rend = flagr1 | flagr2;

    renderer_GLOBAL[0] = SDL_CreateRenderer(window_GLOBAL[0], config_GLOBAL.driver_index, flags_rend);
    if (renderer_GLOBAL[0] == NULL)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to create the renderer: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to create the renderer: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if (SDL_RenderSetLogicalSize(renderer_GLOBAL[0], config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y) != 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to set the logical size: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to set the renderer's logical size: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, config_GLOBAL.textureFiltering.c_str()) != SDL_TRUE)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to set the texture filtering: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to set texture filtering: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1"); //(e.g. "0", "1", or "direct3d", "opengl", etc.)

    return 0;
}

void PRL_DestroyWindowAndRenderer(SDL_Window *window, SDL_Renderer *renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

int PRL_Init()
{
    srand(time(NULL));

    // redirect output and error streams
    #if DEBUGGING == 0
    freopen("output.txt", "w", stdout);
    freopen("error.txt", "w", stderr);
    #endif // DEBUGGING

    cout << PRL_TimeStamp() << " Initialization..." << endl;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_Init, could not initialize SDL: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS

        PRL_SetError(string("Failed to load the SDL: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    int imgflags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF|IMG_INIT_WEBP;

    if (IMG_Init(imgflags) != imgflags)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_Init, could not initialize SDL_Image: " << IMG_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS

        PRL_SetError(string("Failed to load SDL_Image: ") + string(IMG_GetError()));
        return PRL_ERROR;
    }

    if (TTF_Init() == -1)// TTF Initialization
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_Init, could not initialize SDL_TTF: " << TTF_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS

        PRL_SetError(string("Failed to load SDL_TTF: ") + string(TTF_GetError()));
        return PRL_ERROR;
    }

    if (SDL_JoystickEventState(SDL_ENABLE) < 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_Init, could not enable controllers: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS

        PRL_SetError(string("Failed to enable controllers: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if (config_GLOBAL.loadSettings() != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_Init, could not load the settings: " << PRL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    if (config_GLOBAL.loadLng() != 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_Init, could not load the language file: " << PRL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        return PRL_ERROR;
    }

    if(PRL_CreateWindowAndRenderer() != 0) // Creation of window and renderer
    {
        return PRL_ERROR;
    }

    #if PRL_USE_TARGET_TEXTURE == 1
    targetTexture_GLOBAL = SDL_CreateTexture(renderer_GLOBAL, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y);
    PRL_SetTextureColorMod(targetTexture_GLOBAL, COLMOD_NORMAL);
    SDL_SetRenderTarget(renderer_GLOBAL, targetTexture_GLOBAL);
    #endif

    if (SDL_SetWindowBrightness(window_GLOBAL[0], config_GLOBAL.brightness) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_Init, unable to set the brightness: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    SDL_DisableScreenSaver();

    printLaunchDiag(); // Print infos on startup

    /// SET UP ALL GLOBAL VARIABLES!!!!!!!!!!!!!!!



    for (int i(0); i < PRL_MAX_CAMERAS_GLOBAL; i++)
    {
        camera_GLOBAL[i].setCameraNumber(i);
        //camera_GLOBAL[i].setDimension({0, 0, config_GLOBAL.renderResolution.x, config_GLOBAL.renderResolution.y});
        camera_GLOBAL[i].setZoom(1.0);
    }

    #if PRL_COMPILE_ANIM_TESTER == 1
    if (PRL_Tool_AnimationTester() != 0)
    {
        PRL_Quit();
        exit(PRL_ERROR);
    }
    else
    {
        PRL_Quit();
        exit(0);
    }
    #endif // PRL_COMPILE_ANIM_TESTER
    return 0;
}

void PRL_Quit()
{
    cout << PRL_TimeStamp() << " Quiting..." << endl;
    SDL_EnableScreenSaver();

    for (int i(0); i < PRL_MAX_RENDERER_GLOBAL; i++)
    {
        if (renderer_GLOBAL[i] != NULL)
            SDL_DestroyRenderer(renderer_GLOBAL[i]);
    }

    for (int i(0); i < PRL_MAX_WINDOWS_GLOBAL; i++)
    {
        if (window_GLOBAL[i] != NULL)
        {
            SDL_SetWindowBrightness(window_GLOBAL[0], (float) 1.0); // reset brightness
            SDL_DestroyWindow(window_GLOBAL[i]);
        }
    }

    #if PRL_USE_TARGET_TEXTURE==1
    for (int i(0); i < PRL_MAX_RENDERER_GLOBAL; i++)
        if (targetTexture_GLOBAL[i] != NULL)
            SDL_DestroyTexture(targetTexture_GLOBAL[i]);
    #endif // PRL_USE_TARGET_TEXTURE

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void PRL_Logo()
{
}

void PRL_LoadingScreen() // whether or not to display the loading percentage.
{
    TTF_Font *font = TTF_OpenFont("data/fonts/cooper.TTF", 85);
    SDL_Color colorWhite={255, 255, 255};
    SDL_Surface *surface=TTF_RenderText_Blended(font, "Loading...", colorWhite);
    SDL_Texture *texture=SDL_CreateTextureFromSurface(renderer_GLOBAL[0], surface);
    SDL_Rect pos={(config_GLOBAL.renderResolution.x-surface->w)/2, (config_GLOBAL.renderResolution.y-surface->h)/2, surface->w, surface->h};

    SDL_RenderClear(renderer_GLOBAL[0]);
    SDL_RenderCopy(renderer_GLOBAL[0], texture, NULL, &pos);
    SDL_RenderPresent(renderer_GLOBAL[0]);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TTF_CloseFont(font);
}

/* ************ PRL_ErrorHandling ************* */

std::string PRL_ErrorHandling :: last_err = std::string("");
int PRL_ErrorHandling :: err_count = 0;

std::string PRL_ErrorHandling :: getError()
{
    return last_err;
}

void PRL_ErrorHandling :: setError(std::string const& msg)
{
    err_count++;
    last_err = PRL_TimeStamp() + " " + msg;
}

int PRL_ErrorHandling :: getErrorCount()
{
    return err_count;
}

/* ************ PRL_Config ************* */

PRL_Config :: PRL_Config()
{
    loadDefault();
}

PRL_Config :: ~PRL_Config()
{
    mtext.clear();
}

int PRL_Config :: getSettingsNumber() const
{
    return settingsNumber;
}

void PRL_Config :: loadDefault()
{
    settingsNumber = 20;

    _highDpi = false;
    _fullscreen = false;
    _vsync = false;
    _cpuRelieve = true;
    _resizable = false;
    _borderless = false;

    _textureFiltering_int = 0;
    _driver_index = -1;
    _processPriority = SDL_THREAD_PRIORITY_NORMAL;
    _trigoPrecision = 2;

    _fps = 30.0;
    _brightness = 1.0;
    _musicVol = 1.0;
    _effectsVol = 1.0;
    _voiceVol = 1.0;
    _generalVol = 1.0;
    _mouseSensitivity = 1.0;

    _resolution = {720, 360};
    _renderResolution = {720, 360};

    _textureFiltering = "0";
    _language = "english";
    _driver_name = "default";

    _cos_default = cos3;
    _sin_default = sin3;

    makePublic();
}

void PRL_Config :: makePublic()
{
    highDpi = _highDpi;
    fullscreen = _fullscreen;
    vsync = _vsync;
    cpuRelieve = _cpuRelieve;
    resizable = _resizable;
    borderless = _borderless;

    textureFiltering_int = _textureFiltering_int;
    driver_index = _driver_index;
    processPriority = _processPriority;
    trigoPrecision = _trigoPrecision;

    fps = _fps;
    brightness = _brightness;
    musicVol = _musicVol;
    effectsVol = _effectsVol;
    voiceVol = _voiceVol;
    generalVol = _generalVol;
    mouseSensitivity = _mouseSensitivity;

    resolution = _resolution;
    renderResolution = _renderResolution;

    textureFiltering = _textureFiltering;
    language = _language;
    driver_name = _driver_name;

    cos_default = _cos_default;
    sin_default = _sin_default;
}

int PRL_Config :: extractSetting(std::string const& fullLine, std::string& setting, bool keep_spaces) const
{
    std::string eq("=");
    size_t pos(0); pos = fullLine.find(eq);

    if (pos == 0 || fullLine.size() < 2) // min 3 characters
    {
        PRL_SetError("The provided line hasn't the right format (type = value)");
        return PRL_ERROR;
    }

    if (pos != string::npos && pos != 0)
    {
        for (size_t i(pos + 1); i < fullLine.size() - 1; i++)
        {
            if (keep_spaces || fullLine[i] != ' ')
            {
                setting = fullLine.substr(i, string::npos);

                for (size_t j(0); j < setting.size() - 1; j++) // remove spaces or \n
                {
                    if ((!keep_spaces && setting[j] == ' ') || setting[j] == '\n')
                    {
                        setting.erase(j, 1);
                        --j;
                    }
                }
                break;
            }
        }
    }
    return 0;
}

int PRL_Config :: extractBool(std::string const& str, bool& b) const
{
    if (str == "true")
        b = true;
    else if (str == "false")
        b = false;
    else
    {
        PRL_SetError("Keywords \"true\" or \"false\" were not properly spelled");
        return PRL_ERROR;
    }
    return 0;
}

int PRL_Config :: extractFloat(std::string const& str, float& f) const
{
    try
    {
        f = std::stof(str);
    }
    catch (std::invalid_argument)
    {
        PRL_SetError("Couldn't convert the provided string into a float");
        return PRL_ERROR;
    }
    return 0;
}

int PRL_Config :: extractPoint(std::string const& str, PRL_Point& p) const
{
    size_t i(0);
    std::string temp1, temp2;

    for (; i < str.size(); i++)
    {
        if (str[i] == 'x')
        {
            break;
        }
        else if (str[i] != ' ' && str[i] != '\n')
        {
            temp1 += str[i];
        }
    }
    i++; // skip the 'x'

    for (; i < str.size(); i++)
    {
        if (str[i] == 'x')
        {
            break;
        }
        else if (str[i] != ' ' && str[i] != '\n')
        {
            temp2 += str[i];
        }
    }

    try
    {
        p.x = std::stoi(temp1);
        p.y = std::stoi(temp2);
    }
    catch (std::invalid_argument)
    {
        PRL_SetError("Couldn't convert the provided string into a PRL_Point");
        return PRL_ERROR;
    }
    return 0;
}

void checkBounds(float min, float max, float& var)
{
    if (var < min) var = min;
    else if (var > max) var = max;
}

int PRL_Config :: loadSettings()
{
    loadDefault(); // load the default settings first

    string fullLine(""), temp("");
    ifstream file(PRL_CONFIG_FILE_PATH);

    if (!file.is_open())
    {
        temp = "Unable to open the configuration file (";
        temp += PRL_CONFIG_FILE_PATH;
        temp += ")";
        PRL_SetError(temp);
        return PRL_ERROR;
    }

    while (getline(file, fullLine)) // read a single line
    {
        temp.clear();
        extractSetting(fullLine, temp); /// AJOUTER LES BORNEEEEEEEEEEEEEEES!!!!!!!!!!!!!!!!!!!

        // Booleans
        if (fullLine.find("high-dpi") != string::npos)
        {
           extractBool(temp, _highDpi);
        }
        else if (fullLine.find("vsync") != string::npos)
        {
            extractBool(temp, _vsync);
        }
        else if (fullLine.find("fullscreen") != string::npos)
        {
            extractBool(temp, _fullscreen);
        }
        else if (fullLine.find("cpu-relieve") != string::npos)
        {
            extractBool(temp, _cpuRelieve);
        }
        else if (fullLine.find("resizable") != string::npos)
        {
            extractBool(temp, _resizable);
        }
        else if (fullLine.find("borderless") != string::npos)
        {
            extractBool(temp, _borderless);
        }

        // Integers
        else if (fullLine.find("priority") != string::npos)
        {
            if (temp == "0" || temp == "low")
                _processPriority = SDL_THREAD_PRIORITY_LOW;
            else if (temp == "1" || temp == "normal")
                _processPriority = SDL_THREAD_PRIORITY_NORMAL;
            else if (temp == "2" || temp == "high")
                _processPriority = SDL_THREAD_PRIORITY_HIGH;
        }
        else if (fullLine.find("trig-precision") != string::npos)
        {
            if (temp == "0" || temp == "verylow")
                _trigoPrecision = 0;

            else if (temp == "1" || temp == "low")
                _trigoPrecision = 1;

            else if (temp == "2" || temp == "normal")
                _trigoPrecision = 2;

            else if (temp == "3" || temp == "high")
                _trigoPrecision = 3;

            else if (temp == "4" || temp == "veryhigh")
                _trigoPrecision = 4;

            else if (temp == "5" || temp == "ultra")
                _trigoPrecision = 5;


            if (_trigoPrecision < PRL_MIN_TRIG_PRECISION) _trigoPrecision = PRL_MIN_TRIG_PRECISION;
            if (_trigoPrecision > 5) _trigoPrecision = 5; // just in the case where the define would have been badly changed

            switch (_trigoPrecision)
            {
            case 0:
                _sin_default = sin1;
                _cos_default = cos1;
                break;

            case 1:
                _sin_default = sin2;
                _cos_default = cos2;
                break;

            case 2:
                _sin_default = sin3;
                _cos_default = cos3;
                break;

            case 3:
                _sin_default = sin4;
                _cos_default = cos4;
                break;

            case 4:
                _sin_default = sin5;
                _cos_default = cos5;
                break;

            case 5:
                _sin_default = sin6;
                _cos_default = cos6;
                break;
            }
        }

        // Floats
        else if (fullLine.find("fps") != string::npos)
        {
            extractFloat(temp, _fps);
            if (_fps > PRL_MAX_FPS) _fps = PRL_MAX_FPS;
        }
        else if (fullLine.find("brightness") != string::npos)
        {
            extractFloat(temp, _brightness);
            checkBounds(0.0, 1.0, _brightness);
        }
        else if (fullLine.find("music-vol") != string::npos)
        {
            extractFloat(temp, _musicVol);
            checkBounds(0.0, 1.0, _musicVol);
        }
        else if (fullLine.find("effects-vol") != string::npos)
        {
            extractFloat(temp, _effectsVol);
            checkBounds(0.0, 1.0, _effectsVol);
        }
        else if (fullLine.find("voice-vol") != string::npos)
        {
            extractFloat(temp, _voiceVol);
            checkBounds(0.0, 1.0, _voiceVol);
        }
        else if (fullLine.find("general-vol") != string::npos)
        {
            extractFloat(temp, _generalVol);
            checkBounds(0.0, 1.0, _generalVol);
        }
        else if (fullLine.find("mouse-sensitivity") != string::npos)
        {
            extractFloat(temp, _mouseSensitivity);
            checkBounds(0.0, 1.0, _mouseSensitivity);
        }

        // Points
        else if (fullLine.find("render-resolution") != string::npos)
        {
            extractPoint(temp, _renderResolution);
            if (_renderResolution.x < 0) _renderResolution.x *= (-1);
            if (_renderResolution.y < 0) _renderResolution.y *= (-1);
        }
        else if (fullLine.find("resolution") != string::npos)
        {
            extractPoint(temp, _resolution);
            if (_resolution.x < 0) _resolution.x *= (-1);
            if (_resolution.y < 0) _resolution.y *= (-1);
        }

        // Strings
        else if (fullLine.find("texture-filtering") != string::npos)
        {
            if (temp == "0" || temp == "nearest" || temp == "low")
                _textureFiltering = "0";
            else if (temp == "1" || temp == "linear" || temp == "normal")
                _textureFiltering = "1";
            else if (temp == "2" || temp == "best" || temp == "anisotropic" || temp == "high")
                _textureFiltering = "2";

        }
        else if (fullLine.find("driver") != string::npos)
        {
            SDL_RendererInfo info;
            int i(0);

            for (; SDL_GetRenderDriverInfo(i, &info) == 0; i++)
            {
                if (temp == info.name)
                {
                    _driver_name = info.name;
                    _driver_index = i;
                }
            }

            if (i == 0)
            {
                _driver_name = "default";
                    _driver_index = (-1);
            }
        }
        else if (fullLine.find("language") != string::npos)
        {
            _language = temp;
        }
    }

    file.close(); // close the configuration file

    makePublic(); // make the loaded settings publicly available

    return 0;
}

int PRL_Config :: loadLng()
{
    std::string fullLine(""), path = std::string(PRL_CONFIG_FOLDER) + std::string("/") + std::string(_language) + std::string(".lng");
    std::string key(""), value("");

    size_t i(0);

    ifstream file(path);

    if (!file.is_open())
    {
        std::string temp("Unable to open the language file (");
        temp += path;
        temp += ")";
        PRL_SetError(temp);
        return PRL_ERROR;
    }

    for (int cpt(1); getline(file, fullLine); cpt++) // read a single line
    {
        if (fullLine.length() > 3) // not an empty line
        {
            extractSetting(fullLine, value, true);
            i = fullLine.find("=");

            if (i == string::npos || i == 0)
            {
                file.close();
                mtext.clear();
                std::string temp("The line ");
                temp += to_string(cpt);
                temp += " is not well configured (equal sign badly placed)";
                PRL_SetError(temp);
                return PRL_ERROR;
            }
            else
            {
                key = fullLine.substr(0, i);
                mtext[key] = value;
            }
        }
    }

    file.close();
    return 0;
}

int PRL_Config :: writeSettings() const
{
    ofstream filew(PRL_CONFIG_FILE_PATH, std::ofstream::out|std::ofstream::trunc); // erases the old file and reopen it

    if (!filew.is_open())
    {
        std::string temp ("Unable to open the configuration file (");
        temp += PRL_CONFIG_FILE_PATH;
        temp += ")";
        PRL_SetError(temp);
        return PRL_ERROR;
    }

    filew << "# ---------- Configuration File ----------" << endl << endl << endl;
    filew << "# If a specific setting is not defined here, the default one will be loaded." << endl << endl;


    // Booleans
    filew << "high-dpi = " << btoc(_highDpi) << endl;
    filew << "vsync = " << btoc(_vsync) << endl;
    filew << "cpu-relieve = " << btoc(_cpuRelieve) << endl;
    filew << "resizable = " << btoc(_resizable) << endl;
    filew << "borderless = " << btoc(_borderless) << endl;
    filew << endl;

    // Integers
    filew << "texture-filtering = " << ito3scale(stoi(_textureFiltering)) << endl;
    filew << "priority = " << ito3scale((int) _processPriority) << endl;
    filew << "trig-precision = " << ito5scale(_trigoPrecision) << endl;
    filew << endl;

    // Floats
    filew << "fps = " << to_string(_fps).substr(0, 6 + (int)fps / 100) << endl; // keep 3 numbers after the decimal point
    filew << "brightness = " << to_string(_brightness).substr(0, 5) << endl;
    filew << "music-vol = " << to_string(_musicVol).substr(0, 5) << endl;
    filew << "effects-vol = " << to_string(_effectsVol).substr(0, 5) << endl;
    filew << "voice-vol = " << to_string(_voiceVol).substr(0, 5) << endl;
    filew << "general-vol = " << to_string(_generalVol).substr(0, 5) << endl;
    filew << "mouse-sensitivity = " << to_string(_mouseSensitivity).substr(0, 5) << endl;
    filew << endl;

    // Points
    filew << "render-resolution = " << to_string(_renderResolution.x);
    filew << " x " << to_string(_renderResolution.y) << endl;

    filew << "resolution = " << to_string(_resolution.x);
    filew << " x " << to_string(_resolution.y) << endl;
    filew << endl;

    // Strings
    filew << "driver = " << _driver_name << endl;
    filew << "language = " << _language << endl;
    filew << endl << endl;

    filew << "# Modified the " << PRL_TimeStamp() << " by the Pearl Engine." << endl;

    filew.close();
    return 0;
}

int PRL_Config :: getText(std::string key, std::string& txt) const
{
    std::string temp("");
    try
    {
        temp = mtext.at(key);
    }
    catch (std::out_of_range)
    {
        temp = "Could not find the text corresponding to the key \"";
        temp += key;
        temp += "\"";
        PRL_SetError(temp);
        return PRL_ERROR;
    }

    txt = temp;
    return 0;
}

void PRL_GetPath(std::string const& filepath, std::string& parentfolder, std::string& filename, std::string& fileformat)
{
    std::string temp("");
    unsigned int i(filepath.size() - 1);

    for (; i >= 0; i--)
    {
        if (filepath[i] == '.')
        {
            fileformat = temp;
            temp.clear();
            break;
        }
        else
            temp = filepath[i] + temp;
    }
    i--; // removes the point

    for (; i >= 0; i--)
    {
        if (i == 0)
        {
            filename = temp;
            parentfolder.clear(); // no parent folder
            break;
        }
        else if (filepath[i] == '/' || filepath[i] == '\\')
        {
            filename = temp;

            parentfolder = filepath.substr(0, filepath.size() - filename.size() - fileformat.size() - 2);
            break;
        }
        else
            temp = filepath[i] + temp;
    }
}

void PRL_SetTextureColorMod(SDL_Texture *texture, PRL_ColorMod colmod)
{
    Uint8 r, g, b;
    if (colmod==COLMOD_NIGHT)
    {
        r=80; g=100; b=100;
    }
    else if (colmod==COLMOD_DAWN)
    {
        r=245; g=240; b=170;
    }
    else if(colmod==COLMOD_DARKNIGHT)
    {
        r=35; g=25; b=75;
    }
    else if(colmod==COLMOD_EVENING)
    {
        r=220; g=215; b=200;
    }
    SDL_SetTextureColorMod(texture, r, g, b);
}
