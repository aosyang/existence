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
	///	材质管理器
	/// @par
	///		引擎中所有材质的容器，使用管理器创建材质后，材质将于管理器销毁时一同卸载
	/// @see
	///		ResourceManagerBase
	class MaterialManager : public Singleton<MaterialManager>, public ResourceManagerBase
	{
		friend class Singleton<MaterialManager>;
	public:
		/// @copydoc ResourceManagerBase::CreateResourceHandles()
		bool CreateResourceHandles(ResourceFileNameInfo* resName);

		// 创建一个新的材质
		/// @brief
		/// 创建一个新的材质
		/// @param resName
		///		新材质使用的资源名称
		/// @returns
		///		返回成功创建的材质指针
		/// @remarks
		///		资源名称可以留空，管理器将会自动分配
		Material* Create(const String& resName="");

		/// @brief
		/// 获取指定资源名称的材质
		/// @param resName
		///		要获取的材质名称
		/// @returns
		///		如果找到指定名称资源，返回资源指针；否则返回NULL
		BaseMaterial* GetByName(const String& resName);
	protected:
		MaterialManager();
	};
}

#endif
