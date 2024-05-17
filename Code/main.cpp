#include <Novice.h>

#define _USE_MATH_DEFINES
#include <cmath>
#include <stdint.h>

#include <MyMatrix4x4.h>
#include <Vec3.h>

#include <Camera.h>
#include <Lines.h>
#include <Plane.h>
#include <Sphere.h>

#include <imgui.h>

const char kWindowTitle[] = "LE2A_08_オリハライッセイ_MT3";


Vec3 Perpendicular(const Vec3 &vec);


void DrawGrid(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
void DrawSegment(const Segment p0, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);

bool CollisionSphere(const Sphere &a, const Sphere &b);
bool CollisionSphere2Plane(const Sphere &sphere, const Plane &plane);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const float kWindowWidth = 1280.0f;
	const float kWindowHeight = 720.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));

	//==============初期化==================
	Camera camera;
	camera.transform_.scale = { 1.0f,1.0f,1.0f };
	camera.transform_.rotate = { 0.26f,0.0f,0.0f };
	camera.transform_.translate = { 0.0f,1.9f,-6.49f };

	MyMatrix4x4 projectionMa = MakeMatrix::PerspectiveFov(0.45f, kWindowWidth / kWindowHeight, 0.1f, 100.0f);
	camera.vpMa_ = MakeMatrix::Affine(
		camera.transform_.scale,
		camera.transform_.rotate,
		camera.transform_.translate
	) * projectionMa;

	MyMatrix4x4 viewPortMa = MakeMatrix::ViewPort(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

	Plane plane;
	plane.normal = { 1.0f,1.0f,1.0f };
	plane.distance = 10.0f;
	plane.center = { 1.0f,1.0f,1.0f };

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while(Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		ImGui::Begin("Camera");
		ImGui::DragFloat3("Translate", &camera.transform_.translate.x, 0.01f);
		ImGui::DragFloat3("Rotate", &camera.transform_.rotate.x, 0.01f);
		ImGui::End();


		camera.vpMa_ = MakeMatrix::Affine(
			camera.transform_.scale,
			camera.transform_.rotate,
			camera.transform_.translate
		).Inverse() * projectionMa;

		plane.UpdatePoints(camera.vpMa_, viewPortMa);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera.vpMa_, viewPortMa);
		plane.Draw(BLACK);

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


void DrawGrid(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa) {
	// Grid の半分の幅
	constexpr float kGridHalfWidth = 2.0f;
	// 分割数
	constexpr uint32_t kSubDivision = 10;
	// 1 つ分の長さ
	constexpr float kGridEvery = (kGridHalfWidth * 2.0f) / static_cast<float>(kSubDivision);

	for(uint32_t xIndex = 0; xIndex <= kSubDivision; ++xIndex) {
		// 線の始点
		Vec3 startPos = { -kGridHalfWidth + kGridEvery * xIndex, 0.0f,  -kGridHalfWidth };
		// 線の終点
		Vec3 endPos = { -kGridHalfWidth + kGridEvery * xIndex, 0.0f,  kGridHalfWidth };

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos = TransformVector(startPos, viewProjectionMa);
		Vec3 ndcEndPos = TransformVector(endPos, viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos = TransformVector(ndcStartPos, viewPortMa);
		Vec3 scEndPos = TransformVector(ndcEndPos, viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x), static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x), static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
	for(uint32_t zIndex = 0; zIndex <= kSubDivision; ++zIndex) {
		// 線の始点
		Vec3 startPos = { kGridHalfWidth ,0.0f, -kGridHalfWidth + kGridEvery * zIndex };
		// 線の終点
		Vec3 endPos = { -kGridHalfWidth ,0.0f , -kGridHalfWidth + kGridEvery * zIndex };

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos = TransformVector(startPos, viewProjectionMa);
		Vec3 ndcEndPos = TransformVector(endPos, viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos = TransformVector(ndcStartPos, viewPortMa);
		Vec3 scEndPos = TransformVector(ndcEndPos, viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x), static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x), static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
}

void DrawSegment(const Segment seg, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa) {
	Vec3 ndcStartPos = TransformVector(seg.origin, viewProjectionMa);
	Vec3 ndcEndPos = TransformVector(seg.origin + seg.diff, viewProjectionMa);

	Vec3 scStartPos = TransformVector(ndcStartPos, viewPortMa);
	Vec3 scEndPos = TransformVector(ndcEndPos, viewPortMa);

	Novice::DrawLine(
		static_cast<int>(scStartPos.x),
		static_cast<int>(scStartPos.y),
		static_cast<int>(scEndPos.x),
		static_cast<int>(scEndPos.y),
		WHITE
	);
}

bool CollisionSphere(const Sphere &a, const Sphere &b) {
	Vec3 worldPosA = TransformVector({ 0.0f,0.0f,0.0f }, a.worldMa);
	Vec3 worldPosB = TransformVector({ 0.0f,0.0f,0.0f }, b.worldMa);
	if((worldPosA - worldPosB).length() < a.radius + b.radius) {
		return true;
	}

	return false;
}

bool CollisionSphere2Plane(const Sphere &sphere, const Plane &plane) {
	float distance = (sphere.transformData.translate.dot(plane.normal.Normalize())) - plane.distance;

	if(std::abs(distance) <= sphere.radius) {
		return true;
	}

	return false;
}