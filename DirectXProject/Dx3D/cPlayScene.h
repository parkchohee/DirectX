#pragma once

#include "cScene.h"

class cCamera;
class cGrid;
class cTextMap;
class cHeightMap;

class cUIObject;
class cUIImageView;
class cUITextView;
class cStaticMesh;

class cPlayer;
class cAI;
//class cEffect;

class cPlayScene : public cScene
{
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;

	cStaticMesh*				m_pSkyView;
	cTextMap*					m_pTextMap;
	cHeightMap*					m_pHeightMap;

	cUIObject*					m_pUICursorRoot;
	cUIObject*					m_pUIPlayerInfoRoot;
	cUIImageView*				m_pCompassFront;
	cUITextView*				m_pBulletText;
	LPD3DXSPRITE				m_pSprite;

	cPlayer*					m_pPlayer;
	std::vector<cAI*>			m_pvAI;
	std::vector<cAI*>			m_pvDeathAI;

//	cEffect*					test;
	//std::vector<cEffect*>		m_pvEffect;

public:
	cPlayScene();
	virtual ~cPlayScene();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

	virtual void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void PlayerBulletFire();

	float GetDistance(D3DXVECTOR3 BulletPos, D3DXVECTOR3 CrushManPos);
	bool IsCollision(D3DXVECTOR3 BulletPos, float BulletSphereRadius, D3DXVECTOR3 CrushManPos, float CrushManSphereRadius);

	void SettingCursorUI();
	void SettingPlayerInfoUI();
};

