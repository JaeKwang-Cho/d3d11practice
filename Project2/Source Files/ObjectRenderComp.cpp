#include "pch.h"
#include "ObjectRenderComp.h"

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

