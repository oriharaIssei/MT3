#include "Plane.h"

#include <myMath.h>

#include <imgui.h>
#include <Novice.h>

void Plane::Draw(uint32_t color) {

	Novice::DrawLine(
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
		RED
	);
	Novice::DrawLine(
		static_cast<int>(points[0].x),
		static_cast<int>(points[0].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		WHITE
	);
	Novice::DrawLine(
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		static_cast<int>(points[2].x),
		static_cast<int>(points[2].y),
		GREEN
	);

	Novice::DrawLine(
		static_cast<int>(points[3].x),
		static_cast<int>(points[3].y),
		static_cast<int>(points[1].x),
		static_cast<int>(points[1].y),
		BLACK
	);
	color;
}

void Plane::UpdatePoints(const MyMatrix4x4 &viewProj, const MyMatrix4x4 &viewPort) {

	ImGui::Begin("Plane");
	ImGui::SliderFloat3("Normal ", &this->normal.x, -1.0f, 1.0f);
	ImGui::DragFloat(" Distance ", &this->distance, 0.1f);
	this->normal = this->normal.Normalize();
	ImGui::End();

	center = this->normal * this->distance;
	Vec3 perpendiculars[4];

	perpendiculars[0] = Perpendicular(this->normal).Normalize();
	perpendiculars[1] = -perpendiculars[0];
	perpendiculars[2] = this->normal.Cross(perpendiculars[0]);
	perpendiculars[3] = -perpendiculars[2];

	Vec3 extend;
	Vec3 point;
	for(int32_t i = 0; i < 4; ++i) {
		extend = perpendiculars[i] * 2.0f;
		point = center + extend;
		points[i] = TransformVector(TransformVector(point, viewProj), viewPort);
	}
}
