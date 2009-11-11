//-----------------------------------------------------------------------------------
/// SceneSerializer.h �������л��࣬���Խ��������浽�ļ��л��Ǵ��ļ��ж�ȡ
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
	// TODO: ���һ���ӿڣ����������TinyXml�����

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

		// �洢һ���Ӷ���
		virtual void SaveSceneObject(SceneObject* sceneObj);
		virtual void SaveAttribute(const String& name, const String& value);

		virtual bool RestoreSceneObject(SceneGraph* scene, SceneObject* parent=NULL, UnserializeSceneObjectCallback callback=NULL);
		virtual String GetAttribute(const String& name);

	protected:
		SceneSerializerNode*	m_Parent;

		TiXmlElement*			m_XmlNode;
	};

	// �������л����������������浽�ļ������ߴ��ļ���ȡ����
	// ���ش˶���ɿ��Ʋ�ͬ�ı�����Ϊ
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

