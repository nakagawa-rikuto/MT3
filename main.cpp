#include <Novice.h>
#include "Matrix4x4.h"
#include <cmath>
#include "Vector3.h"

const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// 1.X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {

	/*外側の中かっこは、Matrix4x4構造体の初期化を表しており、
	  内側の中かっこは配列の初期化を表しています。*/
	Matrix4x4 result = { {
	   {1, 0, 0, 0},
	   {0, cos(radian), sin(radian), 0},
	   {0, -sin(radian), cos(radian), 0},
	   {0, 0, 0, 1}
   } };

	return result;
};

// 2.Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result = { {
	   {cos(radian), 0, -sin(radian), 0},
	   {0, 1, 0, 0},
	   {sin(radian), 0, cos(radian), 0},
	   {0, 0, 0, 1}
   } };

	return result;
};

// 3.Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result = { {
	   {cos(radian), sin(radian), 0, 0},
	   {-sin(radian), cos(radian), 0, 0},
	   {0, 0, 1, 0},
	   {0, 0, 0, 1}
   } };

	return result;
};

// 行列の積
Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 answer = {};
	for (int x = 0; x < 4; ++x) {
		for (int y = 0; y < 4; ++y) {

			answer.m[x][y] = 0;
			for (int z = 0; z < 4; ++z) {

				answer.m[x][y] += m1.m[x][z] * m2.m[z][y];
			}
		}
	}

	return answer;
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
	Novice::Initialize(kWindowTitle, 300, 520);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	static const int kRowHeight = 20;

	Vector3 rotate{ 0.4f, 1.43f, -0.8f };
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Mutiply(rotateXMatrix, Mutiply(rotateYMatrix, rotateZMatrix));

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

		MatrixScreenPrintf(0, 0, rotateXMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5, rotateYMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, rotateZMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, rotateXYZMatrix);

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
