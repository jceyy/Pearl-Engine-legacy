#ifndef PRL_CONFIGURATION_H_INCLUDED
#define PRL_CONFIGURATION_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "PRL_types.h"

using namespace std;


//! Class responsible for the program configuration.
/*!
This class holds all the different settings used in the program.
They are privately stored and marked by an underscore, and also publicly stored (without any underscore).
The public version of all the attributes is just a copy from the private one, in order to let the user access more quickly the data needed.
Thus the changes are only applied internally, and the settings have to be manually made public.

In the configuration file, each setting occupies one single line with the following structure: setting_type = value (spaced are ignored).
You will find the recognized values of the different settings below:
- for booleans: setting = true or false
- for resolutions: setting = integer (width) x integer (height)
- for floats: setting = float (between 0.0 and 1.0 for brightness, sensitivities and volumes)

There are special cases for the other settings:
- There are several driver values: opengl, direct3d, opengles, software
- Trigonometric precision can be specified as verylow, low, normal, high and veryhigh and ultra, or 0, 1, 2, 3, 4 and 5
- Texture filtering can be specified as low, normal and high, or 0, 1 and 2, or nearest, linear and best, or nearest, linear and anisotropic
- Thread priority (process priority) can be specified as low, normal and high, or 0, 1 and 2

The different setting types are the following:
high-dpi, vsync, fullscreen, cpu-relieve, resizable, borderless, texture-filtering, priority, fps, brightness, music-vol,
effects-vol, voice-vol, general-vol, mouse-sensitivity, resolution, render-resolution, driver, and language.
*/
class PRL_Config
{
public:
    //! Constructor.
    PRL_Config();
    //! Destructor.
    ~PRL_Config();

    bool getHighDpi() const; //!< Boolean enabling high DPI (if available).
    bool getFullscreen() const; //!< Boolean enabling display in full screen mode.
    bool getVsync() const; //!< Boolean enabling vertical synchronization.
    //! Boolean enabling CPU relieve.
    /*!
    CPU relieve results in the use of a delay in the main loop of the handler, allowing the CPU to work on other processes when all computations are done.
    However be careful, this option can produce perceptible slow-downs (you may solve this problem by providing a higher frame rate).
    */
    bool getCpuRelieve() const;
    bool getResizable() const; //!< Boolean enabling resizing of the window.
    bool getBorderless() const; //!< Boolean removing the border of the window.

    int getTextureFiltering_int() const;//!< Storing the texture filtering value as an integer.
    int getDriverIndex() const; //!< Index of the rendering driver used by the SDL.
    SDL_ThreadPriority getProcessPriority() const; //!< Thread priority.

    float getFps() const; //!< Frame rate.
    float getBrightness() const; //!< Brightness.
    float getMusicVol() const; //!< Volume of the music.
    float getEffectsVol() const; //!< Volume of the effects.
    float getVoiceVol() const; //!< Volume of the voice.
    float getGeneralVol() const; //!< General volume.
    float getMouseSensitivity() const; //!< Mouse sensitivity.
    //! Render resolution (display resolution).
    /*!
    The resolution (window - display) is completely independent from the renderer's resolution!
    An upscale / downscale is automatically performed (using render scale quality settings) to match the desired resolution.
    */
    PRL_Point getResolution() const;
    //! Internal render resolution.
    /*!
    Setting a higher render resolution than the one used for display may help getting a smoother result, even though it increases the computational power needed.
    */
    PRL_Point getRenderResolution() const;

    //! Texture filtering value.
    /*!
    The texture filtering value is a scaling quality (see the SDL hint: SDL_HINT_RENDER_SCALE_QUALITY).
    Please note that the "nearest" scale quality is always working, the "linear" option is compatible with OpenGL and Direct3D, but the last one ("best") is only Direct3D compatible.
    This value can also be "0", "1" or "2" (standing respectively for "nearest", "linear" and "best").
    */
    string getTextureFiltering() const;
    string getLanguage() const; //!< Defines the language used by the software.
    string getDriverName() const; //!< Contains the name of the driver in use, or "default" if an error occurred while attempting to load the driver specified by the user.

    //! Get the settings number.
    int getSettingsNumber() const;
    //! Load the settings.
    /*!
    Use this function to load the settings from the configuration file. The configuration file's location is defined as a macro.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int loadSettings();
    //! Load the language.
    /*!
    Use this function to load the language from a language file (corresponding to the language defined in the settings).
    The languages files must be in the same folder than the configuration file.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int loadLng();
    //! Update the configuration file based on settings changed during program execution.
    /*!
    It erases the previous settings file first, then write the latests settings into it.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int writeSettings() const;

    //! Get the date of the program compilation.
    static std::string getCompilationTime();

    //! Get the text in the chosen language (in the configuration file) corresponding to a given key.
    /*!
    The language is the one specified in the configuration file (language = ...).
    Please read the notes concerning the mtext map.
    @param [in] key The key to find the text.
    @param [out] lng_text The corresponding text if the key was relevant.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int getText(std::string key, std::string& lng_text) const;
public: // to become private!!!!!!!!!!!!
//private:
    bool highDpi; //!< Boolean enabling high DPI (if available).
    bool fullscreen; //!< Boolean enabling display in full screen mode.
    bool vsync; //!< Boolean enabling vertical synchronization.
    //! Boolean enabling CPU relieve.
    /*!
    CPU relieve results in the use of a delay in the main loop of the code, allowing the CPU to work on other processes when all computations are done.
    However be careful, this option can produce perceptible slow-downs (you may solve this problem by providing a higher frame rate).
    */
    bool cpuRelieve;
    bool resizable; //!< Boolean enabling resizing of the window.
    bool borderless; //!< Boolean removing the border of the window.

    int textureFiltering_int;//!< Storing the texture filtering value as an integer.
    int driverIndex; //!< Index of the rendering driver used by the SDL.
    SDL_ThreadPriority processPriority; //!< Thread priority.

    float fps; //!< Frame rate.
    float brightness; //!< Brightness.
    float musicVol; //!< Volume of the music.
    float effectsVol; //!< Volume of the effects.
    float voiceVol; //!< Volume of the voice.
    float generalVol; //!< General volume.
    float mouseSensitivity; //!< Mouse sensitivity.
    //! Render resolution (display resolution).
    /*!
    The resolution (window - display) is completely independent from the renderer's resolution!
    An upscale / downscale is automatically performed (using render scale quality settings) to match the desired resolution.
    */
    PRL_Point resolution;
    //! Internal render resolution.
    /*!
    Setting a higher render resolution than the one used for display may help getting a smoother result, even though it increases the computational power needed.
    */
    PRL_Point renderResolution;

    //! Texture filtering value.
    /*!
    The texture filtering value is a scaling quality (see the SDL hint: SDL_HINT_RENDER_SCALE_QUALITY).
    Please note that the "nearest" scale quality is always working, the "linear" option is compatible with OpenGL and Direct3D, but the last one ("best") is only Direct3D compatible.
    This value can also be "0", "1" or "2" (standing respectively for "nearest", "linear" and "best").
    */
    string textureFiltering;
    string language; //!< Defines the language used by the software.
    string driverName; //!< Contains the name of the driver in use, or "default" if an error occurred while attempting to load the driver specified by the user.

    //! Extract a point from a string.
    /*!
    This function extracts a point from a setting's string value obtained with the extractSetting function (spaced and special characters need to be removed first).
    @param [in] str The string containing the point as follows: N1xN2.
    @param [out] point The point filled with its value.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    static int extractPoint(std::string const& str, PRL_Point& point);

private:
    //! Load the default settings.
    void loadDefault();
    //! Extract a setting from a line.
    /*!
    This function extracts the value of a setting as a string and deletes all the spaces and '\n' contained in it.
    It proceeds in searching for the equal sign in the provided line, and if it finds one, all the characters after the equal sign are copied into the second parameter.
    @param [in] fullLine The line read from the configuration file.
    @param [out] setting The string to be filled with the setting's value of the line.
    @param [in] keep_spaces Choose whether to keep spaces or not (removes the spaces by default).
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int extractSetting(std::string const& fullLine, std::string& setting, bool keep_spaces = false) const;

    //! Extract a boolean from a string.
    /*!
    This function extracts a boolean from a setting's string value obtained with the extractSetting function (spaced and special characters need to be removed first).
    @param [in] str The string containing the point as follows: N1xN2.
    @param [out] b The boolean filled with its value.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int extractBool(std::string const& str, bool& b) const;
    //! Extract a floating point number from a string.
    /*!
    This function extracts a float from a setting's string value obtained with the extractSetting function (spaced and special characters need to be removed first).
    @param [in] str The string containing the point as follows: N1xN2.
    @param [out] f The float filled with its value.
    \return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
    */
    int extractFloat(std::string const& str, float& f) const;

    int settingsNumber; //!< The number of settings available.

    //! Map containing all the language dependent text used.
    /*!
    The key is a keyword (or key-expression), and the value the corresponding text in the currently selected language.
    This map is directly loaded from a .lng file. For instance, the english language file could contain "1player=Single Player Mode": here "1player" is the key and
    "Single Player Mode" the corresponding value (text) in english.
    Be careful: in contrary to the configuration file, the spaces are not removed from the key or the string values!
    */
    std::map <std::string, std::string> mtext;
};

//! Abstract class responsible for handling the error messages.
class PRL_ErrorHandling
{
public:
    //! Purely virtual destructor.
    virtual ~PRL_ErrorHandling() = 0;
    //! Get the latest error.
    /*!
    This method returns the last error that was set. Use this function to help with debugging.
    This function is similar to PRL_GetError();
    */
    static std::string getError();
    //! Set an error.
    /*!
    Use this function to help with debugging. This function is similar to PRL_SetError().
    */
    static void setError(std::string const& msg);
    //! Get how many errors happened.
    /*!
    At each time the function setError() is called, the error count is incremented.
    This function is similar to PRL_ErrorCount().
    */
    static int getErrorCount();

private:
	//! Write the last error to the error file
	static void writeError();
    static std::string last_err; //!< Last error.
    static int err_count; //!< Errors count.
};

//! Function responsible for the initialization of the Pearl Engine.
/*!
This function initializes the SDL, SDL_Image, SDL_TTF, loads the settings and creates a window and a renderer.
\return Returns 0 on success or a negative code on failure (call SDL_GetError() for more information).
*/
int PRL_Init();
//! Quit Pearl Engine.
/*!
Quit the SDL, SDL_Image, SDL_TTF, Pearl Engine and destroys all the renderer_GLOBAL as well as the window_GLOBAL.
*/
void PRL_Quit();
// Shows the Pearl Engine's logo.
//void PRL_Logo();
//void PRL_LoadingScreen();
//! Destroy a window and a renderer in a single call.
void PRL_DestroyWindowAndRenderer(SDL_Window *window, SDL_Renderer *renderer);

void PRL_SetTextureColorMod(SDL_Texture *texture, PRL_ColorMod colmod);
//! Provide information about a file.
/*!
Use this function to get informations about a file's path.
@param [in] filepath The complete file's path.
@param [out] parentfolder The parent folder (without a terminating '/').
@param [out] filename The filename without its extension.
@param [out] fileformat The file format without the point.

For instance: providing filepath with "folder1/folder2/folder3/mygreatfile.txt" should output:
- parentfolder folder1/folder2/folder3
- filename mygreatfile
- fileformat txt
*/
void PRL_GetPath(std::string const& filepath, std::string& parentfolder, std::string& filename, std::string& fileformat);
//! Set an error and choose whether to throw an exception or not.
/*!
Use this function to set an error message when an error is encountered. It can be used to throw an exception containing the error message set.
This function is based on PRL_ErrorHandling::setError().
*/
void PRL_SetError(std::string const& msg, bool auto_throw = false);
//! Get the latest error.
/*!
Use this function to get an error message when an error occurred. This function is based on PRL_ErrorHandling::getError().
*/
std::string PRL_GetError();
//! Get the error count of the program.
int PRL_ErrorCount();
#endif //PRL_CONFIGURATION_H_INCLUDED
