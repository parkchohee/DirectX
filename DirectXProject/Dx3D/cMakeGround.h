#pragma once

#define MAPSIZE 129
class cUIObject;
class cMakeGround
{
	LPD3DXMESH					m_pMesh;
	std::vector<ST_PNT_VERTEX>	m_vecVertex;
	std::vector<DWORD>			m_vecIndex;
	D3DMATERIAL9				m_stMtl;
	LPDIRECT3DTEXTURE9			m_pTexture;
	int							m_nTileN;

	std::vector<D3DXVECTOR3>	m_vecGround;

	LPD3DXSPRITE				m_pSprite;
	cUIObject*					m_pUIRoot;

public:
	cMakeGround();
	~cMakeGround();

	void Setup();
	void Update(POINT mouse = {0, 0});
	void Render();

	void SetNormal();
	void SettingUI();
	void SaveMapFile();
};

