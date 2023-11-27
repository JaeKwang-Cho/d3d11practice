#pragma once

// https://github.com/Microsoft/DirectXTex/wiki/DirectXTex
// https://learn.microsoft.com/en-us/windows/win32/wic/-wic-about-windows-imaging-codec

struct TextureComp {

private:
	ID3D11ShaderResourceView* m_TextureResourceView;
	ID3D11SamplerState* m_TextureSampler;

private:
	HRESULT CreateTextureResourceView(LPCWSTR _TextureName);
	HRESULT CreateDefaultTextureSampler();

private:
	HRESULT CreateTextureResourceViewFromdds(LPCWSTR _TextureName);

public:
	TextureComp();
	virtual ~TextureComp();

	friend struct ObjectRenderComp;
};