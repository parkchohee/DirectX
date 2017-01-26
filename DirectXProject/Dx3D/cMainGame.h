#pragma once


class cScene;
#include "cUIButton.h"

enum SCENE_STATE
{
	FIRST_SCENE,
	PLAY_SCENE,
	MAP_TOOL_SCENE
};

class cMainGame
	: public iButtonDelegate
{
private:
	cScene*					m_pFirstScene;
	cScene*					m_pPlayScene;
	cScene*					m_pMapTool;

	SCENE_STATE				m_nSceneState;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();
	void SetLight();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;



};

