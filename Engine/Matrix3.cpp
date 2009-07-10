//-----------------------------------------------------------------------------------
/// Matrix3.cpp 3x3矩阵
/// 
/// File Encoding : GB2312
/// 
/// Copyright (c) 2009 by Mwolf
//-----------------------------------------------------------------------------------
#include "MathUtil.h"
#include "Matrix3.h"

Matrix3 Matrix3::operator* (const Matrix3& rkMatrix) const
{
	Matrix3 kProd;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
		{
			kProd.m[iRow][iCol] =
				m[iRow][0]*rkMatrix.m[0][iCol] +
				m[iRow][1]*rkMatrix.m[1][iCol] +
				m[iRow][2]*rkMatrix.m[2][iCol];
		}
	}
	return kProd;
}

Vector3f Matrix3::operator* (const Vector3f& rkPoint) const
{
	Vector3f kProd;
	for (size_t iRow = 0; iRow < 3; iRow++)
	{
		kProd[iRow] =
			m[iRow][0]*rkPoint[0] +
			m[iRow][1]*rkPoint[1] +
			m[iRow][2]*rkPoint[2];
	}
	return kProd;
}

void Matrix3::Identity()
{
	m[0][0] = m[1][1] = m[2][2] = 1.0f;
	m[0][1] = m[0][2] = m[1][0] = m[1][2] = m[2][0] = m[2][1] = 0.0f;
}

Matrix3 Matrix3::GetInverseMatrix() const
{
	Matrix3 rkInverse;
	rkInverse[0][0] = m[1][1]*m[2][2] -
		m[1][2]*m[2][1];
	rkInverse[0][1] = m[0][2]*m[2][1] -
		m[0][1]*m[2][2];
	rkInverse[0][2] = m[0][1]*m[1][2] -
		m[0][2]*m[1][1];
	rkInverse[1][0] = m[1][2]*m[2][0] -
		m[1][0]*m[2][2];
	rkInverse[1][1] = m[0][0]*m[2][2] -
		m[0][2]*m[2][0];
	rkInverse[1][2] = m[0][2]*m[1][0] -
		m[0][0]*m[1][2];
	rkInverse[2][0] = m[1][0]*m[2][1] -
		m[1][1]*m[2][0];
	rkInverse[2][1] = m[0][1]*m[2][0] -
		m[0][0]*m[2][1];
	rkInverse[2][2] = m[0][0]*m[1][1] -
		m[0][1]*m[1][0];

	float fDet =
		m[0][0]*rkInverse[0][0] +
		m[0][1]*rkInverse[1][0]+
		m[0][2]*rkInverse[2][0];

	float fInvDet = 1.0f/fDet;
	for (unsigned int iRow = 0; iRow < 3; iRow++)
	{
		for (size_t iCol = 0; iCol < 3; iCol++)
			rkInverse[iRow][iCol] *= fInvDet;
	}

	return rkInverse;
}

void Matrix3::setup(const EulerAngle &orientation)
{
	float sh,ch, sp,cp, sb,cb;
	sinCos(&sh, &ch, orientation.heading);
	sinCos(&sp, &cp, orientation.pitch);
	sinCos(&sb, &cb, orientation.bank);

	m[0][0] = ch * cb + sh * sp *sb;
	m[0][1] = - ch * sb + sh * sp * cb;
	m[0][2] = sh * cp;

	m[1][0] = sb * cp;
	m[1][1] = cb * cp;
	m[1][2] = -sp;

	m[2][0] = -sh * cb + ch * sp * cb;
	m[2][1] = sb * sh + ch * sp * cb;
	m[2][2] = ch * cp;
}


void Matrix3::fromInertialToObjectQuaternion(const Quaternion &q)
{
	m[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	m[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
	m[0][2] = 2.0f * (q.x * q.z - q.w * q.y);

	m[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
	m[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	m[1][2] = 2.0f * (q.y * q.z + q.w * q.x);

	m[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
	m[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
	m[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

void Matrix3::fromObjectToInertialQuaternion(const Quaternion &q)
{
	m[0][0] = 1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	m[0][1] = 2.0f * (q.x * q.y - q.w * q.z);
	m[0][2] = 2.0f * (q.x * q.z + q.w * q.y);

	m[1][0] = 2.0f * (q.x * q.y + q.w * q.z);
	m[1][1] = 1.0f - 2.0f * (q.x * q.x + q.z * q.z);
	m[1][2] = 2.0f * (q.y * q.z - q.w * q.x);

	m[2][0] = 2.0f * (q.x * q.z - q.w * q.y);
	m[2][1] = 2.0f * (q.y * q.z + q.w * q.x);
	m[2][2] = 1.0f - 2.0f * (q.x * q.x + q.y * q.y);
}

Vector3f Matrix3::InertialToObject(const Vector3f &v) const
{
	return Vector3f(
		m[0][0] * v.x + m[1][0] * v.y + m[2][0] * v.z,
		m[0][1] * v.x + m[1][1] * v.y + m[2][1] * v.z,
		m[0][2] * v.x + m[1][2] * v.y + m[2][2] * v.z
		);
}

Vector3f Matrix3::ObjectToInertial(const Vector3f &v) const
{
	return Vector3f(
		m[0][0] * v.x + m[1][0] * v.y + m[0][2] * v.z,
		m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
		m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
		);
}

//     |y
//     |
//     |
//     |
//     |          x
//     +-----------
//    /
//   /
//  /
// /z
//
// 根据右手螺旋定则，分别以三个坐标轴正向为旋转轴正向，对应值增加的方向与旋转方向一致
// Yaw   : Y轴正方向，又称Heading
// Pitch : X轴正方向
// Roll  : Z轴正方向

Matrix3 Matrix3::BuildYawRotationMatrix(float rad_angle)
{
	Matrix3 rot;
	rot.m[0][0] = cosf(rad_angle);	rot.m[0][1] = 0.0f;			rot.m[0][2] = sinf(rad_angle);
	rot.m[1][0] = 0.0f;			rot.m[1][1] = 1.0f;			rot.m[1][2] = 0.0f;
	rot.m[2][0] = -sinf(rad_angle);	rot.m[2][1] = 0.0f;			rot.m[2][2] = cos(rad_angle);

	return rot;
}

Matrix3 Matrix3::BuildPitchRotationMatrix(float rad_angle)
{
	Matrix3 rot;
	rot.m[0][0] = 1.0f;			rot.m[0][1] = 0.0f;			rot.m[0][2] = 0.0f;
	rot.m[1][0] = 0.0f;			rot.m[1][1] = cosf(rad_angle);	rot.m[1][2] = -sinf(rad_angle);
	rot.m[2][0] = 0.0f;			rot.m[2][1] = sinf(rad_angle);	rot.m[2][2] = cos(rad_angle);

	return rot;
}

Matrix3 Matrix3::BuildRollRotationMatrix(float rad_angle)
{
	Matrix3 rot;
	rot.m[0][0] = cos(rad_angle);	rot.m[0][1] = -sinf(rad_angle);	rot.m[0][2] = 0.0f;
	rot.m[1][0] = sinf(rad_angle);	rot.m[1][1] = cosf(rad_angle);	rot.m[1][2] = 0.0f;
	rot.m[2][0] = 0.0f;			rot.m[2][1] = 0.0f;			rot.m[2][2] = 1.0f;

	return rot;
}

