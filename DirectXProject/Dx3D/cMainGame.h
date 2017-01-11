#pragma once

class cCamera;
class cGrid;
class cZealot;

#include "cUIButton.h"

class cMainGame
	: public iButtonDelegate
{
private:
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;
	

	cZealot*					m_pHoldZealot;

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

