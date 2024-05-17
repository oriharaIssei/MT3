#pragma once

#include "Vec3.h"

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
};

Vec3 ClosestPoint(const Vec3 &point, const Segment &segment);