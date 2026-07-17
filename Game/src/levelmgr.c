#include <Windows.h>
#include <stdlib.h>
#include <gl/GLU.h>
#include <assert.h>
#include "baseTypes.h"
#include "levelmgr.h"
#include "field.h"

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
    
    /* previous chunk
    // UIPanelDef* panels;
    // TextBoxDef* textboxes;
    // grid of cells for the party level
    */  
    
    Grid* grid;           
} P_Level;

static int32_t _soundId = SOUND_NOSOUND;
static int32_t _interactSound = SOUND_NOSOUND;

// static void _levelMgrPlaySound(Ball* ball);

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
void levelMgrInit(void)
{
    // faceInitTextures();
    

    _soundId = soundLoad("asset/beep.wav");
    _interactSound = soundLoad("asset/sounds/01_brnggg.ogg");

    // sets callback for collision 
    // ballSetCollideCB(_levelMgrPlaySound);
}


/// @brief Shutdown the level manager
void levelMgrShutdown(void)
{
    // ballClearCollideCB();
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


/// It feels weird that these are in this file... I'd like to move them eventually ///
/// @brief Level wrappers for primary/secondary inputs. Incorporate the tile's behavior and also call sound effect
void onGridPrimary(void* ctx) {
    Grid* grid = (Grid*)ctx;
    Tile* tile = gridGetActiveTile(grid);
    if (tile != NULL)
    {
        soundPlay(_interactSound);
        gridPrimaryInput(tile);
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

//////////////////////////////////////////////////////////////////////////////////////////////


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

        level->grid = gridInit(levelDef->fieldBounds);

        level->activeTile = gridGetActiveTile(level->grid);
        level->handles = handlesNew(getATileBounds(level->grid), level->grid);
        
        // directional keybinds
        inputSetCallback(RIGHT_KEY, gridGoNextTile, level->grid);
        inputSetCallback(LEFT_KEY, gridGoPrevTile, level->grid);
        inputSetCallback(UP_KEY, gridGoUpTile, level->grid);
        inputSetCallback(DOWN_KEY, gridGoDownTile, level->grid);

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