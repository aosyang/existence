//-----------------------------------------------------------------------------------
/// EString.cpp 字符串类实现
///
/// File Encoding : GB2312
///
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "EString.h"
#include <stdarg.h>
#include <stdlib.h>

size_t String::npos = std::string::npos;

String::String()
{}

String::String(const char* str)
: m_String(str)
{}

String::String(const std::string& str)
: m_String(str)
{}

String::String(const Color4f& color)
{
	Format("%f %f %f %f", color.r, color.g, color.b, color.a);
}

String::String(bool val)
{
	m_String = val ? "true" : "false";
}

String::String(int val)
{
	Format("%d", val);
}

String::String(float val)
{
	Format("%f", val);
}


bool String::operator==(const String& rhs) const
{
	return (this->m_String == rhs.m_String);
}

bool String::operator!=(const String& rhs) const
{
	return (this->m_String != rhs.m_String);
}

bool String::operator<(const String& rhs) const
{
	return (this->m_String < rhs.m_String);
}

const String String::operator+(const String& rhs) const
{
	return m_String + rhs.m_String;
}

const String String::operator+=(const String& rhs)
{
	m_String += rhs.m_String;
	return *this;
}

const char& String::operator[](size_t pos) const
{
	return m_String[pos];
}

char& String::operator[](size_t pos)
{
	return m_String[pos];
}

size_t String::Find(const String& str, size_t pos) const
{
	return m_String.find(str.m_String, pos);
}

size_t String::FindFirstNotOf(const String& str) const
{
	return m_String.find_first_not_of(str.m_String);
}

size_t String::FindLastNotOf(const String& str) const
{
	return m_String.find_last_not_of(str.m_String);
}

size_t String::FindFirstOf(const String& str) const
{
	return m_String.find_first_of(str.m_String);
}

size_t String::FindLastOf(const String& str) const
{
	return m_String.find_last_of(str.m_String);
}

String String::Substr(size_t begin, size_t len) const
{
	if (len)
		return String(m_String.substr(begin, len));
	return String(m_String.substr(begin));
}

String& String::Replace(size_t pos, size_t len, const String& str)
{
	m_String.replace(pos, len, str.m_String);

	return *this;
}

size_t String::Size() const
{
	return m_String.size();
}

void String::Trim()
{
	size_t header, tail;
	header = this->FindFirstNotOf(" \t\r");
	tail = this->FindLastNotOf(" \t\r");
	*this = this->Substr(header, tail - header + 1);
}

void String::ToLowerCase()
{
	for (size_t n=0; n<m_String.size(); n++)
	{
		if (m_String[n]>='A' && m_String[n]<='Z')
			m_String[n] += 'a' - 'A';
	}
}

int String::Format(const char* format, ...)
{
	// 分配一个定长的缓冲
	static char buf[1024];

	va_list arglst;
	va_start(arglst, format);

	int result = vsprintf(buf, format, arglst);

	va_end(arglst);

	m_String = buf;
	return result;
}

const char* String::Data() const
{
	return m_String.data();
}

const Color4f String::ToColor4f() const
{
	float r, g, b, a;
	sscanf(m_String.data(), "%f %f %f %f", &r, &g, &b, &a);

	return Color4f(r, g, b, a);
}

int String::ToInt() const
{
	return atoi(m_String.c_str());
}

bool String::ToBool() const
{
	return m_String == "true";
}

float String::ToFloat() const
{
	return (float)atof(m_String.c_str());
}
