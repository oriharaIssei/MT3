#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>
#include <stdint.h>

#include <MyMatrix4x4.h>
#include <Vec3.h>

#include <Camera.h>
#include <Lines.h>

const char kWindowTitle[] = "LE2A_08_オリハライッセイ_MT3";

struct Sphere {
	Transform transformData;
	float radius;
	unsigned int color;
	MyMatrix4x4 worldMa;
};
Vec3 Perpendicular(const Vec3 &vec);
struct Plane {
	Vec3 normal;
	Vec3 points[4];
	Vec3 center;
	float distance;
	void UpdatePoints(const MyMatrix4x4 &viewProj, const MyMatrix4x4 &viewPort);
	void Draw(uint32_t color);
};

void DrawGrid(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
void DrawSegment(const Segment p0, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
void DrawSphere(const Sphere &sphere, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa, uint32_t color);

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

	Sphere sphere { {
		{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,5.0f,1.0f}
		},
		5.0f,
		WHITE
	};
	sphere.worldMa = MakeMatrix::Affine(
		sphere.transformData.scale,
		sphere.transformData.rotate,
		sphere.transformData.translate
	);

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

		ImGui::Begin("Sphere");
		ImGui::DragFloat3("A_Translate", &sphere.transformData.translate.x, 0.01f);
		ImGui::DragFloat("A_Radius", &sphere.radius, 0.01f);
		ImGui::End();

		camera.vpMa_ = MakeMatrix::Affine(
			camera.transform_.scale,
			camera.transform_.rotate,
			camera.transform_.translate
		).Inverse() * projectionMa;

		sphere.worldMa = MakeMatrix::Affine(
			sphere.transformData.scale,
			sphere.transformData.rotate,
			sphere.transformData.translate
		);

		plane.UpdatePoints(camera.vpMa_, viewPortMa);
		if(CollisionSphere2Plane(sphere, plane)) {
			sphere.color = 0xff0000ff;
		} else {
			sphere.color = 0xffffffff;
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera.vpMa_, viewPortMa);
		plane.Draw(BLACK);
		DrawSphere(sphere, camera.vpMa_, viewPortMa, sphere.color);


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

Vec3 Perpendicular(const Vec3 &vec) {
	if(vec.x != 0.0f || vec.y != 0.0f) {
		return Vec3(-vec.y, vec.x, 0.0f);
	}
	return Vec3(0.0f, -vec.z, vec.y);
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

void DrawSphere(const Sphere &sphere, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa, uint32_t color) {
	constexpr uint32_t kSubDivision = 16;
	// 経度分割 1 つ分の角度
	constexpr float kLatEvery = static_cast<float>(M_PI) / static_cast<float>(kSubDivision);
	// 緯度分割 1 つ分の角度
	constexpr float kLonEvery = static_cast<float>(M_PI) * 2.0f / static_cast<float>(kSubDivision);

	Vec3 pos[3];

	for(uint32_t latIndex = 0; latIndex < kSubDivision; ++latIndex) {
		// -π/2 ~ π/2
		float lat = -static_cast<float>(M_PI) / 2.0f + (kLatEvery * latIndex);
		for(uint32_t lonIndex = 0; lonIndex < kSubDivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery;
			Vec3 a, b, c;
			a = Vec3({
				std::cosf(lat) * std::cosf(lon),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon) }) * sphere.radius;

			b = Vec3({
				std::cosf(lat + kLatEvery) * std::cosf(lon),
				std::sinf(lat + kLatEvery),
				std::cosf(lat + kLatEvery) * std::sinf(lon) }) * sphere.radius;

			c = Vec3({
				std::cosf(lat) * std::cosf(lon + kLonEvery),
				std::sinf(lat),
				std::cosf(lat) * std::sinf(lon + kLonEvery) }) * sphere.radius;

			// ndc
			pos[0] = TransformVector(a, sphere.worldMa * viewProjectionMa);
			pos[1] = TransformVector(b, sphere.worldMa * viewProjectionMa);
			pos[2] = TransformVector(c, sphere.worldMa * viewProjectionMa);
			// screen
			pos[0] = TransformVector(pos[0], viewPortMa);
			pos[1] = TransformVector(pos[1], viewPortMa);
			pos[2] = TransformVector(pos[2], viewPortMa);

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
