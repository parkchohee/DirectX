#include "stdafx.h"
#include "cMapToolScene.h"
#include "cGrid.h"
#include "cMapCamera.h"
#include "cBuilding.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cStaticMesh.h"
#include "cMakeGround.h"
#include "cMakeBuildings.h"
#include "cRay.h"


cMapToolScene::cMapToolScene()
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_mapMode(MAKE_BUILDINGS)
	, m_pSkyView(NULL)
	, m_pGroundMode(NULL)
	, m_pBuildingMode(NULL)
{
}


cMapToolScene::~cMapToolScene()
{
	
}

void cMapToolScene::Setup()
{
	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cMapCamera;
	m_pCamera->Setup(NULL);

	m_pSkyView = new cStaticMesh("Map/Sky/", "sky.X");//g_pStaticMeshManager->GetStaticMesh("Map/Sky/", "sky.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.05f, 0.05f, 0.05f);
	m_pSkyView->SetWorld(matS);

	m_pGroundMode = new cMakeGround;
	m_pGroundMode->Setup();

	m_pBuildingMode = new cMakeBuildings;
	m_pBuildingMode->Setup();

	SettingUI();

}

void cMapToolScene::Destroy()
{
	if (m_pUIRoot)
		m_pUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);

	SAFE_DELETE(m_pBuildingMode);
	SAFE_DELETE(m_pGroundMode);
	SAFE_DELETE(m_pSkyView);

	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

}

void cMapToolScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();

	switch (m_mapMode)
	{
	case MAKE_GROUND:
		if (m_pGroundMode)
			m_pGroundMode->Update(m_pCamera->GetMousePosition());
		break;
	case MAKE_BUILDINGS:
		if (m_pBuildingMode)
			m_pBuildingMode->Update();
		break;
	}


	if (g_pKeyManager->IsOnceKeyDown('L'))
	{
		if (m_mapMode == MAKE_GROUND)
		if (m_pGroundMode)
		{
			m_pGroundMode->SaveMapFile();
			m_mapMode = MAKE_BUILDINGS;
		}
	}

	if (g_pKeyManager->IsOnceKeyDown('K'))
	{
		if (m_mapMode == MAKE_BUILDINGS)
		if (m_pBuildingMode)
		{
			m_pBuildingMode->SaveMapFile();
			m_mapMode = MAKE_GROUND;
		}
	}

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
	{
		g_pSceneManager->ChangeScene("playScene");
	}
	if (m_pUIRoot)
		m_pUIRoot->Update();
}

void cMapToolScene::Render()
{

	if (m_pSkyView)
		m_pSkyView->Render();

	if (m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);

	switch (m_mapMode)
	{
	case MAKE_GROUND:
		if (m_pGroundMode)
			m_pGroundMode->Render();
		break;
	case MAKE_BUILDINGS:
		if (m_pBuildingMode)
			m_pBuildingMode->Render();
		break;
	}


	if (m_pGrid)
		m_pGrid->Render();


}

void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}

void cMapToolScene::OnClick(cUIButton * pSender)
{
}

void cMapToolScene::SettingUI()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	m_pUIRoot = new cUIObject;
	m_pUIRoot->SetPosition(0, 0);

	cUIImageView* pImageBackground = new cUIImageView;
	pImageBackground->SetTexture("Map/UI/bg.png");
	pImageBackground->SetPosition(rc.right - pImageBackground->GetSize().nWidth / 2, pImageBackground->GetSize().nHeight / 2);

	m_pUIRoot->AddChild(pImageBackground);
}
