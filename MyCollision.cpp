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

// 線と三角形の当たり判定
bool IsCollision(const Segment& segment, const Triangle& triangle) {
	
	// 浮動小数点の誤差を避けるための小さな値
	const float EPSILON = 1e-8f;

	// 三角形の頂点
	Vector3 vertex0 = triangle.vertices[0];
	Vector3 vertex1 = triangle.vertices[1];
	Vector3 vertex2 = triangle.vertices[2];

	// 三角形のエッジベクトル
	Vector3 edge1 = vertex1 - vertex0;
	Vector3 edge2 = vertex2 - vertex0;

	// 線分の差分ベクトルとエッジベクトルのクロス積
	Vector3 h = Cross(segment.diff, edge2);
	float a = Dot(edge1, h);

	// 線分が三角形と平行であるかをチェック
	if (a > -EPSILON && a < EPSILON) {
		return false; // 線分は三角形と平行
	}

	// aの逆数を計算
	float f = 1.0f / a;
	Vector3 s = segment.origin - vertex0;
	float u = f * Dot(s, h);

	// uが0から1の範囲にあるかをチェック（範囲外なら交差しない）
	if (u < 0.0f || u > 1.0f) {
		return false; // 三角形の外
	}

	// sとedge1のクロス積
	Vector3 q = Cross(s, edge1);
	float v = f * Dot(segment.diff, q);

	// vが0から1の範囲にあるか、およびu+vが1以下かをチェック（範囲外なら交差しない）
	if (v < 0.0f || u + v > 1.0f) {
		return false; // 三角形の外
	}

	// 線分の始点から交差点までの距離tを計算
	float t = f * Dot(edge2, q);

	// tがEPSILON以上かつ1+EPSILON以下であることをチェック（線分の範囲内かどうか）
	if (t > EPSILON && t < 1.0f + EPSILON) { 

		// 線分が三角形と交差する
		return true;
	} else {

		// 線分が三角形と交差しない
		return false;
	}
}
