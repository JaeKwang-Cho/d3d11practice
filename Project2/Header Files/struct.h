#pragma once

#include <DirectXMath.h>
#include "pch.h"

#define FLOAT_NEAR_ZERO (0.00001f)
#define FLOAT_NEAR_ONE (0.99f)

float ConvertToRadians(float fDegrees);

float ConvertToDegrees(float fRadians);

// struct
struct FLOAT2 {

    float u;
    float v;

    FLOAT2(float _u, float _v) : u(_u), v(_v)
    {
    }
    FLOAT2() : u(0.f), v(0.f)
    {
    }
    FLOAT2(const POINT& _point);
};

struct FLOAT3 {
    float x;
    float y;
    float z;

    FLOAT3(float _x, float _y, float _z) :x(_x), y(_y), z(_z)
    {
    }
    FLOAT3() :x(0.f), y(0.f), z(0.f)
    {
    }

    FLOAT3 operator+(const FLOAT3& _other) const
    {
        return FLOAT3(x + _other.x, y + _other.y, z + _other.z);
    }

    FLOAT3& operator+=(const FLOAT3& _other)
    {
        x += _other.x;
        y += _other.y;
        z += _other.z;

        return *this;
    }

    FLOAT3 operator*(const FLOAT3& _other) const
    {
        return FLOAT3(x * _other.x, y * _other.y, z * _other.z);
    }

    FLOAT3& operator*=(const FLOAT3& _other)
    {
        x *= _other.x;
        y *= _other.y;
        z *= _other.z;

        return *this;
    }

    FLOAT3 Normalize() const{
        double tmp = (double)x * (double)x + (double)y * (double)y + (double)z*(double)z;
        float len = (float)sqrt(tmp);

        return FLOAT3(x / len, y / len, z / len);
    }
};

struct Transform {
    FLOAT3 Position;
    FLOAT3 Rotation;
    FLOAT3 Scale;

    Transform()
        :Position(0.f,0.f,0.f),Rotation(0.f,0.f,0.f),Scale(1.f,1.f,1.f)
    {}
    Transform(FLOAT3 _Pos, FLOAT3 _Rot, FLOAT3 _SCL)
        :Position(_Pos),Rotation(_Rot),Scale(_SCL)
    {}
    Transform(const Transform& _other)
        :Position(_other.Position)
        ,Rotation(_other.Rotation)
        ,Scale(_other.Scale)
    { }
};

struct FLOAT4 {
    float x;
    float y;
    float z;
    float a;

    FLOAT4(float _x, float _y, float _z, float _a) :x(_x), y(_y), z(_z), a(_a)
    {
    }
    FLOAT4() :x(0.f), y(0.f), z(0.f), a(0.f)
    {
    }

    FLOAT4(FLOAT3 _pos, float _a)
        :x(_pos.x), y(_pos.y), z(_pos.z), a(_a)
    {

    }

    FLOAT4 Nomalize()
    {
        double sumP = (double)x * (double)x + (double)y * (double)y + (double)z * (double)z + (double)a * (double)a;
        float sum = (float)sqrt(sumP);
        if (sum <= FLOAT_NEAR_ZERO)
        {
            return FLOAT4(0.f, 0.f, 0.f, 0.f);
        }
        return FLOAT4(x / sum, y / sum, z / sum, a / sum);
    }
};

// For SIMD
struct Vector4 {
    union
    {
        __m128 m;// <xnamath.h>
        struct
        {
            float x;
            float y;
            float z;
            float w;
        };
    };
    Vector4()
    {
        m = _mm_set_ps(0.f, 0.f, 0.f, 0.f);
    }
    Vector4(float _x, float _y, float _z, float _a)
    {
        m = _mm_set_ps(_a, _z, _y, _x);
    }
    Vector4(const Vector4& _other)
    {
        m = _other.m;
    }
    Vector4(const FLOAT4& _fNum)
    {
        m = _mm_set_ps(_fNum.z, _fNum.z, _fNum.y, _fNum.x);
    }
    Vector4 operator-(const Vector4& _other) const
    {
        Vector4 vec;
        vec.m = _mm_sub_ps(m, _other.m);
        return vec;
    }

    Vector4 operator+(const Vector4& _other)
    {
        Vector4 vec;
        vec.m = _mm_add_ps(m, _other.m);

        return vec;
    }

    Vector4 operator*(float _other) const
    {
        Vector4 vec;
        Vector4 _scalar(_other, _other, _other, _other);
        vec.m = _mm_mul_ps(m, _scalar.m);
        return vec;
    }

    Vector4 operator*(const Vector4& _other) const
    {
        Vector4 vec;
        vec.m = _mm_mul_ps(m, _other.m);
        return vec;
    }

    bool operator==(const Vector4& _other) const
    {
        return _other.x == x &&
            _other.y == y &&
            _other.z == z &&
            _other.w == w;
    }

    bool operator!=(const Vector4& _other) const
    {
        return _other.x != x ||
            _other.y != y ||
            _other.z != z ||
            _other.w != w;
    }

    float Length3Vec() const
    {
        double sum = 0.;
        Vector4 vec;
        vec.m = _mm_mul_ps(m, m);
        sum += (double)vec.x + (double)vec.y + (double)vec.z;

        return (float)sqrt(sum);
    }

    Vector4 Normalize3Vec() const
    {
        float len = Length3Vec();
        if (len <= FLOAT_NEAR_ZERO)
        {
            return Vector4(0.f, 0.f, 0.f, 0.f);
        }
        return Vector4(x / len, y / len, z / len, w);
    }

    static __m128 SetVector4(const Vector4& _other)
    {
        return _other.m;
    }

    static __m128 SetVector4(float _x, float _y, float _z, float _a)
    {
        return _mm_set_ps(_a, _z, _y, _x);
    }

    FLOAT4 GetFloat4()
    {
        return FLOAT4(x, y, z, w);
    }

    friend struct Matrix;
};

struct Quat4 {
    union {
        Vector4 vec;
        struct
        {
            float sinX;
            float sinY;
            float sinZ;
            float cosW;
        };
    };
    Quat4(const Quat4& _quat)
        :vec(_quat.vec)
    {}

    Quat4(const Vector4& _vec)
        :vec(_vec)
    {}
    Quat4()
        :vec()
    {}

    Quat4& operator=(const Quat4& _quat)
    {
        if (this == &_quat)
        {

        }
        vec = _quat.vec;
        return *this;
    }
};

Vector4 CrossVector3Vec(const Vector4& v1, const Vector4& v2);

Vector4 DotVector3Vec(const Vector4& v1, const Vector4& v2);

float SumVectorElements(const Vector4& v);

struct Matrix2X2 {
    union {
        FLOAT2 m[2];
        struct {
            float _11, _12;
            float _21, _22;
        };
    };
    Matrix2X2()
    {
        _11 = _12 = _21 = _22 = 0.f;
    }

    Matrix2X2(float m11, float m12, float m21, float m22)
    {
        _11 = m11;
        _12 = m12;
        _21 = m21;
        _22 = m22;
    }

    float GetDeterminant()
    {
        return _11 * _22 - _21 * _12;
    }

    Matrix2X2 GetInverse();
};

struct Matrix3X3 {
    union {
        FLOAT3 m[3];
        struct {
            float _11, _12, _13;
            float _21, _22, _23;
            float _31, _32, _33;
        };
        float r[3][3];
    };

    Matrix3X3()
    {
        _11 = _12 = _13 = 0.f;
        _21 = _22 = _23 = 0.f;
        _31 = _32 = _33 = 0.f;
    }

    Matrix3X3(float m11, float m12, float m13,
              float m21, float m22, float m23,
              float m31, float m32, float m33)
    {
        _11 = m11;
        _12 = m12;
        _13 = m13;
        _21 = m21;
        _22 = m22;
        _23 = m23;
        _31 = m31;
        _32 = m32;
        _33 = m33;
    }

    float GetDeterminant();

    Matrix3X3 GetTranspose()
    {
        return Matrix3X3(_11, _21, _31, _12, _22, _32, _13, _23, _33);
    }

    Matrix3X3 GetInverse();
};

struct Matrix {
    union
    {
        Vector4 m[4];
        struct
        {
            float _11, _12, _13, _14;
            float _21, _22, _23, _24;
            float _31, _32, _33, _34;
            float _41, _42, _43, _44;
        };
        float r[4][4];
    };
    // Operator Overload
    Matrix()
    {
        m[0] = Vector4();
        m[1] = Vector4();
        m[2] = Vector4();
        m[3] = Vector4();
    };
    Matrix(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 v3)
    {
        m[0].m = Vector4::SetVector4(v0);
        m[1].m = Vector4::SetVector4(v1);
        m[2].m = Vector4::SetVector4(v2);
        m[3].m = Vector4::SetVector4(v3);
    }
    Matrix(float m00, float m01, float m02, float m03,
           float m10, float m11, float m12, float m13,
           float m20, float m21, float m22, float m23,
           float m30, float m31, float m32, float m33)
    {
        m[0].m = Vector4::SetVector4(m00, m01, m02, m03);
        m[1].m = Vector4::SetVector4(m10, m11, m12, m13);
        m[2].m = Vector4::SetVector4(m20, m21, m22, m23);
        m[3].m = Vector4::SetVector4(m30, m31, m32, m33);
    }
    Matrix(const float* pArray)
    {
        for (size_t i = 0; i < 4; i++)
        {
            m[i].m = Vector4::SetVector4(pArray[i * 4], pArray[i * 4 + 1], pArray[i * 4 + 2], pArray[i * 4 + 3]);
        }
    }
    Matrix(const Matrix& _other)
    {
        m[0] = _other.m[0];
        m[1] = _other.m[1];
        m[2] = _other.m[2];
        m[3] = _other.m[3];
    }

    float operator() (UINT Row, UINT Col) const
    {
        return r[Row][Col];
    }
    float& operator() (UINT Row, UINT Col)
    {
        return r[Row][Col];
    }

    Matrix& operator=(const Matrix& _M);

    Matrix& operator*=(const Matrix& _M);

    Matrix operator*(const Matrix& _M) const;

    float GetDeterminant();

    Matrix GetInverse();

    friend struct Vector4;
};

Vector4 VectorTransform(const Vector4& _vec, const Matrix& _mat);

Matrix MatrixTranspose(const Matrix& _other);

Matrix MatrixInverse(const Matrix& _other);

Matrix MatrixIdentity();

Matrix MatrixRotationX(float Radian);

Matrix MatrixRotationY(float Radian);

Matrix MatrixRotationZ(float Radian);

Matrix MatrixScale(float _sx, float _sy, float _sz);

Matrix MatrixScale(FLOAT3 _fNum);

Matrix MatrixTranslation(float _sx, float _sy, float _sz);

Matrix MatrixTranslation(FLOAT3 _fNum);

Matrix MatrixPerspectiveFovLH(float _FovRadianY, float _AspectRatio, float _NearZ, float _FarZ);

Matrix MatrixOrthographicLH(float _ViewWidth, float _ViewHeight, float _NearZ, float _FarZ);

Matrix MatrixOrthographicOffCenterLH(float _ViewLeft, float _ViewRight, float _ViewBottom, float _ViewTop, float _NearZ, float _FarZ);

Matrix MatrixLookAtLH(Vector4 _CameraPosition, Vector4 _LookAtPosition, Vector4 _UpDirection, Vector4& _CameraLeft, Vector4& _CameraUp);

// World Space rotation order : XYZ
 
// Local Space rotation order : ZYX

Vector4 RotateVectorAroundLocalAxis(const Vector4& _vec, const Vector4& _axis, float _rad);

Quat4 GetQuatLocalAxisRotate(const Vector4& _vec, const Vector4& _localAxis, float _rad);

Quat4 GetQuatConjugate(const Quat4& _quat);

Matrix GetRotationMatrixFromQuat(const Quat4& _quat);