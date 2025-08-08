#pragma once
#include "baseTypes.h"

#ifdef __cplusplus
extern "C" {
#endif


// object "virtual" functions
typedef struct object_t Object;
typedef void (*ObjDrawFunc)(Object*);
typedef void (*ObjUpdateFunc)(Object*, uint32_t);

typedef struct object_vtable_t {
    ObjDrawFunc     draw;
    ObjUpdateFunc   update;
} ObjVtable;

typedef struct object_t {
    ObjVtable*      vtable;
    Coord2D         position;
    Coord2D         velocity;
    Bounds2D        bounds;
} Object;

typedef void (*ObjRegistrationFunc)(Object*);

// class-wide registration methods
void objEnableRegistration(ObjRegistrationFunc registerFunc, ObjRegistrationFunc deregisterFunc);
void objDisableRegistration();

// object API
void objInit(Object* obj, ObjVtable* vtable, Coord2D pos, Coord2D vel);
void objDeinit(Object* obj);
void objDraw(Object* obj);
void objUpdate(Object* obj, uint32_t milliseconds);

// default update implementation that just moves at the current velocity
void objDefaultUpdate(Object* obj, uint32_t milliseconds);

#ifdef __cplusplus
}
#endif