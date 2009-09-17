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
	void AddObject(SceneObject* object, bool autoDelete=true);

	// ɾ������
	void RemoveObject(SceneObject* object, bool deleteObj=true);

	// Update & Render
	void UpdateScene(unsigned long deltaTime);

	void SetupRenderView(const RenderView& view);
	void RenderScene();

	// �Գ����������߼��
	// TODO: ���typeĿǰ��ȫû����;
	void RayPickingSceneObject(const Ray& ray, ObjectsCollisionInfos& sceneObjects, int type, int collisionGroup = COLLISION_GROUP_ALL);

	// �������ṹ�����log
	void DumpToLog();

private:

	void RemoveSceneObjectInternal(SceneObject* object, bool deleteObj);

	// ִ��������ɾ����������ÿ֡���º����
	void ProcessRemove();

private:
	//typedef vector<ISceneObject*> SceneObjects;
	//SceneObjects	m_SceneObjects;

	//SceneRootObject*			m_RootObject;		///< �������������

	SceneObjectSet				m_SceneObjects;
	SceneObjectSet				m_AutoDeleteList;
	vector<RemoveListElement>	m_RemoveList;		///< ɾ���б���¼����һ֡���º���Ҫɾ���Ķ���

	RenderableObjectSet			m_RenderableObjects;

	bool						m_ProcessingUpdate;	///< ��ǰ�Ƿ����ڸ��³�����״̬��ʶ

	RenderView					m_RenderView;
};

#endif
