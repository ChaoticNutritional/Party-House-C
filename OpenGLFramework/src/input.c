#include <Windows.h>
#include "baseTypes.h"
#include "input.h"

#include <assert.h>

/// @brief Array to map enum of key ID to their key codes
static char KeyCodes[KEY_COUNT] = { VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0x58, 0x5A, VK_ESCAPE };

/// @brief Keyboard state
typedef struct {
    bool keyDown[256];
} Keyboard;

/// @brief Mouse state
typedef struct {
    Coord2D position;
    bool	buttons[INPUT_BUTTON_COUNT];
} Mouse;

static Keyboard s_Keyboard;
static Mouse s_Mouse;

/// @brief Retrieves the pressed state for a keyboard key
/// @param vkCode 
/// @return 
bool inputKeyPressed(char vkCode)
{
    return s_Keyboard.keyDown[vkCode];
}

/// @brief Retrieves the current mouse position
/// @return 
Coord2D inputMousePosition()
{
    return s_Mouse.position;
}

/// @brief Retrieves the pressed state for a mouse button
/// @param button 
/// @return 
bool inputMousePressed(InputButton button)
{
    return s_Mouse.buttons[button];
}


/// @brief Contains data for button press callbacks
typedef struct
{
    InputCB callback;
    void* context;
    bool pressedLastFrame;
} InputBinding;


/// @brief 
static InputBinding _bindings[KEY_COUNT] = {};


/// @brief Input system initialization
void inputInit()
{
    ZeroMemory(&s_Keyboard, sizeof(Keyboard));
    ZeroMemory(&s_Mouse, sizeof(Mouse));

    // CB ctx initialization
    for (int i = 0; i < KEY_COUNT; ++i) 
    {
        _bindings[i].callback = NULL;
        _bindings[i].context = NULL;
        _bindings[i].pressedLastFrame = false;
    }
}

/// @brief Input system shutdown
void inputShutdown()
{
    ZeroMemory(&s_Keyboard, sizeof(Keyboard));
    ZeroMemory(&s_Mouse, sizeof(Mouse));
}

/// @brief Updates the pressed state of a keyboard key
/// @param vkCode 
/// @param pressed 
void inputKeyUpdate(uint8_t vkCode, bool pressed)
{
    s_Keyboard.keyDown[vkCode] = pressed;
}

/// @brief Updates the coordinates of the mouse
/// @param coords 
void inputMouseUpdatePosition(Coord2D coords)
{
    s_Mouse.position = coords;
}

/// @brief Updates the pressed state of a mouse button
/// @param button 
/// @param pressed 
void inputMouseUpdateButton(InputButton button, bool pressed)
{
    s_Mouse.buttons[button] = pressed;
}


/// @brief Sets callback function to execute on given button press
/// @param keyCode 
/// @param cb 
/// @param ctx 
void inputSetCallback(GameKey key, InputCB cb, void* ctx) {
    assert(key < KEY_COUNT);
    _bindings[key].callback = cb;
    _bindings[key].context = ctx;
}


/// @brief - resets callback functionality to NULL for given key
/// @param key - enum index of key in bindings array
void inputClearCallback(GameKey key) {
    assert(key < KEY_COUNT);
    _bindings[key].callback = NULL;
    _bindings[key].context = NULL;
}



/// @brief - resets callback functionality to NULL for all keybinds
void inputClearAllCallbacks() {
    for (int i = 0; i < KEY_COUNT; ++i) {
        inputClearCallback((GameKey)i);
    }
}


/// @brief called every frame, iterates over list of keycodes to see if one was pressed that frame
void inputUpdate() {

    for (int i = 0; i < KEY_COUNT; ++i) {
        bool pressed = inputKeyPressed(KeyCodes[i]);

        if (pressed && !_bindings[i].pressedLastFrame/* && _bindings[i].callback != NULL*/) {
            assert(_bindings[i].callback != NULL);
            _bindings[i].callback(_bindings[i].context);
        }

        _bindings[i].pressedLastFrame = pressed;
    }
}

