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

using namespace std;

struct RemoveListElement
{
	SceneObject* obj;
	bool	deleteObj;
};

struct RenderView
{
	Vector3f position;
	Matrix4 viewMatrix;
	Matrix4 projMatrix;
	Frustum* frustum;
};


//-----------------------------------------------------------------------------------
/// \brief
/// 用于渲染及更新场景对象的场景图
/// 
/// 每个对象被添加到场景图之后由场景图负责更新及渲染
//-----------------------------------------------------------------------------------
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	// 添加对象
	void AddObject(SceneObject* object, bool autoDelete=true);

	// 删除对象
	void RemoveObject(SceneObject* object, bool deleteObj=true);

	// Update & Render
	void UpdateScene(unsigned long deltaTime);

	void SetupRenderView(const RenderView& view);
	void RenderScene();

	// 对场景进行射线检测
	// TODO: 这个type目前完全没有用途
	void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL);

	// 将场景结构输出到log
	void DumpToLog();

private:

	void RemoveSceneObjectInternal(SceneObject* object, bool deleteObj);

	// 执行真正的删除工作，在每帧更新后进行
	void ProcessRemove();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	//SceneRootObject*			m_RootObject;		///< 场景对象根对象

	SceneObjectSet				m_SceneObjects;
	SceneObjectSet				m_AutoDeleteList;
	vector<RemoveListElement>	m_RemoveList;		///< 删除列表，记录了这一帧更新后需要删除的对象

	RenderableObjectSet			m_RenderableObjects;

	bool						m_ProcessingUpdate;	///< 当前是否正在更新场景的状态标识

	RenderView					m_RenderView;
};

#endif
