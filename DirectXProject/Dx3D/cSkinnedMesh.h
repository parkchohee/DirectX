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

	/// >> : OBB -
	SYNTHESIZE(D3DXVECTOR3, m_vMin, Min);
	SYNTHESIZE(D3DXVECTOR3, m_vMax, Max);

	SYNTHESIZE(D3DXVECTOR3, m_vSale, Scaling);
	SYNTHESIZE_PASS_BY_REF(D3DXVECTOR3, m_vRotation, Rotation);

	SYNTHESIZE(D3DXMATRIXA16*, m_pmatParent, Parent);
	D3DXVECTOR3					m_vPosition;
	ST_SPHERE					m_stBoundingSphere;

public:
	cSkinnedMesh(char* szFolder, char* szFilename);
	~cSkinnedMesh(void);

	void UpdateAndRender();
	void SetAnimationIndex(int nIndex);

	void SetRandomTrackPosition();
	void SetTransform(D3DXMATRIXA16* pmat)
	{
		m_matWorldTM = *pmat;
	}
	D3DXMATRIXA16* GetTransFrom() { return &m_matWorldTM; }
	void ResetAndSetAnimationIndex(int nIndex);
	void SetPosition(D3DXVECTOR3 v)
	{
		m_vPosition = v;
		m_stBoundingSphere.vCenter = v;
	}
	ST_SPHERE* GetBoundingSphere()
	{
		return &m_stBoundingSphere;
	}
	D3DXMATRIXA16* getMatrix(char * name);
	ST_BONE* getBone(char * name, ST_BONE * pBone = NULL);

private:
	cSkinnedMesh();

	void Load(char* szFolder, char* szFilename);
	LPD3DXEFFECT LoadEffect(char* szFilename);
	void Update(ST_BONE* pCurrent, D3DXMATRIXA16* pmatParent);
	void Render(ST_BONE* pBone = NULL);
	void SetupBoneMatrixPtrs(ST_BONE* pBone);
	void Destroy();

};

