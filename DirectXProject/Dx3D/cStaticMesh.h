#pragma once
class cStaticMesh
{
	friend class cStaticMeshManager;

private:
	ID3DXMesh*							m_pStaticMesh;
	std::vector<D3DMATERIAL9>			m_vMtrls;
	std::vector<IDirect3DTexture9*>		m_vTexture;

	SYNTHESIZE_PASS_BY_REF(D3DXMATRIXA16, m_pmatWorld, World);

	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

public:
	cStaticMesh();
	cStaticMesh(char* szDirectory, char* szFilename);
	~cStaticMesh();

	bool Setup(char* szDirectory, char* szFilename);
	void Render();
	void Destroy();
};

