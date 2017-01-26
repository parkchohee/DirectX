#pragma once
#include "cScene.h"

class cFirstScene : public cScene
{
public:
	cFirstScene();
	virtual ~cFirstScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

