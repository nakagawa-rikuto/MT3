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
		Matrix4x4 worldMatrixSphereS = MakeAffineMatrix(shoulder.scales, shoulder.rotates, shoulder.translates);
		Matrix4x4 worldMatrixSphereE = Multiply(MakeAffineMatrix(elbow.scales, elbow.rotates, elbow.translates), worldMatrixSphereS);
		Matrix4x4 worldMatrixSphereH = Multiply(MakeAffineMatrix(hand.scales, hand.rotates, hand.translates), worldMatrixSphereE);
		Matrix4x4 worldMatrixLineS = MakeAffineMatrix({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 0.0f }, Vector3(worldMatrixSphereS.m[3][0], worldMatrixSphereS.m[3][1], worldMatrixSphereS.m[3][2]));
		Matrix4x4 worldMatrixLineE = MakeAffineMatrix({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 0.0f }, Vector3(worldMatrixSphereE.m[3][0], worldMatrixSphereE.m[3][1], worldMatrixSphereE.m[3][2]));
		Matrix4x4 worldMatrixLineH = MakeAffineMatrix({ 0.1f, 0.1f, 0.1f }, { 0.0f, 0.0f, 0.0f }, Vector3(worldMatrixSphereH.m[3][0], worldMatrixSphereH.m[3][1], worldMatrixSphereH.m[3][2]));

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);

		Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixSphereS = Multiply(worldMatrixSphereS, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixSphereE = Multiply(worldMatrixSphereE, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixSphereH = Multiply(worldMatrixSphereH, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixLineS = Multiply(worldMatrixLineS, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixLineE = Multiply(worldMatrixLineE, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixLineH = Multiply(worldMatrixLineH, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

		sphere1.center = Transform(shoulder.translates, wvpMatrixSphereS);
		//sphere1.center = Transform(sphere1.center, viewportMatrix);
		sphere2.center = Transform(elbow.translates, wvpMatrixSphereE);
		//sphere2.center = Transform(sphere2.center, viewportMatrix);
		sphere3.center = Transform(hand.translates, wvpMatrixSphereH);
		//sphere3.center = Transform(sphere3.center, viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(wvpMatrix, viewportMatrix);
		DrawSphere(sphere1, wvpMatrixSphereS, viewportMatrix, RED);
		DrawSphere(sphere2, wvpMatrixSphereE, viewportMatrix, GREEN);
		DrawSphere(sphere3, wvpMatrixSphereH, viewportMatrix, BLUE);

		DrawLine(sphere1.center, sphere2.center, wvpMatrixLineS, viewportMatrix, wvpMatrixLineE, viewportMatrix, WHITE);
		DrawLine(sphere2.center, sphere3.center, wvpMatrixLineE, viewportMatrix, wvpMatrixLineH, viewportMatrix, WHITE);
		
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
