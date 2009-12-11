//-----------------------------------------------------------------------------------
/// MeshManager.cpp ģ�͹�����ʵ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MeshManager.h"
#include "FileSystem.h"
#include "EmdMesh.h"
#include "System.h"

namespace Gen
{
	// Mesh loading wrapper function
	bool LoadMeshes(const String& resName, const String& filename)
	{
		if (MeshManager::Instance().LoadFromFile(resName, filename))
			return true;

		return false;
	}

	MeshManager::MeshManager()
	{

	}

	void MeshManager::RegisterExtensionLoader(const String& ext, MeshLoaderFunc func)
	{
		if (m_MeshLoaders.find(ext)==m_MeshLoaders.end())
			m_MeshLoaders[ext] = func;

		//System::Instance().RegisterExtensionLoader(ext, LoadMeshes);
	}

	bool MeshManager::LoadFromFile(const String& resName, const String& filename)
	{
		String ext = FileSystem::Instance().GetExtension(filename);
		ext.ToLowerCase();

		// δע�����չ��������
		if (m_MeshLoaders.find(ext)==m_MeshLoaders.end())
			return false;

		// �������Ѵ���
		if (m_MeshResources.find(resName)!=m_MeshResources.end())
			return false;

		IMesh* mesh = (*m_MeshLoaders[ext])(filename);
		m_MeshResources[resName] = mesh;
		mesh->SetNamePtr(&m_MeshResources.find(resName)->first);

		return true;
	}

	PrimitiveMesh* MeshManager::CreatePrimitiveMesh(const String& name)
	{
		String resName;
		static int sIndex=0;
		PrimitiveMesh* res = new PrimitiveMesh();

		// ���û��ָ�����ƣ��Զ�����һ��
		if (name == "")
		{
			resName.Format("#UNNAMED%d", sIndex);
			sIndex++;

			while (m_MeshResources.find(resName)!=m_MeshResources.end())
			{
				resName.Format("#UNNAMED%d", sIndex);
				sIndex++;
			}
		}
		else
		{
			resName = name;
			CorrectSlash(resName);

			// ������Դ�Ѵ��ڣ���������
			AssertFatal(m_MeshResources.find(resName)==m_MeshResources.end(), "MeshManager::CreateCustomMesh(): Specified resource name already exists.");
		}
		m_MeshResources[resName] = res;
		res->m_Name = &(m_MeshResources.find(resName)->first);

		return res;
	}

	IMesh* MeshManager::GetByName(const String& resName)
	{
		if (m_MeshResources.find(resName)!=m_MeshResources.end())
			return m_MeshResources[resName];

		return NULL;
	}

	void MeshManager::UnloadAllResources()
	{
		for (MeshResources::iterator iter=m_MeshResources.begin();
			iter!=m_MeshResources.end();
			iter++)
		{
			delete iter->second;
		}
	}
}
