#include "Vec3.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <Novice.h>

Vec3& Vec3::operator=(const Vec3& another) {
	this->x = another.x;
	this->y = another.y;
	this->z = another.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3& another) {
	return Vec3(
		this->x + another.x,
		this->y + another.y,
		this->z + another.z
	);
}

Vec3& Vec3::operator+=(const Vec3& another) {
	this->x += another.x;
	this->y += another.y;
	this->z += another.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& another) {
	return Vec3(
		this->x - another.x,
		this->y - another.y,
		this->z - another.z
	);
}

Vec3& Vec3::operator-=(const Vec3& another) {
	this->x -= another.x;
	this->y -= another.y;
	this->z -= another.z;
	return *this;
}

Vec3 Vec3::operator*(const float& scalar) {
	return Vec3(
		this->x * scalar,
		this->y * scalar,
		this->z * scalar
	);
}

Vec3& Vec3::operator*=(const Vec3& another) {
	this->x *= another.x;
	this->y *= another.y;
	this->z *= another.z;
	return *this;
}

float Vec3::length() {
	return sqrtf(dot(*this));
}

Vec3 Vec3::Normalize() {
	float len = this->length();
	if (len == 0.0f) {
		return *this;
	}
	return Vec3(
		this->x / len,
		this->y / len,
		this->z / len
	);
}

float Vec3::dot(const Vec3& another) {
	return this->x * another.x + this->y * another.y + this->z * another.z;
}

void ScreenPrintVector3(int x, int y, const Vec3& vector, const char* label) {
	static constexpr int kColWidth = 60;
	Novice::ScreenPrintf(x , y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColWidth * 3, y, "%s", label);
}