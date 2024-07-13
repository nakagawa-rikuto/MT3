#include <Novice.h>
#include "MyMath.h"
#include "MyDrow.h"
#include "MyCollision.h"
#include "MyExecution.h"
#include "imgui.h"
const char kWindowTitle[] = "LC1B_17_ナカガワ_リクト_タイトル_";

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = {0};
	char preKeys[256] = {0};
	
	Spring spring{};
	spring.anchor = { 0.0f, 1.0f, 0.0f };
	spring.naturalLength = 0.7f;
	spring.stiffness = 100.0f;
	spring.dampingCoefficient = 2.0f;

	Ball ball{};
	ball.position = { 0.8f, 0.2f, 0.0f };
	ball.mass = 2.0f;
	ball.radius = 0.05f;
	ball.color = BLUE;

	const Vector3 kGravity{ 0.0f, -9.8f, 0.0f };

	float deltaTime = 1.0f / 60.0f;

	bool isStart = false;

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

		ImGui::Begin("Window");

		ImGui::DragFloat3("Ball.position", &ball.position.x, 0.01f);
		ImGui::Checkbox("isStart", &isStart);

		ImGui::End();

#endif 

		/* //////////////////////////////////
						実装
		*/ //////////////////////////////////

		if (isStart) {
			Vector3 diff = ball.position - spring.anchor;
			float length = Length(diff);
			if (length != 0.0f) {
				Vector3 direction = Normalize(diff);
				Vector3 restPosition = spring.anchor + direction * spring.naturalLength;
				Vector3 displacement = (ball.position - restPosition) * length;
				Vector3 restoringForce = displacement * -spring.stiffness;
				// 減衰抵抗を計算する
				Vector3 dampingForce = ball.velocity * -spring.dampingCoefficient;
				// 減衰抵抗も加味して、物体にかかる力を決定する。
				Vector3 force = restoringForce + dampingForce;
				ball.acceleration = force / ball.mass;
			}

			// 重力を追加する
			ball.acceleration += kGravity;

			// 加速度も速度もどちらとも秒を基準とした値である
			// それが、1/60秒間(deltaTime)運用されたと考える
			ball.velocity += ball.acceleration * deltaTime;
			ball.position += ball.velocity * deltaTime;
		}


		// 行列の計算
		Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f, 1.0f,1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		Matrix4x4 worldMatrixSphere = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, ball.position);

		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, 1280.0f / 780.0f, 0.1f, 100.0f);

		Matrix4x4 wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
		Matrix4x4 wvpMatrixSphere = Multiply(worldMatrixSphere, Multiply(viewMatrix, projectionMatrix));

		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, 1280.0f, 720.0f, 0.0f, 1.0f);

		Sphere sphere = {
			Transform(ball.position, wvpMatrixSphere),
			0.1f
		};

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		DrawGrid(wvpMatrix, viewportMatrix);
		DrawSphere(sphere, wvpMatrixSphere, viewportMatrix, static_cast<int>(ball.color));
		DrawLine(ball.position, spring.anchor, wvpMatrix, viewportMatrix, wvpMatrix, viewportMatrix, WHITE);

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
