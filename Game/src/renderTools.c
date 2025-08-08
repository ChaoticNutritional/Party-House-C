#include "renderTools.h"

static struct WindowData
{
    uint32_t height;
    uint32_t width;
    bool changed;
} _windowData;

/// @brief pass in an application obj, check if its dimensions have changed. update those
/// @param app 
void UpdateWindowSize(Application* app)
{
    uint32_t appHeight = p_fwGetWindowHeight(app);
    uint32_t appWidth = p_fwGetWindowWidth(app);
    if (_windowData.height == appHeight && _windowData.width == appWidth)
    {
       // changes were made
        _windowData.changed = false;
        return;
    }
    else
    {
        _windowData.height = appHeight;
        _windowData.width = appWidth;
        _windowData.changed = true;
    }
}

uint32_t GetWindowHeight()
{
    return _windowData.height;
}

uint32_t GetWindowWidth()
{
    return _windowData.width;
}


