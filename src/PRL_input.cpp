#include <SDL2/SDL.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

#include "PRL_input.h"
#include "PRL_types.h"

using std::cout;
using std::cerr;
using std::endl;

PRL_Input :: PRL_Input() : m_x_int(0), m_y_int(0), m_xRel_int(0), m_yRel_int(0), m_x(0), m_y(0), m_xRel(0), m_yRel(0),
battery_percent(-1), battery_sec_left(-1), quit(false), textInputEnabled(false), textInputChanged(false), textCursorPosition(0)
{
    // Initialize key array
    for(int i(0); i < PRL_MAX_KEYBOARDS; i++)
    {
        for(int j(0); j < SDL_NUM_SCANCODES; j++)
            key[i][j] = false;
    }

    // Initialize mouse button array
    for(int i(0); i < PRL_MAX_MICE; i++)
    {
        for(int j(0); j < PRL_MAX_MOUSE_BUTTONS; j++)
            mousebutton[i][j] = false;
    }

    // Initialize joysticks
    for(int i(0); i < PRL_MAX_JOYSTICKS; i++)
    {
        joystick[i]=NULL;
        joyPowerLevel[i]=SDL_JOYSTICK_POWER_UNKNOWN;
        // Initialize joystick button array
        for(int j(0); j < PRL_MAX_JOY_BUTTONS; j++)
            joybutton[i][j] = false;

        // Initialize joystick axis array
        for(int j(0); j < PRL_MAX_JOY_AXIS; j++)
            joyaxis[i][j] = 0.0;

        // Initialize joystick hat array
        for(int j(0); j < PRL_MAX_JOY_HATS; j++)
            joyhat[i][j] = SDL_HAT_CENTERED;

        // Initialize joystick ball array
        for(int j(0); j < PRL_MAX_JOY_BALLS; j++)
            {
                joyballx[i][j] = 0;
                joybally[i][j] = 0;
            }
    }

    // Initialize window events
    for (int i(0); i<PRL_MAX_WINDOW_EVENTS; i++)
        winevent[i] = false;

    SDL_FlushEvent(SDL_JOYDEVICEADDED);

    keyb_count = mice_count = 1;
    joy_count = SDL_NumJoysticks();

    for (int i(0); i < joy_count; i++)
    {
        joystick[i] = SDL_JoystickOpen(i);
        joybutton_count[i] = SDL_JoystickNumButtons(joystick[i]);
        joyaxis_count[i] = SDL_JoystickNumAxes(joystick[i]);
        joyhat_count[i] = SDL_JoystickNumHats(joystick[i]);
        joyball_count[i] = SDL_JoystickNumBalls(joystick[i]);
        joydeadzone[i] = PRL_DEFAULT_DEADZONE;
        strcpy(joyname[i], SDL_JoystickName(joystick[i]));

        if (joystick[i]==NULL)
            #if PRL_USE_WARNINGS == 1
            cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, could not open the controller of index " << i << ": " << SDL_GetError() << endl;
            #endif // PRL_USE_WARNINGS
    }
    posAtLastClick = {0, 0, 1, 1};
    SDL_JoystickEventState(SDL_ENABLE);
    textInput = "";
    textSelected = "";
}

PRL_Input :: ~PRL_Input()
{
    char temp[200] = "";
    for (int i(0); i<PRL_MAX_JOYSTICKS; i++)
    {
        if (SDL_JoystickGetAttached(joystick[i]) == SDL_TRUE)
            SDL_JoystickClose(joystick[i]);
        else
        {
            strcpy(temp, SDL_GetError());
            if (strcmp(temp, "Joystick hasn't been opened yet") != 0)
            {
                #if PRL_USE_WARNINGS == 1
                cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, could not close the controller of index " << i << ": " << temp << endl;
                #endif // PRL_USE_WARNINGS
            }
        }
    }
}

// Joystick
void PRL_Input :: updateJoyPowerLevel()
{
    if (joy_count>0)
    {
        for (int i(0); i<PRL_MAX_JOYSTICKS; i++)
        {
            if (joystick[i] != NULL)
            {
                joyPowerLevel[i] = SDL_JoystickCurrentPowerLevel(joystick[i]);
                if(joyPowerLevel[i] == SDL_JOYSTICK_POWER_UNKNOWN)
                    #if PRL_USE_WARNINGS == 1
                    cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, could not update controller power level: " << SDL_GetError() << endl;
                    #endif // PRL_USE_WARNINGS
            }
        }
    }
}

SDL_JoystickPowerLevel PRL_Input :: getJoyPowerLevel(const unsigned int index) const
{
    return joyPowerLevel[index];
}

void PRL_Input :: updatePowerLevel()
{
    powerState = SDL_GetPowerInfo(&battery_sec_left, &battery_percent);
    if (powerState == SDL_POWERSTATE_UNKNOWN)
        #if PRL_USE_WARNINGS == 1
        cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, could not determine power status" << endl;
    #endif // PRL_USE_WARNINGS
}

// Keyboard
bool PRL_Input :: isKeyPressed(const unsigned int key_scancode) const
{
    return key[0][key_scancode];
}

void PRL_Input :: resetKey(const unsigned int key_scancode)
{
    key[0][key_scancode] = false;
}

bool PRL_Input :: isKeyPressedAndReset(const unsigned int key_scancode)
{
    bool temp = key[0][key_scancode];
    key[0][key_scancode]=false;
    return temp;
}

// Mouse
bool PRL_Input :: isMouseButtonPressed(const unsigned int mouse_button) const
{
    return mousebutton[0][mouse_button];
}

bool PRL_Input :: mouseHasMoved() const
{
    if (m_xRel!=0.0f || m_yRel!=0.0f)
        return true;
    return false;
}

void PRL_Input :: queryMouseRel(int *xRel, int* yRel) const
{
    *xRel=m_xRel_int;
    *yRel=m_yRel_int;
}

void PRL_Input :: queryMouseRel(float *xRel, float* yRel) const
{
    *xRel=m_xRel;
    *yRel=m_yRel;
}

PRL_FPoint PRL_Input :: getMouseRelF() const
{
    return {m_xRel, m_yRel};
}

PRL_Point PRL_Input :: getMouseRel() const
{
    return {m_xRel_int, m_yRel_int};
}

void PRL_Input :: queryMouse(int *x, int *y) const
{
    *x=m_x_int;
    *y=m_y_int;
}

void PRL_Input :: queryMouse(float *x, float *y) const
{
    *x=m_x;
    *y=m_y;
}

PRL_FPoint PRL_Input :: getMouseF() const
{
    return {m_x, m_y};
}

PRL_Point PRL_Input :: getMouse() const
{
    return {m_x_int, m_y_int};
}

SDL_Rect PRL_Input :: getLastClickPos() const
{
    return posAtLastClick;
}

PRL_Point PRL_Input :: getLastClickPoint() const
{
    return {posAtLastClick.x, posAtLastClick.y};
}

PRL_FPoint PRL_Input :: getLastClickFPoint() const
{
    return (PRL_FPoint) {1.0f*posAtLastClick.x, 1.0f*posAtLastClick.y};
}

bool PRL_Input :: quitEvent() const
{
    return quit;
}

// Window

bool PRL_Input :: getWindowEvent(const unsigned int event_code) const
{
    return winevent[event_code];
}

// Text Input

void PRL_Input :: enableTextInput()
{
    textInputEnabled = true;
    SDL_StartTextInput();
}

void PRL_Input :: disableTextInput()
{
    textInputEnabled = false;
    SDL_StopTextInput();
}

void PRL_Input :: resetTextInput()
{
    textInput = "";
}

bool PRL_Input :: hasTextInputChanged() const
{
    return textInputChanged;
}

std::string PRL_Input :: getTextInput() const
{
    return textInput;
}

char* PRL_Input :: getTextInputChar() const
{
    return (char*) textInput.c_str();
}

void PRL_Input :: setTextInputBeginning(std::string str)
{
    textInput = str;
    textCursorPosition = textInput.length();
}

void PRL_Input :: setTextInputBeginning(const char* str)
{
    textInput = str;
    textCursorPosition = textInput.length();
}

// Special

SDL_Event* PRL_Input :: getEvent() const
{
    return (SDL_Event*) &event;
}

// Update
void PRL_Input :: update() // update the input
{
    // Update the relative positions in the case where there isn't any motion
    m_xRel = m_yRel = m_xRel_int = m_yRel_int = 0;
    textInputChanged = false;

    for (int i(0); i<PRL_MAX_JOYSTICKS; i++)
    {
        for (int j(0); j<PRL_MAX_JOY_BALLS; j++)
        {
            joyballx[i][j] = joybally[i][j] = 0;
        }
    }

    if (textInputEnabled && textCursorPosition >= textInput.length()) textCursorPosition = textInput.length();

    // Re-initialize window events
    for (int i(0); i<PRL_MAX_WINDOW_EVENTS; i++)
        winevent[i] = false;

    while(SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quit = true;
            break;

        case SDL_KEYDOWN:
            #if PRL_USE_SCANCODES == 1
            key[0][event.key.keysym.scancode] = true;
            #else
            key[0][event.key.keysym.sym] = true;
            #endif // PRL_USE_SCANCODES

            if (textInputEnabled)
            {
                if (key[0][SDL_SCANCODE_C] && key[0][SDL_SCANCODE_LCTRL]) // if control + c
                {
                    SDL_SetClipboardText(textSelected.c_str());
                }
                else if (key[0][SDL_SCANCODE_V] && key[0][SDL_SCANCODE_LCTRL]) // if control + v
                {
                    std::string cpy = SDL_GetClipboardText();
                    textInput.insert(textCursorPosition, cpy);
                    textCursorPosition += cpy.length();
                    textInputChanged = true;
                }
                else if (key[0][SDL_SCANCODE_BACKSPACE] && textInput.length() > 0)
                {
                    if(key[0][SDL_SCANCODE_LCTRL])
                    {
                        while(textCursorPosition > 0 && textInput[textCursorPosition - 1] == ' ') // remove the spaces right to the text
                        {
                            textCursorPosition--;
                            textInput.erase(textCursorPosition, (size_t) 1);
                        }
                        while(textInput[textCursorPosition - 1] != ' ' && textCursorPosition > 0)
                        {
                            if (textInput[textCursorPosition - 2] == -61) // accents handling, i.e é, à, è, ù, ...
                            { // -2 because é is coded on 2 characters, then we have to test the first one
                                textCursorPosition--;
                                textInput.erase(textCursorPosition, (size_t) 1);
                            }
                            if (textCursorPosition > 0)
                            {
                                textCursorPosition--;
                                textInput.erase(textCursorPosition, (size_t) 1);
                            }
                        }
                    }
                    else
                    {
                        if (textInput[textCursorPosition-2] == -61) // accents handling, i.e é, à, è, ù, ...
                        { // -2 because é is coded on 2 characters, then we have to test the first one
                            if (textCursorPosition > 0)
                            {
                                textCursorPosition--;
                                textInput.erase(textCursorPosition, (size_t) 1);
                            }
                        }
                        if (textCursorPosition > 0)
                        {
                            textCursorPosition--;
                            textInput.erase(textCursorPosition, (size_t) 1);
                        }
                    }
                    textInputChanged = true;
                }
                if (key[0][SDL_SCANCODE_LEFT] && textCursorPosition > 0) // left arrow to move the cursor
                {
                    if(key[0][SDL_SCANCODE_LCTRL])
                    {
                        while(textCursorPosition > 0 && textInput[textCursorPosition - 1] == ' ') // remove the spaces right to the text
                        {
                            textCursorPosition--;
                        }
                        while(textInput[textCursorPosition - 1] != ' ' && textCursorPosition > 0)
                        {
                            if (textInput[textCursorPosition - 2] == -61) // accents handling, i.e é, à, è, ù, ...
                            { // -2 because é is coded on 2 characters, then we have to test the first one
                                textCursorPosition--;
                            }
                            if (textCursorPosition > 0)
                            {
                                textCursorPosition--;
                            }
                        }
                    }
                    else
                    {
                        textCursorPosition--;
                    }
                }
                else if (key[0][SDL_SCANCODE_RIGHT] && 1) // left arrow to move the cursor
                {
                    textCursorPosition++;
                    if (textCursorPosition > textInput.length()) textCursorPosition = textInput.length();
                }
            }
            break;

        case SDL_KEYUP:
            #if PRL_USE_SCANCODES == 1
            key[0][event.key.keysym.scancode] = false;
            #else
            key[0][event.key.keysym.sym] = false;
            #endif // PRL_USE_SCANCODES
            break;

        case SDL_MOUSEBUTTONDOWN:
            mousebutton[0][event.button.button] = true;
            posAtLastClick.x = event.motion.x;
            posAtLastClick.y = event.motion.y;
            break;

        case SDL_MOUSEBUTTONUP:
            mousebutton[0][event.button.button] = false;
            break;

        case SDL_MOUSEMOTION:
            m_x = m_x_int = event.motion.x;
            m_y = m_y_int = event.motion.y;
            m_xRel += event.motion.xrel;
            m_yRel += event.motion.yrel;
            m_xRel_int = m_xRel;
            m_yRel_int = m_yRel;
            break;

        case SDL_MOUSEWHEEL:
            if(event.wheel.y > 0) // scroll up
            {
                 // ...
            }
            else if(event.wheel.y < 0) // scroll down
            {
                 // ...
            }

            if(event.wheel.x > 0) // scroll right
            {
                 // ...
            }
            else if(event.wheel.x < 0) // scroll left
            {
                 // ...
            }
            break;

        case SDL_JOYBUTTONDOWN:
            joybutton[event.jdevice.which][event.jbutton.button] = true;
            break;

        case SDL_JOYBUTTONUP:
            joybutton[event.jdevice.which][event.jbutton.button] = false;
            break;

        case SDL_JOYAXISMOTION:
            if (event.jaxis.value > joydeadzone[event.jaxis.which] || event.jaxis.value < -joydeadzone[event.jaxis.which])
                joyaxis[event.jaxis.which][event.jaxis.axis] = event.jaxis.value/32768.0;
            break;

        case SDL_JOYHATMOTION:
            joyhat[event.jhat.which][event.jhat.hat] = event.jhat.value;
            break;

        case SDL_JOYBALLMOTION:
            joyballx[event.jball.which][event.jball.ball] = event.jball.xrel;
            joybally[event.jball.which][event.jball.ball] = event.jball.yrel;
            break;

        case SDL_JOYDEVICEADDED:
            if (joy_count == PRL_MAX_JOYSTICKS)
                #if PRL_USE_WARNINGS == 1
                cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, the maximum number of controllers is reached: " << PRL_MAX_JOYSTICKS << endl;
                #endif // PRL_USE_WARNINGS
            else
                for (int i(0); i<PRL_MAX_JOYSTICKS; i++)
                {
                    if (joystick[i]==NULL)
                    {
                        joystick[i] = SDL_JoystickOpen(event.jdevice.which);
                        if (joystick[i]==NULL)
                            #if PRL_USE_WARNINGS == 1
                            cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, could not open the controller of index " << i << ": " << SDL_GetError() << endl;
                            #endif // PRL_USE_WARNINGS
                        else
                        {
                            #if PRL_USE_WARNINGS == 1
                            cout << __FILENAME__ << ", line " << __LINE__ << " Info: added controller of ID: " << SDL_JoystickInstanceID(joystick[i]) << endl;
                            #endif // PRL_USE_WARNINGS
                            joy_count++;
                        }
                        break;
                    }
                }
            break;

        case SDL_JOYDEVICEREMOVED:
            #if PRL_USE_WARNINGS == 1
            cout << __FILENAME__ << ", line " << __LINE__ << " Info: removed a controller" << endl;
            #endif // PRL_USE_WARNINGS
            if (SDL_JoystickFromInstanceID(event.jdevice.which) == NULL)
            {
                #if PRL_USE_WARNINGS == 1
                cout << __FILENAME__ << ", line " << __LINE__ << " In PRL_Input, unable to close the controller (ID" << event.jdevice.which << "): " << SDL_GetError() << endl;
                #endif // PRL_USE_WARNINGS
            }
            else
            {
                for (int i(0); i<PRL_MAX_JOYSTICKS; i++)
                {
                    if(joystick[i] == SDL_JoystickFromInstanceID(event.jdevice.which))
                    {
                        SDL_JoystickClose(joystick[i]);
                        joystick[i]=NULL;
                        joy_count--;
                        break;
                    }
                }
            }
            break;

        case SDL_WINDOWEVENT:
            if (event.window.type == SDL_WINDOWEVENT_CLOSE) quit = true;
            winevent[event.window.type] = true;
            break;

        case SDL_TEXTINPUT:
            if ( !((event.text.text[0] == 'c' || event.text.text[0] == 'C' || event.text.text[0] == 'v' || event.text.text[0] == 'V') && key[0][SDL_SCANCODE_LCTRL]) && !key[0][SDL_SCANCODE_BACKSPACE])
            {
                textInput.insert(textCursorPosition, event.text.text);
                textInputChanged = true;
                textCursorPosition++;

                if (*event.text.text == -61) textCursorPosition++; //printf("%d\n", *event.text.text) gives -61 when accent are used. Thus coded on 2 char:
            }
            break;

        default:
            break;
        }
    }
}

void PRL_Input :: backspace()
{
    if (textCursorPosition > 0)
    {
        textCursorPosition--;
        textInput.erase(textCursorPosition);
    }
}
