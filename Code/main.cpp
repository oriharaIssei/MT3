#include <Novice.h>

#include <MyMatrix4x4.h>

const char kWindowTitle[]="LC1B_08_オリハライッセイ";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	MyMatrix4x4 m1={
		3.2f,0.7f,9.6f,4.4f,
		5.5f,1.3f,7.8f,2.1f,
		6.9f,8.0f,2.6f,1.0f,
		0.5f,7.2f,5.1f,3.3f
	};
	MyMatrix4x4 m2={
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f
	};

	MyMatrix4x4 add=m1 + m2;
	MyMatrix4x4 multiply=m1 * m2;
	MyMatrix4x4 sub=m1 - m2;
	MyMatrix4x4 inverse[2]={ m1.Inverse(),m2.Inverse() };
	MyMatrix4x4 transpose[2]={ m1.Transpose(),m2.Transpose() };
	MyMatrix4x4 identity=MakeIdentity4x4();

	constexpr int kRowHeight=20;
	constexpr int kColWidht=60;


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

		ScreenPrintMatrix("Add",0, 0, add);
		ScreenPrintMatrix("Sub", 0, kRowHeight * 5, sub);
		ScreenPrintMatrix("Multiply", 0, kRowHeight * 10, multiply);
		ScreenPrintMatrix("inverse [0]", 0, kRowHeight * 15, inverse[0]);
		ScreenPrintMatrix("inverse [1]", 0, kRowHeight * 20, inverse[1]);
		ScreenPrintMatrix("transpose[0]", kColWidht * 5, 0, transpose[0]);
		ScreenPrintMatrix("transpose[1]", kColWidht * 5, kRowHeight * 5, transpose[1]);
		ScreenPrintMatrix("identity", kColWidht * 5, kRowHeight * 10, identity);

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
