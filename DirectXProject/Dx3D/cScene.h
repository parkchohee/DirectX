#pragma once
class cScene
{
public:
	cScene();
	virtual ~cScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};
