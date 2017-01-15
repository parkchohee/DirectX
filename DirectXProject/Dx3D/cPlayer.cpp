#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"

cPlayer::cPlayer()
	: m_pGun(NULL)
	, m_pController(NULL)
{
}


cPlayer::~cPlayer()
{

	SAFE_RELEASE(m_pController);


}

void cPlayer::Setup()
{
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "Pistol.X");

	m_pController = new cPlayerController;
	m_pController->Setup();

}

void cPlayer::Update(D3DXVECTOR3& camAngle)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pGun)
		m_pGun->Update(camAngle);
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();

}

void cPlayer::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP:
		{
			if (m_pGun)
				m_pGun->Fire(m_vDirection, m_vPosition);
		}
		break;
	}
}
