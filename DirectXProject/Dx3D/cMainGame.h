#pragma once

class cCamera;
class cGrid;
class iMap;
class cFrame;
class cMtlTex;
class cGroup;
class cZealot;
class cFrustum;

#include "cUIButton.h"

class cMainGame
	: public iButtonDelegate
{
private:
	cCamera*					m_pCamera;
	cGrid*						m_pGrid;
	iMap*						m_pMap;
	cFrame*						m_pRootFrame;
	LPD3DXFONT					m_pFont;
	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;
	std::vector<ST_PN_VERTEX>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;
	D3DMATERIAL9				m_stMtlPicked;
	LPD3DXMESH					m_pMesh;
	D3DXVECTOR3					m_vPickedPosition;
	std::vector<ST_SPHERE>		m_vecSphere;
	LPD3DXMESH					m_pSphere;
	LPD3DXMESH					m_pObjMesh;
	std::vector<cMtlTex*>		m_vecMtlTex;
	std::vector<cGroup*>		m_vecGroup;
					
	cFrustum*					m_pFrustum;

	cZealot*					m_pHoldZealot;
	cZealot*					m_pMoveZealot;

public:
	cMainGame(void);
	~cMainGame(void);

	void Setup();
	void Update();
	void Render();

	void WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	//iButtonDelegate override
	virtual void OnClick(cUIButton* pSender) override;
};

