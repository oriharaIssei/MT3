#pragma once

#include "Lines.h"
#include "Triangle.h"
#include "Plane.h"
#include "OBB.h"
#include "AABB.h"
#include "Sphere.h"

bool CollisionSphere(const Sphere &a,const Sphere &b);
bool CollisionSphere2Plane(const Sphere &sphere,const Plane &plane);
bool CollisionPlaneSegment(const Plane &plane,const Segment &seg);
bool CollisionTriangleSegment(const Triangle &tri,const Segment &seg);
bool CollisionAABBSphere(const AABB &aabb,const Sphere &sphere);
bool CollisionAABBSeg(const AABB &aabb,const Segment &seg);
bool CollisionOBBSphere(const OBB &obb,const Sphere &sphere);
bool CollisionOBBSegment(const OBB &obb,const Segment &segment);
bool CollisionOBBOBB(const OBB &a,const OBB &b);