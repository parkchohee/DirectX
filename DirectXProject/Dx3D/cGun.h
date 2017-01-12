#pragma once

class cSkinnedMesh;

class cGun
{
	cSkinnedMesh*				m_pGun;
	D3DXVECTOR3*				m_pvTarget;

	float						m_fAttackPower;		// 공격 세기
	float						m_fAttackRange;		// 사거리
	float						m_fAttackSpeed;		// 공격(연사) 속도
	
	int							m_nMaxAmmo;			// 총알 최대
	int							m_nCurrentAmmo;		// 현재 총알 수

public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update();
	void Render();

	void Setting();
};

