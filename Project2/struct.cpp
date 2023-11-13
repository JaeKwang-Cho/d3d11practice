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



Matrix MatrixInverse(const Matrix& _other)
{
    return Matrix();
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

Matrix MatrixLookAtLH(Vector4 _CameraPosition, Vector4 _LookAtPosition, Vector4 _UpDirection, Vector4& _CameraLeft, Vector4& _CameraUp)
{
    Matrix Mat;

    Vector4 CameraDirection = _LookAtPosition - _CameraPosition;

    Vector4 NegEyePosition;
    Vector4 D0, D1, D2;
    Vector4 R0, R1, R2;
    Vector4 M;

    Vector4 ZeroVector(0.f, 0.f, 0.f, 0.f);
    assert(CameraDirection != ZeroVector);
    assert(_UpDirection != ZeroVector);

    R2 = CameraDirection.Normalize3Vec(); // 카메라가 바라보는 방향 벡터 R2

    R0 = CrossVector3Vec(_UpDirection, R2); // 카메라의 위쪽 백터와 카메라가 바라보는 벡터(R2)의 외적 -> 카메라의 왼쪽 R0
    R0 = R0.Normalize3Vec();
    _CameraLeft = R0;

    R1 = CrossVector3Vec(R2, R0); // 카메라의 정수리 벡터 R1
    _CameraUp = R1.Normalize3Vec();

    // 월드 원점 대한 카메라의 상대 위치
    NegEyePosition = _CameraPosition * Vector4(-1.f, -1.f, -1.f, -1.f);

    // 월드 원점에 대한 카메라의 상대 위치를 각 축에 대해서 도트
    D0 = DotVector3Vec(R0, NegEyePosition); // 카메라의 왼쪽 
    D1 = DotVector3Vec(R1, NegEyePosition); // 카메라의 정수리
    D2 = DotVector3Vec(R2, NegEyePosition); // 카메라의 방향

    Mat.m[0] = Vector4(R0.x, R0.y, R0.z, D0.w);
    Mat.m[1] = Vector4(R1.x, R1.y, R1.z, D1.w);
    Mat.m[2] = Vector4(R2.x, R2.y, R2.z, D2.w);
    Mat.m[3] = Vector4(0.f, 0.f, 0.f, 1.f);

    Mat = MatrixTranspose(Mat);

    return Mat;
}

Vector4 RotateVectorAroundLocalAxis(const Vector4& _vec, const Vector4& _axis, float _rad)
{
    Quat4 quat = GetQuatLocalAxisRotate(_vec, _axis, _rad);
    Matrix RotMat = MatrixTranspose(GetRotationMatrixFromQuat(quat));

    return VectorTransform(_vec, RotMat);
}

Quat4 GetQuatLocalAxisRotate(const Vector4& _vec, const Vector4& _localAxis, float _rad)
{
    Vector4 axis = _localAxis.Normalize3Vec();
    axis = Vector4(axis.x, axis.y, axis.z, _vec.w);

    float SinVal = sinf(_rad / 2.f);
    float CosVal = cosf(_rad / 2.f);

    Vector4 Scale = Vector4(SinVal, SinVal, SinVal, CosVal);
    Quat4 result = { axis * Scale };
    return result;
}

Quat4 GetQuatConjugate(const Quat4& _quat)
{
    Vector4 vec = _quat.vec * Vector4(-1.f, - 1.f, -1.f, 1.f);
    return Quat4{ vec };
}

Matrix GetRotationMatrixFromQuat(const Quat4& _quat)
{
    float sx = _quat.sinX;
    float sy = _quat.sinY;
    float sz = _quat.sinZ;
    float cw = _quat.cosW;

    Vector4 m1 = Vector4(
        1.f - 2.f * (sy * sy + sz * sz),
        2.f * (sx * sy + cw * sz),
        2.f * (sx * sz - cw * sy), 
        0.f
    );

    Vector4 m2 = Vector4(
        2.f * (sx * sy - cw * sz),
        1.f - 2.f * (sx * sx + sz * sz),
        2.f * (sy * sz + cw * sx),
        0.f
    );

    Vector4 m3 = Vector4(
        2.f * (sx * sz + cw * sy),
        2.f * (sy * sz - cw * sx),
        1.f - 2.f * (sx * sx * + sy * sy),
        1.f
    );

    Vector4 m4 = Vector4(0.f, 0.f, 0.f, 1.f);

    Matrix mat(m1, m2, m3, m4);

    return MatrixTranspose(mat);
}

Vector4 VectorLocalPitchRotate(const Vector4& _vec, const Vector4& _localXAxis, float _rad)
{
    Vector4 result;
    float vecLen = _vec.Length3Vec();

    float Xcomp = _vec.x;
    float Ycomp = _vec.y;
    float Zcomp = _vec.z;

    float LocalXZPlaneCast = sqrtf(Zcomp*Zcomp + Xcomp*Xcomp);

    float CosToX = Xcomp / LocalXZPlaneCast;
    float CosToZ = Zcomp / LocalXZPlaneCast;

    float FormalRad = acosf(LocalXZPlaneCast / vecLen);
    float NewRad = FormalRad + _rad;

    if (abs(NewRad) >= PIDiv2)
    {
        NewRad = NewRad > 0.f ? PIDiv2 - FLOAT_NEAR_ZERO : FLOAT_NEAR_ZERO - PIDiv2;
    }

    float newXZPlaneCast = vecLen * cosf(NewRad);
    float newXcomp = CosToX * newXZPlaneCast;
    float newZcomp = CosToZ * newXZPlaneCast;
    float newYcomp = Ycomp >= 0.f ? vecLen * sinf(NewRad) : vecLen * sinf(NewRad) * -1.f;

    result = Vector4(newXcomp, newYcomp, newZcomp, 0.f);

    return result.Normalize3Vec();
}

Vector4 VectorLocalYawRotate(const Vector4& _vec, const Vector4& _localYAxis, float _rad)
{
    Vector4 result;
    float CosVal = cosf(_rad);
    float SinVal = sinf(_rad);

    float newXcomp = _vec.x * CosVal - _vec.z * SinVal;
    float newZcomp = _vec.x * SinVal + _vec.z * CosVal;

    result = Vector4(newXcomp, _vec.y, newZcomp, 0.f);

    return result.Normalize3Vec();
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

float Matrix::GetDeterminant()
{
    Matrix3X3 Minor11 = Matrix3X3(_22, _23, _24,
                                  _32, _33, _34,
                                  _42, _43, _44);
    Matrix3X3 Minor12 = Matrix3X3(_21, _23, _24,
                                  _31, _33, _34,
                                  _41, _43, _44);
    Matrix3X3 Minor13 = Matrix3X3(_21, _22, _24,
                                  _31, _32, _34,
                                  _41, _42, _44);
    Matrix3X3 Minor14 = Matrix3X3(_21, _22, _23,
                                  _31, _32, _33,
                                  _41, _42, _43);

    return _11 * Minor11.GetDeterminant() - _12 * Minor12.GetDeterminant() + _13 * Minor13.GetDeterminant() - _14 * Minor14.GetDeterminant();
}

Matrix Matrix::GetInverse()
{
    float det = GetDeterminant();
    if (abs(det) <= FLOAT_NEAR_ZERO)
    {
        return Matrix();
    }

    Matrix TransposedMat = MatrixTranspose(*this);

    Matrix3X3 Minor11 = Matrix3X3(TransposedMat._22, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._32, TransposedMat._33, TransposedMat._34,
                                  TransposedMat._42, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor12 = Matrix3X3(TransposedMat._21, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._31, TransposedMat._33, TransposedMat._34,
                                  TransposedMat._41, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor13 = Matrix3X3(TransposedMat._21, TransposedMat._22, TransposedMat._24,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._34,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._44);
    Matrix3X3 Minor14 = Matrix3X3(TransposedMat._21, TransposedMat._22, TransposedMat._23,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._33,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._43);

    Matrix3X3 Minor21 = Matrix3X3(TransposedMat._12, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._32, TransposedMat._33, TransposedMat._34,
                                  TransposedMat._42, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor22 = Matrix3X3(TransposedMat._11, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._31, TransposedMat._33, TransposedMat._34,
                                  TransposedMat._41, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor23 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._14,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._34,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._44);
    Matrix3X3 Minor24 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._13,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._33,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._43);

    Matrix3X3 Minor31 = Matrix3X3(TransposedMat._12, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._22, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._42, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor32 = Matrix3X3(TransposedMat._11, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._21, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._41, TransposedMat._43, TransposedMat._44);
    Matrix3X3 Minor33 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._14,
                                  TransposedMat._21, TransposedMat._22, TransposedMat._24,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._44);
    Matrix3X3 Minor34 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._13,
                                  TransposedMat._21, TransposedMat._22, TransposedMat._23,
                                  TransposedMat._41, TransposedMat._42, TransposedMat._43);

    Matrix3X3 Minor41 = Matrix3X3(TransposedMat._12, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._22, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._32, TransposedMat._33, TransposedMat._34);
    Matrix3X3 Minor42 = Matrix3X3(TransposedMat._11, TransposedMat._13, TransposedMat._14,
                                  TransposedMat._21, TransposedMat._23, TransposedMat._24,
                                  TransposedMat._31, TransposedMat._33, TransposedMat._34);
    Matrix3X3 Minor43 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._14,
                                  TransposedMat._21, TransposedMat._22, TransposedMat._24,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._34);
    Matrix3X3 Minor44 = Matrix3X3(TransposedMat._11, TransposedMat._12, TransposedMat._13,
                                  TransposedMat._21, TransposedMat._22, TransposedMat._23,
                                  TransposedMat._31, TransposedMat._32, TransposedMat._33);

    float c11 = Minor11.GetDeterminant();
    float c12 = Minor12.GetDeterminant() * -1.f;
    float c13 = Minor13.GetDeterminant();
    float c14 = Minor14.GetDeterminant() * -1.f;

    float c21 = Minor21.GetDeterminant() * -1.f;
    float c22 = Minor22.GetDeterminant();
    float c23 = Minor23.GetDeterminant() * -1.f;
    float c24 = Minor24.GetDeterminant();

    float c31 = Minor31.GetDeterminant();
    float c32 = Minor32.GetDeterminant() * -1.f;
    float c33 = Minor33.GetDeterminant();
    float c34 = Minor34.GetDeterminant() * -1.f;

    float c41 = Minor41.GetDeterminant() * -1.f;
    float c42 = Minor42.GetDeterminant();
    float c43 = Minor43.GetDeterminant() * -1.f;
    float c44 = Minor44.GetDeterminant();

    return Matrix(c11 / det, c12 / det, c13 / det, c14 / det,
                  c21 / det, c22 / det, c23 / det, c24 / det,
                  c31 / det, c32 / det, c33 / det, c34 / det,
                  c41 / det, c42 / det, c43 / det, c44 / det);
}

float Matrix3X3::GetDeterminant()
{
    Matrix2X2 minor11 = Matrix2X2(_22, _23, _32, _33);
    Matrix2X2 minor12 = Matrix2X2(_21, _23, _31, _33);
    Matrix2X2 minor13 = Matrix2X2(_21, _22, _31, _32);

    return _11 * minor11.GetDeterminant() - _12 * minor12.GetDeterminant() + _13 * minor13.GetDeterminant();
}

Matrix3X3 Matrix3X3::GetInverse()
{

    float det = GetDeterminant();
    if (abs(det) <= FLOAT_NEAR_ZERO)
    {
        return Matrix3X3();
    }

    Matrix3X3 MatTransePosed = GetTranspose();

    Matrix2X2 minor11 = Matrix2X2(MatTransePosed._22, MatTransePosed._23, MatTransePosed._32, MatTransePosed._33);
    Matrix2X2 minor12 = Matrix2X2(MatTransePosed._21, MatTransePosed._23, MatTransePosed._31, MatTransePosed._33);
    Matrix2X2 minor13 = Matrix2X2(MatTransePosed._21, MatTransePosed._22, MatTransePosed._31, MatTransePosed._32);
    Matrix2X2 minor21 = Matrix2X2(MatTransePosed._12, MatTransePosed._13, MatTransePosed._32, MatTransePosed._33);
    Matrix2X2 minor22 = Matrix2X2(MatTransePosed._11, MatTransePosed._13, MatTransePosed._31, MatTransePosed._33);
    Matrix2X2 minor23 = Matrix2X2(MatTransePosed._11, MatTransePosed._13, MatTransePosed._31, MatTransePosed._32);
    Matrix2X2 minor31 = Matrix2X2(MatTransePosed._12, MatTransePosed._13, MatTransePosed._22, MatTransePosed._23);
    Matrix2X2 minor32 = Matrix2X2(MatTransePosed._11, MatTransePosed._13, MatTransePosed._21, MatTransePosed._23);
    Matrix2X2 minor33 = Matrix2X2(MatTransePosed._11, MatTransePosed._12, MatTransePosed._21, MatTransePosed._22);

    float c11 = minor11.GetDeterminant();
    float c12 = minor12.GetDeterminant() * -1.f;
    float c13 = minor13.GetDeterminant();
    float c21 = minor21.GetDeterminant() * -1.f;
    float c22 = minor22.GetDeterminant();
    float c23 = minor23.GetDeterminant() * -1.f;
    float c31 = minor31.GetDeterminant();
    float c32 = minor32.GetDeterminant() * -1.f;
    float c33 = minor33.GetDeterminant();

    return Matrix3X3(c11 / det, c12 / det, c13 / det,
                     c21 / det, c22 / det, c23 / det,
                     c31 / det, c32 / det, c33 / det);
}

Matrix2X2 Matrix2X2::GetInverse()
{
    float det = GetDeterminant();
    if (abs(det) <= FLOAT_NEAR_ZERO)
    {
        return Matrix2X2();
    }
    return Matrix2X2(_22 / det, -12 / det, -21 / det, _11 / det);
}


