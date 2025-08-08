#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"

#include "tile.h"
#include <assert.h>

#include "object.h"

#define TILE_TESTING

#ifdef TILE_TESTING
typedef struct tile_t
{
    Object obj; // hold space for an object
    Bounds2D tileBounds;    // bounds of tile
 
    void* setColor;
    long color;
    InteractionCB _interactionCB;
} Tile;

#else
typedef struct tile_t
{
    void* characterStruct;  // will contain sprite sheet and stats
    void* interactionCB;    // can contain data from character
    void* hoverCB;          // will perform some actions related to the level that the grid instance a part of
} Tile;
#endif


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

#ifndef TILE_TESTING
    /// empty parameter: NO GUEST
    if (guest == NULL)
    {
        tile->characterStruct = NULL;
        tile->interactionCB = interactCB;
        tile->hoverCB = NULL;
    }

    /// non empty parameter: HAS GUEST
    //else
    //{
    //    
    //}
#else
    tile->_interactionCB = interactCB;
#endif

	return tile;
}

/// Dtor
void tileDelete(Tile* tile)
{

}

/// TEST FUNCTIONS
#ifdef TILE_TESTING
void tileSetColor(Tile* tile, long color)
{
    //tile->color = color;
}

void tileFillRed(Tile* tile)
{

}

void tileFillGreen(Tile* tile)
{
}

void tileFillBlue(Tile* tile)
{
}
#endif


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
        obj->position.x + tile->tileBounds.botRight.x, 
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
