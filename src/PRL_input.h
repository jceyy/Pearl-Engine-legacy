#ifndef PRL_INPUT_H_INCLUDED
#define PRL_INPUT_H_INCLUDED

#include <SDL2/SDL.h>
#include "PRL_types.h"
#include "PRL_defines.h"

#define PRL_MAX_MOUSE_BUTTONS 8
#define PRL_MAX_KEYBOARDS 1
#define PRL_MAX_MICE 1
#define PRL_MAX_WINDOW_EVENTS 17*100

#define PRL_MAX_JOYSTICKS 8
#define PRL_MAX_JOY_BUTTONS 16
#define PRL_MAX_JOY_AXIS 16
#define PRL_MAX_JOY_HATS 4
#define PRL_MAX_JOY_BALLS 2
#define PRL_MAX_JOY_AXIS_VALUE 32767
#define PRL_MIN_JOY_AXIS_VALUE (-32768)
#define PRL_DEFAULT_DEADZONE 2000
#define PRL_MAX_JOYNAME_LENGTH 300

class PRL_Input
{
public:
    PRL_Input();
    ~PRL_Input();

    void update();

    // Joystick
    void updateJoyPowerLevel();
    bool isJoyButtonPressed(const unsigned int button_code) const;
    SDL_JoystickPowerLevel getJoyPowerLevel(const unsigned int index) const;

    // Keyboard
    bool isKeyPressed(const unsigned int key_scancode) const;
    void resetKey(const unsigned int key_scancode);
    bool isKeyPressedAndReset(const unsigned int key_scancode);
    // TO BE IMPLEMENTED: bool isKeyPressed(const unsigned int which, const unsigned int key_scancode) const;

    // Mouse
    bool isMouseButtonPressed(const unsigned int mouse_button) const;
    // TO BE IMPLEMENTED: bool isMouseButtonPressed(const unsigned int which, const unsigned int mouse_button) const;
    bool mouseHasMoved() const;
    void queryMouseRel(int *xRel, int *yRel) const;
    void queryMouseRel(float *xRel, float *yRel) const;
    PRL_FPoint getMouseRelF() const;
    PRL_Point getMouseRel() const;
    void queryMouse(int *x, int *y) const;
    void queryMouse(float *x, float *y) const;
    PRL_FPoint getMouseF() const;
    PRL_Point getMouse() const;
    SDL_Rect getLastClickPos() const;
    PRL_Point getLastClickPoint() const;
    PRL_FPoint getLastClickFPoint() const;

    // Quit
    bool quitEvent() const;

    // Power
    void updatePowerLevel();

    // Window
    bool getWindowEvent(const unsigned int event_code) const;

    // Text input
    void enableTextInput();
    void disableTextInput();
    void resetTextInput();
    bool hasTextInputChanged() const;
    std::string getTextInput() const;
    char* getTextInputChar() const;
    void setTextInputBeginning(std::string str);
    void setTextInputBeginning(const char* str);

    // Special
    SDL_Event* getEvent() const;


private:
    SDL_Event event;
    SDL_Rect posAtLastClick; // position of the mouse at the last click (mouse button down)

    // Keyboard
    bool key[PRL_MAX_KEYBOARDS][SDL_NUM_SCANCODES];

    // Mouse
    bool mousebutton[PRL_MAX_MICE][PRL_MAX_MOUSE_BUTTONS];
    int m_x_int, m_y_int;
    int m_xRel_int, m_yRel_int;
    float m_x, m_y;
    float m_xRel, m_yRel;

    // Joystick
    SDL_Joystick *joystick[PRL_MAX_JOYSTICKS];
    SDL_JoystickPowerLevel joyPowerLevel[PRL_MAX_JOYSTICKS];
    bool joybutton[PRL_MAX_JOYSTICKS][PRL_MAX_JOY_BUTTONS];
    float joyaxis[PRL_MAX_JOYSTICKS][PRL_MAX_JOY_AXIS];
    int joyhat[PRL_MAX_JOYSTICKS][PRL_MAX_JOY_HATS];
    int joyballx[PRL_MAX_JOYSTICKS][PRL_MAX_JOY_BALLS];
    int joybally[PRL_MAX_JOYSTICKS][PRL_MAX_JOY_BALLS];
    int joydeadzone[PRL_MAX_JOYSTICKS];
    int joybutton_count[PRL_MAX_JOYSTICKS];
    int joyaxis_count[PRL_MAX_JOYSTICKS];
    int joyhat_count[PRL_MAX_JOYSTICKS];
    int joyball_count[PRL_MAX_JOYSTICKS];
    char joyname[PRL_MAX_JOYSTICKS][PRL_MAX_JOYNAME_LENGTH];

    // Power
    int battery_sec_left, battery_percent;
    SDL_PowerState powerState;

    // Window
    bool winevent[PRL_MAX_WINDOW_EVENTS];

    // Text input
    bool textInputEnabled;
    bool textInputChanged;
    std::string textInput, textSelected;
    int textCursorPosition;
    void backspace();

    int keyb_count, mice_count, joy_count;

    bool quit;
};

#endif // PRL_INPUT_H_INCLUDED
