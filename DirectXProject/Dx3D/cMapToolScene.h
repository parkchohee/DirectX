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
	SAVE,
	EXIT
};

class cBuilding;
class cGrid;
class cCamera;
class cUIObject;
class cStaticMesh;

#include "cUIButton.h"
class cMapToolScene : public cScene,
	public iButtonDelegate
{
	cBuilding*					m_pBuilding;

	cCamera*					m_pCamera;
	cGrid*						m_pGrid;
	cStaticMesh*				m_pSkyView;

	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;

	std::vector<cBuilding*>		m_vpAllBuildings;
	std::vector<cBuilding*>		m_vpSettingBuildings;

	std::vector<char*>			m_pName;
	int							m_nBuildingNum;

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

	void AddBuilding();
	void SaveMapFile();
};

