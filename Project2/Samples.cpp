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
    // 렌더 타겟 뷰를 디바이스 컨텍스트에서 특정 색으로 싹 깔끔하게 해준다.
    float ClearColor[4] = { 0.f, 0.125f, 0.3f, 1.f };
    g_pd3dDeviceContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    g_pd3dDeviceContext->VSSetShader(g_pVertexShader, nullptr, 0);
    g_pd3dDeviceContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pd3dDeviceContext->Draw(3, 0);

    // 백 버퍼를 프론트 버퍼로 보여준다.
    g_pSwapChain->Present(0, 0);
#endif
    // 시간을 계속 쌓아주고
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

    // 첫번째 큐브는 그냥 누적된 시간에 맞춰서 Y축으러 자전만 시켜주고
    g_WorldMat = MatrixRotationY(t);

    // 두번째 큐브는 궤도를 따라 공전을 하면서, 동시에 자전도 시켜준다.
    Matrix mSpin = MatrixRotationZ(-t);
    Matrix mOrbit = MatrixRotationY(-t * 2.0f);
    Matrix mTranslate = MatrixTranslation(-4.f, 0.f, 0.f);
    Matrix mScale = MatrixScale(0.3f, 0.3f, 0.3f);

    // 행렬 곱 순서는 스케일 -> 자전 -> 공전 (이동 + 회전) 순서로 곱하면 된다.
    g_WorldMat2 = mScale * mSpin * mTranslate * mOrbit;


    // 백 버퍼를 깔끔하게 치워주고
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red,green,blue,alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼도 1.0으로 깔끔하게 지워준다.
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.f, 0);

    // ================
    // == 첫번째 큐브 ==
    // ================

    // 쉐이더로 넘겨줄 값을 초기화하고, 
    ConstantBuffer cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.mView = MatrixTranspose(g_ViewMat);
    cb.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant 버퍼를 위한 리소스 메모리인 g_pConstantBuffer에다가 그것을 넘겨준다. (첫번째 큐브)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb, 0, 0);

    // 삼각형을 그린다.
    g_pImmediateContext->VSSetShader(g_pVertexShader, nullptr, 0);
    // 설정된 상수버퍼를 세팅한다. (슬롯 번호, 갯수, 포인터)
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
    g_pImmediateContext->PSSetShader(g_pPixelShader, nullptr, 0);
    g_pImmediateContext->DrawIndexed(36, 0, 0);        // 36 vertices needed for 12 triangles in a triangle list

    // ================
    // == 두번째 큐브 ==
    // ================

    // 쉐이더로 넘겨줄 값을 초기화하고, 
    ConstantBuffer cb2;
    cb2.mWorld = MatrixTranspose(g_WorldMat2);
    cb2.mView = MatrixTranspose(g_ViewMat);
    cb2.mProjection = MatrixTranspose(g_ProjectionMat);

    // Constant 버퍼를 위한 리소스 메모리인 g_pConstantBuffer에다가 그것을 넘겨준다. (두번째 큐브)
    g_pImmediateContext->UpdateSubresource(g_pConstantBuffer, 0, nullptr, &cb2, 0, 0);

    // 똑같은 버텍스 버퍼를 사용한다.
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // 다 그린거를 화면에 뿌린다.
    g_pSwapChain->Present(0, 0);
}

void RenderCube_Norm()
{
    // 시간 업데이트
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

    // 빙글빙글 도는 큐브
    g_WorldMat = MatrixRotationY(t);

    // lighting parameter 세팅
    FLOAT4 vLightDirections[2] = {
        FLOAT4(-0.577f, 0.577f, -0.577f, 1.0f), //  고정적으로 뒤에서 쏴주는 친구
        FLOAT4(0.f, 0.f, -1.f, 1.0f), // 주위를 빙글빙글 돌면서 쏴주는 친구
    };

    FLOAT4 vLightColors[2] = {
        FLOAT4(0.5f, 0.5f, 0.5f,1.0f), // 대충 회색
        FLOAT4(0.8f,0.2f,0.f,1.f), // 주황색
    };

    // 빨간 조명을 공전하게 한다.
    Matrix mRotate = MatrixRotationY(-2.f * t);
    Vector4 vLightDir(vLightDirections[1]);
    vLightDir = VectorTransform(vLightDir, mRotate);
    vLightDirections[1] = vLightDir.GetFloat4();

    // 백 버퍼 지우고
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼 지우고 (1.0이 최대로)
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 매트릭스와 라이트 변수를 업데이트 해준다.
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

    // 큐브 그리기
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

    // 백 버퍼 그리기
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

    // 자전 시키기
    g_WorldMat = MatrixRotationY(t);

    // 색 변환
    g_vMeshColor.x = (sinf(t * 1.f) + 1.f) * 0.5f;
    g_vMeshColor.y = (cosf(t * 3.f) + 1.f) * 0.5f;
    g_vMeshColor.z = (sinf(t * 5.f) + 1.f) * 0.5f;

    // 백 버퍼 지우기
    float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; // red, green, blue, alpha
    g_pImmediateContext->ClearRenderTargetView(g_pRenderTargetView, ClearColor);

    // 뎁스 버퍼 지우기
    // 뎁스 버퍼 지우기
    g_pImmediateContext->ClearDepthStencilView(g_pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

    // 매 프레임 마다 변하는 constant buffer 업데이트 하기
    CBChangesEveryFrame cb;
    cb.mWorld = MatrixTranspose(g_WorldMat);
    cb.vMeshColor = g_vMeshColor;
    // 쉐이더에서 사용할 Constant buffer 객체에 시스템 메모리 값을 카피해준다.
    g_pImmediateContext->UpdateSubresource(g_pCBChangesEveryFrame, 0, nullptr, &cb, 0, 0);

    // 이제 GPU로 하여금 셰이더에 적힌, 레지스터 번호로 Contant buffer가 어디로 들어가야 하는지 알려주고
    // 계산을 돌리고 인덱스를 따라 삼각형을 그리도록 시킨다.
    g_pImmediateContext->VSSetShader(g_pVertexShader, NULL, 0);
    g_pImmediateContext->VSSetConstantBuffers(0, 1, &g_pCBNeverChanges);
    g_pImmediateContext->VSSetConstantBuffers(1, 1, &g_pCBChangeOnResize);
    g_pImmediateContext->VSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    // Vectex shader에 constant buffer를 세팅을 했어도, Pixel Shader에도 따로 세팅을 해야 한다.
    g_pImmediateContext->PSSetShader(g_pPixelShader, NULL, 0);
    g_pImmediateContext->PSSetConstantBuffers(2, 1, &g_pCBChangesEveryFrame);
    g_pImmediateContext->PSSetShaderResources(0, 1, &g_pTextureRV);
    // 그리고 sampler state도 설정을 해준다.
    g_pImmediateContext->PSSetSamplers(0, 1, &g_pSamplerLinear);

    // 마저 그린다.
    g_pImmediateContext->DrawIndexed(36, 0, 0);

    // 버퍼를 화면에 쏜다.
    g_pSwapChain->Present(0, 0);
}

bool SetTriangle()
{
    HRESULT hr = S_OK;

    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial02.fx", "VS", "vs_4_0", &pVSBlob);
    if (!Result)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };
    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();

    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial02.fx", "PS", "ps_4_0", &pPSBlob);
    if (!Result)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    if (FAILED(hr))
    {
        return false;
    }

    // 버텍스 버퍼 만들기
    SimpleVertex vertices[] =
    {
        {FLOAT3(0.f, 0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)},
        {FLOAT3(-0.5f, -0.5f, 0.5f), FLOAT4(0.f,0.f,0.f,0.f)}
    };

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 3;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return true;
}

bool SetCube()
{
    HRESULT hr = S_OK;
    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial05.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic 뒤에 있는 숫자는 똑같은 Sementic Name을 쓰고 싶을때, 인덱스로 넣는 친구이다.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };
    // 저기 COLOR 부분에 12가 뭐냐면, 이후에 넣어준 Vertex 데이터에
    // 점->색->점->색 이렇게 데이터가 들어가는데,
    // 점이 float 3개니까 12바이트가 지나가면 COLOR가 나온다는 뜻이다.
    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial05.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼 만들기
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

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex) * 8;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    ZeroMemory(&InitData, sizeof(InitData));
    // 여기가 포인터를 넣는 곳이다.
    InitData.pSysMem = vertices;


    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex);
    UINT offset = 0;
    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex를 여러번 쓰기 위한 Index 버퍼를 만든다.
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
    bd.ByteWidth = sizeof(WORD) * 36;        // 12개의 삼각형
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // 인덱스 버퍼 만들기
    // 변화할 여지가 있으면 원래는 Render에서 해준다.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 셰이더에 들어가는 상수 버퍼 만들기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 카메라 메트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 1.0f, -5.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective로 프로젝션 매트릭스 초기화
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, (float)width / (float)height, 0.01f, 100.0f);

    return true;
}

bool SetCubeWithNdotL()
{
    HRESULT hr = S_OK;
    // 버텍스 쉐이더 컴파일
    ID3DBlob* pVSBlob = NULL;
    bool Result = CompileShaderFromFile(L"Tutorial06.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이터 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // Vertex Input layer 정의
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        // Sementic 뒤에 있는 숫자는 똑같은 Sementic Name을 쓰고 싶을때, 인덱스로 넣는 친구이다.
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    //  input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
        return false;

    // input layout 세팅하기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일 #1 (빛을 반사하는 친구)
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PS", "ps_4_0", &pPSBlob);
    if (FAILED(hr))
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 픽셀 쉐이더 컴파일 #1 (빛을 반사하지 않는 친구)
    pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial06.fx", "PSSolid", "ps_4_0", &pPSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &g_pPixelShaderSolid);
    pPSBlob->Release();
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼 만들기
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
        { FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, 1.0f) }, // 이 점이 뭔가 이상하다.
    };

    // 버텍스 버퍼 디스크라이브를 지정한다.
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Norm) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;


    // 버텍스 버퍼의 데이터를 세팅한다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));

    // 여기가 포인터를 넣는 곳이다.
    InitData.pSysMem = vertices;


    // 디바이스를 버퍼를 초기화하면서 만들어준다.
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
        return false;

    // 버텍스 버퍼를 세팅한다.
    UINT stride = sizeof(SimpleVertex_Norm);
    UINT offset = 0;

    // 버텍스 버퍼를 Input - Assembly 상태로 만든다.
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // Vertex를 여러번 쓰기 위한 Index 버퍼를 만든다.
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
    bd.ByteWidth = sizeof(WORD) * 36;        // 점을 많이 쓰는 이유는 한 평면에 일관적인 Normal을 주기 위함이다.
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    InitData.pSysMem = indices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
        return false;

    // 인덱스 버퍼 만들기
    // 변화할 여지가 있으면 원래는 Render에서 해준다.
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // primitive topology는 간단하게 리스트로 설정한다.
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // 셰이더에 들어가는 상수 버퍼 만들기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(ConstantBuffer_Norm);
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;
    hr = g_pd3dDevice->CreateBuffer(&bd, NULL, &g_pConstantBuffer);
    if (FAILED(hr))
        return false;

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 카메라 메트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 4.0f, -10.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    RECT rc;
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    // Perspective로 프로젝션 매트릭스 초기화
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

    // Shader를 컴파일 해서 Binary Large Object file로 만들기
    ID3DBlob* pVSBlob = nullptr;
    bool Result = CompileShaderFromFile(L"Tutorial07.fx", "VS", "vs_4_0", &pVSBlob);
    if (Result == false)
    {
        MessageBox(nullptr,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 버텍스 쉐이더 객체 만들기
    hr = g_pd3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &g_pVertexShader);
    if (FAILED(hr))
    {
        pVSBlob->Release();
        return false;
    }

    // input layout 정해주기
    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    UINT numElements = ARRAYSIZE(layout);

    // input layout 만들기
    hr = g_pd3dDevice->CreateInputLayout(
        layout, numElements, pVSBlob->GetBufferPointer(),
        pVSBlob->GetBufferSize(), &g_pVertexLayout);

    pVSBlob->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // input layer 세팅해주기
    g_pImmediateContext->IASetInputLayout(g_pVertexLayout);

    // 픽셀 쉐이더 컴파일 해주기
    ID3DBlob* pPSBlob = nullptr;
    Result = CompileShaderFromFile(L"Tutorial07.fx", "PS", "ps_4_0", &pPSBlob);
    if (Result == false)
    {
        MessageBox(NULL,
                   L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
        return false;
    }

    // 픽셀 쉐이더 만들기
    hr = g_pd3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &g_pPixelShader);
    pPSBlob->Release();
    if (FAILED(hr))
    {
        return false;
    }

    // 버텍스 버퍼 만들기
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

    // 버텍스 버퍼 속성 정해주기
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(SimpleVertex_Tex) * 24;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // 버텍스 데이터 포인터를 연결해준다.
    D3D11_SUBRESOURCE_DATA InitData;
    memset(&InitData, 0, sizeof(InitData));
    InitData.pSysMem = vertices;
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pVertexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // 버텍스 버퍼 객체 세팅 (바꿀 일이 없이 때문에 이렇게 한다.)
    UINT stride = sizeof(SimpleVertex_Tex);
    UINT offset = 0;
    g_pImmediateContext->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);

    // 인덱스 버퍼 만들기
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

    // 인덱스 버퍼 속성 정해주기
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(WORD) * 36;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bd.CPUAccessFlags = 0;

    // 인덱스 데이터 포인터 연결해주기
    InitData.pSysMem = indices;

    // 인덱스 버퍼 만들어주기
    hr = g_pd3dDevice->CreateBuffer(&bd, &InitData, &g_pIndexBuffer);
    if (FAILED(hr))
    {
        return false;
    }

    // 인덱스 버퍼 세팅 (인덱스 개수가 엄청 많아지면 DXGI_FORMAT_R32_UINT로 바꾼다.)
    g_pImmediateContext->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);

    // triangle list로 그리기
    g_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Constant 버퍼 만들기
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
    // DirectTex로 나중에 바꾸도록 하자.

    ScratchImage scratchImage = ScratchImage{};
    hr = LoadFromDDSFile(L"seafloor.dds", DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
    //hr = D3DX11CreateShaderResourceViewFromFile(g_pd3dDevice, L"seafloor.dds", nullptr, nullptr, &g_pTextureRV, nullptr);
    if (FAILED(hr))
    {
        return false;
    }

    hr = CreateShaderResourceView(g_pd3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), &g_pTextureRV);

    // 텍스쳐의 Sampler state 만들기
    D3D11_SAMPLER_DESC sampDesc;
    memset(&sampDesc, 0, sizeof(sampDesc));
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR; // 조합해서 쓸 수 있다. 지금은 선형 보간하는데 쓰인다. 
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP; // Wrap은 값이 넘어간다면 0 ~ 1 , 1 ~ 0 으로 반복이 되는 것이고
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; // Mirror는 0 ~ 1 ~ 0으로 반복이 된다.
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP; // Clamp는 0 ~ 1을 벗어나면, 마지막 픽셀로 채워진다.
    // Border는 넘어가면 Border Color로 지정해준 색으로 채워진다.
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER; // 셈플링된 데이터를 기존 샘플링된 데이터와 비교하는 함수
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;


    hr = g_pd3dDevice->CreateSamplerState(&sampDesc, &g_pSamplerLinear);
    if (FAILED(hr))
    {
        return false;
    }

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 뷰 매트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    // 변할일이 없는 constant buffer 초기화
    CBChangeOnInput cbNeverChanges;
    cbNeverChanges.mView = MatrixTranspose(g_ViewMat);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

    // 프로젝션 매트릭스 초기화
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, width / (float)height, 0.01f, 100.f);

    // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);

    return true;
}