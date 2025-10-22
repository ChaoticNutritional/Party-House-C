/// Intended to encapsulate objects with a texture to initialize their texture when they get loaded and manage lifetime

#pragma once
#include "baseTypes.h"
#include "object.h"
#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>
#include "SOIL.h"

#ifdef __cplusplus
extern "C" {
#endif

    // 'class' type
    typedef struct visibleObject_t VisibleObject;

    // "class" def
    typedef struct visibleObject_t
    {
        // VisibleObjVtable* vtable;
        GLuint instanceGLuint;
        const char* TEXTURE_SHEET_NAME;
        uint32_t framesCount;
        Bounds2D frameBounds;
        Object obj;
    } VisibleObject;

    void visObjInit(VisibleObject* visObj /*, VisibleObjVtable* vtable*/, const char FILE_NAME[], GLuint otherGLuint);
    void visObjDeinit(VisibleObject* obj);
    
    // available funcs
    VisibleObject* visibleObjectNew(const char textureName[]);
    void visibleObjectDelete(VisibleObject* visibleObject);
    void VisObjInitTexture(VisibleObject* visObj);
    
#ifdef __cplusplus
}
#endif