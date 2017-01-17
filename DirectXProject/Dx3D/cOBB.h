#pragma once
class cOBB
{
private:
	D3DXVECTOR3 m_vOrgCenterPos;		
	D3DXVECTOR3 m_vOrgAxisDir[3];		
	
	D3DXVECTOR3 m_vCenterPos;		/// :  박스 중심r 좌표
	D3DXVECTOR3 m_vAxisDir[3];		/// :  박스에 평행한 세 축의 단위벡터
	float		m_fAxisLen[3];		/// :  박스에 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.
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

