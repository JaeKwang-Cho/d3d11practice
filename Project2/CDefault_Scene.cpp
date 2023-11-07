#include "pch.h"
#include "CDefault_Scene.h"
#include "TestActor.h"

void CDefault_Scene::EnterScene()
{
	TestActor* tempActor = new TestActor;

	AddObject(tempActor, GROUP_TYPE::Default);

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 뷰 매트릭스 초기화
    Vector4 Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    Vector4 At = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up);

    // Constant 버퍼 만들기
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.CPUAccessFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    //  #1 
    bd.ByteWidth = sizeof(CBNeverChanges);
    HRESULT hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBNeverChanges);
    if (FAILED(hr))
    {
        assert(false && "CreateBuffer Failed");
    }
    // #2
    bd.ByteWidth = sizeof(CBChangeOnResize);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangeOnResize);
    if (FAILED(hr))
    {
        assert(false && "CreateBuffer Failed");
    }
    // #3
    bd.ByteWidth = sizeof(CBChangesEveryFrame);
    hr = g_pd3dDevice->CreateBuffer(&bd, nullptr, &g_pCBChangesEveryFrame);
    if (FAILED(hr))
    {
        assert(false && "CreateBuffer Failed");
    }
    // 변할일이 없는 constant buffer 초기화
    CBNeverChanges cbNeverChanges;
    cbNeverChanges.mView = MatrixTranspose(g_ViewMat);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbNeverChanges, 0, 0);

    // 프로젝션 매트릭스 초기화
    RECT rc = {};
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;
    g_ViewMat;
    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv4, width / (float)height, 0.01f, 100.f);

    // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);

}

void CDefault_Scene::UpdateScene()
{

}

void CDefault_Scene::Exit()
{
}

CDefault_Scene::CDefault_Scene()
{
}

CDefault_Scene::~CDefault_Scene()
{
    if (g_pCBNeverChanges) g_pCBNeverChanges->Release();
    if (g_pCBChangeOnResize) g_pCBChangeOnResize->Release();
    if (g_pCBChangesEveryFrame) g_pCBChangesEveryFrame->Release();
}
