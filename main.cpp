#include <Novice.h>
#include "MyMath.h"

const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// Vector3の数値表示
void VectorScreenPritf(int x, int y, const Vector3& vector, const char* label) {

	static const int kColumnWidth = 60;

	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

//4x4行列の数値表示
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {

	static const int kRowHeight = 20;
	static const int kColumnWidth = 60;

	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {

			Novice::ScreenPrintf(
				x + col * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][col]
			);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	// クロス積
	Vector3 Cross(const Vector3 & v1, const Vector3 & v2);

	// クロス積の確認用
	Vector3 v1 = { 1.2f, -3.9f, 2.5f };
	Vector3 v2 = { 2.8f, 0.4f, -1.3f };
	Vector3 cross = Cross(v1, v2);

	Vector3 rotate = {};
	Vector3 translate = {};
	Vector3 screenVertices[3];

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		/// **********************************************************
		/// キー入力
		/// **********************************************************
		if (keys[DIK_W] && !preKeys[DIK_W]) {

			translate.y += 4.0f;
		}

		if (keys[DIK_S] && !preKeys[DIK_S]) {

			translate.y -= 4.0f;
		}

		if (keys[DIK_A] && !preKeys[DIK_A]) {

			translate.x -= 4.0f;
		}

		if (keys[DIK_D] && !preKeys[DIK_D]) {

			translate.x += 4.0f;
		}

		/// 回転
		rotate.y += 0.05f;

		// 各種行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, rotate, translate);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f, }, { 0.0f,0.0f,0.0f }, { 100.0f,100.0f,0.0f });
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);
		Matrix4x4 worldViewProjectionMatix = Mutiply(worldMatrix, Mutiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 780.0f, 0.0f, 1.0f);
		
		for (uint32_t i = 0; i < 3; ++i) {
			Vector3 ndcVertex = Transform(kLocalVertices[1])
		}

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		// クロス積の確認用
		VectorScreenPritf(0, 0, cross, "Cross");

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
