#ifndef _SCENESERIALIZER_H
#define _SCENESERIALIZER_H

#include "SceneGraph.h"
#include "EString.h"
#include "tinyxml/tinyxml.h"

// 场景序列化，将整个场景保存到文件，或者从文件读取场景
// 重载此对象可控制不同的保存行为
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
	// Scene访问函数，派生类也通过这些方法访问Scene graph的私有成员
	SceneObjectSet& GetSceneObjectSet();
	SceneObjectSet& GetChildrenObjectSet(SceneObject* sceneObj);

protected:
	SceneGraph*		m_Scene;

	TiXmlElement*	m_RootElement;
};

#endif

