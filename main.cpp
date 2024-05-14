#include <Novice.h>
#include "MyMath.h"
#include "imgui.h"
const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};

	Segment segment = { {-2.0f, -1.0f, 0.0f}, {3.0f, 2.0f, 2.0f} };
	Vector3 point = { -1.5f, 0.6f, 0.6f };

	Vector3 project = Project({point - segment.origin}, segment.diff);
	Vector3 closesPoint = ClosestPoint(point, segment);

	Sphere pointSphere = { point, 0.01f }; // 1cmの球を描画
	Sphere closestPointSpere = { closesPoint, 0.01f };

	Sphere sphere1;
	sphere1.center = { -1.0f, 0.0f, 0.0f };
	sphere1.radius = 1.0f;
	unsigned int color = WHITE;

	Sphere sphere2;
	sphere2.center = { 1.0f, 0.0f, 0.0f };
	sphere2.radius = 1.0f;

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

#ifdef  _DEBUG

		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("SphereCenter", &sphere1.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere1.radius, 0.01f);
		//ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();

#endif 

		if (IsCollision(sphere1, sphere2)) {

			color = RED;
		} else {

			color = WHITE;
		}
		
		// 行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);
		Matrix4x4 wvpMatrix = Mutiply(worldMatrix, Mutiply(viewMatrix, projectionMatrix));
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 780.0f, 0.0f, 1.0f);

		//Vector3 start = Transform(Transform(segment.origin, wvpMatrix), viewportMatrix);
		//Vector3 end = Transform(Transform(segment.origin + segment.diff, wvpMatrix), viewportMatrix);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		//DrawSphere(pointSphere, wvpMatrix, viewportMatrix, RED);
		//DrawSphere(closestPointSpere, wvpMatrix, viewportMatrix, BLACK);
		//Novice::DrawLine(static_cast<int>(start.x), static_cast<int>(start.y), static_cast<int>(end.x), static_cast<int>(end.y), WHITE);

		DrawGrid(wvpMatrix, viewportMatrix);
		DrawSphere(sphere1, wvpMatrix, viewportMatrix, color);
		DrawSphere(sphere2, wvpMatrix, viewportMatrix, WHITE);

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
