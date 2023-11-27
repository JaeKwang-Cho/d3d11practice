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
	// ���̾ƿ� ���� �ֱ�
	g_pImmediateContext->IASetInputLayout(m_VertexShader.m_pLayout);

	// ���ؽ� ���� ��ü ���� (�ٲ� ���� ���� ������ �̷��� �Ѵ�.)
	UINT stride = m_VertexBuffer.m_stride;
	UINT offset = m_VertexBuffer.m_offset;
	g_pImmediateContext->IASetVertexBuffers(0, 1, &m_VertexBuffer.m_pVertexBuffer, &stride, &offset);

	// Render Asset���� ����
	// �ε��� ���� ���� (�ε��� ������ ��û �������� DXGI_FORMAT_R32_UINT�� �ٲ۴�.)
	g_pImmediateContext->IASetIndexBuffer(m_IndexBuffer.m_IndexBuffer, DXGI_FORMAT_R16_UINT, 0);

	// triangle list�� �׸���
	g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}