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
#include "RefPtr.h"

#include <set>
#include <list>
#include <map>



namespace Gen
{
	class SceneGraph;
	class SceneObject;
	class RenderableObjectBase;
	class SceneSerializer;
	class SceneSerializerNode;
	class Anchor;

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
		protected: \
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

	#define FACTORY_CREATE_NO_AUTO_DELETE(scenePtr, className) static_cast<className*>(scenePtr->CreateSceneObject(#className, false))

	// 注册一个类型的工厂函数
	#define REGISTER_FACTORY(className) SceneObjectFactory::RegisterFactoryCreator(#className, &className::FactoryCreateSceneObject);

	typedef std::map<const String, FactoryCreateFuncPtr>	SceneObjectFactoryCreators;

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





	typedef std::set<RenderableObjectBase*> RenderableObjectSet;
	typedef std::set<SceneObject*>	SceneObjectSet;
	typedef std::list<SceneObject*>	SceneObjectList;
	typedef std::map<const String, RefPtr<Anchor> >	AnchorMap;

	/// @brief
	///	枚举一个场景结点如何绑定到父结点
	enum SceneObjectAttachType
	{
		ATTACH_TYPE_NONE = 0,
		ATTACH_TYPE_PARENT_OBJECT,
		ATTACH_TYPE_PARENT_ANCHOR,
	};

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
		friend class SceneGraph;
	public:
		/// @brief
		/// 构造函数
		/// @param scene
		///		对象要添加到的场景
		SceneObject(SceneGraph* scene);
		~SceneObject();

		// ----- Overwrite IObject

		/// @copydoc MovableObjectBase::Update()
		/// @remarks
		///		场景对象的WorldTransform必须在调用了Update之后才具有可信的值
		void Update(unsigned long deltaTime);

		/// @copydoc IObject::GetTypeName()
		const String GetTypeName() const { return "SceneObject"; }

		// ----- Overwrite MovableObjectBase

		/// @brief
		/// 对象是否从渲染管线中被剔除
		/// @param frustum
		///		用于进行剔除测试的平头视截体
		/// @returns
		///		如果对象会被剔除，返回true，需要保留则返回false
		/// @remarks
		///		派生类覆写这个方法以控制对象的渲染剔除，场景对象剔除m_Visible=false的对象
		virtual bool IsCulled(Frustum* frustum);

		// ----- SceneObject methods

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
		/// 指定渲染顺序
		/// @param order
		///		渲染序号
		void SetRenderOrder(unsigned int order) { m_RenderOrder = order; }

		/// @brief
		/// 获取渲染顺序
		unsigned int GetRenderOrder() const { return m_RenderOrder; }

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
		/// 将一个对象绑定到当前对象的锚点上
		/// @param child
		///		子对象
		/// @param anchorName
		///		锚点名称
		void AttachChildObjectToAnchor(SceneObject* child, const String& anchorName);

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

		/// @brief
		///	递归准备对象渲染
		/// @param frustum
		///		渲染所使用的视截体
		/// @remarks
		///		这个方法由引擎调用，递归将场景对象送去渲染
		void PrepareObjectRendering(Frustum* frustum);

		/// @brief
		///	渲染当前对象
		/// @par
		///		调用渲染器对当前对象进行渲染
		/// @remarks
		///		派生类覆写此方法以实现不同的渲染效果
		virtual void RenderSingleObject();

	public:
		/// @brief
		///	为渲染对象准备光照
		/// @remarks
		///		默认下对象渲染关闭所有光照
		virtual void SetupLightForRendering();

	protected:
		/// @brief
		/// 渲染调试信息
		/// @remarks
		///		子类覆写这个函数以实现渲染不同的调试信息
		/// @note
		///		这个方法在物体本身被渲染之后调用
		/// @see
		///		SceneGraph::RenderScene()
		virtual void DebugRender();

		/// @brief
		/// 渲染AABB
		/// @remarks
		///		这个方法在默认继承关系下由DebugRender调用
		void DebugRenderAABB();

		/// @brief
		///	渲染OBB
		/// @remarks
		///		这个方法在默认继承关系下由DebugRender调用
		void DebugRenderOBB();

		void AddAnchor(const String& anchorName, RefPtr<Anchor> anchor);

	public:
		static bool						m_sDebugRender;				///< 控制是否渲染场景对象调试信息的静态成员变量

	protected:
		SceneObjectAttachType			m_AttachType;				///< 记录该对象如何绑定到父结点

		bool							m_Visible;					///< 可见性
		SceneGraph*						m_Scene;					///< 对象所处的场景指针


		SceneObjectSet					m_ChildrenObjects;			///< 子结点对象集合
		SceneObject*					m_ParentObject;				///< 父对象

		AnchorMap						m_Anchors;					///< 对象所具有的锚点集合
		RefPtr<Anchor>					m_ParentAnchor;				///< 父对象锚点

		bool							m_KeepRotation;				///< 当前对象的旋转不会受到父结点旋转的影响
		int								m_CollisionGroupMask;		///< 碰撞组掩码

		unsigned int					m_RenderOrder;				///< 渲染顺序

		bool							m_Serializable;				///< 该对象是否可以保存到数据
	};

	/// @brief
	///	锚点
	/// @par
	///		场景对象上的锚点是用于定位或者挂载其他对象的辅助定位点
	class Anchor : public RefObject
	{
	public:
		/// @brief
		///	获取锚点基于对象的变换
		virtual const Matrix4& GetTransformToParentObject() const { return m_Transform; }
	protected:
		Matrix4		m_Transform;
	};
}

#endif

