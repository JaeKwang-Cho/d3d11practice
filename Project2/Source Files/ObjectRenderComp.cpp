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

void ObjectRenderComp::Render(Matrix _WorldMat)
{
    // 매 프레임 마다 변하는 constant buffer 업데이트 하기
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(_WorldMat);
    // 쉐이더에서 사용할 Constant buffer 객체에 시스템 메모리 값을 카피해준다.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    {
        {
            g_pImmediateContext->VSSetShader(GetVertexShader(), NULL, 0);
            g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBNeverChanges);
            g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangeOnResize);
            g_pImmediateContext->VSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
        }
        
        // Vectex shader에 constant buffer를 세팅을 했어도, Pixel Shader에도 따로 세팅을 해야 한다.
        {
            g_pImmediateContext->PSSetShader(GetPixelShader(), NULL, 0);
            g_pImmediateContext->PSSetConstantBuffers(3, 1, &g_pCBChangesEveryFrame);
            //g_pImmediateContext->PSSetShaderResources(0, 1, GetResourceView());
            // 그리고 sampler state도 설정을 해준다.
            //g_pImmediateContext->PSSetSamplers(0, 1, GetSampler());
        }
        
        // 마저 그린다.
        g_pImmediateContext->DrawIndexed(GetNumOfIndices(), 0, 0);
    }
}
