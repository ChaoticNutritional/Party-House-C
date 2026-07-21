#pragma once
#ifdef __cplusplus
extern "C" {
#endif

typedef struct tile_t Tile;
typedef struct guest_t guest;

/// new and delete
Tile* tileNewTest(Coord2D topLeftStart, float width, float height);
Tile* tileNew(Coord2D topLeftStart, float width, float height);
void tileDelete(Tile* tile);

/// Callback:
void tileInvokeInteraction(Tile* tile);
void tileInvokeCancel(Tile* tile);

/// Guest occupancy: NULL guest means the tile is empty, and interaction is a no-op.
/// Tile borrows the guest pointer. It does not own or free it.
void tileSetGuest(Tile* tile, guest* g);
guest* tileGetGuest(Tile* tile);
void tileClearGuest(Tile* tile);

Bounds2D getTileBounds(Tile* tile);

/// Testing functions:
void tileSetColor(Tile* tile, uint32_t color);
    
#ifdef __cplusplus
}
#endif