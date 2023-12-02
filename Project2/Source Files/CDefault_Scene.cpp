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

    // 프로젝션 매트릭스 초기화
    RECT rc = {};
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    g_ProjectionMat = MatrixPerspectiveFovLH(PIDiv2, width / (float)height, 0.01f, 100.f);
    /*
    // 윈도우 크기 변환 시에 변하는 Constant buffer 초기화
    CBChangeOnResize cbChangeOnResize;
    cbChangeOnResize.mProjection = MatrixTranspose(g_ProjectionMat);
    g_pImmediateContext->UpdateSubresource(g_pCBChangeOnResize, 0, nullptr, &cbChangeOnResize, 0, 0);
    */
}

void CDefault_Scene::UpdateScene()
{
    if (GetFocus() != CCore::GetInstance()->GetMainHwnd())
    {
        return;
    }
    else if (KEYINPUTHOLD(KEY::RMOUSE))
    {

        if (KEYINPUTHOLD(KEY::W))
        {
            Eye = Eye + Direction * 3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }
        if (KEYINPUTHOLD(KEY::A))
        {
            Eye = Eye + CameraLeft * -3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }
        if (KEYINPUTHOLD(KEY::S))
        {
            Eye = Eye + Direction * -3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }
        if (KEYINPUTHOLD(KEY::D))
        {
            Eye = Eye + CameraLeft * 3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }
        if (KEYINPUTHOLD(KEY::E))
        {
            Eye = Eye + CameraUp * 3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }
        if (KEYINPUTHOLD(KEY::Q))
        {
            Eye = Eye + CameraUp * -3.f * DELTA_F;

            At = Eye + Direction;
            Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
        }

        FLOAT2 vMouseMov = CMouseManager::GetInstance()->GetRelativePos();

        if (vMouseMov.u != 0.f || vMouseMov.v != 0.f)
        {
            RECT rc = {};
            GetClientRect(g_hWnd, &rc);
            UINT width = rc.right - rc.left;
            UINT height = rc.bottom - rc.top;

            float Ratio = (float)width / (float)height;

            float Pitch = vMouseMov.v * DELTA_F;
            float Yaw = vMouseMov.u * DELTA_F;

            bool bChanged = false;

            if (abs(Pitch) > MOUSE_THRESHOLD)
            {
                Pitch *= 3.f;

                Vector4 Formal = Direction;
                Direction = RotateVectorAroundLocalAxis(Direction, CameraLeft, Pitch);
                if (abs(Direction.y) >= FLOAT_NEAR_ONE)
                {
                    Direction = Formal;
                }
                else
                {
                    bChanged = true;
                }
            }
            if (abs(Yaw) > MOUSE_THRESHOLD)
            {
                Yaw *= 3.f;
                Direction = RotateVectorAroundLocalAxis(Direction, CameraUp, Yaw);
                bChanged = true;            
            }

            if (bChanged)
            {
                Direction = Direction.Normalize3Vec();
                At = Eye + Direction;
                Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
                g_ViewMat = MatrixLookAtLH(Eye, At, Up, CameraLeft, CameraUp);
            }

            CMouseManager::GetInstance()->RezeroRelativePos();
        }

        if (KEYINPUTTAP(KEY::RMOUSE))
        {
            ShowCursor(false);
        }

        if (KEYINPUTAWAY(KEY::RMOUSE) || KEYINPUTCHECK(KEY::RMOUSE, KEY_STATE::KS_NONE))
        {
            ShowCursor(true);
        }

        if (KEYINPUTAWAY(KEY::LMOUSE))
        {
            ID3D11Resource* pResource = nullptr;
            g_pRenderTargetView->GetResource(&pResource);
            GetShaderResourceFromViewToFile(pResource);
            if (pResource) pResource->Release();
        }
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
}
