#include "StdAfx.h"
#include "cMainGame.h"
#include "cScene.h"
#include "cFirstScene.h"
#include "cPlayScene.h"
#include "cMapToolScene.h"

cMainGame::cMainGame(void)
	: m_pFirstScene(NULL)
	, m_pPlayScene(NULL)
	, m_pMapTool(NULL)
	, m_nSceneState(MAP_TOOL_SCENE)
{
}


cMainGame::~cMainGame(void)
{
	SAFE_DELETE(m_pMapTool);
	SAFE_DELETE(m_pPlayScene);
	SAFE_DELETE(m_pFirstScene);

	g_pSceneManager->Destroy();
	g_pStaticMeshManager->Destroy();
	g_pSkinnedMeshManager->Destroy();
	g_pObjectManager->Destroy();
	g_pFontManager->Destroy();
	g_pTextureManager->Destroy();
	g_pDeviceManager->Destroy();
}

void cMainGame::Setup()
{
	//ShowCursor(FALSE);
	
	/*m_pFirstScene = new cFirstScene;
	m_pFirstScene->Setup();

	m_pPlayScene = new cPlayScene;
	m_pPlayScene->Setup();

	m_pMapTool = new cMapToolScene;
	m_pMapTool->Setup();
*/
	g_pSceneManager->AddScene("mapTool", new cMapToolScene);
	g_pSceneManager->AddScene("playScene", new cPlayScene);
	g_pSceneManager->AddScene("firstScene", new cFirstScene);

	g_pSceneManager->ChangeScene("mapTool");

	SetLight();

}

void cMainGame::Update()
{
	g_pTimeManager->Update();

	g_pSceneManager->Update(g_pTimeManager->GetElapsedTime());
	
	/*switch (m_nSceneState)
	{
	case FIRST_SCENE:
		if (m_pFirstScene)
			m_pFirstScene->Update();
		break;
	case PLAY_SCENE:
		if (m_pPlayScene)
			m_pPlayScene->Update();
		break;
	case MAP_TOOL_SCENE:
		if (m_pMapTool)
			m_pMapTool->Update();
		break;
	default:
		break;
	}
	
	if(g_pKeyManager->IsOnceKeyDown(VK_RETURN))
		m_nSceneState = PLAY_SCENE;
*/
	if (g_pKeyManager->IsOnceKeyDown(VK_RETURN))
		g_pSceneManager->ChangeScene("firstScene");

	if (g_pKeyManager->IsOnceKeyDown(VK_SPACE))
		g_pSceneManager->ChangeScene("playScene");
}

void cMainGame::Render()
{
	g_pD3DDevice->Clear(NULL,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		//D3DCOLOR_XRGB(47, 121, 112),
		D3DCOLOR_XRGB(0, 0, 0),
		1.0f, 0);

	g_pD3DDevice->BeginScene();

	g_pSceneManager->Render();

	//switch (m_nSceneState)
	//{
	//case FIRST_SCENE:
	//	if (m_pFirstScene)
	//		m_pFirstScene->Render();
	//	break;
	//case PLAY_SCENE:
	//	if (m_pPlayScene)
	//		m_pPlayScene->Render();
	//	break;
	//case MAP_TOOL_SCENE:
	//	if (m_pMapTool)
	//		m_pMapTool->Render();
	//	break;
	//default:
	//	break;
	//}



	char szTemp[1024];
	sprintf(szTemp, "FPS : %d", g_pTimeManager->GetFPS());

	LPD3DXFONT pFont = g_pFontManager->GetFont(cFontManager::E_DEFAULT);
	RECT rc;
	SetRect(&rc, 0, 0, 300, 300);
	pFont->DrawText(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_TOP | DT_WORDBREAK,
		D3DCOLOR_XRGB(255, 255, 0));

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
	g_pSceneManager->WndProc(hWnd, message, wParam, lParam);

	/*switch (m_nSceneState)
	{
	case FIRST_SCENE:
		if (m_pFirstScene)
			m_pFirstScene->WndProc(hWnd, message, wParam, lParam);
		break;
	case PLAY_SCENE:
		if (m_pPlayScene)
			m_pPlayScene->WndProc(hWnd, message, wParam, lParam);
		break;
	case MAP_TOOL_SCENE:
		if (m_pMapTool)
			m_pMapTool->WndProc(hWnd, message, wParam, lParam);
		break;
	default:
		break;
	}*/


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
