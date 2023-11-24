#include "pch.h"
#include "TextureComp.h"

HRESULT TextureComp::CreateTextureResourceView(LPCWSTR _TextureName)
{
	// Switch ~
	return CreateTextureResourceViewFromdds(_TextureName);
}

HRESULT TextureComp::CreateDefaultTextureSampler()
{
	// 텍스쳐의 Sampler state 만들기
	D3D11_SAMPLER_DESC sampDesc;
	memset(&sampDesc, 0, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 조합해서 쓸 수 있다. 지금은 선형 보간하는데 쓰인다. 
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap은 값이 넘어간다면 0 ~ 1 , 1 ~ 0 으로 반복이 되는 것이고
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror는 0 ~ 1 ~ 0으로 반복이 된다.
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp는 0 ~ 1을 벗어나면, 마지막 픽셀로 채워진다.
	// Border는 넘어가면 Border Color로 지정해준 색으로 채워진다.
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // 셈플링된 데이터를 기존 샘플링된 데이터와 비교하는 함수
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
