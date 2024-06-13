#pragma once

#include "MyMatrix4x4.h"
#include "Vec3.h"
#include <stdint.h>

struct OBB {
	Vec3 center;
	Vec3 rotate;
	/// <summary>
	/// 座標軸(正規化,直交必須) 
	/// </summary>
	Vec3 orientations[3];
	/// <summary>
	/// centerから面までの距離
	/// </summary>
	Vec3 size;

	uint32_t color;

	MyMatrix4x4 worldMat;

	void UpdateOrientations();
	void Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa);
};