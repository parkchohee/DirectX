#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"
#include "cRay.h"
#include "cOBB.h"
#include "cHeightMap.h"
#include "cTextMap.h"
#include "cSkinnedMesh.h"
#include "cRay.h"

cPlayer::cPlayer()
	: m_nSelectGun(0)
	, m_pPlayerOBB(NULL)
	, m_pBulletRay(NULL)
{
}


cPlayer::~cPlayer()
{
	SAFE_RELEASE(m_pController);
	SAFE_DELETE(m_pPlayerOBB);

	for each (auto p in m_vecGun)
		SAFE_RELEASE(p);
}

void cPlayer::Setup()
{
	m_vecGun.resize(GUNMAX);
	
	cGun* pGun1 = new cGun;
	pGun1->Setup(&m_vPosition, "Gun/shotgun/", "shotgun.X");
	m_vecGun[0] = pGun1;

	cGun* pGun2 = new cGun;
	pGun2->Setup(&m_vPosition, "Gun/winc/", "winchester.X");
	m_vecGun[1] = pGun2;
	
	cGun* pGun3 = new cGun;
	pGun3->Setup(&m_vPosition, "Gun/9mm/", "9mm.X");
	m_vecGun[2] = pGun3;

	m_pGun = m_vecGun[m_nSelectGun];

	m_pPlayerOBB = new cOBB;
	m_pPlayerOBB->Setup(D3DXVECTOR3(-0.5f, 0.0f, -0.5f),
		D3DXVECTOR3(0.5f, 2.0f, 0.5f));

	m_pController = new cPlayerController;
	m_pController->Setup(0.1f, this);
	m_pController->SetOBB(m_pPlayerOBB);

	m_stSphere.fRadius = PLAYER_BOUNDING_SPHERE_SIZE;
	m_stSphere.vCenter = m_vPosition;

	D3DXCreateSphere(g_pD3DDevice, m_stSphere.fRadius, 20, 20, &m_pBoundingSphereMesh, NULL);


}

void cPlayer::Update(D3DXVECTOR3 & camAngle)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pGun)
	{
		m_pGun->Update();
	}

	if (g_pKeyManager->IsStayKeyDown(VK_RBUTTON))
	{
		GunSettingZoom(camAngle);
	}
	else
	{
		GunSetting(camAngle);
	}

	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_nSelectGun++;
		m_nSelectGun %= m_vecGun.size();
		m_pGun = m_vecGun[m_nSelectGun];
	}

	D3DXMATRIXA16 matOBBWorld, matT, matRX, matRY;
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matOBBWorld = matRX * matRY * matT;

	if(m_pPlayerOBB)
		m_pPlayerOBB->Update(&matOBBWorld);

}

void cPlayer::Render()
{
	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, false);
	
	if (m_pGun)
		m_pGun->Render();

	g_pD3DDevice->SetRenderState(D3DRS_ZENABLE, true);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorldTM);
	
	if (m_pPlayerOBB)
		m_pPlayerOBB->OBBBox_Render(D3DCOLOR_XRGB(0, 0, 255));




	m_stSphere.vCenter = m_vPosition;
	m_stSphere.vCenter.y = 1;

	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	matWorld._41 = m_stSphere.vCenter.x;
	matWorld._42 = m_stSphere.vCenter.y;
	matWorld._43 = m_stSphere.vCenter.z;

	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_pBoundingSphereMesh->DrawSubset(0);




}

void cPlayer::SetHeightMap(cHeightMap * hMap)
{
	m_pController->SetHeightMap(hMap);
}

void cPlayer::SetTextMap(cTextMap * tMap)
{
	m_pController->SetTextMap(tMap);
}

void cPlayer::GunSetting(D3DXVECTOR3 & camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);
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

void cPlayer::GunSettingZoom(D3DXVECTOR3 & camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y + 0.5f, m_vPosition.z + 3.0f);

	// 중심 축 맞춰주기 위해 이동후 회전, 다시 원위치로
	D3DXMATRIXA16 matTempT, matTempTInv;
	D3DXMatrixTranslation(&matTempT, 0.f, 0.5, 3.0f);
	D3DXMatrixTranslation(&matTempTInv, 0.f, -0.5, -3.0f);

	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matR = matR * matTempT * matRX * matRY * matTempTInv;
	matSRT = matS * matR * matT;

	m_matWorldTM = matSRT;

	m_pGun->SetWorldMatrix(&m_matWorldTM);
}

void cPlayer::BulletFire(D3DXVECTOR3 dir)
{
	if (m_pGun)
	{
		m_pBulletRay = new cRay;

		g_pSoundManager->play("ShotgunFire", 0.5f);

		m_pGun->GetGunMesh()->PlayOneShot(1, 0, 0);
		m_pGun->Fire(dir, m_matWorldTM);
	}
}

cRay * cPlayer::GetBullet()
{
	return m_pBulletRay;
}

void cPlayer::DeleteRay()
{
	SAFE_DELETE(m_pBulletRay);
}
