#include "Vec3.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include <Novice.h>

Vec3 &Vec3::operator=(const Vec3 &another) {
	this->x=another.x;
	this->y=another.y;
	this->z=another.z;
	return *this;
}

Vec3 Vec3::operator+(const Vec3 &another) const {
	return Vec3(
		this->x + another.x,
		this->y + another.y,
		this->z + another.z
	);
}

Vec3 &Vec3::operator+=(const Vec3 &another) {
	this->x+=another.x;
	this->y+=another.y;
	this->z+=another.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3 &another) const {
	return Vec3(
		this->x - another.x,
		this->y - another.y,
		this->z - another.z
	);
}

Vec3 &Vec3::operator-=(const Vec3 &another) {
	this->x-=another.x;
	this->y-=another.y;
	this->z-=another.z;
	return *this;
}

Vec3 Vec3::operator*(const float &scalar) const {
	return Vec3(
		this->x * scalar,
		this->y * scalar,
		this->z * scalar
	);
}

Vec3 &Vec3::operator*=(const Vec3 &another) {
	this->x*=another.x;
	this->y*=another.y;
	this->z*=another.z;
	return *this;
}

float Vec3::length() const {
	return sqrtf(dot(*this));
}

Vec3 Vec3::Normalize() const {
	float len=this->length();
	if(len == 0.0f) {
		return *this;
	}
	return Vec3(
		this->x / len,
		this->y / len,
		this->z / len
	);
}

float Vec3::dot(const Vec3 &another) const {
	return this->x * another.x + this->y * another.y + this->z * another.z;
}

Vec3 Vec3::Cross(const Vec3 &another) const {
	return Vec3(
		y * another.z - z * another.y,
		z * another.x - x * another.z,
		x * another.y - y * another.x
	);
}

Vec3 Projection(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v2.Normalize() * (v1.dot(v2.Normalize())));
}

void ScreenPrintVector3(int x, int y, const Vec3 &vector, const char *label) {
	static constexpr int kColWidth=60;
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColWidth * 3, y, "%s", label);
}