#pragma once

#include <MyMatrix4x4.h>
#include <Transform.h>

#include <string>

#include <stdint.h>

struct Sphere {
	Transform transformData;
	float radius;
	uint32_t color;
	MyMatrix4x4 worldMa;

	void DebugUpdate(const std::string &windowName);
	void Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa);
};
