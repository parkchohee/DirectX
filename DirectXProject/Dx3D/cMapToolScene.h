#pragma once
#include "cScene.h"

#define MAPMAXSIZE 4096

enum BUTTON_TYPE
{
	PREV_TITLE,
	NEXT_TITLE,
	DOWN_SCALE,
	UP_SCALE,
	DOWN_ANGLE,
	UP_ANGLE,
	SAVE
};

class cBuilding;
class cGrid;
class cCamera;
class cUIObject;

#include "cUIButton.h"
class cMapToolScene : public cScene,
	public iButtonDelegate
{
	cBuilding*					m_pBuilding;

	cCamera*					m_pCamera;
	cGrid*						m_pGrid;

	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;

	std::vector<cBuilding*>		m_vpBuildings;

public:
	cMapToolScene();
	virtual ~cMapToolScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	void OnClick(cUIButton* pSender);
	
	void SettingUI();
	void PositionSettingController();
	void SaveMapFile();
};

