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

public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update();
	void Render();

	void Setting();
};

