#include "stdafx.h"
#include "cFirstScene.h"
#include "cUIImageView.h"


cFirstScene::cFirstScene()
	: m_pMainUIRoot(NULL)
	, m_pOptionUIRoot(NULL)
	, m_pSprite(NULL)
	, m_stMenu(MAIN_MENU)
{
}


cFirstScene::~cFirstScene()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Destroy();

	if (m_pOptionUIRoot)
		m_pOptionUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
}

void cFirstScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	MainMenuUISetting();
	OptionMenuUISetting();
}

void cFirstScene::Update()
{
	switch (m_stMenu)
	{
	case MAIN_MENU:
		if (m_pMainUIRoot)
			m_pMainUIRoot->Update();
		break;
	case OPTION_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Update();
		break;
	default:
		break;
	}
}

void cFirstScene::Render()
{
	switch (m_stMenu)
	{
	case MAIN_MENU:
		if (m_pMainUIRoot)
			m_pMainUIRoot->Render(m_pSprite);
		break;
	case OPTION_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		break;
	default:
		break;
	}
}

void cFirstScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
}

void cFirstScene::OnClick(cUIButton * pSender)
{
	switch (pSender->GetTag())
	{
	case CAMPAIN_BTN:
		g_pSceneManager->ChangeScene("playScene");
		break;
	case OPTION_BTN:
		m_stMenu = OPTION_MENU;
		break;
	case QUIT_BTN:
		exit(0);
		break;
	case AUDIO_BTN:
		break;
	case MAP_BTN:
		g_pSceneManager->ChangeScene("mapTool");
		break;
	case BACK_BTN:
		m_stMenu = MAIN_MENU;
		break;
	}
}

void cFirstScene::MainMenuUISetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pMainUIRoot = new cUIObject;
	m_pMainUIRoot->SetPosition(0, 0);

	cUIImageView* MainMenuBackground = new cUIImageView;
	MainMenuBackground->SetTexture("MainMenuUI/main_1.png");
	MainMenuBackground->SetPosition(MainMenuBackground->GetSize().nWidth / 2, MainMenuBackground->GetSize().nHeight / 2);

	cUIImageView* MainMenuImg = new cUIImageView;
	MainMenuImg->SetTexture("MainMenuUI/main_menu.png");
	MainMenuImg->SetPosition(rc.left + MainMenuImg->GetSize().nWidth / 2+ 30, rc.top + 50);

	cUIButton* BtnCampain = new cUIButton;
	BtnCampain->SetTexture("MainMenuUI/campain_black.png", "MainMenuUI/campain_white.png", "MainMenuUI/campain_white.png");
	BtnCampain->SetTag(CAMPAIN_BTN);
	BtnCampain->SetPosition(rc.right - BtnCampain->GetSize().nWidth - 50, rc.top + 250);
	BtnCampain->SetDelegate(this);

	cUIButton* BtnOption = new cUIButton;
	BtnOption->SetTexture("MainMenuUI/option_black.png", "MainMenuUI/option_white.png", "MainMenuUI/option_white.png");
	BtnOption->SetTag(OPTION_BTN);
	BtnOption->SetPosition(rc.right - BtnOption->GetSize().nWidth - 50, rc.top + 300);
	BtnOption->SetDelegate(this);

	cUIButton* BtnQuit = new cUIButton;
	BtnQuit->SetTexture("MainMenuUI/quit_black.png", "MainMenuUI/quit_white.png", "MainMenuUI/quit_white.png");
	BtnQuit->SetTag(QUIT_BTN);
	BtnQuit->SetPosition(rc.right - BtnQuit->GetSize().nWidth - 50, rc.top + 350);
	BtnQuit->SetDelegate(this);

	m_pMainUIRoot->AddChild(MainMenuBackground);
	m_pMainUIRoot->AddChild(MainMenuImg);
	m_pMainUIRoot->AddChild(BtnCampain);
	m_pMainUIRoot->AddChild(BtnOption);
	m_pMainUIRoot->AddChild(BtnQuit);
	
}

void cFirstScene::OptionMenuUISetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pOptionUIRoot = new cUIObject;
	m_pOptionUIRoot->SetPosition(0, 0);
	
	cUIImageView* OptionBackground = new cUIImageView;
	OptionBackground->SetTexture("MainMenuUI/main_2.png");
	OptionBackground->SetPosition(OptionBackground->GetSize().nWidth / 2, OptionBackground->GetSize().nHeight / 2);

	cUIImageView* OptionImg = new cUIImageView;
	OptionImg->SetTexture("MainMenuUI/option_white.png");
	OptionImg->SetPosition(rc.left + OptionImg->GetSize().nWidth / 2 + 30, rc.top + 50);

	cUIButton* BtnAudio = new cUIButton;
	BtnAudio->SetTexture("MainMenuUI/audio_black.png", "MainMenuUI/audio_white.png", "MainMenuUI/audio_white.png");
	BtnAudio->SetTag(AUDIO_BTN);
	BtnAudio->SetPosition(rc.left + 30, 100);
	BtnAudio->SetDelegate(this);

	cUIButton* BtnMap = new cUIButton;
	BtnMap->SetTexture("MainMenuUI/map_black.png", "MainMenuUI/map_white.png", "MainMenuUI/map_white.png");
	BtnMap->SetTag(MAP_BTN);
	BtnMap->SetPosition(rc.left + 30, 150);
	BtnMap->SetDelegate(this);
	
	cUIButton* BtnBack = new cUIButton;
	BtnBack->SetTexture("MainMenuUI/back_black.png", "MainMenuUI/back_white.png", "MainMenuUI/back_white.png");
	BtnBack->SetTag(BACK_BTN);
	BtnBack->SetPosition(rc.left + 30, 200);
	BtnBack->SetDelegate(this);
	
	m_pOptionUIRoot->AddChild(OptionBackground);
	m_pOptionUIRoot->AddChild(OptionImg);
	m_pOptionUIRoot->AddChild(BtnAudio);
	m_pOptionUIRoot->AddChild(BtnMap);
	m_pOptionUIRoot->AddChild(BtnBack);

}
