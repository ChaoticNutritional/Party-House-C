#pragma once
#include "baseTypes.h"
#include "object.h"
#include "input.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct leveldef_t {
        Bounds2D fieldBounds;
        uint32_t fieldColor;
        uint32_t numBalls;
        uint32_t numFaces;
        uint32_t windowHeight;
        uint32_t windowWidth;
    } LevelDef;

    typedef struct p_levelDef_t
    {
        uint32_t numCells;      // array of cells
        uint32_t windowHeight;
        uint32_t windowWidth;
        Bounds2D gameSpace;     // space that is useable for the game interaction system

        // callbacks for each input on a given level?
    } P_LevelDef;

    typedef struct textbox_t
    {
        Object obj;
        char* text;
        char* font;
        uint32_t color;
        Coord2D relativePosition;
        uint32_t fontSize;
    } TextBoxDef;

    typedef struct panel_t
    {
        Coord2D position;
        Bounds2D bounds;
    } UIPanelDef;

    typedef struct level_t Level;
    typedef struct panel_t Panel;
    typedef struct textbox_t TextBox;
    typedef struct p_level_t PartyLevel;

    void levelMgrInit();
    void levelMgrShutdown();
    Level* levelMgrLoad(const LevelDef* levelDef);
    void levelMgrUnload(Level* level);

    //void levelMgrBindInputCB(Level* level, GameKey key, )

#ifdef __cplusplus
}
#endif