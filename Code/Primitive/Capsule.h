#pragma once

#include "Vec3.h"

struct Capsule{
	Vec3 start,end;
	float radius;
	Vec3 getDiff()const{ return end - start; }
};