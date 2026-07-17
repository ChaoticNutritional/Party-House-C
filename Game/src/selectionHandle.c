#include <Windows.h>
#include <stdlib.h>
#include <assert.h>
#include <gl/GLU.h>

#include "baseTypes.h"
#include "Object.h"
#include "selectionHandle.h"
#include "tile.h"
#include "visibleObject.h"

// all of these values are based upon the layout of the PNG
static const char SELECTION_PAGE[] = "asset/selector.png";    // rename this probably for clarity

typedef struct handles_t
{
    Object      obj;
    VisibleObject visObj;
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


/// @brief Allocates & initializes a handles object
/// @param box 
/// @return 
Handles* handlesNew(Bounds2D box, Grid* grid)
{
    Handles* handles = malloc(sizeof(Handles));
    if (handles != NULL)
    {
        Coord2D center = boundsGetCenter(&box);
        Coord2D vel = { 0.0f, 0.0f };
        objInit(&handles->obj, &_handlesVtable, center, vel);
        visObjInit(&handles->visObj, SELECTION_PAGE, _handlesTexture);

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

