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
#include "BaseSceneObject.h"
#include "SceneRootObject.h"
#include "Platform.h"
#include "Camera.h"
#include "Ray.h"
#include "Color4f.h"

#include <vector>

using namespace std;

struct ElementLine
{
	Vector3f v1, v2;
	Color4f color;
};

struct RemoveListElement
{
	BaseSceneObject* obj;
	bool	deleteObj;
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
	void AddObject(BaseSceneObject* object, bool autoDelete=true);

	// 删除对象
	void RemoveObject(BaseSceneObject* object, bool deleteObj=true);

	//void SetCamera(Camera* camera, bool useCameraFrustum = true);
	//Camera* GetCamera() { return m_Camera; }
	//void NotifyUpdatingProjectionMatrix();

	// Update & Render
	void UpdateScene(unsigned long deltaTime);

	void SetupRenderView(const RenderView& view);
	void RenderScene();

	void CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL);

	void SetAmbientColor(const Color4f color);
	const Color4f GetAmbientColor() const { return m_Ambient; }

private:

	void RemoveObjectInternal(BaseSceneObject* object, bool deleteObj);

	// 执行真正的删除工作，在每帧更新后进行
	void ProcessRemove();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	SceneRootObject*			m_RootObject;		///< 场景对象根对象

	set<BaseSceneObject*>		m_AutoDeleteList;
	vector<RemoveListElement>	m_RemoveList;		///< 删除列表，记录了这一帧更新后需要删除的对象

	bool						m_ProcessingUpdate;	///< 当前是否正在更新场景的状态标识

	RenderView					m_RenderView;
	//Camera*						m_Camera;

	Color4f						m_Ambient;
};

#endif
