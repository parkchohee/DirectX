#pragma once
class cOBB
{
private:
	D3DXVECTOR3 m_vOrgCenterPos;		
	D3DXVECTOR3 m_vOrgAxisDir[3];		
	
	D3DXVECTOR3 m_vCenterPos;		/// :  �ڽ� �߽�r ��ǥ
	D3DXVECTOR3 m_vAxisDir[3];		/// :  �ڽ��� ������ �� ���� ��������
	float		m_fAxisLen[3];		/// :  �ڽ��� ������ �� ���� ���� fAxisLen[n]�� vAxisDir[n]�� ���� �����Ѵ�.
	float		m_fAxisHalfLen[3];

	D3DXMATRIXA16 m_matWorldTM;
public:
	cOBB(void);
	~cOBB(void);

	void Setup(cSkinnedMesh* pSkinnedMesh);
	void Update(D3DXMATRIXA16* pmatWorld);
	static bool IsCollision(cOBB* pOBB1, cOBB* pOBB2);

	void OBBBox_Render(D3DCOLOR c); 

	void SetCenter(D3DXMATRIXA16& matSRT);
};

