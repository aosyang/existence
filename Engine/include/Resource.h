//-----------------------------------------------------------------------------------
/// Resource.h ��Դ�࣬�ṩ��Դ�Ĺ�������
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "EString.h"

namespace Gen
{
	class ResourceManagerBase;

	/// @brief
	/// ��Դ��
	/// @par
	///		��¼һ�������������Դ��������ļ��Ĺ���
	///		���Ը���������ļ����ػ��ߴ��ڴ��ͷ���Դ
	class Resource
	{
	public:

		/// ���캯��
		Resource();
		
		/// @brief
		///	���캯��
		/// @param filename ��Դʹ�õ��ļ���
		Resource(const String& filename);
		virtual ~Resource();

		/// @brief
		/// ������Դ
		/// @par
		///		���ļ�����Դ���ص��ڴ�
		/// @remarks
		///		����������û�����
		bool Load();

		/// @brief
		/// ж����Դ
		/// @par
		///		���ڴ����Դ�ͷ�
		/// @remarks
		///		����������û�����
		void Unload();

		/// @brief
		/// ������Դ������Դ���в��ظ��ļ��ز���
		/// @par
		///		������Դǰ�����������ȷ����Դ�Ѽ���
		/// @returns
		///		����true��ʾ�����ɹ�������false��ʾ��Դ����Ҫ����
		virtual bool Trigger();

		/// �ж���Դ�Ƿ��Ѿ�����
		bool IsLoaded() const { return m_IsResourceLoaded; }

		// TODO: ��Դ����
		//ResourceType GetType() const;

		/// ��ȡ��Դ��
		String GetResourceName() const { return m_ResourceName; }

		/// ��ȡ��Դ�ļ���
		String GetFilename() const { return m_Filename; }
	protected:

		/// @brief
		///	��Դ����ʵ�ַ���
		/// @returns
		///		�������ɹ�������true�����򷵻�false
		/// @remarks
		///		����ʵ����Դ��������������าд�˷�����ʵ���Լ����������
		virtual bool LoadImpl() { return true; }

		/// @brief
		/// ��Դж��ʵ�ַ���
		/// @remarks
		///		����ʵ����Դж�ز����������าд�˷�����ʵ���Լ���ж�ز���
		virtual void UnloadImpl() {}

	protected:
		ResourceManagerBase*	m_Manager;			///< ���������Դ�Ĺ�����

		String					m_ResourceName;		///< ��Դ����
		String					m_Filename;			///< ��Դ�ļ���

		bool					m_IsResourceLoaded;	///< ��Դ�Ƿ��Ѿ�����
	};
}

#endif
