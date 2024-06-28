#include "OBB.h"

#include "imgui.h"
#include "Novice.h"

void OBB::DebugUpdate(const std::string &windowName) {
#ifdef _DEBUG
	ImGui::Begin(windowName.c_str());
	ImGui::DragFloat3("Center",&center.x,0.1f);
	ImGui::DragFloat3("Size",&size.x,0.1f,1.0f);
	ImGui::DragFloat3("Rotate",&rotate.x,0.1f);
	UpdateOrientations();
	ImGui::DragFloat3("Orientation[0]",&orientations[0].x,0.1f);
	ImGui::DragFloat3("Orientation[1]",&orientations[1].x,0.1f);
	ImGui::DragFloat3("Orientation[2]",&orientations[2].x,0.1f);
	ImGui::End();
	worldMat = MakeMatrix::Affine({1.0f,1.0f,1.0f},rotate,center);
#endif // _DEBUG
}

void OBB::UpdateOrientations() {
	MyMatrix4x4 rotateMat = MakeMatrix::RotateXYZ(rotate);
	for(int i = 0; i < 3; i++) {
		orientations[i] = {rotateMat[i][0],rotateMat[i][1],rotateMat[i][2]};
	}
}

void OBB::Draw(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa) {
	Vec3 min = -size;
	Vec3 max = size;

	Vec3 vertices[8];
	vertices[0] = {min.x,min.y,min.z};
	vertices[1] = {max.x,min.y,min.z};
	vertices[2] = {min.x,max.y,min.z};
	vertices[3] = {max.x,max.y,min.z};
	vertices[4] = {min.x,min.y,max.z};
	vertices[5] = {max.x,min.y,max.z};
	vertices[6] = {min.x,max.y,max.z};
	vertices[7] = {max.x,max.y,max.z};

	for(int i = 0; i < 8; ++i) {
		vertices[i] = TransformVector(TransformVector(TransformVector(vertices[i],worldMat),viewProjectionMa),viewPortMa);
	}

	Novice::DrawLine(static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),color);
	Novice::DrawLine(static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),color);
	Novice::DrawLine(static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),color);
	Novice::DrawLine(static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),color);

	Novice::DrawLine(static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),color);
	Novice::DrawLine(static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),color);
	Novice::DrawLine(static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),color);
	Novice::DrawLine(static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),color);

	Novice::DrawLine(static_cast<int>(vertices[0].x),static_cast<int>(vertices[0].y),static_cast<int>(vertices[4].x),static_cast<int>(vertices[4].y),color);
	Novice::DrawLine(static_cast<int>(vertices[1].x),static_cast<int>(vertices[1].y),static_cast<int>(vertices[5].x),static_cast<int>(vertices[5].y),color);
	Novice::DrawLine(static_cast<int>(vertices[2].x),static_cast<int>(vertices[2].y),static_cast<int>(vertices[6].x),static_cast<int>(vertices[6].y),color);
	Novice::DrawLine(static_cast<int>(vertices[3].x),static_cast<int>(vertices[3].y),static_cast<int>(vertices[7].x),static_cast<int>(vertices[7].y),color);
}
