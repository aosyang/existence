//-----------------------------------------------------------------------------------
/// IEffect.h ��Ч�ӿڣ��ṩ������Ⱦģ�͵Ĳ�����Ч�Ĺ�������
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
		/// ʹ��ָ����ŵ�Pass��Ⱦ�Ժ�Ķ����
		/// @param i
		///		Pass���
		virtual void UsePassState(unsigned int i) = 0;

		/// @brief
		/// ��ʹ����Pass���ù�����Ⱦ״̬�ָ�ԭ��
		/// @param i
		///		Pass���
		virtual void ResetPassState(unsigned int i) = 0;

		/// @brief
		///	��ȡ�ò���ӵ�е�Pass����
		/// @returns
		///		����Pass����
		virtual unsigned int GetPassCount() = 0;
	};
}

#endif
