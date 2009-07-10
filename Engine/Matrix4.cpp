//-----------------------------------------------------------------------------------
/// Matrix4.cpp 4x4矩阵
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "Matrix4.h"

Matrix4 Matrix4::IDENTITY = Matrix4(1.0f, 0.0f, 0.0f, 0.0f,
									0.0f, 1.0f, 0.0f, 0.0f,
									0.0f, 0.0f, 1.0f, 0.0f,
									0.0f, 0.0f, 0.0f, 1.0f);

Matrix4::Matrix4(void)
{
}

Matrix4::~Matrix4(void)
{
}

void Matrix4::Identity()
{
	m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = 0.0f;
	m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = 0.0f;
	m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = 0.0f;
	m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

Matrix4 Matrix4::operator * (const Matrix4& m2) const
{
	Matrix4 r;
	r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
	r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
	r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
	r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

	r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
	r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
	r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
	r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

	r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
	r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
	r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
	r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

	r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
	r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
	r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
	r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

	return r;
}

Vector3f Matrix4::operator * (const Vector3f& vec) const
{
	Vector3f r;

	float invW = 1.0f / (m[3][0] * vec.x + m[3][1] * vec.y + m[3][2] * vec.z + m[3][3]);

	r.x = (m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z + m[0][3]) * invW;
	r.y = (m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z + m[1][3]) * invW;
	r.z = (m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z + m[2][3]) * invW;

	return r;
}

// 矩阵对射线进行变换
Ray Matrix4::operator*(const Ray& ray) const
{
	Ray r;
	r.origin = (*this) * ray.origin;
	r.direction = this->GetRotationMatrix() * ray.direction;
	r.distance = ray.distance;

	return r;
}

Matrix4 Matrix4::operator*=(const Matrix4& m2)
{
	return (*this = *this * m2);
}

void Matrix4::TranslateWorld(const Vector3f& vec)
{
	m[0][3] += vec.x;
	m[1][3] += vec.y;
	m[2][3] += vec.z;
}

void Matrix4::TranslateLocal(const Vector3f& vec)
{
	Matrix3 rotMat = GetRotationMatrix();

	Vector3f translate = rotMat * vec;

	m[0][3] += translate.x;
	m[1][3] += translate.y;
	m[2][3] += translate.z;

	//float dx = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2];
	//float dy = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2];
	//float dz = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2];


	//m[0][3] += dx;
	//m[1][3] += dy;
	//m[2][3] += dz;

}

void Matrix4::SetLookAt(const Vector3f& direction, const Vector3f& up)
{
	Vector3f f = direction;
	Vector3f normalized_up = up;
	f.normalize();
	normalized_up.normalize();

	Vector3f s = CrossProduct(f, normalized_up);
	Vector3f u = CrossProduct(s, f);

	m[0][0] = s[0]; m[0][1] = u[0]; m[0][2] = -f[0];
	m[1][0] = s[1]; m[1][1] = u[1]; m[1][2] = -f[1];
	m[2][0] = s[2]; m[2][1] = u[2]; m[2][2] = -f[2];
}


const Vector3f Matrix4::GetRightVector() const
{
	return Vector3f(m[0][0], m[1][0], m[2][0]);
}

const Vector3f Matrix4::GetUpVector() const
{
	return Vector3f(m[0][1], m[1][1], m[2][1]);
}

const Vector3f Matrix4::GetForwardVector() const
{
	return Vector3f(-m[0][2], -m[1][2], -m[2][2]);
}


void Matrix4::SetRotation(const Matrix3& rot)
{
	m[0][0] = rot.m[0][0]; m[0][1] = rot.m[0][1]; m[0][2] = rot.m[0][2];
	m[1][0] = rot.m[1][0]; m[1][1] = rot.m[1][1]; m[1][2] = rot.m[1][2];
	m[2][0] = rot.m[2][0]; m[2][1] = rot.m[2][1]; m[2][2] = rot.m[2][2];
}

Matrix3 Matrix4::GetRotationMatrix() const
{
	Matrix3 rot;

	rot.m[0][0] = m[0][0]; rot.m[0][1] = m[0][1]; rot.m[0][2] = m[0][2];
	rot.m[1][0] = m[1][0]; rot.m[1][1] = m[1][1]; rot.m[1][2] = m[1][2];
	rot.m[2][0] = m[2][0]; rot.m[2][1] = m[2][1]; rot.m[2][2] = m[2][2];

	return rot;
}

//-----------------------------------------------------------------------------------
/// \brief
/// 求矩阵的逆矩阵
/// 
/// \returns
/// 这个矩阵的逆矩阵
/// 
/// 正交矩阵的逆矩阵等于其转置矩阵
//-----------------------------------------------------------------------------------
Matrix4 Matrix4::GetInverseMatrix() const
{
        float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
        float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
        float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
        float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];

        float v0 = m20 * m31 - m21 * m30;
        float v1 = m20 * m32 - m22 * m30;
        float v2 = m20 * m33 - m23 * m30;
        float v3 = m21 * m32 - m22 * m31;
        float v4 = m21 * m33 - m23 * m31;
        float v5 = m22 * m33 - m23 * m32;

        float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
        float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
        float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
        float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);

        float invDet = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);

        float d00 = t00 * invDet;
        float d10 = t10 * invDet;
        float d20 = t20 * invDet;
        float d30 = t30 * invDet;

        float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m10 * m31 - m11 * m30;
        v1 = m10 * m32 - m12 * m30;
        v2 = m10 * m33 - m13 * m30;
        v3 = m11 * m32 - m12 * m31;
        v4 = m11 * m33 - m13 * m31;
        v5 = m12 * m33 - m13 * m32;

        float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        v0 = m21 * m10 - m20 * m11;
        v1 = m22 * m10 - m20 * m12;
        v2 = m23 * m10 - m20 * m13;
        v3 = m22 * m11 - m21 * m12;
        v4 = m23 * m11 - m21 * m13;
        v5 = m23 * m12 - m22 * m13;

        float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * invDet;
        float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * invDet;
        float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * invDet;
        float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * invDet;

        return Matrix4(
            d00, d01, d02, d03,
            d10, d11, d12, d13,
            d20, d21, d22, d23,
            d30, d31, d32, d33);
}

//-----------------------------------------------------------------------------------
/// \brief
/// 指定变换矩阵的坐标
/// 
/// \param pos
/// 3D空间坐标
/// 
/// \remarks
/// 这个函数相当于将矩阵按照以下方法重置：<br>
/// [m00, m01, m02,  x ]<br>
/// |m10, m11, m12,  y |<br>
/// |m20, m21, m22,  z |<br>
/// [m30, m31, m31, m33]<br>
/// 
//-----------------------------------------------------------------------------------
void Matrix4::SetPosition(const Vector3f& pos)
{
	m[0][3] = pos.x;
	m[1][3] = pos.y;
	m[2][3] = pos.z;
}
