#pragma once

#define MAPSIZE 100

class cMakeGround
{
	LPD3DXMESH					m_pMesh;
	std::vector<D3DXVECTOR3>	m_vecVertex;
	D3DMATERIAL9				m_stMtl;
	LPDIRECT3DTEXTURE9			m_pTexture;
	int							m_nTileN;

	std::vector<D3DXVECTOR3>	m_vecGround;

public:
	cMakeGround();
	~cMakeGround();

	void Setup();
	void Update(POINT mouse = {0, 0});
	void Render();

	void SaveMapFile();
};

