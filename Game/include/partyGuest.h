#pragma once
#include "baseTypes.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>
#include "SOIL.h"
#include "Object.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct partyGuest_t
    {
        Object obj;
        Bounds2D bounds;
    } PartyGuest;

    void partyGuestInitTextures();
    PartyGuest* partyGuestNew();
    void partyGuestDelete(PartyGuest* pg);

    
#ifdef __cplusplus
}
#endif
