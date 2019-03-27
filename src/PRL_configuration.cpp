#include <iostream>
#include <exception>
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

#include "PRL_time.h"
#include "PRL_global_variables.h"

#include "PRL_configuration.h"
#include "PRL_math.h"

#include "PRL_handler.h"
#include "PRL_input.h"
#include "PRL_defines.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;

// Declaration of extern variable
PRL_Handler handler;

// convert a bool into a string
string btoc(bool b)
{
    if (b == true)
        return "true";
    else
        return "false";
}

// convert an int into a string
string ito3scale(int n)
{
    if (n <= 0)
        return "low";
    else if (n == 1)
        return "normal";
    else
        return "high";
}

// convert an int into a string
string ito5scale(int n)
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

void PRL_SetError(std::string const& msg, bool auto_throw)
{
    PRL_ErrorHandling::setError(msg);

    if (auto_throw)
	{
		std::runtime_error e(PRL_GetError());
		throw e;
	}
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
    SDL_GetRendererInfo(handler.display.renderer[0], &info);
    SDL_DisplayMode current;
    const SDL_version *linkv = IMG_Linked_Version();

    cout << "Platform detection: " << SDL_GetPlatform() << endl << endl;

    cout << "--- Program compilation properties ---" << endl << endl;
    cout << "Program compiled on " << handler.config.getCompilationTime() << " using:" << endl;
    cout << "SDL version: " << SDL_MAJOR_VERSION << "." << SDL_MINOR_VERSION << "." << SDL_PATCHLEVEL << endl;
    cout << "SDL_TTF version: " << TTF_MAJOR_VERSION << "." << TTF_MINOR_VERSION << "." << TTF_PATCHLEVEL << endl;
    cout << "SDL_IMG version: " << (int) linkv->major << "." << (int) linkv->minor << "." << (int) linkv->patch << endl;
    cout << "PEARL ENGINE version: " << PRL_MAJOR_VERSION << "." << PRL_MINOR_VERSION << "." << PRL_PATCHLEVEL << endl;
    cout << endl << endl;

    cout << "--- System settings ---" << endl << endl;
    cout << "Logical CPU cores: " << SDL_GetCPUCount() << endl;
    cout << "L1 cache line size: " << SDL_GetCPUCacheLineSize() << " bytes" << endl;
    cout << "System RAM: " << SDL_GetSystemRAM() << " MB" << endl;
    cout << "'long long' type stored on  " << sizeof(long long) << " bytes" << endl;
    cout << endl << endl;

    cout << "--- Devices ---" << endl << endl;
    cout << "Available video displays: " << SDL_GetNumVideoDisplays() << endl;

    for(i = 0; i < SDL_GetNumVideoDisplays(); ++i)
    {
        if (SDL_GetCurrentDisplayMode(i, &current) != 0)
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
    cout << "Display resolution: " << handler.config.getResolution().x << "x" << handler.config.getResolution().y << endl;
    cout << "Renderer resolution: " << handler.config.getRenderResolution().x << "x" << handler.config.getRenderResolution().y << endl;
    cout << "Video driver: " << SDL_GetCurrentVideoDriver() << endl;
    cout << "Renderer name: " << info.name << endl; // compare to settings values
    cout << "Renderer max. texture size: " << info.max_texture_width << " x " << info.max_texture_height << endl;
    cout << endl;

    cout << "Using high-DPI: " << btoc(handler.config.getHighDpi()) << endl;
    cout << "Using vSync: " << btoc(handler.config.getVsync()) << endl;
    cout << "Using full-screen: " << btoc(handler.config.getFullscreen()) << endl;
    cout << "Using CPU-relieve: " << btoc(handler.config.getCpuRelieve()) << endl;
    cout << "Resizable window: " << btoc(handler.config.getResizable()) << endl;
    cout << "Borderless window: " << btoc(handler.config.getBorderless()) << endl;
    cout << endl;

    cout << "Texture filtering: " << ito3scale(stoi(handler.config.getTextureFiltering())) << endl;
    cout << "Process priority: " << ito3scale((int)handler.config.getProcessPriority()) << endl;
    cout << endl;

    cout << "Frames per second: " << handler.config.getFps() << endl;
    cout << "Brightness: " << handler.config.getBrightness() << endl;
    cout << "Music volume: " << handler.config.getMusicVol() << endl;
    cout << "Effects volume: " << handler.config.getEffectsVol() << endl;
    cout << "Voice volume: " << handler.config.getVoiceVol() << endl;
    cout << "General volume: " << handler.config.getGeneralVol() << endl;
    cout << "Mouse sensitivity: " << handler.config.getMouseSensitivity() << endl;
    cout << endl;

    cout << "Language: " << handler.config.getLanguage() << endl;
    cout << endl;

    // IDEAS:
    // controllers plugged in?
    // Letterboxing / ...
}

int PRL_CreateWindowAndRenderer()
{
    // SYSTEM
    if (SDL_SetThreadPriority(handler.config.getProcessPriority()) != 0)
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

    if (handler.config.getFullscreen() == true)
        flagw1 = SDL_WINDOW_FULLSCREEN;

    if (handler.config.getHighDpi() == true)
        flagw2 = SDL_WINDOW_ALLOW_HIGHDPI;

    if (handler.config.getResizable() == true)
        flagw3 = SDL_WINDOW_RESIZABLE;

    if (handler.config.getBorderless() == true)
        flagw4 = SDL_WINDOW_BORDERLESS;

    /*if (PRL_CONFIG_WINDOW_ALLWAYS_ON_THE_TOP)
        flagw5 = SDL_WINDOW_ALWAYS_ON_TOP;

    if (PRL_CONFIG_CAPTURE_MOUSE)
        flagw6 = SDL_WINDOW_MOUSE_CAPTURE;*/

    Uint32 flagsw = flagw1|flagw2|flagw3|flagw4|flagw5|flagw6;

    handler.display.window.push_back(SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                             handler.config.getResolution().x, handler.config.getResolution().y, flagsw));

    if (handler.display.window[0] == nullptr)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to create the window: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to create the window: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }


    // RENDERER
    Uint32 flagr1(0), flagr2(SDL_RENDERER_ACCELERATED);

    if(handler.config.getVsync() == true)
        flagr1 = SDL_RENDERER_PRESENTVSYNC;

    if (handler.config.getDriverName() == "software")
        flagr2 = SDL_RENDERER_SOFTWARE;

    Uint32 flags_rend = flagr1 | flagr2;

    handler.display.renderer.push_back(SDL_CreateRenderer(handler.display.window[0], handler.config.getDriverIndex(), flags_rend));
    if (handler.display.renderer[0] == nullptr)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to create the renderer: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to create the renderer: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if (SDL_RenderSetLogicalSize(handler.display.renderer[0], handler.config.getRenderResolution().x, handler.config.getRenderResolution().y) != 0)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to set the logical size: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to set logical size of renderer: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, handler.config.getTextureFiltering().c_str()) != SDL_TRUE)
    {
        #if PRL_AUTO_WRITE_ERRORS == 1
        cerr << __CERR_REF__ << "In PRL_CreateWindowAndRenderer, unable to set the texture filtering: " << SDL_GetError() << endl;
        #endif // PRL_AUTO_WRITE_ERRORS
        PRL_SetError(string("Unable to set texture filtering: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    //SDL_SetHint(SDL_HINT_FRAMEBUFFER_ACCELERATION, "1"); //(e.g. "0", "1", or "direct3d", "opengl", etc.)

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

    #if DEBUGGING == 0
    freopen("output.txt", "w", stdout);
    freopen("error.txt", "w", stderr);
    #endif // DEBUGGING

    cout << PRL_TimeStamp() << " Initialization..." << endl;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        PRL_SetError(string("Failed to load the SDL: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    int imgflags = IMG_INIT_JPG|IMG_INIT_PNG|IMG_INIT_TIF|IMG_INIT_WEBP;

    if (IMG_Init(imgflags) != imgflags)
    {
        PRL_SetError(string("Failed to load SDL_Image: ") + string(IMG_GetError()));
        return PRL_ERROR;
    }

    if (TTF_Init() == -1)// TTF Initialization
    {
        PRL_SetError(string("Failed to load SDL_TTF: ") + string(TTF_GetError()));
        return PRL_ERROR;
    }

    if (SDL_JoystickEventState(SDL_ENABLE) < 0)
    {
        PRL_SetError(string("Failed to enable controllers: ") + string(SDL_GetError()));
        return PRL_ERROR;
    }

    if (handler.config.loadSettings() != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_Init, could not load the settings: " << PRL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    if (handler.config.loadLng() != 0)
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

    if (SDL_SetWindowBrightness(handler.display.window[0], handler.config.getBrightness()) != 0)
    {
        #if PRL_USE_WARNINGS == 1
        cout << __CERR_REF__ << "In PRL_Init, unable to set the brightness: " << SDL_GetError() << endl;
        #endif // PRL_USE_WARNINGS
    }

    SDL_DisableScreenSaver();

    printLaunchDiag(); // Print infos on startup

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
    cout << PRL_TimeStamp() << " Quitting..." << endl << endl;

    cout << "Errors count: " << PRL_ErrorCount() << endl;
    cout << "Quitting PRL" << endl;
    SDL_EnableScreenSaver();
    handler.freeall();

    cout << "Quitting SDL_TTF" << endl;
    TTF_Quit();

    cout << "Quitting SDL_IMG" << endl;
    IMG_Quit();

    cout << "Quitting SDL" << endl;
    SDL_Quit();

    cout << endl << PRL_TimeStamp() + " Done" << endl;
}

void PRL_Logo()
{
}

void PRL_LoadingScreen() // whether or not to display the loading percentage.
{
    TTF_Font *font = TTF_OpenFont("data/fonts/cooper.TTF", 85);
    SDL_Color colorWhite = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Blended(font, "Loading...", colorWhite);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(handler.display.renderer[0], surface);
    SDL_Rect pos = {(handler.config.getRenderResolution().x-surface->w)/2, (handler.config.getRenderResolution().y-surface->h)/2, surface->w, surface->h};

    SDL_RenderClear(handler.display.renderer[0]);
    SDL_RenderCopy(handler.display.renderer[0], texture, NULL, &pos);
    SDL_RenderPresent(handler.display.renderer[0]);

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

    #if PRL_AUTO_WRITE_ERRORS == 1
	writeError();
	#endif // PRL_AUTO_WRITE_ERRORS
}

int PRL_ErrorHandling :: getErrorCount()
{
    return err_count;
}

void PRL_ErrorHandling::writeError()
{
	cerr << PRL_GetError() << endl;
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
    settingsNumber = 19;

    highDpi = false;
    fullscreen = false;
    vsync = false;
    cpuRelieve = true;
    resizable = false;
    borderless = false;

    textureFiltering_int = 0;
    driverIndex = -1;
    processPriority = SDL_THREAD_PRIORITY_NORMAL;
    fps = 30.0;
    brightness = 1.0;
    musicVol = 1.0;
    effectsVol = 1.0;
    voiceVol = 1.0;
    generalVol = 1.0;
    mouseSensitivity = 1.0;

    resolution = {720, 360};
    renderResolution = {720, 360};

    textureFiltering = "0";
    language = "english";
    driverName = "default";
}

int PRL_Config :: extractSetting(std::string const& fullLine, std::string& setting, bool keep_spaces) const
{
    std::string eq("=");
    size_t pos(0); pos = fullLine.find(eq);

    if (pos == 0 || fullLine.size() < 2) // min 3 characters
    {
        PRL_SetError("The provided line hasn't the right format (type = value) (setting: " + setting + ")");
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

int PRL_Config :: extractPoint(std::string const& str, PRL_Point& p)
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
        while (fullLine == "")
		{
			getline(file, fullLine);
		}
        extractSetting(fullLine, temp); /// AJOUTER LES BORNEEEEEEEEEEEEEEES!!!!!!!!!!!!!!!!!!!

        // Booleans
        if (fullLine.find("high-dpi") != string::npos)
        {
           extractBool(temp, highDpi);
        }
        else if (fullLine.find("vsync") != string::npos)
        {
            extractBool(temp, vsync);
        }
        else if (fullLine.find("fullscreen") != string::npos)
        {
            extractBool(temp, fullscreen);
        }
        else if (fullLine.find("cpu-relieve") != string::npos)
        {
            extractBool(temp, cpuRelieve);
        }
        else if (fullLine.find("resizable") != string::npos)
        {
            extractBool(temp, resizable);
        }
        else if (fullLine.find("borderless") != string::npos)
        {
            extractBool(temp, borderless);
        }

        // Integers
        else if (fullLine.find("priority") != string::npos)
        {
            if (temp == "0" || temp == "low")
                processPriority = SDL_THREAD_PRIORITY_LOW;
            else if (temp == "1" || temp == "normal")
                processPriority = SDL_THREAD_PRIORITY_NORMAL;
            else if (temp == "2" || temp == "high")
                processPriority = SDL_THREAD_PRIORITY_HIGH;
        }

        // Floats
        else if (fullLine.find("fps") != string::npos)
        {
            extractFloat(temp, fps);
            if (fps > PRL_MAX_FPS) fps = PRL_MAX_FPS;
        }
        else if (fullLine.find("brightness") != string::npos)
        {
            extractFloat(temp, brightness);
            checkBounds(0.0, 1.0, brightness);
        }
        else if (fullLine.find("music-vol") != string::npos)
        {
            extractFloat(temp, musicVol);
            checkBounds(0.0, 1.0, musicVol);
        }
        else if (fullLine.find("effects-vol") != string::npos)
        {
            extractFloat(temp, effectsVol);
            checkBounds(0.0, 1.0, effectsVol);
        }
        else if (fullLine.find("voice-vol") != string::npos)
        {
            extractFloat(temp, voiceVol);
            checkBounds(0.0, 1.0, voiceVol);
        }
        else if (fullLine.find("general-vol") != string::npos)
        {
            extractFloat(temp, generalVol);
            checkBounds(0.0, 1.0, generalVol);
        }
        else if (fullLine.find("mouse-sensitivity") != string::npos)
        {
            extractFloat(temp, mouseSensitivity);
            checkBounds(0.0, 1.0, mouseSensitivity);
        }

        // Points
        else if (fullLine.find("render-resolution") != string::npos)
        {
            extractPoint(temp, renderResolution);
            if (renderResolution.x < 0) renderResolution.x *= (-1);
            if (renderResolution.y < 0) renderResolution.y *= (-1);
        }
        else if (fullLine.find("resolution") != string::npos)
        {
            extractPoint(temp, resolution);
            if (resolution.x < 0) resolution.x *= (-1);
            if (resolution.y < 0) resolution.y *= (-1);
        }

        // Strings
        else if (fullLine.find("texture-filtering") != string::npos)
        {
            if (temp == "0" || temp == "nearest" || temp == "low")
                textureFiltering = "0";
            else if (temp == "1" || temp == "linear" || temp == "normal")
                textureFiltering = "1";
            else if (temp == "2" || temp == "best" || temp == "anisotropic" || temp == "high")
                textureFiltering = "2";

        }
        else if (fullLine.find("driver") != string::npos)
        {
            SDL_RendererInfo info;
            int i(0);

            for (; SDL_GetRenderDriverInfo(i, &info) == 0; i++)
            {
                if (temp == info.name)
                {
                    driverName = info.name;
                    driverIndex = i;
                }
            }

            if (i == 0)
            {
                driverName = "default";
				driverIndex = (-1);
            }
        }
        else if (fullLine.find("language") != string::npos)
        {
            language = temp;
        }
    }

    file.close(); // close the configuration file
    return 0;
}

int PRL_Config :: loadLng()
{
    std::string fullLine(""), path = std::string(PRL_CONFIG_FOLDER) + std::string("/") + std::string(language) + std::string(".lng");
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
    filew << "high-dpi = " << btoc(highDpi) << endl;
    filew << "vsync = " << btoc(vsync) << endl;
    filew << "cpu-relieve = " << btoc(cpuRelieve) << endl;
    filew << "resizable = " << btoc(resizable) << endl;
    filew << "borderless = " << btoc(borderless) << endl;
    filew << endl;

    // Integers
    filew << "texture-filtering = " << ito3scale(stoi(textureFiltering)) << endl;
    filew << "priority = " << ito3scale((int) processPriority) << endl;
    filew << endl;

    // Floats
    filew << "fps = " << to_string(fps).substr(0, 6 + (int)fps / 100) << endl; // keep 3 numbers after the decimal point
    filew << "brightness = " << to_string(brightness).substr(0, 5) << endl;
    filew << "music-vol = " << to_string(musicVol).substr(0, 5) << endl;
    filew << "effects-vol = " << to_string(effectsVol).substr(0, 5) << endl;
    filew << "voice-vol = " << to_string(voiceVol).substr(0, 5) << endl;
    filew << "general-vol = " << to_string(generalVol).substr(0, 5) << endl;
    filew << "mouse-sensitivity = " << to_string(mouseSensitivity).substr(0, 5) << endl;
    filew << endl;

    // Points
    filew << "render-resolution = " << to_string(renderResolution.x);
    filew << " x " << to_string(renderResolution.y) << endl;

    filew << "resolution = " << to_string(resolution.x);
    filew << " x " << to_string(resolution.y) << endl;
    filew << endl;

    // Strings
    filew << "driver = " << driverName << endl;
    filew << "language = " << language << endl;
    filew << endl << endl;

    filew << "# Modified the " << PRL_TimeStamp() << " by Pearl Engine." << endl;

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

bool PRL_Config :: getHighDpi() const
{
	return highDpi;
}

bool PRL_Config :: getFullscreen() const
{
	return fullscreen;
}

bool PRL_Config :: getVsync() const
{
	return vsync;
}
bool PRL_Config :: getCpuRelieve() const
{
	return cpuRelieve;
}

bool PRL_Config :: getResizable() const
{
	return resizable;
}

bool PRL_Config :: getBorderless() const
{
	return borderless;
}

int PRL_Config :: getTextureFiltering_int() const
{
	return textureFiltering_int;
}

int PRL_Config :: getDriverIndex() const
{
	return driverIndex;
}

SDL_ThreadPriority PRL_Config :: getProcessPriority() const
{
	return processPriority;
}

float PRL_Config :: getFps() const
{
	return fps;
}

float PRL_Config :: getBrightness() const
{
	return brightness;
}

float PRL_Config :: getMusicVol() const
{
	return musicVol;
}

float PRL_Config :: getEffectsVol() const
{
	return effectsVol;
}

float PRL_Config :: getVoiceVol() const
{
	return voiceVol;
}

float PRL_Config :: getGeneralVol() const
{
	return generalVol;
}

float PRL_Config :: getMouseSensitivity() const
{
	return mouseSensitivity;
}

PRL_Point PRL_Config :: getResolution() const
{
	return resolution;
}

PRL_Point PRL_Config :: getRenderResolution() const
{
	return renderResolution;
}

string PRL_Config :: getTextureFiltering() const
{
	return textureFiltering;
}

string PRL_Config :: getLanguage() const
{
	return language;
}

string PRL_Config :: getDriverName() const
{
	return driverName;
}

void PRL_GetPath(std::string const& filepath, std::string& parentfolder, std::string& filename, std::string& fileformat)
{
    std::string temp("");
    int i(filepath.size() - 1);

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

            parentfolder = filepath.substr(0, filepath.size() - filename.size() - fileformat.size() - 1);
            break;
        }
        else
            temp = filepath[i] + temp;
    }
}
