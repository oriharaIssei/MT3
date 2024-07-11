#pragma once

#include <stdint.h>
#include "Vec3.h"
#include "MyMatrix4x4.h"

struct Spring {
	Vec3 anchor;
	float naturalLength;
	float stiffness;
	float dampCoefficient;

	void Draw(const Vec3 &diff,const MyMatrix4x4 &viewProjectionMat,const MyMatrix4x4 &viewPortMat,uint32_t color);
};