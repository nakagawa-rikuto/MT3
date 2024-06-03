#pragma once
#include "MyMath.h"

// 球と球のの当たり判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// 球と平面の当たり判定
bool IsCollision(const Sphere& sphere, const Plane& plane);

// 線と平面の当たり判定
bool IsCollision(const Segment& segment, const Plane& plane);

// 線と三角形の当たり判定
bool IsCollision(const Segment& segment, const Triangle& triangle);

// AABBとAABBの当たり判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);