#pragma once
#include "MyMath.h"

// 球と球のの当たり判定
bool IsCollision(const Sphere& s1, const Sphere& s2);

// 球と平面の当たり判定
bool IsCollision(const Sphere& sphere, const Plane& plane);