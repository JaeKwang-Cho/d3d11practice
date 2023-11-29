#pragma once

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexBuffer.h"

struct ObjectRenderComp {
	VertexShader m_VertexShader;
	PixelShader m_PixelShader;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;

public:
	void Render();

public:
	HRESULT CreateVertexShader(LPCWSTR _ShaderName,
							   LPCSTR _EntryPoint,
							   LPCSTR _ShaderModel)
	{
		return m_VertexShader.CreateVertexShader(_ShaderName, _EntryPoint, _ShaderModel);
	}

	HRESULT SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* _layoutDesc,
						   UINT _numElements)
	{
		return m_VertexShader.SetInputLayout(_layoutDesc, _numElements);
	}

	bool IsVertexShaderValid()
	{
		return m_VertexShader.IsValid();
	}

	HRESULT CreatePixelShader(LPCWSTR _ShaderName,
							  LPCSTR _EntryPoint,
							  LPCSTR _ShaderModel)
	{
		return m_PixelShader.CreatePixelShader(_ShaderName, _EntryPoint, _ShaderModel);
	}

	bool IsPixelShaderValid()
	{
		return m_PixelShader.IsValid();
	}

	template <typename T>
	HRESULT CreateVertexBuffer(T* _VertexData, UINT _numVertices)
	{
		return m_VertexBuffer.CreateVertexBuffer(_VertexData, _numVertices);
	}

	HRESULT CreateIndexBuffer(WORD* _IndiceData, UINT _numIndices)
	{
		return m_IndexBuffer.CreateIndexBuffer(_IndiceData, _numIndices);
	}

public:

	ID3D11VertexShader* GetVertexShader()
	{
		return m_VertexShader.m_pVertexShader;
	}

	ID3D11PixelShader* GetPixelShader()
	{
		return m_PixelShader.m_pPixelShader;
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