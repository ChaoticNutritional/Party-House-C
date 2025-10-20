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
#include "grid.h"
#include "selectionHandle.h"

/// Note to self:
/// implement a sound queue to kill sounds after 3 concurrent sounds exist
/// put in its own file, include header

typedef struct level_t
{
    const LevelDef* def;
    //p_LevelDef* p_def;

    Field* field;
    Handles* handles;

    Grid* grid;
    Tile* activeTile;
} Level;

// reference this in load new level? Get it from level def?
typedef struct inputContext_t
{
    Grid** grid;
    uint32_t* gridIndexPtr;
}inputContext;

typedef struct p_level_t
{
    P_LevelDef* def;
    //UIPanelDef* panels;
    //TextBoxDef* textboxes;
    Grid* grid;           // grid of cells for the party level

} P_Level;

static int32_t _soundId = SOUND_NOSOUND;
static int32_t _interactSound = SOUND_NOSOUND;

static void _levelMgrPlaySound(Ball* ball);

// /// DEMO KEYBIND CALLBACKS
// static void changeBallColorRed(void* ctx)
// {
//     Ball* ball = (Ball*)ctx;
//     ballSetColor(ball, 0xFF0000);
// }
//
// static void changeBallColorBlue(void* ctx)
// { 
//     Ball* ball = (Ball*)ctx;
//     ballSetColor(ball, 0x0000FF);
// }


/// @brief Initialize the level manager
void levelMgrInit()
{
    // faceInitTextures();
    

    _soundId = soundLoad("asset/beep.wav");
    _interactSound = soundLoad("asset/sounds/01_brnggg.ogg");

    // sets callback for collision 
    ballSetCollideCB(_levelMgrPlaySound);
}

/// @brief Shutdown the level manager
void levelMgrShutdown()
{
    ballClearCollideCB();
    soundUnload(_soundId);
    soundUnload(_interactSound);
}



/// @brief PARTY VERSION
/// @param levelDef 
/// @return 
Level* levelMgrLoadNew(const LevelDef* levelDef)
{
    Level* level = malloc(sizeof(Level));

    return level;
}


/// @brief 
/// @param level 
/// @return 
Level* levelUpdateCurrentTile(Level* level)
{
    assert(level->grid != NULL);

    level->activeTile = gridGetActiveTile(level->grid);
    return level;
}



/// Level Input Callbacks
/// local Grid* g because we KNOW that g is a grid, even if the compiler doesn't trust it outside the wrapper.
void onGridRight(void* ctx) {
    Grid* g = (Grid*)ctx;
    gridGoNextTile(g);
}
void onGridLeft(void* ctx) {
    Grid* g = (Grid*)ctx;
    gridGoLeftTile(g);
}
void onGridUp(void* ctx) {
    Grid* g = (Grid*)ctx;
    gridGoUpTile(g);
}
void onGridDown(void* ctx) {
    Grid* g = (Grid*)ctx;
    gridGoDownTile(g);
}


void onGridPrimary(void* ctx) {
    Grid* grid = (Grid*)ctx;
    Tile* tile = gridGetActiveTile(grid);  // current tile at press-time
    if (tile != NULL)
    {
        soundPlay(_interactSound);
        gridPrimaryInput(tile); // see tile section below
    }
}

void onGridSecondary(void* ctx) {
    Grid* grid = (Grid*)ctx;
    Tile* tile = gridGetActiveTile(grid);
    if (tile != NULL) 
    {
        soundPlay(_interactSound);
        gridSecondaryInput(tile);
    }
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

        level->grid = gridNewTest(levelDef->fieldBounds);

        level->activeTile = gridGetActiveTile(level->grid);
        level->handles = handlesNew(getATileBounds(level->grid), level->grid);
        
        // directional keybinds
        inputSetCallback(RIGHT_KEY, onGridRight, level->grid);
        inputSetCallback(LEFT_KEY, onGridLeft, level->grid);
        inputSetCallback(UP_KEY, onGridUp, level->grid);
        inputSetCallback(DOWN_KEY, onGridDown, level->grid);

        // primary/secondary keys
        inputSetCallback(Z_KEY, onGridPrimary, level->grid);
        inputSetCallback(X_KEY, onGridSecondary, level->grid);
    }
    return level;
}

/// @brief Unloads the level and frees up any assets associated
/// @param level 
void levelMgrUnload(Level* level)
{
    if (level != NULL) 
    {
        gridDelete(level->grid);
        handlesDelete(level->handles);
        fieldDelete(level->field);
    }
    free(level);
}

static void _levelMgrPlaySound(Ball* ball)
{
    //Bounds2D bounds = { {0.0f, 0.0f}, {20.0f, 20.0f} };
    soundPlay(_soundId);
}