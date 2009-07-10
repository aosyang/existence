#ifndef _POINT3_H
#define _POINT3_H

#include <math.h>

class Point3
{
public:
	int x, y, z;

	Point3() { x = y = z = 0; }
	Point3(int _x, int _y, int _z)
	{
		x = _x; y = _y; z = _z;
	}

	Point3& operator=(const Point3& rhs)
	{
		this->x = rhs.x;
		this->y = rhs.y;
		this->z = rhs.z;

		return *this;
	}

	bool operator==(const Point3& rhs) const
	{
		return (this->x==rhs.x && this->y==rhs.y && this->z==rhs.z);
	}

	bool operator==(const Point3& rhs)
	{
		return (this->x==rhs.x && this->y==rhs.y && this->z==rhs.z);
	}

	bool operator!=(const Point3& rhs) const
	{
		return !(this->x==rhs.x && this->y==rhs.y && this->z==rhs.z);
	}

	bool operator!=(const Point3& rhs)
	{
		return !(this->x==rhs.x && this->y==rhs.y && this->z==rhs.z);
	}

	Point3 operator-(const Point3& rhs)
	{
		return Point3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	Point3 operator-(const Point3& rhs) const
	{
		return Point3(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}

	int SquaredLength()
	{
		return x * x + y * y + z * z;
	}
};

#endif
