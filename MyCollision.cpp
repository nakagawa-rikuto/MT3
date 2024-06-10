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

// AABB同士の当たり判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2) {

	// x軸方向の判定
	if (aabb1.max.x < aabb2.min.x || aabb1.min.x > aabb2.max.x) {
		return false;
	}

	// y軸方向の判定
	if (aabb1.max.y < aabb2.min.y || aabb1.min.y > aabb2.max.y) {
		return false;
	}

	// z軸方向の判定
	if (aabb1.max.z < aabb2.min.z || aabb1.min.z > aabb2.max.z) {
		return false;
	}

	// 全ての軸で重なっている場合は衝突とみなす
	return true;
}

// AABBと球の当たり判定
bool IsCollision(const AABB& aabb, const Sphere& sphere) {
	
	// AABB における球の中心の最近接点を求める
	Vector3 closest = ClosestPoint(sphere.center, aabb);

	// 球の中心から最近接点までの距離の平方を計算する
	float distanceSquared = Length(closest - sphere.center);

	// 距離が球の半径の二乗よりも小さいかどうかをチェックする
	if (distanceSquared <= sphere.radius) {

		return true;
	} else {

		return false;
	}
}

// AABBと線の当たり判定
bool IsCollision(const AABB& aabb, const Segment& segment) {
	float tmin = 0.0f; // 線分の始点
	float tmax = 1.0f; // 線分の終点

	Vector3 p = segment.origin; // 線分の始点
	Vector3 d = segment.diff;   // 線分の方向ベクトル

	// 各軸 (x, y, z) についての判定
	for (int i = 0; i < 3; ++i) {
		float min = (&aabb.min.x)[i]; // AABBの最小点
		float max = (&aabb.max.x)[i]; // AABBの最大点
		float o = (&p.x)[i];          // 線分の始点の位置
		float dir = (&d.x)[i];        // 線分の方向ベクトルの成分

		// 方向ベクトルの成分がほぼ0の場合
		if (std::abs(dir) < 1e-8) {
			// 線分の始点がAABBの範囲外であれば衝突なし
			if (o < min || o > max) return false;
		} else {
			// 方向ベクトルの成分が非0の場合、交差区間を計算
			float invDir = 1.0f / dir;
			float t1 = (min - o) * invDir;
			float t2 = (max - o) * invDir;
			if (t1 > t2) std::swap(t1, t2); // t1とt2を入れ替える
			tmin = std::max(tmin, t1); // tminを更新
			tmax = std::min(tmax, t2); // tmaxを更新
			// tminがtmaxを超えた場合、交差はない
			if (tmin > tmax) return false;
		}
	}

	return true; // 衝突あり
}

// OBBと球の当たり判定(ローカル空間)
bool IsCollision(const OBB& obb, const Sphere& sphere) {

	Vector3 centerInOBBLocalSpace =
		Transform(sphere.center, Inverse(CreateOBBWorldMatrix(obb)));

	AABB aabbOBBLocal{
		.min = {-obb.size.x, -obb.size.y, -obb.size.z},
		.max = obb.size };

	Sphere sphereOBBLocal{ centerInOBBLocalSpace, sphere.radius };

	if (IsCollision(aabbOBBLocal, sphereOBBLocal)) {

		return true;
	} else {

		return false;
	}
}
