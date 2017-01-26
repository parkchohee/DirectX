#pragma once

class cStaticMesh
{
private:
	ID3DXMesh*							m_pStaticMesh;
	std::vector<D3DMATERIAL9>			m_vMtrls;
	std::vector<IDirect3DTexture9*>		m_vTexture;

	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_pmatWorld, Wolrd);

public:
	cStaticMesh();
	~cStaticMesh();

	bool Setup(char* szDirectory, char* szFilename);
	void Render();
};

