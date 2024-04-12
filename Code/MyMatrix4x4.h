#pragma once

#include "Novice.h"
#include "Vec3.h"

struct MyMatrix4x4 {
	float m[4][4];

	///
	///	演算子
	///
	float* operator[](int index) {
		return m[index];
	}
	const float* operator[](int index) const{
		return m[index];
	}

	MyMatrix4x4 operator+(const MyMatrix4x4& another)const;
	MyMatrix4x4 operator-(const MyMatrix4x4& another)const;
	MyMatrix4x4 operator*(const MyMatrix4x4& another)const;
	MyMatrix4x4 operator*(const float& scalar)const;

	MyMatrix4x4 Transpose();
	
    //Determinant
	float det() const;
	MyMatrix4x4 Inverse()const;
};

namespace MakeMatrix {
	MyMatrix4x4 Identity();
	MyMatrix4x4 Translate(const Vec3 &vec);
	MyMatrix4x4 Scale(const Vec3 &vec);
	MyMatrix4x4 RotateX(const float &radian);
	MyMatrix4x4 RotateY(const float &radian);
	MyMatrix4x4 RotateZ(const float &radian);
	MyMatrix4x4 RotateXYZ(const Vec3 &radian);
	MyMatrix4x4 RotateXYZ(const MyMatrix4x4 &x, const MyMatrix4x4 &y, const MyMatrix4x4 &z);
	MyMatrix4x4 Affine(const Vec3 &scale, const Vec3 &rotate, const Vec3 &translate);

	MyMatrix4x4 PerspectiveFov(const float &fovY, const float &aspectRatio, const float &nearClip, const float &farClip);
	MyMatrix4x4 Orthographic(const float &left, const float &top, const float &right, const float &bottom, const float &nearClip, const float &farClip);
	MyMatrix4x4 ViewPort(const float &left, const float &top, const float &width, const float &height, const float &minDepth, const float &maxDepth);
}

Vec3 Transform(const Vec3 &vec, const MyMatrix4x4 &matrix);

void ScreenPrintMatrix(const char* label,int x, int y, const MyMatrix4x4& matrix);