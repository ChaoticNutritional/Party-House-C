#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct tile_t Tile;

/// new and delete
Tile* tileNewTest(Coord2D topLeftStart, float width, float height);
void tileDelete(Tile* tile);

/// Callback:
typedef void (*InteractionCB)(Tile*);
void tileSetInteractionCB(Tile* tile, InteractionCB cb);
void tileInvokeInteraction(Tile* tile);
void tileInvokeCancel(Tile* tile);
void tileClearInteractionCB(Tile* tile);

Bounds2D getTileBounds(Tile* tile);

/// Testing functions:
void tileSetColor(Tile* tile, uint32_t color);
    
#ifdef __cplusplus
}
#endif