#pragma once

#include "guest.h"

typedef struct tile_t Tile;

/// new and delete
Tile* tileNew(Guest* guest, void* interactCB);
Tile* tileNewTest(Coord2D topLeftStart, float width, float height);
void tileDelete(Tile* tile);

/// Callback:
typedef void (*InteractionCB)(Tile*);
void tileSetInteractionCB(InteractionCB cb);
void tileClearInteractionCB();

/// Testing functions:
void tileSetColor(Tile* tile, uint32_t color);

void tileFillRed(Tile* tile);
void tileFillGreen(Tile* tile);
void tileFillBlue(Tile* tile);