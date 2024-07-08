#include "MyExecution.h"

void SpringMove(Spring spring, Ball ball, float deltaTime) {

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
		Vector3 force = restoringForce;
		ball.acceleration = force / ball.mass;
	}

	// 加速度も速度もどちらとも秒を基準とした値である
	// それが、1/60秒間(deltaTime)運用されたと考える
	ball.velocity += ball.acceleration * deltaTime;
	ball.position += ball.velocity * deltaTime;
}
