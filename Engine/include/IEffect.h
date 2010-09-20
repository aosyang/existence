//-----------------------------------------------------------------------------------
/// IEffect.h 特效接口，提供用于渲染模型的材质特效的公共方法
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------

#ifndef _IEFFECT_H
#define _IEFFECT_H

namespace Gen
{
	class IEffect
	{
	public:
		virtual ~IEffect() {}

		/// @brief
		/// 使用指定编号的Pass渲染以后的多边形
		/// @param i
		///		Pass编号
		virtual void UsePassState(unsigned int i) = 0;

		/// @brief
		/// 将使用了Pass设置过的渲染状态恢复原样
		/// @param i
		///		Pass编号
		virtual void ResetPassState(unsigned int i) = 0;

		/// @brief
		///	获取该材质拥有的Pass数量
		/// @returns
		///		返回Pass数量
		virtual unsigned int GetPassCount() = 0;
	};
}

#endif
