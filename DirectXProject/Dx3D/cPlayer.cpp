#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"
#include "cRay.h"

cPlayer::cPlayer()
	: m_pGun(NULL)
	, m_pController(NULL)
{
}


cPlayer::~cPlayer()
{
	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pGun);
}

void cPlayer::Setup()
{
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "Pistol.X");

	m_pController = new cPlayerController;
	m_pController->Setup(0.1f);

}

void cPlayer::Update(D3DXVECTOR3 & camAngle, iMap * pMap)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pGun)
		m_pGun->Update(camAngle);
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();

}

void cPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
		{
			float centerX, centerY;
			RECT rc;
			GetClientRect(g_hWnd, &rc);
			centerX = (rc.left + rc.right) / 2;
			centerY = (rc.top + rc.bottom) / 2;

			cRay r = cRay::RayAtWorldSpace(centerX, centerY);

			D3DXVECTOR3 vDir = r.GetRayDir();
			D3DXVECTOR3 vUp = D3DXVECTOR3(0, 1, 0);
			D3DXVECTOR3 vPosition;
			D3DXVec3Cross(&vPosition, &vUp, &vDir);

			D3DXVec3Normalize(&vPosition, &vPosition);

			if (m_pGun)
				m_pGun->Fire(vDir, m_vPosition + vPosition * 0.5f);
	

		}
		break;
	}
}
