#include <Novice.h>
#define _USE_MATH_DEFINES
#include <cmath>
#include <imgui.h>
#include <stdint.h>

#include <MyMatrix4x4.h>
#include <Vec3.h>

const char kWindowTitle[]="LE2A_08_オリハライッセイ_MT3";

void DrawGrid(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);

struct TransformData {
	Vec3 scale;
	Vec3 rotate;
	Vec3 translate;
};

struct Camera {
	TransformData transformData;
	MyMatrix4x4 vpMa;
};
struct Sphere {
	TransformData transformData;
	float radius;
	MyMatrix4x4 worldMa;
};
void DrawGrid(const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa);
void DrawSphere(const Sphere &sphere, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa, uint32_t color);

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const float kWindowWidth=1280.0f;
	const float kWindowHeight=720.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));
	
	//==============初期化====================
	Camera camera;
	camera.transformData.scale={ 1.0f,1.0f,1.0f };
	camera.transformData.rotate={ 0.26f,0.0f,0.0f };
	camera.transformData.translate={ 0.0f,1.9f,-6.49f };

	Sphere sphere;
	sphere.transformData.scale={ 1.0f,1.0f,1.0f };
	sphere.transformData.rotate={ 0.0f,0.0f,0.0f };
	sphere.transformData.translate={ 0.0f,0.0f,1.0f };
	sphere.radius=1.0f;

	MyMatrix4x4 projectionMa=MakeMatrix::PerspectiveFov(0.45f, kWindowWidth / kWindowHeight, 0.1f, 100.0f);
	camera.vpMa=MakeMatrix::Affine(
		camera.transformData.scale,
		camera.transformData.rotate,
		camera.transformData.translate
	) * projectionMa;

	MyMatrix4x4 viewPortMa=MakeMatrix::ViewPort(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

	// キー入力結果を受け取る箱
	char keys[256]={ 0 };
	char preKeys[256]={ 0 };

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

		ImGui::Begin("window");
		ImGui::DragFloat3("camera Translate", &camera.transformData.translate.x, 0.01f);
		ImGui::DragFloat3("camera Rotate", &camera.transformData.rotate.x, 0.01f);
		ImGui::DragFloat3("sphere CenterPos", &sphere.transformData.translate.x, 0.01f);
		ImGui::DragFloat3("sphere Rotate", &sphere.transformData.rotate.x, 0.01f);
		ImGui::End();

		camera.vpMa=MakeMatrix::Affine(
			camera.transformData.scale,
			camera.transformData.rotate,
			camera.transformData.translate
		).Inverse() * projectionMa;

		sphere.worldMa=MakeMatrix::Affine({ 1.0f,1.0f ,1.0f }, sphere.transformData.rotate, sphere.transformData.translate);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(camera.vpMa, viewPortMa);
		DrawSphere(sphere, camera.vpMa, viewPortMa, BLACK);

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
	constexpr float kGridHalfWidth=2.0f;
	// 分割数
	constexpr uint32_t kSubDivision=10;
	// 1 つ分の長さ
	constexpr float kGridEvery=(kGridHalfWidth * 2.0f) / static_cast<float>(kSubDivision);

	for(uint32_t xIndex=0; xIndex <= kSubDivision; ++xIndex) {
		// 線の始点
		Vec3 startPos={ -kGridHalfWidth + kGridEvery * xIndex, 0.0f,  -kGridHalfWidth };
		// 線の終点
		Vec3 endPos={ -kGridHalfWidth + kGridEvery * xIndex, 0.0f,  kGridHalfWidth };

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos=Transform(startPos, viewProjectionMa);
		Vec3 ndcEndPos=Transform(endPos, viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos=Transform(ndcStartPos, viewPortMa);
		Vec3 scEndPos=Transform(ndcEndPos, viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x), static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x), static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
	for(uint32_t zIndex=0; zIndex <= kSubDivision; ++zIndex) {
		// 線の始点
		Vec3 startPos={ kGridHalfWidth ,0.0f, -kGridHalfWidth + kGridEvery * zIndex };
		// 線の終点
		Vec3 endPos={ -kGridHalfWidth ,0.0f , -kGridHalfWidth + kGridEvery * zIndex };

		// 始点と終点をビュープロジェクション変換
		Vec3 ndcStartPos=Transform(startPos, viewProjectionMa);
		Vec3 ndcEndPos=Transform(endPos, viewProjectionMa);

		// NDCからスクリーン座標へ変換
		Vec3 scStartPos=Transform(ndcStartPos, viewPortMa);
		Vec3 scEndPos=Transform(ndcEndPos, viewPortMa);

		// 線を描画
		Novice::DrawLine(
			static_cast<int>(scStartPos.x), static_cast<int>(scStartPos.y),
			static_cast<int>(scEndPos.x), static_cast<int>(scEndPos.y),
			0xaaaaaaff
		);
	}
}

void DrawSphere(const Sphere &sphere, const MyMatrix4x4 &viewProjectionMa, const MyMatrix4x4 &viewPortMa, uint32_t color) {
	constexpr uint32_t kSubDivision=16;
	// 経度分割 1 つ分の角度
	constexpr float kLatEvery=static_cast<float>(M_PI) / static_cast<float>(kSubDivision);
	// 緯度分割 1 つ分の角度
	constexpr float kLonEvery=static_cast<float>(M_PI) * 2.0f / static_cast<float>(kSubDivision);

	Vec3 pos[3];

	for(uint32_t latIndex=0; latIndex < kSubDivision; ++latIndex) {
		// -π/2 ~ π/2
		float lat=-static_cast<float>(M_PI) / 2.0f + (kLatEvery * latIndex);
		for(uint32_t lonIndex=0; lonIndex < kSubDivision; ++lonIndex) {
			float lon=lonIndex * kLonEvery;
			Vec3 a, b, c;
			a=Vec3({ std::cosf(lat) * std::cosf(lon),std::sinf(lat),std::cosf(lat) * std::sinf(lon) }) * sphere.radius;
			b=Vec3({ std::cosf(lat + kLatEvery) * std::cosf(lon),std::sinf(lat + kLatEvery),std::cosf(lat + kLatEvery) * std::sinf(lon) }) * sphere.radius;
			c=Vec3({ std::cosf(lat) * std::cosf(lon + kLonEvery),std::sinf(lat),std::cosf(lat) * std::sinf(lon + kLonEvery) }) * sphere.radius;

			// ndc
			pos[0]=Transform(a, sphere.worldMa * viewProjectionMa);
			pos[1]=Transform(b, sphere.worldMa * viewProjectionMa);
			pos[2]=Transform(c, sphere.worldMa * viewProjectionMa);
			// screen
			pos[0]=Transform(pos[0], viewPortMa);
			pos[1]=Transform(pos[1], viewPortMa);
			pos[2]=Transform(pos[2], viewPortMa);

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