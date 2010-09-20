//-----------------------------------------------------------------------------------
/// RefPtr.h 引用计数指针
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
	///	引用计数指针
	/// @par
	///		指针指向的RefObject在引用数量为0的时候将会自动删除
	template <typename T> class RefPtr
	{
	public:
		RefPtr()
			: m_Pointer(0)
		{
		}

		/// @brief
		/// 构造函数
		/// @par
		///		从一个普通指针转换为RefPtr
		RefPtr(T* p)
			: m_Pointer(p)
		{
			if (m_Pointer) m_Pointer->AddRef();
		}

		/// @brief
		/// 构造函数
		/// @par
		///		指向另一个同类的RefPtr的指针
		RefPtr(const RefPtr& ref)
			: m_Pointer(ref.m_Pointer)
		{
			if (m_Pointer) m_Pointer->AddRef();
		}

		/// @brief
		///	构造函数
		/// @par
		///		指向不同类型的RefPtr，根据隐式转换规则
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
			// 等号将指针复制给RefPtr
			// 释放当前指针，减少引用计数
			// 指向新指针，增加引用计数
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
		///	指针计数减一，计数为零时删除指针
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
	///	引用计数对象
	/// @remarks
	///		使用RefPtr进行引用计数的对象必须派生自这个类型
	class RefObject
	{
	public:
		RefObject() : m_RefCount(0) {}
		virtual ~RefObject() {}

		void AddRef() { m_RefCount++; }

		/// @brief
		/// 减少对象的引用
		/// @return
		///		返回true表示这个对象仍由其他RefPtr引用，false表明没有引用，需要删除
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
