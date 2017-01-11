#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"


cPlayer::cPlayer()
	: m_pShotGun(NULL)
{
}


cPlayer::~cPlayer()
{

	SAFE_DELETE(m_pShotGun);
}

void cPlayer::Setup()
{
	m_pShotGun = new cSkinnedMesh("Gun/", "ShotGun.X");
	D3DXMATRIXA16 matS, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationZ(&matR, -D3DX_PI);
	D3DXMatrixTranslation(&matT, 1, -1, 0);
	matSRT = matS*matR*matT;
	D3DXMatrixRotationX(&matR, -D3DX_PI / 2);
	matSRT = matSRT * matR;
	m_pShotGun->SetSRT(matSRT);
}

void cPlayer::Update()
{
}

void cPlayer::Render()
{
	if (m_pShotGun)
		m_pShotGun->UpdateAndRender();
}
