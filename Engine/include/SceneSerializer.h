#ifndef _SCENESERIALIZER_H
#define _SCENESERIALIZER_H

#include "SceneGraph.h"
#include "EString.h"
#include "tinyxml/tinyxml.h"

// �������л����������������浽�ļ������ߴ��ļ���ȡ����
// ���ش˶���ɿ��Ʋ�ͬ�ı�����Ϊ
class SceneSerializer
{
public:
	SceneSerializer(SceneGraph* scene);
	virtual ~SceneSerializer();

	// Serialize / Unserialize
	virtual bool Serialize(const String& filename);
	virtual void Unserialize(const String& filename);

	virtual void SerializeSceneObject(SceneObject* sceneObj);

protected:
	// Scene���ʺ�����������Ҳͨ����Щ��������Scene graph��˽�г�Ա
	SceneObjectSet& GetSceneObjectSet();
	SceneObjectSet& GetChildrenObjectSet(SceneObject* sceneObj);

protected:
	SceneGraph*		m_Scene;

	TiXmlElement*	m_RootElement;
};

#endif

