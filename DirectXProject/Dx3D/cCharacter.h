#pragma once

class cGun;
class cController;
class cOBB;

class cCharacter : public cGameObject
{
protected:
	cGun*					m_pGun;
	cController*			m_pController;

	LPD3DXMESH				m_pBoundingSphereMesh;
	ST_SPHERE				m_stBoundingSphere;

	iMap*					m_pMap;

	D3DXMATRIXA16			m_matWorldTM;

	// 적인지, 우리팀인지 확인하는 bool형 변수
	SYNTHESIZE(bool, m_isEnemy, IsEnemy);

	// HP
	SYNTHESIZE(float, m_fMaxHp, MaxHp);
	SYNTHESIZE(float, m_fCurrentHp, CurrentHp);

public:
	cCharacter(void);
	virtual ~cCharacter(void);

	virtual void Setup(char* szFolder, char* szFilename);
	virtual void Update(iMap* pMap);
	virtual void Render();
	
	virtual bool IsAttacked(float power);
};

