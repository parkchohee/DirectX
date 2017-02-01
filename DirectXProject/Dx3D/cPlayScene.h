#pragma once

#include "cScene.h"

class cCamera;
class cGrid;
class cTextMap;

class cUIObject;
class cUIImageView;
class cStaticMesh;

class cPlayer;
class cAI;

class cPlayScene : public cScene
{
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;

	cStaticMesh*				m_pSkyView;
	cTextMap*					m_pTextMap;

	cUIObject*					m_pUICursorRoot;
	cUIObject*					m_pUIPlayerInfoRoot;
	cUIImageView*				m_pCompassFront;
	LPD3DXSPRITE				m_pSprite;

	cPlayer*					m_pPlayer;
	std::vector<cAI*>			m_pvAI;

public:
	cPlayScene();
	virtual ~cPlayScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void BulletCollisionCheck();

	float GetDistance(D3DXVECTOR3 BulletPos, D3DXVECTOR3 CrushManPos);
	bool IsCollision(D3DXVECTOR3 BulletPos, float BulletSphereRadius, D3DXVECTOR3 CrushManPos, float CrushManSphereRadius);

	void SettingCursorUI();
	void SettingPlayerInfoUI();
};

