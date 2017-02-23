#include "stdafx.h"
#include "cFirstScene.h"
#include "cUIImageView.h"


cFirstScene::cFirstScene()
	: m_pMainUIRoot(NULL)
	, m_pOptionUIRoot(NULL)
	, m_pSprite(NULL)
	, m_pMedalUIRoot(NULL)
	, m_stMenu(MAIN_MENU)
{
}


cFirstScene::~cFirstScene()
{

}

void cFirstScene::Setup()
{
	D3DXCreateSprite(g_pD3DDevice, &m_pSprite);

	LoadHoner("Guntext.txt");
	MainMenuUISetting();
	OptionMenuUISetting();
	SoundMenuSetting();
	MedalMenuSetting();
}

void cFirstScene::Destroy()
{
	if (m_pMainUIRoot)
		m_pMainUIRoot->Destroy();

	if (m_pOptionUIRoot)
		m_pOptionUIRoot->Destroy();

	if (m_pSoundUIRoot)
		m_pSoundUIRoot->Destroy();

	if (m_pMedalUIRoot)
		m_pMedalUIRoot->Destroy();

	SAFE_RELEASE(m_pSprite);
}

void cFirstScene::Update()
{
	switch (m_stMenu)
	{
	case MAIN_MENU:
		if (m_pMainUIRoot)
			m_pMainUIRoot->Update();
		break;
	case SOUND_MENU:
		if (m_pSoundUIRoot)
			m_pSoundUIRoot->Update();
	case OPTION_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Update();
		break;
	case MEDAL_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Update();
		if (m_pMedalUIRoot)
			m_pMedalUIRoot->Update();
		break;
	default:
		break;
	}



	SoundBarControl();


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
	case SOUND_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		if (m_pSoundUIRoot)
			m_pSoundUIRoot->Render(m_pSprite);
		break;
	case MEDAL_MENU:
		if (m_pOptionUIRoot)
			m_pOptionUIRoot->Render(m_pSprite);
		if (m_pMedalUIRoot)
			m_pMedalUIRoot->Render(m_pSprite);
		break;
	default:
		break;
	}
}

void cFirstScene::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);
		m_bIsLButtonDown = true;
		break;
	case WM_LBUTTONUP:
		m_bIsLButtonDown = false;
	case WM_MOUSEMOVE:
		m_ptMouse.x = LOWORD(lParam);
		m_ptMouse.y = HIWORD(lParam);

	}
}

void cFirstScene::OnClick(cUIButton * pSender)
{
	switch (pSender->GetTag())
	{
	case CAMPAIN_BTN:
		g_pSceneManager->ChangeSceneWithLoading("playScene", "loadingScene");
		break;
	case OPTION_BTN:
		m_stMenu = OPTION_MENU;
		break;
	case QUIT_BTN:
		exit(0);
		break;
	case AUDIO_BTN:
		if (m_stMenu == SOUND_MENU)
			m_stMenu = OPTION_MENU;
		else
			m_stMenu = SOUND_MENU;
		break;
	case MEDAL_BTN:

		m_stMenu = MEDAL_MENU;
		break;
	case BACK_BTN:
		m_stMenu = MAIN_MENU;
		break;
	}




	g_pSoundManager->play("MenuSelect", 0.5f * g_pSoundManager->GetSoundVol());
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
	MainMenuImg->SetPosition(rc.left + MainMenuImg->GetSize().nWidth / 2 + 30, rc.top + 50);

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

	cUIButton* BtnMedal = new cUIButton;
	BtnMedal->SetTexture("MainMenuUI/medal_black.png", "MainMenuUI/medal_white.png", "MainMenuUI/medal_white.png");
	BtnMedal->SetTag(MEDAL_BTN);
	BtnMedal->SetPosition(rc.left + 30, 150);
	BtnMedal->SetDelegate(this);

	cUIButton* BtnBack = new cUIButton;
	BtnBack->SetTexture("MainMenuUI/back_black.png", "MainMenuUI/back_white.png", "MainMenuUI/back_white.png");
	BtnBack->SetTag(BACK_BTN);
	BtnBack->SetPosition(rc.left + 30, 200);
	BtnBack->SetDelegate(this);

	m_pOptionUIRoot->AddChild(OptionBackground);
	m_pOptionUIRoot->AddChild(OptionImg);
	m_pOptionUIRoot->AddChild(BtnAudio);
	m_pOptionUIRoot->AddChild(BtnMedal);
	m_pOptionUIRoot->AddChild(BtnBack);

}

void cFirstScene::SoundMenuSetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pSoundUIRoot = new cUIObject;
	m_pSoundUIRoot->SetPosition(0, 0);


	m_pSoundSlider = new cUIImageView;
	m_pSoundSlider->SetTexture("MainMenuUI/VolCast_off.png");
	m_pSoundSlider->SetPosition(rc.right - 200, (rc.top + rc.bottom) / 2);

	m_pSoundSlideBar = new cUIImageView;
	m_pSoundSlideBar->SetTexture("MainMenuUI/VolScaleButton_off.png");
	m_pSoundSlideBar->SetPosition(rc.right - 200, (rc.top + rc.bottom) / 2);


	m_rSliderRect = { (LONG)(m_pSoundSlider->GetPosition().x - m_pSoundSlider->GetSize().nWidth / 2 + 15),
		(LONG)(m_pSoundSlider->GetPosition().y - m_pSoundSlider->GetSize().nHeight / 2),
		(LONG)(m_pSoundSlider->GetPosition().x + m_pSoundSlider->GetSize().nWidth - 190) ,
		(LONG)(m_pSoundSlider->GetPosition().y + m_pSoundSlider->GetSize().nHeight) };

	m_pSoundUIRoot->AddChild(m_pSoundSlideBar);
	m_pSoundUIRoot->AddChild(m_pSoundSlider);
}

void cFirstScene::MedalMenuSetting()
{
	RECT rc;
	GetClientRect(g_hWnd, &rc);

	m_pMedalUIRoot = new cUIObject;
	m_pMedalUIRoot->SetPosition(0, 0);

	cUIImageView* Collection = new cUIImageView;
	Collection->SetTexture("MainMenuUi/Collection.png");
	Collection->SetPosition(rc.left + 600, rc.top + 400);

	cUIImageView * ShotGunImage = new cUIImageView;
	ShotGunImage->SetTexture("MainMenuUI/shotgun.png");
	ShotGunImage->SetPosition(rc.left + 300, rc.top + 300);

	cUIImageView * WincImage = new cUIImageView;
	WincImage->SetTexture("MainMenuUI/winc.png");
	WincImage->SetPosition(rc.left + 300, rc.top + 450);

	cUIImageView * NinemmImage = new cUIImageView;
	NinemmImage->SetTexture("MainMenuUI/9mm.png");
	NinemmImage->SetPosition(rc.left + 300, rc.top + 600);

	m_pMedalUIRoot->AddChild(Collection);
	m_pMedalUIRoot->AddChild(ShotGunImage);
	m_pMedalUIRoot->AddChild(WincImage);
	m_pMedalUIRoot->AddChild(NinemmImage);

	if (GunLv[2] >= 1)
	{
		m_pNinemmLv1 = new cUIImageView;
		m_pNinemmLv1->SetTexture("MainMenuUI/9mmlv1.tga");
		m_pNinemmLv1->SetPosition(rc.left + 500, rc.top + 600);
		m_pMedalUIRoot->AddChild(m_pNinemmLv1);
	}
	if (GunLv[2] >= 2)
	{
		m_pNinemmLv2 = new cUIImageView;
		m_pNinemmLv2->SetTexture("MainMenuUI/9mmlv2.tga");
		m_pNinemmLv2->SetPosition(rc.left + 700, rc.top + 600);
		m_pMedalUIRoot->AddChild(m_pNinemmLv2);

	}
	if (GunLv[2] >= 3)
	{
		m_pNinemmLv3 = new cUIImageView;
		m_pNinemmLv3->SetTexture("MainMenuUI/9mmlv3.tga");
		m_pNinemmLv3->SetPosition(rc.left + 900, rc.top + 600);
		m_pMedalUIRoot->AddChild(m_pNinemmLv3);
	}
	if (GunLv[0] >= 1)
	{
		m_pShotgunLv1 = new cUIImageView;
		m_pShotgunLv1->SetTexture("MainMenuUI/shotgunlv1.tga");
		m_pShotgunLv1->SetPosition(rc.left + 500, rc.top + 300);
		m_pMedalUIRoot->AddChild(m_pShotgunLv1);
	}
	if (GunLv[0] >= 2)
	{
		m_pShotgunLv2 = new cUIImageView;
		m_pShotgunLv2->SetTexture("MainMenuUI/shotgunlv2.tga");
		m_pShotgunLv2->SetPosition(rc.left + 700, rc.top + 300);
		m_pMedalUIRoot->AddChild(m_pShotgunLv2);
	}
	if (GunLv[0] >= 3)
	{
		m_pShotgunLv3 = new cUIImageView;
		m_pShotgunLv3->SetTexture("MainMenuUI/shotgunlv3.tga");
		m_pShotgunLv3->SetPosition(rc.left + 900, rc.top + 300);
		m_pMedalUIRoot->AddChild(m_pShotgunLv3);
	}
	if (GunLv[1] >= 1)
	{
		m_pWincLv1 = new cUIImageView;
		m_pWincLv1->SetTexture("MainMenuUI/winclv1.tga");
		m_pWincLv1->SetPosition(rc.left + 500, rc.top + 450);
		m_pMedalUIRoot->AddChild(m_pWincLv1);
		int b = 0;
	}
	if (GunLv[1] >= 2)
	{
		m_pWincLv2 = new cUIImageView;
		m_pWincLv2->SetTexture("MainMenuUI/winclv2.tga");
		m_pWincLv2->SetPosition(rc.left + 700, rc.top + 450);
		m_pMedalUIRoot->AddChild(m_pWincLv2);
	}
	if (GunLv[2] >= 3)
	{
		m_pWincLv3 = new cUIImageView;
		m_pWincLv3->SetTexture("MainMenuUI/winclv3.tga");
		m_pWincLv3->SetPosition(rc.left + 900, rc.top + 450);
		m_pMedalUIRoot->AddChild(m_pWincLv3);
	}






}

void cFirstScene::SoundBarControl()
{
	if (m_bIsLButtonDown)
	{
		if (PtInRect(&m_rSliderRect, m_ptMouse))
		{
			RECT rc;
			GetClientRect(g_hWnd, &rc);
			m_pSoundSlideBar->SetPosition(m_ptMouse.x, m_pSoundSlideBar->GetPosition().y);

		}
	}


	g_pSoundManager->SetSoundVol((m_pSoundSlideBar->GetPosition().x - 827) / 311); // 0 ~ 1.f  »ç¿îµå º¼·ý



}

void cFirstScene::LoadHoner(char * szFilename)
{

	FILE* fp = NULL;
	fopen_s(&fp, szFilename, "r");
	int lvtemp;

	while (true)
	{
		if (feof(fp))break;

		char szTemp[1024] = { '\0', };
		fgets(szTemp, 1024, fp);

		if (szTemp[0] == 'S')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %*c %*c %*c %*c %d", &lvtemp);
			GunLv[0] = lvtemp;
		}
		if (szTemp[0] == 'W')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %*c %d", &lvtemp);
			GunLv[1] = lvtemp;
		}
		if (szTemp[0] == '9')
		{

			sscanf_s(szTemp, "%*c %*c %*c %*c %*c %*c %d", &lvtemp);
			GunLv[2] = lvtemp;
		}
	}


	fclose(fp);

}