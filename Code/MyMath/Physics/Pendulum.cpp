#include "Pendulum.h"

#include <cmath>
#include <numbers>

#include "Sphere.h"

#include "Novice.h"

constexpr float delTime = 1.0f / 60.0f;

void Pendulum::Update(){
	angularAcceleration = -(9.8f / length) * std::sinf(angle);

	angularVelocity += angularAcceleration * delTime;
	angle += angularVelocity * delTime;
}

void Pendulum::Draw(const MyMatrix4x4 &viewProjMat,const MyMatrix4x4 &viewPortMat,uint32_t color){
	Vec3 ballPos = anchor + Vec3(std::sinf(angularVelocity) * length,-std::cosf(angularVelocity) * length,0.0f);

	MyMatrix4x4 worldMat = MakeMatrix::Affine({1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},ballPos);

	Vec3 ndcStartPos = TransformVector(anchor,viewProjMat);
	Vec3 ndcEndPos = TransformVector(ballPos,viewProjMat);

	Vec3 scStartPos = TransformVector(ndcStartPos,viewPortMat);
	Vec3 scEndPos = TransformVector(ndcEndPos,viewPortMat);

	Novice::DrawLine(
		static_cast<int>(scStartPos.x),
		static_cast<int>(scStartPos.y),
		static_cast<int>(scEndPos.x),
		static_cast<int>(scEndPos.y),
		color
	);


	constexpr uint32_t kSubDivision = 16;
	// 経度分割 1 つ分の角度
	constexpr float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubDivision);
	// 緯度分割 1 つ分の角度
	constexpr float kLonEvery = std::numbers::pi_v<float> *2.0f / static_cast<float>(kSubDivision);

	Vec3 pos[3];

	for(uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex){
		// -π/2 ~ π/2
		float lat = -std::numbers::pi_v<float> / 2.0f + (kLatEvery * latIndex);
		for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex){
			float lon = lonIndex * kLonEvery;
			Vec3 a,b,c;
			a = Vec3({
				std::cosf(lat) * std::cosf(lon),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon)}) * ballRadius;

			b = Vec3({
				std::cosf(lat + kLatEvery) * std::cosf(lon),
				std::sinf(lat + kLatEvery),
				std::cosf(lat + kLatEvery) * std::sinf(lon)}) * ballRadius;

			c = Vec3({
				std::cosf(lat) * std::cosf(lon + kLonEvery),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon + kLonEvery)}) * ballRadius;

			// ndc
			pos[0] = TransformVector(a,worldMat * viewProjMat);
			pos[1] = TransformVector(b,worldMat * viewProjMat);
			pos[2] = TransformVector(c,worldMat * viewProjMat);
			// screen
			pos[0] = TransformVector(pos[0],viewPortMat);
			pos[1] = TransformVector(pos[1],viewPortMat);
			pos[2] = TransformVector(pos[2],viewPortMat);

			// ab
			Novice::DrawLine(
				static_cast<int>(pos[0].x),
				static_cast<int>(pos[0].y),
				static_cast<int>(pos[1].x),
				static_cast<int>(pos[1].y),
				color
			);
			// bc
			Novice::DrawLine(
				static_cast<int>(pos[0].x),
				static_cast<int>(pos[0].y),
				static_cast<int>(pos[2].x),
				static_cast<int>(pos[2].y),
				color
			);
		}
	}
}