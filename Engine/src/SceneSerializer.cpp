#include "SceneSerializer.h"
#include "StringConverter.h"

SceneSerializer::SceneSerializer(SceneGraph* scene)
: m_Scene(scene)
{
}

SceneSerializer::~SceneSerializer()
{
}

bool SceneSerializer::Serialize(const String& filename)
{
	AssertFatal(m_Scene, "SceneSerializer::Serialize(): Unable to deal with a NULL scene graph.");

	TiXmlDocument doc(filename.Data());
	TiXmlDeclaration decl("1.0", "UTF-8", "yes");
	doc.InsertEndChild(decl);

	m_RootElement = new TiXmlElement("Scene");

	SceneObjectSet::iterator iter;
	for (iter=m_Scene->m_SceneObjects.begin(); iter!=m_Scene->m_SceneObjects.end(); iter++)
	{
		SerializeSceneObject(*iter);
	}

	doc.InsertEndChild(*m_RootElement);
	doc.SaveFile();
	delete m_RootElement;

	return true;
}

void SceneSerializer::Unserialize(const String& filename)
{
	AssertFatal(m_Scene, "SceneSerializer::Unserialize(): Unable to deal with a NULL scene graph.");
}

void SceneSerializer::SerializeSceneObject(SceneObject* sceneObj)
{
	TiXmlElement objElem("SceneObject");
	objElem.SetAttribute("type", sceneObj->GetTypeName().Data());
	objElem.SetAttribute("position", ToString(sceneObj->Transform().GetPosition()).Data());
	objElem.SetAttribute("quaterion", ToString(sceneObj->GetLocalQuaternion()).Data());
	m_RootElement->InsertEndChild(objElem);
}


SceneObjectSet& SceneSerializer::GetSceneObjectSet()
{
	return m_Scene->m_SceneObjects;
}

SceneObjectSet& SceneSerializer::GetChildrenObjectSet(SceneObject* sceneObj)
{
	return sceneObj->m_ChildrenObjects;
}

