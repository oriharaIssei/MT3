#include "Ball.h"

#include <cmath>
#include <numbers>

#include "Novice.h"

void Ball::Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa) {
	constexpr uint32_t kSubDivision = 16;
	// 経度分割 1 つ分の角度
	constexpr float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubDivision);
	// 緯度分割 1 つ分の角度
	constexpr float kLonEvery = std::numbers::pi_v<float> *2.0f / static_cast<float>(kSubDivision);

	MyMatrix4x4 worldMat = MakeMatrix::Affine({1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},pos);

	Vec3 scPos[3];

	for(uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {
		// -π/2 ~ π/2
		float lat = -std::numbers::pi_v<float> / 2.0f + (kLatEvery * latIndex);
		for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vec3 a,b,c;
			a = Vec3({
				std::cosf(lat) * std::cosf(lon),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon)}) * this->radius;

			b = Vec3({
				std::cosf(lat + kLatEvery) * std::cosf(lon),
				std::sinf(lat + kLatEvery),
				std::cosf(lat + kLatEvery) * std::sinf(lon)}) * this->radius;

			c = Vec3({
				std::cosf(lat) * std::cosf(lon + kLonEvery),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon + kLonEvery)}) * this->radius;

			// ndc
			scPos[0] = TransformVector(a,worldMat * viewProjectionMa);
			scPos[1] = TransformVector(b,worldMat * viewProjectionMa);
			scPos[2] = TransformVector(c,worldMat * viewProjectionMa);
			// screen
			scPos[0] = TransformVector(scPos[0],viewPortMa);
			scPos[1] = TransformVector(scPos[1],viewPortMa);
			scPos[2] = TransformVector(scPos[2],viewPortMa);

			// ab
			Novice::DrawLine(
				static_cast<int>(scPos[0].x),
				static_cast<int>(scPos[0].y),
				static_cast<int>(scPos[1].x),
				static_cast<int>(scPos[1].y),
				color
			);
			// bc
			Novice::DrawLine(
				static_cast<int>(scPos[0].x),
				static_cast<int>(scPos[0].y),
				static_cast<int>(scPos[2].x),
				static_cast<int>(scPos[2].y),
				color
			);
		}
	}
}
