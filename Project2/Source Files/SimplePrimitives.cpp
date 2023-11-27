#include "pch.h"
#include "SimplePrimitives.h"

SampleVertex DefaultCube[] =
{
    // Pos, Color, Norm, UV
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 1.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, -1.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f),FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f),FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(-1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(1.0f, 0.0f, 0.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, -1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f), FLOAT3(0.0f, 0.0f, -1.0f), FLOAT2(0.0f, 1.0f) },

	{ FLOAT3(-1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 0.0f) },
	{ FLOAT3(1.0f, -1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 0.0f) },
	{ FLOAT3(1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(1.0f, 1.0f) },
	{ FLOAT3(-1.0f, 1.0f, 1.0f), FLOAT4(1.0f, 1.0f, 1.0f, 1.0f) , FLOAT3(0.0f, 0.0f, 1.0f), FLOAT2(0.0f, 1.0f) },
};

WORD SampleIndices[] =
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

bool GetSimpleCube(ID3D11Device* _d3dDevice,
                   ID3D11DeviceContext* _d3dImmediateContext,
                   ID3D11VertexShader** _VS,
                   ID3D11Buffer** _VertexBuff,
                   ID3D11InputLayout** _Layout,
                   ID3D11PixelShader** _PS,
                   ID3D11Buffer** _IndexBuff,
                   const wchar_t* _texFile,
                   ID3D11ShaderResourceView** _texRV,
                   ID3D11SamplerState** _SamplerLinear)
{
    HRESULT hr = S_OK;

    bool Result;
    // _d3dDevice, _VS, _Layout -> 버텍스 쉐이더 만들기 + 인풋 레이아웃 정해주기
    {
        // Shader를 컴파일 해서 Binary Large Object file로 만들기
        ID3DBlob* pVSBlob = nullptr;
        Result = CompileShaderFromFile(L"DefaultShader.fx", "VS", "vs_5_0", &pVSBlob);
        if (Result == false)
        {
            MessageBox(nullptr,
                       L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            return false;
        }

        // 버텍스 쉐이더 객체 만들기
        hr = _d3dDevice->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, _VS);
        if (FAILED(hr))
        {
            pVSBlob->Release();
            return false;
        }


        // input layout 정해주기
        D3D11_INPUT_ELEMENT_DESC layout[] =
        {
            {"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0}

        };
        UINT numElements = ARRAYSIZE(layout);

        // input layout 만들기
        hr = _d3dDevice->CreateInputLayout(
            layout, numElements, pVSBlob->GetBufferPointer(),
            pVSBlob->GetBufferSize(), _Layout);

        pVSBlob->Release();
        if (FAILED(hr))
        {
            return false;
        }

        // input layer 세팅해주기
        _d3dImmediateContext->IASetInputLayout(*_Layout);
    }

    // _PS -> 픽셀 쉐이더 만들기
    {
        // 픽셀 쉐이더 컴파일 해주기
        ID3DBlob* pPSBlob = nullptr;
        Result = CompileShaderFromFile(L"DefaultShader.fx", "PS", "ps_5_0", &pPSBlob);
        if (Result == false)
        {
            MessageBox(NULL,
                       L"The FX file cannot be compiled.  Please run this executable from the directory that contains the FX file.", L"Error", MB_OK);
            return false;
        }

        // 픽셀 쉐이더 만들기
        hr = _d3dDevice->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, _PS);
        pPSBlob->Release();
        if (FAILED(hr))
        {
            return false;
        }
    }

    // _VertexBuff -> 버텍스 버퍼에 값 넘겨주기  
    {
        // 버텍스 버퍼 속성 정해주기
        D3D11_BUFFER_DESC bd;
        memset(&bd, 0, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(SampleVertex) * 24;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        // 버텍스 데이터 포인터를 연결해준다.
        D3D11_SUBRESOURCE_DATA InitData;
        memset(&InitData, 0, sizeof(InitData));
        InitData.pSysMem = DefaultCube;
        hr = _d3dDevice->CreateBuffer(&bd, &InitData, _VertexBuff);
        if (FAILED(hr))
        {
            return false;
        }

        // 버텍스 버퍼 객체 세팅 (바꿀 일이 없이 때문에 이렇게 한다.)
        UINT stride = sizeof(SampleVertex);
        UINT offset = 0;
        _d3dImmediateContext->IASetVertexBuffers(0, 1, _VertexBuff, &stride, &offset);
    }

    // _IndexBuff -> 인덱스 버퍼 세팅
    {
        // 인덱스 버퍼 속성 정해주기
        D3D11_BUFFER_DESC bd;
        memset(&bd, 0, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WORD) * 36;
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;

        // 인덱스 데이터 포인터 연결해주기
        D3D11_SUBRESOURCE_DATA InitData;
        memset(&InitData, 0, sizeof(InitData));
        InitData.pSysMem = SampleIndices;

        // 인덱스 버퍼 만들어주기
        hr = _d3dDevice->CreateBuffer(&bd, &InitData, _IndexBuff);
        if (FAILED(hr))
        {
            return false;
        }
    }
    
    // Render Asset으로 빼기
    {
        // 인덱스 버퍼 세팅 (인덱스 개수가 엄청 많아지면 DXGI_FORMAT_R32_UINT로 바꾼다.)
        _d3dImmediateContext->IASetIndexBuffer(*_IndexBuff, DXGI_FORMAT_R16_UINT, 0);

        // triangle list로 그리기
        _d3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }

    // _texRV -> 텍스쳐 리소스 뷰 만들기
    {
        // Load the Texture
        ScratchImage scratchImage = ScratchImage{};
        hr = LoadFromDDSFile(L"seafloor.dds", DirectX::DDS_FLAGS_NONE, nullptr, scratchImage);
        //hr = D3DX11CreateShaderResourceViewFromFile(_d3dDevice, L"seafloor.dds", nullptr, nullptr, &g_pTextureRV, nullptr);
        if (FAILED(hr))
        {
            return false;
        }

        hr = CreateShaderResourceView(_d3dDevice, scratchImage.GetImages(), scratchImage.GetImageCount(), scratchImage.GetMetadata(), _texRV);
    }
    
    // _SamplerLinear -> 텍스쳐 샘플러 만들기
    {
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


        hr = _d3dDevice->CreateSamplerState(&sampDesc, _SamplerLinear);
        if (FAILED(hr))
        {
            return false;
        }
    }

    return true;
}
