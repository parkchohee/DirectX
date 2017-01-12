#include "StdAfx.h"
#include "cCamera.h"


cCamera::cCamera(void)
	: m_vEye(0, 1, 0)
	, m_vLookAt(0, 1, 5)
	, m_vUp(0, 1, 0)
	, m_pvTarget(NULL)
	, m_fCameraDistance(5.0f)
	, m_isLButtonDown(false)
	, m_vCamRotAngle(0, 0, 0)
{
	m_ptPrevMouse.x = 0;
	m_ptPrevMouse.y = 0;
}


cCamera::~cCamera(void)
{
}

void cCamera::Setup( D3DXVECTOR3* pvTarget )
{
	m_pvTarget = pvTarget;

	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom,
		1.0f, 
		10000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
}

void cCamera::Update()
{
	m_vEye = D3DXVECTOR3(0, 1, 0);
	m_vLookAt = D3DXVECTOR3(0, 1, 5);

	D3DXMATRIXA16 matR, matRX, matRY;
	D3DXMatrixRotationX(&matRX, m_vCamRotAngle.x);
	D3DXMatrixRotationY(&matRY, m_vCamRotAngle.y);

	matR = matRX * matRY;
	D3DXVec3TransformCoord(&m_vLookAt, &m_vLookAt, &matR);

	if (m_pvTarget)
	{
		m_vEye = m_vEye + *m_pvTarget;
		m_vLookAt = m_vLookAt + *m_pvTarget;
	}

	D3DXMATRIXA16 matView;
	D3DXMatrixLookAtLH(&matView, &m_vEye, &m_vLookAt, &m_vUp);
	
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptPrevMouse.x = LOWORD(lParam);
		m_ptPrevMouse.y = HIWORD(lParam);
		m_isLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_isLButtonDown = false;
		break;
	case WM_MOUSEMOVE:
		{
			if(m_isLButtonDown)
			{
				POINT ptCurrMouse;
				ptCurrMouse.x = LOWORD(lParam);
				ptCurrMouse.y = HIWORD(lParam);
				float fDeltaX = (float)ptCurrMouse.x - m_ptPrevMouse.x;
				float fDeltaY = (float)ptCurrMouse.y - m_ptPrevMouse.y;
				m_vCamRotAngle.y += (fDeltaX / 100.f);
				m_vCamRotAngle.x += (fDeltaY / 100.f);
				if(m_vCamRotAngle.x < -D3DX_PI / 2.0f + 0.0001f)
					m_vCamRotAngle.x = -D3DX_PI / 2.0f + 0.0001f;
				if(m_vCamRotAngle.x > D3DX_PI / 2.0f - 0.0001f)
					m_vCamRotAngle.x = D3DX_PI / 2.0f - 0.0001f;
				m_ptPrevMouse = ptCurrMouse;
			}
		}
		break;
	case WM_MOUSEWHEEL:
		m_fCameraDistance -= (GET_WHEEL_DELTA_WPARAM(wParam) / 30.f);
		if(m_fCameraDistance < 0.0001f)
			m_fCameraDistance = 0.0001f;
		break;
	}
}
