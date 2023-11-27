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

void ObjectRenderComp::StartRender()
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

void ObjectRenderComp::Render(Matrix _WorldMat)
{
    // �� ������ ���� ���ϴ� constant buffer ������Ʈ �ϱ�
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(_WorldMat);
    // ���̴����� ����� Constant buffer ��ü�� �ý��� �޸� ���� ī�����ش�.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // ���� GPU�� �Ͽ��� ���̴��� ����, �������� ��ȣ�� Contant buffer�� ���� ���� �ϴ��� �˷��ְ�
    // ����� ������ �ε����� ���� �ﰢ���� �׸����� ��Ų��.
    {
        {
            g_pImmediateContext->VSSetShader(GetVertexShader(), NULL, 0);
            g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBNeverChanges);
            g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangeOnResize);
            g_pImmediateContext->VSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
        }
        
        // Vectex shader�� constant buffer�� ������ �߾, Pixel Shader���� ���� ������ �ؾ� �Ѵ�.
        {
            g_pImmediateContext->PSSetShader(GetPixelShader(), NULL, 0);
            g_pImmediateContext->PSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
            g_pImmediateContext->PSSetShaderResources(0, 1, GetResourceView());
            // �׸��� sampler state�� ������ ���ش�.
            g_pImmediateContext->PSSetSamplers(0, 1, GetSampler());
        }
        
        // ���� �׸���.
        g_pImmediateContext->DrawIndexed(GetNumOfIndices(), 0, 0);
    }
}
