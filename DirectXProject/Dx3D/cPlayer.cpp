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
	m_pGun->Setup(&m_vPosition, "Gun/", "ShotGun.X");

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

	if (m_pGun)
		m_pGun->Update();
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();
	
}
