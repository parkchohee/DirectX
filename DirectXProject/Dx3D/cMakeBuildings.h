#pragma once

#define MAPMAXSIZE 4096

class cBuilding;
class cUIObject;
class cStaticMesh;

#include "cUIButton.h"

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

class cMakeBuildings : public iButtonDelegate
{
	cBuilding*					m_pBuilding;

	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;

	std::vector<cBuilding*>		m_vpAllBuildings;
	std::vector<cBuilding*>		m_vpSettingBuildings;

	std::vector<char*>			m_pName;
	int							m_nBuildingNum;

public:
	cMakeBuildings();
	~cMakeBuildings();

	void Setup();
	void Update();
	void Render();

	void OnClick(cUIButton* pSender);

	void SettingUI();
	void PositionSettingController();

	void AddBuilding();
	void SaveMapFile();
};

