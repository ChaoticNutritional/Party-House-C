#include "grid.h"

#include <assert.h>
#include <stdlib.h>

#include "gameConstDefs.h"
#include "random.h"
#include "tile.h"

typedef struct grid_t
{
    uint32_t houseSize;    // actual number of tiles
    Bounds2D bounds;       // overall bounds of the grid
    uint8_t rows;          // number of rows
    uint8_t cols;          // number of columns
    uint32_t tileWidth;    // width of each tile
    uint32_t tileHeight;   // height of each tile
    Tile** tiles;          // compact array of tile pointers (size = houseSize)
    uint32_t currentIdx;
    Tile* activeTile;
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

    // grid->houseSize = 7;
    grid->bounds = bounds;

    grid->rows = HOUSE_CELLS_HIGH;
    grid->cols = HOUSE_CELLS_WIDE;

    grid->tileWidth = (uint32_t)bounds.botRight.x / HOUSE_CELLS_HIGH;
    grid->tileHeight = (uint32_t)bounds.botRight.y / HOUSE_CELLS_WIDE;

    return grid;
}

/// @brief helper function to initialize tiles
/// @param grid - the grid
void initTiles(Grid* grid)
{
    // Allocate compact array (only houseSize tiles, no gaps)
    grid->tiles = malloc(grid->houseSize * sizeof(*grid->tiles));
    assert(grid->tiles != NULL);

    Bounds2D bounds = grid->bounds;

    // Create tiles sequentially (left-aligned)
    for (size_t i = 0; i < grid->houseSize; ++i)
    {
        size_t r = i / grid->cols;
        size_t c = i % grid->cols;

        Coord2D tileTopCornerPos = {
            bounds.topLeft.x + (float)(grid->tileWidth * c),
            bounds.topLeft.y + (float)(grid->tileHeight * r)
        };

        Tile* t = tileNewTest(tileTopCornerPos, (uint16_t)grid->tileWidth, (uint16_t)grid->tileHeight);
        assert(t != NULL);

        uint32_t randColor = randGetInt(0, 256);
        randColor += randGetInt(0, 256) << 8;
        randColor += randGetInt(0, 256) << 16;

        tileSetColor(t, randColor);
        grid->tiles[i] = t;
    }
}



/// @brief Should create and return a new grid, currently not supporting customizable grid shapes after construction.
/// @param bounds - panel bounds to create the grid within, passed by LevelDef->WindowHeight and Width
/// @param rows - number of rows in the grid
/// @param cols - number of columns in the grid
/// @param color - a test color to fill all tiles with
/// @return 
Grid* gridInit(const Bounds2D bounds)
{
    // ensure that rows and cols are greater than 0
    //assert(cols > 0 && rows > 0);

    Grid* grid = malloc(sizeof(Grid));
    assert(grid != NULL);

    grid->houseSize = 14; 
    grid->bounds = bounds;
    grid->cols = HOUSE_CELLS_WIDE;
    grid->rows = HOUSE_CELLS_HIGH;
    grid->currentIdx = 0;

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
    for (size_t i = 0; i < grid->houseSize; ++i)
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


// ============================================================================
// Navigation Helper (inline for simplicity)
// ============================================================================

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))


/// @brief Primary input callback for grid interaction
/// @param ctx the tile providing our interact context
void gridPrimaryInput(void* ctx)
{
    Tile* tile =  (Tile*)ctx;
    tileInvokeInteraction(tile);
}

/// @brief Secondary input callback for grid interaction
/// @param ctx the tile providing our interact context
void gridSecondaryInput(void* ctx)
{
    Tile* tile = (Tile*)ctx;
    tileInvokeCancel(tile);
}

/// @brief Move to the next tile (RIGHT), wrapping within current row
/// @param ctx = pointer to grid
void gridGoNextTile(void* ctx)
{
    Grid* grid = (Grid*)ctx;
    uint32_t row = grid->currentIdx / grid->cols;
    uint32_t col = grid->currentIdx % grid->cols;
    uint32_t rowStartIdx = row * grid->cols;
    uint32_t rowEnd = MIN(rowStartIdx + grid->cols - 1, grid->houseSize - 1);

    // Move right, wrap to row start if at row end
    grid->currentIdx = (grid->currentIdx >= rowEnd) ? rowStartIdx : grid->currentIdx + 1;
    grid->activeTile = grid->tiles[grid->currentIdx];
}

/// @brief Move to previous tile (LEFT), wrapping within current row
/// @param ctx = pointer to grid
void gridGoPrevTile(void* ctx)
{
    Grid* grid = (Grid*)ctx;
    uint32_t row = grid->currentIdx / grid->cols;
    uint32_t rowStartIdx = row * grid->cols;
    uint32_t rowEnd = MIN(rowStartIdx + grid->cols - 1, grid->houseSize - 1);

    // Move left, wrap to row end if at row start
    grid->currentIdx = (grid->currentIdx <= rowStartIdx) ? rowEnd : grid->currentIdx - 1;
    grid->activeTile = grid->tiles[grid->currentIdx];
}

/// @brief Move up one row, clamping to incomplete rows
/// @param ctx = pointer to grid
void gridGoUpTile(void* ctx)
{
    Grid* grid = (Grid*)ctx;
    uint32_t currentRow = grid->currentIdx / grid->cols;
    uint32_t currentCol = grid->currentIdx % grid->cols;

    // Calculate the actual last row with tiles (not grid->rows - 1, which may be empty)
    uint32_t lastRowWithTiles = (grid->houseSize - 1) / grid->cols;

    // Wrap to last row with tiles if at top
    uint32_t targetRow = (currentRow == 0) ? lastRowWithTiles : (currentRow - 1);
    uint32_t targetIdx = targetRow * grid->cols + currentCol;

    // If target position doesn't exist, clamp to last tile in target row
    if (targetIdx >= grid->houseSize) {
        // Find the last tile in the target row
        uint32_t rowStartIdx = targetRow * grid->cols;
        uint32_t rowEnd = MIN(rowStartIdx + grid->cols - 1, grid->houseSize - 1);
        targetIdx = rowEnd;
    }

    grid->currentIdx = targetIdx;
    grid->activeTile = grid->tiles[grid->currentIdx];
}



/// @brief Move down one row, clamping to incomplete rows
/// @param ctx = pointer to grid
void gridGoDownTile(void* ctx)
{
    Grid* grid = (Grid*)ctx;
    uint32_t currentRow = grid->currentIdx / grid->cols;
    uint32_t currentCol = grid->currentIdx % grid->cols;

    // Calculate the actual last row with tiles
    uint32_t lastRowWithTiles = (grid->houseSize - 1) / grid->cols;

    // Wrap to top row if at last row with tiles
    uint32_t targetRow = (currentRow >= lastRowWithTiles) ? 0 : (currentRow + 1);
    uint32_t targetIdx = targetRow * grid->cols + currentCol;

    // If target position doesn't exist, clamp to last tile in target row
    if (targetIdx >= grid->houseSize) {
        // Find the last tile in the target row
        uint32_t rowStartIdx = targetRow * grid->cols;
        uint32_t rowEnd = MIN(rowStartIdx + grid->cols - 1, grid->houseSize - 1);
        targetIdx = rowEnd;
    }

    grid->currentIdx = targetIdx;
    grid->activeTile = grid->tiles[grid->currentIdx];
}



/// @brief return the currently active tile in the grid
Tile* gridGetActiveTile(const Grid* grid)
{
    return grid->activeTile;
}



/// @brief return some tile from grid
/// @param gp - grid payload contains grid and tile index
/// @return 
Tile* gridGetTile(const GridPayload gp)
{
    return gp.grid->tiles[gp.tileIdx];
}
