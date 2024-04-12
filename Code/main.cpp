#include <Novice.h>

#include "Vec3.h"
#include <MyMatrix4x4.h>

const char kWindowTitle[]="LC1B_08_オリハライッセイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//==============初期化====================
	Vec3 rotate{ 0.4f,1.43f,-0.8f };
	MyMatrix4x4 rotateXMa=MakeMatrix::RotateX(rotate.x);
	MyMatrix4x4 rotateYMa=MakeMatrix::RotateY(rotate.y);
	MyMatrix4x4 rotateZMa=MakeMatrix::RotateZ(rotate.z);
	MyMatrix4x4 ratateXYZMa=MakeMatrix::RotateXYZ(rotateXMa, rotateYMa, rotateZMa);

	constexpr int kRowHeight=20;

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


		ScreenPrintMatrix("RotateX",0, 0, rotateXMa);
		ScreenPrintMatrix("RotateY", 0, kRowHeight * 5, rotateYMa);
		ScreenPrintMatrix("RotateZ", 0, kRowHeight * 10, rotateZMa);
		ScreenPrintMatrix("RotateXYZ", 0, kRowHeight * 15, ratateXYZMa);

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
