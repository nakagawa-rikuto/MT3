#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include <cmath>
#include <assert.h>

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

// �N���X��
Vector3 Cross(const Vector3& v1, const Vector3& v2);

// �P�ʍs��̍쐬
Matrix4x4 MakeIdenitiy4x4();

// �]�u�s��
Matrix4x4 TransposeMatrix(const Matrix4x4& m);

// �r���[�|�[�g�ϊ��s��
Matrix4x4 MakeViewportMatrix(
	float left, float top, float width, float height, float minDepth, float maxDepth);

// �����e�s��
Matrix4x4 MakePerspectiveFovMatrix(
	float fovY, float aspectRatio, float nearClip, float farClip);

// ���ˉe�s��
Matrix4x4 MakeOrethographicMatrx(
	float left, float top, float right, float bottom, float nearClip, float farClip);

// �t�s��
Matrix4x4 Inverse(const Matrix4x4& m);

// ���W�ϊ�
Matrix4x4 

