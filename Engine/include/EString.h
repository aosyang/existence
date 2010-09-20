//-----------------------------------------------------------------------------------
/// EString.h 字符串类
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _ESTRING_H
#define _ESTRING_H

#include <string>
#include "Color4f.h"

namespace Gen
{
	class String
	{
	public:
		String();
		String(const char* str);
		String(const std::string& str);

		String(const Color4f& color);
		String(bool val);
		String(int val);
		String(unsigned int val);
		String(long val);
		String(unsigned long val);
		String(float val);
		String(double val);

		bool operator==(const String& rhs) const;
		bool operator!=(const String& rhs) const;
		bool operator<(const String& rhs) const;
		const String operator+(const String& rhs) const;
		const String operator+=(const String& rhs);

		const char& operator[](size_t pos) const;
		char& operator[](size_t pos);

		size_t Find(const String& str, size_t pos=0) const;

		size_t FindFirstNotOf(const String& str) const;
		size_t FindLastNotOf(const String& str) const;
		size_t FindFirstOf(const String& str) const;
		size_t FindLastOf(const String& str) const;

		String Substr(size_t begin, size_t len=npos) const;

		String& Replace(size_t pos, size_t len, const String& str);

		// 返回字串的长度
		size_t Size() const;

		// 去除字串首尾多余的空格、制表符、换行符
		void Trim();

		void ToLowerCase();

		// 字串格式化
		int Format(const char* format, ...);

		const char* Data() const;

		const Color4f ToColor4f() const;
		int ToInt() const;
		bool ToBool() const;
		float ToFloat() const;

	private:
		std::string		m_String;
	public:
		static size_t npos;
	};
}

#endif
