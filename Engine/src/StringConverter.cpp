#include "StringConverter.h"

namespace Gen
{
	String ToString(const Vector3f& vec)
	{
		String s;
		s.Format("%f %f %f", vec.x, vec.y, vec.z);
		return s;
	}

	String ToString(const Quaternion& quat)
	{
		String s;
		s.Format("%f %f %f %f", quat.w, quat.x, quat.y, quat.z);
		return s;
	}

	Vector3f ToVector3f(const String& str)
	{
		float x, y, z;
		sscanf(str.Data(), "%f %f %f", &x, &y, &z);
		return Vector3f(x, y, z);
	}

	Quaternion ToQuaternion(const String& str)
	{
		Quaternion quat;
		sscanf(str.Data(), "%f %f %f %f", &quat.w, &quat.x, &quat.y, &quat.z);
		return quat;
	}
}
