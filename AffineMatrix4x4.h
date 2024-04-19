#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <assert.h>

class AffineMatrix4x4 {

public:

	// •½sˆÚ“®
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// Šgks—ñ
	Matrix4x4 MakeScalseMatrix(const Vector3& scale);

	// X²‰ñ“]s—ñ
	Matrix4x4 MakeRotateXMatrix(float radian);

	// Y²‰ñ“]s—ñ
	Matrix4x4 MakeRotateYMatrix(float radian);

	// Z²‰ñ“]s—ñ
	Matrix4x4 MakeRotateZMatrix(float radian);

	// s—ñ“¯m‚ÌŠ|‚¯Z
	Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 3ŸŒ³ƒAƒtƒBƒ“•ÏŠ·s—ñ
	Matrix4x4 MakeAffineMatrix(
		const Vector3& scale, const Vector3& rotate, const Vector3& translate);

private:

	/// *************************************
	/// ƒƒ“ƒo•Ï”
	/// *************************************
	// ƒAƒtƒBƒ“(W)
	Matrix4x4 affineMatrix_;

	// •½sˆÚ“®(S)
	Matrix4x4 translateMatrix_;

	// Šgk(S)
	Matrix4x4 scaleMatrix_;

	// ‰ñ“](R)
	Matrix4x4 roteMatrixX_;
	Matrix4x4 roteMatrixY_;
	Matrix4x4 roteMatrixZ_;
	Matrix4x4 roteMatrixXYZ_;


};

