#include "pch.h"
#include "CObject.h"

void CObject::Start()
{
	StartObject();

	FindWorldTransform();

	StartComponent();
}

void CObject::StartComponent()
{
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		(*iter)->StartObject();
	}
}

void CObject::Update()
{
	UpdateObject();

	FindWorldTransform();

	UpdateComponent();
}

void CObject::UpdateComponent()
{
	// update all component
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		(*iter)->Update();
	}
}

void CObject::Render()
{
	RenderObject();

	Render_Component();
}

void CObject::Render_Component()
{
	// update all component
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		(*iter)->Render();
	}
}

void CObject::FindWorldTransform()
{
	if (m_OwnerObject != nullptr)
	{
		m_WorldTransform.Position = m_OwnerObject->GetWorldTransform().Position + m_RelativeTransform.Position;
		m_WorldTransform.Rotation = m_OwnerObject->GetWorldTransform().Rotation + m_RelativeTransform.Rotation;
		m_WorldTransform.Scale = m_OwnerObject->GetWorldTransform().Scale * m_RelativeTransform.Scale;
	}
}

CObject::CObject()
	: m_OwnerObject(nullptr)
	, m_RelativeTransform()
	, m_WorldTransform()
	, m_Name{}
	, m_bAlive(true)
	, m_bRenderComponent(false)
{
}

CObject::CObject(const CObject& _other)
	: m_OwnerObject(nullptr)
	, m_RelativeTransform()
	, m_WorldTransform()
	, m_Name{}
	, m_bAlive(true)
	, m_bRenderComponent(false)
{
}

CObject::~CObject()
{
	// delete all component
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		delete (*iter);
	}
	m_Components.clear();
}