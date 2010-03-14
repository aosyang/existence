//-----------------------------------------------------------------------------------
/// SceneObject.h 场景对象，添加到场景的对象继承自此类，包含了绑定子对象的方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _SCENEOBJECT_H
#define _SCENEOBJECT_H

#include "MovableObjectBase.h"
#include "Frustum.h"
#include "Collision.h"
#include "EString.h"
#include "SceneSerializer.h"

#include <set>
#include <list>
#include <map>

using namespace std;

namespace Gen
{
	class SceneGraph;
	class SceneObject;
	class RenderableObjectBase;
	class SceneSerializer;
	class SceneSerializerNode;




	typedef SceneObject* (*FactoryCreateFuncPtr)(SceneGraph*);

	// 在每个SceneObject的派生类中声明这个对象，事先创建该类对象的工厂
	// 返回类型字串
	// 同时将构造和析构函数声明为私有，防止对象被直接构造
	// 调用了这个宏声明的对象必须实现带一个参数SceneGraph*的构造函数和析构函数

	/// @def DECLARE_FACTORY_OBJECT(className, baseClassName)
	/// 将一个类声明为工厂对象，用户稍后可以使用FACTORY_CREATE宏创建这个对象
	/// @note
	///		在创建对象之前，必须先使用REGISTER_FACTORY注册类
	#define DECLARE_FACTORY_OBJECT(className, baseClassName) \
		typedef baseClassName BaseClass; \
		public:	\
			static SceneObject* FactoryCreateSceneObject(SceneGraph* scene) { \
				return new className(scene); \
			} \
			const String GetTypeName() const { return #className; } \
		protected: \
			className(SceneGraph* scene); \
			~className(); \
		private: \
			className();

	// 在场景中创建一个对象并转换为对应类型的指针
	#define FACTORY_CREATE(scenePtr, className) static_cast<className*>(scenePtr->CreateSceneObject(#className))

	// 注册一个类型的工厂函数
	#define REGISTER_FACTORY(className) SceneObjectFactory::RegisterFactoryCreator(#className, &className::FactoryCreateSceneObject);

	typedef map<const String, FactoryCreateFuncPtr>	SceneObjectFactoryCreators;

	// 场景对象工厂，使用工厂方法创建对象
	class SceneObjectFactory
	{
	public:
		static void RegisterFactoryCreator(const String& className, FactoryCreateFuncPtr ptrFunc);
		static void RegisterDefaultFactories();

		SceneObject* CreateSceneObject(SceneGraph* scene, const String& className);
	protected:
		static SceneObjectFactoryCreators	m_FactoryCreators;
	};





	typedef set<RenderableObjectBase*> RenderableObjectSet;
	typedef set<SceneObject*>	SceneObjectSet;
	typedef list<SceneObject*>	SceneObjectList;

	/// @brief
	/// 场景对象
	/// @par
	///		每一个能够在场景中更新的对象都继承自场景对象类
	///		场景对象提供了拾取判定、设置可视状况、绑定子结点等功能
	/// @see
	///		SceneGraph
	class SceneObject : public MovableObjectBase
	{
		friend class SceneSerializer;
	public:
		/// @brief
		/// 构造函数
		/// @param scene
		///		对象要添加到的场景
		SceneObject(SceneGraph* scene);
		~SceneObject();

		// ----- Overwrite IObject

		/// @copydoc MovableObjectBase::Update()
		void Update(unsigned long deltaTime);

		/// @copydoc IObject::GetTypeName()
		const String GetTypeName() const { return "SceneObject"; }

		// ----- Overwrite MovableObjectBase

		/// @copydoc MovableObjectBase::IsCulled()
		/// 场景对象剔除m_Visible=false的对象
		bool IsCulled(Frustum* frustum);

		// ----- SceneObject methods

		/// @brief
		/// 渲染调试信息
		/// @remarks
		///		子类覆写这个函数以实现渲染不同的调试信息
		/// @note
		///		这个方法在物体本身被渲染之后调用
		/// @see
		///		SceneGraph::RenderScene()
		virtual void DebugRender();

	protected:
		/// @brief
		/// 渲染AABB
		/// @remarks
		///		这个方法在默认继承关系下由DebugRender调用
		//void DebugRenderAABB();

		/// @brief
		///	渲染OBB
		/// @remarks
		///		这个方法在默认继承关系下由DebugRender调用
		//void DebugRenderOBB();

	public:
		// 碰撞检测

		// AABB碰撞查询
		// virtual void QueryAABB

		// 可视性判断

		/// @brief
		/// 设置当前对象是否可见
		/// @param visible
		///		true为可见，false为不可见
		void SetVisible(bool visible) { m_Visible = visible; }

		/// @brief
		/// 获取当前对象是否可见
		/// @returns
		///		true为可见，false为不可见
		bool GetVisible() const { return m_Visible; }

		/// @brief
		/// 指定该对象所在的场景
		/// @param scene
		///		对象的场景
		/// @remarks
		///		许多对象需要得知它们所在的场景，以进行其他操作
		void SetSceneGraph(SceneGraph* scene) { m_Scene = scene; }

		/// @brief
		/// 递归指定对象及其子对象所在的场景
		/// @param scene
		///		对象的场景
		/// @remarks
		///		这个方法将该对象及其所有子对象的场景指定为参数内容
		void SetSceneGraphRecursively(SceneGraph* scene);

		/// @brief
		/// 获取对象当前持有的场景指针
		/// @returns
		///		返回对象当前的场景指针
		SceneGraph* GetSceneGraph() { return m_Scene; }

		/// @brief
		/// 递归收集场景中需要渲染的对象
		/// @param renderableObjs
		///		渲染对象列表，需要渲染的对象加到这个列表中
		/// @param frustum
		///		当前渲染使用的平头视截体
		virtual void CollectRenderableObject(RenderableObjectList& renderableObjs, Frustum* frustum);


		// Collision

		/// @brief
		/// 射线递归拾取场景对象
		/// @param ray
		///		拾取判断的射线
		///	@param sceneObjects
		///		如果相交判断成立，将射线与该对象的碰撞信息添加到这个列表中
		/// @param type
		///		(暂时不用)
		/// @param collisionGroup
		///		碰撞分组
		void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup);

		/// @brief
		/// 射线与对象相交判断
		/// @param ray
		///		所判断的射线
		/// @param info
		///		交点信息
		/// @returns
		///		如果相交，返回true；否则返回false
		virtual bool IntersectsRay(const Ray& ray, CollisionResult& info);

		virtual bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);

		/// 获取碰撞组掩码
		int GetCollisionGroupMask() const { return m_CollisionGroupMask; }

		/// 指定碰撞组掩码
		void SetCollisionGroupMask(int mask) { m_CollisionGroupMask = mask; }

		// Attachable

		/// @brief
		/// 绑定其他对象作为该对象的子对象
		/// @param child
		///		子对象
		/// @param keepRotation
		///		是否维持子对象的旋转不受到父结点影响
		/// @param keepWorldTransform
		///		是否在附加的时候保持子对象的世界坐标不变
		void AttachChildObject(SceneObject* child, bool keepRotation = false, bool keepWorldTransform = false);

		/// @brief
		/// 将该对象从父结点上拆下
		/// @param keepWorldTransform
		///		是否维持对象从父结点分离以后的世界坐标
		void DetachFromParent(bool keepWorldTransform);

		/// 获取父对象
		inline SceneObject* GetParent() const { return m_ParentObject; }

		/// @brief
		/// 删除所有子对象
		/// @remarks
		///		这个方法会将子对象从场景中删除
		void DeleteAllChildren();

		// 序列化
		virtual bool OnSave(SceneSerializerNode* node);
		virtual void OnRestore(SceneSerializerNode* node);

		void SetSerializable(bool serializable) { m_Serializable = serializable; }
		bool GetSerializable() const { return m_Serializable; }

		// Debug

		/// @brief
		/// 将场景物体信息递归导出到日志
		/// @param level
		///		导出深度，用于输出文本的缩进
		void DumpToLogRecursively(int level=0);

	protected:
		/// @brief
		/// 指定父对象
		/// @param parent
		///		父对象
		/// @param keepRotation
		///		保持当前对象的旋转信息不受到父对象影响
		void SetParentObject(SceneObject* parent, bool keepRotation);

	protected:
		//// 添加一个可渲染物体
		//void AddRenderableObject(RenderableObjectBase* renderable);

		//// 删除一个可渲染物体
		//void RemoveRenderableObject(RenderableObjectBase* renderable);

	protected:
		bool							m_Visible;					///< 可见性
		SceneGraph*						m_Scene;					///< 对象所处的场景指针


		SceneObjectSet					m_ChildrenObjects;			///< 子结点对象集合
		SceneObject*					m_ParentObject;				///< 父对象

		bool							m_KeepRotation;				///< 当前对象的旋转不会受到父结点旋转的影响
		int								m_CollisionGroupMask;		///< 碰撞组掩码
		//RenderableObjectSet				m_RenderableObjects;
		bool							m_Serializable;				///< 该对象是否可以保存到数据
	};
}

#endif

