//-----------------------------------------------------------------------------------
/// Color4f.h ÑÕÉ«Àà(RGBA)
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef _COLOR4F_H
#define _COLOR4F_H

class Color4f
{
public:
	float r, g, b, a;

	Color4f() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
	Color4f(float _r, float _g, float _b, float _a = 1.0f) { r = _r; g = _g; b = _b; a = _a; }
	Color4f(const Color4f& rhs) { r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; }
	Color4f(float color[]) { r = color[0]; g = color[1]; b = color[2]; a = color[3]; }

	Color4f& operator=(const Color4f& rhs) { r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; return *this; }

	bool operator==(const Color4f& rhs) const { return (r==rhs.r && g==rhs.g && b==rhs.b && a==rhs.a); }
	bool operator!=(const Color4f& rhs) const { return (r!=rhs.r || g!=rhs.g || b!=rhs.b || a!=rhs.a); }

	const float* GetArray() const { return &r; }

};

namespace Color
{
	static const Color4f RED(1.0f, 0.0f, 0.0f);
	static const Color4f GREEN(0.0f, 1.0f, 0.0f);
	static const Color4f BLUE(0.0f, 0.0f, 1.0f);
	static const Color4f BLACK(0.0f, 0.0f, 0.0f);
	static const Color4f WHITE(1.0f, 1.0f, 1.0f);

}

#endif
