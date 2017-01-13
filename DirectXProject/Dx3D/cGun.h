#pragma once

class cSkinnedMesh;

class cGun
{
	cSkinnedMesh*				m_pGun;
	D3DXVECTOR3*				m_pvTarget;

	float						m_fAttackPower;		// ���� ����
	float						m_fAttackRange;		// ��Ÿ�
	float						m_fAttackSpeed;		// ����(����) �ӵ�
	
	int							m_nMaxAmmo;			// �Ѿ� �ִ�
	int							m_nCurrentAmmo;		// ���� �Ѿ� ��

	int n;
public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void Setting(D3DXVECTOR3& camAngle);
	void Fire(D3DXVECTOR3& vDirection, D3DXVECTOR3& vPosition);
};

