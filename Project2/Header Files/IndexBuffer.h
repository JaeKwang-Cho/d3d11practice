#pragma once

struct IndexBuffer {

private:
	ID3D11Buffer* m_IndexBuffer;
	UINT m_NumOfIndices;
private:
	HRESULT CreateIndexBuffer(WORD* _IndiceData, UINT _numIndices);
public:
	IndexBuffer();
	virtual ~IndexBuffer();

	friend struct ObjectRenderComp;
};