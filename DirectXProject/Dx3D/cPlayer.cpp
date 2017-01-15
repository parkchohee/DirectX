#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cPlayerController.h"
#include "cUIObject.h"
#include "cUIImageView.h"

cPlayer::cPlayer()
	: m_pGun(NULL)
	, m_pUIRoot(NULL)
	, m_pController(NULL)
{
}


cPlayer::~cPlayer()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pController);
	SAFE_RELEASE(m_pSprite);

}

void cPlayer::Setup()
{
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "Pistol.X");

	m_pController = new cPlayerController;
	m_pController->Setup();

	/// >> : cursor UI, playScene으로 옮겨야 할 것 같음.
	{
		RECT rc;
		GetClientRect(g_hWnd, &rc);

		float centerX, centerY;
		centerX = (rc.left + rc.right) / 2;
		centerY = (rc.top + rc.bottom) / 2;

		D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

		m_pUIRoot = new cUIObject;
		m_pUIRoot->SetPosition(centerX, centerY);

		cUIImageView* pImageCursorL = new cUIImageView;
		pImageCursorL->SetTexture("PlayerUI/cursor_h.tga");
		pImageCursorL->SetPosition(-16, 7);

		cUIImageView* pImageCursorR = new cUIImageView;
		pImageCursorR->SetTexture("PlayerUI/cursor_h.tga");
		pImageCursorR->SetPosition(16, 7);

		cUIImageView* pImageCursorT = new cUIImageView;
		pImageCursorT->SetTexture("PlayerUI/cursor_v.tga");
		pImageCursorT->SetPosition(7, -16);

		cUIImageView* pImageCursorB = new cUIImageView;
		pImageCursorB->SetTexture("PlayerUI/cursor_v.tga");
		pImageCursorB->SetPosition(7, 16);

		m_pUIRoot->AddChild(pImageCursorL);
		m_pUIRoot->AddChild(pImageCursorR);
		m_pUIRoot->AddChild(pImageCursorT);
		m_pUIRoot->AddChild(pImageCursorB);
	}
}

void cPlayer::Update(D3DXVECTOR3& camAngle)
{
	if (m_pController)
		m_pController->Update(camAngle, m_vDirection, m_vPosition);

	if (m_pUIRoot)
	{
		m_pUIRoot->Update();
	}

	if (m_pGun)
		m_pGun->Update(camAngle);
}

void cPlayer::Render()
{
	if (m_pGun)
		m_pGun->Render();

	if(m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
	
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
