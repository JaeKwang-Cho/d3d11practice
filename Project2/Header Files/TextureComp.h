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

public:
	ID3D11ShaderResourceView* m_pTextureResourceView;
	ID3D11Resource* m_texture;
	aiTextureType m_type;
	wstring m_filename;

public:
	TextureComp()
		: m_pTextureResourceView(nullptr)
		, m_texture(nullptr)
		, m_type(aiTextureType::aiTextureType_UNKNOWN)
	{}

	TextureComp(const TextureComp& _other)
		: m_pTextureResourceView(_other.m_pTextureResourceView)
		, m_texture(_other.m_texture)
		, m_type(_other.m_type)
		, m_filename(_other.m_filename)
	{
		if (m_pTextureResourceView)m_pTextureResourceView->AddRef();
		if (m_texture) m_texture->AddRef();
	}

	virtual ~TextureComp()
	{
		if (m_pTextureResourceView)m_pTextureResourceView->Release();
		if (m_texture) m_texture->Release();
	}
};

