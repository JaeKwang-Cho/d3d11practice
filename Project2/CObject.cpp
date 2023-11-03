#include "pch.h"
#include "CObject.h"

void CObject::AddComponent(const CObject* _comp)
{
}

void CObject::AddSubObject(const CObject* _subObj)
{
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

void CObject::Render_Component()
{
	// update all component
	auto iter = m_Components.begin();
	for (; iter != m_Components.end(); iter++)
	{
		iter->Render();
	}
}

CObject::CObject()
	:m_Name{}, m_bAlive(true), m_bRenderComponent(false)
{
}

CObject::CObject(const CObject& _other)
{
}

CObject::~CObject()
{
	// delete all component
}
