#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"

#include "tile.h"
#include <assert.h>

#include "object.h"

typedef struct tile_t
{
    Object obj; // hold space for an object
    Bounds2D tileBounds;    // bounds of tile
    uint32_t color;
    InteractionCB _interactionCB;
} Tile;

/// For future reference using character details
//typedef struct tile_t
//{
//    void* characterStruct;  // will contain sprite sheet and stats
//    void* interactionCB;    // can contain data from character
//    void* hoverCB;          // will perform some actions related to the level that the grid instance a part of
//} Tile;


/// callback definition
//static InteractionCB _interactionCB = NULL;


static InteractionCB _changeColor = NULL;

/// prototype
static void _tileDraw(Object* obj);
static void _tileUpdate(Object* obj, uint32_t milliseconds);

static ObjVtable _tileVtable = {
    _tileDraw,
    _tileUpdate
};

/// Ctor
Tile* tileNew(Guest* guest, void* interactCB)
{
	Tile* tile = malloc(sizeof(Tile));
    assert(tile != NULL);

	return tile;
}


/// @brief 
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

    // set the interaction callback
    tile->_interactionCB = NULL;

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



void tileCBTest(Tile* tile)
{
    tile->color = 0xFF0000FF; // set color to blue
}
void tileCBTest2(Tile* tile)
{
    tile->color = 0xFFFF0000; // set color to red
}


void tileSetInteractionCB(Tile* tile, InteractionCB cb)
{
    tile->_interactionCB = cb;
}

void tileInvokeInteraction(Tile* tile)
{
    if (tile != NULL/* && tile->_interactionCB != NULL*/)
    {
        tile->_interactionCB = tileCBTest;
        tile->_interactionCB(tile);
    }
}

void tileInvokeCancel(Tile* tile)
{
    if (tile != NULL/* && tile->_interactionCB != NULL*/)
    {
        tile->_interactionCB = tileCBTest2;
        tile->_interactionCB(tile);
    }
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
        filledVal);
}

/// @brief 
/// @param obj 
/// @param milliseconds 
void _tileUpdate(Object* obj, uint32_t milliseconds)
{
    objDefaultUpdate(obj, milliseconds);
}
