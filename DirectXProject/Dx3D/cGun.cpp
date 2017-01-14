#include "stdafx.h"
#include "cGun.h"
#include "cSkinnedMesh.h"
#include "cBullet.h"

cGun::cGun()
	: m_pGun(NULL)
	, m_pvTarget(NULL)
	, m_pBullet(NULL)
	, m_fAttackPower(1.f)
	, m_fAttackRange(10.f)
	, m_fAttackSpeed(1.f)
	, m_nMaxBullet(10)
	, m_nCurrentBullet(10)
{
}


cGun::~cGun()
{
	SAFE_DELETE(m_pGun);
	SAFE_DELETE(m_pBullet);
}

void cGun::Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename)
{
	m_pvTarget = pvTarget;

	m_pGun = new cSkinnedMesh(szFolder, szFilename);
}

void cGun::Update(D3DXVECTOR3& camAngle)
{
	Setting(camAngle);

	if (m_pBullet)
		m_pBullet->Update();
}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();


	if (m_pBullet)
		m_pBullet->Render();
}

void cGun::Setting(D3DXVECTOR3& camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2 - 0.15);

	if (m_pvTarget)
		D3DXMatrixTranslation(&matT, m_pvTarget->x + 1.0f, m_pvTarget->y, m_pvTarget->z + 3.0f);
	else
		D3DXMatrixIdentity(&matT);
	
	// 중심 축 맞춰주기 위해 이동후 회전, 다시 원위치로
	D3DXMATRIXA16 matTempT, matTempTInv;
	D3DXMatrixTranslation(&matTempT, 1.0f, 0, 3.0f);
	D3DXMatrixTranslation(&matTempTInv, -1.0f, 0, -3.0f);

	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matR = matR * matTempT * matRX * matRY * matTempTInv;

	matSRT = matS * matR * matT;

	m_pGun->SetSRT(matSRT);
}

void cGun::Fire(D3DXVECTOR3 & vDirection, D3DXVECTOR3 & vPosition)
{
 	m_pGun->SetAnimationIndex(7);
	
	if (!m_pBullet)
	{
		m_pBullet = new cBullet;
		m_pBullet->Setup(vDirection, vPosition);
	}
}
