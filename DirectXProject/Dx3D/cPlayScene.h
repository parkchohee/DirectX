#pragma once

class cCamera;
class cGrid;
class cUIObject;

class cPlayer;
class cAI;

class cPlayScene
{
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;

	cUIObject*					m_pUIRoot;
	LPD3DXSPRITE				m_pSprite;

	cPlayer*					m_pPlayer;

	cAI*						m_pAI;
	cAI*	test;


public:
	cPlayScene();
	~cPlayScene();

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

