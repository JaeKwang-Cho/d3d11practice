#include "pch.h"
#include "CScene.h"
#include "CObject.h"
#include "CKeyManager.h"
#include "CTimeManager.h"
#include "CMouseManager.h"

void CScene::AddObject(CObject* _pObj, GROUP_TYPE _eType)
{
    m_vecObjectGroup[(UINT)_eType].push_back(_pObj);
    _pObj->SetOwnerScene(this);
}

void CScene::Enter()
{
    InitCamera3D();

    InitCamera2D();

	EnterScene();

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Start();
		}
	}
}

void CScene::Update()
{
    FlyCamera();

	UpdateScene();

	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			m_vecObjectGroup[i][j]->Update();
		}
	}
}

void CScene::InitCamera3D()
{
    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 뷰 매트릭스 초기화
    m_CameraStruct3D.Eye = Vector4(0.0f, 3.0f, -6.0f, 0.0f);
    m_CameraStruct3D.Direction = Vector4(0.0f, -2.0f, 6.0f, 0.0f).Normalize3Vec();
    m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
    m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
    g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);

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

void CScene::InitCamera2D()
{
    // 월드 매트릭스 초기화
    g_WorldMat = MatrixIdentity();

    // 오쏘 그래픽 뷰 초기화
    RECT rc = {};
    GetClientRect(g_hWnd, &rc);
    UINT width = rc.right - rc.left;
    UINT height = rc.bottom - rc.top;

    g_OrthogratphicMat = MatrixOrthographicOffCenterLH(0.f, width, height, 0.f, 0.01f, 100.f);
}

void CScene::FlyCamera()
{
    if (GetFocus() != CCore::GetInstance()->GetMainHwnd())
    {
        return;
    }
    else if (KEYINPUTHOLD(KEY::RMOUSE))
    {

        if (KEYINPUTHOLD(KEY::W))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction * 3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
        }
        if (KEYINPUTHOLD(KEY::A))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.CameraLeft * -3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
        }
        if (KEYINPUTHOLD(KEY::S))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction * -3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
        }
        if (KEYINPUTHOLD(KEY::D))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.CameraLeft * 3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
        }
        if (KEYINPUTHOLD(KEY::E))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.CameraUp * 3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
        }
        if (KEYINPUTHOLD(KEY::Q))
        {
            m_CameraStruct3D.Eye = m_CameraStruct3D.Eye + m_CameraStruct3D.CameraUp * -3.f * DELTA_F;

            m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
            m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
            g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
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

                Vector4 Formal = m_CameraStruct3D.Direction;
                m_CameraStruct3D.Direction = RotateVectorAroundLocalAxis(m_CameraStruct3D.Direction, m_CameraStruct3D.CameraLeft, Pitch);
                if (abs(m_CameraStruct3D.Direction.y) >= FLOAT_NEAR_ONE)
                {
                    m_CameraStruct3D.Direction = Formal;
                }
                else
                {
                    bChanged = true;
                }
            }
            if (abs(Yaw) > MOUSE_THRESHOLD)
            {
                Yaw *= 3.f;
                m_CameraStruct3D.Direction = RotateVectorAroundLocalAxis(m_CameraStruct3D.Direction, m_CameraStruct3D.CameraUp, Yaw);
                bChanged = true;
            }

            if (bChanged)
            {
                m_CameraStruct3D.Direction = m_CameraStruct3D.Direction.Normalize3Vec();
                m_CameraStruct3D.At = m_CameraStruct3D.Eye + m_CameraStruct3D.Direction;
                m_CameraStruct3D.Up = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
                g_ViewMat = MatrixLookAtLH(m_CameraStruct3D.Eye, m_CameraStruct3D.At, m_CameraStruct3D.Up, m_CameraStruct3D.CameraLeft, m_CameraStruct3D.CameraUp);
            }

            CMouseManager::GetInstance()->RezeroRelativePos();
        }
    }
}

CScene::CScene()
	:m_CameraStruct3D()
    , m_pSceneLight(nullptr)
{
}

CScene::~CScene()
{
	for (UINT i = 0; i < (UINT)GROUP_TYPE::END; i++)
	{
		for (size_t j = 0; j < m_vecObjectGroup[i].size(); j++)
		{
			delete m_vecObjectGroup[i][j];
		}
	}
}

