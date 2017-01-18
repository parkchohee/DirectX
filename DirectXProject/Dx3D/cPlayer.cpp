#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"
#include "cRay.h"

cPlayer::cPlayer()
	: m_pController(NULL)
	, m_pGun(NULL)
	, m_nSelectGun(0)
{
}


cPlayer::~cPlayer()
{
	SAFE_RELEASE(m_pController);

	for each (auto p in m_vecGun)
		SAFE_RELEASE(p);
}

void cPlayer::Setup()
{
	m_vecGun.resize(GUNMAX);
	
	cGun* pGun1 = new cGun;
	pGun1->Setup(&m_vPosition, "Gun/Pistol/", "Pistol.X");
	m_vecGun[0] = pGun1;

	cGun* pGun2 = new cGun;
	pGun2->Setup(&m_vPosition, "Gun/Pistol2/", "Pistol2.X");
	m_vecGun[1] = pGun2;

	cGun* pGun3 = new cGun;
	pGun3->Setup(&m_vPosition, "Gun/MG_42/", "MG_42.X");
	m_vecGun[2] = pGun3;

	m_pGun = m_vecGun[m_nSelectGun];

	m_pController = new cPlayerController;
	m_pController->Setup(0.1f);

}

void cPlayer::Update(D3DXVECTOR3 & camAngle, iMap * pMap)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	
	// 한번만 눌리게 바꿔야함...
	
	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_nSelectGun++;
		m_nSelectGun %= 3;
		m_pGun = m_vecGun[m_nSelectGun];
	}

	if (m_pGun)
		m_pGun->Update(camAngle);
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();
}

cGun * cPlayer::GetGun()
{
	if (m_pGun)
		return m_pGun;
	
	return NULL;
}

void cPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	/*case WM_MOUSEWHEEL:
		m_nSelectGun = abs((int)(GET_WHEEL_DELTA_WPARAM(wParam) / 100.f) % 3);
		m_pGun = m_vecGun[m_nSelectGun];
		break;*/
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
			vPosition.y += 0.5f;

			//if (m_vecGun[m_nSelectGun])
			//	m_vecGun[m_nSelectGun]->Fire(vDir, m_vPosition + vPosition * 0.5f);

			if (m_pGun)
				m_pGun->Fire(vDir, m_vPosition + vPosition * 0.5f);
	

		}
		break;
	}
}
