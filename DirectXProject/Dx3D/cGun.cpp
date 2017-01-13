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

void cGun::Update(D3DXVECTOR3& camAngle)
{
	Setting(camAngle);
}

void cGun::Render()
{
	if (m_pGun)
		m_pGun->UpdateAndRender();
}

void cGun::Setting(D3DXVECTOR3& camAngle)
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.1f, 0.1f, 0.1f);
	
	D3DXMatrixRotationY(&matR, -D3DX_PI / 2);

	if (m_pvTarget)
		D3DXMatrixTranslation(&matT, m_pvTarget->x + 1.0f, m_pvTarget->y, m_pvTarget->z + 3.0f);
	else
		D3DXMatrixIdentity(&matT);
	
	// �߽� �� �����ֱ� ���� �̵��� ȸ��, �ٽ� ����ġ��
	D3DXMATRIXA16 matTempT, matTempTInv;
	D3DXMatrixTranslation(&matTempT, 1.0f, 0, 3.0f);
	D3DXMatrixTranslation(&matTempTInv, -1.0f, 0, -3.0f);

	D3DXMatrixRotationX(&matRX, camAngle.x);
	D3DXMatrixRotationY(&matRY, camAngle.y);

	matR = matR * matTempT * matRX * matRY * matTempTInv;

	matSRT = matS * matR * matT;

	m_pGun->SetSRT(matSRT);
}
