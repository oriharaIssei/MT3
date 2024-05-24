#include "Triangle.h"

#include "stdint.h"

#include <Novice.h>

void Triangle::UpdateMatrix() {
	worldMat = MakeMatrix::Affine(transform);
}

void Triangle::Draw(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa) {
	MyMatrix4x4 wvpMa = worldMat * viewProjectionMa;

	for(uint32_t i = 0; i < 3; i++) {
		Vec3 ndcPos = TransformVector(vert[i], wvpMa);
		screenVert[i] = TransformVector(ndcPos, viewPortMa);
	}

	Novice::DrawTriangle(
		static_cast<int>(this->screenVert[0].x),
		static_cast<int>(this->screenVert[0].y),
		static_cast<int>(this->screenVert[1].x),
		static_cast<int>(this->screenVert[1].y),
		static_cast<int>(this->screenVert[2].x),
		static_cast<int>(this->screenVert[2].y),
		WHITE,
		kFillModeSolid
	);
}
