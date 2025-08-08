#pragma once

#include "tile.h"
#include "levelmgr.h"

typedef struct grid_t
{
    uint8_t width;
    uint8_t height;
    Tile* firstTile;
    Tile* lastTile;
    P_LevelDef* owningLevel;
} grid;