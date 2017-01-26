#include "stdafx.h"
#include "cMapToolScene.h"
#include "cGrid.h"
#include "cStaticMesh.h"


cMapToolScene::cMapToolScene()
	: test(NULL)
	, m_pGrid(NULL)
{
}


cMapToolScene::~cMapToolScene()
{
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(test);

}

void cMapToolScene::Setup()
{
	test = new cStaticMesh;
	test->Setup("Map/", "test.X");

	m_pGrid = new cGrid;
	m_pGrid->Setup();
}

void cMapToolScene::Update()
{
}

void cMapToolScene::Render()
{

	if (test)
		test->Render();

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
}
