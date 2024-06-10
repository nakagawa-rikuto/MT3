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

// AABBと球の当たり判定
bool IsCollision(const AABB& aabb, const Sphere& sphere);

// AABBと線の当たり判定
bool IsCollision(const AABB& aabb, const Segment& segment);

// OBBと球の当たり判定(ローカル空間)
bool IsCollision(const OBB& obb, const Sphere& sphere);

// OBBと線の当たり判定
bool IsCollision(const Segment& segment, const OBB& obb);
