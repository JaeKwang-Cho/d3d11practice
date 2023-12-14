#include "pch.h"
#include "CObject2D.h"

void CObject2D::FindWorldTransform()
{
	assert("CObject2D::FindWorldTransform() is not implemented" && false);

	if (m_OwnerObject != nullptr)
	{
		m_WorldTransform.Position = m_OwnerObject->GetWorldTransform().Position + m_RelativeTransform.Position;
		m_WorldTransform.Rotation = m_OwnerObject->GetWorldTransform().Rotation + m_RelativeTransform.Rotation;
		m_WorldTransform.Scale = m_OwnerObject->GetWorldTransform().Scale * m_RelativeTransform.Scale;
	}
}

void CObject2D::UpdateWorldMat()
{
	// 행렬 곱 순서는 스케일 -> 자전 -> 공전 (이동 + 회전) 순서로 곱하면 된다.
	m_WorldMat = MatrixIdentity();

	Matrix ScaleMat = MatrixScale(GetWorldTransform().Scale);

	Matrix RotMat = MatrixRotationZ(GetWorldTransform().Rotation.z);
	RotMat *= MatrixRotationX(GetWorldTransform().Rotation.x);
	RotMat *= MatrixRotationX(GetWorldTransform().Rotation.y);

	Matrix TransMat = MatrixTranslation(GetWorldTransform().Position);

	m_WorldMat = ScaleMat * RotMat * TransMat;
}

void CObject2D::UpdateRenderMat()
{
	UpdateWorldMat();

	m_RenderMat = m_WorldMat * g_ViewMat * g_ProjectionMat;
}
