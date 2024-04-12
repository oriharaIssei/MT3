#include <Novice.h>

#include <MyMatrix4x4.h>

const char kWindowTitle[]="LC1B_08_オリハライッセイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);
	//==============初期化====================
	MyMatrix4x4 translateMa=MakeMatrix::Translate({ 4.1f,2.6f,0.8f });
	MyMatrix4x4 scaleMa=MakeMatrix::Scale({ 1.5f,5.2f,7.3f });

	Vec3 point={ 2.3f,3.8f,1.4f };
	MyMatrix4x4 transformMa={
		1.0f,2.0f,3.0f,4.0f,
		3.0f,1.0f,1.0f,2.0f,
		1.0f,4.0f,2.0f,3.0f,
		2.0f,2.0f,1.0f,3.0f
	};

	Vec3 transfomed=Transform(point, transformMa);

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


		ScreenPrintVector3(0, 0, transfomed, ": Transformed");
		ScreenPrintMatrix("Translate Matrix", 0, kRowHeight, translateMa);
		ScreenPrintMatrix("scale Matrix",0, kRowHeight * 6, scaleMa);

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
