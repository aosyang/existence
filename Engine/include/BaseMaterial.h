#ifndef _BASEMATERIAL_H
#define _BASEMATERIAL_H

#include "Resource.h"
#include "ResourceManager.h"

namespace Gen
{
	class Material;
	class MaterialManager;

	class BaseMaterial : public Resource
	{
		friend class MaterialManager;
	public:
		BaseMaterial(const String& filename) : Resource(filename) {}

		virtual void UsePassState(unsigned int i) {}
		virtual void ResetPassState(unsigned int i) {}

		virtual unsigned int GetPassCount() { return 0; }
	};


	/// @brief
	///	���ʹ�����
	/// @par
	///		���������в��ʵ�������ʹ�ù������������ʺ󣬲��ʽ��ڹ���������ʱһͬж��
	/// @see
	///		ResourceManagerBase
	class MaterialManager : public Singleton<MaterialManager>, public ResourceManagerBase
	{
		friend class Singleton<MaterialManager>;
	public:
		/// @copydoc ResourceManagerBase::CreateResourceHandles()
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// ����һ���µĲ���
		/// @brief
		/// ����һ���µĲ���
		/// @param resName
		///		�²���ʹ�õ���Դ����
		/// @returns
		///		���سɹ������Ĳ���ָ��
		/// @remarks
		///		��Դ���ƿ������գ������������Զ�����
		Material* Create(const String& resName="");

		/// @brief
		/// ��ȡָ����Դ���ƵĲ���
		/// @param resName
		///		Ҫ��ȡ�Ĳ�������
		/// @returns
		///		����ҵ�ָ��������Դ��������Դָ�룻���򷵻�NULL
		BaseMaterial* GetByName(const String& resName);
	protected:
		MaterialManager();
	};
}

#endif
