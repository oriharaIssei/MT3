#pragma once

#include <stdint.h>
#include "Vec3.h"
#include "MyMatrix4x4.h"

struct Pendulum{
	Vec3 anchor;

	float length;
	float angle;

	float angularAcceleration;
	float angularVelocity;

	float ballRadius = 0.08f;

	void Update();

	void Draw(const MyMatrix4x4 &viewProjMat,const MyMatrix4x4 &viewPort,uint32_t color);
};