//-----------------------------------------------------------------------------------
/// SceneSerializer.cpp 场景序列化类实现
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "SceneSerializer.h"
#include "SceneGraph.h"

namespace Gen
{
	SceneSerializerNode::SceneSerializerNode()
		: m_Parent(NULL)
	{
	}

	SceneSerializerNode::SceneSerializerNode(SceneSerializerNode* parent)
		: m_Parent(parent)
	{
	}

	void SceneSerializerNode::SaveSceneObject(SceneObject* sceneObj)
	{
		m_XmlNode = new TiXmlElement("SceneObject");

		SceneSerializerNode node(this);

		if (sceneObj->OnSave(this))
			m_Parent->m_XmlNode->InsertEndChild(*m_XmlNode);

		delete m_XmlNode;
	}

	void SceneSerializerNode::SaveAttribute(const String& name, const String& value)
	{
		m_XmlNode->SetAttribute(name.Data(), value.Data());
	}

	bool SceneSerializerNode::RestoreSceneObject(SceneGraph* scene, SceneObject* parent, UnserializeSceneObjectCallback callback)
	{
		m_XmlNode = m_Parent->m_XmlNode->FirstChildElement();

		if (!m_XmlNode) return true;

		do
		{
			SceneObject* sceneObj = scene->CreateSceneObject(GetAttribute("type"));
			sceneObj->OnRestore(this);
			if (callback) (*callback)(sceneObj);
			if (parent) parent->AttachChildObject(sceneObj);
			SceneSerializerNode childNode(this);
			childNode.RestoreSceneObject(scene, sceneObj, callback);
		} while (m_XmlNode = m_XmlNode->NextSiblingElement());

		return true;
	}

	String SceneSerializerNode::GetAttribute(const String& name)
	{
		return String(m_XmlNode->Attribute(name.Data()));
	}

	SceneSerializer::SceneSerializer()
		: m_Scene(NULL)
	{
	}

	//SceneSerializer::SceneSerializer(SceneGraph* scene)
	//: m_Scene(scene)
	//{
	//}

	SceneSerializer::~SceneSerializer()
	{
	}

	bool SceneSerializer::Serialize(const String& filename)
	{
		AssertFatal(m_Scene, "SceneSerializer::Serialize(): Unable to deal with a NULL scene graph.");

		TiXmlDocument doc(filename.Data());
		TiXmlDeclaration decl("1.0", "UTF-8", "yes");
		doc.InsertEndChild(decl);

		m_XmlNode = new TiXmlElement("Scene");

		SceneObjectSet::iterator iter;
		for (iter=m_Scene->m_SceneObjects.begin(); iter!=m_Scene->m_SceneObjects.end(); iter++)
		{
			SceneSerializerNode node(this);
			//SerializeSceneObject(*iter);
			node.SaveSceneObject(*iter);
		}

		doc.InsertEndChild(*m_XmlNode);
		doc.SaveFile();
		delete m_XmlNode;

		return true;
	}

	void SceneSerializer::Unserialize(const String& filename, UnserializeSceneObjectCallback callback)
	{
		AssertFatal(m_Scene, "SceneSerializer::Unserialize(): Unable to deal with a NULL scene graph.");

		TiXmlDocument doc(filename.Data());
		doc.LoadFile();
		//TiXmlElement* rootElement = doc.FirstChildElement();
		//m_XmlNode = rootElement->FirstChildElement();
		m_XmlNode = doc.FirstChildElement();

		if (!m_XmlNode) return;

		do
		{
			SceneSerializerNode node(this);
			node.RestoreSceneObject(m_Scene, NULL, callback);
		} while (m_XmlNode = m_XmlNode->NextSiblingElement());

		//m_Scene->
	}
}
