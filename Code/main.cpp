#include <Novice.h>

#include "Vec3.h"
#include <MyMatrix4x4.h>

const char kWindowTitle[]="LC1B_08_オリハライッセイ";

struct Triangle {
	Vec3 vert[3];
	Vec3 scVert[3];
	MyMatrix4x4 worldMa;
};

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	const float kWindowWidth=1280.0f;
	const float kWindowHeight=720.0f;

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, static_cast<int>(kWindowWidth), static_cast<int>(kWindowHeight));
	
	//==============初期化====================
	Triangle triangle;
	triangle.vert[0]={ 0.0f, 1.5f, 0.0f };
	triangle.vert[1]={ 1.5f, 0.0f, 0.0f };
	triangle.vert[2]={ -1.5f, 0.0f, 0.0f };

	Vec3 rotate={ 0.0f,0.0f,0.0f };

	Vec3 translate={ 0.0f,0.0f,0.0f };

	triangle.worldMa=MakeMatrix::Affine({ 1.0f,1.0f,1.0f }, rotate, translate);

	MyMatrix4x4 cameraMa=MakeMatrix::Affine({ 1.0f,1.0f,1.0f }, { 0.0f,0.0f,0.0f }, { 0.0f,0.0f,-6.49f });
	MyMatrix4x4 viewMa=cameraMa.Inverse();

	MyMatrix4x4 projectionMa=MakeMatrix::PerspectiveFov(0.45f, kWindowWidth / kWindowHeight, 0.1f, 100.0f);

	MyMatrix4x4 wvpMa=triangle.worldMa * (viewMa * projectionMa);

	MyMatrix4x4 viewPortMa=MakeMatrix::ViewPort(0.0f, 0.0f, kWindowWidth, kWindowHeight, 0.0f, 1.0f);

	Vec3 cross=Vec3(1.2f, -3.9f, 2.5f).Cross({ 2.8f,0.4f,-1.3f });

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

		// rotate
		rotate.y+=0.03f;

		// move
		if(Novice::CheckHitKey(DIK_W)) {
			translate.z-=0.1f;
		}
		if(Novice::CheckHitKey(DIK_S)) {
			translate.z+=0.1f;
		}
		if(Novice::CheckHitKey(DIK_A)) {
			translate.x-=0.1f;
		}
		if(Novice::CheckHitKey(DIK_D)) {
			translate.x+=0.1f;
		}


		triangle.worldMa=MakeMatrix::Affine({ 1.0f,1.0f,1.0f }, rotate, translate);
		wvpMa=triangle.worldMa * viewMa * projectionMa;

		for(int i=0; i < 3; i++) {
			Vec3 ndcPos=Transform(triangle.vert[i], wvpMa);
			triangle.scVert[i]=Transform(ndcPos, viewPortMa);
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Novice::DrawTriangle(
			static_cast<int>(triangle.scVert[0].x),
			static_cast<int>(triangle.scVert[0].y),
			static_cast<int>(triangle.scVert[1].x),
			static_cast<int>(triangle.scVert[1].y),
			static_cast<int>(triangle.scVert[2].x),
			static_cast<int>(triangle.scVert[2].y),
			RED,
			kFillModeSolid
		);

		ScreenPrintVector3(
			0, 0,
			cross,
			"Cross"
		);

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
