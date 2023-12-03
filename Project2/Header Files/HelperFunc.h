#pragma once

struct ColorComp;

namespace StringHelper
{
	wstring StrToWstr(string _str);
	wstring GetDirectoryFromPath(const wstring& _filePath);
	string GetDirectoryFromPath(const string& _filePath);
	wstring GetFileExtension(const wstring& _fileName);
}

namespace TexResource
{
	HRESULT CreateTextureResourceViewFromdds(ID3D11ShaderResourceView** _ppTextureResourceView, LPCWSTR _TextureName);

	HRESULT CreateTextureResourceViewFromImage(ID3D11ShaderResourceView** _ppTextureResourceView, wstring _TexturePath);

	HRESULT CreateTextureResourceViewFromColor(ID3D11ShaderResourceView** _ppTextureResourceView, const ColorComp* _colorData, UINT _width, UINT _height);

	HRESULT CreateTextureResourceViewSingleColor(ID3D11ShaderResourceView** _ppTextureResourceView, const ColorComp& _colorData);
}