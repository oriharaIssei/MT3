#include <Novice.h>

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

float Lerp(float p0,float p1,float t);
Vec3 Lerp(const Vec3 &p0,const Vec3 &p1,float t);

void DrawBezier(const Vec3 &p0,const Vec3 &p1,const Vec3 &p2,const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 viewPortMa,uint32_t color);

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

	Sphere p0 = {.transformData = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.8f,0.58f,1.0f}},
		.radius = 0.1f,
		.color = BLACK
	};
	Sphere p1 = {.transformData = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{1.76f,1.0f,-0.3f}},
		.radius = 0.1f,
		.color = BLACK
	};
	Sphere p2 = {.transformData = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.94f,-0.7f,2.3f}},
		.radius = 0.1f,
		.color = BLACK
	};

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

		camera.vpMa_ = MakeMatrix::Affine(
			camera.transform_.scale,
			camera.transform_.rotate,
			camera.transform_.translate
		).Inverse() * projectionMa;

		p0.DebugUpdate("p0");
		p1.DebugUpdate("p1");
		p2.DebugUpdate("p2");

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera.vpMa_,viewPortMa);
		DrawBezier(p0.transformData.translate,p1.transformData.translate,p2.transformData.translate,camera.vpMa_,viewPortMa,WHITE);
		p0.Draw(camera.vpMa_,viewPortMa);
		p1.Draw(camera.vpMa_,viewPortMa);
		p2.Draw(camera.vpMa_,viewPortMa);

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

float Lerp(float p0,float p1,float t) {
	return ((1 - t) * p0) + (t * p1);
}

Vec3 Lerp(const Vec3 &p0,const Vec3 &p1,float t) {
	return Vec3(Lerp(p0.x,p1.x,t),Lerp(p0.y,p1.y,t),Lerp(p0.z,p1.z,t));
}

void DrawBezier(const Vec3 &p0,const Vec3 &p1,const Vec3 &p2,const MyMatrix4x4 &viewProjectionMa,const MyMatrix4x4 viewPortMa,uint32_t color) {
	constexpr float division = 64;

	Vec3 start,end;
	Vec3 ndcStartPos,ndcEndPos;
	Vec3 scStartPos,scEndPos;
	float currentDivision = 0,nextDevision;

	for(uint32_t i = 0; i < static_cast<uint32_t>(division); i++) {
		nextDevision = static_cast<float>(i + 1) / division;
		start = Lerp(Lerp(p0,p1,currentDivision),Lerp(p1,p2,currentDivision),currentDivision);
		end = Lerp(Lerp(p0,p1,nextDevision),Lerp(p1,p2,nextDevision),nextDevision);

		ndcStartPos = TransformVector(start,viewProjectionMa);
		ndcEndPos = TransformVector(end,viewProjectionMa);

		scStartPos = TransformVector(ndcStartPos,viewPortMa);
		scEndPos = TransformVector(ndcEndPos,viewPortMa);

		Novice::DrawLine(
			static_cast<int>(scStartPos.x),
			static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x),
			static_cast<int>(scEndPos.y),
			color
		);

		currentDivision = nextDevision;
	}
}