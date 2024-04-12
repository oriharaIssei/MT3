#pragma once

#include "Novice.h"

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

MyMatrix4x4 MakeIdentity4x4();
void ScreenPrintMatrix(const char* label,int x, int y, const MyMatrix4x4& matrix);