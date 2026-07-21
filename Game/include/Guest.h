#pragma once
#include "baseTypes.h"


#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct tile_t Tile;
    typedef void (*InteractionCB)(Tile*);
    
    typedef struct guest_t guest;
    
    typedef struct guest_data_t {
        const char* name;
        uint32_t testColor;       // stand-in for the sprite until real art exists

        // capped and signed
        int8_t popularity;
        int8_t money;
        int8_t rowdiness;
        bool star;

        InteractionCB onPrimaryInteract;
        InteractionCB onSecondaryInteract;
        
    } guest_data;
    
    typedef struct guest_t
    {
        const guest_data* data;
    } guest;
    
    guest* guestNew(const guest_data* data);
    void guestDelete(guest* c);
    
    
#ifdef __cplusplus
}
#endif