#include "EString.h"
#include <stdarg.h>
#include <stdlib.h>

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

int String::Format(const char* format, ...)
{
	static char buf[1024];
	va_list arglst;

	va_start(arglst, format);
	int result = vsprintf(buf, format, arglst);

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
	return atof(m_String.c_str());
}
