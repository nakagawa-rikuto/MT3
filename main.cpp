#include <Novice.h>
#include "Matrix4x4.h"

const char kWindowTitle[] = "LE2B_18_ナカガワ_リクト_MT";

//1. 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 answer = {};
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			answer.m[row][col] = m1.m[row][col] + m2.m[row][col];
		}
	}

	return answer;
}

//2. 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 answer = {};
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			answer.m[row][col] = m1.m[row][col] - m2.m[row][col];
		}
	}

	return answer;
}

//2. 行列の積
Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

	Matrix4x4 answer = {};
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			answer.m[row][col] = m1.m[row][col] * m2.m[row][col];
		}
	}

	return answer;
}

//4. 逆行列

  //4.5 行列式を計算する関数
float CalculateDeterminant(const Matrix4x4& m) {
	float a = m.m[0][0];
	float b = m.m[0][1];
	float c = m.m[0][2];
	float d = m.m[0][3];
	float e = m.m[1][0];
	float f = m.m[1][1];
	float g = m.m[1][2];
	float h = m.m[1][3];
	float i = m.m[2][0];
	float j = m.m[2][1];
	float k = m.m[2][2];
	float l = m.m[2][3];
	float n = m.m[3][0];
	float o = m.m[3][1];
	float p = m.m[3][2];
	float q = m.m[3][3];

	return a * f * k * q - a * f * l * p - a * g * j * q + a * g * l * o + a * h * j * p - a * h * k * o
		- b * e * k * q + b * e * l * p + b * g * i * q - b * g * l * n - b * h * i * p + b * h * k * n
		+ c * e * j * q - c * e * l * o - c * f * i * q + c * f * l * n + c * h * i * o - c * h * j * n
		- d * e * j * p + d * e * k * o + d * f * i * p - d * f * k * n - d * g * i * o + d * g * j * n;
}


Matrix4x4 Inverse(const Matrix4x4& m) {

	Matrix4x4 answer;

	float det = CalculateDeterminant(m);
	float invDet = 1.0f / det;

	answer.m[0][0] = invDet * (m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[2][3] * m.m[3][2]) -
		m.m[1][2] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) +
		m.m[1][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));
	answer.m[0][1] = invDet * (m.m[0][2] * (m.m[2][3] * m.m[3][1] - m.m[2][1] * m.m[3][3]) -
		m.m[0][1] * (m.m[2][3] * m.m[3][2] - m.m[2][2] * m.m[3][3]) +
		m.m[0][3] * (m.m[2][1] * m.m[3][2] - m.m[2][2] * m.m[3][1]));
	answer.m[0][2] = invDet * (m.m[0][1] * (m.m[1][3] * m.m[3][2] - m.m[1][2] * m.m[3][3]) -
		m.m[0][2] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) +
		m.m[0][3] * (m.m[1][2] * m.m[3][1] - m.m[1][1] * m.m[3][2]));
	answer.m[0][3] = invDet * (m.m[0][1] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) -
		m.m[0][2] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) +
		m.m[0][3] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]));

	answer.m[1][0] = invDet * (m.m[1][2] * (m.m[2][3] * m.m[3][0] - m.m[2][0] * m.m[3][3]) -
		m.m[1][0] * (m.m[2][3] * m.m[3][2] - m.m[2][2] * m.m[3][3]) +
		m.m[1][3] * (m.m[2][0] * m.m[3][2] - m.m[2][2] * m.m[3][0]));
	answer.m[1][1] = invDet * (m.m[0][0] * (m.m[2][3] * m.m[3][2] - m.m[2][2] * m.m[3][3]) -
		m.m[0][2] * (m.m[2][3] * m.m[3][0] - m.m[2][0] * m.m[3][3]) +
		m.m[0][3] * (m.m[2][2] * m.m[3][0] - m.m[2][0] * m.m[3][2]));
	answer.m[1][2] = invDet * (m.m[0][2] * (m.m[1][3] * m.m[3][0] - m.m[1][0] * m.m[3][3]) -
		m.m[0][0] * (m.m[1][3] * m.m[3][2] - m.m[1][2] * m.m[3][3]) +
		m.m[0][3] * (m.m[1][2] * m.m[3][0] - m.m[1][0] * m.m[3][2]));
	answer.m[1][3] = invDet * (m.m[0][0] * (m.m[1][2] * m.m[2][3] - m.m[1][3] * m.m[2][2]) -
		m.m[0][2] * (m.m[1][0] * m.m[2][3] - m.m[1][3] * m.m[2][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]));

	answer.m[2][0] = invDet * (m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[2][3] * m.m[3][1]) -
		m.m[1][1] * (m.m[2][0] * m.m[3][3] - m.m[2][3] * m.m[3][0]) +
		m.m[1][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	answer.m[2][1] = invDet * (m.m[0][1] * (m.m[2][3] * m.m[3][0] - m.m[2][0] * m.m[3][3]) -
		m.m[0][0] * (m.m[2][3] * m.m[3][1] - m.m[2][1] * m.m[3][3]) +
		m.m[0][3] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	answer.m[2][2] = invDet * (m.m[0][0] * (m.m[1][1] * m.m[3][3] - m.m[1][3] * m.m[3][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[3][3] - m.m[1][3] * m.m[3][0]) +
		m.m[0][3] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));
	answer.m[2][3] = invDet * (m.m[0][1] * (m.m[1][3] * m.m[2][0] - m.m[1][0] * m.m[2][3]) -
		m.m[0][0] * (m.m[1][1] * m.m[2][3] - m.m[1][3] * m.m[2][1]) +
		m.m[0][3] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));

	answer.m[3][0] = invDet * (m.m[1][1] * (m.m[2][2] * m.m[3][0] - m.m[2][0] * m.m[3][2]) -
		m.m[1][0] * (m.m[2][2] * m.m[3][1] - m.m[2][1] * m.m[3][2]) +
		m.m[1][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	answer.m[3][1] = invDet * (m.m[0][0] * (m.m[2][2] * m.m[3][1] - m.m[2][1] * m.m[3][2]) -
		m.m[0][1] * (m.m[2][2] * m.m[3][0] - m.m[2][0] * m.m[3][2]) +
		m.m[0][2] * (m.m[2][0] * m.m[3][1] - m.m[2][1] * m.m[3][0]));
	answer.m[3][2] = invDet * (m.m[0][1] * (m.m[1][2] * m.m[3][0] - m.m[1][0] * m.m[3][2]) -
		m.m[0][0] * (m.m[1][2] * m.m[3][1] - m.m[1][1] * m.m[3][2]) +
		m.m[0][2] * (m.m[1][0] * m.m[3][1] - m.m[1][1] * m.m[3][0]));
	answer.m[3][3] = invDet * (m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) -
		m.m[0][1] * (m.m[1][0] * m.m[2][2] - m.m[1][2] * m.m[2][0]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]));

	return answer;
}

//5.転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {

	Matrix4x4 answer;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			answer.m[row][col] = m.m[row][col];
		}
	}

	return answer;
}

//6.単位行列の作成
Matrix4x4 MakeSIdentitiy4x4() {

	Matrix4x4 answer;
	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			if (row == col) {

				answer.m[row][col] = 1.0f;
			} else {

				answer.m[row][col] = 0.0f;
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
	Novice::Initialize(kWindowTitle, 600, 520);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	static const int kRowHeight = 20;
	static const int kColumnWidth = 60;

	Matrix4x4 m1 = {
		3.2f,0.7f,9.6f,4.4f,
		5.5f,1.3f,7.8f,2.1f,
		6.9f,8.0f,2.6f,1.0f,
		0.5f,7.2f,5.1f,3.3f
	};

	Matrix4x4 m2 = {
		4.1f,6.5f,3.3f,2.2f,
		8.8f,0.6f,9.9f,7.7f,
		1.1f,5.5f,6.6f,0.0f,
		3.3f,9.9f,8.8f,2.2f
	};

	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resultMultiply = Mutiply(m1, m2);
	Matrix4x4 resultSubtract = Subtract(m1, m2);
	Matrix4x4 inverseM1 = Inverse(m1);
	Matrix4x4 inverseM2 = Inverse(m2);
	Matrix4x4 transposeM1 = Transpose(m1);
	Matrix4x4 transposeM2 = Transpose(m2);
	Matrix4x4 identity = MakeSIdentitiy4x4();

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

		MatrixScreenPrintf(0, 0, resultAdd);
		MatrixScreenPrintf(0, kRowHeight * 5, resultSubtract);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, resultMultiply);
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, inverseM1);
		MatrixScreenPrintf(0, kRowHeight * 5 * 4, inverseM2);
		MatrixScreenPrintf(kColumnWidth * 5, 0, transposeM1);
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5, transposeM2);
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5 * 2, identity);

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
