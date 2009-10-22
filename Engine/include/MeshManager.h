#ifndef _MESHMANAGER_H
#define _MESHMANAGER_H

#include "EString.h"
#include "IMesh.h"
#include "Singleton.h"

#include <map>

using namespace std;

typedef map<const String, IMesh*>	MeshResources;

class MeshManager : public Singleton<MeshManager>
{
	friend class Singleton<MeshManager>;
public:
	static bool LoadFromFile(const String& resName, const String& filename);
protected:
	MeshManager();

	MeshResources	m_MeshResources;
};

#endif
