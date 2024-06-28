#include <Novice.h>

#include <vector>

#include <algorithm>

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdint.h>

#include "MyMatrix4x4.h"
#include "Vec3.h"

#include "Camera.h"

#include "Lines.h"
#include "Triangle.h"
#include "Plane.h"
#include "Sphere.h"

#include "OBB.h"
#include "AABB.h"

#include <imgui.h>

const char kWindowTitle[] = "LE2A_08_オリハライッセイ_MT3";

void DrawGrid(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa);



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int) {

	const float kWindowWidth = 1280.0f;
	const float kWindowHeight = 720.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle,static_cast<int>(kWindowWidth),static_cast<int>(kWindowHeight));

	//==============初期化==================
	Camera camera;
	camera.transform_.scale = {1.0f,1.0f,1.0f};
	camera.transform_.rotate = {0.26f,0.0f,0.0f};
	camera.transform_.translate = {0.0f,1.9f,-6.49f};

	MyMatrix4x4 projectionMa = MakeMatrix::PerspectiveFov(0.45f,kWindowWidth / kWindowHeight,0.1f,100.0f);
	camera.vpMa_ = MakeMatrix::Affine(
		camera.transform_.scale,
		camera.transform_.rotate,
		camera.transform_.translate
	) * projectionMa;

	MyMatrix4x4 viewPortMa = MakeMatrix::ViewPort(0.0f,0.0f,kWindowWidth,kWindowHeight,0.0f,1.0f);

	Sphere p0;
	Sphere p1;
	Sphere p2;

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// ウィンドウの×ボタンが押されるまでループ
	while(Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys,keys,256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Camera");
		ImGui::DragFloat3("Translate",&camera.transform_.translate.x,0.01f);
		ImGui::DragFloat3("Rotate",&camera.transform_.rotate.x,0.01f);
		ImGui::End();



		ImGui::Begin("OBB 2");
		ImGui::DragFloat3("Center",&obb2.center.x,0.1f);
		ImGui::DragFloat3("Size",&obb2.size.x,0.1f,1.0f);
		ImGui::DragFloat3("Rotate",&obb2.rotate.x,0.1f);
		obb2.UpdateOrientations();
		ImGui::DragFloat3("Orientation[0]",&obb2.orientations[0].x,0.1f);
		ImGui::DragFloat3("Orientation[1]",&obb2.orientations[1].x,0.1f);
		ImGui::DragFloat3("Orientation[2]",&obb2.orientations[2].x,0.1f);
		ImGui::End();
		obb2.worldMat = MakeMatrix::Affine({1.0f,1.0f,1.0f},obb2.rotate,obb2.center);

		camera.vpMa_ = MakeMatrix::Affine(
			camera.transform_.scale,
			camera.transform_.rotate,
			camera.transform_.translate
		).Inverse() * projectionMa;

		if(CollisionOBBOBB(obb,obb2)) {
			obb.color = RED;
		} else {
			obb.color = WHITE;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera.vpMa_,viewPortMa);
		obb.Draw(camera.vpMa_,viewPortMa);
		obb2.Draw(camera.vpMa_,viewPortMa);

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if(preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}


void DrawGrid(const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 &viewPortMa) {
	// Grid の半分の幅
	constexpr float kGridHalfWidth = 2.0f;
	// 分割数
	constexpr uint32_t kSubDivision = 10;
	// 1 つ分の長さ
	constexpr float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubDivision);

	for(uint32_t xIndex = 0; xIndex <= kSubDivision; ++xIndex) {
		// 線の始点
		Vec3 startPos = {-kGridHalfWidth + kGridEvery * xIndex,0.0f,-kGridHalfWidth};
		// 線の終点
		Vec3 endPos = {-kGridHalfWidth + kGridEvery * xIndex,0.0f,kGridHalfWidth};

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos = TransformVector(startPos,viewProjectionMa);
		Vec3 ndcEndPos = TransformVector(endPos,viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos = TransformVector(ndcStartPos,viewPortMa);
		Vec3 scEndPos = TransformVector(ndcEndPos,viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x),static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x),static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
	for(uint32_t zIndex = 0; zIndex <= kSubDivision; ++zIndex) {
		// 線の始点
		Vec3 startPos = {kGridHalfWidth,0.0f,-kGridHalfWidth + kGridEvery * zIndex};
		// 線の終点
		Vec3 endPos = {-kGridHalfWidth,0.0f,-kGridHalfWidth + kGridEvery * zIndex};

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos = TransformVector(startPos,viewProjectionMa);
		Vec3 ndcEndPos = TransformVector(endPos,viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos = TransformVector(ndcStartPos,viewPortMa);
		Vec3 scEndPos = TransformVector(ndcEndPos,viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x),static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x),static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
}

