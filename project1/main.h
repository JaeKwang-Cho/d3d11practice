#pragma once

#include "resource.h"

// struct
struct FLOAT3 {
    float x;
    float y;
    float z;

    FLOAT3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
    FLOAT3() :x(0.f), y(0.f), z(0.f) {}
};

struct FLOAT4 {
    float x;
    float y;
    float z;
    float a;

    FLOAT4(float _x, float _y, float _z, float _a) :x(_x), y(_y), z(_z), a(_a) {}
    FLOAT4() :x(0.f), y(0.f), z(0.f), a(0.f) {}
};

struct SimpleVertex
{
    FLOAT3 Pos; 
    FLOAT4 Color;
};

// For SIMD
typedef struct Vector4 {
    union
    {
        __m128 m;// <xnamath.h>
        struct
        {
            float x;
            float y;
            float z;
            float a;
        };
    };
    Vector4() {}
    Vector4(float _x, float _y, float _z, float _a) {
        m = _mm_set_ps(_a, _z, _y, _x);
    }
    Vector4(const Vector4& _other) {
        m = _mm_set_ps(_other.a, _other.z, _other.y, _other.x);
    }
    static __m128 SetVector4(const Vector4& _other) {
        return _mm_set_ps(_other.a, _other.z, _other.y, _other.x);
    }

    static __m128 SetVector4(float _x, float _y, float _z, float _a) {
        return _mm_set_ps(_a, _z, _y, _x);
    }
};

typedef struct Matrix {
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
    Matrix(Vector4 v0, Vector4 v1, Vector4 v2, Vector4 v3) {
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
    Matrix(const float* pArray) {
        const float* p = pArray;
        for (size_t i = 0; i < 4; i++) {
            m[i].m = Vector4::SetVector4(pArray[i * 4], pArray[i * 4 + 1], pArray[i * 4 + 2], pArray[i * 4 + 3]);
        }
    }
    Matrix(const Matrix& _other) {
        m[0] = _other.m[0];
        m[1] = _other.m[1];
        m[2] = _other.m[2];
        m[3] = _other.m[3];
    }

    float operator() (UINT Row, UINT Col) const { return r[Row][Col]; }
    float& operator() (UINT Row, UINT Col) { return r[Row][Col]; }

    Matrix& operator=(const Matrix& _M) {
        for (size_t i = 0; i < 4; i++) {
            m[i].m = Vector4::SetVector4(_M.m[i]);
        }
    }

    Matrix& operator*=(const Matrix& _M) {
        m[0].m = _mm_mul_ps(m[0].m, _M.m[0].m);
        m[1].m = _mm_mul_ps(m[1].m, _M.m[1].m);
        m[2].m = _mm_mul_ps(m[2].m, _M.m[2].m);
        m[3].m = _mm_mul_ps(m[3].m, _M.m[3].m);

        return *this;
    }

    Matrix operator*(const Matrix& _M) const {
        Matrix mat;
        mat.m[0].m = _mm_mul_ps(m[0].m, _M.m[0].m);
        mat.m[1].m = _mm_mul_ps(m[1].m, _M.m[1].m);
        mat.m[2].m = _mm_mul_ps(m[2].m, _M.m[2].m);
        mat.m[3].m = _mm_mul_ps(m[3].m, _M.m[3].m);

        return mat;
    }

    static Matrix MatrixTranspose(const Matrix& _other) {
        return Matrix(
            _other._11, _other._21, _other._31, _other._41,
            _other._12, _other._22, _other._32, _other._42,
            _other._13, _other._23, _other._33, _other._43,
            _other._14, _other._24, _other._34, _other._44);
    }
};

struct ConstantBuffer
{
    Matrix mWorld;
    Matrix mView;
    Matrix mProjection;
};