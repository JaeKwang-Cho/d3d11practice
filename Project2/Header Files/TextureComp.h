#pragma once
#include <assimp/material.h>

// https://github.com/Microsoft/DirectXTex/wiki/DirectXTex
// https://learn.microsoft.com/en-us/windows/win32/wic/-wic-about-windows-imaging-codec

struct ColorComp {

public:
	union {
		char rgba[4];
		struct {
			char r;
			char g;
			char b;
			char a;
		};
		int color;
	};
public:
	ColorComp()
		:color(0)
	{}
	ColorComp(char _r, char _g, char _b)
		:r(_r), g(_g), b(_b)
	{}
	ColorComp(char _r, char _g, char _b, char _a)
		:r(_r), g(_g), b(_b), a(_a)
	{ }
	ColorComp(const ColorComp& _other)
		:color(_other.color)
	{ }

	ColorComp& operator =(const ColorComp& _other)
	{
		color = _other.color;
		return *this;
	}

	bool operator==(const ColorComp& _other) const
	{
		return (color == _other.color);
	}

	bool operator!=(const ColorComp& _other) const
	{
		return !(color == _other.color);
	}
};

namespace DefaultColors
{
	const ColorComp UnloadedTextureColor(100, 100, 100);
	const ColorComp UnhandledTextureColor(255, 0, 0);
}

enum class TextureStorageType {
	Invalid,
	None,
	EmbeddedIndexCompressed,
	EmbeddedIndexNonCompressed,
	EmbeddedCompressed,
	EmbeddedNonCompressed,
	Disk
};

struct TextureComp {

private:
	ID3D11ShaderResourceView* m_TextureResourceView;
	ID3D11SamplerState* m_TextureSampler;
	ID3D11Resource* m_texture;
	aiTextureType m_type;

private:
	HRESULT CreateTextureResourceViewFromImage(LPCWSTR _TextureName);
	HRESULT CreateTextureResourceViewFromColor(const ColorComp* _colorData, UINT _width, UINT _height, aiTextureType _type);
	HRESULT CreateDefaultTextureSampler();

private:
	HRESULT CreateTextureResourceViewFromdds(LPCWSTR _TextureName);

public:
	TextureComp();
	virtual ~TextureComp();

	friend struct ObjectRenderComp;
};