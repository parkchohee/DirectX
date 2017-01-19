#pragma once

class cSkinnedMesh;
class cBullet;

class cGun : public cObject
{
	cSkinnedMesh*				m_pGun;				// Gun�� �޽�
	D3DXVECTOR3*				m_pvTarget;			// Gun�� ������ object�� ��ġ

	std::vector<cBullet*>		m_pvBullet;			// �Ѿ� ������

	float						m_fAttackPower;		// ���� ����
	float						m_fAttackRange;		// ��Ÿ�
	float						m_fAttackSpeed;		// ����(����) �ӵ�
	
	int							m_nMaxBullet;			// �Ѿ� �ִ�
	int							m_nCurrentBullet;		// ���� ���� �Ѿ� ��

	D3DXMATRIXA16				m_pWorldTM;

public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update();
	void Render();

	std::vector<cBullet*> GetBullets() { return m_pvBullet; }
	void SetParentWorldMatrix(D3DXMATRIXA16* matWorld);
	void SetWorldMatrix(D3DXMATRIXA16* matWorld);
	void SetWorldMatrixByBoneName(D3DXMATRIXA16* matRot, char* name);
	
	void Fire(D3DXVECTOR3& vDirection, D3DXVECTOR3& vPosition);
};

