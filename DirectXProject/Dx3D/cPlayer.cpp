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
	D3DXMatrixIdentity(&m_matWorldTM);
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
	pGun1->Setup(&m_vPosition, "Gun/", "Shotgun.X");
	m_vecGun[0] = pGun1;

	cGun* pGun2 = new cGun;
	pGun2->Setup(&m_vPosition, "Gun/", "Law.X");
	m_vecGun[1] = pGun2;
	
	cGun* pGun3 = new cGun;
	pGun3->Setup(&m_vPosition, "Gun/", "Bullpup.X");
	m_vecGun[2] = pGun3;

	m_pGun = m_vecGun[m_nSelectGun];

	m_pController = new cPlayerController;
	m_pController->Setup(0.1f);

}

void cPlayer::Update(D3DXVECTOR3 & camAngle, iMap * pMap)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pGun)
	{
		GunSetting(camAngle);
		m_pGun->Update();
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_LBUTTON))
	{
		BulletFire();
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_RBUTTON))
	{
		
	}

	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_nSelectGun++;
		m_nSelectGun %= 3;
		m_pGun = m_vecGun[m_nSelectGun];
	}

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

void cPlayer::GunSetting(D3DXVECTOR3 & camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);

	D3DXMatrixRotationY(&matR, -D3DX_PI / 2 - 0.15);

	D3DXMatrixTranslation(&matT, m_vPosition.x + 1.0f, m_vPosition.y + 0.5f, m_vPosition.z + 3.0f);

	// 중심 축 맞춰주기 위해 이동후 회전, 다시 원위치로
	D3DXMATRIXA16 matTempT, matTempTInv;
	D3DXMatrixTranslation(&matTempT, 1.0f, 0.5, 3.0f);
	D3DXMatrixTranslation(&matTempTInv, -1.0f, -0.5, -3.0f);

	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matR = matR * matTempT * matRX * matRY * matTempTInv;

	matSRT = matS * matR * matT;

	m_matWorldTM = matSRT;

	m_pGun->SetWorldMatrix(&m_matWorldTM);
}

void cPlayer::BulletFire()
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

	if (m_pGun)
		m_pGun->Fire(vDir, m_vPosition + vPosition * 0.5f);
}
