#include "Lines.h"

#include <cmath>

Vec3 ClosestPoint(const Vec3 &point, const Segment &segment) {
	return segment.origin + Projection(point - segment.origin, segment.diff);
}