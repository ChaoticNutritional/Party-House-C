#pragma once

#include "baseTypes.h"
#include "object.h"

#ifdef __cplusplus
extern "C" {
#endif



typedef struct grid_t Grid;

Grid* gridNew(Bounds2D bounds);

Grid* gridNewTest(const Bounds2D bounds, uint8_t rows, uint8_t cols, uint32_t color);
void gridDelete(Grid* grid);


static void _gridUpdate(Object* obj, uint32_t milliseconds);

#ifdef __cplusplus
}
#endif