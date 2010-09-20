#ifndef _IPLUGIN_H
#define _IPLUGIN_H

namespace Gen
{
	enum PluginType
	{
		PLUGIN_TYPE_INVALID = 0,
		PLUGIN_TYPE_RENDER_SYSTEM,
		PLUGIN_TYPE_AUDIO_SYSTEM,
		PLUGIN_TYPE_INPUT_SYSTEM,
	};

	class IPlugin;

	/// @brief
	/// ����ӿ�
	class IPlugin
	{
	public:
		virtual ~IPlugin() {}

		virtual PluginType GetPluginType() const = 0;
	};

	typedef IPlugin*(*CreatePluginFunc)();

}

#endif
