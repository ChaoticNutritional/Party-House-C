#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"

#include "tile.h"
#include "Guest.h"
#include <assert.h>

#include "object.h"

typedef struct tile_t
{
    Object obj; // hold space for an object
    Bounds2D tileBounds;    // bounds of tile
    uint32_t color;
    guest* occupant;    // NULL = empty tile; borrowed, not owned/freed by Tile
} Tile;

/// prototype
static void _tileDraw(Object* obj);
static void _tileUpdate(Object* obj, uint32_t milliseconds);

static ObjVtable _tileVtable = {
    _tileDraw,
    _tileUpdate
};

/// @brief temp usage to create a tile
/// @param topLeftStart - screen coordinate of the top-left corner of the tile
/// @param width
/// @param height 
/// @return 
Tile* tileNewTest(Coord2D topLeftStart, float width, float height)
{
    Tile* tile = malloc(sizeof(Tile));
    assert(tile != NULL);

    // register object... Or do we? @return to this later
    objInit(&tile->obj, &_tileVtable, topLeftStart, (Coord2D){0, 0});

    // set the tile bounds
    tile->tileBounds.topLeft = topLeftStart;
    tile->tileBounds.botRight.x = topLeftStart.x + width;
    tile->tileBounds.botRight.y = topLeftStart.y + height;

    // no guest occupies this tile yet
    tile->occupant = NULL;

    // set the color to white by default
    tile->color = 0xFFFFFFFF;

    return tile;
}


/// @brief initialize a tile
/// @param topLeftStart - allows for adjustable bounds within a grid
/// @param width 
/// @param height 
/// @return 
Tile* tileNew(Coord2D topLeftStart, float width, float height)
{
    Tile* tile = malloc(sizeof(Tile));
    assert(tile != NULL);

    // register object... Or do we? @return to this later
    objInit(&tile->obj, &_tileVtable, topLeftStart, (Coord2D){0, 0});

    // set the tile bounds
    tile->tileBounds.topLeft = topLeftStart;
    tile->tileBounds.botRight.x = topLeftStart.x + width;
    tile->tileBounds.botRight.y = topLeftStart.y + height;

    // no guest occupies this tile yet
    tile->occupant = NULL;

    // set the color to white by default
    tile->color = 0xFFFFFFFF;

    return tile;
}

/// Dtor
void tileDelete(Tile* tile)
{
    objDeinit(&tile->obj);
    free(tile);
}


void tileInvokeInteraction(Tile* tile)
{
    if (tile != NULL && tile->occupant != NULL && tile->occupant->data != NULL
        && tile->occupant->data->onPrimaryInteract != NULL)
    {
        tile->occupant->data->onPrimaryInteract(tile);
    }
}

void tileInvokeCancel(Tile* tile)
{
    if (tile != NULL && tile->occupant != NULL && tile->occupant->data != NULL
        && tile->occupant->data->onSecondaryInteract != NULL)
    {
        tile->occupant->data->onSecondaryInteract(tile);
    }
}

void tileSetGuest(Tile* tile, guest* g)
{
    tile->occupant = g;
}

guest* tileGetGuest(Tile* tile)
{
    return tile->occupant;
}

void tileClearGuest(Tile* tile)
{
    tile->occupant = NULL;
}

/// easier getter
Bounds2D getTileBounds(Tile* tile)
{
    return tile->tileBounds;
}


/// @brief Set color of tile as testing
/// @param tile 
/// @param color 
void tileSetColor(Tile* tile, uint32_t color)
{
    tile->color = color;
}


/// @brief 
/// @param obj 
void _tileDraw(Object* obj)
{
    // c-style cast of obj to a tile;
    Tile* tile = (Tile*)obj;

    // gather color data into params
    uint8_t red = (uint8_t)((tile->color >> 16) & 0xFF);
    uint8_t green = (uint8_t)((tile->color >> 8) & 0xFF);
    uint8_t blue = (uint8_t)((tile->color >> 0) & 0xFF);
    bool filledVal = true;

    // call shape draw functions
    shapeDrawRect(obj->position.x, 
        obj->position.y, 
        tile->tileBounds.botRight.x, 
        tile->tileBounds.botRight.y, 
        red, green, blue, 
        false);
}

/// @brief 
/// @param obj 
/// @param milliseconds 
void _tileUpdate(Object* obj, uint32_t milliseconds)
{
    objDefaultUpdate(obj, milliseconds);
}
