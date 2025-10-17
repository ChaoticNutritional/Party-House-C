#pragma once

#include "baseTypes.h"
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct grid_t Grid;
typedef struct tile_t Tile; // forward declaration of tile

typedef struct gridPayload_t
{
    Grid* grid;          // pointer to the grid
    uint32_t tileIdx; 
} GridPayload;

Grid* gridNew(Bounds2D bounds);

Grid* gridNewTest(const Bounds2D bounds);
void gridDelete(Grid* grid);


Bounds2D getATileBounds(const Grid* grid);
    
// Input Callbacks
void gridPrimaryInput(void* ctx);
void gridSecondaryInput(void* ctx);
void gridGoNextTile(Grid* grid); // test
void gridGoLeftTile(Grid* grid); // test
void gridGoUpTile(Grid* grid); // test
void gridGoDownTile(Grid* grid); // test


Tile* gridGetActiveTile(Grid* grid);
Tile* gridGetTile(GridPayload gp);    // might be nice to pass a struct known by both this and the level mgr...



#ifdef __cplusplus
}
#endif