#include "pch.h"
#include "TextureComp.h"

HRESULT TextureComp::CreateTextureResourceViewFromImage(wstring _TexturePath, aiTextureType _type)
{
	HRESULT hr = E_NOTIMPL;
	m_type = _type;
	if (StringHelper::GetFileExtension(_TexturePath) == L".dds")
	{
		ScratchImage scratchImage = ScratchImage{};

		hr = LoadFromDDSFile(_TexturePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &m_TextureResourceView);

		m_TextureResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureComp::CreateTextureResourceViewFromImage") - 1, "TextureComp::CreateTextureResourceViewFromImage");
	}
	else if (StringHelper::GetFileExtension(_TexturePath) ==L".tga")
	{

	}
	else if (StringHelper::GetFileExtension(_TexturePath) == L".hdr")
	{

	}
	else
	{
		ScratchImage scratchImage = ScratchImage{};

		hr = LoadFromWICFile(_TexturePath.c_str(), DirectX::WIC_FLAGS_NONE, nullptr, scratchImage);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &m_TextureResourceView);

		m_TextureResourceView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureComp::CreateTextureResourceViewFromImage") - 1, "TextureComp::CreateTextureResourceViewFromImage");
	}

	return hr;
}

HRESULT TextureComp::CreateTextureResourceViewFromColor(const ColorComp* _colorData, UINT _width, UINT _height, aiTextureType _type)
{
	// 타입을 일단 저장해준다.
	m_type = _type;

	// 2D Texture desc를 지정해준다.
	CD3D11_TEXTURE2D_DESC td;
	// 이렇게 하지 말고 생성자를 쓰는 게 훨씬 편하다 #1
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

	// 2D Texture data를 지정해준다.
	D3D11_SUBRESOURCE_DATA initData;
	memset(&initData, 0, sizeof(initData));
	initData.pSysMem = _colorData;
	initData.SysMemPitch = _width * sizeof(ColorComp);
	initData.SysMemSlicePitch = 0; //3d에서만 사용한다 의미 없다.

	// 설정한 값들로 Texture2D를 만든다.
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&td, &initData, (ID3D11Texture2D**) & m_texture);

	// 그리고 그것을 쉐이더가 볼 수 있게, 리소스 뷰로 만든다.
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	// 이렇게 하지 말고 생성자를 쓰는 게 훨씬 편하다 #2
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

HRESULT TextureComp::CreateTextureResourceViewFromImage(LPCSTR _TexturePath, aiTextureType _type)
{
	string str(_TexturePath);
	return CreateTextureResourceViewFromImage(StringHelper::StrToWstr(str), _type);
}

HRESULT TextureComp::CreateTextureResourceViewFromImage(LPCWSTR _TexturePath, aiTextureType _type)
{
	wstring wstr(_TexturePath);
	return CreateTextureResourceViewFromImage(wstr, _type);
}

HRESULT TextureComp::CreateTextureResourceViewFromImage(string _TexturePath, aiTextureType _type)
{
	return CreateTextureResourceViewFromImage(StringHelper::StrToWstr(_TexturePath), _type);
}

TextureComp::TextureComp()
	: m_TextureResourceView(nullptr)
	, m_texture(nullptr)
	, m_type(aiTextureType::aiTextureType_UNKNOWN)
{
	
}

TextureComp::TextureComp(const ColorComp& _colorData, aiTextureType _type)
	: m_TextureResourceView(nullptr)
	, m_texture(nullptr)
	, m_type(_type)
{
	this->CreateTextureResourceViewSimpleColor(_colorData, _type);
}

TextureComp::TextureComp(const TextureComp& _other)
	: m_TextureResourceView(_other.m_TextureResourceView)
	, m_texture(_other.m_texture)
	, m_type(_other.m_type)
{
	m_TextureResourceView->AddRef();
	m_texture->AddRef();
}

TextureComp::~TextureComp()
{
	if (m_TextureResourceView)m_TextureResourceView->Release();
	if (m_texture) m_texture->Release();
}
