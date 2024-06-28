#include "Sphere.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include <numbers>

#include <imgui.h>
#include <Novice.h>

void Sphere::DebugUpdate(const std::string &windowName) {
#ifdef _DEBUG
	ImGui::Begin(windowName.c_str());
	ImGui::DragFloat3("Scale",&transformData.scale.x,0.1f,1.0f);
	ImGui::DragFloat3("Rotate",&transformData.rotate.x,0.1f);
	ImGui::DragFloat3("Transform",&transformData.translate.x,0.1f);
	ImGui::DragFloat("radius",&radius,0.1f);
	ImGui::End();
	worldMa = MakeMatrix::Affine(transformData);
#endif // _DEBUG
}

void Sphere::Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa) {
	constexpr uint32_t kSubDivision = 16;
	// 経度分割 1 つ分の角度
	constexpr float kLatEvery = std::numbers::pi_v<float> / static_cast<float>(kSubDivision);
	// 緯度分割 1 つ分の角度
	constexpr float kLonEvery = std::numbers::pi_v<float> * 2.0f / static_cast<float>(kSubDivision);

	Vec3 pos[3];

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
			pos[0] = TransformVector(a,this->worldMa * viewProjectionMa);
			pos[1] = TransformVector(b,this->worldMa * viewProjectionMa);
			pos[2] = TransformVector(c,this->worldMa * viewProjectionMa);
			// screen
			pos[0] = TransformVector(pos[0],viewPortMa);
			pos[1] = TransformVector(pos[1],viewPortMa);
			pos[2] = TransformVector(pos[2],viewPortMa);

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