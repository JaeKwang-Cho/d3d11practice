#include "pch.h"
#include "struct.h"

float ConvertToRadians(float fDegrees)
{
    return fDegrees * (PI / 180.0f);
}
float ConvertToDegrees(float fRadians)
{
    return fRadians * (180.0f / PI);
}

FLOAT2::FLOAT2(const POINT& _point)
    : u((float)_point.x), v((float)_point.y)
{
}

Vector4 CrossVector3Vec(const Vector4& v1, const Vector4& v2)
{
    Vector4 vResult(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v2.z * v1.x,
        v1.x * v2.y - v2.x * v1.y,
        0.f);

    return vResult;
}

Vector4 DotVector3Vec(const Vector4& v1, const Vector4& v2)
{
    float Result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    return Vector4(Result, Result, Result, Result);
}

float SumVectorElements(const Vector4& v)
{
    return v.x + v.y + v.z + v.w;
}

Vector4 VectorTransform(const Vector4& _vec, const Matrix& _mat)
{
    Vector4 vResult;
    Vector4 vTemp;

    vResult.m = _mm_shuffle_ps(_vec.m, _vec.m, _MM_SHUFFLE(0, 0, 0, 0));
    vResult.m = _mm_mul_ps(vResult.m, _mat.m[0].m);
    vTemp.m = _mm_shuffle_ps(_vec.m, _vec.m, _MM_SHUFFLE(1, 1, 1, 1));
    vTemp.m = _mm_mul_ps(vTemp.m, _mat.m[1].m);
    vResult.m = _mm_add_ps(vResult.m, vTemp.m);
    vTemp.m = _mm_shuffle_ps(_vec.m, _vec.m, _MM_SHUFFLE(2, 2, 2, 2));
    vTemp.m = _mm_mul_ps(vTemp.m, _mat.m[2].m);
    vResult.m = _mm_add_ps(vResult.m, vTemp.m);

    // w 성분은 그냥 더하기
    vResult.m = _mm_add_ps(vResult.m, _mat.m[3].m);

    return vResult;
}


Matrix MatrixTranspose(const Matrix& _other)
{
    return Matrix(
        _other._11, _other._21, _other._31, _other._41,
        _other._12, _other._22, _other._32, _other._42,
        _other._13, _other._23, _other._33, _other._43,
        _other._14, _other._24, _other._34, _other._44);
}

Matrix MatrixIdentity()
{
    return Matrix(
        1.f, 0.f, 0.f, 0.f,
        0.f, 1.f, 0.f, 0.f,
        0.f, 0.f, 1.f, 0.f,
        0.f, 0.f, 0.f, 1.f);
}

Matrix MatrixRotationX(float Radian)
{
    float SinRadian = sinf(Radian);
    float CosRadian = cosf(Radian);

    Matrix M;
    M.m[0] = Vector4(1.f, 0.f, 0.f, 0.f);
    M.m[1] = Vector4(0.f, CosRadian, SinRadian * -1.f, 0.f);
    M.m[2] = Vector4(0.f, SinRadian, CosRadian, 0.f);
    M.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    return M;
}

Matrix MatrixRotationY(float Radian)
{
    float SinRadian = sinf(Radian);
    float CosRadian = cosf(Radian);

    Matrix M;
    M.m[0] = Vector4(CosRadian, 0.f, SinRadian, 0.f);
    M.m[1] = Vector4(0.f, 1.f, 0.f, 0.f);
    M.m[2] = Vector4(SinRadian * -1.f, 0.f, CosRadian, 0.f);
    M.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    return M;
}

Matrix MatrixRotationZ(float Radian)
{
    float SinRadian = sinf(Radian);
    float CosRadian = cosf(Radian);

    Matrix M;
    M.m[0] = Vector4(CosRadian, SinRadian * -1.f, 0.f, 0.f);
    M.m[1] = Vector4(SinRadian, CosRadian, 0.f, 0.f);
    M.m[2] = Vector4(0.f, 0.f, 1.f, 0.f);
    M.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    return M;
}

Matrix MatrixScale(float _sx, float _sy, float _sz)
{
    Matrix Mat;

    Mat.m[0] = Vector4(_sx, 0.f, 0.f, 0.f);
    Mat.m[1] = Vector4(0.f, _sy, 0.f, 0.f);
    Mat.m[2] = Vector4(0.f, 0.f, _sz, 0.f);
    Mat.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    return Mat;
}

Matrix MatrixTranslation(float _sx, float _sy, float _sz)
{
    Matrix Mat;

    Mat.m[0] = Vector4(1.f, 0.f, 0.f, 0.f);
    Mat.m[1] = Vector4(0.f, 1.f, 0.f, 0.f);
    Mat.m[2] = Vector4(0.f, 0.f, 1.f, 0.f);
    Mat.m[3] = Vector4(_sx, _sy, _sz, 1.f);

    return Mat;
}

Matrix MatrixTranslation(FLOAT4 _fNum)
{
    Matrix Mat;

    Mat.m[0] = Vector4(1.f, 0.f, 0.f, 0.f);
    Mat.m[1] = Vector4(0.f, 1.f, 0.f, 0.f);
    Mat.m[2] = Vector4(0.f, 0.f, 1.f, 0.f);
    Mat.m[3] = Vector4(_fNum.x, _fNum.y, _fNum.z, 1.f);

    return Mat;
}

Matrix MatrixPerspectiveFovLH(float FovRadianY, float AspectRatio, float NearZ, float FarZ)
{
    Matrix Mat;

    float SinRadian = sinf(FovRadianY * 0.5f);
    float CosRadian = cosf(FovRadianY * 0.5f);

    float Height = CosRadian / SinRadian;
    float Width = Height / AspectRatio;

    Mat.m[0] = Vector4(Width, 0.0f, 0.0f, 0.0f);
    Mat.m[1] = Vector4(0.0f, Height, 0.0f, 0.0f);
    Mat.m[2] = Vector4(0.0f, 0.0f, FarZ / (FarZ - NearZ), 1.0f);
    Mat.m[3] = Vector4(0.0f, 0.0f, -Mat.m[2].z * NearZ, 0.0f);

    return Mat;
}

Matrix MatrixLookAtLH(Vector4 CameraPosition, Vector4 LookAtPosition, Vector4 UpDirection)
{
    Matrix Mat;

    Vector4 CameraDirection = LookAtPosition - CameraPosition;

    Vector4 NegEyePosition;
    Vector4 D0, D1, D2;
    Vector4 R0, R1, R2;
    Vector4 M;

    Vector4 ZeroVector(0.f, 0.f, 0.f, 0.f);
    assert(CameraDirection != ZeroVector);
    assert(UpDirection != ZeroVector);

    R2 = CameraDirection.Normalize3Vec(); // 카메라가 바라보는 방향 벡터 R2

    R0 = CrossVector3Vec(UpDirection, R2); // 카메라의 위쪽 백터와 카메라가 바라보는 벡터(R2)의 외적 -> 카메라의 왼쪽 R0
    R0 = R0.Normalize3Vec();

    R1 = CrossVector3Vec(R2, R0); // 카메라의 정수리 벡터 R1

    // 물체에 대한 카메라의 상대 위치
    NegEyePosition = CameraPosition * Vector4(-1.f, -1.f, -1.f, -1.f);

    // 물체에 대한 카메라의 상대 위치를 각 축에 대해서 도트
    D0 = DotVector3Vec(R0, NegEyePosition);
    D1 = DotVector3Vec(R1, NegEyePosition);
    D2 = DotVector3Vec(R2, NegEyePosition);

    Mat.m[0] = Vector4(R0.x, R0.y, R0.z, D0.w);
    Mat.m[1] = Vector4(R1.x, R1.y, R1.z, D1.w);
    Mat.m[2] = Vector4(R2.x, R2.y, R2.z, D2.w);
    Mat.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    Mat = MatrixTranspose(Mat);

    return Mat;
}

Matrix& Matrix::operator=(const Matrix& _M)
{
    if (this != &_M)
    {
        for (size_t i = 0; i < 4; i++)
        {
            m[i].m = Vector4::SetVector4(_M.m[i]);
        }
    }
    return *this;
}

Matrix& Matrix::operator*=(const Matrix& _M)
{
    Matrix mat = *this;;
    Matrix t_M = MatrixTranspose(_M);

    Vector4 temp1 = mat.m[0] * t_M.m[0];
    Vector4 temp2 = mat.m[0] * t_M.m[1];
    Vector4 temp3 = mat.m[0] * t_M.m[2];
    Vector4 temp4 = mat.m[0] * t_M.m[3];

    Vector4 v(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    this->m[0] = v;

    temp1 = mat.m[1] * t_M.m[0];
    temp2 = mat.m[1] * t_M.m[1];
    temp3 = mat.m[1] * t_M.m[2];
    temp4 = mat.m[1] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    this->m[1] = v;

    temp1 = mat.m[2] * t_M.m[0];
    temp2 = mat.m[2] * t_M.m[1];
    temp3 = mat.m[2] * t_M.m[2];
    temp4 = mat.m[2] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    this->m[2] = v;

    temp1 = mat.m[3] * t_M.m[0];
    temp2 = mat.m[3] * t_M.m[1];
    temp3 = mat.m[3] * t_M.m[2];
    temp4 = mat.m[3] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    this->m[3] = v;

    return *this;
}

Matrix Matrix::operator*(const Matrix& _M) const
{
    Matrix mat = *this;;
    Matrix t_M = MatrixTranspose(_M);

    Vector4 temp1 = mat.m[0] * t_M.m[0];
    Vector4 temp2 = mat.m[0] * t_M.m[1];
    Vector4 temp3 = mat.m[0] * t_M.m[2];
    Vector4 temp4 = mat.m[0] * t_M.m[3];

    Vector4 v(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    mat.m[0] = v;

    temp1 = mat.m[1] * t_M.m[0];
    temp2 = mat.m[1] * t_M.m[1];
    temp3 = mat.m[1] * t_M.m[2];
    temp4 = mat.m[1] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    mat.m[1] = v;

    temp1 = mat.m[2] * t_M.m[0];
    temp2 = mat.m[2] * t_M.m[1];
    temp3 = mat.m[2] * t_M.m[2];
    temp4 = mat.m[2] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    mat.m[2] = v;

    temp1 = mat.m[3] * t_M.m[0];
    temp2 = mat.m[3] * t_M.m[1];
    temp3 = mat.m[3] * t_M.m[2];
    temp4 = mat.m[3] * t_M.m[3];

    v = Vector4(SumVectorElements(temp1), SumVectorElements(temp2), SumVectorElements(temp3), SumVectorElements(temp4));
    mat.m[3] = v;

    return mat;
}