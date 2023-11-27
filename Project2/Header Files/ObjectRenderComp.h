#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "TextureComp.h"
#include "VertexBuffer.h"

struct ObjectRenderComp {
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
	TextureComp m_TextureComp;

public:

	HRESULT CreateVertexShader(LPCWSTR _ShaderName,
							   LPCSTR _EntryPoint,
							   LPCSTR _ShaderModel);

	HRESULT SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* _layoutDesc,
						   UINT _numElements);

	bool IsVertexShaderValid();

	HRESULT CreatePixelShader(LPCWSTR _ShaderName,
							  LPCSTR _EntryPoint,
							  LPCSTR _ShaderModel);

	bool IsPixelShaderValid();

	template <typename T>
	HRESULT CreateVertexBuffer(T* _VertexData, UINT _numVertices)
	{
		return m_VertexBuffer.CreateVertexBuffer(_VertexData, _numVertices);
	}

	HRESULT CreateIndexBuffer(WORD* _IndiceData, UINT _numIndices);

	HRESULT CreateTextureResourceView(LPCWSTR _TextureName);

	HRESULT CreateDefaultTextureSampler();

public:
	void Update();

	ID3D11VertexShader* GetVertexShader()
	{
		return m_VertexShader.m_pVertexShader;
	}

	ID3D11PixelShader* GetPixelShader()
	{
		return m_PixelShader.m_pPixelShader;
	}

	ID3D11SamplerState** GetSampler()
	{
		return &(m_TextureComp.m_TextureSampler);
	}

	ID3D11ShaderResourceView** GetResourceView()
	{
		return &(m_TextureComp.m_TextureResourceView);
	}

	UINT GetNumOfIndices()
	{
		return m_IndexBuffer.m_NumOfIndices;
	}

public:

	ObjectRenderComp()
	{}

	virtual ~ObjectRenderComp()
	{}
};