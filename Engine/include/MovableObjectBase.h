//-----------------------------------------------------------------------------------
/// MovableObjectBase.h ���ƶ�������࣬ʵ���˶����ƶ������������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _MOVABLEOBJECTBASE_H
#define _MOVABLEOBJECTBASE_H

#include "IObject.h"
#include "OBB.h"
#include "AABB.h"
#include "Vector3f.h"
#include "Quaternion.h"
#include "Matrix4.h"
#include "Frustum.h"

#include <set>
#include <vector>

using namespace std;

namespace Gen
{
	class MovableObjectBase;
	class RenderableObjectBase;

	typedef set<RenderableObjectBase*>	RenderableObjectSet;
	typedef vector<RenderableObjectBase*> RenderableObjectList;

	/// @brief
	///	���ƶ��������
	/// @par
	///		��¼�ƶ���������ľֲ�������ռ������Ԫ����Ϣ���ṩ����������Ŀռ�任���в���
	class MovableObjectBase : public IObject
	{
	public:
		MovableObjectBase();

		// ----- Overwrite IObject

		void Destroy();

		/// @copydoc IObject::Update()
		/// ÿ�θ��¶���ʱͬʱ���¶����������Χ��
		void Update(unsigned long deltaTime);

		/// �ж϶���������������Ƿ��Ѿ������¹�
		/// @returns
		///		���������¹�������true�����򷵻�false
		/// @note
		///		���������ʱû��ʵ��ʹ��
		bool IsUpdated();

		// ----- MovableObjectBase methods

		// Transform

		/// @brief
		/// �趨���������
		virtual void SetPosition(const Vector3f& pos);

		/// @brief
		/// �趨�������ת��Ϣ
		virtual void SetRotation(const Quaternion& rot);

		/// @brief
		/// ʹ�����ھֲ��ռ������ת
		/// @param quat
		///		�ֲ��ռ���ת��ʹ�õ���Ԫ��
		void RotateLocal(const Quaternion& quat);

		/// @brief
		/// ʹ�����ھֲ��ռ����λ��
		/// @param trans
		///		�������ֲ��ռ�λ�Ƶ�����
		void TranslateLocal(const Vector3f& trans);

		/// @brief
		/// ʹ�����ڸ��ռ����λ��
		/// @param trans
		///		���������ռ�λ�Ƶ�����
		void TranslateParent(const Vector3f& trans);

		/// ��ȡ����ľֲ��ռ����
		/// @returns
		///		����ľֲ��ռ����
		inline const Matrix4& Transform() const { return m_Transform; }
		
		/// ��ȡ���������ռ����
		inline const Matrix4& WorldTransform() const { return m_WorldTransform; }

		/// ��ȡ����ľֲ��ռ�����
		inline const Vector3f GetLocalPosition() const { return m_Transform.GetPosition(); }

		/// ��ȡ���������ռ�����
		inline const Vector3f GetWorldPosition() const { return m_WorldTransform.GetPosition(); }

		/// ��ȡ����ľֲ��ռ���ת��Ԫ��
		inline const Quaternion& GetLocalQuaternion() const { return m_Rotation; }

		// ��ȡ��Χ��

		/// ��ȡ������Χ��
		/// @returns
		///		���ض����������Χ��
		const AABB& GetAABB();

		/// ��ȡ�ֲ��ռ��Χ��
		/// @returns
		///		���ض���ľֲ��ռ��Χ��
		const OBB& GetOBB();

		// Render Collection

		/// @brief
		/// �����Ƿ����Ⱦ�����б��޳�
		/// @param frustum
		///		���ڽ����޳����Ե�ƽͷ�ӽ���
		/// @returns
		///		�������ᱻ�޳�������true����Ҫ�����򷵻�false
		/// @remarks
		///		�����าд��������Կ��ƶ������Ⱦ�޳�
		virtual bool IsCulled(Frustum* frustum);

	protected:
		bool							m_Updated;

		Quaternion						m_Rotation;					///< �ֲ��ռ���ת����Ԫ��
		Matrix4							m_Transform;				///< �ֲ��ռ�任
		Matrix4							m_WorldTransform;			///< ����ռ�任��ע��ͨ������Ҫ�ֶ����£���Update��������Ӹ��������


		OBB								m_OBB;						///< �ֲ��ռ��Χ��
		AABB							m_AABB;						///< ������Χ��

	};
}

#endif
