#include "stdafx.h"
#include "cPlayScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cUIObject.h"
#include "cUIImageView.h"
#include "cPlayer.h"
#include "cAI.h"

cPlayScene::cPlayScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pUIRoot(NULL)
	, m_pPlayer(NULL)
	, m_pAI(NULL)
{
}


cPlayScene::~cPlayScene()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pPlayer);
	SAFE_RELEASE(m_pAI);
}

void cPlayScene::Setup()
{
	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();

	m_pAI = new cAI;
	m_pAI->Setup("AI/", "testMan.X");

	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));

	m_pGrid = new cGrid;
	m_pGrid->Setup();

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

void cPlayScene::Update()
{
	if (m_pPlayer && m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	if (m_pAI)
		m_pAI->Update(NULL);

	if (m_pCamera)
		m_pCamera->Update();

	if (m_pUIRoot)
		m_pUIRoot->Update();
}

void cPlayScene::Render()
{
	
	if (m_pPlayer)
		m_pPlayer->Render();
	
	if (m_pAI)
		m_pAI->Render();
	
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);
}

void cPlayScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pPlayer)
	{
		m_pPlayer->WndProc(hWnd, message, wParam, lParam);
	}

	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}

}
