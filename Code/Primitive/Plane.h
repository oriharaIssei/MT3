#pragma once

#include <MyMatrix4x4.h>
#include <stdint.h>
#include <Vec3.h>

struct Plane {
	Vec3 normal;
	Vec3 points[4];
	Vec3 center;
	float distance;
	void UpdatePoints(const MyMatrix4x4 &viewProj, const MyMatrix4x4 &viewPort);
	void Draw();
};
