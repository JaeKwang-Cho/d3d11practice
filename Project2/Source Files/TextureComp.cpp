#include "pch.h"
#include "TextureComp.h"

HRESULT TextureComp::CreateTextureResourceViewFromImage(LPCWSTR _TextureName)
{
	// Switch ~
	return CreateTextureResourceViewFromdds(_TextureName);
}

HRESULT TextureComp::CreateTextureResourceViewFromColor(const ColorComp* _colorData, UINT _width, UINT _height, aiTextureType _type)
{
	// 타입을 일단 저장해준다.
	m_type = _type;

	// 2D Texture desc를 지정해준다.
	CD3D11_TEXTURE2D_DESC td;
	memset(&td, 0, sizeof(td));
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = _width;
	td.Height = _height;

	// 2D Texture data를 지정해준다.
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = _colorData;
	initData.SysMemPitch = _width * sizeof(_colorData);

	// 설정한 값들로 Texture2D를 만든다.
	ID3D11Texture2D* p2DTexture = static_cast<ID3D11Texture2D*>(m_texture);
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&td, &initData, &p2DTexture);

	// 그리고 그것을 쉐이더가 볼 수 있게, 리소스 뷰로 만든다.
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	memset(&srvd, 0, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Format = td.Format;

	hr = g_pd3dDevice->CreateShaderResourceView(m_texture, &srvd, &m_TextureResourceView);

	return hr;
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
	m_TextureSampler->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureComp::CreateDefaultTextureSampler") - 1, "TextureComp::CreateDefaultTextureSampler");
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
	m_TextureResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("extureComp::CreateTextureResourceViewFromdds") - 1, "extureComp::CreateTextureResourceViewFromdds");
	return hr;
}

TextureComp::TextureComp()
	: m_TextureResourceView(nullptr)
	, m_TextureSampler(nullptr)
	, m_texture(nullptr)
	, m_type(aiTextureType::aiTextureType_UNKNOWN)
{
}

TextureComp::~TextureComp()
{
	if (m_TextureResourceView)m_TextureResourceView->Release();
	if (m_TextureSampler) m_TextureSampler->Release();
	if (m_texture) m_texture->Release();
}
