#pragma once

#include "MyMatrix4x4.h"
#include "Vec3.h"
#include <stdint.h>

// 点と点を通る線
struct Line {
	Vec3 origin;
	// 始点と終点の差分ベクトル
	Vec3 diff;
};
// 始点からある方向に伸び続ける線
struct Ray {
	Vec3 origin;
	// 始点と終点の差分ベクトル
	Vec3 diff;
};
// 点と点を結ぶ線
struct Segment {
	Vec3 origin;
	// 始点と終点の差分ベクトル
	Vec3 diff;

	uint32_t color;
	void Draw(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
};

Vec3 ClosestPoint(const Vec3 &point, const Segment &segment);