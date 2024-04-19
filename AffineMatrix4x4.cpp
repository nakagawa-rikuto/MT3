#include "AffineMatrix4x4.h"

Matrix4x4 AffineMatrix4x4::MakeTranslateMatrix(const Vector3& translate) {

    Matrix4x4 translateMatrix = { {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {translate.x, translate.y, translate.z, 1}
    } };

    return translateMatrix;
}

Matrix4x4 AffineMatrix4x4::MakeScalseMatrix(const Vector3& scale) {

    Matrix4x4 scaleMatrix = { {
        {scale.x, 0, 0, 0},
        {0, scale.y, 0, 0},
        {0, 0, scale.z, 0},
        {0, 0, 0, 1}
    } };

    return scaleMatrix;
}

Matrix4x4 AffineMatrix4x4::MakeRotateXMatrix(float radian) {

    /*�O���̒��������́AMatrix4x4�\���̂̏�������\���Ă���A
      �����̒��������͔z��̏�������\���Ă��܂��B*/
    Matrix4x4 result = { {
       {1, 0, 0, 0},
       {0, cos(radian), sin(radian), 0},
       {0, -sin(radian), cos(radian), 0},
       {0, 0, 0, 1}
   } };

    return result;
}

Matrix4x4 AffineMatrix4x4::MakeRotateYMatrix(float radian) {

    Matrix4x4 result = { {
       {cos(radian), 0, -sin(radian), 0},
       {0, 1, 0, 0},
       {sin(radian), 0, cos(radian), 0},
       {0, 0, 0, 1}
   } };

    return result;
}

Matrix4x4 AffineMatrix4x4::MakeRotateZMatrix(float radian) {

    Matrix4x4 result = { {
       {cos(radian), sin(radian), 0, 0},
       {-sin(radian), cos(radian), 0, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 1}
   } };

    return result;
}

Matrix4x4 AffineMatrix4x4::Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

    Matrix4x4 answer = {};
    for (int x = 0; x < 4; ++x) {
        for (int y = 0; y < 4; ++y) {

            answer.m[x][y] = 0;
            for (int z = 0; z < 4; ++z) {

                answer.m[x][y] += m1.m[x][z] * m2.m[z][y];
            }
        }
    }

    return answer;
}

Matrix4x4 AffineMatrix4x4::MakeAffineMatrix(
    const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

    // ���s�ړ�(T)
    translateMatrix_ = MakeTranslateMatrix(translate);

    // �g�k(S)
    scaleMatrix_ = MakeScalseMatrix(scale);

    // ��](R)
    roteMatrixX_ = MakeRotateXMatrix(rotate.x);
    roteMatrixY_ = MakeRotateYMatrix(rotate.y);
    roteMatrixZ_ = MakeRotateZMatrix(rotate.z);

    roteMatrixXYZ_ =
        Mutiply(roteMatrixX_, Mutiply(roteMatrixY_, roteMatrixZ_));

    // �s��̐ς̌����@��(W = SRT)
    affineMatrix_ = Mutiply(scaleMatrix_, roteMatrixXYZ_);
    affineMatrix_ = Mutiply(affineMatrix_, translateMatrix_);

    return affineMatrix_;
}
