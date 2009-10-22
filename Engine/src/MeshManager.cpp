#include "MeshManager.h"
#include "FileSystem.h"

MeshManager::MeshManager()
{
}

bool MeshManager::LoadFromFile(const String& resName, const String& filename)
{
	//MeshManager::Instance().m_MeshResources[resName]

	FileSystem::Instance().GetExtension(filename);

	return true;
}
