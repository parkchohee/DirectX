#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cAseLoader.h"
#include "cFrame.h"
#include "cUIImageView.h"
#include "cUITextView.h"
#include "cUIButton.h"
#include "cRay.h"
#include "cObjLoader.h"
#include "cGroup.h"
#include "cSkinnedMesh.h"
#include "cFrustum.h"
#include "cZealot.h"
#include "cCharacter.h"
#include "cOBB.h"			/// << : OBB

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pMap(NULL)
	, m_pRootFrame(NULL)
	, m_pFont(NULL)
	, m_pSprite(NULL)
	, m_pUIRoot(NULL)
	, m_pMesh(NULL)
	, m_pSphere(NULL)
	, m_pObjMesh(NULL)
	, m_pFrustum(NULL)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);
	SAFE_DELETE(m_pMap);
	SAFE_DELETE(m_pFrustum);

	SAFE_RELEASE(m_pFont);
	SAFE_RELEASE(m_pSprite);
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pSphere);
	SAFE_RELEASE(m_pObjMesh);
	
	SAFE_DELETE(m_pHoldZealot);
	SAFE_DELETE(m_pMoveZealot);
	if(m_pRootFrame)
		m_pRootFrame->Destroy();
	
	if(m_pUIRoot)
		m_pUIRoot->Destroy();

	for each(auto p in m_vecMtlTex)
	{
		SAFE_RELEASE(p);
	}


	for each(auto p in m_vecGroup)
	{
		SAFE_RELEASE(p);
	}
	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	m_pHoldZealot = new cZealot;
	m_pHoldZealot->Setup();

	m_pMoveZealot = new cZealot;
	m_pMoveZealot->Setup();
	cCharacter* pCharacter = new cCharacter;
	m_pMoveZealot->SetCharacterController(pCharacter);
	SAFE_RELEASE(pCharacter);
	
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);
	D3DXCreateTeapot(g_pD3DDevice, &m_pMesh, NULL);
	D3DXCreateSphere(g_pD3DDevice, 0.5f, 10, 10, &m_pSphere, NULL);
	
	for (int i = 0; i < 11; ++i)
	{
		ST_SPHERE s;
		s.fRadius = 0.5f;
		s.vCenter = D3DXVECTOR3(0, 0, -10 + 2.0f * i);
		m_vecSphere.push_back(s);
	}

	ZeroMemory(&m_stMtl, sizeof(D3DMATERIAL9));
	m_stMtl.Ambient = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Diffuse = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	m_stMtl.Specular = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

	ZeroMemory(&m_stMtlPicked, sizeof(D3DMATERIAL9));
	m_stMtlPicked.Ambient = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
	m_stMtlPicked.Diffuse = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);
	m_stMtlPicked.Specular = D3DXCOLOR(0.7f, 0.0f, 0.0f, 1.0f);

	
	ST_PN_VERTEX v;
	v.n = D3DXVECTOR3(0, 1, 0);

	v.p = D3DXVECTOR3(-10, 0, -10); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-10, 0,  10); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 10, 0,  10); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3(-10, 0, -10); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 10, 0,  10); m_vecVertex.push_back(v);
	v.p = D3DXVECTOR3( 10, 0, -10); m_vecVertex.push_back(v);


	m_pFrustum = new cFrustum;
	m_pFrustum->Setup();

	m_pCamera = new cCamera;
	m_pCamera->Setup(NULL);

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	D3DXVECTOR3 vDir(1.0f, -1.0f,  1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	/// >> : OBB
	{
		if (m_pHoldZealot)
			m_pHoldZealot->Update(m_pMap);

		if (m_pMoveZealot)
			m_pMoveZealot->Update(m_pMap);
	}

	if(m_pCamera)
		m_pCamera->Update();

	if(m_pFrustum)
		m_pFrustum->Update();

	if(m_pRootFrame)
	{
		int nKeyFrame = GetTickCount() % (3200 - 640) + 640;
		m_pRootFrame->Update(nKeyFrame, NULL);
	}

	if(m_pUIRoot)
	{
		m_pUIRoot->Update();
	}
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	if(m_pGrid)
		m_pGrid->Render();

	D3DXMATRIXA16 matWorld;
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);
	if(m_pRootFrame)
		m_pRootFrame->Render();

	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	/// >> : obb
	{
		D3DCOLOR c = cOBB::IsCollision(m_pHoldZealot->GetOBB(), m_pMoveZealot->GetOBB()) ? D3DCOLOR_XRGB(255, 0, 0) : D3DCOLOR_XRGB(255, 255, 255);
		if (m_pHoldZealot)
			m_pHoldZealot->Render(c);

		if (m_pMoveZealot)
			m_pMoveZealot->Render(c);
	}

	char szTemp[1024];
	sprintf(szTemp, "FPS : %d", g_pTimeManager->GetFPS());

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 100, 100, 300, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));

	if(m_pUIRoot)
		m_pUIRoot->Render(m_pSprite);


	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pCamera)
	{
		m_pCamera->WndProc(hWnd, message, wParam, lParam);
	}

	switch(message)
	{
	case WM_LBUTTONDOWN:
		{
		}

		break;
	case WM_RBUTTONDOWN:
		{
		}
		
		break;
	}
}

void cMainGame::OnClick( cUIButton* pSender )
{
}
