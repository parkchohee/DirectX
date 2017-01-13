#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"


cPlayer::cPlayer()
	: m_vPosition(0, 0, 0)
	, m_pGun(NULL)
{
}


cPlayer::~cPlayer()
{
}

void cPlayer::Setup()
{
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "Pistol.X");

}

void cPlayer::Update(D3DXVECTOR3& camAngle)
{
	// angle�� �̿��� direction�� ���Ѵ�. 
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, camAngle.y);
	D3DXVECTOR3 vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);

	// direction �������� ������,
	if (GetKeyState('W') & 0x8000)			// ������ ������
	{
		m_vPosition += (vDirection * 0.1f);
	}
	else if (GetKeyState('S') & 0x8000)		// �ڷ� ������
	{
		m_vPosition -= (vDirection * 0.1f);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);
	
	if (GetKeyState('A') & 0x8000)			// �������� ������
	{
		m_vPosition += (vDirection * 0.1f);
	}
	else if (GetKeyState('D') & 0x8000)		// ���������� ������
	{
		m_vPosition -= (vDirection * 0.1f);
	}

	if (m_pGun)
		m_pGun->Update(camAngle);
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();
	
}
