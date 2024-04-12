#pragma once

struct Vec3 {
	float x, y, z;

	Vec3(float x, float y, float z) :x(x), y(y), z(z) {};
	Vec3() :x(0), y(0), z(0) {};

	///
	///	演算子
	///
	Vec3& operator=(const Vec3& another);

	Vec3 operator+(const Vec3& another);
	Vec3& operator+=(const Vec3& another);

	Vec3 operator-(const Vec3& another);
	Vec3& operator-=(const Vec3& another);

	Vec3 operator*(const float& scalar);
	Vec3& operator*=(const Vec3& another);

	float dot(const Vec3& another);
	float length();
	Vec3 Normalize();
};

void ScreenPrintVector3(int x, int y, const Vec3& vector, const char* label);