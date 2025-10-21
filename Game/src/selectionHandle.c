#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>
#include "SOIL.h"

#include "baseTypes.h"
#include "Object.h"
#include "selectionHandle.h"
#include "tile.h"

// all of these values are based upon the layout of the PNG
static const char SELECTION_PAGE[] = "asset/selector.png";    // rename this probably for clarity

typedef struct handles_t
{
    Object      obj;
    Coord2D     size;
    Grid*        grid;
} Handles;

static GLuint _handlesTexture = 0;

// the object vtable for all handless
static void _handlesUpdate(Object* obj, uint32_t milliseconds);
static void _handlesDraw(Object* obj);
static ObjVtable _handlesVtable = {
    _handlesDraw,
    _handlesUpdate
};

/// @brief one time initialization of textures
void handlesInitTextures()
{
    if (_handlesTexture == 0)
    {
        _handlesTexture = SOIL_load_OGL_texture(SELECTION_PAGE, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID,
            SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
        glBindTexture(GL_TEXTURE_2D, _handlesTexture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        assert(_handlesTexture != 0);
    }
}

/// @brief Allocates & initializes a handles object
/// @param box 
/// @return 
Handles* handlesNew(Bounds2D box, Grid* grid)
{
    handlesInitTextures();
    Handles* handles = malloc(sizeof(Handles));
    if (handles != NULL)
    {
        Coord2D center = boundsGetCenter(&box);
        Coord2D vel = { 0.0f, 0.0f };
        objInit(&handles->obj, &_handlesVtable, center, vel);

        // extract the dimensions from the bounding box for rendering
        handles->size = boundsGetDimensions(&box);
        handles->grid = grid;
    }

    return handles;
}

/// @brief Frees up resources from a handles object
/// @param handles 
void handlesDelete(Handles* handles)
{
    objDeinit(&handles->obj);

    free(handles);
}


/// @brief Object draw handler
/// @param obj 
static void _handlesDraw(Object* obj)
{
    Handles* handles = (Handles*)obj;

    obj->position = getTileBounds(gridGetActiveTile(handles->grid)).topLeft;

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_STRIP);
    {
        // calculate the bounding box
        GLfloat xPositionLeft = (obj->position.x);
        GLfloat xPositionRight = (obj->position.x + (handles->size.x));
        GLfloat yPositionTop = (obj->position.y);
        GLfloat yPositionBottom = (obj->position.y + (handles->size.y));

        // // calculate the starting uv... remember v of 0 is the bottom of the texture
        // GLfloat xTextureCoord = handles->character * uPerChar;
        // GLfloat yTextureCoord = (MOOD_COUNT - handles->mood) * vPerMood;

        // const float BG_DEPTH = -0.99f;
        const float OVERLAY_DEPTH = 0.99f;

        // draw the textured quad as a tristrip
        glColor4ub(0xFF, 0xFF, 0xFF, 0xFF);

        // TL
        glTexCoord2f(0, 1); // copied from faces but just use full image
        glVertex3f(xPositionLeft, yPositionTop, OVERLAY_DEPTH);

        // BL
        glTexCoord2f(0, 0);
        glVertex3f(xPositionLeft, yPositionBottom, OVERLAY_DEPTH);

        // TR
        glTexCoord2f(1, 1);
        glVertex3f(xPositionRight, yPositionTop, OVERLAY_DEPTH);

        // BR
        glTexCoord2f(1, 0);
        glVertex3f(xPositionRight, yPositionBottom, OVERLAY_DEPTH);

    }
    glEnd();
}

/// @brief Updates the character's mood every so often
/// @param obj 
/// @param milliseconds 
static void _handlesUpdate(Object* obj, uint32_t milliseconds)
{
    objDefaultUpdate(obj, milliseconds);

    Handles* handles = (Handles*)obj;
}

