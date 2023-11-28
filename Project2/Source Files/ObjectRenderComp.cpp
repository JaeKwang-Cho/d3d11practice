#include "pch.h"
#include "ObjectRenderComp.h"

void ObjectRenderComp::StartRender()
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

