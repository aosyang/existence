#ifndef _AUDIOMANAGER_H
#define _AUDIOMANAGER_H

#include "ResourceManager.h"

namespace Gen
{
	class AudioBuffer;

	class AudioManager : public Singleton<AudioManager>, public ResourceManagerBase
	{
		friend class Singleton<AudioManager>;
	public:
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		AudioBuffer* GetByName(const String& resName);
	private:
		AudioManager();
	};
}

#endif
