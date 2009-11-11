//-----------------------------------------------------------------------------------
/// ILight.h 灯光接口，包含灯光的基本方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ILIGHT_H
#define _ILIGHT_H

#include "Color4f.h"
#include "Vector3f.h"

namespace Gen
{
	enum LightType
	{
		LIGHT_TYPE_POINT,
		LIGHT_TYPE_SPOT,
		LIGHT_TYPE_DIRECTIONAL,
	};

	class ILight
	{
	public:
		virtual ~ILight() {}

		virtual const Color4f& GetAmbientColor() = 0;
		virtual const Color4f& GetDiffuseColor() = 0;
		virtual const Color4f& GetSpecularColor() = 0;

		virtual const float GetConstantAttenuation() = 0;
		virtual const float GetLinearAttenuation() = 0;
		virtual const float GetQuadraticAttenuation() = 0;

		virtual int GetLightType() = 0;

		virtual Vector3f GetDirection() = 0;

		virtual const Vector3f GetPosition() const = 0;

		virtual float GetRange() const = 0;
	};
}

#endif
