#include <Windows.h>
#include <stdlib.h>
#include <gl/GLU.h>
#include <assert.h>
#include "baseTypes.h"
#include "levelmgr.h"
#include "field.h"
#include "ball.h"
#include "face.h"
#include "sound.h"
#include "input.h"

typedef struct level_t
{
    const LevelDef* def;
    //p_LevelDef* p_def;

    Field* field;
    Ball** balls;
    Face** faces;

    Ball* justBall;
} Level;

typedef struct p_level_t
{
    P_LevelDef* def;
    UIPanelDef* panels;
    TextBoxDef* textboxes;
} P_Level;

static int32_t _soundId = SOUND_NOSOUND;

static void _levelMgrPlaySound(Ball* ball);
static void _crashTheGame(void);


/// DEMO KEYBIND CALLBACKS
static void changeBallColorRed(void* ctx)
{
    Ball* ball = (Ball*)ctx;
    ballSetColor(ball, 0xFF0000);
}

static void changeBallColorBlue(void* ctx)
{
    Ball* ball = (Ball*)ctx;
    ballSetColor(ball, 0x0000FF);
}

/// @brief Initialize the level manager
void levelMgrInit()
{
    faceInitTextures();

    _soundId = soundLoad("asset/beep.wav");

    // sets callback for collision 
    ballSetCollideCB(_levelMgrPlaySound);
}

/// @brief Shutdown the level manager
void levelMgrShutdown()
{
    ballClearCollideCB();
    soundUnload(_soundId);
}



/// @brief PARTY VERSION
/// @param levelDef 
/// @return 
Level* levelMgrLoadNew(const LevelDef* levelDef)
{
    Level* level = malloc(sizeof(Level));

    return level;
}



/// @brief Loads the level and all required objects/assets
/// @param levelDef 
/// @return 
Level* levelMgrLoad(const LevelDef* levelDef)
{
    Level* level = malloc(sizeof(Level));
    if (level != NULL)
    {
        level->def = levelDef;

        // the field provides the boundaries of the scene & encloses the faces & balls
        level->field = fieldNew(levelDef->fieldBounds, levelDef->fieldColor);

        /*level->justBall = malloc(sizeof(Ball*));*/
        Bounds2D windowBounds = { {0.0f, 0.0f}, { (float)levelDef->windowWidth, (float)levelDef->windowHeight } };
        level->justBall = ballNew(windowBounds);

        inputSetCallback(Z_KEY, changeBallColorBlue, level->justBall);
        inputSetCallback(X_KEY, changeBallColorRed, level->justBall);
    }
    return level;
}

/// @brief Unloads the level and frees up any assets associated
/// @param level 
void levelMgrUnload(Level* level)
{
    if (level != NULL) 
    {
        for (uint32_t i = 0; i < level->def->numFaces * level->def->numFaces; ++i)
        {
            faceDelete(level->faces[i]);
        }
        free(level->faces);

        for (uint32_t i = 0; i < level->def->numBalls; ++i)
        {
            ballDelete(level->balls[i]);
        }
        free(level->balls);

        fieldDelete(level->field);
    }
    free(level);
}

static void _levelMgrPlaySound(Ball* ball)
{
    Bounds2D bounds = { {0.0f, 0.0f}, {20.0f, 20.0f} };
    soundPlay(_soundId);
}

void _crashTheGame(void)
{
    assert(0);
}