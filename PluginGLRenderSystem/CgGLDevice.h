//-----------------------------------------------------------------------------------
/// CgGLDevice.h Cg for OpenGL�����豸
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
		///	��ʼ��CgGL�豸
		void Initialize();

		/// @brief
		/// �ر�CgGL�豸
		void Shutdown();

		/// @brief
		/// ���Cg�Ƿ����
		static void CheckCgError();

		/// @brief
		/// ���ļ�����һ��Cg Shader
		/// @param filename
		///		�ļ���
		/// @param entry
		///		��ں�������
		/// @param type
		///		�������������س���
		/// @returns
		///		���سɹ�������Shaderָ��
		IGpuProgram* LoadGpuProgramFromFile(const String& filename, const String& entry, GpuProgramType type);

		/// @brief
		/// ����ָ��profile
		/// @param profile
		///		Ҫ������profile����
		void EnableProfile(GpuProfile profile);

		/// @brief
		/// ����ָ��profile
		/// @param profile
		///		Ҫ���õ�profile����
		void DisableProfile(GpuProfile profile);

		/// @brief
		/// ��Cg��ɫ����
		/// @param program
		///		Ҫ�󶨵�Cg��ɫ����
		void BindCgProgram(IGpuProgram* program);

		/// @brief
		/// ���ļ���ȡһ��CgFX����
		/// @param filename
		///		�����ļ���
		/// @returns
		///		���ض�ȡ��CgFX����
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
