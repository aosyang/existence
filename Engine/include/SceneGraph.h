//-----------------------------------------------------------------------------------
/// SceneGraph.h 场景图，管理添加到场景的对象
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SCENEGRAPH_H
#define _SCENEGRAPH_H

#include "ISceneObject.h"
#include "SceneObject.h"
#include "Platform.h"
#include "Camera.h"
#include "Ray.h"
#include "Color4f.h"
#include "Collision.h"

#include <set>
#include <vector>



namespace Gen
{

	struct RenderView
	{
		Vector3f position;
		Matrix4 viewMatrix;
		Matrix4 projMatrix;
		Frustum* frustum;
	};

	class SceneSerializer;

	/// @brief
	/// 场景图
	/// @par
	///		用于渲染及更新场景对象的场景图
	/// @remarks
	///		每个对象被添加到场景图之后由场景图负责更新及渲染
	class SceneGraph
	{
		// 场景序列化需要对场景图的完全访问权限
		friend class SceneSerializer;
	public:
		/// 构造函数
		SceneGraph();
		/// 析构函数
		~SceneGraph();

	private:
		/// @brief
		/// 添加对象到场景中
		/// @param object
		///		要添加到场景的对象
		/// @param autoDelete
		///		该对象是否在场景销毁时同时销毁
		/// @remarks
		///		向场景中重复添加同一个对象会产生assert
		void AddObject(SceneObject* object, bool autoDelete=true);

	public:
		/// @brief
		/// 从场景中移除对象
		/// @param object
		///		要从场景中移除的对象
		/// @param deleteObj
		///		移除对象后是否销毁对象
		/// @param recursive
		///		是否递归移除对象的子对象
		/// @remarks
		///		这个方法调用了RemoveSceneObjectInternal方法进行实际的删除操作
		/// @see
		///		RemoveSceneObjectInternal()
		void RemoveObject(SceneObject* object, bool deleteObj=true, bool recursive=false);

		/// @brief
		/// 创建一个已注册的场景对象
		/// @param className
		///		场景对象类型名
		/// @returns
		///		返回成功创建的对象指针
		SceneObject* CreateSceneObject(const String& className, bool autoDelete=true);

		// Update & Render

		/// @brief
		/// 更新场景
		/// @par
		///		场景中所有对象的Update方法将被调用
		/// @param deltaTime
		///		自上一帧更新以来经过的时间(毫秒)
		void UpdateScene(unsigned long deltaTime);

		/// @brief
		/// 设置渲染视点
		/// @param view
		///		渲染视点参数
		void SetupRenderView(const RenderView& view);

		/// @brief
		/// 使用摄像机设置渲染视点
		/// @param camera
		///		要渲染的摄像机
		void SetupRenderView(Camera* camera);

		/// @brief
		/// 渲染场景
		void RenderScene();

		/// @brief
		/// 射线拾取场景对象
		/// @param ray
		///		拾取的射线
		/// @param sceneObjects
		///		用于记录拾取的对象列表
		/// @param type
		///		过滤拾取对象的类型(暂时没有用途)
		/// @param collisionGroup
		///		碰撞组掩码
		/// @param sortResult
		///		是否对结果进行由近及远的排序
		/// @remarks
		///		默认对象不会被放入任何的碰撞组，需要手动指定后方能拾取
		void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL, bool sortResult = false);

		/// @brief
		///	场景球体碰撞
		/// @param pos
		///		球心坐标
		/// @param newpos
		///		经过碰撞反馈后的球心坐标
		/// @param radius
		///		球体半径
		/// @returns
		///		如果球体与任何物体发生碰撞返回true，否则返回false
		/// @remarks
		///		球体与各个对象的碰撞根据对象覆写的PushSphere方法完成
		bool PushSphere(const Vector3f& pos, Vector3f& newpos, float radius);

		/// @brief
		/// 将场景结构输出到log
		void DumpToLog();

	private:
		/// @brief
		///		移除场景对象(内部方法)
		/// @param object
		///		要移除的场景对象
		/// @param deleteObj
		///		是否同时销毁对象
		/// @param recursive
		///		是否递归移除对象
		void RemoveSceneObjectInternal(SceneObject* object, bool deleteObj, bool recursive);

		/// @brief
		/// 执行实际的移除工作
		/// @par
		///		操作将在每帧更新后进行
		/// @remarks
		///		这个方法将最终移除需要从场景中移除的所有对象
		void ProcessRemove();

	private:
		//typedef std::vector<ISceneObject*> SceneObjects;
		//SceneObjects	m_SceneObjects;

		//SceneRootObject*			m_RootObject;		///< 场景对象根对象

		SceneObjectSet				m_SceneObjects;		///< 场景对象列表
		SceneObjectSet				m_AutoDeleteList;	///< 自动删除对象列表

		/// @brief
		/// 移除对象列表元素
		struct RemoveListElement
		{
			SceneObject* obj;		///< 要移除的对象
			bool	deleteObj;		///< 是否在移除后删除对象
			bool	recursive;		///< 是否递归操作子对象
		};

		std::vector<RemoveListElement>	m_RemoveList;		///< 移除列表，记录了这一帧更新后需要移除的对象

		//RenderableObjectSet			m_RenderableObjects;

		bool						m_ProcessingUpdate;	///< 当前是否正在更新场景的状态标识

		RenderView					m_RenderView;		///< 渲染视点信息

		SceneObjectFactory			m_ObjectFactory;	///< 场景对象工厂
	};
}

#endif
