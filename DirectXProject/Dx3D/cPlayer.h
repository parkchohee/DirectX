#pragma once

class cGun;
class cUIObject;
class cController;

class cPlayer : public cGameObject
{
	cGun*				m_pGun;

	cUIObject*			m_pUIRoot;
	cController*		m_pController;
	LPD3DXSPRITE		m_pSprite;

	D3DXVECTOR3			m_vCamAngle;
	//D3DXVECTOR3			m_vDirection;
	//SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vPosition, Position);

public:
	cPlayer();
	~cPlayer();

	void Setup();
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

};

