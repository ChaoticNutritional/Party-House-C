#pragma once

#include "guest.h"

typedef struct tile_t Tile;

/// new and delete
Tile* tileNew(Guest* guest, void* interactCB);
Tile* tileNewTest(Coord2D topLeftStart, float width, float height);
void tileDelete(Tile* tile);

/// Callback:
typedef void (*InteractionCB)(Tile*);
void tileSetInteractionCB(Tile* tile, InteractionCB cb);
void tileInvokeInteraction(Tile* tile);
void tileInvokeCancel(Tile* tile);
void tileClearInteractionCB(Tile* tile);

/// Testing functions:
void tileSetColor(Tile* tile, uint32_t color);

//InteractionCB tileGetInteractionCB(Tile* tile);

/// Testing
void* getTileCallback(Tile* tile);
void* getTileCallback2(Tile* tile);