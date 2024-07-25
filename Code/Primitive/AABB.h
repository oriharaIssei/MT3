#pragma once

#include <MyMatrix4x4.h>
#include <stdint.h>
#include <Vec3.h>

struct AABB {
	Vec3 min;
	Vec3 max;

	Transform transform = {.scale = {1.0f,1.0f,1.0f}};

	uint32_t color = 0xffffffff;

	static bool Collision(const AABB &a,const AABB &b);
	void Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa);
};

