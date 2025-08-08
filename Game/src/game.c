#include <assert.h>

#include "application.h"
#include "baseTypes.h"
#include "framework.h"
#include "input.h"
#include "levelmgr.h"
#include "objmgr.h"
#include "renderTools.h"

static void _gameInit();
static void _gameShutdown();
static void _gameDraw();
static void _gameUpdate(uint32_t milliseconds);


/// TESTING Ball manual color change:



static LevelDef _levelDefs[] = {
    {
        {/*{0,0}, {20,20}*/{0, 0}, {974, 600}},		// fieldBounds
    0xff0000FF,					// fieldColor
        0,							// numBalls
        0							// numFaces
    }
};
static Level* _curLevel = NULL;


/// @brief new def for house
//static LevelDef HouseLevel[] =
//{
//    {
//                {{0,0}, {0,0}/*{50, 50}, {974, 600}*/}
//
//    }
//};
//static P_LevelDef HouseDef{
//    {0}
//};



/// @brief Program Entry Point (WinMain)
/// @param hInstance 
/// @param hPrevInstance 
/// @param lpCmdLine 
/// @param nCmdShow 
/// @return 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char GAME_NAME[] = "Framework1";

    Application* app = appNew(hInstance, GAME_NAME, _gameDraw, _gameUpdate);

    if (app != NULL)
    {
        GLWindow* window = fwInitWindow(app);
        if (window != NULL)
        {
            UpdateWindowSize(app);
            _levelDefs->windowHeight = GetWindowHeight();
            _levelDefs->windowWidth = GetWindowWidth();
            _gameInit();

            bool running = true;
            while (running)
            {
                /// Get Current Window Size
                //p_fwGetWindowHeight(app);
                //p_fwGetWindowWidth(app);

                running = fwUpdateWindow(window);
            }

            _gameShutdown();
            fwShutdownWindow(window);
        }

        appDelete(app);
    }
}

/// @brief Initialize code to run at application startup
static void _gameInit()
{
    const uint32_t MAX_OBJECTS = 500;
    objMgrInit(MAX_OBJECTS);
    levelMgrInit();
    inputInit();
    _curLevel = levelMgrLoad(&_levelDefs[0]);

}

/// @brief Cleanup the game and free up any allocated resources
static void _gameShutdown()
{
    levelMgrUnload(_curLevel);

    levelMgrShutdown();
    inputShutdown();
    objMgrShutdown();
}

/// @brief Draw everything to the screen for current frame
static void _gameDraw()
{
    objMgrDraw();
}

/// @brief Perform updates for all game objects, for the elapsed duration
/// @param milliseconds 
static void _gameUpdate(uint32_t milliseconds)
{
    inputUpdate();
    objMgrUpdate(milliseconds);
}

    /// change levels

    /// accept input