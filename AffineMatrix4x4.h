#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <assert.h>

class AffineMatrix4x4 {

public:

	// ���s�ړ�
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

	// �g�k�s��
	Matrix4x4 MakeScalseMatrix(const Vector3& scale);

	// X����]�s��
	Matrix4x4 MakeRotateXMatrix(float radian);

	// Y����]�s��
	Matrix4x4 MakeRotateYMatrix(float radian);

	// Z����]�s��
	Matrix4x4 MakeRotateZMatrix(float radian);

	// �s�񓯎m�̊|���Z
	Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2);

	// 3�����A�t�B���ϊ��s��
	Matrix4x4 MakeAffineMatrix(
		const Vector3& scale, const Vector3& rotate, const Vector3& translate);

private:

	/// *************************************
	/// �����o�ϐ�
	/// *************************************
	// �A�t�B��(W)
	Matrix4x4 affineMatrix_;

	// ���s�ړ�(S)
	Matrix4x4 translateMatrix_;

	// �g�k(S)
	Matrix4x4 scaleMatrix_;

	// ��](R)
	Matrix4x4 roteMatrixX_;
	Matrix4x4 roteMatrixY_;
	Matrix4x4 roteMatrixZ_;
	Matrix4x4 roteMatrixXYZ_;


};

