#pragma once

struct VertexShader {

public:
	ID3DBlob* m_pVSBlob;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pLayout;

public:
	VertexShader()
		: m_pVSBlob(nullptr)
		, m_pVertexShader(nullptr)
		, m_pLayout(nullptr)
	{
	}

	virtual ~VertexShader()
	{
		if (m_pVertexShader) m_pVertexShader->Release();
		if (m_pVSBlob) m_pVSBlob->Release();
		if (m_pLayout) m_pLayout->Release();
	}
};

struct PixelShader {

public:
	ID3DBlob* m_pPSBlob;
	ID3D11PixelShader* m_pPixelShader;

public:
	PixelShader()
		: m_pPSBlob(nullptr)
		, m_pPixelShader(nullptr)
	{
	}

	virtual ~PixelShader()
	{
		if (m_pPixelShader) m_pPixelShader->Release();
		if (m_pPSBlob) m_pPSBlob->Release();
	}
};

struct VertexBuffer {

public:
    ID3D11Buffer* m_pVertexBuffer;
    UINT m_stride;
    UINT m_offset;

public:
    VertexBuffer()
        :m_pVertexBuffer(nullptr)
        , m_stride(0)
        , m_offset(0)
    {
    }

    virtual ~VertexBuffer()
    {
        if (m_pVertexBuffer) m_pVertexBuffer->Release();
    }
};

struct IndexBuffer {
public:
	ID3D11Buffer* m_pIndexBuffer;
	UINT m_NumOfIndices;

public:
	IndexBuffer()
		:m_pIndexBuffer(nullptr)
		, m_NumOfIndices(0)
	{
	}

	virtual ~IndexBuffer()
	{
		if (m_pIndexBuffer) m_pIndexBuffer->Release();
	}
};