#ifndef _STRINGCONVERTER_H
#define _STRINGCONVERTER_H

#include "EString.h"
#include "Vector3f.h"
#include "Quaternion.h"

String ToString(const Vector3f& vec);

String ToString(const Quaternion& quat);

Vector3f ToVector3f(const String& str);

Quaternion ToQuaternion(const String& str);

#endif

