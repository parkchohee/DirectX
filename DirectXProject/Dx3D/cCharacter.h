#pragma once

class cGun;
class cController;
class cOBB;

class cHeightMap;
class cTextMap;

class cCharacter : public cGameObject
{
protected:
	cGun*					m_pGun;
	cController*			m_pController;

	LPD3DXMESH				m_pBoundingSphereMesh;
	ST_SPHERE				m_stBoundingSphere;

	D3DXMATRIXA16			m_matWorldTM;

	// ������, �츮������ Ȯ���ϴ� bool�� ����
	SYNTHESIZE(bool, m_isEnemy, IsEnemy);

	// HP
	SYNTHESIZE(float, m_fMaxHp, MaxHp);
	SYNTHESIZE(float, m_fCurrentHp, CurrentHp);

public:
	cCharacter(void);
	virtual ~cCharacter(void);

	virtual void Setup(char* szFolder, char* szFilename);
	virtual void Update();
	virtual void Render();
	
	virtual cGun* GetGun();
	virtual bool IsAttacked(float power);
};

