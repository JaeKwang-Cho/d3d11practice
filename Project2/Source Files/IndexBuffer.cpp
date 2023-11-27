#include "pch.h"
#include "IndexBuffer.h"

HRESULT IndexBuffer::CreateIndexBuffer(WORD* _IndiceData, UINT _numIndices)
{
    m_NumOfIndices = _numIndices;
    // �ε��� ���� �Ӽ� �����ֱ�
    D3D11_BUFFER_DESC ibd;
    memset(&ibd, 0, sizeof(ibd));
    ibd.Usage = D3D11_USAGE_DEFAULT;
    ibd.ByteWidth = sizeof(WORD) * _numIndices;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;

    // �ε��� ������ ������ �������ֱ�
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = _IndiceData;

    // �ε��� ���� ������ֱ�
    HRESULT hr = g_pd3dDevice->CreateBuffer(&ibd, &InitData, &m_IndexBuffer);
    m_IndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("IndexBuffer::CreateIndexBuffer") - 1, "IndexBuffer::CreateIndexBuffer");
    return hr;
}

IndexBuffer::IndexBuffer()
	:m_IndexBuffer(nullptr)
    , m_NumOfIndices(0)
{
}

IndexBuffer::~IndexBuffer()
{
	if (m_IndexBuffer) m_IndexBuffer->Release();
}
