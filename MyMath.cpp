#include "MyMath.h"

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {

    Matrix4x4 translateMatrix = { {
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {translate.x, translate.y, translate.z, 1}
    } };

    return translateMatrix;
}

Matrix4x4 MakeScalseMatrix(const Vector3& scale) {

    Matrix4x4 scaleMatrix = { {
        {scale.x, 0, 0, 0},
        {0, scale.y, 0, 0},
        {0, 0, scale.z, 0},
        {0, 0, 0, 1}
    } };

    return scaleMatrix;
}

Matrix4x4 MakeRotateXMatrix(float radian) {

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

Matrix4x4 MakeRotateYMatrix(float radian) {

    Matrix4x4 result = { {
       {cos(radian), 0, -sin(radian), 0},
       {0, 1, 0, 0},
       {sin(radian), 0, cos(radian), 0},
       {0, 0, 0, 1}
   } };

    return result;
}

Matrix4x4 MakeRotateZMatrix(float radian) {

    Matrix4x4 result = { {
       {cos(radian), sin(radian), 0, 0},
       {-sin(radian), cos(radian), 0, 0},
       {0, 0, 1, 0},
       {0, 0, 0, 1}
   } };

    return result;
}

Matrix4x4 Mutiply(const Matrix4x4& m1, const Matrix4x4& m2) {

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

Matrix4x4 MakeAffineMatrix(
    const Vector3& scale, const Vector3& rotate, const Vector3& translate) {

    // ���s�ړ�(T)
    Matrix4x4 translateMatrix_ = MakeTranslateMatrix(translate);

    // �g�k(S)
    Matrix4x4 scaleMatrix_ = MakeScalseMatrix(scale);

    // ��](R)
    Matrix4x4 roteMatrixX_ = MakeRotateXMatrix(rotate.x);
    Matrix4x4 roteMatrixY_ = MakeRotateYMatrix(rotate.y);
    Matrix4x4 roteMatrixZ_ = MakeRotateZMatrix(rotate.z);

    Matrix4x4 roteMatrixXYZ_ =
        Mutiply(roteMatrixX_, Mutiply(roteMatrixY_, roteMatrixZ_));

    // �s��̐ς̌����@��(W = SRT)
    Matrix4x4 affineMatrix_ = Mutiply(scaleMatrix_, roteMatrixXYZ_);
    affineMatrix_ = Mutiply(affineMatrix_, translateMatrix_);

    return affineMatrix_;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
   
    float x = v1.y * v2.z - v1.z * v2.y;
    float y = v1.z * v2.x - v1.x * v2.z;
    float z = v1.x * v2.y - v1.y * v2.x;

    return Vector3(x, y, z);
}

Matrix4x4 MakeIdenitiy4x4() {

    Matrix4x4 answer;
    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {

            if (row == col) {

                answer.m[row][col] = 1.0f;
            } else {

                answer.m[row][col] = 0.0f;
            }
        }
    }
    return answer;
}

Matrix4x4 TransposeMatrix(const Matrix4x4& m) {

    Matrix4x4 answer;

    for (int row = 0; row < 4; row++) {
        for (int col = 0; col < 4; col++) {

            answer.m[row][col] = m.m[row][col];
        }
    }

    return answer;
}

Matrix4x4 MakeViewportMatrix(
    float left, float top, float width, float height, float minDepth, float maxDepth) {
   
    float scaleX = width / 2.0f;
    float scaleY = height / 2.0f;
    float scaleZ = (maxDepth - minDepth);
    float offsetX = left + scaleX;
    float offsetY = top + scaleY;
    float offsetZ = minDepth;

    Matrix4x4 result = { {
        {scaleX, 0, 0, 0},
        {0, -scaleY, 0, 0},  // - scaleY�́A�r���[�|�[�g��Y�������]���Ă��邽�߁B
        {0, 0, scaleZ, 0},
        {offsetX, offsetY, offsetZ, 1}
    } };

    return result;
}

Matrix4x4 MakePerspectiveFovMatrix(
    float fovY, float aspectRatio, float nearClip, float farClip) {
    
    float tanHalfFovY = tan(fovY * 0.5f);
    float scaleX = 1.0f / (aspectRatio * tanHalfFovY);
    float scaleY = 1.0f / tanHalfFovY;
    float nearMinusFar = farClip - nearClip;

    Matrix4x4 result = { {
        {scaleX, 0, 0, 0},
        {0, scaleY, 0, 0},
        {0, 0, farClip / nearMinusFar, 1},
        {0, 0, (-farClip * nearClip) / nearMinusFar, 0}
    } };

    return result;
}

Matrix4x4 MakeOrethographicMatrx(
    float left, float top, float right, float bottom, float nearClip, float farClip) {
    
    float scaleX = 2.0f / (right - left);
    float scaleY = 2.0f / (top - bottom);
    float scaleZ = 1.0f / (farClip - nearClip);
    float offsetX = (right + left) / (left - right);
    float offsetY = (top + bottom) / (bottom - top);
    float offsetZ = (nearClip) / (nearClip - farClip);

    Matrix4x4 result = { {
        {scaleX, 0, 0, 0},
        {0, scaleY, 0, 0},
        {0, 0, scaleZ, 0},
        { offsetX, offsetY, offsetZ, 1}
    } };

    return result;
}

Matrix4x4 Inverse(const Matrix4x4& m) {
    
    Matrix4x4 invMatrix;

    float det =
        m.m[0][0] * (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) +
        m.m[0][1] * (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) +
        m.m[0][2] * (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]);

    if (det == 0) {
        return invMatrix; // �[�����Z�̃G���[����
    }

    float invDet = 1.0f / det;

    invMatrix.m[0][0] = (m.m[1][1] * m.m[2][2] - m.m[1][2] * m.m[2][1]) * invDet;
    invMatrix.m[0][1] = (m.m[0][2] * m.m[2][1] - m.m[0][1] * m.m[2][2]) * invDet;
    invMatrix.m[0][2] = (m.m[0][1] * m.m[1][2] - m.m[0][2] * m.m[1][1]) * invDet;
    invMatrix.m[0][3] = 0.0f;

    invMatrix.m[1][0] = (m.m[1][2] * m.m[2][0] - m.m[1][0] * m.m[2][2]) * invDet;
    invMatrix.m[1][1] = (m.m[0][0] * m.m[2][2] - m.m[0][2] * m.m[2][0]) * invDet;
    invMatrix.m[1][2] = (m.m[0][2] * m.m[1][0] - m.m[0][0] * m.m[1][2]) * invDet;
    invMatrix.m[1][3] = 0.0f;

    invMatrix.m[2][0] = (m.m[1][0] * m.m[2][1] - m.m[1][1] * m.m[2][0]) * invDet;
    invMatrix.m[2][1] = (m.m[0][1] * m.m[2][0] - m.m[0][0] * m.m[2][1]) * invDet;
    invMatrix.m[2][2] = (m.m[0][0] * m.m[1][1] - m.m[0][1] * m.m[1][0]) * invDet;
    invMatrix.m[2][3] = 0.0f;

    invMatrix.m[3][0] = -(m.m[3][0] * invMatrix.m[0][0] + m.m[3][1] * invMatrix.m[1][0] + m.m[3][2] * invMatrix.m[2][0]);
    invMatrix.m[3][1] = -(m.m[3][0] * invMatrix.m[0][1] + m.m[3][1] * invMatrix.m[1][1] + m.m[3][2] * invMatrix.m[2][1]);
    invMatrix.m[3][2] = -(m.m[3][0] * invMatrix.m[0][2] + m.m[3][1] * invMatrix.m[1][2] + m.m[3][2] * invMatrix.m[2][2]);
    invMatrix.m[3][3] = 1.0f;

    return invMatrix;
}
