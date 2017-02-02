#pragma once
class cStaticMesh;
class cOBB
{
private:
	D3DXVECTOR3 m_vOrgCenterPos;		
	D3DXVECTOR3 m_vOrgAxisDir[3];		
	
	D3DXVECTOR3 m_vCenterPos;		/// :  �ڽ� �߽�r ��ǥ
	D3DXVECTOR3 m_vAxisDir[3];		/// :  �ڽ��� ������ �� ���� ��������
	float		m_fAxisLen[3];		/// :  �ڽ��� ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	float		m_fAxisHalfLen[3];


	D3DXVECTOR3 m_vOrgAxisLenVec;
	D3DXVECTOR3 m_vAxisLenVec;

	D3DXMATRIXA16 m_matWorldTM;
public:
	cOBB(void);
	~cOBB(void);

	void Setup(cSkinnedMesh* pSkinnedMesh);
	void Setup(cStaticMesh* pStaticMesh);
	void Setup(D3DXVECTOR3 vMin, D3DXVECTOR3 vMax);
	void Update(D3DXMATRIXA16* pmatWorld);
	static bool IsCollision(cOBB* pOBB1, cOBB* pOBB2);

	void OBBBox_Render(D3DCOLOR c); 

	D3DXMATRIXA16* GetWorldTM() { return &m_matWorldTM; }

	void SetCenter(D3DXMATRIXA16& matSRT);
	void SetScale(D3DXMATRIXA16& matScale);
};

