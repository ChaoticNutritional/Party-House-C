#include "grid.h"

#include <assert.h>
#include <stdlib.h>

#include "random.h"
#include "tile.h"

typedef struct grid_t
{
    //Object obj;

    Bounds2D bounds;       // overall bounds of the grid
    uint8_t rows;          // number of rows
    uint8_t cols;          // number of columns
    float tileWidth;       // width of each tile
    float tileHeight;      // height of each tile
    Tile** tiles;          // array of pointers to tiles
} Grid;



/// @brief 
/// @param bounds - panel bounds to create the grid within, passed by LevelDef->WindowHeight and Width
/// @return 
Grid* gridNew(Bounds2D bounds)
{
    Grid* grid = malloc(sizeof(Grid));
    assert(grid != NULL);
    grid->bounds = bounds;

    /// making sure the bounds are within reasonable limits (what are reasonable limits?...), level defs has the application window size. This will eventually contain a panel struct definition which is limited size within the app window.
    /// therefore, level needs to pass the panel bounds to the grid, which is the input parameter for this function
    float _width = bounds.botRight.x / 5.0f;
    assert((uint32_t)_width > 0 && (uint32_t)_width < 0xFFFFFF);
    float _height = bounds.botRight.y / 7.0f;
    assert((uint32_t)_height > 0 && (uint32_t)_height < 0xFFFFFF);

    grid->tileWidth = bounds.botRight.x / 5.0f;
    grid->tileHeight = bounds.botRight.y / 7.0f;

    grid->rows = (uint8_t)bounds.botRight.x / (uint8_t)grid->tileWidth;
    grid->cols = (uint8_t)bounds.botRight.y / (uint8_t)7;

    return grid;
}



/// @brief Should create and return a new grid
/// @param bounds - panel bounds to create the grid within, passed by LevelDef->WindowHeight and Width
/// @param rows - number of rows in the grid
/// @param cols - number of columns in the grid
/// @param color - a test color to fill all tiles with
/// @return 
Grid* gridNewTest(const Bounds2D bounds, uint8_t rows, uint8_t cols, uint32_t color)
{
    // ensure that rows and cols are greater than 0
    assert(cols > 0 && rows > 0);

    Grid* grid = malloc(sizeof(Grid));
    assert(grid != NULL);
    grid->bounds = bounds;

    // accounts for offset of bounds not at (0,0)
    const float _width = (bounds.botRight.x - bounds.topLeft.x);  // width in pixels
    const float _height = (bounds.botRight.y - bounds.topLeft.y); // height in pixels

    // calculate tile width and height based on height and width divided by desired number of rows and columns respectively
    grid->tileWidth = _width / (float)cols;
    grid->tileHeight = _height / (float)rows;

    const size_t N = (size_t)rows * (size_t)cols; // total number of tiles

    // allocate space a number of tile pointers equal to rows * cols
    grid->tiles = malloc(N * sizeof(*grid->tiles));    // use *grid->tiles to stay DRY, if I change how grid->tiles is defined
    assert(grid->tiles != NULL);

    // register the grid as an object
    //objInit(&grid->obj, NULL, bounds.topLeft, (Coord2D) { 0, 0 });

    // create new tiles and store them in the array of tiles
    for (size_t i= 0; i < N; ++i)
    {
        size_t r = i / cols;
        size_t c = i % cols;

        Coord2D tileTopCornerPos = {
            bounds.topLeft.x + (grid->tileWidth * (float)(c)),
            bounds.topLeft.y + (grid->tileHeight * (float)(r))
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

    //objDeinit(&grid->obj);
    free(grid);
}
