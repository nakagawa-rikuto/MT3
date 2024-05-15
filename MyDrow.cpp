#include "MyDrow.h"
#include "Novice.h"
#include "imgui.h"

// グリッドの描画
void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;                                      // Gridの半分の幅
	const uint32_t kSubdivision = 10;                                       // 分散数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つ分の長さ

	// 奥から手前への線を順序に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {

		// ワールド座標系上の始点を計算する
		Vector3 start(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth);

		// ワールド座標系上の終点を計算する
		Vector3 end(-kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth);

		// ワールド座標系からスクリーン座標系への変換
		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		Vector3 endScreen = Transform(end, viewProjectionMatrix);

		// スクリーン座標系からビューポート座標系への変換
		startScreen = Transform(startScreen, viewportMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		// 変換した座標を使って表示。色は薄い灰色(0xAAAAAAFF)
		Novice::DrawLine(
			static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
			static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
	}

	// 左から右も同じように順々に引いていく
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {
		// 奥から手前が左右に変わるだけ

		 // ワールド座標系上の始点を計算する
		Vector3 start(-kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);
		// ワールド座標系上の終点を計算する
		Vector3 end(kGridHalfWidth, 0.0f, -kGridHalfWidth + zIndex * kGridEvery);

		// ワールド座標系からスクリーン座標系への変換
		Vector3 startScreen = Transform(start, viewProjectionMatrix);
		Vector3 endScreen = Transform(end, viewProjectionMatrix);

		// スクリーン座標系からビューポート座標系への変換
		startScreen = Transform(startScreen, viewportMatrix);
		endScreen = Transform(endScreen, viewportMatrix);

		//描画
		Novice::DrawLine(
			static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
			static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
	}
}

// スフィアの描画
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	const uint32_t kSubdivision = 16; // 分散数
	const float kLatEvery = static_cast<float>(M_PI) / kSubdivision; // 経度分割1つの角度
	const float kLonEvery = static_cast<float>(2 * M_PI) / kSubdivision;// 緯度分割1つの角度

	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -static_cast<float>(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度

			// world座標系でのa, b, cを求める
			Vector3 a, b, c;
			a = { sphere.radius * std::cos(lon) * std::cos(lat) + sphere.center.x,
				  sphere.radius * std::sin(lon) + sphere.center.y,
				  sphere.radius * std::cos(lon) * std::sin(lat) + sphere.center.z };

			b = { sphere.radius * std::cos(lon + kLonEvery) * std::cos(lat) + sphere.center.x,
				  sphere.radius * std::sin(lon + kLonEvery) + sphere.center.y,
				  sphere.radius * std::cos(lon + kLonEvery) * std::sin(lat) + sphere.center.z };

			c = { sphere.radius * std::cos(lon) * std::cos(lat + kLatEvery) + sphere.center.x,
				  sphere.radius * std::sin(lon) + sphere.center.y,
				  sphere.radius * std::cos(lon) * std::sin(lat + kLatEvery) + sphere.center.z };

			// a,b,cをScreen座標系まで変換


			Vector3 screenA = Transform(a, viewProjectionMatrix);
			Vector3 screenB = Transform(b, viewProjectionMatrix);
			Vector3 screenC = Transform(c, viewProjectionMatrix);

			screenA = Transform(screenA, viewportMatrix);
			screenB = Transform(screenB, viewportMatrix);
			screenC = Transform(screenC, viewportMatrix);

			// ab, bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

// 平面の描画
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	// 1.中心点を決める
	Vector3 center = plane.normal * plane.distance;

	// 2.法線と垂直なベクトルを1つ求める
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));

	// 3.2の逆ベクトルを求める
	perpendiculars[1] = { -perpendiculars[0].x, -perpendiculars[0].y, -perpendiculars[0].z };

	// 4.2と法線とのクロス積を求める
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);

	// 5.4の逆ベクトルを求める
	perpendiculars[3] = { -perpendiculars[2].x, -perpendiculars[2].y, -perpendiculars[2].z };

	// 6.2 ~ 5ベクトルを中心点にそれぞれ定数倍して足すと4頂点が出来上がる
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = perpendiculars[index] * 2.0f;
		Vector3 point = center + extend;
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}

	// 描画
	Novice::DrawLine(
		static_cast<int>(points[0].x), static_cast<int>(points[0].y), static_cast<int>(points[2].x), static_cast<int>(points[2].y), color);
	Novice::DrawLine(
		static_cast<int>(points[2].x), static_cast<int>(points[2].y), static_cast<int>(points[1].x), static_cast<int>(points[1].y), color);
	Novice::DrawLine(
		static_cast<int>(points[1].x), static_cast<int>(points[1].y), static_cast<int>(points[3].x), static_cast<int>(points[3].y), color);
	Novice::DrawLine(
		static_cast<int>(points[3].x), static_cast<int>(points[3].y), static_cast<int>(points[0].x), static_cast<int>(points[0].y), color);
}
