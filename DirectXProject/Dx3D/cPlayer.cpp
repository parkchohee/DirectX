#include "stdafx.h"
#include "cPlayer.h"
#include "cSkinnedMesh.h"


cPlayer::cPlayer()
	: m_pShotGun(NULL)
	, m_vPosition(0, 0, 0)
{
}


cPlayer::~cPlayer()
{
	SAFE_DELETE(m_pShotGun);
}

void cPlayer::Setup()
{
	m_pShotGun = new cSkinnedMesh("Gun/", "ShotGun.X");
	/*D3DXMATRIXA16 matS, matRX, matRY, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matRX, D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixTranslation(&matT, 1, 0, 0);

	matSRT = matS * matRY * matRX * matT;
	m_pShotGun->SetSRT(matSRT);*/
}

void cPlayer::Update()
{
	if (GetKeyState('W') & 0x8000)			// 앞으로 움직임
	{
		m_vPosition.z += 0.1f;
	}
	else if (GetKeyState('S') & 0x8000)		// 뒤로 움직임
	{
		m_vPosition.z -= 0.1f;
	}

	if (GetKeyState('A') & 0x8000)			// 왼쪽으로 움직임
	{
		m_vPosition.x -= 0.1f;
	}
	else if (GetKeyState('D') & 0x8000)		// 오른쪽으로 움직임
	{
		m_vPosition.x += 0.1f;
	}

	GunSetting();
}

void cPlayer::Render()
{
	if (m_pShotGun)
		m_pShotGun->UpdateAndRender();
}

void cPlayer::GunSetting()
{
	D3DXMATRIXA16 matS, matRX, matRY, matR, matT, matSRT;
	D3DXMatrixScaling(&matS, 0.03f, 0.03f, 0.03f);
	D3DXMatrixRotationX(&matRX, D3DX_PI / 2);
	D3DXMatrixRotationY(&matRY, D3DX_PI);
	D3DXMatrixTranslation(&matT, m_vPosition.x + 1, m_vPosition.y, m_vPosition.z + 1.5);
	matR = matRY * matRX;
	matSRT = matS * matR * matT;
	m_pShotGun->SetSRT(matSRT);
}
