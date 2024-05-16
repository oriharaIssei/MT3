#pragma once

struct Vec3 {
	float x, y, z;

	Vec3(float x, float y, float z):x(x), y(y), z(z) {};
	Vec3():x(0), y(0), z(0) {};

	///
	///	演算子
	///
	Vec3 &operator=(const Vec3 &another);

	Vec3 operator+(const Vec3 &another)const;
	Vec3 &operator+=(const Vec3 &another);

	Vec3 operator-(const Vec3 &another)const;
	Vec3 operator-()const { return *this * -1.0f; }
	Vec3 &operator-=(const Vec3 &another);

	Vec3 operator*(const float &scalar)const;
	Vec3 &operator*=(const Vec3 &another);

	float dot(const Vec3 &another)const;
	float length()const;
	Vec3 Normalize()const;
	Vec3 Cross(const Vec3 &another)const;
};

Vec3 Projection(const Vec3 &v1, const Vec3 &v2);

void ScreenPrintVector3(int x, int y, const Vec3 &vector, const char *label);