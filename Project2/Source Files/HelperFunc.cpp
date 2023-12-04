#include "pch.h"
#include "HelperFunc.h"
#include "TextureComp.h"

wstring StringHelper::StrToWstr(string _str)
{
	wstring wstr(_str.begin(), _str.end());
	return wstr;
}

wstring StringHelper::GetDirectoryFromPath(const wstring& _filePath)
{
	size_t off = _filePath.find_last_of(L'\\');
	if (off == wstring::npos)
	{
		return wstring();
	}
	return _filePath.substr(0, off);
}

string StringHelper::GetDirectoryFromPath(const string& _filePath)
{
	size_t off = _filePath.find_last_of(L'\\');
	if (off == string::npos)
	{
		return string();
	}
	return _filePath.substr(0, off);
}

wstring StringHelper::GetFileExtension(const wstring& _fileName)
{
	size_t off = _fileName.find_last_of(L'.');
	if (off == wstring::npos)
	{
		return wstring();
	}
	return wstring(_fileName.substr(off));
}

HRESULT TexResource::CreateTextureResourceViewFromdds(ID3D11ShaderResourceView** _ppTextureResourceView, LPCWSTR _TextureName)
{
	ScratchImage scratchImage = ScratchImage{};

	HRESULT hr = LoadFromDDSFile(_TextureName, DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), _ppTextureResourceView);
	(*_ppTextureResourceView)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CreateTextureResourceViewFromdds") - 1, "CreateTextureResourceViewFromdds");
	return hr;
}

HRESULT TexResource::CreateTextureResourceViewFromImage(ID3D11ShaderResourceView** _ppTextureResourceView, wstring _TexturePath)
{
	HRESULT hr = E_NOTIMPL;

	wstring fileExtension = StringHelper::GetFileExtension(_TexturePath);
	if (fileExtension == L".dds")
	{
		ScratchImage scratchImage = ScratchImage{};

		hr = LoadFromDDSFile(_TexturePath.c_str(), DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);

		if (FAILED(hr))
		{
			return hr;
		}

		hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), _ppTextureResourceView);

		(*_ppTextureResourceView)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CreateTextureResourceViewFromImage") - 1, "CreateTextureResourceViewFromImage");
	}
	else if (fileExtension == L".tga")
	{

	}
	else if (fileExtension == L".hdr")
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

		hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), _ppTextureResourceView);

		(*_ppTextureResourceView)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CreateTextureResourceViewFromImage") - 1, "CreateTextureResourceViewFromImage");
	}

	return hr;
}

HRESULT TexResource::CreateTextureResourceViewFromColor(ID3D11ShaderResourceView** _ppTextureResourceView, const ColorComp* _colorData, UINT _width, UINT _height)
{
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

	ID3D11Resource* ptexture;

	// ������ ����� Texture2D�� �����.
	HRESULT hr = g_pd3dDevice->CreateTexture2D(&td, &initData, (ID3D11Texture2D**)&ptexture);

	// �׸��� �װ��� ���̴��� �� �� �ְ�, ���ҽ� ��� �����.
	CD3D11_SHADER_RESOURCE_VIEW_DESC srvd;
	// �̷��� ���� ���� �����ڸ� ���� �� �ξ� ���ϴ� #2
	memset(&srvd, 0, sizeof(srvd));
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Format = td.Format;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;


	if (ptexture)
	{
		hr = g_pd3dDevice->CreateShaderResourceView(ptexture, &srvd, _ppTextureResourceView);
	}

	(*_ppTextureResourceView)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CreateTextureResourceViewFromColor") - 1, "CreateTextureResourceViewFromColor");

	return hr;
}

HRESULT TexResource::CreateTextureResourceViewSingleColor(ID3D11ShaderResourceView** _ppTextureResourceView, const ColorComp& _colorData)
{
	return CreateTextureResourceViewFromColor(_ppTextureResourceView, &_colorData, 1, 1);
}

HRESULT TexResource::CreateTextureResourceViewFromData(ID3D11ShaderResourceView** _ppTextureResourceView, const uint8_t* _pData, size_t _width)
{
	TexMetadata texMetadata = TexMetadata{};
	GetMetadataFromWICMemory(_pData, _width, DirectX::WIC_FLAGS_NONE, texMetadata);

	ScratchImage scratchImage = ScratchImage{};
	LoadFromWICMemory(_pData, _width, DirectX::WIC_FLAGS_NONE, &texMetadata, scratchImage);


	HRESULT hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), _ppTextureResourceView);
	if (FAILED(hr))
	{
		assert("TexResource::CreateTextureResourceViewFromData Failed" && false);
		return hr;
	}
	(*_ppTextureResourceView)->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("CreateTextureResourceViewFromData") - 1, "CreateTextureResourceViewFromData");

	return hr;
}

