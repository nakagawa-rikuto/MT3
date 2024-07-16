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
#include <algorithm>

/// <summary>
/// 3x3行列
/// </summary>
struct Matrix3x3 final {
	float m[3][3];
};

// WorldTransform
struct WorldTransform {
	Vector3 translates;
	Vector3 rotates;
	Vector3 scales;
};

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

// OBB
struct OBB {
	Vector3 center;          // !<中心
	Vector3 orientations[3]; // !<座標軸。正規化・直交必須
	Vector3 size;            // !<座標軸方向の長さの半分。中心空面までの距離
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

// ばね
struct Spring {
	Vector3 anchor; // アンカー
	float naturalLength; // 自然長
	float stiffness; // 剛性
	float dampingCoefficient; // 減衰係数
};

// ボール
struct Ball {
	Vector3 position; // 位置
	Vector3 velocity; // 速度
	Vector3 acceleration; // 加速度
	float mass; // 質量
	float radius; // 半径
	unsigned int color; // 色
};

// 角速度
struct Angular {
	float velocity;
	float angle;
	float radius;
};

// 振り子
struct Pendulum {
	Vector3 anchor;  // アンカーポイント
	float length;    // 紐の長さ
	float angle;     // 現在の角度
	float angleVelocity;        // 角速度w 
	float angularAcceleration;  // 角加速度
};

// 円錐振り子
struct ConicalPendulum {
	Vector3 anchor;      // アンカーポイント
	float length;        // 紐の長さ
	float halfApexAngle; // 円錐の頂角の半分
	float angle;         // 現在の角度
	float angularVelocity; // 角度速w
};

// カプセル
struct Capsule {
	Segment segment;
	float radius;
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

// OBBのWorldMatrixの作成
Matrix4x4 CreateOBBWorldMatrix(const OBB& obb);

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

// 線形補間の関数
Vector3 Leap(const Vector3& v1, const Vector3& v2, float t);

// 3次元のベジュの線形補間の関数
Vector3 CubicBezier(const Vector3& P0, const Vector3& P1, const Vector3& P2, float t);

// ばねの実装
void SpringMove(Spring& spring, Ball& ball, Vector3 gravity, float t);

// 角速度の実装(円運動)
void AngularMove(Angular& angular, Vector3& position, Vector3& center, float deltaTime);

// 振り子の実装
void PendulumMove(Pendulum& pendulum, Vector3& position, float gravity, float deltaTime);

// 円錐振り子の実装
void ConicalPendulumMove(ConicalPendulum& pendulum, Vector3& position, float gravity, float deltaTime);

// 反射ベクトルを求める関数
Vector3 Reflect(const Vector3& input, const Vector3& normal);

// 指定された範囲内に制限する関数(クランプ)
float Clamp(float Value, float Max, float Min);