#include "MyCollision.h"

// 球と球の当たり判定
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

// 球と平面の当たり判定
bool IsCollision(const Sphere& sphere, const Plane& plane) {

	// 平面の法線の正規化
	Vector3 normalizePlane = Normalize(plane.normal);

	// 球の中心と平面との距離を計算
	float distanceFromCenterToPlane = Dot(sphere.center, normalizePlane) - plane.distance;

	// 球と中心から平面までの距離の絶対値が球の半径以下ならば衝突
	if (std::abs(distanceFromCenterToPlane) <= sphere.radius) {

		return true;
	} else {

		return false;
	}
}

// 線と平面の当たり判定
bool IsCollision(const Segment& segment, const Plane& plane) {

	// 線分の終点を求める
	Vector3 endPoint = 
	{ segment.origin.x + segment.diff.x,segment.origin.y + segment.diff.y, segment.origin.z + segment.diff.z };

	// 始点と終点の平面からの距離を計算
	float distanceOrigin = Dot(plane.normal, segment.origin) + plane.distance;
	float distanceEndPoint = Dot(plane.normal, endPoint) + plane.distance;

	// 異なる側にあるかどうかを判別
	// 異なる側にある場合,一方が正、一方が負になる
	if (distanceOrigin * distanceEndPoint <= 0) {

		return true;
	} else {

		return false;
	}

}
