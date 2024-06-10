#pragma once
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>
#include <array>
#include <cstdint>
#include <algorithm>
#include <vector>

// スフィアの情報
struct Sphere {
	Vector3 center;  // 中心
	float radius;    // 半径
};

// AABB
struct AABB {
	Vector3 min; //!<最小点
	Vector3 max; //!<最大点
};

// 三角形の情報
struct Triangle {
	Vector3 vertices[3];
};

// 直線
struct Line {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 半直線
struct Ray {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 線分
struct Segment {
	Vector3 origin; // !< 始点
	Vector3 diff;   // !< 終点への差分ベクトル
};

// 平面
struct Plane {
	Vector3 normal; // !< 法線
	float distance; // !< 距離
};

#pragma region /// 座標変換 ///
// 平行移動
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// 拡縮行列
Matrix4x4 MakeScalseMatrix(const Vector3& scale);

// X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian);

// 行列同士の掛け算
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(
	const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// クロス積
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// 単位行列の作成
Matrix4x4 MakeIdenitiy4x4();

// 転置行列
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

// ビューポート変換行列
Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth);

// 透視影行列
Matrix4x4 MakePerspectiveFovMatrix(
	float fovY, float aspectRatio, float nearClip, float farClip);

// 正射影行列
Matrix4x4 MakeOrethographicMatrx(
	float left, float top, float right, float bottom, float nearClip, float farClip);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m);

// 座標変換
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);

#pragma endregion

// 正射影ベクトル
Vector3 Project(const Vector3& v1, const Vector3& v2);

// 最近接点(線)
Vector3 ClosestPoint(const Vector3& point, const Segment& segment);

// 最近接点(AABB)
Vector3 ClosestPoint(const Vector3& point, const AABB& aabb);

#pragma region /// 計算 ///

// 内積の計算
float Dot(const Vector3& v1, const Vector3& v2);

// 長さの計算
float Length(const Vector3& v);
float Length(const Vector3& v1, const Vector3& v2);

// 正規化
Vector3 Normalize(const Vector3& v);

#pragma endregion

// 垂直なベクトルをl求める関数
Vector3 Perpendicular(const Vector3& vector);