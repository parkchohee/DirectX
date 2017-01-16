#include "StdAfx.h"
#include "cMainGame.h"
#include "cPlayScene.h"

cMainGame::cMainGame(void)
	: m_pPlayScene(NULL)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pPlayScene);

	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//ShowCursor(FALSE);
	
	m_pPlayScene = new cPlayScene;
	m_pPlayScene->Setup();

	SetLight();

}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	if(m_pPlayScene)
		m_pPlayScene->Update();

}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(47, 121, 112),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	if (m_pPlayScene)
		m_pPlayScene->Render();


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


	if (m_pPlayScene)
	{
		m_pPlayScene->WndProc(hWnd, message, wParam, lParam);
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
