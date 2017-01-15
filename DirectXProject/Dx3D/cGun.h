#pragma once

class cSkinnedMesh;
class cBullet;

class cGun
{
	cSkinnedMesh*				m_pGun;				// Gun�� �޽�
	D3DXVECTOR3*				m_pvTarget;			// Gun�� ������ object
	cBullet*					m_pBullet;			// �Ѿ� ��ü
	
	
	std::vector<cBullet*>		m_pvBullet;			// �Ѿ� ������
	

	float						m_fAttackPower;		// ���� ����
	float						m_fAttackRange;		// ��Ÿ�
	float						m_fAttackSpeed;		// ����(����) �ӵ�
	
	int							m_nMaxBullet;			// �Ѿ� �ִ�
	int							m_nCurrentBullet;		// ���� ���� �Ѿ� ��

public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void Setting(D3DXVECTOR3& camAngle);
	void Fire(D3DXVECTOR3& vDirection, D3DXVECTOR3& vPosition);
};

