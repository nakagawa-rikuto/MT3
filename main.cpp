#include <Novice.h>
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>

const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// 1.透視影行列
Matrix4x4 MakePerspectiveFowMatrix(
	float fovY, float aspectRatio, float nearClip, float farClip) {

	float tanHalfFovY = tan(fovY * 0.5f);
	float scaleX = 1.0f / (aspectRatio * tanHalfFovY);
	float scaleY = 1.0f / tanHalfFovY;
	float nearMinusFar = farClip - nearClip;

	Matrix4x4 result = { {
		{scaleX, 0, 0, 0},
		{0, scaleY, 0, 0},
		{0, 0, farClip / nearMinusFar, 1},
		{0, 0, (-farClip * nearClip) / nearMinusFar, 0}
	} };

	return result;
};

// 2.正射影行列
Matrix4x4 MakeOrethographicMatrx(
	float left, float top, float right, float bottom, float nearClip, float farClip) {

		float scaleX = 2.0f / (right - left);
		float scaleY = 2.0f / (top - bottom);
		float scaleZ = 1.0f / (farClip - nearClip);
		float offsetX = (right + left) / (left - right);
		float offsetY = (top + bottom) / (bottom - top);
		float offsetZ = (nearClip) / (nearClip - farClip);

		Matrix4x4 result = { {
			{scaleX, 0, 0, 0},
			{0, scaleY, 0, 0},
			{0, 0, scaleZ, 0},
			{ offsetX, offsetY, offsetZ, 1}
		} };

		return result;
};

// 3.ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth) {

	float scaleX = width / 2.0f;
	float scaleY = height / 2.0f;
	float scaleZ = (maxDepth - minDepth);
	float offsetX = left + scaleX;
	float offsetY = top + scaleY;
	float offsetZ = minDepth;

	Matrix4x4 result = { {
		{scaleX, 0, 0, 0},
		{0, -scaleY, 0, 0},  // - scaleYは、ビューポートのY軸が反転しているため。
		{0, 0, scaleZ, 0},
		{offsetX, offsetY, offsetZ, 1}
	} };

	return result;
};

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
	Novice::Initialize(kWindowTitle, 300, 350);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	static const int kRowHeight = 20;

	Matrix4x4 orthographicMatrix =
		MakeOrethographicMatrx(-160.0f, 160.0f, 200.0f, 300.0f, 0.0f, 1000.0f);

	Matrix4x4 perspectibeFovMatrix =
		MakePerspectiveFowMatrix(0.63f, 1.33f, 0.1f, 1000.0f);

	Matrix4x4 viewportMatrix = 
		MakeViewportMatrix(100.0f, 200.0f, 600.0f, 300.0f, 0.0f, 1.0f);

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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(0, 0, orthographicMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5, perspectibeFovMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, viewportMatrix);

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
