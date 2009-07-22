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
#include "BaseUIObject.h"
#include "SceneRootObject.h"
#include "UIRootObject.h"
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

	// 添加UI控件
	void AddUIObject(BaseUIObject* object);

	// UI点击操作
	//void OnMousePressed(MouseButtonID id);
	//void OnMouseReleased(MouseButtonID id);
	//void OnMouseMoved(int x, int y);

	void SetCamera(Camera* camera, bool useCameraFrustum = true);
	Camera* GetCamera() { return m_Camera; }
	//void NotifyUpdatingProjectionMatrix();

	// Update & Render
	void UpdateScene(unsigned long deltaTime);
	void RenderScene();

	void CollectRayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& baseSceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL);

	void SetAmbientColor(const Color4f color);
	const Color4f GetAmbientColor() const { return m_Ambient; }

	// 渲染线框
	void DrawLine(const Vector3f& start, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

private:
	void SetFrustum(Frustum* frustum);
	void DrawAssistantElements();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	SceneRootObject*	m_RootObject;		///< 场景对象根对象
	UIRootObject*		m_UIRootObject;		///< UI根对象
	vector<ElementLine> m_LineElements;

	set<BaseSceneObject*>	m_AutoDeleteList;
	//vector<BaseSceneObject*>	m_RemoveList;

	Camera*				m_Camera;

	Color4f				m_Ambient;
};

#endif
