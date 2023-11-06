#include "pch.h"
#include "CActor.h"

CActor::CActor()
	: CObject()
	, m_vPosition(FLOAT4(0.f, 0.f, 0.f, 0.f))
	, m_vRotation(FLOAT3(0.f, 0.f, 0.f))
	, m_vScale(FLOAT3(1.f, 1.f, 1.f))
{
}

CActor::~CActor()
{
}

void CActor::StartObject()
{
	SetPos(m_vPosition);
	SetRotation(m_vRotation);
	SetScale(m_vScale);
}

void CActor::UpdateObject()
{
}

void CActor::RenderObject()
{
}

CObject* CActor::Clone()
{
	return nullptr;
}



void CActor::SetPos(float _x, float _y, float _z)
{
	m_vPosition = FLOAT4(_x, _y, _z, 1);
	m_PosMat = MatrixTranslation(m_vPosition);
}

void CActor::SetPos(FLOAT4 _pos)
{
	m_vPosition = _pos;
	m_PosMat = MatrixTranslation(m_vPosition);
}

void CActor::SetRotation(float _xAxis, float _yAxis, float _zAxis, ROT_ORDER _order)
{
	m_vRotation = FLOAT3(_xAxis, _yAxis, _zAxis);
	switch (_order)
	{
	case ROT_ORDER::XYZ:
	{
		m_RotMat = MatrixRotationX(_xAxis);
		m_RotMat *= MatrixRotationY(_yAxis);
		m_RotMat *= MatrixRotationZ(_zAxis);
	}
	break;
	case ROT_ORDER::XZY:
	{
		m_RotMat = MatrixRotationX(_xAxis);
		m_RotMat *= MatrixRotationZ(_zAxis);
		m_RotMat *= MatrixRotationY(_yAxis);	
	}
	break;
	case ROT_ORDER::YXZ:
	{
		m_RotMat = MatrixRotationY(_yAxis);
		m_RotMat *= MatrixRotationX(_xAxis);
		m_RotMat *= MatrixRotationZ(_zAxis);
	}
	break;
	case ROT_ORDER::YZX:
	{
		m_RotMat = MatrixRotationY(_yAxis);
		m_RotMat *= MatrixRotationZ(_zAxis);
		m_RotMat *= MatrixRotationX(_xAxis);
	}
	break;
	case ROT_ORDER::ZXY:
	{
		m_RotMat = MatrixRotationZ(_zAxis);
		m_RotMat *= MatrixRotationX(_xAxis);
		m_RotMat *= MatrixRotationY(_yAxis);
	}
	break;
	case ROT_ORDER::ZYX:
	{
		m_RotMat = MatrixRotationZ(_zAxis);
		m_RotMat *= MatrixRotationY(_yAxis);
		m_RotMat *= MatrixRotationX(_xAxis);
	}
	break;
	default:
		break;
	}
}

void CActor::SetRotation(FLOAT3 _Rot, ROT_ORDER _order)
{
	m_vRotation = _Rot;
	switch (_order)
	{
	case ROT_ORDER::XYZ:
	{
		m_RotMat = MatrixRotationX(_Rot.x);
		m_RotMat *= MatrixRotationY(_Rot.y);
		m_RotMat *= MatrixRotationZ(_Rot.z);
	}
	break; case ROT_ORDER::XZY:
	{
		m_RotMat = MatrixRotationX(_Rot.x);
		m_RotMat *= MatrixRotationZ(_Rot.z);
		m_RotMat *= MatrixRotationY(_Rot.y);
	}
	break;
	case ROT_ORDER::YXZ:
	{
		m_RotMat = MatrixRotationY(_Rot.y);
		m_RotMat *= MatrixRotationX(_Rot.x);
		m_RotMat *= MatrixRotationZ(_Rot.z);
	}
	break;
	case ROT_ORDER::YZX:
	{
		m_RotMat = MatrixRotationY(_Rot.y);
		m_RotMat *= MatrixRotationZ(_Rot.z);
		m_RotMat *= MatrixRotationX(_Rot.x);
	}
	break;
	case ROT_ORDER::ZXY:
	{
		m_RotMat = MatrixRotationZ(_Rot.z);
		m_RotMat *= MatrixRotationX(_Rot.x);
		m_RotMat *= MatrixRotationY(_Rot.y);
	}
	break;
	case ROT_ORDER::ZYX:
	{
		m_RotMat = MatrixRotationZ(_Rot.z);
		m_RotMat *= MatrixRotationY(_Rot.y);
		m_RotMat *= MatrixRotationX(_Rot.x);
	}
	break;
	default:
		break;
	}
}

void CActor::SetScale(float _xs, float _ys, float _zs)
{
	m_vScale = FLOAT3(_xs, _ys, _zs);
	m_ScalMat = MatrixScale(_xs, _ys, _zs);
}

void CActor::SetScale(FLOAT3 _Scale)
{
	m_vScale = _Scale;
	m_ScalMat = MatrixScale(_Scale.x, _Scale.y, _Scale.z);
}