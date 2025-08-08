#pragma once
#include "baseTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct rect_t rect;

    typedef void (*rectCollideCB)(rect*);
    void rectSetCollideCB(rectCollideCB cb);
    void rectClearCollideCB();

    rect* rectNew(Bounds2D bounds);
    void rectDelete(rect* rect);

#ifdef __cplusplus
}
#endif