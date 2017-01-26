#include "stdafx.h"
#include "cMapToolScene.h"
#include "cGrid.h"
#include "cMapCamera.h"
#include "cStaticMesh.h"


cMapToolScene::cMapToolScene()
	: /*test(NULL)
	,*/ m_pCamera(NULL)
	, m_pGrid(NULL)
{
}


cMapToolScene::~cMapToolScene()
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	//SAFE_DELETE(test);

}

void cMapToolScene::Setup()
{
	/*test = new cStaticMesh;
	test->Setup("Map/", "test.X");
	D3DXMATRIXA16 matS;
	D3DXMatrixScaling(&matS, 0.01, 0.01, 0.01);
	test->SetWolrd(matS);
*/
	m_pGrid = new cGrid;
	m_pGrid->Setup();

	m_pCamera = new cMapCamera;
	m_pCamera->Setup(NULL);
}

void cMapToolScene::Update()
{
	if (m_pCamera)
		m_pCamera->Update();
}

void cMapToolScene::Render()
{
	//if (test)
	//	test->Render();

	if (m_pGrid)
		m_pGrid->Render();

	char szTemp[1024];
	sprintf(szTemp, "¸ÊÅø");

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 0, 100, 300, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));

}

void cMapToolScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}
}
