//-----------------------------------------------------------------------------------
/// MovableObjectBase.h 可移动对象基类，实现了对象移动的最基本方法
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
	///	可移动对象基类
	/// @par
	///		记录移动对象所需的局部及世界空间矩阵、四元数信息，提供方法对物体的空间变换进行操作
	class MovableObjectBase : public IObject
	{
	public:
		MovableObjectBase();

		// ----- Overwrite IObject

		void Destroy();

		/// @copydoc IObject::Update()
		/// 每次更新对象时同时更新对象的轴对齐包围盒
		void Update(unsigned long deltaTime);

		/// 判断对象在这个周期内是否已经被更新过
		/// @returns
		///		如果对象更新过，返回true，否则返回false
		/// @note
		///		这个方法暂时没有实际使用
		bool IsUpdated();

		// ----- MovableObjectBase methods

		// Transform

		/// @brief
		/// 设定对象的坐标
		virtual void SetPosition(const Vector3f& pos);

		/// @brief
		/// 设定对象的旋转信息
		virtual void SetRotation(const Quaternion& rot);

		/// @brief
		/// 使对象在局部空间进行旋转
		/// @param quat
		///		局部空间旋转所使用的四元数
		void RotateLocal(const Quaternion& quat);

		/// @brief
		/// 使对象在局部空间进行位移
		/// @param trans
		///		对象做局部空间位移的向量
		void TranslateLocal(const Vector3f& trans);

		/// @brief
		/// 使对象在父空间进行位移
		/// @param trans
		///		对象做父空间位移的向量
		void TranslateParent(const Vector3f& trans);

		/// 获取对象的局部空间矩阵
		/// @returns
		///		对象的局部空间矩阵
		inline const Matrix4& Transform() const { return m_Transform; }
		
		/// 获取对象的世界空间矩阵
		inline const Matrix4& WorldTransform() const { return m_WorldTransform; }

		/// 获取对象的局部空间坐标
		inline const Vector3f GetLocalPosition() const { return m_Transform.GetPosition(); }

		/// 获取对象的世界空间坐标
		inline const Vector3f GetWorldPosition() const { return m_WorldTransform.GetPosition(); }

		/// 获取对象的局部空间旋转四元数
		inline const Quaternion& GetLocalQuaternion() const { return m_Rotation; }

		// 获取包围盒

		/// 获取轴对齐包围盒
		/// @returns
		///		返回对象的轴对齐包围盒
		const AABB& GetAABB();

		/// 获取局部空间包围盒
		/// @returns
		///		返回对象的局部空间包围盒
		const OBB& GetOBB();

		// Render Collection

		/// @brief
		/// 对象是否从渲染管线中被剔除
		/// @param frustum
		///		用于进行剔除测试的平头视截体
		/// @returns
		///		如果对象会被剔除，返回true，需要保留则返回false
		/// @remarks
		///		派生类覆写这个方法以控制对象的渲染剔除
		virtual bool IsCulled(Frustum* frustum);

	protected:
		bool							m_Updated;

		Quaternion						m_Rotation;					///< 局部空间旋转的四元数
		Matrix4							m_Transform;				///< 局部空间变换
		Matrix4							m_WorldTransform;			///< 世界空间变换，注：通常不需要手动更新，由Update方法负责从父对象更新


		OBB								m_OBB;						///< 局部空间包围盒
		AABB							m_AABB;						///< 轴对齐包围盒

	};
}

#endif
