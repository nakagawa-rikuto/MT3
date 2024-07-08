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

	WorldTransform shoulder = {
		{0.2f, 1.0f, 0.0f},
		{0.0f, 0.0f, -6.8f},
		{1.0f, 1.0f, 1.0f}
	};

	WorldTransform elbow = {
		{0.4f, 0.0f, 0.0f},
		{0.0f, 0.0f, -1.4f},
		{1.0f, 1.0f, 1.0f}
	};

	WorldTransform hand = {
		{0.3f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f}
	};

	Sphere sphere1 = {
		shoulder.translates,
		0.1f
	};

	Sphere sphere2 = {
		elbow.translates,
		0.1f
	};

	Sphere sphere3 = {
		hand.translates,
		0.1f
	};

	//unsigned int color = WHITE;

	Vector3 cameraTranslate = { 0.0f, 3.0f, -10.0f };
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
		ImGui::DragFloat3("shoulder.translates", &shoulder.translates.x, 0.01f);
		ImGui::DragFloat3("shoulder.rotates", &shoulder.rotates.x, 0.01f);
		ImGui::DragFloat3("shoulder.scales", &shoulder.scales.x, 0.01f);

		ImGui::DragFloat3("elbow.translates", &elbow.translates.x, 0.01f);
		ImGui::DragFloat3("elbow.rotates", &elbow.rotates.x, 0.01f);
		ImGui::DragFloat3("elbow.scales", &elbow.scales.x, 0.01f);

		ImGui::DragFloat3("hand.translates", &hand.translates.x, 0.01f);
		ImGui::DragFloat3("hand.rotates", &hand.rotates.x, 0.01f);
		ImGui::DragFloat3("hand.scales", &hand.scales.x, 0.01f);
		ImGui::End();

#endif 

		// 行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Matrix4x4 worldMatrixShoulder = MakeAffineMatrix(shoulder.scales, shoulder.rotates, shoulder.translates);
		Matrix4x4 worldMatrixElbow = Multiply(worldMatrixShoulder,MakeAffineMatrix(elbow.scales, elbow.rotates, elbow.translates));
		Matrix4x4 worldMatrixHand = Multiply(worldMatrixElbow, MakeAffineMatrix(hand.scales, hand.rotates, hand.translates)));

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);

		Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixShoulder = Multiply(worldMatrixShoulder, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixElbow = Multiply(worldMatrixElbow, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixHand = Multiply(worldMatrixHand, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 780.0f, 0.0f, 1.0f);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(wvpMatrix, viewportMatrix);
		DrawSphere(sphere1, wvpMatrixShoulder, viewportMatrix, RED);
		DrawSphere(sphere2, wvpMatrixElbow, viewportMatrix, GREEN);
		DrawSphere(sphere3, wvpMatrixHand, viewportMatrix, BLUE);

		DrawLine(sphere1.center, sphere2.center, wvpMatrixShoulder, viewportMatrix, wvpMatrixElbow, viewportMatrix, WHITE);
		DrawLine(sphere2.center, sphere3.center, wvpMatrixElbow, viewportMatrix, wvpMatrixHand, viewportMatrix, WHITE);
		
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
