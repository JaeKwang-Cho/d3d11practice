#include "pch.h"
#include "CRenderAsset.h"

void CRenderAsset::AddComponent(const CObject* _comp)
{
    __super::AddComponent(_comp);
}

void CRenderAsset::AddSubObject(const CObject* _subObj)
{
    __super::AddSubObject(_subObj);
}

void CRenderAsset::Start()
{
    __super::Start();
}

void CRenderAsset::UpdateObject()
{
    __super::UpdateObject();
}

void CRenderAsset::UpdateComponent()
{
    __super::UpdateComponent();
}

void CRenderAsset::Render()
{
    __super::Render();
}

CRenderAsset::CRenderAsset()
    :CObject(), Vertices(nullptr), Indices(nullptr)
{
   
}

CRenderAsset::~CRenderAsset()
{
}
