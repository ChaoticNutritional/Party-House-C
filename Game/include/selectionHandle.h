#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include "grid.h"

    typedef struct handles_t Handles;

    // void handlesInitTextures(void);
    Handles* handlesNew(Bounds2D box, Grid* grid);
    void handlesDelete(Handles* handles);
    
#ifdef __cplusplus
}
#endif