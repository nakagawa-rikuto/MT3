#include "MyMath.h"
#include "Novice.h"
#include "imgui.h"

// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

	Matrix4x4 translateMatrix = { {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{translate.x, translate.y, translate.z, 1}
	} };

	return translateMatrix;
}

// 拡縮行列
Matrix4x4 MakeScalseMatrix(const Vector3& scale) {

	Matrix4x4 scaleMatrix = { {
		{scale.x, 0, 0, 0},
		{0, scale.y, 0, 0},
		{0, 0, scale.z, 0},
		{0, 0, 0, 1}
	} };

	return scaleMatrix;
}

// X軸回転行列
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
}

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {

	Matrix4x4 result = { {
	   {cos(radian), 0, -sin(radian), 0},
	   {0, 1, 0, 0},
	   {sin(radian), 0, cos(radian), 0},
	   {0, 0, 0, 1}
   } };

	return result;
}

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {

	Matrix4x4 result = { {
	   {cos(radian), sin(radian), 0, 0},
	   {-sin(radian), cos(radian), 0, 0},
	   {0, 0, 1, 0},
	   {0, 0, 0, 1}
   } };

	return result;
}

// 行列同士の掛け算
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

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(
	const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

	// 平行移動(T)
	Matrix4x4 translateMatrix_ = MakeTranslateMatrix(translate);

	// 拡縮(S)
	Matrix4x4 scaleMatrix_ = MakeScalseMatrix(scale);

	// 回転(R)
	Matrix4x4 roteMatrixX_ = MakeRotateXMatrix(rotate.x);
	Matrix4x4 roteMatrixY_ = MakeRotateYMatrix(rotate.y);
	Matrix4x4 roteMatrixZ_ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 roteMatrixXYZ_ =
		Mutiply(roteMatrixX_, Mutiply(roteMatrixY_, roteMatrixZ_));

	// 行列の積の結合法則(W = SRT)
	Matrix4x4 affineMatrix_ = Mutiply(scaleMatrix_, roteMatrixXYZ_);
	affineMatrix_ = Mutiply(affineMatrix_, translateMatrix_);

	return affineMatrix_;
}

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {

	float x = v1.y * v2.z - v1.z * v2.y;
	float y = v1.z * v2.x - v1.x * v2.z;
	float z = v1.x * v2.y - v1.y * v2.x;

	return Vector3(x, y, z);
}

// 単位行列の作成
Matrix4x4 MakeIdenitiy4x4() {

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

// 転置行列
Matrix4x4 TransposeMatrix(const Matrix4x4& m) {

	Matrix4x4 answer;

	for (int row = 0; row < 4; row++) {
		for (int col = 0; col < 4; col++) {

			answer.m[row][col] = m.m[row][col];
		}
	}

	return answer;
}

// ビューポート変換行列
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
}

// 透視影行列
Matrix4x4 MakePerspectiveFovMatrix(
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
}

// 正射影行列
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
}

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {

	Matrix4x4 invMatrix;

	float det =
		m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) +
		m.m[0][1] * (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) +
		m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

	if (det == 0) {
		return invMatrix; // ゼロ除算のエラー処理
	}

	float invDet = 1.0f / det;

	invMatrix.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * invDet;
	invMatrix.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * invDet;
	invMatrix.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * invDet;
	invMatrix.m[0][3] = 0.0f;

	invMatrix.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * invDet;
	invMatrix.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * invDet;
	invMatrix.m[1][2] = (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]) * invDet;
	invMatrix.m[1][3] = 0.0f;

	invMatrix.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * invDet;
	invMatrix.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * invDet;
	invMatrix.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]) * invDet;
	invMatrix.m[2][3] = 0.0f;

	invMatrix.m[3][0] = -(m.m[3][0] * invMatrix.m[0][0] + m.m[3][1] * invMatrix.m[1][0] + m.m[3][2] * invMatrix.m[2][0]);
	invMatrix.m[3][1] = -(m.m[3][0] * invMatrix.m[0][1] + m.m[3][1] * invMatrix.m[1][1] + m.m[3][2] * invMatrix.m[2][1]);
	invMatrix.m[3][2] = -(m.m[3][0] * invMatrix.m[0][2] + m.m[3][1] * invMatrix.m[1][2] + m.m[3][2] * invMatrix.m[2][2]);
	invMatrix.m[3][3] = 1.0f;

	return invMatrix;
}

// 座標変換
Vector3 Transform(Vector3 vector, Matrix4x4 matrix) {

	float x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	float y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	float z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[3][3];
	// Ensure w is not zero to avoid division by zero
	if (w != 0.0f) {
		x /= w;
		y /= w;
		z /= w;
	}
	return Vector3(x, y, z);
}

// グリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;                                      // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                       // 分散数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ

	// 奥から手前への線を順序に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		// ワールド座標系上の始点を計算する
		Vector3 start(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth);

		// ワールド座標系上の終点を計算する
		Vector3 end(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth);

		// ワールド座標系からスクリーン座標系への変換
		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		Vector3 endScreen = Transform(end, viewProjectionMatrix);

		// スクリーン座標系からビューポート座標系への変換
		startScreen = Transform(startScreen, viewportMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)
		Novice::DrawLine(
			static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
			static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
	}

	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 奥から手前が左右に変わるだけ

		 // ワールド座標系上の始点を計算する
		Vector3 start(-kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);
		// ワールド座標系上の終点を計算する
		Vector3 end(kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);

		// ワールド座標系からスクリーン座標系への変換
		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		Vector3 endScreen = Transform(end, viewProjectionMatrix);

		// スクリーン座標系からビューポート座標系への変換
		startScreen = Transform(startScreen, viewportMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		//描画
		Novice::DrawLine(
			static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
			static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
	}
}

// スフィアの描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	const uint32_t kSubdivision = 16; // 分散数
	const float kLatEvery = static_cast<float>(M_PI) / kSubdivision; // 経度分割1つの角度
	const float kLonEvery = static_cast<float>(2 * M_PI) / kSubdivision;// 緯度分割1つの角度

	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度

			// world座標系でのa, b, cを求める
			Vector3 a, b, c;
			a = { sphere.radius * std::cos(lon) * std::cos(lat) + sphere.center.x,
				  sphere.radius * std::sin(lon) + sphere.center.y,
				  sphere.radius * std::cos(lon) * std::sin(lat) + sphere.center.z };

			b = { sphere.radius * std::cos(lon + kLonEvery) * std::cos(lat) + sphere.center.x,
				  sphere.radius * std::sin(lon + kLonEvery) + sphere.center.y,
				  sphere.radius * std::cos(lon + kLonEvery) * std::sin(lat) + sphere.center.z };

			c = { sphere.radius * std::cos(lon) * std::cos(lat + kLatEvery) + sphere.center.x,
				  sphere.radius * std::sin(lon) + sphere.center.y,
				  sphere.radius * std::cos(lon) * std::sin(lat + kLatEvery) + sphere.center.z };

			// a,b,cをScreen座標系まで変換


			Vector3 screenA = Transform(a, viewProjectionMatrix);
			Vector3 screenB = Transform(b, viewProjectionMatrix);
			Vector3 screenC = Transform(c, viewProjectionMatrix);

			screenA = Transform(screenA, viewportMatrix);
			screenB = Transform(screenB, viewportMatrix);
			screenC = Transform(screenC, viewportMatrix);

			// ab, bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2) {

	// ベクトルv2の単位ベクトルを計算する
	float lengthSquared =
		sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z) * sqrt(v2.x * v2.x + v2.y * v2.y + v2.z * v2.z);

	Vector3 unitV2 =
	{ v2.x * (1.0f / lengthSquared), v2.y * (1.0f / lengthSquared), v2.z * (1.0f / lengthSquared) };

	// v1をv2に対して正射影する
	float projectLength = 
	{ v1.x * unitV2.x + v1.y * unitV2.y + v1.z * unitV2.z };

	return Vector3(unitV2.x * projectLength, unitV2.y * projectLength, unitV2.z * projectLength);
}

// 最近接点
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {

	// 線分の方向ベクトル
	Vector3 v = segment.diff;

	// 始点からの点へのベクトル
	Vector3 w = point - segment.origin;

	// 線分の始点よりも前にある場合、始点が最近接点
	float c1 = w.x * v.x + w.y * v.y + w.z * v.z;
	if (c1 <= 0) {
		return segment.origin;
	}
		
	float c2 = v.x * v.x + v.y * v.y + v.z * v.z;
	// 線分の終点よりも後ろにある場合、終点が最近接点
	if (c2 <= c1) {
		return segment.origin + v;
	}
		
	// 線分上にある場合、垂線の足を求める
	float b = c1 / c2;
	Vector3 closest = segment.origin + Vector3(v.x * b, v.y * b, v.z * b);
	return closest;
}

// 長さの計算
float Length(Vector3 v) {

	float length = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);

	return length;
}
float Length(Vector3 v1, Vector3 v2) {

	float length = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return length;
}

bool IsCollision(const Sphere& s1, const Sphere& s2) {
	
	// 中心間の距離の二乗を計算
	Vector3 distance;
	distance.x = s1.center.x - s2.center.x;
	distance.y = s1.center.y - s2.center.y;
	distance.z = s1.center.z - s2.center.z;
	float distanceSquared = Length(distance);

	// 半径の合計を計算
	float radiusSum = s1.radius + s2.radius;

	// 中心間の距離の二乗が半径の合計の二乗以下なら衝突
	if (distanceSquared <= radiusSum) {

		return true;
	} else {

		return false;
	}
}
