#include <stdlib.h>

#include "baseTypes.h"
#include "shape.h"
#include "rect.h"
#include "random.h"
#include "field.h"
#include "Object.h"

typedef struct rect_t {
	Object obj;

	Bounds2D bounds;
	uint32_t color;
} rect;

// the object vtable for all rects
static void _rectUpdate(Object* obj, uint32_t milliseconds);
static void _rectDraw(Object* obj);
static ObjVtable _rectVtable = {
	_rectDraw,
	_rectUpdate
};

// storage for a collision callback
static rectCollideCB _rectCollideCB = NULL;

// other private methods
static void _rectSetRandomColor(rect* rect);
static void _rectDoCollisions(rect* rect);
static void _rectCollideField(rect* rect);
static void _rectTriggerCollideCB(rect* rect);

/// @brief Sets the callback
/// @param cb 
void rectSetCollideCB(rectCollideCB cb)
{
	_rectCollideCB = cb;
}

/// @brief Clears the callback
void rectClearCollideCB()
{
	_rectCollideCB = NULL;
}

/// @brief Instantiate and initialize a rect object
/// @param bounds 
/// @return 
rect* rectNew(Bounds2D bounds)
{
	const float MAX_VEL = 5.0f;
	const float MIN_RADIUS = 10.0f;
	const float MAX_RADIUS = 50.0f;

	rect* rect = malloc(sizeof(rect));
	if (rect != NULL)
	{
		Coord2D pos = boundsGetCenter(&bounds);
		Coord2D vel = { randGetFloat(-MAX_VEL, MAX_VEL), randGetFloat(-MAX_VEL, MAX_VEL) };
		objInit(&rect->obj, &_rectVtable, pos, vel);

		rect->bounds = bounds;
		rect->radius = randGetFloat(MIN_RADIUS, MAX_RADIUS);
		_rectSetRandomColor(rect);
	}
	return rect;
}

/// @brief Free up any resources pertaining to a rect object
/// @param rect 
void rectDelete(rect* rect)
{
	objDeinit(&rect->obj);

	free(rect);
}

/// @brief Sets the color of the rect to a random RGB
/// @param rect 
static void _rectSetRandomColor(rect* rect)
{
	rect->color = randGetInt(0, 256);
	rect->color += randGetInt(0, 256) << 8;
	rect->color += randGetInt(0, 256) << 16;
}

/// @brief Mainly process rect collisions
/// @param obj 
/// @param milliseconds 
static void _rectUpdate(Object* obj, uint32_t milliseconds)
{
	objDefaultUpdate(obj, milliseconds);

	_rectDoCollisions((rect*)obj);
}

static void _rectDraw(Object* obj)
{
	rect* rect = (rect*)obj;

	uint8_t red = (uint8_t)((rect->color >> 16) & 0xFF);
	uint8_t green = (uint8_t)((rect->color >> 8) & 0xFF);
	uint8_t blue = (uint8_t)((rect->color >> 0) & 0xFF);
	bool filledVal = true;

	shapeDrawRect(rect->bounds.topLeft.x, rect->bounds.topLeft.y, rect->bounds.botRight.x, rect->bounds.botRight.y, red, green, blue, filledVal);
}

static void _rectDoCollisions(rect* rect)
{
	_rectCollideField(rect);
}

static void _rectCollideField(rect* rect)
{
	float rightSide = rect->bounds.botRight.x;
	float leftSide = rect->bounds.topLeft.x;
	float topSide = rect->bounds.topLeft.y;
	float bottomSide = rect->bounds.botRight.y;

	if (rect->obj.position.x - rect->radius <= leftSide)
	{
		rect->obj.velocity.x = -rect->obj.velocity.x;
		rect->obj.position.x = leftSide + rect->radius;
		_rectSetRandomColor(rect);
		_rectTriggerCollideCB(rect);
	}
	if (rect->obj.position.x + rect->radius >= rightSide)
	{
		rect->obj.velocity.x = -rect->obj.velocity.x;
		rect->obj.position.x = rightSide - rect->radius;
		_rectSetRandomColor(rect);
		_rectTriggerCollideCB(rect);
	}
	if (rect->obj.position.y + rect->radius >= bottomSide)
	{
		rect->obj.velocity.y = -rect->obj.velocity.y;
		rect->obj.position.y = bottomSide - rect->radius;
		_rectSetRandomColor(rect);
		_rectTriggerCollideCB(rect);
	}
	if (rect->obj.position.y - rect->radius <= topSide)
	{
		rect->obj.velocity.y = -rect->obj.velocity.y;
		rect->obj.position.y = topSide + rect->radius;
		_rectSetRandomColor(rect);
		_rectTriggerCollideCB(rect);
	}
}

/// @brief Triggers the callback, if one is registered
/// @param rect 
static void _rectTriggerCollideCB(rect* rect)
{
	if (_rectCollideCB != NULL)
	{
		_rectCollideCB(rect);
	}
}

