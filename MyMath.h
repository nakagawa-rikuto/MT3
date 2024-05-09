#pragma once
#include "Matrix4x4.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <assert.h>

// ฝsฺฎ
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

// gks๑
Matrix4x4 MakeScalseMatrix(const Vector3& scale);

// Xฒ๑]s๑
Matrix4x4 MakeRotateXMatrix(float radian);

// Yฒ๑]s๑
Matrix4x4 MakeRotateYMatrix(float radian);

// Zฒ๑]s๑
Matrix4x4 MakeRotateZMatrix(float radian);

// s๑ฏmฬ|ฏZ
Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2);

// 3ณAtBฯทs๑
Matrix4x4 MakeAffineMatrix(
	const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// NXฯ
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// Pสs๑ฬ์ฌ
Matrix4x4 MakeIdenitiy4x4();

// ]us๑
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

// r[|[gฯทs๑
Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth);

// งes๑
Matrix4x4 MakePerspectiveFovMatrix(
	float fovY, float aspectRatio, float nearClip, float farClip);

// ณหes๑
Matrix4x4 MakeOrethographicMatrx(
	float left, float top, float right, float bottom, float nearClip, float farClip);

// ts๑
Matrix4x4 Inverse(const Matrix4x4& m);

// ภWฯท
Vector3 Transform(Vector3 vector, Matrix4x4 matrix);

// Grid๐\ฆท้^R[h
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);