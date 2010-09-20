//-----------------------------------------------------------------------------------
/// RenderTarget.h ��ȾĿ���࣬ʵ��һ�����Ի������صĶ�̬����
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _RENDERTARGET_H
#define _RENDERTARGET_H

#include <map>
#include "EString.h"
#include "IRenderTarget.h"

namespace Gen
{

	// ����Manager��û�м̳й�ϵ
	#define DECLARE_RESOURCE_MANAGER(className) \
		private: \
			className(); \
			~className(); \
			\
			static className* FactoryCreate(const String& name) \
			{ \
				className* obj = new className(); \
				m_ObjectMap[name] = obj; \
				return obj; \
			} \
		public: \
			static className* GetByName(const String& name) \
			{ \
				if (m_ObjectMap.find(name)!=m_ObjectMap.end()) \
					return m_ObjectMap[name]; \
				return NULL; \
			} \
				\
			static void DestroyObject(const String& name) \
			{ \
				if (m_ObjectMap.find(name)!=m_ObjectMap.end()) \
				{ \
					className* obj = m_ObjectMap[name]; \
					m_ObjectMap.erase(name); \
					delete obj; \
				} \
			} \
				\
			static void DestroyAllObjects() \
			{ \
				std::map<const String, className*>::iterator iter; \
				for (iter=m_ObjectMap.begin(); iter!=m_ObjectMap.end(); iter++) \
				{ \
					delete iter->second; \
				} \
				m_ObjectMap.clear(); \
			} \
		private: \
			static std::map<const String, className*>		m_ObjectMap;

	#define IMPLEMENT_RESOURCE_MANAGER(className) \
		std::map<const String, className*>		className::m_ObjectMap;

	class RenderTarget
	{
		DECLARE_RESOURCE_MANAGER(RenderTarget)

	public:
		/// ����һ���µ���ȾĿ��
		static RenderTarget* CreateRenderTarget(const String& name, unsigned int width, unsigned int height, unsigned int bpp);

		/// ��ȡAPI RT
		IRenderTarget* GetDeviceRT() { return m_DeviceRT; }

		void Resize(unsigned int width, unsigned int height);

	protected:
		bool BuildRenderTarget(unsigned int width, unsigned int height, unsigned int bpp);

	protected:
		IRenderTarget*		m_DeviceRT;			///< ʹ��APIʵ�ֵ�RenderTarget����
	};
}

#endif
