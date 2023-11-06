#include "pch.h"
#include "TextActor.h"
#include "CRenderAsset.h"

void TextActor::StartObject()
{
	CRenderAsset* pRA = new CRenderAsset();
	pRA->SetOwnerObject(this);
	AddComponent(pRA);
}

TextActor::TextActor()
{
}

TextActor::~TextActor()
{
}
