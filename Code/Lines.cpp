#include "Lines.h"

#include <cmath>

Vec3 ClosestPoint(const Vec3 &point, const Segment &segment) {
	Vec3 a = point - segment.origin;
	Vec3 b = segment.diff;
	return segment.origin + Projection(point - segment.origin, segment.diff);
}