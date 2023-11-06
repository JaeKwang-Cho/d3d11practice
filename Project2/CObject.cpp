#include "pch.h"
#include "CObject.h"

void CObject::AddComponent(const CObject* _comp)
{
}

void CObject::Start()
{
	StartObject();

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

	UpdateComponent();
}

void CObject::UpdateComponent()
{
	// update all component
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		(*iter)->UpdateObject();
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

CObject::CObject()
	: m_OwnerObject(nullptr)
	, m_Name{}
	, m_bAlive(true)
	, m_bRenderComponent(false)
{
}

CObject::CObject(const CObject& _other)
	: m_OwnerObject(nullptr)
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
