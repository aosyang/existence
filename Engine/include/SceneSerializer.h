//-----------------------------------------------------------------------------------
/// SceneSerializer.h 场景序列化类，可以将场景保存到文件中或是从文件中读取
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _SCENESERIALIZER_H
#define _SCENESERIALIZER_H

//#include "SceneObject.h"
#include "EString.h"
#include "tinyxml/tinyxml.h"

namespace Gen
{
	// TODO: 设计一个接口，解除基类与TinyXml的耦合

	class SceneGraph;
	class SceneObject;
	class SceneSerializer;
	class SceneSerializerNode;

	typedef void(*UnserializeSceneObjectCallback)(SceneObject*);

	class SceneSerializerNode
	{
	public:
		SceneSerializerNode();
		SceneSerializerNode(SceneSerializerNode* parent);
		virtual ~SceneSerializerNode() {}

		// 存储一个子对象
		virtual void SaveSceneObject(SceneObject* sceneObj);
		virtual void SaveAttribute(const String& name, const String& value);

		virtual bool RestoreSceneObject(SceneGraph* scene, SceneObject* parent=NULL, UnserializeSceneObjectCallback callback=NULL);
		virtual String GetAttribute(const String& name);

	protected:
		SceneSerializerNode*	m_Parent;

		TiXmlElement*			m_XmlNode;
	};

	// 场景序列化，将整个场景保存到文件，或者从文件读取场景
	// 重载此对象可控制不同的保存行为
	class SceneSerializer : public SceneSerializerNode
	{
		friend class SceneSerializerNode;
	public:
		SceneSerializer();
		//SceneSerializer(SceneGraph* scene);
		virtual ~SceneSerializer();

		// Serialize / Unserialize
		virtual bool Serialize(const String& filename);
		virtual void Unserialize(const String& filename, UnserializeSceneObjectCallback callback=NULL);

		void SetSceneGraph(SceneGraph* scene) { m_Scene = scene; }
	protected:
		SceneGraph*		m_Scene;

		//TiXmlElement*	m_RootElement;
	};
}

#endif

