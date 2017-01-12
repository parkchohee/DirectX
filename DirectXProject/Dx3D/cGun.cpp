#include "stdafx.h"
#include "cGun.h"
#include "cSkinnedMesh.h"


cGun::cGun()
	: m_pGun(NULL)
	, m_pvTarget(NULL)
	, m_fAttackPower(1.f)
	, m_fAttackRange(10.f)
	, m_fAttackSpeed(1.f)
	, m_nMaxAmmo(10)
	, m_nCurrentAmmo(10)
{
}


cGun::~cGun()
{
	SAFE_DELETE(m_pGun);
}

void cGun::Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename)
{
	m_pvTarget = pvTarget;

	m_pGun = new cSkinnedMesh(szFolder, szFilename);
}

void cGun::Update()
{
	Setting();
}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();
}

void cGun::Setting()
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matRX, D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	
	if (m_pvTarget)
		D3DXMatrixTranslation(&matT, m_pvTarget->x + 1, m_pvTarget->y, m_pvTarget->z + 1.5);
	else
		D3DXMatrixIdentity(&matT);

	matR = matRY * matRX;
	matSRT = matS * matR * matT;
	m_pGun->SetSRT(matSRT);
}
