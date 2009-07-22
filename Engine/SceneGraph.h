//-----------------------------------------------------------------------------------
/// SceneGraph.h ����ͼ��������ӵ������Ķ���
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
/// ������Ⱦ�����³�������ĳ���ͼ
/// 
/// ÿ��������ӵ�����ͼ֮���ɳ���ͼ������¼���Ⱦ
//-----------------------------------------------------------------------------------
class SceneGraph
{
public:
	SceneGraph();
	~SceneGraph();

	// ��Ӷ���
	void AddObject(BaseSceneObject* object, bool autoDelete=true);

	// ɾ������
	void RemoveObject(BaseSceneObject* object, bool deleteObj=true);

	// ���UI�ؼ�
	void AddUIObject(BaseUIObject* object);

	// UI�������
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

	// ��Ⱦ�߿�
	void DrawLine(const Vector3f& start, const Vector3f& end, const Color4f& color = Color4f(1.0f, 1.0f, 1.0f));

private:
	void SetFrustum(Frustum* frustum);
	void DrawAssistantElements();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	SceneRootObject*	m_RootObject;		///< �������������
	UIRootObject*		m_UIRootObject;		///< UI������
	vector<ElementLine> m_LineElements;

	set<BaseSceneObject*>	m_AutoDeleteList;
	//vector<BaseSceneObject*>	m_RemoveList;

	Camera*				m_Camera;

	Color4f				m_Ambient;
};

#endif
