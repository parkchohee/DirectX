#pragma once
#include "cCharacter.h"

class cGun;
class cController;

class cPlayer : public cGameObject
{
	cGun*				m_pGun;
	cController*		m_pController;

	D3DXVECTOR3			m_vCamAngle;
	
public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle, iMap* pMap = NULL);
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

