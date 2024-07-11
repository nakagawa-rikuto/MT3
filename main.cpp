#include <Novice.h>
#include "MyMath.h"
#include "MyDrow.h"
#include "MyCollision.h"
#include "imgui.h"
const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Vector3 controlPoints[4] = {
		{-0.8f, 0.58f, 1.0f},
		{1.76f, 1.0f, -0.3f},
		{0.94f, -0.7f, 2.0f},
		{-0.53f, -0.28f, -0.15f}
	};

	unsigned int color = WHITE;

	Vector3 cameraTranslate = { 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate = { 0.26f, 0.0f, 0.0f };
	

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

		/* ///////////////////////////////
		            ImGui
		*/ ///////////////////////////////
#ifdef  _DEBUG

		ImGui::Begin("Camera");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::End();

		ImGui::Begin("Data");
		ImGui::DragFloat3("Bezier1", &controlPoints[0].x, 0.01f);
		ImGui::DragFloat3("Bezier2", &controlPoints[1].x, 0.01f);
		ImGui::DragFloat3("Bezier3", &controlPoints[2].x, 0.01f);
		ImGui::DragFloat3("Bezier4", &controlPoints[3].x, 0.01f);
		ImGui::End();

#endif 

		

		// ローカル空間で衝突判定
		/*if (IsCollision(obb, obb2)) {

			color = RED;
		} else {

			color = WHITE;
		}*/

		// 行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);
		Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 780.0f, 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(wvpMatrix, viewportMatrix);
		DrawCtarmullRow(controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3], wvpMatrix, viewportMatrix, color);
		
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
