#pragma once
#include "MyMath.h"

// Gridの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix);

// スフィアの描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 三角形の描画
void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// AABBの描画
void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// OBBの描画
void DrawOBB(const OBB& obb, const Matrix4x4& viewProjectionMatrix, Matrix4x4& viewportMatrix, uint32_t color);

// ベジュ曲線の描画
void DrawBezier(const Vector3& controlPoint0, const Vector3& controlPoint1, const Vector3& controlPoint, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color);

// 指定された場所と場所をつなぐ線の描画
void DrawLine(const Vector3& pos1, const Vector3& pos2, 
	const Matrix4x4& viewProjectionMatrix1, const Matrix4x4& viewportMatrix1, 
	const Matrix4x4& viewProjectionMatrix2, const Matrix4x4& viewportMatrix2, 
	uint32_t color);