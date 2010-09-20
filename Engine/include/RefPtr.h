//-----------------------------------------------------------------------------------
/// RefPtr.h ���ü���ָ��
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 - 2010 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _REFPTR_H
#define _REFPTR_H

namespace Gen
{
	/// @brief
	///	���ü���ָ��
	/// @par
	///		ָ��ָ���RefObject����������Ϊ0��ʱ�򽫻��Զ�ɾ��
	template <typename T> class RefPtr
	{
	public:
		RefPtr()
			: m_Pointer(0)
		{
		}

		/// @brief
		/// ���캯��
		/// @par
		///		��һ����ָͨ��ת��ΪRefPtr
		RefPtr(T* p)
			: m_Pointer(p)
		{
			if (m_Pointer) m_Pointer->AddRef();
		}

		/// @brief
		/// ���캯��
		/// @par
		///		ָ����һ��ͬ���RefPtr��ָ��
		RefPtr(const RefPtr& ref)
			: m_Pointer(ref.m_Pointer)
		{
			if (m_Pointer) m_Pointer->AddRef();
		}

		/// @brief
		///	���캯��
		/// @par
		///		ָ��ͬ���͵�RefPtr��������ʽת������
		template<class R>
		RefPtr(const RefPtr<R>& ref)
			: m_Pointer(ref.m_Pointer)
		{
			if (m_Pointer) m_Pointer->AddRef();
		}

		template<class R> friend class RefPtr;

		~RefPtr()
		{
			if (m_Pointer) Deref();
		};

		T* operator=(T* rhs)
		{
			// �ȺŽ�ָ�븴�Ƹ�RefPtr
			// �ͷŵ�ǰָ�룬�������ü���
			// ָ����ָ�룬�������ü���
			if (m_Pointer!=rhs)
			{
				if (m_Pointer) Deref();
				if (rhs) rhs->AddRef();

				m_Pointer = rhs;
			}

			return m_Pointer;
		}

		T* operator=(const RefPtr<T>& rhs)
		{
			if (m_Pointer!=rhs.m_Pointer)
			{
				if (m_Pointer) Deref();
				if (rhs.m_Pointer) rhs.m_Pointer->AddRef();

				m_Pointer = rhs.m_Pointer;
			}

			return m_Pointer;
		}

		bool operator==(const RefPtr<T>& rhs) const { return m_Pointer==rhs.m_Pointer; }
		bool operator==(T* rhs) const { return m_Pointer==rhs; }

		bool operator!=(const RefPtr<T>& rhs) const { return m_Pointer!=rhs.m_Pointer; }
		bool operator!=(T* rhs) const { return m_Pointer!=rhs; }

		bool operator>(const RefPtr<T>& rhs) const { return m_Pointer>rhs.m_Pointer; }
		bool operator>(T* rhs) const { return m_Pointer>rhs; }

		bool operator>=(const RefPtr<T>& rhs) const { return m_Pointer>=rhs.m_Pointer; }
		bool operator>=(T* rhs) const { return m_Pointer>=rhs; }

		bool operator<(const RefPtr<T>& rhs) const { return m_Pointer<rhs.m_Pointer; }
		bool operator<(T* rhs) const { return m_Pointer<rhs; }

		bool operator<=(const RefPtr<T>& rhs) const { return m_Pointer<=rhs.m_Pointer; }
		bool operator<=(T* rhs) const { return m_Pointer<=rhs; }

		T& operator*() const { return *m_Pointer; }
		T* operator->() const { return m_Pointer; }

		bool operator!() const { return !m_Pointer; }
		operator bool() const { return (m_Pointer!=0); }

		/// @brief
		///	ָ�������һ������Ϊ��ʱɾ��ָ��
		void Deref()
		{
			if (!m_Pointer->DecreaseRef())
				delete m_Pointer;
			m_Pointer = 0;
		}

	private:
		T*		m_Pointer;
	};

	/// @brief
	///	���ü�������
	/// @remarks
	///		ʹ��RefPtr�������ü����Ķ�������������������
	class RefObject
	{
	public:
		RefObject() : m_RefCount(0) {}
		virtual ~RefObject() {}

		void AddRef() { m_RefCount++; }

		/// @brief
		/// ���ٶ��������
		/// @return
		///		����true��ʾ���������������RefPtr���ã�false����û�����ã���Ҫɾ��
		bool DecreaseRef()
		{
			m_RefCount--;
			if (!m_RefCount) return false;
			return true;
		}
	private:
		unsigned int m_RefCount;
	};
}

#endif
