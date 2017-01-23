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
	std::vector<cAI*>			m_pvAI;
	//cAI*						m_pAI;

public:
	cPlayScene();
	~cPlayScene();

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void CollisionCheck();
	float GetDistance(D3DXVECTOR3 BulletPos, D3DXVECTOR3 CrushManPos);
	bool IsCollision(D3DXVECTOR3 BulletPos, float BulletSphereRadius, D3DXVECTOR3 CrushManPos, float CrushManSphereRadius);

};

