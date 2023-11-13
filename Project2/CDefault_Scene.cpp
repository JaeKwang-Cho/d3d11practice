#include "pch.h"
#include "CDefault_Scene.h"
#include "TestActor.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CMouseManager.h"

static Vector4 Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
static Vector4 Direction =  Vector4(0.0f, -2.0f, 6.0f, 0.0f).Normalize3Vec();
static Vector4 At = Eye + Direction;
static Vector4 Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
static Vector4 CameraLeft;
static Vector4 CameraUp;

void CDefault_Scene::EnterScene()
{
	TestActor* tempActor = new TestActor;

	AddObject(tempActor, GROUP_TYPE::Default);

    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 뷰 매트릭스 초기화
    Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    At = Eye + Direction;
    Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

    // Constant 버퍼 만들기
    D3D11_BUFFER_DESC bd;
    memset(&bd, 0, sizeof(bd));
    bd.CPUAccessFlags = 0;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = 0;

    //  #1 
    bd.ByteWidth = sizeof(CBChangeOnInput);
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
    CBChangeOnInput cbChangeOnInput;
    cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
    g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);

    // 프로젝션 매트릭스 초기화
    RECT rc = {};
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, width / (float)height, 0.01f, 100.f);

    // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);

}

void CDefault_Scene::UpdateScene()
{
    if (GetFocus() != CCore::GetInstance()->GetMainHwnd())
    {
        return;
    }
    if (KEYINPUTHOLD(KEY::RMOUSE))
    {

        if (KEYINPUTHOLD(KEY::W))
        {
            Eye = Eye + Vector4(0.f, 0.f, 3.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }
        if (KEYINPUTHOLD(KEY::A))
        {
            Eye = Eye + Vector4(-3.f, 0.f, 0.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }
        if (KEYINPUTHOLD(KEY::S))
        {
            Eye = Eye + Vector4(0.f, 0.f, -3.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }
        if (KEYINPUTHOLD(KEY::D))
        {
            Eye = Eye + Vector4(3.f, 0.f, 0.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }
        if (KEYINPUTHOLD(KEY::Q))
        {
            Eye = Eye + Vector4(0.f, 3.f, 0.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }
        if (KEYINPUTHOLD(KEY::E))
        {
            Eye = Eye + Vector4(0.f, -3.f, 0.f, 0.f) * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

            CBChangeOnInput cbChangeOnInput;
            cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
            g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);
        }

        FLOAT2 vMouseMov = CMouseManager::GetInstance()->GetRelativePos();

        if (vMouseMov.u != 0.f || vMouseMov.v != 0.f)
        {
            RECT rc = {};
            GetClientRect(g_hWnd, &rc);
            UINT width = rc.right - rc.left;
            UINT height = rc.bottom - rc.top;

            float Ratio = (float)width / (float)height;

            float Pitch = vMouseMov.v * DELTA_F * -1.f;
            float Yaw = vMouseMov.u * DELTA_F * -1.f * Ratio;

            bool bChanged = false;
            if (abs(Pitch) > MOUSE_THRESHOLD)
            {
                //Pitch *= 10.f;
                Direction = RotateVectorAroundLocalAxis(Direction, CameraLeft, Pitch);
                bChanged = true;
            }
            if (abs(Yaw) > MOUSE_THRESHOLD)
            {
                //Yaw *= 10.f;
                Direction = RotateVectorAroundLocalAxis(Direction, CameraUp, Yaw);
                bChanged = true;
            }

            if (bChanged)
            {
                At = Eye + Direction;
                Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
                g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);

                CBChangeOnInput cbChangeOnInput;
                cbChangeOnInput.mView = MatrixTranspose(g_ViewMat);
                g_pImmediateContext->UpdateSubresource(g_pCBNeverChanges, 0, nullptr, &cbChangeOnInput, 0, 0);

                CMouseManager::GetInstance()->SetCursorMiddle();
            }

            CMouseManager::GetInstance()->RezeroRelativePos();
        }
    }
    if (KEYINPUTTAP(KEY::RMOUSE))
    {
        ShowCursor(false);
    }

    if (KEYINPUTAWAY(KEY::RMOUSE) || KEYINPUTCHECK(KEY::RMOUSE, KEY_STATE::KS_NONE))
    {
        ShowCursor(true);
    }
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
