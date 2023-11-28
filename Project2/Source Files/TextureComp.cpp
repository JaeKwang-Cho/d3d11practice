#include "pch.h"
#include "TextureComp.h"

HRESULT TextureComp::CreateTextureResourceViewFromImage(LPCWSTR _TextureName)
{
	// Switch ~
	return CreateTextureResourceViewFromdds(_TextureName);
}

HRESULT TextureComp::CreateTextureResourceViewFromColor(const ColorComp* _colorData, UINT _width, UINT _height, aiTextureType _type)
{
	// Ÿ���� �ϴ� �������ش�.
	m_type = _type;

	// 2D Texture desc�� �������ش�.
	CD3D11_TEXTURE2D_DESC td;
	memset(&td, 0, sizeof(td));
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = _width;
	td.Height = _height;

	// 2D Texture data�� �������ش�.
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = _colorData;
	initData.SysMemPitch = _width * sizeof(_colorData);

	// ������ ����� Texture2D�� �����.
	ID3D11Texture2D* p2DTexture = static_cast<ID3D11Texture2D*>(m_texture);
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&td, &initData, &p2DTexture);

	// �׸��� �װ��� ���̴��� �� �� �ְ�, ���ҽ� ��� �����.
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	memset(&srvd, 0, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Format = td.Format;

	hr = g_pd3dDevice->CreateShaderResourceView(m_texture, &srvd, &m_TextureResourceView);

	return hr;
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
