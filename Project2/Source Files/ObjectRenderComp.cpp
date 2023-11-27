#include "pch.h"
#include "ObjectRenderComp.h"

HRESULT ObjectRenderComp::CreateVertexShader(LPCWSTR _ShaderName,
						   LPCSTR _EntryPoint,
						   LPCSTR _ShaderModel)
{
	return m_VertexShader.CreateVertexShader(_ShaderName, _EntryPoint, _ShaderModel);
}

HRESULT ObjectRenderComp::SetInputLayout(const D3D11_INPUT_ELEMENT_DESC* _layoutDesc,
					   UINT _numElements)
{
	return m_VertexShader.SetInputLayout(_layoutDesc, _numElements);
}

bool ObjectRenderComp::IsVertexShaderValid()
{
	return m_VertexShader.IsValid();
}

HRESULT ObjectRenderComp::CreatePixelShader(LPCWSTR _ShaderName,
						  LPCSTR _EntryPoint,
						  LPCSTR _ShaderModel)
{
	return m_PixelShader.CreatePixelShader(_ShaderName, _EntryPoint, _ShaderModel);
}

bool ObjectRenderComp::IsPixelShaderValid()
{
	return m_PixelShader.IsValid();
}

HRESULT ObjectRenderComp::CreateIndexBuffer(WORD* _IndiceData, UINT _numIndices)
{
	return m_IndexBuffer.CreateIndexBuffer(_IndiceData, _numIndices);
}

HRESULT ObjectRenderComp::CreateTextureResourceView(LPCWSTR _TextureName)
{
	return m_TextureComp.CreateTextureResourceView(_TextureName);
}

HRESULT ObjectRenderComp::CreateDefaultTextureSampler()
{
	return m_TextureComp.CreateDefaultTextureSampler();
}

void ObjectRenderComp::Update()
{
	// 레이아웃 집어 넣기
	g_pImmediateContext->IASetInputLayout(m_VertexShader.m_pLayout);

	// 버텍스 버퍼 객체 세팅 (바꿀 일이 없이 때문에 이렇게 한다.)
	UINT stride = m_VertexBuffer.m_stride;
	UINT offset = m_VertexBuffer.m_offset;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_VertexBuffer.m_pVertexBuffer, &stride, &offset);

	// Render Asset으로 빼기
	// 인덱스 버퍼 세팅 (인덱스 개수가 엄청 많아지면 DXGI_FORMAT_R32_UINT로 바꾼다.)
	g_pImmediateContext->IASetIndexBuffer(m_IndexBuffer.m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// triangle list로 그리기
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}