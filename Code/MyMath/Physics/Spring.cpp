#include "Spring.h"

#include "Novice.h"

void Spring::Draw(const Vec3 &diff,const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa,uint32_t color) {
	Vec3 ndcStartPos = TransformVector(anchor,viewProjectionMa);
	Vec3 ndcEndPos = TransformVector(anchor + diff,viewProjectionMa);

	Vec3 scStartPos = TransformVector(ndcStartPos,viewPortMa);
	Vec3 scEndPos = TransformVector(ndcEndPos,viewPortMa);

	Novice::DrawLine(
		static_cast<int>(scStartPos.x),
		static_cast<int>(scStartPos.y),
		static_cast<int>(scEndPos.x),
		static_cast<int>(scEndPos.y),
		color
	);
}