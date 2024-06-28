#include "CollisionFunc.h"

#include <algorithm>

#include <vector>

#define _USE_MATH_DEFINES
#include <cmath>

bool CollisionSphere(const Sphere &a,const Sphere &b) {
	Vec3 worldPosA = TransformVector({0.0f,0.0f,0.0f},a.worldMa);
	Vec3 worldPosB = TransformVector({0.0f,0.0f,0.0f},b.worldMa);
	if((worldPosA - worldPosB).length() < a.radius + b.radius) {
		return true;
	}

	return false;
}

bool CollisionSphere2Plane(const Sphere &sphere,const Plane &plane) {
	float distance = (sphere.transformData.translate.dot(plane.normal.Normalize())) - plane.distance;

	if(std::abs(distance) <= sphere.radius) {
		return true;
	}

	return false;
}

bool CollisionPlaneSegment(const Plane &plane,const Segment &seg) {
	float dot = plane.normal.dot(seg.diff);
	if(dot == 0.0f) {
		return false;
	}

	float t = (plane.distance - (seg.origin.dot(plane.normal))) / dot;

	if(t <= -1 || t >= 2) {
		return false;
	}

	return true;
}

bool CollisionTriangleSegment(const Triangle &tri,const Segment &seg) {
	Vec3 transformedVerts[3] = {
		TransformVector(tri.vert[0],tri.worldMat),
		TransformVector(tri.vert[1],tri.worldMat),
		TransformVector(tri.vert[2],tri.worldMat)
	};

	Vec3 triangleNormal = (transformedVerts[1] - transformedVerts[0]).Cross(transformedVerts[2] - transformedVerts[0]);

	float dot = triangleNormal.dot(seg.diff);
	if(fabs(dot) <= 0.00000001f) {
		return false;
	}

	float t = (transformedVerts[0].dot(triangleNormal) - seg.origin.dot(triangleNormal)) / dot;

	if(t < 0.0f || t > 1.0f) {
		return false;
	}

	Vec3 p = seg.origin + (seg.diff * t);

	Vec3 cross01 = (transformedVerts[1] - transformedVerts[0]).Cross(p - transformedVerts[0]);
	Vec3 cross12 = (transformedVerts[2] - transformedVerts[1]).Cross(p - transformedVerts[1]);
	Vec3 cross20 = (transformedVerts[0] - transformedVerts[2]).Cross(p - transformedVerts[2]);

	if(cross01.dot(triangleNormal) >= 0.0f &&
	   cross12.dot(triangleNormal) >= 0.0f &&
	   cross20.dot(triangleNormal) >= 0.0f) {
		return true;
	}
	return false;
}

bool CollisionAABBSphere(const AABB &aabb,const Sphere &sphere) {
	Vec3 sphereCenter = {sphere.worldMa[3][0],sphere.worldMa[3][1],sphere.worldMa[3][2]};
	Vec3 closestPoint = {
		std::clamp<float>(sphereCenter.x,aabb.min.x,aabb.max.x),
		std::clamp<float>(sphereCenter.y,aabb.min.y,aabb.max.y),
		std::clamp<float>(sphereCenter.z,aabb.min.z,aabb.max.z)
	};

	float distance = (closestPoint - sphereCenter).length();

	if(distance <= sphere.radius) {
		return true;
	}

	return false;
}

bool CollisionAABBSeg(const AABB &aabb,const Segment &seg) {
	Vec3 maxT;
	Vec3 minT;
	maxT = {
		(aabb.max.x - seg.origin.x) / seg.diff.x,
		(aabb.max.y - seg.origin.y) / seg.diff.y,
		(aabb.max.z - seg.origin.z) / seg.diff.z,
	};
	minT = {
		(aabb.min.x - seg.origin.x) / seg.diff.x,
		(aabb.min.y - seg.origin.y) / seg.diff.y,
		(aabb.min.z - seg.origin.z) / seg.diff.z,
	};

	if(std::isnan(maxT.x)) {
		maxT.x = 99;
	} else if(std::isnan(maxT.y)) {
		maxT.y = 99;
	} else if(std::isnan(maxT.z)) {
		maxT.z = 99;
	} else if(std::isnan(minT.x)) {
		minT.x = 0;
	} else if(std::isnan(minT.y)) {
		minT.y = 0;
	} else if(std::isnan(minT.z)) {
		minT.z = 0;
	}

	Vec3 tNear = {
		(std::min)(minT.x,maxT.x),
		(std::min)(minT.y,maxT.y),
		(std::min)(minT.z,maxT.z)
	};
	Vec3 tFar = {
		(std::max)(minT.x,maxT.x),
		(std::max)(minT.y,maxT.y),
		(std::max)(minT.z,maxT.z)
	};

	float tMax = (std::min)((std::min)(tFar.x,tFar.y),tFar.z);
	float tMin = (std::max)((std::max)(tNear.x,tNear.y),tNear.z);

	if(tMax >= tMin) {
		if(tMax >= 0.0f && tMax <= 1.0f) {
			return true;
		}
		if(tMin >= 0.0f && tMin <= 1.0f) {
			return true;
		}
		if(tMin <= 0.0f && tMax >= 1.0f) {
			return true;
		}
	}

	return false;
}

bool CollisionOBBSphere(const OBB &obb,const Sphere &sphere) {
	Vec3 pointFormObbLocal = TransformVector(sphere.transformData.translate,obb.worldMat.Inverse());
	AABB aabbFormObbLocal {.min = -obb.size,.max = obb.size};
	Sphere sphereFromObbLocal = sphere;
	sphereFromObbLocal.transformData.translate = pointFormObbLocal;
	sphereFromObbLocal.worldMa[3][0] = pointFormObbLocal.x;
	sphereFromObbLocal.worldMa[3][1] = pointFormObbLocal.y;
	sphereFromObbLocal.worldMa[3][2] = pointFormObbLocal.x;
	if(CollisionAABBSphere(aabbFormObbLocal,sphereFromObbLocal)) {
		return true;
	}
	return false;
}

bool CollisionOBBSegment(const OBB &obb,const Segment &segment) {
	MyMatrix4x4 inverseObbWorldMat = obb.worldMat.Inverse();
	Segment segFromObbLocal = {.origin = TransformVector(segment.origin,inverseObbWorldMat),
		.diff = TransformVector(segment.diff,inverseObbWorldMat)};
	AABB collisionAABB {
		.min = -obb.size,.max = obb.size
	};
	if(CollisionAABBSeg(collisionAABB,segFromObbLocal)) {
		return true;
	}
	return false;
}

bool CollisionOBBOBB(const OBB &a,const OBB &b) {
	Vec3 faceNormal[2][3];

	faceNormal[0][0] = a.orientations[0];
	faceNormal[0][1] = a.orientations[1];
	faceNormal[0][2] = a.orientations[2];
	faceNormal[1][0] = b.orientations[0];
	faceNormal[1][1] = b.orientations[1];
	faceNormal[1][2] = b.orientations[2];

	Vec3 min = -a.size;
	Vec3 max = a.size;
	Vec3 vertices[2][8];
	vertices[0][0] = TransformVector({min.x,min.y,min.z},a.worldMat);
	vertices[0][1] = TransformVector({max.x,min.y,min.z},a.worldMat);
	vertices[0][2] = TransformVector({min.x,max.y,min.z},a.worldMat);
	vertices[0][3] = TransformVector({max.x,max.y,min.z},a.worldMat);
	vertices[0][4] = TransformVector({min.x,min.y,max.z},a.worldMat);
	vertices[0][5] = TransformVector({max.x,min.y,max.z},a.worldMat);
	vertices[0][6] = TransformVector({min.x,max.y,max.z},a.worldMat);
	vertices[0][7] = TransformVector({max.x,max.y,max.z},a.worldMat);

	min = -b.size;
	max = b.size;
	vertices[1][0] = TransformVector({min.x,min.y,min.z},b.worldMat);
	vertices[1][1] = TransformVector({max.x,min.y,min.z},b.worldMat);
	vertices[1][2] = TransformVector({min.x,max.y,min.z},b.worldMat);
	vertices[1][3] = TransformVector({max.x,max.y,min.z},b.worldMat);
	vertices[1][4] = TransformVector({min.x,min.y,max.z},b.worldMat);
	vertices[1][5] = TransformVector({max.x,min.y,max.z},b.worldMat);
	vertices[1][6] = TransformVector({min.x,max.y,max.z},b.worldMat);
	vertices[1][7] = TransformVector({max.x,max.y,max.z},b.worldMat);

	for(int32_t row = 0; row < 2; row++) {
		for(int32_t col = 0; col < 3; col++) {
			std::vector<float> afterProjection[2];
			for(int32_t vertCol = 0; vertCol < 2; vertCol++) {
				for(int32_t vertRow = 0; vertRow < 8; vertRow++) {
					afterProjection[vertCol].push_back(vertices[vertCol][vertRow].dot(faceNormal[row][col]));
				}
			}
			float aMax = (std::ranges::max)(afterProjection[0]);
			float aMin = (std::ranges::min)(afterProjection[0]);
			float aLength = aMax - aMin;

			float bMax = (std::ranges::max)(afterProjection[1]);
			float bMin = (std::ranges::min)(afterProjection[1]);
			float bLength = bMax - bMin;

			float biggest = (std::max<float>)(aMax,bMax);
			float smallest = (std::min<float>)(aMin,bMin);
			if((biggest - smallest) > aLength + bLength) {
				return false;
			}
		}
	}

	Vec3 lineCross[3][3];
	for(size_t row = 0; row < 3; row++) {
		for(size_t col = 0; col < 3; col++) {
			lineCross[row][col] = a.orientations[row].Cross(b.orientations[col]);
		}
	}

	for(int32_t row = 0; row < 3; row++) {
		for(int32_t col = 0; col < 3; col++) {
			std::vector<float> afterProjection[2];
			for(int32_t vertCol = 0; vertCol < 2; vertCol++) {
				for(int32_t vertRow = 0; vertRow < 8; vertRow++) {
					afterProjection[vertCol].push_back(vertices[vertCol][vertRow].dot(lineCross[row][col]));
				}
			}

			float aMax = (std::ranges::max)(afterProjection[0]);
			float aMin = (std::ranges::min)(afterProjection[0]);
			float aLength = aMax - aMin;

			float bMax = (std::ranges::max)(afterProjection[1]);
			float bMin = (std::ranges::min)(afterProjection[1]);
			float bLength = bMax - bMin;

			float biggest = (std::max<float>)(aMax,bMax);
			float smallest = (std::min<float>)(aMin,bMin);

			if((biggest - smallest) > aLength + bLength) {
				return false;
			}
		}
	}

	return true;
}