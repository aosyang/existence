//-----------------------------------------------------------------------------------
/// Quaternion.cpp 四元数
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
//#include <assert.h>
//#include <math.h>
//
//#include "Quaternion.h"
//#include "MathUtil.h"
//#include "Vector3f.h"
//#include "EulerAngle.h"
//#include "Matrix3.h"
//
//const Quaternion kQuaternionIdentity = {1.0f, 0.0f, 0.0f, 0.0f};
//
//void Quaternion::setToRotateAboutX(float theta)
//{
//	float thetaOver2 = theta * 0.5f;
//
//	w = cos(thetaOver2);
//	x = sin(thetaOver2);
//	y = z = 0.0f;
//}
//
//void Quaternion::setToRotateAboutY(float theta)
//{
//	float thetaOver2 = theta * 0.5f;
//
//	w = cos(thetaOver2);
//	y = sin(thetaOver2);
//	x = z = 0.0f;
//}
//
//void Quaternion::setToRotateAboutZ(float theta)
//{
//	float thetaOver2 = theta * 0.5f;
//
//	w = cos(thetaOver2);
//	z = sin(thetaOver2);
//	x = y = 0.0f;
//}
//
//void Quaternion::setToRotateAboutAxis(const Vector3f &axis, float theta)
//{
//	assert(fabs(vectorMag(axis) - 1.0f) < 0.01f);
//
//	float thetaOver2 = theta * 0.5f;
//	float sinThetaOver2 = sin(thetaOver2);
//
//	w = cos(thetaOver2);
//	x = axis.x * sinThetaOver2;
//	y = axis.y * sinThetaOver2;
//	z = axis.z * sinThetaOver2;
//}
//
//void Quaternion::setToRotateObjectToInertial(const EulerAngle &orientation)
//{
//	float sp, sb, sh;
//	float cp, cb, ch;
//	sinCos(&sp, &cp, orientation.pitch * 0.5f);
//	sinCos(&sb, &cb, orientation.bank * 0.5f);
//	sinCos(&sh, &ch, orientation.heading * 0.05f);
//
//	w = ch * cp * cb + sh * sp * sb;
//	x = ch * sp * cb + sh * cp * sb;
//	y = - ch * sp * sb + sh * cp * cb;
//	z = - sh * sp * cb + ch * cp *sb;
//}
//
//void Quaternion::setToRotateInertialToObject(const EulerAngle &orientation)
//{
//	float sp, sb, sh;
//	float cp, cb, ch;
//	sinCos(&sp, &cp, orientation.pitch * 0.5f);
//	sinCos(&sb, &cb, orientation.bank * 0.5f);
//	sinCos(&sh, &ch, orientation.heading * 0.05f);
//
//	w = ch * cp * cb + sh * sp * sb;
//	x = - ch * sp * cb - sh * cp * sb;
//	y = ch * sp * sb - sh * cp * cb;
//	z = sh * sp * cb - ch * cp *sb;
//}
//
//// TODO: 验证正确性
//Matrix3 Quaternion::GetRotMatrix()
//{
//	Matrix3 result;
//
//	float tr = result.m11 + result.m22 + result.m33;
//	float temp = 0.0f;
//	if(tr > 0.0f)
//	{
//		temp = 0.5f / sqrtf(tr+1);
//		w = 0.25f / temp; 
//		x = (result.m23 - result.m32) * temp;
//		y = (result.m31 - result.m13) * temp;
//		z = (result.m12 - result.m21) * temp;
//	}
//	else
//	{
//		if(result.m11 > result.m22 && result.m11 > result.m33)
//		{
//			temp = 2.0f * sqrtf(1.0f + result.m11 - result.m22 - result.m33);
//			w = (result.m32 - result.m23) / temp;
//			x = 0.25f * temp;
//			y = (result.m12 + result.m21) / temp;
//			z = (result.m13 + result.m31) / temp;
//		}
//		else if( result.m22 > result.m33)
//		{
//			temp = 2.0f * sqrtf(1.0f + result.m22 - result.m11 - result.m33);
//			w = (result.m13 - result.m31) / temp;
//			x = (result.m12 + result.m21) / temp;
//			y =  0.25f * temp;
//			z = (result.m23 + result.m32) / temp;
//		}
//		else
//		{
//			temp = 2.0f * sqrtf(1.0f + result.m33 - result.m11 - result.m22);
//			w = (result.m21 - result.m12) / temp;
//			x = (result.m13 + result.m31) / temp;
//			y = (result.m23 + result.m32) / temp;
//			z = 0.25f * temp;
//		}
//	}
//
//	return result;
//}
//
//
//Quaternion Quaternion::operator *(const Quaternion &a) const
//{
//	Quaternion result;
//
//	result.w = w * a.w - x * a.x - y * a.y - z * a.z;
//	result.x = w * a.x + x * a.w + z * a.y - y * a.z;
//	result.y = w * a.y + y * a.w + x * a.z - z * a.x;
//	result.z = w * a.z + z * a.w + y * a.x - x * a.y;
//
//	return result;
//}
//
//Quaternion &Quaternion::operator *=(const Quaternion &a)
//{
//	*this = *this * a;
//	return *this;
//}
//
//void Quaternion::normalize()
//{
//	float mag = (float)sqrt(w * w + x * x + y * y + z * z);
//
//	if ( mag > 0.0f)
//	{
//		float oneOverMag = 1.0f / mag;
//		w *= oneOverMag;
//		x *= oneOverMag;
//		y *= oneOverMag;
//		z *= oneOverMag;
//	}
//	else
//	{
//		assert(false);
//		identity();
//	}
//}
//
//float Quaternion::getRotationAngle() const
//{
//	float thetaOver2 = safeAcos(w);
//
//	return thetaOver2 * 2.0f;
//}
//
//Vector3f Quaternion::getRotationAxis() const
//{
//	float sinThetaOver2Sq = 1.0f - w * w;
//
//	if(sinThetaOver2Sq <= 0.0f)return Vector3f(1.0f, 0.0f, 0.0f);
//
//	float oneOverSinThetaOver2 = 1.0f / sqrt(sinThetaOver2Sq);
//
//	return Vector3f(x * oneOverSinThetaOver2, y * oneOverSinThetaOver2, z * oneOverSinThetaOver2);
//}
//
//float dotProduct (const Quaternion &a, const Quaternion &b){return a.w * b.w + a.x * b.x + a.y * b.y + a.z * a.z;}
//
//Quaternion slerp(const Quaternion &p, const Quaternion &q, float t)
//{
//	if (t <= 0.0f) return p;
//	if (t >= 1.0f) return q;
//
//	float cosOmega = dotProduct(p, q);
//
//	float qw = q.w;
//	float qx = q.x;
//	float qy = q.y;
//	float qz = q.z;
//
//	if (cosOmega < 0.0f)
//	{
//		qw = -qw;
//		qx = -qx;
//		qy = -qy;
//		qz = -qz;
//
//		cosOmega = - cosOmega;
//	}
//
//	assert(cosOmega < 1.1f);
//
//	float k0, k1;		//计算插值片，注意检查非常接近的情况
//
//	if (cosOmega > 0.9999f)
//	{
//		k0 = 1.0f - t;
//		k1 = t;
//	}
//	else
//	{
//		float sinOmega = sqrt(1.0f - cosOmega * cosOmega);
//
//		float omega = atan2(sinOmega, cosOmega);
//
//		float oneOverSinOmega = 1.0f / sinOmega;
//
//		k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
//		k1 = sin(t * omega) * oneOverSinOmega;
//	}
//
//	Quaternion result;
//
//	result.x = k0 * p.x + k1 * qx;
//	result.y = k0 * p.y + k1 * qy;
//	result.z = k0 * p.z + k1 * qz;
//	result.w = k0 * p.w + k1 * qw;
//
//	return result;
//}
//
//Quaternion conjugare(const Quaternion &q)
//{
//	Quaternion result;
//
//	result.w = q.w;		//旋转相同
//
//	result.x = -q.x;
//	result.y = -q.y;
//	result.z = -q.z;
//
//	return result;
//}
//
//Quaternion pow(const Quaternion &q, float exponent)
//{
//	if (fabs(q.w) > 0.9999f)return q;
//
//	float alpha = acos(q.w);
//
//	float newAlpha = alpha * exponent;
//
//	Quaternion result;
//	result.w = cos(newAlpha);
//
//	float mult = sin(newAlpha) / sin(alpha);
//	result.x = q.x * mult;
//	result.y = q.y * mult;
//	result.z = q.z * mult;
//
//	return result;
//}


#include "Quaternion.h"
#include "Debug.h"

namespace Gen
{
	Quaternion::Quaternion()
		: x(0.0f), y(0.0f), z(0.0f), w(1.0f)
	{
	}

	Quaternion::Quaternion(const Matrix3& mat)
	{
		//w = sqrtf(1.0f + mat.m[0][0] + mat.m[1][1] + mat.m[2][2]) * 0.5f;
		//float wOver4 = 0.25f / w;
		//x = (mat.m[2][1] - mat.m[1][2]) * wOver4;
		//y = (mat.m[0][2] - mat.m[2][0]) * wOver4;
		//z = (mat.m[1][0] - mat.m[0][1]) * wOver4;

		//Normalize();

		float trace = mat.m[0][0] + mat.m[1][1] + mat.m[2][2]; // I removed + 1.0f; see discussion with Ethan
		if(trace > 0) {// I changed M_EPSILON to 0
			float s = 0.5f / sqrtf(trace + 1.0f);
			w = 0.25f / s;
			x = ( mat.m[2][1] - mat.m[1][2] ) * s;
			y = ( mat.m[0][2] - mat.m[2][0] ) * s;
			z = ( mat.m[1][0] - mat.m[0][1] ) * s;
		}
		else
		{
			if ( mat.m[0][0] > mat.m[1][1] && mat.m[0][0] > mat.m[2][2] ) {
				float s = 2.0f * sqrtf( 1.0f + mat.m[0][0] - mat.m[1][1] - mat.m[2][2]);
				w = (mat.m[2][1] - mat.m[1][2] ) / s;
				x = 0.25f * s;
				y = (mat.m[0][1] + mat.m[1][0] ) / s;
				z = (mat.m[0][2] + mat.m[2][0] ) / s;
			} else if (mat.m[1][1] > mat.m[2][2]) {
				float s = 2.0f * sqrtf( 1.0f + mat.m[1][1] - mat.m[0][0] - mat.m[2][2]);
				w = (mat.m[0][2] - mat.m[2][0] ) / s;
				x = (mat.m[0][1] + mat.m[1][0] ) / s;
				y = 0.25f * s;
				z = (mat.m[1][2] + mat.m[2][1] ) / s;
			} else {
				float s = 2.0f * sqrtf( 1.0f + mat.m[2][2] - mat.m[0][0] - mat.m[1][1] );
				w = (mat.m[1][0] - mat.m[0][1] ) / s;
				x = (mat.m[0][2] + mat.m[2][0] ) / s;
				y = (mat.m[1][2] + mat.m[2][1] ) / s;
				z = 0.25f * s;
			}
		}
	}

	Quaternion Quaternion::operator*(const Quaternion& rhs) const
	{
		Quaternion r;

		r.w = w * rhs.w - x * rhs.x - y * rhs.y - z * rhs.z;
		r.x = w * rhs.x + x * rhs.w + y * rhs.z - z * rhs.y;
		r.y = w * rhs.y + y * rhs.w + z * rhs.x - x * rhs.z;
		r.z = w * rhs.z + z * rhs.w + x * rhs.y - y * rhs.x;

		return r;
	}

	Quaternion Quaternion::operator*=(const Quaternion& rhs)
	{
		*this = *this * rhs;

		return (*this);
	}

	void Quaternion::Normalize()
	{
		float oneOverNorm = 1.0f / sqrtf(x * x + y * y + z * z + w * w);

		x *= oneOverNorm;
		y *= oneOverNorm;
		z *= oneOverNorm;
		w *= oneOverNorm;

		x = Math::Clamp(-1.0f, 1.0f, x);
		y = Math::Clamp(-1.0f, 1.0f, y);
		z = Math::Clamp(-1.0f, 1.0f, z);
		w = Math::Clamp(-1.0f, 1.0f, w);
	}

	// 沿局部空间轴向作旋转
	void Quaternion::CreateFromLocalAxisAngle(const Vector3f axis, float angle_rad)
	{
		float result = sinf(angle_rad / 2.0f);

		w = cosf(angle_rad / 2.0f);

		x = axis.x * result;
		y = axis.y * result;
		z = axis.z * result;

		Normalize();
	}

	Matrix3 Quaternion::GetRotationMatrix() const
	{
		Matrix3 rot;
		rot.mArray[0] = 1.0f - 2.0f * (y * y + z * z);
		rot.mArray[1] = 2.0f * ( x * y - z * w );  
		rot.mArray[2] = 2.0f * ( x * z + y * w );

		rot.mArray[3] = 2.0f * (x * y + z * w);
		rot.mArray[4] = 1.0f - 2.0f * ( x * x + z * z ); 
		rot.mArray[5] = 2.0f * ( y * z - x * w );

		rot.mArray[6] = 2.0f * (x * z - y * w);
		rot.mArray[7] = 2.0f * (z * y + x * w );  
		rot.mArray[8] = 1.0f - 2.0f * ( x * x + y * y );  

		return rot;
	}

	Quaternion Quaternion::Slerp(const Quaternion &p, const Quaternion &q, float t)
	{
		if (t <= 0.0f) return p;
		if (t >= 1.0f) return q;

		float cosOmega = DotProduct(p, q);

		float qw = q.w;
		float qx = q.x;
		float qy = q.y;
		float qz = q.z;

		if (cosOmega < 0.0f)
		{
			qw = -qw;
			qx = -qx;
			qy = -qy;
			qz = -qz;

			cosOmega = - cosOmega;
		}

		AssertFatal(cosOmega < 1.1f, "Quaternion::Slerp(): Invalid value");

		float k0, k1;		//计算插值片，注意检查非常接近的情况

		if (cosOmega > 0.9999f)
		{
			k0 = 1.0f - t;
			k1 = t;
		}
		else
		{
			float sinOmega = sqrt(1.0f - cosOmega * cosOmega);

			float omega = atan2(sinOmega, cosOmega);

			float oneOverSinOmega = 1.0f / sinOmega;

			k0 = sin((1.0f - t) * omega) * oneOverSinOmega;
			k1 = sin(t * omega) * oneOverSinOmega;
		}

		Quaternion result;

		result.x = k0 * p.x + k1 * qx;
		result.y = k0 * p.y + k1 * qy;
		result.z = k0 * p.z + k1 * qz;
		result.w = k0 * p.w + k1 * qw;

		return result;

		// --------------------------------------------------------------------

		//// quaternion to return
		//Quaternion result;

		//// Calculate angle between them.
		//float cosHalfTheta = p.w * q.w + p.x * q.x + p.y * q.y + p.z * q.z;
		//// if p=q or p=-q then theta = 0 and we can return p
		//if (abs(cosHalfTheta) >= 1.0){
		//	result.w = p.w;result.x = p.x;result.y = p.y;result.z = p.z;
		//	return result;
		//}
		//// Calculate temporary values.
		//float halfTheta = acosf(cosHalfTheta);
		//float sinHalfTheta = sqrtf(1.0f - cosHalfTheta*cosHalfTheta);
		//// if theta = 180 degrees then result is not fully defined
		//// we could rotate around any axis normal to p or q
		//if (fabs(sinHalfTheta) < 0.001){ // fabs is floating point absolute
		//	result.w = (p.w * 0.5f + q.w * 0.5f);
		//	result.x = (p.x * 0.5f + q.x * 0.5f);
		//	result.y = (p.y * 0.5f + q.y * 0.5f);
		//	result.z = (p.z * 0.5f + q.z * 0.5f);
		//	return result;
		//}
		//float ratioA = sinf((1 - t) * halfTheta) / sinHalfTheta;
		//float ratioB = sinf(t * halfTheta) / sinHalfTheta; 
		////calculate Quaternion.
		//result.w = (p.w * ratioA + q.w * ratioB);
		//result.x = (p.x * ratioA + q.x * ratioB);
		//result.y = (p.y * ratioA + q.y * ratioB);
		//result.z = (p.z * ratioA + q.z * ratioB);
		//return result;

		// --------------------------------------------------------------------

		//Quaternion result;
		//float fCos = DotProduct(p, q);
		//float fAngle = acosf(fCos);

		//if (fAngle>=0.0f)
		//{
		//	float fSin = sinf(fAngle);
		//	float fInvSin = ((float)1.0f)/fSin;
		//	float fTAngle = t * fAngle;
		//	float fCoeff0 = sinf(fAngle - fTAngle)*fInvSin;
		//	float fCoeff1 = sinf(fTAngle)*fInvSin;

		//	// Profiling showed that the old line of code,
		//	//   *this = fCoeff0*p + fCoeff1*q;
		//	// was using a large number of cycles, more so than the sin and cos
		//	// function calls.  The following inlined code is much faster.
		//	result.w = fCoeff0*p.w + fCoeff1*q.w;
		//	result.x = fCoeff0*p.x + fCoeff1*q.x;
		//	result.y = fCoeff0*p.y + fCoeff1*q.y;
		//	result.z = fCoeff0*p.z + fCoeff1*q.z;
		//}
		//else
		//{
		//	// Based on the problem with the code in the previous block, inlining
		//	// the old code
		//	//   *this = p;
		//	result.w = p.w;
		//	result.x = p.x;
		//	result.y = p.y;
		//	result.z = p.z;
		//}

		//return result;
	}

	float Quaternion::DotProduct (const Quaternion &a, const Quaternion &b){ return a.w * b.w + a.x * b.x + a.y * b.y + a.z * a.z; }
}
