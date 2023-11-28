#pragma once
#include <assimp/material.h>

// https://github.com/Microsoft/DirectXTex/wiki/DirectXTex
// https://learn.microsoft.com/en-us/windows/win32/wic/-wic-about-windows-imaging-codec

struct ColorComp {

public:
	union {
		unsigned char rgba[4];
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		};
		int color;
	};
public:
	ColorComp()
		:color(0)
	{}
	ColorComp(unsigned char _r, unsigned char _g, unsigned char _b)
		:r(_r), g(_g), b(_b), a(255)
	{}
	ColorComp(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
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
	const ColorComp UnhandledTextureColor(250, 0, 0);
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
	ID3D11Resource* m_texture;
	aiTextureType m_type;

public:
	HRESULT CreateTextureResourceViewFromImage(LPCWSTR _TextureName);
	HRESULT CreateTextureResourceViewFromColor(const ColorComp* _colorData, UINT _width, UINT _height, aiTextureType _type);
	HRESULT CreateTextureResourceViewSimpleColor(const ColorComp& _colorData, aiTextureType _type);

private:
	HRESULT CreateTextureResourceViewFromdds(LPCWSTR _TextureName);

public:
	aiTextureType GetTextureType()
	{
		return m_type;
	}

	ID3D11ShaderResourceView** GetTextureResourceViewAddress()
	{
		return &m_TextureResourceView;
	}

public:
	TextureComp();
	virtual ~TextureComp();
};