//-----------------------------------------------------------------------------------
/// Light.h �ƹ⣬�������������еĸ��ֶ���
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _LIGHT_H
#define _LIGHT_H

#include "ILight.h"
#include "SceneObject.h"
#include "Color4f.h"

namespace Gen
{
	class SceneObject;

	class Light : public SceneObject, public ILight
	{
		DECLARE_FACTORY_OBJECT(Light);
	public:
		// ----- Overwrite IObject
		void Destroy();

		// ----- Overwrite SceneObject
		void DebugRender();

		// ----- Overwrite ILight
		void SetAmbientColor(const Color4f& color) { m_Ambient = color; }
		const Color4f& GetAmbientColor() { return m_Ambient; }

		void SetDiffuseColor(const Color4f& color) { m_Diffuse = color; }
		const Color4f& GetDiffuseColor() { return m_Diffuse; }

		void SetSpecularColor(const Color4f& color) { m_Specular = color; }
		const Color4f& GetSpecularColor() { return m_Specular; }

		// ȡ�ù�Դλ��
		const Vector3f GetPosition() const;

		void SetConstantAttenuation(float val) { m_ConstantAttenuation = val; }
		const float GetConstantAttenuation() { return m_ConstantAttenuation; }

		void SetLinearAttenuation(float val) { m_LinearAttenuation = val; }
		const float GetLinearAttenuation() { return m_LinearAttenuation; }

		void SetQuadraticAttenuation(float val) { m_ConstantAttenuation = val; }
		const float GetQuadraticAttenuation() { return m_QuadraticAttenuation; }

		void SetLightType(LightType type) { m_Type = type; }
		int GetLightType() { return m_Type; }

		void SetDirection(const Vector3f dir) { m_Direction = dir; }
		Vector3f GetDirection() { return m_Direction; }

		float GetRange() const { return m_Range; }

		// ----- Light Methods

		// ���Ƶƹ⿪��
		void ToggleActive(bool active) { m_Active = active; }
		bool GetActive() const { return m_Active; }

		void SetRange(float range) { m_Range = range; }

		float GetAttenuationFactor(float d);

		bool HasMoved();

	protected:
		Color4f		m_Ambient;			///< ��������ɫ
		Color4f		m_Diffuse;			///< ��������ɫ
		Color4f		m_Specular;			///< �߹���ɫ

		LightType	m_Type;

		Vector3f	m_Direction;		///< ����ʹ�÷����ʱ����Ч

		float		m_ConstantAttenuation;
		float		m_LinearAttenuation;
		float		m_QuadraticAttenuation;

		bool		m_Active;			///< �Ƿ񼤻�
		float		m_Range;			///< ���÷�Χ
	};
}

#endif
