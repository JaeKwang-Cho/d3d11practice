#pragma once
#include "CObject.h"

class CRenderAsset :
    public CObject
{
private:
	ID3D11VertexShader* m_VS;
	ID3D11Buffer* m_VertexBuff;
	ID3D11InputLayout* m_Layout;
	ID3D11PixelShader* m_PS;
	ID3D11Buffer* m_IndexBuff;
	const wchar_t* m_texFile;
	ID3D11ShaderResourceView* m_texRV;
	ID3D11SamplerState* m_SamplerLinear;
public:

public:
	virtual void StartObject() override;

	virtual void UpdateObject() override;
	virtual void RenderObject() override;

	virtual CRenderAsset* Clone()
	{
		return nullptr;
	};

public:
	CRenderAsset();
	virtual ~CRenderAsset();
};

