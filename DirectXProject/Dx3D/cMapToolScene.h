#pragma once
#include "cScene.h"

class cStaticMesh;
class cGrid;

class cMapToolScene : public cScene
{
	cStaticMesh* test;


	cGrid*		m_pGrid;

public:
	cMapToolScene();
	virtual ~cMapToolScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

