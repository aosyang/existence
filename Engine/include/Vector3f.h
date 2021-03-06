//-----------------------------------------------------------------------------------
/// Vector3f.h 三维向量
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#ifndef VECTOR3F_H
#define VECTOR3F_H

#include "MathUtil.h"

namespace Gen
{
	/// @brief
	/// 三维列向量
	/// @remarks
	///		引擎中的三维向量在与矩阵进行乘法运算时，向量在矩阵右侧
	class Vector3f
	{
	public:
		float x, y, z;

		Vector3f(void){};
		Vector3f(const Vector3f &a):x(a.x), y(a.y), z(a.z){}
		Vector3f(float nx, float ny, float nz):x(nx), y(ny), z(nz){}
		Vector3f(const float vec[3])
		{
			x = vec[0];
			y = vec[1];
			z = vec[2];
		}

		Vector3f &operator = (const Vector3f &a)
		{
			x = a.x; y = a.y; z = a.z;
			return *this;
		}
		bool operator ==(const Vector3f &a) const{return FLOAT_EQUAL(x, a.x) && FLOAT_EQUAL(y, a.y) && FLOAT_EQUAL(z, a.z);}
		bool operator !=(const Vector3f &a) const{return !FLOAT_EQUAL(x, a.x) || !FLOAT_EQUAL(y, a.y) || !FLOAT_EQUAL(z, a.z);}

		void zero(){ x = y = z = 0.0f;}
		Vector3f operator - () const {return Vector3f (-x, -y, -z);}
		Vector3f operator + (const Vector3f &a) const { return Vector3f (x + a.x, y + a.y, z + a.z);}
		Vector3f operator - (const Vector3f &a) const { return Vector3f (x - a.x, y - a.y, z - a.z);}
		Vector3f operator * (float a) const { return Vector3f (x * a, y * a, z * a);}
		Vector3f operator / (float a) const { float oneOverA = 1.0f / a; return Vector3f(x * oneOverA, y * oneOverA, z * oneOverA);}

		Vector3f &operator += (const Vector3f &a) { x += a.x; y += a.y; z += a.z;return *this;}
		Vector3f &operator -= (const Vector3f &a) { x -= a.x; y -= a.y; z -= a.z;return *this;}
		Vector3f &operator *= (float a) { x *= a; y *= a; z *= a;return *this;}
		Vector3f &operator /= (float a) { float oneOverA = 1.0f / a; x *= oneOverA; y *= oneOverA; z *= oneOverA; return *this;}

		inline float operator [] ( const size_t i ) const { return *(&x+i);	}
		inline float& operator [] ( const size_t i ) { return *(&x+i);	}

		inline float SquaredLength() const { return x * x + y * y + z * z; }

		/// @brief
		/// 获取向量的实际长度
		/// @remarks
		///		这个方法中含有一个开平方运算，如果仅仅作长度比较时，使用SquaredLength()完成
		/// @see
		///		Vector3f::SquaredLength()
		inline float Length() const { return sqrt(SquaredLength()); }

		/// @brief
		///	获取数组形式向量的值
		/// @returns
		///		向量数组的首地址
		inline const float* GetArray() const { return &x; }

		/// @brief
		/// 标准化向量
		/// @par
		///		等比缩放向量使向量的长度为1.0
		/// @remarks
		///		如果向量长度为0，不做任何操作
		void normalize()
		{
			float magSq = x*x + y*y + z*z;
			if (magSq > 0.0f)
			{
				float oneOverMag = 1.0f / sqrt(magSq);
				x *= oneOverMag;
				y *= oneOverMag;
				z *= oneOverMag;
			}
		}

		float operator *(const Vector3f &a) const { return x * a.x + y * a.y + z * a.z; }
	};


	inline float vectorMag(const Vector3f &a){ return sqrt(a.x*a.x + a.y*a.y + a.z*a.z); }

	inline Vector3f CrossProduct(const Vector3f &a,  const Vector3f &b)
	{
		return Vector3f(
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x);
	};

	inline Vector3f operator * (float k, const Vector3f &v){ return Vector3f(k * v.x, k * v.y, k*v.z);}

	inline float distance(const Vector3f &a, const Vector3f &b)
	{
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		float dz = a.z - b.z;

		return sqrt(dx * dx + dy * dy + dz * dz);
	}

	extern const Vector3f kZeroVector;
}

#endif
