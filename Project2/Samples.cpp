#include "pch.h"
#include "Samples.h"

// Shaders
ID3D11VertexShader* g_pVertexShader = nullptr;
ID3D11PixelShader* g_pPixelShader = nullptr;
ID3D11PixelShader* g_pPixelShaderSolid = nullptr;
// Vertex and Indices
ID3D11InputLayout* g_pVertexLayout = nullptr;
ID3D11Buffer* g_pVertexBuffer = nullptr;
ID3D11Buffer* g_pIndexBuffer = nullptr;
// Constant Buffer
ID3D11Buffer* g_pConstantBuffer = nullptr;
ID3D11Buffer* g_pCBNeverChanges = nullptr;
ID3D11Buffer* g_pCBChangeOnResize = nullptr;
ID3D11Buffer* g_pCBChangesEveryFrame = nullptr;
// Texture
ID3D11ShaderResourceView* g_pTextureRV = nullptr;
ID3D11SamplerState* g_pSamplerLinear = nullptr;


// Transform Matrices
Matrix                      g_WorldMat;
Matrix                      g_WorldMat2;
Matrix                      g_ViewMat;
Matrix                      g_ProjectionMat;
FLOAT4                      g_vMeshColor(0.7f, 0.7f, 0.7f, 1.0f);

void RenderCube()
{
#if 0
    // ���� Ÿ�� �並 ����̽� ���ؽ�Ʈ���� Ư�� ������ �� ����ϰ� ���ش�.
    float ClearColor[4] = { 0.f, 0.125f, 0.3f, 1.f };
    g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    g_pd3dDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pd3dDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pd3dDeviceContext->Draw(3, 0);

    // �� ���۸� ����Ʈ ���۷� �����ش�.
    g_pSwapChain->Present(0, 0);
#endif
    // �ð��� ��� �׾��ְ�
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)PI * 0.0125f;
    }
    else
    {
        static ULONGLONG dwTimeStart = 0;
        ULONGLONG dwTimeCur = GetTickCount64();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // ù��° ť��� �׳� ������ �ð��� ���缭 Y������ ������ �����ְ�
    g_WorldMat = MatrixRotationY(t);

    // �ι�° ť��� �˵��� ���� ������ �ϸ鼭, ���ÿ� ������ �����ش�.
    Matrix mSpin = MatrixRotationZ(-t);
    Matrix mOrbit = MatrixRotationY(-t * 2.0f);
    Matrix mTranslate = MatrixTranslation(-4.f, 0.f, 0.f);
    Matrix mScale = MatrixScale(0.3f, 0.3f, 0.3f);

    // ��� �� ������ ������ -> ���� -> ���� (�̵� + ȸ��) ������ ���ϸ� �ȴ�.
    g_WorldMat2 = mScale * mSpin * mTranslate * mOrbit;


    // �� ���۸� ����ϰ� ġ���ְ�
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // ���� ���۵� 1.0���� ����ϰ� �����ش�.
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    // ================
    // == ù��° ť�� ==
    // ================

    // ���̴��� �Ѱ��� ���� �ʱ�ȭ�ϰ�, 
    ConstantBuffer cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.mView = MatrixTranspose(g_ViewMat);
    cb.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant ���۸� ���� ���ҽ� �޸��� g_pConstantBuffer���ٰ� �װ��� �Ѱ��ش�. (ù��° ť��)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // �ﰢ���� �׸���.
    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    // ������ ������۸� �����Ѵ�. (���� ��ȣ, ����, ������)
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

    // ================
    // == �ι�° ť�� ==
    // ================

    // ���̴��� �Ѱ��� ���� �ʱ�ȭ�ϰ�, 
    ConstantBuffer cb2;
    cb2.mWorld = MatrixTranspose(g_WorldMat2);
    cb2.mView = MatrixTranspose(g_ViewMat);
    cb2.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant ���۸� ���� ���ҽ� �޸��� g_pConstantBuffer���ٰ� �װ��� �Ѱ��ش�. (�ι�° ť��)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb2, 0, 0);

    // �Ȱ��� ���ؽ� ���۸� ����Ѵ�.
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // �� �׸��Ÿ� ȭ�鿡 �Ѹ���.
    g_pSwapChain->Present(0, 0);
}

void RenderCube_Norm()
{
    // �ð� ������Ʈ
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)PI * 0.0125f;
    }
    else
    {
        static ULONGLONG dwTimeStart = 0;
        ULONGLONG dwTimeCur = GetTickCount64();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // ���ۺ��� ���� ť��
    g_WorldMat = MatrixRotationY(t);

    // lighting parameter ����
    FLOAT4 vLightDirections[2] = {
        FLOAT4(-0.577f, 0.577f, -0.577f, 1.0f), //  ���������� �ڿ��� ���ִ� ģ��
        FLOAT4(0.f, 0.f, -1.f, 1.0f), // ������ ���ۺ��� ���鼭 ���ִ� ģ��
    };

    FLOAT4 vLightColors[2] = {
        FLOAT4(0.5f, 0.5f, 0.5f,1.0f), // ���� ȸ��
        FLOAT4(0.8f,0.2f,0.f,1.f), // ��Ȳ��
    };

    // ���� ������ �����ϰ� �Ѵ�.
    Matrix mRotate = MatrixRotationY(-2.f * t);
    Vector4 vLightDir(vLightDirections[1]);
    vLightDir = VectorTransform(vLightDir, mRotate);
    vLightDirections[1] = vLightDir.GetFloat4();

    // �� ���� �����
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // ���� ���� ����� (1.0�� �ִ��)
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // ��Ʈ������ ����Ʈ ������ ������Ʈ ���ش�.
    ConstantBuffer_Norm cb1;
    cb1.mWorld = MatrixTranspose(g_WorldMat);
    cb1.mView = MatrixTranspose(g_ViewMat);
    cb1.mProjection = MatrixTranspose(g_ProjectionMat);
    cb1.vLightDir[0] = vLightDirections[0].Nomalize();
    cb1.vLightDir[1] = vLightDirections[1].Nomalize();
    cb1.vLightColor[0] = vLightColors[0];
    cb1.vLightColor[1] = vLightColors[1];
    cb1.vOutputColor = FLOAT4(0, 0, 0, 0);
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

    // ť�� �׸���
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pImmediateContext->PSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    for (int m = 0; m < 2; m++)
    {
        Vector4 vLightDirVec = Vector4(vLightDirections[m]) * 5.0f;
        Matrix mLight = MatrixTranslation(vLightDirVec.GetFloat4());
        Matrix mLightScale = MatrixScale(0.2f, 0.2f, 0.2f);
        mLight = mLightScale * mLight;

        // Update the world variable to reflect the current light
        cb1.mWorld = MatrixTranspose(mLight);
        cb1.vOutputColor = vLightColors[m];
        g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, NULL, &cb1, 0, 0);

        g_pImmediateContext->PSSetShader(g_pPixelShaderSolid, NULL, 0);
        g_pImmediateContext->DrawIndexed(36, 0, 0);
    }

    // �� ���� �׸���
    g_pSwapChain->Present(0, 0);
}

void RenderCube_Tex()
{
    // Update our time
    static float t = 0.0f;
    if (g_driverType == D3D_DRIVER_TYPE_REFERENCE)
    {
        t += (float)PI * 0.0125f;
    }
    else
    {
        static ULONGLONG dwTimeStart = 0;
        ULONGLONG dwTimeCur = GetTickCount64();
        if (dwTimeStart == 0)
            dwTimeStart = dwTimeCur;
        t = (dwTimeCur - dwTimeStart) / 1000.0f;
    }

    // ���� ��Ű��
    g_WorldMat = MatrixRotationY(t);

    // �� ��ȯ
    g_vMeshColor.x = (sinf(t * 1.f) + 1.f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.f) + 1.f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.f) + 1.f) * 0.5f;

    // �� ���� �����
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // ���� ���� �����
    // ���� ���� �����
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // �� ������ ���� ���ϴ� constant buffer ������Ʈ �ϱ�
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.vMeshColor = g_vMeshColor;
    // ���̴����� ����� Constant buffer ��ü�� �ý��� �޸� ���� ī�����ش�.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // ���� GPU�� �Ͽ��� ���̴��� ����, �������� ��ȣ�� Contant buffer�� ���� ���� �ϴ��� �˷��ְ�
    // ����� ������ �ε����� ���� �ﰢ���� �׸����� ��Ų��.
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
    g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
    g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    // Vectex shader�� constant buffer�� ������ �߾, Pixel Shader���� ���� ������ �ؾ� �Ѵ�.
    g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    // �׸��� sampler state�� ������ ���ش�.
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    // ���� �׸���.
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // ���۸� ȭ�鿡 ���.
    g_pSwapChain->Present(0, 0);
}

bool SetTriangle()
{
    HRESULT hr = S_OK;

    // ���ؽ� ���̴� ������
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob);
    if (!Result)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // ���ؽ� ������ �����
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer ����
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = ARRAYSIZE(layout);

    //  input layout �����
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();

    if (FAILED(hr))
        return false;

    // input layout �����ϱ�
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // �ȼ� ���̴� ������
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob);
    if (!Result)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // ���ؽ� ������ �����
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    if (FAILED(hr))
    {
        return false;
    }

    // ���ؽ� ���� �����
    SimpleVertex vertices[] =
    {
        {FLOAT3(0.f, 0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(-0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)}
    };

    // ���ؽ� ���� ��ũ���̺긦 �����Ѵ�.
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    // ���ؽ� ������ �����͸� �����Ѵ�.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    // ����̽��� ���۸� �ʱ�ȭ�ϸ鼭 ������ش�.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // ���ؽ� ���۸� �����Ѵ�.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // ���ؽ� ���۸� Input - Assembly ���·� �����.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // primitive topology�� �����ϰ� ����Ʈ�� �����Ѵ�.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool SetCube()
{
    HRESULT hr = S_OK;
    // ���ؽ� ���̴� ������
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial05.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // ���ؽ� ������ �����
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer ����
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic �ڿ� �ִ� ���ڴ� �Ȱ��� Sementic Name�� ���� ������, �ε����� �ִ� ģ���̴�.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // ���� COLOR �κп� 12�� ���ĸ�, ���Ŀ� �־��� Vertex �����Ϳ�
    // ��->��->��->�� �̷��� �����Ͱ� ���µ�,
    // ���� float 3���ϱ� 12����Ʈ�� �������� COLOR�� ���´ٴ� ���̴�.
    UINT numElements = ARRAYSIZE(layout);

    //  input layout �����
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout �����ϱ�
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // �ȼ� ���̴� ������
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial05.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // �ȼ� ���̴� �����
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // ���ؽ� ���� �����
    SimpleVertex vertices[] =
    {
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(0.0f, 1.0f, 1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 0.0f, 1.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(0.0f, 0.0f, 0.0f, 1.0f) },
    };

    // ���ؽ� ���� ��ũ���̺긦 �����Ѵ�.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // ���ؽ� ������ �����͸� �����Ѵ�.
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    // ���Ⱑ �����͸� �ִ� ���̴�.
    InitData.pSysMem = vertices;


    // ����̽��� ���۸� �ʱ�ȭ�ϸ鼭 ������ش�.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // ���ؽ� ���۸� �����Ѵ�.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // ���ؽ� ���۸� Input - Assembly ���·� �����.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex�� ������ ���� ���� Index ���۸� �����.
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        0,5,4,
        1,5,0,

        3,4,7,
        0,4,3,

        1,6,5,
        2,6,1,

        2,7,6,
        3,7,2,

        6,4,5,
        7,4,6,
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;        // 12���� �ﰢ��
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // �ε��� ���� �����
    // ��ȭ�� ������ ������ ������ Render���� ���ش�.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology�� �����ϰ� ����Ʈ�� �����Ѵ�.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ���̴��� ���� ��� ���� �����
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // ���� ��Ʈ���� �ʱ�ȭ
    g_WorldMat = MatrixIdentity();

    // ī�޶� ��Ʈ���� �ʱ�ȭ
    Vector4 Eye = Vector4(0.0f, 1.0f, -5.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective�� �������� ��Ʈ���� �ʱ�ȭ
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, (float)width / (float)height, 0.01f, 100.0f);

    return true;
}

bool SetCubeWithNdotL()
{
    HRESULT hr = S_OK;
    // ���ؽ� ���̴� ������
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial06.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // ���ؽ� ������ �����
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer ����
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic �ڿ� �ִ� ���ڴ� �Ȱ��� Sementic Name�� ���� ������, �ε����� �ִ� ģ���̴�.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    //  input layout �����
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout �����ϱ�
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // �ȼ� ���̴� ������ #1 (���� �ݻ��ϴ� ģ��)
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // �ȼ� ���̴� �����
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // �ȼ� ���̴� ������ #1 (���� �ݻ����� �ʴ� ģ��)
    pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PSSolid", "ps_4_0", &pPSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // �ȼ� ���̴� �����
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderSolid);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // ���ؽ� ���� �����
    SimpleVertex_Norm vertices[] =
    {
         { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f) },

        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT3(0.0f, 0.0f, -1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) }, // �� ���� ���� �̻��ϴ�.
    };

    // ���ؽ� ���� ��ũ���̺긦 �����Ѵ�.
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Norm) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // ���ؽ� ������ �����͸� �����Ѵ�.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));

    // ���Ⱑ �����͸� �ִ� ���̴�.
    InitData.pSysMem = vertices;


    // ����̽��� ���۸� �ʱ�ȭ�ϸ鼭 ������ش�.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // ���ؽ� ���۸� �����Ѵ�.
    UINT stride = sizeof(SimpleVertex_Norm);
    UINT offset = 0;

    // ���ؽ� ���۸� Input - Assembly ���·� �����.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex�� ������ ���� ���� Index ���۸� �����.
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;        // ���� ���� ���� ������ �� ��鿡 �ϰ����� Normal�� �ֱ� �����̴�.
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // �ε��� ���� �����
    // ��ȭ�� ������ ������ ������ Render���� ���ش�.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology�� �����ϰ� ����Ʈ�� �����Ѵ�.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ���̴��� ���� ��� ���� �����
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer_Norm);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // ���� ��Ʈ���� �ʱ�ȭ
    g_WorldMat = MatrixIdentity();

    // ī�޶� ��Ʈ���� �ʱ�ȭ
    Vector4 Eye = Vector4(0.0f, 4.0f, -10.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective�� �������� ��Ʈ���� �ʱ�ȭ
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, (float)width / (float)height, 0.01f, 100.0f);

    return true;
}

bool SetCubeWithTex()
{
    HRESULT hr = S_OK;

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Shader�� ������ �ؼ� Binary Large Object file�� �����
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // ���ؽ� ���̴� ��ü �����
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // input layout �����ֱ�
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = ARRAYSIZE(layout);

    // input layout �����
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // input layer �������ֱ�
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // �ȼ� ���̴� ������ ���ֱ�
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // �ȼ� ���̴� �����
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // ���ؽ� ���� �����
    SimpleVertex_Tex vertices[] =
    {
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT2(0.0f, 1.0f) },

        { FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
        { FLOAT3(1.0f, -1.0f, 1.0f), FLOAT2(1.0f, 0.0f) },
        { FLOAT3(1.0f, 1.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT2(0.0f, 1.0f) },
    };

    // ���ؽ� ���� �Ӽ� �����ֱ�
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Tex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // ���ؽ� ������ �����͸� �������ش�.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // ���ؽ� ���� ��ü ���� (�ٲ� ���� ���� ������ �̷��� �Ѵ�.)
    UINT stride = sizeof(SimpleVertex_Tex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // �ε��� ���� �����
    WORD indices[] =
    {
        3,1,0,
        2,1,3,

        6,4,5,
        7,4,6,

        11,9,8,
        10,9,11,

        14,12,13,
        15,12,14,

        19,17,16,
        18,17,19,

        22,20,21,
        23,20,22
    };

    // �ε��� ���� �Ӽ� �����ֱ�
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // �ε��� ������ ������ �������ֱ�
    InitData.pSysMem = indices;

    // �ε��� ���� ������ֱ�
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // �ε��� ���� ���� (�ε��� ������ ��û �������� DXGI_FORMAT_R32_UINT�� �ٲ۴�.)
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // triangle list�� �׸���
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Constant ���� �����
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    //  #1 
    bd.ByteWidth = sizeof(CBChangeOnInput);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBNeverChanges);
    if (FAILED(hr))
    {
        return false;
    }
    // #2
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangeOnResize);
    if (FAILED(hr))
    {
        return false;
    }
    // #3
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
    if (FAILED(hr))
    {
        return false;
    }

    // Load the Texture
    // DirectTex�� ���߿� �ٲٵ��� ����.

    ScratchImage scratchImage = ScratchImage{};
    hr = LoadFromDDSFile(L"seafloor.dds", DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
    //hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"seafloor.dds", nullptr, nullptr, &g_pTextureRV, nullptr);
    if (FAILED(hr))
    {
        return false;
    }

    hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &g_pTextureRV);

    // �ؽ����� Sampler state �����
    D3D11_SAMPLER_DESC sampDesc;
    memset(&sampDesc, 0, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // �����ؼ� �� �� �ִ�. ������ ���� �����ϴµ� ���δ�. 
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap�� ���� �Ѿ�ٸ� 0 ~ 1 , 1 ~ 0 ���� �ݺ��� �Ǵ� ���̰�
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror�� 0 ~ 1 ~ 0���� �ݺ��� �ȴ�.
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp�� 0 ~ 1�� �����, ������ �ȼ��� ä������.
    // Border�� �Ѿ�� Border Color�� �������� ������ ä������.
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // ���ø��� �����͸� ���� ���ø��� �����Ϳ� ���ϴ� �Լ�
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr))
    {
        return false;
    }

    // ���� ��Ʈ���� �ʱ�ȭ
    g_WorldMat = MatrixIdentity();

    // �� ��Ʈ���� �ʱ�ȭ
    Vector4 Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    // �������� ���� constant buffer �ʱ�ȭ
    CBChangeOnInput cbNeverChanges;
    cbNeverChanges.mView = MatrixTranspose(g_ViewMat);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

    // �������� ��Ʈ���� �ʱ�ȭ
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, width / (float)height, 0.01f, 100.f);

    // ������ ũ�� ��ȯ �ÿ� ���ϴ� Constant buffer �ʱ�ȭ
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);

    return true;
}