#include "StdAfx.h"
#include "cMainGame.h"
#include "cCamera.h"
#include "cGrid.h"
#include "cPlayer.h"

DWORD FtoDw(float f)
{
	return *((DWORD*)&f);
}

cMainGame::cMainGame(void)
	: m_pCamera(NULL)
	, m_pGrid(NULL)
	, m_pPlayer(NULL)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pCamera);
	SAFE_DELETE(m_pGrid);

	SAFE_RELEASE(m_pPlayer);

	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	ShowCursor(FALSE);

	m_pPlayer = new cPlayer;
	m_pPlayer->Setup();
	
	m_pCamera = new cCamera;
	m_pCamera->Setup(&(m_pPlayer->GetPosition()));

	m_pGrid = new cGrid;
	m_pGrid->Setup();

	SetLight();

	Setup_Particle();
}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if (m_pPlayer && m_pCamera)
		m_pPlayer->Update(m_pCamera->GetCamRotAngle());

	if (m_pCamera)
		m_pCamera->Update();

	Update_Particle(); 	/// >> : Particle Update
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
	
	if (m_pPlayer)
		m_pPlayer->Render();

	Render_Particle(); /// : particle

	g_pD3DDevice->EndScene();

	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void cMainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));
	stLight.Type = D3DLIGHT_DIRECTIONAL;
	stLight.Ambient = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	stLight.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	D3DXVECTOR3 vDir(1.0f, -1.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;
	g_pD3DDevice->SetLight(0, &stLight);
	g_pD3DDevice->LightEnable(0, true);

	g_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
}

void cMainGame::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if (m_pPlayer)
	{
		m_pPlayer->WndProc(hWnd, message, wParam, lParam);
	}

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






void cMainGame::Setup_Particle()
{
	//m_vecParticle.resize(1000);

	/// >> : 파티클 랜덤하게 생성
	m_vecVertex.resize(1000);
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		float fRadius = rand() % 100 / 10.0f;

		m_vecVertex[i].p = D3DXVECTOR3(0, 0, fRadius);


		D3DXVECTOR3 vAngle = D3DXVECTOR3(
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f),
			D3DXToRadian(rand() % 3600 / 10.0f));

		D3DXMATRIX matRX, matRY, matRZ, matWorld;
		D3DXMatrixRotationX(&matRX, vAngle.x);
		D3DXMatrixRotationY(&matRY, vAngle.y);
		D3DXMatrixRotationZ(&matRZ, vAngle.z);
		matWorld = matRX * matRY * matRZ;

		D3DXVec3TransformCoord(
			&m_vecVertex[i].p,
			&m_vecVertex[i].p,
			&matWorld);

		m_vecVertex[i].c = D3DCOLOR_ARGB(255, 180, 70, 20);
		//m_vecParticle[i]->m_pVertex = &m_vecVertex[i];
	}


	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);	/// >> : 포인트 확대 여부 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE, FtoDw(5.f));	/// >> : 포인트 사이즈 설정 

																/// : 기본 도형에 대한 거리 사이즈의 감소를 제어 , 포인트 스케일링 Factor값 설정
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_A, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_B, FtoDw(0.0f));
	g_pD3DDevice->SetRenderState(D3DRS_POINTSCALE_C, FtoDw(1.0f));	/// : vs 10.0f check 

	g_pD3DDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true); /// : 포인트에 텍스쳐를 입힐 수 있게 해줌
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDw(0.0f));	/// : 포인트 최소 크기
	g_pD3DDevice->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDw(10.0f));	/// : 포인트 최대 크기 vs 10.0f check 

																		// 텍스쳐 알파 옵션 설정
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정.
	g_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}


/// : Update Particle
void cMainGame::Update_Particle()
{
	static int nAlpha = 0;
	static int nDelta = 4;
	nAlpha += nDelta;
	if (nAlpha > 255)
	{
		nAlpha = 255;
		nDelta *= -1;
	}
	if (nAlpha < 0)
	{
		nAlpha = 0;
		nDelta *= -1;
	}
	for (int i = 0; i < m_vecVertex.size(); ++i)
	{
		if (i % 2)
			continue;
		m_vecVertex[i].c = D3DCOLOR_ARGB(nAlpha, 180, 70, 20); /// : 알파값만 변경
	}
}

/// : particle
void cMainGame::Render_Particle()
{
	D3DXMATRIXA16 matWorld;
	D3DXMatrixIdentity(&matWorld);
	g_pD3DDevice->SetTransform(D3DTS_WORLD, &matWorld);

	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, false);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, false);

	g_pD3DDevice->SetFVF(ST_PC_VERTEX::FVF);
	g_pD3DDevice->SetTexture(0, g_pTextureManager->GetTexture("alpha_tex.tga"));
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_POINTLIST,
		m_vecVertex.size(),
		&m_vecVertex[0],
		sizeof(ST_PC_VERTEX));

	g_pD3DDevice->SetRenderState(D3DRS_ZWRITEENABLE, true);
	g_pD3DDevice->SetRenderState(D3DRS_LIGHTING, true);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}