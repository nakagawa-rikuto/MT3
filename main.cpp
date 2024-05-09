#include <Novice.h>
#include "MyMath.h"
#include "imgui.h"
const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

struct Sphere {
	Vector3 center;  // 中心
	float radius;    // 半径
};

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
		endScreen = Transform(startScreen, viewportMatrix);

		//描画
		Novice::DrawLine(
			static_cast<int>(startScreen.x), static_cast<int>(startScreen.y),
			static_cast<int>(endScreen.x), static_cast<int>(endScreen.y), 0xAAAAAAFF);
	}
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {

	const uint32_t kSubdivision = 16; // 分散数
	const float kLonEvery = M_PI / kSubdivision; // 経度分割1つの角度
	const float kLatEvery = (2 * M_PI) / kSubdivision;// 緯度分割1つの角度
	
	// 緯度の方向に分割 -π/2 ~ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -M_PI / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ~ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度

			// world座標系でのa, b, cを求める
			Vector3 a, b, c;
			a = { std::cos(lon) * std::cos(lat), 
				  std::sin(lon), 
				  std::cos(lon) * std::sin(lat) };

			b = { std::cos(lon + kLonEvery) * std::cos(lat), 
				  std::sin(lon + kLonEvery), 
				  std::cos(lon + kLonEvery) * std::sin(lat) };

			c = { std::cos(lon) * std::cos(lat + kLatEvery), 
				  std::sin(lon), 
				  std::cos(lon) * std::sin(lat + kLatEvery) };

			// a,b,cをScreen座標系まで変換
			

			Vector3 screenA = Transform(a, viewProjectionMatrix);
			Vector3 screenB = Transform(b, viewProjectionMatrix);
			Vector3 screenC = Transform(c, viewProjectionMatrix);

			screenA = Transform(a, viewportMatrix);
			screenB = Transform(b, viewportMatrix);
			screenC = Transform(c, viewportMatrix);

			// ab, bcで線を引く
			Novice::DrawLine(static_cast<int>(screenA.x), static_cast<int>(screenA.y), static_cast<int>(screenB.x), static_cast<int>(screenB.y), color);
			Novice::DrawLine(static_cast<int>(screenB.x), static_cast<int>(screenB.y), static_cast<int>(screenC.x), static_cast<int>(screenC.y), color);
		}
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Sphere sphere;
	sphere.center = { 5.0f, 5.0f, 0.0f };
	sphere.radius = 10.0f;

	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };

	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 orthoMatrix;
	Matrix4x4 viewportMatrix;
	Matrix4x4 wvpVpMatrix;


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

#ifdef  _DEBUG

		ImGui::Begin("Window");
		//ImGui::DragFloat3("CameraTranslate", );

#endif 
		


		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
