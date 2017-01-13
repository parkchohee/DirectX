#include "stdafx.h"
#include "cPlayer.h"
#include "cGun.h"
#include "cUIObject.h"
#include "cUIImageView.h"

cPlayer::cPlayer()
	: m_vPosition(0, 0, 0)
	, m_pGun(NULL)
	, m_pUIRoot(NULL)
	, m_vDirection(0,0,1)
{
}


cPlayer::~cPlayer()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);

}

void cPlayer::Setup()
{
	m_pGun = new cGun;
	m_pGun->Setup(&m_vPosition, "Gun/", "Pistol.X");

	/// >> : cursor UI
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
	// angle을 이용해 direction을 구한다. 
	D3DXMATRIXA16 matR, matT;
	D3DXMatrixRotationY(&matR, camAngle.y);
	D3DXVECTOR3 vDirection = D3DXVECTOR3(0, 0, 1);

	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);
	m_vDirection = vDirection;

	// direction 방향으로 앞으로,
	if (GetKeyState('W') & 0x8000)			// 앞으로 움직임
	{
		m_vPosition += (vDirection * 0.1f);
	}
	else if (GetKeyState('S') & 0x8000)		// 뒤로 움직임
	{
		m_vPosition -= (vDirection * 0.1f);
	}

	D3DXMatrixRotationY(&matR, camAngle.y - D3DX_PI / 2);
	vDirection = D3DXVECTOR3(0, 0, 1);
	D3DXVec3TransformNormal(&vDirection, &vDirection, &matR);
	
	if (GetKeyState('A') & 0x8000)			// 왼쪽으로 움직임
	{
		m_vPosition += (vDirection * 0.1f);
	}
	else if (GetKeyState('D') & 0x8000)		// 오른쪽으로 움직임
	{
		m_vPosition -= (vDirection * 0.1f);
	}
	
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
