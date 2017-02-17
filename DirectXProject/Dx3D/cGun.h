#pragma once

class cSkinnedMesh;
class cBullet;
class cEffect;

class cGun : public cObject
{
	cSkinnedMesh*				m_pGun;				// Gun의 메쉬
	D3DXVECTOR3*				m_pvTarget;			// Gun을 가지는 object의 위치

	std::vector<cBullet*>		m_pvBullet;			// 총알 여러개

	//float						m_fAttackPower;		// 공격 세기
	//float						m_fAttackRange;		// 사거리
	//float						m_fAttackSpeed;		// 공격(연사) 속도
	
	//int							m_nMaxBullet;			// 총알 최대
	//int							m_nCurrentBullet;		// 현재 남은 총알 수

	D3DXMATRIXA16				m_pWorldTM;
	D3DXMATRIXA16				m_pTrans;
	D3DXMATRIXA16				m_pTransInv;

	std::vector<cEffect*>		m_pvEffect;

	SYNTHESIZE(float, m_fAttackPower, AttackPower);		// 공격 세기
	SYNTHESIZE(float, m_fAttackRange, AttackRange);		// 사거리
	SYNTHESIZE(float, m_fAttackSpeed, AttackSpeed);		// 공격(연사) 속도

	SYNTHESIZE(int, m_nMaxBullet, MaxBullet);			// 총알 최대
	SYNTHESIZE(int, m_nCurrentBullet, CurrentBullet);	// 현재 남은 총알 수

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
	
	void Fire(D3DXVECTOR3& vDirection, D3DXMATRIXA16& vPosition);
	void Reload();
	void RemoveBullet(int bulletIndex);

	cSkinnedMesh* GetGunMesh();
};

