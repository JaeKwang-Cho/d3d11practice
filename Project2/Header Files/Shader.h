#pragma once

struct VertexShader {

private:
	ID3DBlob* m_pVSBlob;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11InputLayout* m_pLayout;

private:
	HRESULT CreateVertexShader(LPCWSTR _ShaderName, 
							   LPCSTR _EntryPoint, 
							   LPCSTR _ShaderModel);
	HRESULT SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* _layoutDesc, 
						   UINT _numElements);

	bool IsValid()
	{
		return m_pVSBlob != nullptr && m_pVertexShader != nullptr && m_pLayout != nullptr;
	}

public:
	VertexShader();
	virtual ~VertexShader();

	friend struct ObjectRenderComp;
};

struct PixelShader {

private:
	ID3DBlob* m_pPSBlob;
	ID3D11PixelShader* m_pPixelShader;

private:
	HRESULT CreatePixelShader(LPCWSTR _ShaderName,
							  LPCSTR _EntryPoint,
							  LPCSTR _ShaderModel);

	bool IsValid()
	{
		return m_pPSBlob != nullptr && m_pPixelShader != nullptr;
	}

public:
	PixelShader();
	virtual ~PixelShader();

	friend struct ObjectRenderComp;
};
