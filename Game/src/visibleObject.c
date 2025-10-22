#include "visibleObject.h"

// fwd
void _baseLoadTexture(const VisibleObject* obj);
// object def
static void _visDraw(Object* obj);
static void _visUpdate(Object* obj, uint32_t milliseconds);
static ObjVtable _visObjVtable =
{
    _visDraw,
    _visUpdate
};


/// Note: loads texture on initialization of this object
void visObjInit(VisibleObject* visObj, const char FILE_NAME[], GLuint otherGLuint)
{
    // visObj->vtable = vtable;
    visObj->TEXTURE_SHEET_NAME = FILE_NAME;
    visObj->instanceGLuint = otherGLuint;
    
    _baseLoadTexture(visObj);
}


void visObjDeinit(VisibleObject* obj)
{
    
}


// def
void _baseLoadTexture(const VisibleObject* obj)
{
    GLuint textureLoaded = obj->instanceGLuint;
    if (textureLoaded == 0)
    {
        textureLoaded = SOIL_load_OGL_texture(obj->TEXTURE_SHEET_NAME, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        glBindTexture(GL_TEXTURE_2D, textureLoaded);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        assert(textureLoaded != 0);
    }
}

void _visDraw(Object* obj)
{
}

void _visUpdate(Object* obj, uint32_t milliseconds)
{
    objDefaultUpdate(obj, milliseconds);
}
