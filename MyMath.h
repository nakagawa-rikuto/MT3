#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <assert.h>

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
Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2);

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
Matrix4x4 

