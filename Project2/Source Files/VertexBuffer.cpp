#include "pch.h"
#include "VertexBuffer.h"

VertexBuffer::VertexBuffer()
	:m_pVertexBuffer(nullptr)
	, m_stride(0)
	, m_offset(0)
{
}

VertexBuffer::~VertexBuffer()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
}
