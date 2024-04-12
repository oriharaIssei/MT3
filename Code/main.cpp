#include <Novice.h>

#include "Vec3.h"

const char kWindowTitle[]="LE2A_08_オリハライッセイ_MT3_00_01";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256]={ 0 };
	char preKeys[256]={ 0 };

	Vec3 v1={ 1.0f,3.0f,-5.0f };
	Vec3 v2={ 4.0f,-1.0f,2.0f };
	float k={ 4.0f };

	Vec3 add=v1 + v2;
	Vec3 sub=v1 - v2;
	Vec3 multiply=v1 * k;
	float dot=v1.dot(v2);
	float len=v1.length();
	Vec3 normalize=v2.Normalize();

	const int kRowHeight=32;

	// ウィンドウの×ボタンが押されるまでループ
	while(Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		ScreenPrintVector3(0, 0, add, ": Add");
		ScreenPrintVector3(0, kRowHeight, sub, ": Sub");
		ScreenPrintVector3(0, kRowHeight * 2, multiply, ": Multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.02f : Dot", dot);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.02f : Lenght", len);
		ScreenPrintVector3(0, kRowHeight * 5, normalize, ": Normalize");

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
