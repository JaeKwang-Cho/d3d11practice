#include "pch.h"
#include "CActorComponent.h"

void CActorComponent::AddComponent(const CObject* _comp)
{
	__super::AddComponent(_comp);
}

void CActorComponent::AddSubObject(const CObject* _subObj)
{
	__super::AddSubObject(_subObj);
}

void CActorComponent::Start()
{
	__super::Start();
}

void CActorComponent::UpdateObject()
{
	__super::UpdateObject();
}

void CActorComponent::UpdateComponent()
{
	__super::UpdateComponent();
}

void CActorComponent::Render()
{
	__super::Render();
}

CActorComponent::CActorComponent()
	:CObject()
{
}

CActorComponent::~CActorComponent()
{
}
