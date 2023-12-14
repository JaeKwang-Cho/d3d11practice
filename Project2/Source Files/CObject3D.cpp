#include "pch.h"
#include "CObject3D.h"

void CObject3D::FindWorldTransform()
{
	if (m_OwnerObject != nullptr)
	{
		m_WorldTransform.Position = m_OwnerObject->GetWorldTransform().Position + m_RelativeTransform.Position;
		m_WorldTransform.Rotation = m_OwnerObject->GetWorldTransform().Rotation + m_RelativeTransform.Rotation;
		m_WorldTransform.Scale = m_OwnerObject->GetWorldTransform().Scale * m_RelativeTransform.Scale;
	}
}
