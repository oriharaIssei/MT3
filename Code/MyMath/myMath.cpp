#include "myMath.h"

Vec3 Perpendicular(const Vec3 &vec) {
	if(vec.x != 0.0f || vec.y != 0.0f) {
		return Vec3(-vec.y, vec.x, 0.0f);
	}
	return Vec3(0.0f, -vec.z, vec.y);
}