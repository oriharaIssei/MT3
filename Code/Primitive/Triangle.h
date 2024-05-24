#pragma once

#include "MyMatrix4x4.h"
#include "Transform.h"
#include "Vec3.h"

struct Triangle {
	Vec3 screenVert[3];
	Vec3 vert[3];
	Transform transform = { {1.0f,1.0f,1.0f} };
	MyMatrix4x4 worldMat;
	void UpdateMatrix();
	void Draw(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
};