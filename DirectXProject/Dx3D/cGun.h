#pragma once

class cSkinnedMesh;
class cBullet;

class cGun
{
	cSkinnedMesh*				m_pGun;
	D3DXVECTOR3*				m_pvTarget;
	cBullet*					m_pBullet;

	float						m_fAttackPower;		// 공격 세기
	float						m_fAttackRange;		// 사거리
	float						m_fAttackSpeed;		// 공격(연사) 속도
	
	int							m_nMaxBullet;			// 총알 최대
	int							m_nCurrentBullet;		// 현재 총알 수

public:
	cGun();
	~cGun();

	void Setup(D3DXVECTOR3* pvTarget, char* szFolder, char* szFilename);
	void Update(D3DXVECTOR3& camAngle);
	void Render();

	void Setting(D3DXVECTOR3& camAngle);
	void Fire(D3DXVECTOR3& vDirection, D3DXVECTOR3& vPosition);
};

