#pragma once
#include "cScene.h"
#include "cUIButton.h"

class cUIObject;

enum MAIN_MENU_STATE {
	MAIN_MENU,
	OPTION_MENU,
	SOUND_MENU
};

enum MAIN_MENU_BUTTON {
	CAMPAIN_BTN,
	OPTION_BTN,
	QUIT_BTN,
	AUDIO_BTN,
	MAP_BTN,
	BACK_BTN
};

class cFirstScene : public cScene
	, public iButtonDelegate
{
	cUIObject*			m_pMainUIRoot;
	cUIObject*			m_pOptionUIRoot;
	cUIObject*			m_pSoundUIRoot;
	LPD3DXSPRITE		m_pSprite;

	MAIN_MENU_STATE		m_stMenu;

public:
	cFirstScene();
	virtual ~cFirstScene();

	virtual void Setup();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void OnClick(cUIButton * pSender);

	void MainMenuUISetting();
	void OptionMenuUISetting();
	void SoundMenuSetting();
};

