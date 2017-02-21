#include "stdafx.h"
#include "cVideoScene.h"


cVideoScene::cVideoScene()
{
}


cVideoScene::~cVideoScene()
{
}

void cVideoScene::Setup()
{
	m_pintroVideo = new cVideo;
	m_pintroVideo->Play("intro.wmv", WINSIZEX / 2, WINSIZEY / 2, WINSIZEX, WINSIZEY);
	m_pintroVideo->SetVolume(200);
	m_binitComplete = false;
	m_bstartBackbuffer = false;
}

void cVideoScene::Destroy()
{
}

void cVideoScene::Update()
{
	if (g_pKeyManager->IsOnceKeyDown('1'))
	{
		m_pintroVideo->Stop();
		m_pintroVideo->Release();
		m_bstartBackbuffer = true;
		g_pSceneManager->ChangeScene("firstScene");
	}

	if (!m_bstartBackbuffer)
	{
		if (!m_pintroVideo->VideoState())
		{
			m_pintroVideo->Stop();
			m_pintroVideo->Release();
			m_bstartBackbuffer = true;
			g_pSceneManager->ChangeScene("firstScene");
		}
	}
}

void cVideoScene::Render()
{
}
