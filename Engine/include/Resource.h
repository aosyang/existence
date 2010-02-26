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

	class Resource
	{
	public:
		// ���캯��

		Resource();
		//   filename ��Դʹ�õ��ļ���
		Resource(const String& filename);
		virtual ~Resource();

		// ��Դ�������
		bool Load();

		// ж����Դ
		void Unload();

		// ������Դ��������Դǰ�����������ȷ����Դ�Ѽ���
		// ����true��ʾ�����ɹ�
		// ����false��ʾ����Ҫ����
		virtual bool Trigger();

		// �ж���Դ�Ƿ��Ѿ�����
		bool IsLoaded() const { return m_IsResourceLoaded; }

		// TODO: ��Դ����
		//ResourceType GetType() const;

		// ��ȡ��Դ��
		String GetResourceName() const { return m_ResourceName; }

		// ��ȡ��Դ�ļ���
		String GetFilename() const { return m_Filename; }
	protected:

		// ���������ش˺�����ʵ��ʵ�ʵ�����/ж�ز���
		virtual bool LoadImpl() { return true; }
		virtual void UnloadImpl() {}

	protected:
		ResourceManagerBase*	m_Manager;			///< ���������Դ�Ĺ�����

		String					m_ResourceName;		///< ��Դ����
		String					m_Filename;			///< ��Դ�ļ���

		bool					m_IsResourceLoaded;
	};
}

#endif
