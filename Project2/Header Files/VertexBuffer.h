#pragma once

struct VertexBuffer {

private:
	ID3D11Buffer* m_pVertexBuffer;
    UINT m_stride;
    UINT m_offset;

private:
	template <typename T>
	HRESULT CreateVertexBuffer(T* _VertexData, UINT _numVertices)
	{
        // 스트라이드 구하고
        m_stride = sizeof(T);

        D3D11_BUFFER_DESC vbd;
        memset(&vbd, 0, sizeof(vbd));
        vbd.Usage = D3D11_USAGE_DEFAULT;
        vbd.ByteWidth = m_stride * _numVertices;
        vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        vbd.CPUAccessFlags = 0;

        // 버텍스 데이터 포인터를 연결해준다.
        D3D11_SUBRESOURCE_DATA InitData;
        memset(&InitData, 0, sizeof(InitData));
        InitData.pSysMem = _VertexData;

        HRESULT hr = g_pd3dDevice->CreateBuffer(&vbd, &InitData, &m_pVertexBuffer);
        m_pVertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("VertexBuffer::CreateVertexBuffer") - 1, "VertexBuffer::CreateVertexBuffer");
        return hr;
	}
public:
	VertexBuffer();
	virtual ~VertexBuffer();

    friend struct ObjectRenderComp;
};