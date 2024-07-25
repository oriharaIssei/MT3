#pragma once

#include "Vec3.h"
#include <MyMatrix4x4.h>

#include <stdint.h>

struct ConicalPendulum{
	Vec3 anchor;
	Vec3 ballPos;

	float length;
	float halfApexAngle;
	float angle;

	float angularVelocity;

	float ballRadius = 0.08f;

	void Update();

	void Draw(const MyMatrix4x4 &viewProjMat,const MyMatrix4x4 &viewPortMat,uint32_t color);
};

