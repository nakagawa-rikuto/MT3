#include <Novice.h>
#include "Vector3.h"
#include "cmath"

const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// 加算

Vector3 Add(const Vector3& v1, const Vector3& v2) {

	Vector3 answer = { v1.x + v2.x, v1.y + v1.y, v1.z + v2.z };

	return answer;
}
// 減産

Vector3 Subtract(const Vector3& v1, const Vector3& v2) {

	Vector3 answer = { v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };

	return answer;
}
// スカラー倍

Vector3 Multiply(const float k, const Vector3& v1) {

	Vector3 answer = { k * v1.x, k * v1.y, k * v1.z };

	return answer;
}
// 内積

float Dot(const Vector3& v1, const Vector3& v2) {

	float answer = (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);

	return answer;
}
// 長さ(ノルム)

float Length(const Vector3& v) {

	float answer = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);

	return answer;
}
// 正規化

Vector3 Normalize(const Vector3& v) {

	float length = Length(v);
	if (length != 0) {

		return { v.x / length, v.y / length, v.z / length };
	} else {

		return { 0.0f,0.0f,0.0f };
	}
}

void VectorScreenPritf(int x, int y, const Vector3& vector, const char* label) {

	static const int kColumnWidth = 60;

	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
	Novice::ScreenPrintf(x + kColumnWidth * 3, y, "%s", label);
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 500, 500);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	Vector3 v1 = { 1.0f, 3.0f, -5.0f };
	Vector3 v2 = { 4.0f,-1.0f,2.0f };
	float k = 4.0f;

	Vector3 resultAdd = Add(v1, v2);
	Vector3 resultSubtract = Subtract(v1, v2);
	Vector3 resultMultiply = Multiply(k, v1);
	float resultDot = Dot(v1, v2);
	float resultLength = Length(v1);
	Vector3 resultNormalize = Normalize(v2);

	int kRowHeight = 20;

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

		VectorScreenPritf(0, 0, resultAdd, " : Add");
		VectorScreenPritf(0, kRowHeight, resultSubtract, " : Subtract");
		VectorScreenPritf(0, kRowHeight * 2, resultMultiply, " :Multiply");
		Novice::ScreenPrintf(0, kRowHeight * 3, "%.02f  : Dot", resultDot);
		Novice::ScreenPrintf(0, kRowHeight * 4, "%.02f  :Length", resultLength);
		VectorScreenPritf(0, kRowHeight * 5, resultNormalize, " :Normalize");

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
