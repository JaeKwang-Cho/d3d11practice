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
	// �̷��� ���� ���� �����ڸ� ���� �� �ξ� ���ϴ� #1
	memset(&td, 0, sizeof(td));
	td.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	td.Width = _width;
	td.Height = _height;
	td.ArraySize = 1;
	td.MipLevels = 0;
	td.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.CPUAccessFlags = 0;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.MiscFlags = 0;

	// 2D Texture data�� �������ش�.
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = _colorData;
	initData.SysMemPitch = _width * sizeof(ColorComp);
	initData.SysMemSlicePitch = 0; //3d������ ����Ѵ� �ǹ� ����.

	// ������ ����� Texture2D�� �����.
	ID3D11Texture2D* p2DTexture = nullptr;
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&td, &initData, &p2DTexture);
	m_texture = static_cast<ID3D11Texture2D*>(p2DTexture);

	// �׸��� �װ��� ���̴��� �� �� �ְ�, ���ҽ� ��� �����.
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	// �̷��� ���� ���� �����ڸ� ���� �� �ξ� ���ϴ� #2
	memset(&srvd, 0, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Format = td.Format;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;
	

	if (m_texture)
	{
		hr = g_pd3dDevice->CreateShaderResourceView(m_texture, &srvd, &m_TextureResourceView);
	}	

	m_TextureResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureComp::CreateTextureResourceViewFromColor") - 1, "TextureComp::CreateTextureResourceViewFromColor");

	return hr;
}

HRESULT TextureComp::CreateTextureResourceViewSimpleColor(const ColorComp& _colorData, aiTextureType _type)
{
	return CreateTextureResourceViewFromColor(&_colorData, 1, 1, _type);
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
	, m_texture(nullptr)
	, m_type(aiTextureType::aiTextureType_UNKNOWN)
{
	this->CreateTextureResourceViewSimpleColor(DefaultColors::UnloadedTextureColor, aiTextureType::aiTextureType_DIFFUSE);
}

TextureComp::~TextureComp()
{
	//if (m_TextureResourceView)m_TextureResourceView->Release();
	//if (m_texture) m_texture->Release();
}
