#include "stdafx.h"
#include "cPlayScene.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cPlayer.h"

cPlayScene::cPlayScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pPlayer(NULL)
{
}


cPlayScene::~cPlayScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pPlayer);
}

void cPlayScene::Setup()
{
	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));

	m_pGrid = new cGrid;
	m_pGrid->Setup();

}

void cPlayScene::Update()
{
	if (m_pPlayer && m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	if (m_pCamera)
		m_pCamera->Update();
}

void cPlayScene::Render()
{
	if (m_pGrid)
		m_pGrid->Render();

	if (m_pPlayer)
		m_pPlayer->Render();
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
