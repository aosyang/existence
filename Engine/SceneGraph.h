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

	// ִ��������ɾ����������ÿ֡���º����
	void ProcessRemove();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	SceneRootObject*			m_RootObject;		///< �������������

	set<BaseSceneObject*>		m_AutoDeleteList;
	vector<RemoveListElement>	m_RemoveList;		///< ɾ���б���¼����һ֡���º���Ҫɾ���Ķ���

	bool						m_ProcessingUpdate;	///< ��ǰ�Ƿ����ڸ��³�����״̬��ʶ

	RenderView					m_RenderView;
	//Camera*						m_Camera;

	Color4f						m_Ambient;
};

#endif
