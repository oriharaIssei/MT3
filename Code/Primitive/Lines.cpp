#include "Lines.h"

#include <cmath>

#include <Novice.h>

Vec3 ClosestPoint(const Vec3 &point, const Segment &segment) {
	return segment.origin + Projection(point - segment.origin, segment.diff);
}

void Segment::Draw(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa) {
	Vec3 ndcStartPos = TransformVector(origin, viewProjectionMa);
	Vec3 ndcEndPos = TransformVector(origin + diff, viewProjectionMa);

	Vec3 scStartPos = TransformVector(ndcStartPos, viewPortMa);
	Vec3 scEndPos = TransformVector(ndcEndPos, viewPortMa);

	Novice::DrawLine(
		static_cast<int>(scStartPos.x),
		static_cast<int>(scStartPos.y),
		static_cast<int>(scEndPos.x),
		static_cast<int>(scEndPos.y),
		color
	);
}