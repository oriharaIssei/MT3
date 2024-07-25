#pragma once

#include "stdint.h"
#include "Vec3.h"
#include "MyMatrix4x4.h"

struct Ball {
	Vec3 pos;
	float radius;
	float mass;
	Vec3 acceleration;
	Vec3 velocity;

	uint32_t color;

	void Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa);
};