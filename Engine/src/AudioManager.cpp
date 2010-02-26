#include "AudioManager.h"
#include "FileSystem.h"
#include "AudioBuffer.h"
#include "Debug.h"

namespace Gen
{
	AudioManager::AudioManager()
	{
	}

	bool AudioManager::CreateResourceHandles(ResourceFileNameInfo* resName)
	{
		String relativePathName = resName->path + CORRECT_SLASH + resName->filename;

		AudioBuffer* buffer = new AudioBuffer(relativePathName);
		AssertFatal(m_ResourceMap.find(resName->filename)==m_ResourceMap.end(),
					"MeshManager::CreateResourceHandles(): Specified resource name already exists.");

		m_ResourceMap[resName->filename] = buffer;
		buffer->m_ResourceName = resName->filename;

		return true;
	}

	AudioBuffer* AudioManager::GetByName(const String& resName)
	{
		return static_cast<AudioBuffer*>(GetResourceByName(resName));
	}
}
