#ifndef _ESTRING_H
#define _ESTRING_H

#include <string>
#include "Color4f.h"

class String
{
public:
	String();
	String(const char* str);
	String(const std::string& str);

	String(const Color4f& color);
	String(bool val);
	String(int val);
	String(float val);

	bool operator==(const String& rhs) const;
	bool operator!=(const String& rhs) const;
	bool operator<(const String& rhs) const;
	const String operator+(const String& rhs) const;
	const String operator+=(const String& rhs);

	size_t FindFirstNotOf(const String& str) const;
	size_t FindLastNotOf(const String& str) const;
	size_t FindFirstOf(const String& str) const;
	size_t FindLastOf(const String& str) const;

	String Substr(size_t begin, size_t len=0) const;

	int Format(const char* format, ...);

	const char* Data() const;

	const Color4f ToColor4f() const;
	int ToInt() const;
	bool ToBool() const;
	float ToFloat() const;
private:
	std::string		m_String;
};

#endif
