#ifndef _GAMEDEFINE_H
#define _GAMEDEFINE_H

#include "Existence.h"

#include <list>

using namespace std;

enum TETROMINO_TYPE
{
	SHAPE_TYPE_I = 0,
	SHAPE_TYPE_J,
	SHAPE_TYPE_L,
	SHAPE_TYPE_O,
	SHAPE_TYPE_S,
	SHAPE_TYPE_T,
	SHAPE_TYPE_Z,
	MAX_SHAPE_TYPE_NUM,
};

enum ROTATION_TYPE	// In clockwise
{
	ROT_0 = 0,
	ROT_90,
	ROT_180,
	ROT_270,
};

// 2D坐标
// TODO: 大规模应用，否则摒弃
struct Point2D
{
	int x, y;

	Point2D(int _x, int _y) : x(_x), y(_y) {}

	Point2D& operator= (const Point2D& rhs)
	{
		x = rhs.x;
		y = rhs.y;

		return *this;
	}

	Point2D operator+ (const Point2D& rhs)
	{
		return Point2D(x + rhs.x, y + rhs.y);
	}

	Point2D& operator+= (const Point2D& rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}
};

#endif
