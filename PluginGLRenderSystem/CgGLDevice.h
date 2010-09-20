//-----------------------------------------------------------------------------------
/// CgGLDevice.h Cg for OpenGL控制设备
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _CGGLDEVICE_H
#define _CGGLDEVICE_H

#include "IRenderDevice.h"

#include <Cg/cg.h>

namespace Gen
{
	class GLRenderer;
	class IGpuProgram;
	class CgEffect;

	class CgGLDevice
	{
	public:
		CgGLDevice();

		/// @brief
		///	初始化CgGL设备
		void Initialize();

		/// @brief
		/// 关闭CgGL设备
		void Shutdown();

		/// @brief
		/// 检查Cg是否出错
		static void CheckCgError();

		/// @brief
		/// 从文件载入一个Cg Shader
		/// @param filename
		///		文件名
		/// @param entry
		///		入口函数名称
		/// @param type
		///		顶点程序或是像素程序
		/// @returns
		///		返回成功创建的Shader指针
		IGpuProgram* LoadGpuProgramFromFile(const String& filename, const String& entry, GpuProgramType type);

		/// @brief
		/// 开启指定profile
		/// @param profile
		///		要开启的profile类型
		void EnableProfile(GpuProfile profile);

		/// @brief
		/// 禁用指定profile
		/// @param profile
		///		要禁用的profile类型
		void DisableProfile(GpuProfile profile);

		/// @brief
		/// 绑定Cg着色程序
		/// @param program
		///		要绑定的Cg着色程序
		void BindCgProgram(IGpuProgram* program);

		/// @brief
		/// 从文件读取一个CgFX材质
		/// @param filename
		///		材质文件名
		/// @returns
		///		返回读取的CgFX材质
		CgEffect* LoadCgEffectFromFile(const String& filename);

		static GLRenderer*		m_sGLRenderer;
	protected:
		CGcontext		m_CgContext;

		CGprofile		m_CgVertexProfile,
						m_CgFragmentProfile;

		bool			m_VPEnabled,
						m_FPEnabled;
	};
}

#endif
