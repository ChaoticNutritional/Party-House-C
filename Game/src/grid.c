#include "grid.h"

#include <assert.h>
#include <stdlib.h>

#include "gameConstDefs.h"
#include "random.h"
#include "tile.h"

typedef struct grid_t
{
    Bounds2D bounds;       // overall bounds of the grid
    uint8_t rows;          // number of rows
    uint8_t cols;          // number of columns
    uint32_t tileWidth;       // width of each tile
    uint32_t tileHeight;      // height of each tile
    Tile** tiles;          // array of pointers to tiles
    uint32_t currentIdx;
    Tile* activeTile;       //
    InteractionCB* primaryInteraction;
    InteractionCB* secondaryInteraction;
} Grid;



/// @brief 
/// @param bounds - panel bounds to create the grid within, passed by LevelDef->WindowHeight and Width
/// @return 
Grid* gridNew(Bounds2D bounds)
{
    Grid* grid = malloc(sizeof(Grid));
    assert(grid != NULL);
    grid->bounds = bounds;

    grid->rows = HOUSE_CELLS_HIGH;
    grid->cols = HOUSE_CELLS_WIDE;

    grid->tileWidth = (uint32_t)bounds.botRight.x / HOUSE_CELLS_HIGH;
    grid->tileHeight = (uint32_t)bounds.botRight.y / HOUSE_CELLS_WIDE;

    return grid;
}

void initTiles(Grid* grid)
{
    const size_t N = (size_t)grid->rows * (size_t)grid->cols; // total number of tiles

    // allocate space a number of tile pointers equal to rows * cols
    grid->tiles = malloc(N * sizeof(*grid->tiles));    // use *grid->tiles to stay DRY, if I change how grid->tiles is defined
    assert(grid->tiles != NULL);
    
    Bounds2D bounds = grid->bounds;
    // create new tiles and store them in the array of tiles
    for (size_t i= 0; i < N; ++i)
    {
        size_t r = i / grid->cols;
        size_t c = i % grid->cols;

        Coord2D tileTopCornerPos = {
            bounds.topLeft.x + (float)(grid->tileWidth * (c)),
            bounds.topLeft.y + (float)(grid->tileHeight * (r))
        };

        Tile* t = tileNewTest(tileTopCornerPos, (uint16_t)grid->tileWidth, (uint16_t)grid->tileHeight);
        assert(t != NULL);
        // if t is NULL, then we should cleanup any created tiles before failing

        uint32_t randColor = randGetInt(0, 256);
        randColor += randGetInt(0, 256) << 8; // shift left by 8 bits to get green
        randColor += randGetInt(0, 256) << 16; // shift left by 16 bits to get red

        tileSetColor(t, randColor);
        grid->tiles[i] = t;
    }
}



/// @brief Should create and return a new grid
/// @param bounds - panel bounds to create the grid within, passed by LevelDef->WindowHeight and Width
/// @param rows - number of rows in the grid
/// @param cols - number of columns in the grid
/// @param color - a test color to fill all tiles with
/// @return 
Grid* gridNewTest(const Bounds2D bounds)
{
    // ensure that rows and cols are greater than 0
    //assert(cols > 0 && rows > 0);

    Grid* grid = malloc(sizeof(Grid));
    assert(grid != NULL);
    grid->bounds = bounds;
    grid->cols = HOUSE_CELLS_WIDE;
    grid->rows = HOUSE_CELLS_HIGH;
    grid->currentIdx = 0; // intentionally not 0 based!

    // accounts for offset of bounds not at (0,0) GRID HEIGHT AND WIDTH
    const uint32_t _width = (uint32_t)(bounds.botRight.x - bounds.topLeft.x);  // width in pixels
    const uint32_t _height = (uint32_t)(bounds.botRight.y - bounds.topLeft.y); // height in pixels

    // calculate tile width and height based on height and width divided by desired number of rows and columns respectively
    grid->tileWidth = _width / grid->cols;
    grid->tileHeight = _height / grid->rows;
    
    // init tiles:
    initTiles(grid);

    grid->activeTile = grid->tiles[grid->currentIdx];
    return grid;
}


/// @brief Delete a grid and free up any allocated resources
void gridDelete(Grid* grid)
{
    const size_t N = (size_t)grid->rows * (size_t)grid->cols;

    for (size_t i = 0; i < N; ++i)
    {
        tileDelete(grid->tiles[i]);
    }

    free(grid->tiles);
    free(grid);
}

Bounds2D getATileBounds(const Grid* grid)
{
    return getTileBounds(grid->tiles[0]);
}


/// @brief Primary input callback for grid interaction
/// @param ctx the tile providing our interact context
void gridPrimaryInput(void* ctx)
{
    Tile* tile =  (Tile*)ctx;
    tileInvokeInteraction(tile);
    //return getTileCallback(grid->activeTile);
}

/// @brief Secondary input callback for grid interaction
/// @param ctx the tile providing our interact context
void gridSecondaryInput(void* ctx)
{
    Tile* tile = (Tile*)ctx;
    tileInvokeCancel(tile);
    //return getTileCallback2(grid->activeTile);
}

/// @brief Move to the next tile in the grid, wrapping around if at the end
/// @param grid
void gridGoNextTile(Grid* grid)
{
    Tile* t = grid->activeTile;
    assert(t != NULL);

    const size_t N = (size_t)grid->rows * (size_t)grid->cols;

    grid->currentIdx = (grid->currentIdx + 1) % N;
    grid->activeTile = grid->tiles[grid->currentIdx];
}

/// @brief Move to previous tile in the grid, wrapping around if at the beginning
/// @param grid 
void gridGoLeftTile(Grid* grid)
{
    Tile* t = grid->activeTile;
    assert(t != NULL);

    const size_t N = (size_t)grid->rows * (size_t)grid->cols;
    grid->currentIdx = (grid->currentIdx + N - 1) % N;  // Fixed
    grid->activeTile = grid->tiles[grid->currentIdx];
}

/// @brief Move up one tile in the grid, wrapping around if at the top
/// @param grid
void gridGoUpTile(Grid* grid)
{
    Tile* t = grid->activeTile;
    assert(t != NULL);

    const size_t N = (size_t)grid->rows * (size_t)grid->cols;
    grid->currentIdx = (grid->currentIdx + N - grid->cols) % N;
    grid->activeTile = grid->tiles[grid->currentIdx];
}



/// @brief Move down one tile in the grid, wrapping around if at the bottom
/// @param grid
void gridGoDownTile(Grid* grid)
{
    Tile* t = grid->activeTile;
    assert(t != NULL);

    const size_t N = (size_t)grid->rows * (size_t)grid->cols;
    grid->currentIdx = (grid->currentIdx + HOUSE_CELLS_WIDE) % N;
    grid->activeTile = grid->tiles[grid->currentIdx];
}



/// @brief return the currently active tile in the grid
Tile* gridGetActiveTile(Grid* grid)
{
    assert(grid->tiles[grid->currentIdx] != NULL);
    return grid->activeTile;
}



/// @brief return some tile from grid
/// @param gp - grid payload contains grid and tile index
/// @return 
Tile* gridGetTile(GridPayload gp)
{
    return gp.grid->tiles[gp.tileIdx];
}
