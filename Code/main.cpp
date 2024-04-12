#include <Novice.h>

#include "Vec3.h"
#include <MyMatrix4x4.h>

const char kWindowTitle[]="LC1B_08_オリハライッセイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	
	//==============初期化====================
	const int kRowHeight=20;

	MyMatrix4x4 ortho=MakeMatrix::Orthographic(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);
	MyMatrix4x4 fov=MakeMatrix::PerspectiveFov(0.63f, 1.33f, 0.1f, 1000.0f);
	MyMatrix4x4 viewPort=MakeMatrix::ViewPort(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

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

		ScreenPrintMatrix("Orthographic", 0, 0, ortho);
		ScreenPrintMatrix("PerspectiveFov",0, kRowHeight * 5, fov);
		ScreenPrintMatrix("ViewPort", 0, kRowHeight * 10, viewPort);

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
