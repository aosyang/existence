#include "BaseMaterial.h"
#include "FileSystem.h"
#include "Debug.h"
#include "Material.h"
#include "EffectMaterial.h"

#include "tinyxml/tinyxml.h"

namespace Gen
{
	// ----------------------------------- MaterialManager
	MaterialManager::MaterialManager()
	{
	}

	bool MaterialManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		String ext = FileSystem::Instance().GetExtension(resName->filename);
		ext.ToLowerCase();

		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		String rname;
		BaseMaterial* material = NULL;

		if (ext==".emt")
		{
			// 打开材质脚本，获取材质名称
			TiXmlDocument matDoc(relativePathName.Data());
			matDoc.LoadFile();

			TiXmlElement* elem = matDoc.FirstChildElement();

			if (!elem) return false;

			const char* name = elem->Attribute("name");
			if (!name) return false;

			material = new Material(relativePathName);

			rname = name;
		}
		else if (ext==".cgfx")
		{
			material = new EffectMaterial(relativePathName);

			rname = resName->filename;
		}

		if (material)
		{
			AssertFatal(m_ResourceMap.find(rname)==m_ResourceMap.end(),
						"MaterialManager::CreateResourceHandles(): Specified resource name already exists.");

			m_ResourceMap[rname] = material;
			material->m_ResourceName = rname;

			return true;
		}

		return false;
	}

	Material* MaterialManager::Create(const String& resName)
	{
		String name = CheckResName(resName);

		Material* material = new Material("");
		m_ResourceMap[name] = material;
		material->m_ResourceName = name;
		material->m_IsResourceLoaded = true;

		return material;
	}

	BaseMaterial* MaterialManager::GetByName(const String& resName)
	{
		return static_cast<BaseMaterial*>(GetResourceByName(resName));
	}
}
