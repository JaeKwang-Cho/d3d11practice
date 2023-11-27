#include "pch.h"
#include "TextureComp.h"

HRESULT TextureComp::CreateTextureResourceView(LPCWSTR _TextureName)
{
	// Switch ~
	return CreateTextureResourceViewFromdds(_TextureName);
}

HRESULT TextureComp::CreateDefaultTextureSampler()
{
	// �ؽ����� Sampler state �����
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // �����ؼ� �� �� �ִ�. ������ ���� �����ϴµ� ���δ�. 
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap�� ���� �Ѿ�ٸ� 0 ~ 1 , 1 ~ 0 ���� �ݺ��� �Ǵ� ���̰�
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror�� 0 ~ 1 ~ 0���� �ݺ��� �ȴ�.
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp�� 0 ~ 1�� �����, ������ �ȼ��� ä������.
	// Border�� �Ѿ�� Border Color�� �������� ������ ä������.
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // ���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ�
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


	HRESULT hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &m_TextureSampler);
	return hr;
}

HRESULT TextureComp::CreateTextureResourceViewFromdds(LPCWSTR _TextureName)
{
	ScratchImage scratchImage = ScratchImage{};

	HRESULT hr = LoadFromDDSFile(_TextureName, DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &m_TextureResourceView);
	return hr;
}

TextureComp::TextureComp()
	: m_TextureResourceView(nullptr)
	, m_TextureSampler(nullptr)
{
}

TextureComp::~TextureComp()
{
	if (m_TextureResourceView)m_TextureResourceView->Release();
	if (m_TextureSampler) m_TextureSampler->Release();
}
