#pragma once

#include <MyMatrix4x4.h>
#include <Transform.h>

#include <stdint.h>

struct Sphere {
	Transform transformData;
	float radius;
	unsigned int color;
	MyMatrix4x4 worldMa;

	void Draw(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);

};
