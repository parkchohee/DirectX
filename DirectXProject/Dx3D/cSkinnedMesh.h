#pragma once

struct ST_BONE;

class cSkinnedMesh
{
	friend class cSkinnedMeshManager;

private:
	ST_BONE*					m_pRootFrame;			
	DWORD						m_dwWorkingPaletteSize;
	D3DXMATRIX*					m_pmWorkingPalette;
	LPD3DXEFFECT				m_pEffect;
	
	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXMATRIXA16				m_matWorldTM;
	D3DXMATRIXA16				m_matSaveWorldTM;

	/// >> : OBB -
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);



public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);
	
	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);

	void SetSRT(D3DXMATRIXA16& matSRT);
	D3DXMATRIXA16& GetSRT() 
	{
		return m_matWorldTM;
	}

	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pmat)
	{
		m_matWorldTM = *pmat;
	}
private:
	cSkinnedMesh();
	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();
};

